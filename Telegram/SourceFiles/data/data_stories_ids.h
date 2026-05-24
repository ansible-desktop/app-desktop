/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "data/data_abstract_sparse_ids.h"

class PeerData;

namespace Main {
class Session;
} // namespace Main

namespace Data {

using StoriesIdsSlice = AbstractSparseIds<std::vector<StoryId>>;

[[nodiscard]] rpl::producer<StoriesIdsSlice> AlbumStoriesIds(
	not_null<PeerData*> peer,
	int albumId,
	StoryId aroundId,
	int limit);

} // namespace Data
