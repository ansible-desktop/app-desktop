/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

class PeerData;

namespace Info::Saved {

struct MusicTag {
	explicit MusicTag(not_null<PeerData*> peer)
	: peer(peer) {
	}

	not_null<PeerData*> peer;
};

} // namespace Info::Saved
