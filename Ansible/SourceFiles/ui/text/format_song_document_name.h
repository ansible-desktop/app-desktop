/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "ui/text/format_song_name.h"

class DocumentData;

namespace Ui::Text {

[[nodiscard]] FormatSongName FormatSongNameFor(
	not_null<DocumentData*> document);

[[nodiscard]] TextWithEntities FormatDownloadsName(
	not_null<DocumentData*> document);

[[nodiscard]] FormatSongName FormatVoiceName(
	not_null<DocumentData*> document,
	FullMsgId contextId);

} // namespace Ui::Text
