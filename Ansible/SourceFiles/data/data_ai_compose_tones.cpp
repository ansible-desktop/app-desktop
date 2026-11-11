/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#include "data/data_ai_compose_tones.h"

#include "apiwrap.h"
#include "api/api_text_entities.h"
#include "data/data_session.h"
#include "main/main_session.h"

namespace Data {

AiComposeTones::AiComposeTones(not_null<Main::Session*> session)
: _session(session)
, _refreshTimer([=] { refresh(); }) {
	// No background catalog requests for retired AI tools.
}

void AiComposeTones::refresh() {
	_list.clear();
	_hash = 0;
}

void AiComposeTones::refreshWithHash(uint64 hash) {
	_list.clear();
	_hash = 0;
}

void AiComposeTones::parseTones(const QVector<MTPAiComposeTone> &list) {
	_list.clear();
	_list.reserve(list.size());
	for (const auto &tone : list) {
		_list.push_back(parseTone(tone));
	}
	reapplyRecentCustomToneOrder();
}

AiComposeTone AiComposeTones::parseTone(
		const MTPAiComposeTone &tone) const {
	return tone.match([&](const MTPDaiComposeTone &data) {
		auto result = AiComposeTone{
			.id = data.vid().v,
			.accessHash = data.vaccess_hash().v,
			.slug = qs(data.vslug()),
			.title = qs(data.vtitle()),
			.emojiId = data.vemoji_id().value_or_empty(),
			.prompt = qs(data.vprompt().value_or_empty()),
			.installsCount = data.vinstalls_count().value_or_empty(),
			.authorId = data.vauthor_id()
				? UserId(data.vauthor_id()->v)
				: UserId(0),
			.creator = data.is_creator(),
		};
		if (const auto example = data.vexample_english()) {
			example->match([&](const MTPDaiComposeToneExample &d) {
				result.firstExample = AiComposeToneExample{
					.from = Api::ParseTextWithEntities(_session, d.vfrom()),
					.to = Api::ParseTextWithEntities(_session, d.vto()),
				};
			});
		}
		return result;
	}, [&](const MTPDaiComposeToneDefault &data) {
		return AiComposeTone{
			.title = qs(data.vtitle()),
			.emojiId = data.vemoji_id().v,
			.isDefault = true,
			.defaultType = qs(data.vtone()),
		};
	});
}

void AiComposeTones::create(
		const QString &title,
		const QString &prompt,
		DocumentId emojiId,
		bool displayAuthor,
		Fn<void(AiComposeTone)> done,
		Fn<void(const MTP::Error &)> fail) {
	if (fail) {
		crl::on_main(_session, [fail = std::move(fail)] {
			fail(MTP::Error::Local("AI_DISABLED", "Built-in AI is disabled."));
		});
	}
}

void AiComposeTones::update(
		const AiComposeTone &tone,
		std::optional<QString> title,
		std::optional<QString> prompt,
		std::optional<DocumentId> emojiId,
		std::optional<bool> displayAuthor,
		Fn<void(AiComposeTone)> done,
		Fn<void(const MTP::Error &)> fail) {
	if (fail) {
		crl::on_main(_session, [fail = std::move(fail)] {
			fail(MTP::Error::Local("AI_DISABLED", "Built-in AI is disabled."));
		});
	}
}

void AiComposeTones::save(
		const AiComposeTone &tone,
		bool unsave,
		Fn<void()> done,
		Fn<void(const MTP::Error &)> fail) {
	if (fail) {
		crl::on_main(_session, [fail = std::move(fail)] {
			fail(MTP::Error::Local("AI_DISABLED", "Built-in AI is disabled."));
		});
	}
}

void AiComposeTones::remove(
		const AiComposeTone &tone,
		Fn<void()> done) {
	_list.clear();
	_hash = 0;
}

void AiComposeTones::resolve(
		const QString &slug,
		Fn<void(AiComposeTone)> done,
		Fn<void(const MTP::Error &)> fail) {
	if (fail) {
		crl::on_main(_session, [fail = std::move(fail)] {
			fail(MTP::Error::Local("AI_DISABLED", "Built-in AI is disabled."));
		});
	}
}

void AiComposeTones::getToneExample(
		const AiComposeTone &tone,
		int num,
		Fn<void(AiComposeToneExample)> done,
		Fn<void(const MTP::Error &)> fail) {
	if (fail) {
		crl::on_main(_session, [fail = std::move(fail)] {
			fail(MTP::Error::Local("AI_DISABLED", "Built-in AI is disabled."));
		});
	}
}

void AiComposeTones::applyUpdate() {
	_list.clear();
	_hash = 0;
}

void AiComposeTones::finishRefresh() {
	const auto pending = _pendingRefresh;
	_pendingRefresh = PendingRefresh::None;
	if (pending == PendingRefresh::None) {
		return;
	}
	refreshWithHash((pending == PendingRefresh::Full) ? 0 : _hash);
}

void AiComposeTones::promoteCustomTone(AiComposeTone tone) {
	if (tone.isDefault) {
		return;
	}
	removeCustomTone(tone.id);
	rememberRecentCustomTone(tone.id);
	_list.insert(begin(_list), std::move(tone));
}

void AiComposeTones::removeCustomTone(uint64 id) {
	const auto i = ranges::find(_list, id, &AiComposeTone::id);
	if (i != end(_list)) {
		_list.erase(i);
	}
}

void AiComposeTones::rememberRecentCustomTone(uint64 id) {
	const auto i = ranges::find(_recentCustomToneIds, id);
	if (i != end(_recentCustomToneIds)) {
		_recentCustomToneIds.erase(i);
	}
	_recentCustomToneIds.insert(begin(_recentCustomToneIds), id);
}

void AiComposeTones::forgetRecentCustomTone(uint64 id) {
	const auto i = ranges::find(_recentCustomToneIds, id);
	if (i != end(_recentCustomToneIds)) {
		_recentCustomToneIds.erase(i);
	}
}

void AiComposeTones::reapplyRecentCustomToneOrder() {
	auto reordered = std::vector<AiComposeTone>();
	reordered.reserve(_list.size());

	auto recent = std::vector<uint64>();
	recent.reserve(_recentCustomToneIds.size());
	for (const auto id : _recentCustomToneIds) {
		const auto i = ranges::find(_list, id, &AiComposeTone::id);
		if (i != end(_list) && !i->isDefault) {
			reordered.push_back(*i);
			recent.push_back(id);
		}
	}
	for (const auto &tone : _list) {
		const auto promoted = !tone.isDefault
			&& (ranges::find(recent, tone.id) != end(recent));
		if (!promoted) {
			reordered.push_back(tone);
		}
	}
	_recentCustomToneIds = std::move(recent);
	_list = std::move(reordered);
}

MTPInputAiComposeTone AiComposeTones::toneToMTP(
		const AiComposeTone &tone) const {
	return tone.isDefault
		? MTP_inputAiComposeToneDefault(MTP_string(tone.defaultType))
		: MTP_inputAiComposeToneID(
			MTP_long(tone.id),
			MTP_long(tone.accessHash));
}

const std::vector<AiComposeTone> &AiComposeTones::list() const {
	return _list;
}

rpl::producer<> AiComposeTones::updated() const {
	return _updates.events();
}

} // namespace Data
