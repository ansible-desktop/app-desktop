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
// 🚨 AppVersion — ВНУТРЕННЕЕ целое tdesktop: по нему выбирается формат
// сериализации на диске и сравниваются версии при обновлении. Оно обязано
// оставаться >= 2008007, иначе readPeer() идёт по legacy-ветке (без flags
// и inlinePlaceholder), а writePeer() всегда пишет современный формат —
// свой аккаунт после перезапуска показывается ботом.
//
// Отображаемая версия — AppVersionStr, она отвязана. Схема: строке
// "0.<minor>.<patch>" отвечает целое 3000000 плюс minor*1000 плюс patch.
//
// 🚨 РУКАМИ ЗДЕСЬ НИЧЕГО НЕ ПРАВИТЬ. Версию ставит Ansible/build/set_version.py:
// он патчит СРАЗУ пять файлов — этот, build/version, оба winrc/*.rc и
// AppxManifest.xml, — и только так они не разъезжаются.
//
// 🚨 И ФОРМУЛУ ВЫШЕ НЕ ЗАПИСЫВАТЬ ВИДОМ «AppVersion = <число>»: патчер ищет
// такую запись регуляркой построчно и переписал бы ЧИСЛО В КОММЕНТАРИИ,
// молча превратив документацию в ложь. Один раз уже переписал.
constexpr auto AppVersion = 3002000;
constexpr auto AppVersionStr = "0.2.0";
constexpr auto AppBetaVersion = false;
constexpr auto AppAlphaVersion = TDESKTOP_ALPHA_VERSION;
