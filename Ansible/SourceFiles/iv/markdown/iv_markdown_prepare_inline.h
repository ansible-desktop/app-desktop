/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "iv/markdown/iv_markdown_prepare_state.h"

namespace Iv::Markdown {

void PrepareInlineRichText(
	const MarkdownNode &node,
	int textSize,
	int renderWidthCap,
	int renderHeightCap,
	QString *blockAnchorId,
	TextWithEntities *text,
	std::vector<PreparedLink> *links,
	PrepareState *state);

} // namespace Iv::Markdown
