/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "base/object_ptr.h"

namespace Ui {

class RpWidget;

[[nodiscard]] object_ptr<RpWidget> CreateOutdatedBar(
	not_null<QWidget*> parent,
	const QString &workingPath);

} // namespace Ui
