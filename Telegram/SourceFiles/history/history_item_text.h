/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

class HistoryItem;

namespace Data {
struct Group;
} // namespace Data

TextForMimeData HistoryItemText(not_null<HistoryItem*> item);
TextForMimeData HistoryGroupText(not_null<const Data::Group*> group);
