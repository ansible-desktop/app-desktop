/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

class History;

namespace Support {

// Returns histories().request, not api().request.
[[nodiscard]] int SendPreloadRequest(
	not_null<History*> history,
	Fn<void()> retry);

} // namespace Support
