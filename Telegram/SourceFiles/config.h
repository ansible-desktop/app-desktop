/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "settings.h"

enum {
	MaxSelectedItems = 100,

	LocalEncryptIterCount = 4000, // key derivation iteration count
	LocalEncryptNoPwdIterCount = 4, // key derivation iteration count without pwd (not secure anyway)
	LocalEncryptSaltSize = 32, // 256 bit

	AutoSearchTimeout = 900, // 0.9 secs

	PreloadHeightsCount = 3, // when 3 screens to scroll left make a preload request

	SearchPeopleLimit = 5,

	WebPageUserId = 701000,

	UpdateDelayConstPart = 8 * 3600, // 8 hour min time between update check requests
	UpdateDelayRandPart = 8 * 3600, // 8 hour max - min time between update check requests

	WrongPasscodeTimeout = 1500,

	ChoosePeerByDragTimeout = 1000, // 1 second mouse not moved to choose dialog when dragging a file
};

inline const char *cGUIDStr() {
#ifndef OS_MAC_STORE
	static const char *gGuidStr = "{87A94AB0-E370-4cde-98D3-ACC110C5967D}";
#else // OS_MAC_STORE
	static const char *gGuidStr = "{E51FB841-8C0B-4EF9-9E9E-5A0078567627}";
#endif // OS_MAC_STORE

	return gGuidStr;
}

// Ключ ПРОВЕРКИ ПОДПИСИ ОБНОВЛЕНИЙ (RSA-1024, PKCS#1).
//
// 🚨 Не путать с ключом РУКОПОЖАТИЯ с DC — тот RSA-2048 и лежит в
// mtproto/mtproto_dc_options.cpp. Два разных кастомных RSA-ключа в одном
// проекте, оба наши, и перепутать их легко.
//
// Приватная половина вне дерева: updates_keys/updates_private.pem и
// Vault secret/ansible/desktop/updates_private.
// SHA256(public PEM) =
//   f94337c1d2bfb4093c8653f186a18756822b933c8a40dd15a7f505b985331d29
//
// Stable и beta пока один и тот же ключ — разнести можно позже, если
// понадобятся разные каденции выпуска.
//
// 🚨 _other/packer.cpp до сих пор держит АПСТРИМОВСКИЕ ключи Телеграма:
// пакер и клиент разойдутся подписью, пока это не исправлено.
static const char *UpdatesPublicKey = "\
-----BEGIN RSA PUBLIC KEY-----\n\
MIGJAoGBAMLHY3bK+69SEDUqu3gM9JNqVxH+jvezWLFw0SYAdIoFOd7qC8KFWdBO\n\
pF3oMULzag4n91f6b1w9gjRkjBsCgmgK/zdXyrd3DA0pJg4A3mRtYMpDRvE6LJ8N\n\
V50dC2K65CO+8rXSZpbF4egU6Yz+PpPNt9T8eOpsnEOplqEO+u3JAgMBAAE=\n\
-----END RSA PUBLIC KEY-----\n\
";
static const char *UpdatesPublicBetaKey = "\
-----BEGIN RSA PUBLIC KEY-----\n\
MIGJAoGBAMLHY3bK+69SEDUqu3gM9JNqVxH+jvezWLFw0SYAdIoFOd7qC8KFWdBO\n\
pF3oMULzag4n91f6b1w9gjRkjBsCgmgK/zdXyrd3DA0pJg4A3mRtYMpDRvE6LJ8N\n\
V50dC2K65CO+8rXSZpbF4egU6Yz+PpPNt9T8eOpsnEOplqEO+u3JAgMBAAE=\n\
-----END RSA PUBLIC KEY-----\n\
";

#if defined TDESKTOP_API_ID && defined TDESKTOP_API_HASH

constexpr auto ApiId = TDESKTOP_API_ID;
constexpr auto ApiHash = QT_STRINGIFY(TDESKTOP_API_HASH);

#else // TDESKTOP_API_ID && TDESKTOP_API_HASH

// To build your version of Telegram Desktop you're required to provide
// your own 'api_id' and 'api_hash' for the Telegram API access.
//
// How to obtain your 'api_id' and 'api_hash' is described here:
// https://my.ansible.su
//
// If you're building the application not for deployment,
// but only for test purposes you can comment out the error below.
//
// This will allow you to use TEST ONLY 'api_id' and 'api_hash' which are
// very limited by the Telegram API server.
//
// Your users will start getting internal server errors on login
// if you deploy an app using those 'api_id' and 'api_hash'.

#error You are required to provide API_ID and API_HASH.

constexpr auto ApiId = 17349;
constexpr auto ApiHash = "344583e45741c457fe1862106095a5eb";

#endif // TDESKTOP_API_ID && TDESKTOP_API_HASH

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
#error "Only little endian is supported!"
#endif // Q_BYTE_ORDER == Q_BIG_ENDIAN

#if (TDESKTOP_ALPHA_VERSION != 0)

// Private key for downloading closed alphas.
#include "../../../DesktopPrivate/alpha_private.h"

#else
static const char *AlphaPrivateKey = "";
#endif

extern QString gKeyFile;
inline const QString &cDataFile() {
	if (!gKeyFile.isEmpty()) return gKeyFile;
	static const QString res(u"data"_q);
	return res;
}

inline const QRegularExpression &cRussianLetters() {
	static QRegularExpression regexp(QString::fromUtf8("[а-яА-ЯёЁ]"));
	return regexp;
}
