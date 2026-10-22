/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {

enum class BubbleCornerRounding : uchar;

[[nodiscard]] int BubbleRadiusSmall();
[[nodiscard]] int BubbleRadiusLarge();
[[nodiscard]] int BubbleCornerRadius(BubbleCornerRounding corner);

[[nodiscard]] int MsgFileThumbRadiusSmall();
[[nodiscard]] int MsgFileThumbRadiusLarge();

extern const char kOptionUseSmallMsgBubbleRadius[];

} // namespace Ui
