/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#include "tray_accounts_menu.h"

namespace Core::TrayAccountsMenu {

void SetupChangesSubscription(
		[[maybe_unused]] Fn<void()> callback,
		[[maybe_unused]] rpl::lifetime &lifetime) {
}

void Fill([[maybe_unused]] Platform::Tray &tray) {
}

} // namespace Core::TrayAccountsMenu
