/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Main {
class Session;
} // namespace Main

namespace AppConfig {

[[nodiscard]] std::optional<QString> FragmentLink(not_null<Main::Session*>);

} // namespace AppConfig
