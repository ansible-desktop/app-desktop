/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/

#pragma once

#include <QtCore/QByteArray>

#include <functional>
#include <memory>

namespace Platform::WebAuthn::Cable {

class BleScanner {
public:
	virtual ~BleScanner() = default;

	[[nodiscard]] virtual bool start(
		std::function<void(QByteArray)> onAdvert,
		std::function<void(bool)> onAvailability) = 0;
	virtual void stop() = 0;
};

[[nodiscard]] std::unique_ptr<BleScanner> MakeBleScanner();

} // namespace Platform::WebAuthn::Cable
