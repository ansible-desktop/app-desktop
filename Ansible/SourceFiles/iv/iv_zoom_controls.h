/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "base/basic_types.h"
#include "base/unique_qptr.h"

namespace Ui::Menu {
class ItemBase;
class Menu;
} // namespace Ui::Menu

namespace Iv {

class Delegate;

[[nodiscard]] base::unique_qptr<Ui::Menu::ItemBase> CreateZoomMenuAction(
	not_null<Ui::Menu::Menu*> menu,
	not_null<Delegate*> delegate);

} // namespace Iv
