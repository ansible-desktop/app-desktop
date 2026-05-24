/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "ui/rp_widget.h"

namespace Ui {

class VerticalLayout;

[[nodiscard]] QRect BubbleWrapInnerRect(const QRect &r);

not_null<Ui::RpWidget*> AddBubbleWrap(
	not_null<Ui::VerticalLayout*> container,
	const QSize &size);

} // namespace Ui
