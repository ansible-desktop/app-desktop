/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

class ClickHandler;

namespace Main {
class SessionShow;
} // namespace Main

namespace Api {

[[nodiscard]] std::shared_ptr<ClickHandler> AcceptClickHandler(
	not_null<HistoryItem*> item);
[[nodiscard]] std::shared_ptr<ClickHandler> DeclineClickHandler(
	not_null<HistoryItem*> item);
[[nodiscard]] std::shared_ptr<ClickHandler> SuggestChangesClickHandler(
	not_null<HistoryItem*> item);

void AddOfferToMessage(
	std::shared_ptr<Main::SessionShow> show,
	FullMsgId itemId);

} // namespace Api
