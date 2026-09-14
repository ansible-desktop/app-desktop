/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "info/profile/tabs/info_profile_tab_content.h"
#include "storage/storage_shared_media.h"

namespace Info::Profile {

extern const char kOptionProfileMediaTabsExpanded[];

[[nodiscard]] bool MediaTabsExpanded();
[[nodiscard]] rpl::producer<bool> MediaTabsExpandedValue();
void SetMediaTabsExpanded(bool expanded);

[[nodiscard]] MediaTabDescriptor MakeMediaTabDescriptor(
	Storage::SharedMediaType type,
	rpl::producer<bool> shown);

} // namespace Info::Profile
