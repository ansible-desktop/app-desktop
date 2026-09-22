/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "iv/markdown/iv_markdown_prepare_state.h"

namespace Iv::Markdown {

[[nodiscard]] int CountPreparedBlocks(const std::vector<PreparedBlock> &blocks);
[[nodiscard]] int FormulaSlotCount(const PreparedDocument &document);
void MeasurePreparedFormulas(PrepareState *state);
void MeasureNativeIvPreparedFormulas(NativeIvPrepareState *state);
void MeasureNativeIvPreparedFormulas(
	NativeIvPrepareState *state,
	int from,
	int till);

} // namespace Iv::Markdown
