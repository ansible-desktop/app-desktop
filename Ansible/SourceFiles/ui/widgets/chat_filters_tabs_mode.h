/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {

// Values are serialized in Core::Settings, don't change them.
enum class ChatsFiltersTabsMode {
	Default = 0,
	TextAndIcons = 1,
	IconsOnly = 2,
	TextOnly = 3,
};

[[nodiscard]] inline ChatsFiltersTabsMode HorizontalChatsFiltersTabsMode(
		ChatsFiltersTabsMode mode) {
	return (mode == ChatsFiltersTabsMode::Default)
		? ChatsFiltersTabsMode::TextOnly
		: mode;
}

[[nodiscard]] inline ChatsFiltersTabsMode VerticalChatsFiltersTabsMode(
		ChatsFiltersTabsMode mode) {
	return (mode == ChatsFiltersTabsMode::Default)
		? ChatsFiltersTabsMode::TextAndIcons
		: mode;
}

} // namespace Ui
