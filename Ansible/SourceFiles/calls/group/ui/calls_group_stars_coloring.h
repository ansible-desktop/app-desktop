/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace Ui {
class RpWidget;
} // namespace Ui

namespace Calls::Group::Ui {

using namespace ::Ui;

struct DiamondsColoring {
	int bgLight = 0;
	int bgDark = 0;
	int fromDiamonds = 0;
	TimeId secondsPin = 0;
	int charactersMax = 0;
	int emojiLimit = 0;

	friend inline auto operator<=>(
		const DiamondsColoring &,
		const DiamondsColoring &) = default;
	friend inline bool operator==(
		const DiamondsColoring &,
		const DiamondsColoring &) = default;
};

[[nodiscard]] DiamondsColoring DiamondsColoringForCount(
	const std::vector<DiamondsColoring> &colorings,
	int stars);

[[nodiscard]] int DiamondsRequiredForMessage(
	const std::vector<DiamondsColoring> &colorings,
	const TextWithTags &text);

[[nodiscard]] object_ptr<Ui::RpWidget> VideoStreamDiamondsLevel(
	not_null<Ui::RpWidget*> box,
	const std::vector<DiamondsColoring> &colorings,
	rpl::producer<int> diamondsValue);

} // namespace Calls::Group::Ui
