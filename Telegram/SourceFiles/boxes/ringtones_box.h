/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "ui/layers/generic_box.h"

namespace Data {
struct NotifySound;
class Thread;
enum class DefaultNotify : uint8_t;
struct VolumeController;
} // namespace Data

namespace Main {
class Session;
} // namespace Main

namespace Ui {
class GenericBox;
} // namespace Ui

[[nodiscard]] QString ExtractRingtoneName(not_null<DocumentData*> document);

void RingtonesBox(
	not_null<Ui::GenericBox*> box,
	not_null<Main::Session*> session,
	Data::NotifySound selected,
	Fn<void(Data::NotifySound)> save,
	Data::VolumeController volumeController);

void ThreadRingtonesBox(
	not_null<Ui::GenericBox*> box,
	not_null<Data::Thread*> thread);
