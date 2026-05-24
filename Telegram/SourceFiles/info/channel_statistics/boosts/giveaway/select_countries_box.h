/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {

class GenericBox;

void SelectCountriesBox(
	not_null<Ui::GenericBox*> box,
	const std::vector<QString> &selected,
	Fn<void(std::vector<QString>)> doneCallback,
	Fn<bool(int)> checkErrorCallback);

} // namespace Ui
