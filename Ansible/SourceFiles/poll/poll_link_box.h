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

namespace Poll {

void AddPollOptionLinkBox(
	not_null<Ui::GenericBox*> box,
	const QString &initial,
	Fn<void(QString)> callback);

} // namespace Poll
