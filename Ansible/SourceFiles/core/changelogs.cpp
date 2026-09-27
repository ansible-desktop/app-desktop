/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#include "core/changelogs.h"

#include "lang/lang_keys.h"
#include "core/application.h"
#include "core/update_channel.h"
#include "core/version.h"
#include "main/main_domain.h"
#include "main/main_session.h"
#include "storage/storage_domain.h"
#include "data/data_session.h"
#include "base/qt/qt_common_adapters.h"
#include "mainwindow.h"
#include "apiwrap.h"

namespace Core {
namespace {

std::map<int, const char*> BetaLogs() {
	return {
	{
		4008011,
		"- Fix initial video playback speed.\n"

		"- Use native window resize on Windows 11.\n"

		"- Fix memory leak in Direct3D 11 media viewer on Windows.\n"
	},
	{
		4010004,
		"- Statistics in channels and group chats.\n"

		"- Nice looking code blocks with syntax highlight.\n"

		"- Copy full code block by click on its header.\n"

		"- Send a highlighted code block using ```language syntax.\n"
	}
	};
};

} // namespace

Changelogs::Changelogs(not_null<Main::Session*> session, int oldVersion)
: _session(session)
, _oldVersion(oldVersion) {
	_session->data().chatsListChanges(
	) | rpl::filter([](Data::Folder *folder) {
		return !folder;
	}) | rpl::on_next([=] {
		requestCloudLogs();
	}, _chatsSubscription);
}

std::unique_ptr<Changelogs> Changelogs::Create(
		not_null<Main::Session*> session) {
	if (BuildIsCanary) {
		// Canary changelogs live in the canary channels themselves.
		return nullptr;
	}
	auto &local = Core::App().domain().local();
	const auto oldVersion = local.oldVersion();
	local.clearOldVersion();
	return (oldVersion > 0 && oldVersion < AppVersion)
		? std::make_unique<Changelogs>(session, oldVersion)
		: nullptr;
}

void Changelogs::requestCloudLogs() {
	_chatsSubscription.destroy();

	const auto callback = [this](const MTPUpdates &result) {
		_session->api().applyUpdates(result);

		auto resultEmpty = true;
		switch (result.type()) {
		case mtpc_updateShortMessage:
		case mtpc_updateShortChatMessage:
		case mtpc_updateShort:
			resultEmpty = false;
			break;
		case mtpc_updatesCombined:
			resultEmpty = result.c_updatesCombined().vupdates().v.isEmpty();
			break;
		case mtpc_updates:
			resultEmpty = result.c_updates().vupdates().v.isEmpty();
			break;
		case mtpc_updatesTooLong:
		case mtpc_updateShortSentMessage:
			LOG(("API Error: Bad updates type in app changelog."));
			break;
		}
		if (resultEmpty) {
			addLocalLogs();
		}
	};
	_session->api().requestChangelog(
		FormatVersionPrecise(_oldVersion),
		crl::guard(this, callback));
}

void Changelogs::addLocalLogs() {
	if (AppBetaVersion || cAlphaVersion()) {
		addBetaLogs();
	}
	if (!_addedSomeLocal) {
		const auto text = tr::lng_new_version_wrap(
			tr::now,
			lt_version,
			QString::fromLatin1(AppVersionStr),
			lt_changes,
			tr::lng_new_version_minor(tr::now),
			lt_link,
			Core::App().changelogLink());
		addLocalLog(text.trimmed());
	}
}

void Changelogs::addLocalLog(const QString &text) {
	auto textWithEntities = TextWithEntities{ text };
	TextUtilities::ParseEntities(textWithEntities, TextParseLinks);
	_session->data().serviceNotification(textWithEntities);
	_addedSomeLocal = true;
};

void Changelogs::addBetaLogs() {
	for (const auto &[version, changes] : BetaLogs()) {
		addBetaLog(version, changes);
	}
}

void Changelogs::addBetaLog(int changeVersion, const char *changes) {
	if (_oldVersion >= changeVersion) {
		return;
	}
	const auto text = [&] {
		static const auto simple = u"\n- "_q;
		static const auto separator = '\n' + Ui::kQBullet + ' ';
		auto result = QString::fromUtf8(changes).trimmed();
		if (result.startsWith(base::StringViewMid(simple, 1))) {
			result = separator.mid(1) + result.mid(simple.size() - 1);
		}
		return result.replace(simple, separator);
	}();
	const auto version = FormatVersionDisplay(changeVersion);
	const auto log = u"New in version %1 beta:\n\n"_q.arg(version) + text;
	addLocalLog(log);
}

// 🚨 Обратная сторона схемы из core/version.h: строке "0.<minor>.<patch>"
// отвечает целое 3000000 плюс minor*1000 плюс patch. Апстримовский разбор
// (major = version / 1000000) читает наши номера как 3.x.y — 3003001 у него
// «3.3.1» вместо «0.3.1». Одним местом это не ограничивается: тем же
// FormatVersionDisplay подписывается версия в tdata/version распакованного
// обновления, а Updater.exe кладёт эту строку в DisplayVersion реестра.
//
// Держим минимум 2008007 (см. version.h), так что верхний диапазон 3xxxxxx
// в форке занят только нами, и разобрать его по своей схеме безопасно.
namespace {

[[nodiscard]] int ForkVersionMajor(int version) {
	return (version >= 3000000 && version < 4000000) ? 0 : (version / 1000000);
}

[[nodiscard]] int ForkVersionMinor(int version) {
	return (version % 1000000) / 1000;
}

} // namespace

QString FormatVersionDisplay(int version) {
	return QString::number(ForkVersionMajor(version))
		+ '.' + QString::number(ForkVersionMinor(version))
		+ ((version % 1000)
			? ('.' + QString::number(version % 1000))
			: QString());
}

QString FormatVersionPrecise(int version) {
	return QString::number(ForkVersionMajor(version))
		+ '.' + QString::number(ForkVersionMinor(version))
		+ '.' + QString::number(version % 1000);
}

} // namespace Core
