/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

class UserData;

namespace Ui {
class RpWidget;
} // namespace Ui

namespace Info::Profile {

void StartProfileBirthdayEffect(
	not_null<Ui::RpWidget*> cover,
	not_null<UserData*> user,
	Fn<QRect()> userpicGeometry,
	Fn<bool()> paused);

} // namespace Info::Profile
