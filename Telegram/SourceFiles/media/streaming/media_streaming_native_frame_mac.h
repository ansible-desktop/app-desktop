/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#ifdef Q_OS_MAC

#include <QtGui/QImage>

namespace Media::Streaming {

struct NativeFrame;

[[nodiscard]] QImage ConvertNativeFrameToARGB32(const NativeFrame &frame);

} // namespace Media::Streaming

#endif // Q_OS_MAC
