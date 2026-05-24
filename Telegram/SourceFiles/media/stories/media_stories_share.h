/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "base/object_ptr.h"

namespace ChatHelpers {
class Show;
} // namespace ChatHelpers

namespace Ui {
class BoxContent;
} // namespace Ui

namespace Media::Stories {

[[nodiscard]] object_ptr<Ui::BoxContent> PrepareShareBox(
	std::shared_ptr<ChatHelpers::Show> show,
	FullStoryId id,
	bool viewerStyle = false);

[[nodiscard]] QString FormatShareAtTime(TimeId seconds);

[[nodiscard]] object_ptr<Ui::BoxContent> PrepareShareAtTimeBox(
	std::shared_ptr<ChatHelpers::Show> show,
	not_null<HistoryItem*> item,
	TimeId videoTimestamp);

} // namespace Media::Stories
