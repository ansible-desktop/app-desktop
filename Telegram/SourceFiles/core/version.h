/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "base/const_string.h"

#define TDESKTOP_REQUESTED_ALPHA_VERSION (0ULL)

#ifdef TDESKTOP_ALLOW_CLOSED_ALPHA
#define TDESKTOP_ALPHA_VERSION TDESKTOP_REQUESTED_ALPHA_VERSION
#else // TDESKTOP_ALLOW_CLOSED_ALPHA
#define TDESKTOP_ALPHA_VERSION (0ULL)
#endif // TDESKTOP_ALLOW_CLOSED_ALPHA

// used in Updater.cpp and Setup.iss for Windows
constexpr auto AppId = "{5DE84367-0BFF-4829-8074-A3C10556F2FB}"_cs;
constexpr auto AppNameOld = "Ansible Desktop"_cs;
constexpr auto AppName = "Ansible Desktop"_cs;
constexpr auto AppFile = "Ansible"_cs;
// AppVersion is the INTERNAL integer used by tdesktop for on-disk serialization
// format gates and auto-update comparison. It MUST stay >= 2008007 — below that,
// readPeer() takes legacy paths (skips flags/inlinePlaceholder) while writePeer()
// always writes the modern format, corrupting the deserialized self user
// (botInfoVersion reads the saved lastseen value -> self wrongly shown as a bot).
// The fork's previous value 1002 fell into tdesktop's pre-0.9.x legacy range.
//
// Display version is AppVersionStr below (decoupled). Versioning scheme:
//   displayed "0.<minor>.<patch>"  <->  AppVersion = 3000000 + minor*1000 + patch
// Bump BOTH on each release, and keep the bh-updates manifest version in sync.
constexpr auto AppVersion = 3001003;     // displayed as 0.1.3 (see AppVersionStr)
constexpr auto AppVersionStr = "0.1.3";
constexpr auto AppBetaVersion = false;
constexpr auto AppAlphaVersion = TDESKTOP_ALPHA_VERSION;
