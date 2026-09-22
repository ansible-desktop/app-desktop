/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {
class RpWidget;
class VerticalLayout;
} // namespace Ui

namespace Window {
class SessionController;
} // namespace Window

namespace UserpicBuilder {

struct StartData;
struct Result;

template <typename Result>
struct BothWayCommunication;

not_null<Ui::VerticalLayout*> CreateUserpicBuilder(
	not_null<Ui::RpWidget*> parent,
	not_null<Window::SessionController*> controller,
	StartData data,
	BothWayCommunication<UserpicBuilder::Result> communication);

[[nodiscard]] not_null<Ui::RpWidget*> CreateEmojiUserpic(
	not_null<Ui::RpWidget*> parent,
	const QSize &size,
	rpl::producer<not_null<DocumentData*>> document,
	rpl::producer<int> colorIndex,
	bool isForum);

} // namespace UserpicBuilder
