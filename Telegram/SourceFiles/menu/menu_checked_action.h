/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "base/basic_types.h"
#include "ui/style/style_core_types.h"

class QAction;

namespace Ui {
class PopupMenu;
} // namespace Ui

namespace Menu {

not_null<QAction*> AddCheckedAction(
	not_null<Ui::PopupMenu*> menu,
	const QString &text,
	Fn<void()> callback,
	const style::icon *icon,
	bool checked);

} // namespace Menu
