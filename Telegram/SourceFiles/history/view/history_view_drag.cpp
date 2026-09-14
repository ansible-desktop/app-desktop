/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#include "history/view/history_view_drag.h"

#include "data/data_photo.h"
#include "data/data_photo_media.h"

#include <QtCore/QMimeData>

namespace HistoryView {

PhotoDragData PreparePhotoDragData(not_null<PhotoData*> photo) {
	if (photo->isNull()) {
		return {};
	}
	const auto media = photo->activeMediaView();
	constexpr auto kSize = Data::PhotoSize::Large;
	const auto animated = media
		&& !media->videoContent(kSize).isEmpty();
	if (!media || !media->loaded() || animated) {
		return {};
	}
	auto result = PhotoDragData();
	if (const auto image = media->image(kSize)) {
		result.image = image->original();
	}
	result.bytes = media->imageBytes(kSize);
	return result;
}

void FillDragMimeWithPhoto(not_null<QMimeData*> mime, PhotoDragData &&data) {
	if (data.image.isNull()) {
		return;
	}
	mime->setImageData(std::move(data.image));
	if (!data.bytes.isEmpty()) {
		mime->setData(u"image/jpeg"_q, std::move(data.bytes));
		mime->setData(u"application/x-td-use-jpeg"_q, "1");
	}
}

} // namespace HistoryView
