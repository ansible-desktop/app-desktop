/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "settings/settings_common_session.h"

namespace Settings {

class Experimental : public Section<Experimental> {
public:
	Experimental(
		QWidget *parent,
		not_null<Window::SessionController*> controller);

	[[nodiscard]] rpl::producer<QString> title() override;
	void fillTopBarMenu(const Ui::Menu::MenuCallback &addAction) override;

private:
	void setupContent();

	rpl::event_stream<> _reloadOptionsRequests;

};

} // namespace Settings
