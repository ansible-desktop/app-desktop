/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "ui/dynamic_image.h"

namespace Poll {

[[nodiscard]] std::shared_ptr<Ui::DynamicImage> MakeLinkThumbnail();

} // namespace Poll
