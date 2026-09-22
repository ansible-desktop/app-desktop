/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "base/object_ptr.h"
#include "storage/storage_shared_media.h"

namespace Ui {
class RpWidget;
} // namespace Ui

namespace Info::Profile {

[[nodiscard]] object_ptr<Ui::RpWidget> CreateTabSkeleton(
	not_null<QWidget*> parent,
	Storage::SharedMediaType type);

} // namespace Info::Profile
