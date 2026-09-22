/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

enum class HistoryReactionSource : char;

class HistoryItem;

namespace Calls {
class GroupCall;
} // namespace Calls

namespace HistoryView {
class Element;
} // namespace HistoryView

namespace Main {
class SessionShow;
} // namespace Main

namespace Window {
class SessionController;
} // namespace Window

namespace Payments {

[[nodiscard]] bool LookupMyPaidAnonymous(not_null<HistoryItem*> item);

void TryAddingPaidReaction(
	not_null<HistoryItem*> item,
	HistoryView::Element *view,
	int count,
	std::optional<PeerId> shownPeer,
	std::shared_ptr<Main::SessionShow> show,
	Fn<void(bool)> finished = nullptr);

void TryAddingPaidReaction(
	not_null<Calls::GroupCall*> call,
	int count,
	std::shared_ptr<Main::SessionShow> show,
	Fn<void(bool)> finished = nullptr);

void ShowPaidReactionDetails(
	not_null<Window::SessionController*> controller,
	not_null<HistoryItem*> item,
	HistoryView::Element *view,
	HistoryReactionSource source);

} // namespace Payments
