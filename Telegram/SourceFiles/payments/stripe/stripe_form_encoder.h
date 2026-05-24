/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "stripe/stripe_form_encodable.h"

namespace Stripe {

class FormEncoder {
public:
	[[nodiscard]] static QByteArray formEncodedDataForObject(
		FormEncodable &&object);

};

} // namespace Stripe
