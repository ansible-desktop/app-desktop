/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#include "layout/layout_item_base.h"

#include "history/view/history_view_cursor_state.h"

[[nodiscard]] HistoryView::TextState LayoutItemBase::getState(
		QPoint point,
		StateRequest request) const {
	return {};
}

[[nodiscard]] TextSelection LayoutItemBase::adjustSelection(
		TextSelection selection,
		TextSelectType type) const {
	return selection;
}
