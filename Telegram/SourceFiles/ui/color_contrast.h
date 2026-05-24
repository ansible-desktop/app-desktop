/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

class QColor;

namespace Ui {

[[nodiscard]] float64 CountContrast(const QColor &a, const QColor &b);

} // namespace Ui
