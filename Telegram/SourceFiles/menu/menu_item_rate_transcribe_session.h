/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

class HistoryItem;

namespace Menu {

[[nodiscard]] Fn<void(bool)> RateTranscribeCallbackFactory(
	not_null<HistoryItem*>);

[[nodiscard]] bool HasRateTranscribeItem(not_null<HistoryItem*>);

} // namespace Menu
