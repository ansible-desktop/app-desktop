/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

class QKeyEvent;

namespace Ui {
class InputField;
} // namespace Ui

namespace Iv::Editor {

[[nodiscard]] bool HandleAutoPairKey(
	not_null<Ui::InputField*> field,
	not_null<QKeyEvent*> e);

} // namespace Iv::Editor
