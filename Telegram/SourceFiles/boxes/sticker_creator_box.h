/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "data/stickers/data_stickers.h"

#include <QtGui/QImage>

namespace ChatHelpers {
class Show;
} // namespace ChatHelpers

namespace Ui {
class GenericBox;
} // namespace Ui

namespace Api {

void CreateStickerBox(
	not_null<Ui::GenericBox*> box,
	std::shared_ptr<ChatHelpers::Show> show,
	StickerSetIdentifier set,
	QImage image,
	Fn<void(MTPmessages_StickerSet)> done);

void OpenCreateStickerFlow(
	std::shared_ptr<ChatHelpers::Show> show,
	StickerSetIdentifier set,
	Fn<void(MTPmessages_StickerSet)> done = nullptr);

} // namespace Api
