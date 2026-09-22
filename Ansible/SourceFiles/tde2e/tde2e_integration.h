/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace TdE2E {

struct UserId;
struct PublicKey;

[[nodiscard]] UserId MakeUserId(not_null<UserData*> user);
[[nodiscard]] UserId MakeUserId(::UserId id);

[[nodiscard]] MTPint256 PublicKeyToMTP(const PublicKey &key);

} // namespace TdE2E
