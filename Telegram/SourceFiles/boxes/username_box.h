/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {
class GenericBox;
class VerticalLayout;
} // namespace Ui

class PeerData;

void UsernamesBox(
	not_null<Ui::GenericBox*> box,
	not_null<PeerData*> peer);

void UsernamesBoxWithCallback(
	not_null<Ui::GenericBox*> box,
	not_null<PeerData*> peer,
	Fn<void()> onSaved);

struct UsernameCheckInfo final {
	[[nodiscard]] static UsernameCheckInfo PurchaseAvailable(
		const QString &username,
		not_null<PeerData*> peer);

	enum class Type {
		Good,
		Error,
		Default,
	};
	Type type;
	TextWithEntities text;
};

void AddUsernameCheckLabel(
	not_null<Ui::VerticalLayout*> container,
	rpl::producer<UsernameCheckInfo> checkInfo);
