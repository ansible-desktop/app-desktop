/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "intro/intro_step.h"

namespace Ui {
class FlatLabel;
class LinkButton;
class RoundButton;
} // namespace Ui

namespace Intro {
namespace details {

class StartWidget : public Step {
public:
	StartWidget(
		QWidget *parent,
		not_null<Main::Account*> account,
		not_null<Data*> data);

	void submit() override;
	rpl::producer<QString> nextButtonText() const override;
	rpl::producer<> nextButtonFocusRequests() const override;
	void activate() override;
	void setInnerFocus() override;

private:
	rpl::event_stream<> _nextButtonFocusRequests;

};

} // namespace details
} // namespace Intro
