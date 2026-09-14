/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace UserpicBuilder {

struct Result {
	QImage&& image;
	DocumentId id = 0;
	std::vector<QColor> colors;
};

[[nodiscard]] QImage GenerateGradient(
	const QSize &size,
	const std::vector<QColor> &colors,
	bool circle = true,
	bool roundForumRect = false);

struct StartData {
	DocumentId documentId = DocumentId(0);
	int builderColorIndex = 0;
	rpl::producer<std::vector<DocumentId>> documents;
	std::vector<QColor> gradientEditorColors;
	bool isForum = false;
};

template <typename Result>
struct BothWayCommunication {
	rpl::producer<> triggers;
	Fn<void(Result)> result;
};

} // namespace UserpicBuilder
