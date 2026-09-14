/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "ui/basic_click_handlers.h"

namespace Main {
class Session;
} // namespace Main

class BankCardClickHandler : public ClickHandler {
public:
	BankCardClickHandler(not_null<Main::Session*> session, QString text);

	void onClick(ClickContext context) const override;

	TextEntity getTextEntity() const override;

	QString tooltip() const override;

private:
	const not_null<Main::Session*> _session;
	QString _text;

};
