/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace ChatHelpers {
enum class PauseReason;
} // namespace ChatHelpers

namespace Main {
class Session;
} // namespace Main

namespace Ui {
class RpWidget;
} // namespace Ui

namespace Window {
class SessionController;
} // namespace Window

namespace Ui {

not_null<Ui::RpWidget*> AddChatFiltersTabsStrip(
	not_null<Ui::RpWidget*> parent,
	not_null<Main::Session*> session,
	Fn<void(FilterId)> choose,
	ChatHelpers::PauseReason pauseLevel,
	Window::SessionController *controller = nullptr,
	bool trackActiveFilterAndUnreadAndReorder = false);

} // namespace Ui
