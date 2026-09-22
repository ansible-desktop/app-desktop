/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {
struct MarkdownEnabledState;
} // namespace Ui

namespace Platform {

void CreateGlobalMenu();
void DestroyGlobalMenu();
void RequestUpdateGlobalMenu();

[[nodiscard]] rpl::producer<Ui::MarkdownEnabledState> GlobalMenuMarkdownState();

} // namespace Platform
