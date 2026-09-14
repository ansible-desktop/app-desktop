/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "styles/style_iv.h"

class QTextDocument;

namespace Iv::Editor {

[[nodiscard]] style::Markdown CreateEditorMarkdownStyle();

[[nodiscard]] const style::margins &EditorBodyPadding();

[[nodiscard]] int MaxVisualLineWidth(not_null<const QTextDocument*> document);

[[nodiscard]] int MaxVisualLineWidthForWidth(
	not_null<const QTextDocument*> document,
	int width);

} // namespace Iv::Editor
