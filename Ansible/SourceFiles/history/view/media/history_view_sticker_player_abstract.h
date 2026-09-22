/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace HistoryView {

class StickerPlayer {
public:
	virtual ~StickerPlayer() = default;

	struct FrameInfo {
		QImage image;
		int index = 0;
	};
	virtual void setRepaintCallback(Fn<void()> callback) = 0;
	[[nodiscard]] virtual bool ready() = 0;
	[[nodiscard]] virtual int framesCount() = 0;
	[[nodiscard]] virtual FrameInfo frame(
		QSize size,
		QColor colored,
		bool mirrorHorizontal,
		crl::time now,
		bool paused) = 0;
	virtual bool markFrameShown() = 0;

};

} // namespace HistoryView
