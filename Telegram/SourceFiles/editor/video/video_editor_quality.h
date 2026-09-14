/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Editor {

struct VideoQualityLevel {
	// Target shorter side, zero means the original resolution is kept.
	int shorterSide = 0;
	QSize resolution;

	friend inline bool operator==(
		const VideoQualityLevel &,
		const VideoQualityLevel &) = default;
};

[[nodiscard]] std::vector<VideoQualityLevel> VideoQualityLevels(QSize edited);

[[nodiscard]] QString VideoQualityLabel(const VideoQualityLevel &level);

} // namespace Editor
