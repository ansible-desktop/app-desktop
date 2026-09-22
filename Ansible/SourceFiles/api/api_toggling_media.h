/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace ChatHelpers {
class Show;
} // namespace ChatHelpers

namespace Api {

void ToggleFavedSticker(
	std::shared_ptr<ChatHelpers::Show> show,
	not_null<DocumentData*> document,
	Data::FileOrigin origin);

void ToggleFavedSticker(
	std::shared_ptr<ChatHelpers::Show> show,
	not_null<DocumentData*> document,
	Data::FileOrigin origin,
	bool faved);

void ToggleRecentSticker(
	not_null<DocumentData*> document,
	Data::FileOrigin origin,
	bool saved);

void ToggleSavedGif(
	std::shared_ptr<ChatHelpers::Show> show,
	not_null<DocumentData*> document,
	Data::FileOrigin origin,
	bool saved);

void ToggleSavedRingtone(
	not_null<DocumentData*> document,
	Data::FileOrigin origin,
	Fn<void()> &&done,
	bool saved);

} // namespace Api
