/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

class ChannelData;

namespace Window {
class SessionNavigation;
} // namespace Window

void ShowCommunityPendingRequestsBox(
	not_null<Window::SessionNavigation*> navigation,
	not_null<ChannelData*> community);
