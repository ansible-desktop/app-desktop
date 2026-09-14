/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Window {
class Controller;
} // namespace Window

namespace Ui {

void ShowPhoneBannedError(
	not_null<Window::Controller*> controller,
	const QString &phone);

} // namespace Ui
