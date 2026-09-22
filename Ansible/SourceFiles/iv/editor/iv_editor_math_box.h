/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "base/basic_types.h"

#include <QtCore/QString>

#include <optional>

namespace Ui {
class GenericBox;
} // namespace Ui

namespace Iv::Editor {

void EditMathBox(
	not_null<Ui::GenericBox*> box,
	QString startSource,
	bool editingExisting,
	std::optional<bool> separateLine,
	Fn<void(QString, bool)> callback,
	Fn<void(bool)> setExternalInteractionActive,
	Fn<void()> restoreFocus);

} // namespace Iv::Editor
