/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "ui/widgets/menu/menu_action.h"

namespace Ui {
class CheckView;
} // namespace Ui

namespace Menu {

class ItemWithCheck final : public Ui::Menu::Action {
public:
	using Ui::Menu::Action::Action;

	void init(bool checked);

	not_null<Ui::CheckView*> checkView() const;

private:
	std::unique_ptr<Ui::CheckView> _checkView;
};

} // namespace Menu
