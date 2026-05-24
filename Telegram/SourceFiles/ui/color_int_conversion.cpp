/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#include "ui/color_int_conversion.h"

namespace Ui {

QColor ColorFromSerialized(quint32 serialized) {
	return QColor(
		int((serialized >> 16) & 0xFFU),
		int((serialized >> 8) & 0xFFU),
		int(serialized & 0xFFU));
}

std::optional<QColor> MaybeColorFromSerialized(quint32 serialized) {
	return (serialized == quint32(-1))
		? std::nullopt
		: std::make_optional(ColorFromSerialized(serialized));
}

QColor Color32FromSerialized(quint32 serialized) {
	return QColor(
		int(serialized & 0xFFU),
		int((serialized >> 8) & 0xFFU),
		int((serialized >> 16) & 0xFFU),
		int((serialized >> 24) & 0xFFU));
}

} // namespace Ui
