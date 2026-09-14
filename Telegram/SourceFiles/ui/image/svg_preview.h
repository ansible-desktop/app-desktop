/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QSize>
#include <QtGui/QImage>

namespace Ui {

[[nodiscard]] int SvgPreviewBytesLimit();
[[nodiscard]] QImage RenderSvgPreview(
	const QByteArray &bytes,
	QSize maxSize);

} // namespace Ui
