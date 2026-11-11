/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#include "api/api_compose_with_ai.h"

#include "api/api_text_entities.h"
#include "apiwrap.h"
#include "base/options.h"
#include "core/shortcuts.h"
#include "data/data_ai_compose_tones.h"
#include "data/data_session.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "ui/layers/show.h"
#include "ui/widgets/fields/input_field.h"

namespace Api {

ComposeWithAi::ComposeWithAi(not_null<ApiWrap*> api)
: _session(&api->session())
, _api(&api->instance()) {
}

MTPInputAiComposeTone ComposeWithAi::SerializeTone(
		const std::optional<ToneRef> &tone) {
	if (!tone) {
		return MTPInputAiComposeTone();
	} else if (tone->id) {
		return MTP_inputAiComposeToneID(
			MTP_long(tone->id),
			MTP_long(tone->accessHash));
	} else if (!tone->customPrompt.isEmpty()) {
		return MTP_inputAiComposeToneSingleUse(
			MTP_string(tone->customPrompt));
	}
	return MTP_inputAiComposeToneDefault(MTP_string(tone->defaultTone));
}

mtpRequestId ComposeWithAi::request(
		Request request,
		Fn<void(Result &&)> done,
		Fn<void(const MTP::Error &)> fail) {
	if (fail) {
		crl::on_main(_session, [fail = std::move(fail)] {
			fail(MTP::Error::Local("AI_DISABLED", "Built-in AI is disabled."));
		});
	}
	return 0;
}

void ComposeWithAi::cancel(mtpRequestId requestId) {
	if (requestId) {
		_api.request(requestId).cancel();
	}
}

ComposeWithAi::Diff ComposeWithAi::ParseDiff(
		not_null<Main::Session*> session,
		const MTPTextWithEntities &text) {
	const auto &data = text.data();
	auto result = Diff{
		.text = ParseTextWithEntities(session, text),
	};
	const auto &entities = data.ventities().v;
	result.entities.reserve(entities.size());
	for (const auto &entity : entities) {
		entity.match([&](const MTPDmessageEntityDiffInsert &data) {
			result.entities.push_back({
				.type = DiffEntity::Type::Insert,
				.offset = data.voffset().v,
				.length = data.vlength().v,
			});
		}, [&](const MTPDmessageEntityDiffReplace &data) {
			result.entities.push_back({
				.type = DiffEntity::Type::Replace,
				.offset = data.voffset().v,
				.length = data.vlength().v,
				.oldText = qs(data.vold_text()),
			});
		}, [&](const MTPDmessageEntityDiffDelete &data) {
			result.entities.push_back({
				.type = DiffEntity::Type::Delete,
				.offset = data.voffset().v,
				.length = data.vlength().v,
			});
		}, [](const auto &) {
		});
	}
	return result;
}

void ApplyAiInPlaceBySlug(
		not_null<Main::Session*> session,
		TextWithEntities text,
		QString slug,
		Fn<void(TextWithEntities)> done,
		Fn<void(const MTP::Error &)> fail) {
	if (fail) {
		crl::on_main(session, [fail = std::move(fail)] {
			fail(MTP::Error::Local("AI_DISABLED", "Built-in AI is disabled."));
		});
	}
}

QString AiApplyBoundSlug() {
	return {};
}

void SetAiApplyBoundSlug(const QString &) {
}

void ClearAiApplyBoundSlug() {
}

QString AiApplyShortcutText() {
	return {};
}

void TriggerAiApplyInPlace(
		not_null<Main::Session*> session,
		std::shared_ptr<Ui::Show> show,
		not_null<QObject*> guard,
		not_null<Ui::InputField*> field,
		TextWithEntities fullFieldText,
		Fn<void(TextWithTags textWithTags, int cursor)> applyToField) {
	// Built-in AI actions are unavailable in this product.
}

} // namespace Api
