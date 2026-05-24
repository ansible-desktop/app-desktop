/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui::Controls {

struct SwipeContextData final {
	[[nodiscard]] bool empty() const {
		return !ratio
			&& !reachRatio
			&& !translation
			&& !cursorTop;
	}
	[[nodiscard]] explicit operator bool() const {
		return !empty();
	}

	float64 ratio = 0.;
	float64 reachRatio = 0.;
	int64 msgBareId = 0;
	int translation = 0;
	int cursorTop = 0;
};

struct SwipeBackResult final {
	rpl::lifetime lifetime;
	Fn<void(SwipeContextData)> callback;
};

} // namespace Ui::Controls
