/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {

class RpWidget;
class ScrollArea;

void SetupShadowsToScrollContent(
	not_null<Ui::RpWidget*> parent,
	not_null<Ui::ScrollArea*> scroll,
	rpl::producer<int> &&innerHeightValue);

} // namespace Ui
