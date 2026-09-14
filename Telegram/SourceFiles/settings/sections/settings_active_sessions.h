/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "settings/settings_type.h"

namespace Ui {
class VerticalLayout;
} // namespace Ui

namespace Window {
class SessionController;
} // namespace Window

namespace Settings {

[[nodiscard]] Type SessionsId();

void AddSessionInfoRow(
	not_null<Ui::VerticalLayout*> container,
	rpl::producer<QString> label,
	const QString &value,
	const style::icon &icon);

} // namespace Settings
