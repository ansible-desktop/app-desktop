/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {
class GenericBox;
class RpWidget;
} // namespace Ui

namespace Window {
class SessionController;
} // namespace Window

namespace Data {
struct Reaction;
} // namespace Data

void AddReactionAnimatedIcon(
	not_null<Ui::RpWidget*> parent,
	rpl::producer<QPoint> iconPositionValue,
	int iconSize,
	const Data::Reaction &reaction,
	rpl::producer<> &&selects,
	rpl::producer<> &&destroys,
	not_null<rpl::lifetime*> stateLifetime);
void AddReactionCustomIcon(
	not_null<Ui::RpWidget*> parent,
	rpl::producer<QPoint> iconPositionValue,
	int iconSize,
	not_null<Window::SessionController*> controller,
	DocumentId customId,
	rpl::producer<> &&destroys,
	not_null<rpl::lifetime*> stateLifetime);

void ReactionsSettingsBox(
	not_null<Ui::GenericBox*> box,
	not_null<Window::SessionController*> controller);
