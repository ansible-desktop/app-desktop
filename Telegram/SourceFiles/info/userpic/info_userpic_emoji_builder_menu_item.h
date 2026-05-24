/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Window {
class SessionController;
} // namespace Window

namespace Ui {
class PopupMenu;
} // namespace Ui

namespace UserpicBuilder {

struct Result;

void AddEmojiBuilderAction(
	not_null<Window::SessionController*> controller,
	not_null<Ui::PopupMenu*> menu,
	rpl::producer<std::vector<DocumentId>> documents,
	Fn<void(UserpicBuilder::Result)> &&done,
	bool isForum);

} // namespace UserpicBuilder
