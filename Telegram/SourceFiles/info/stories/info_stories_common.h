/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Info::Stories {

[[nodiscard]] int ArchiveId();

struct Tag {
	explicit Tag(
		not_null<PeerData*> peer,
		int albumId = 0,
		int addingToAlbumId = 0)
	: peer(peer)
	, albumId(albumId)
	, addingToAlbumId(addingToAlbumId) {
	}

	not_null<PeerData*> peer;
	int albumId = 0;
	int addingToAlbumId = 0;
};

} // namespace Info::Stories
