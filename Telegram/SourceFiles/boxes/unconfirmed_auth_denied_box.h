/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {
class GenericBox;
} // namespace Ui

namespace Dialogs {

void ShowAuthDeniedBox(
	not_null<Ui::GenericBox*> box,
	float64 count,
	const QString &messageText);

} // namespace Dialogs
