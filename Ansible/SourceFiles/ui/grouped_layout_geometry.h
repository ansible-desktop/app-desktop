/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include <QtCore/QRect>

namespace Ui {

// LayoutMediaGroup without the per-item sides, so that targets which do
// not link lib_ui, like td_export, can use the same album geometry.
[[nodiscard]] std::vector<QRect> LayoutMediaGroupGeometry(
	const std::vector<QSize> &sizes,
	int maxWidth,
	int minWidth,
	int spacing);

} // namespace Ui
