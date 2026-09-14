/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

class WindowListDelegate;

namespace HistoryView {
class ListWidget;
} // namespace HistoryView

namespace Ui {
class ElasticScroll;
class RpWidget;
} // namespace Ui

namespace Window {

class SectionWidget;

void SetupSwipeBackSection(
	not_null<Ui::RpWidget*> parent,
	not_null<Ui::ElasticScroll*> scroll,
	not_null<HistoryView::ListWidget*> list);

} // namespace Window
