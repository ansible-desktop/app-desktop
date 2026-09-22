/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "iv/iv_rich_page.h"

class HistoryItem;

namespace Data {
struct Group;
} // namespace Data

struct HistorySelectedTextEntry {
	not_null<HistoryItem*> item;
	const Data::Group *group = nullptr;
};

TextForMimeData HistoryItemText(not_null<HistoryItem*> item);
[[nodiscard]] Iv::RichPageBlocksSlice HistoryItemRichBlocks(
	not_null<HistoryItem*> item);
TextForMimeData HistoryGroupText(not_null<const Data::Group*> group);
TextForMimeData HistorySelectedItemsText(
	const std::vector<HistorySelectedTextEntry> &entries,
	bool richContext);
