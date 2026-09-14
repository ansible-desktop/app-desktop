/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "info/profile/tabs/info_profile_tab_content.h"

class PeerData;

namespace Info::Profile {

[[nodiscard]] MediaTabDescriptor MakeSavedTabDescriptor(
	not_null<PeerData*> peer);

} // namespace Info::Profile
