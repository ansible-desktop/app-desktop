/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {
class RpWindow;
} // namespace Ui

namespace Iv {

class Delegate {
public:
	[[nodiscard]] virtual QRect ivGeometry(not_null<Ui::RpWindow*> window) const = 0;
	virtual void ivSaveGeometry(not_null<Ui::RpWindow*> window) = 0;

	[[nodiscard]] virtual int ivZoom() const = 0;
	[[nodiscard]] virtual rpl::producer<int> ivZoomValue() const = 0;
	virtual void ivSetZoom(int value) = 0;
};

} // namespace Iv
