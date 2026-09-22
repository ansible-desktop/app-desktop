/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "base/weak_ptr.h"

namespace Data {
struct AiComposeTone;
} // namespace Data

namespace Main {
class Session;
} // namespace Main

namespace Window {
class SessionController;
} // namespace Window

namespace Ui {
class GenericBox;
} // namespace Ui

void PreviewAiToneBox(
	not_null<Ui::GenericBox*> box,
	not_null<Main::Session*> session,
	Data::AiComposeTone tone,
	base::weak_ptr<Window::SessionController> controller);
