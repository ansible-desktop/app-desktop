/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include <QtCore/QJsonObject>
#include <vector>

namespace Stripe {

[[nodiscard]] bool ContainsFields(
	const QJsonObject &object,
	std::vector<QStringView> keys);

} // namespace Stripe
