/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace HistoryView {

struct WebPageText {
	QString title;
	QString description;
};

WebPageText TitleAndDescriptionFromWebPage(not_null<WebPageData*> d);
bool DrawWebPageDataPreview(
	QPainter &p,
	not_null<WebPageData*> webpage,
	not_null<PeerData*> context,
	QRect to);

} // namespace HistoryView
