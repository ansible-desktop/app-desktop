/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "data/data_channel_earn.h"

namespace Info::ChannelEarn {

[[nodiscard]] QString MajorPart(Data::EarnInt value);
[[nodiscard]] QString MajorPart(CreditsAmount value);
[[nodiscard]] QString MinorPart(Data::EarnInt value);
[[nodiscard]] QString MinorPart(CreditsAmount value);
[[nodiscard]] QString ToUsd(
	Data::EarnInt value,
	float64 rate,
	int afterFloat);
[[nodiscard]] QString ToUsd(
	CreditsAmount value,
	float64 rate,
	int afterFloat);

} // namespace Info::ChannelEarn
