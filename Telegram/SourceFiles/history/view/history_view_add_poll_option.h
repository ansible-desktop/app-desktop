/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

struct PollData;

namespace Ui {
class ChatStyle;
} // namespace Ui

namespace Window {
class SessionController;
} // namespace Window

namespace HistoryView {

class Element;
class ElementOverlayHost;

void ShowAddPollOptionOverlay(
	ElementOverlayHost &host,
	not_null<QWidget*> parent,
	not_null<Element*> view,
	not_null<PollData*> poll,
	FullMsgId context,
	not_null<Window::SessionController*> controller,
	not_null<const Ui::ChatStyle*> st);

} // namespace HistoryView
