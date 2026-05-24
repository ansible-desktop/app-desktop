/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/

#include "platform/platform_webauthn.h"

namespace Platform::WebAuthn {

bool IsSupported() {
	return false;
}

void RegisterKey(
		const Data::Passkey::RegisterData &data,
		Fn<void(RegisterResult result)> callback) {
	callback({});
}

void Login(
		const Data::Passkey::LoginData &data,
		Fn<void(LoginResult result)> callback) {
	callback({});
}

} // namespace Platform::WebAuthn
