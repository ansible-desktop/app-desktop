/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Data {
struct SponsoredFrom;
} // namespace Data

namespace Main {
class Session;
} // namespace Main

namespace Ui {

class RpWidget;

void FillSponsoredMessageBar(
	not_null<RpWidget*> container,
	not_null<Main::Session*> session,
	FullMsgId fullId,
	Data::SponsoredFrom from,
	const TextWithEntities &textWithEntities);

} // namespace Ui
