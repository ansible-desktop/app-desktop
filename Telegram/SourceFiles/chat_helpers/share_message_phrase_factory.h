/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

class PeerData;

namespace ChatHelpers {

struct ForwardedMessagePhraseArgs final {
	size_t toCount = 0;
	bool singleMessage = false;
	PeerData *to1 = nullptr;
	PeerData *to2 = nullptr;
	bool toSelfWithPremiumIsEmpty = true;
};

rpl::producer<TextWithEntities> ForwardedMessagePhrase(
	const ForwardedMessagePhraseArgs &args);

} // namespace ChatHelpers
