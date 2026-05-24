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
class GenericBox;
} // namespace Ui

namespace Data {
class ChatFilter;
} // namespace Data

void EditFilterBox(
	not_null<Ui::GenericBox*> box,
	not_null<Window::SessionController*> window,
	const Data::ChatFilter &filter,
	Fn<void(const Data::ChatFilter &)> doneCallback,
	Fn<void(const Data::ChatFilter &, Fn<void(Data::ChatFilter)>)> saveAnd);

void EditExistingFilter(
	not_null<Window::SessionController*> window,
	FilterId id);
