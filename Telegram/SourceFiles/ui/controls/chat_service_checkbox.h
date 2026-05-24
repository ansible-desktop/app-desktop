/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "base/object_ptr.h"

namespace style {
struct Checkbox;
struct ServiceCheck;
} // namespace style

namespace Ui {

class Checkbox;

[[nodiscard]] object_ptr<Checkbox> MakeChatServiceCheckbox(
	QWidget *parent,
	const QString &text,
	const style::Checkbox &st,
	const style::ServiceCheck &stCheck,
	bool checked,
	Fn<QColor()> bg = nullptr);

} // namespace Ui
