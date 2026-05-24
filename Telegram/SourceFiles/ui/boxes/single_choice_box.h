/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "ui/layers/generic_box.h"
#include "base/required.h"

namespace style {
struct Checkbox;
struct Radio;
} // namespace style

struct SingleChoiceBoxArgs {
	template <typename T>
	using required = base::required<T>;

	required<rpl::producer<QString>> title;
	const std::vector<QString> &options;
	int initialSelection = 0;
	required<Fn<void(int)>> callback;
	const style::Checkbox *st = nullptr;
	const style::Radio *radioSt = nullptr;
};

void SingleChoiceBox(
	not_null<Ui::GenericBox*> box,
	SingleChoiceBoxArgs &&args);
