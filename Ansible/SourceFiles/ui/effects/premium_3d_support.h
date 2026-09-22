/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui::Premium {

[[nodiscard]] float64 CubicBezier(
	float64 x1,
	float64 y1,
	float64 x2,
	float64 y2,
	float64 x);

[[nodiscard]] bool Object3dSupported();

} // namespace Ui::Premium
