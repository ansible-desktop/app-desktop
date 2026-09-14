/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "ui/layers/show.h"

namespace Main {

class Session;

class SessionShow : public Ui::Show {
public:
	[[nodiscard]] virtual Main::Session &session() const = 0;

	bool showFrozenError();

};

[[nodiscard]] std::shared_ptr<SessionShow> MakeSessionShow(
	std::shared_ptr<Ui::Show> show,
	not_null<Session*> session);

} // namespace Main
