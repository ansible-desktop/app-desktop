/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "ui/rp_widget.h"

namespace Dialogs {

enum class SearchEmptyIcon {
	Search,
	NoResults,
};

class SearchEmpty final : public Ui::RpWidget {
public:
	using Icon = SearchEmptyIcon;

	SearchEmpty(
		QWidget *parent,
		Icon icon,
		rpl::producer<TextWithEntities> text);

	void setMinimalHeight(int minimalHeight);

	void animate();

	[[nodiscard]] rpl::producer<ClickHandlerPtr> handlerActivated() const {
		return _handlerActivated.events();
	}

private:
	void setup(Icon icon, rpl::producer<TextWithEntities> text);

	Fn<void()> _animate;
	rpl::event_stream<ClickHandlerPtr> _handlerActivated;

};

} // namespace Dialogs
