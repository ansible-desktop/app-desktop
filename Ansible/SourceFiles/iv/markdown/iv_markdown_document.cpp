/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#include "iv/markdown/iv_markdown_document.h"

#include <utility>

namespace Iv::Markdown {

PreparedDocument EmptyDocument(QString sourceName) {
	auto document = PreparedDocument();
	document.sourceName = std::move(sourceName);
	document.document.kind = NodeKind::Document;
	return document;
}

} // namespace Iv::Markdown
