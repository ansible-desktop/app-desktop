/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

namespace ChatHelpers {
class Show;
} // namespace ChatHelpers

namespace Data {
struct MessageReactionsTopPaid;
} // namespace Data

namespace Main {
class Session;
} // namespace Main

namespace Ui {
class BoxContent;
class GenericBox;
} // namespace Ui

namespace Calls::Group::Ui {
using namespace ::Ui;
struct DiamondsColoring;
} // namespace Calls::Group::Ui

namespace Calls::Group {

[[nodiscard]] int MaxVideoStreamDiamondsCount(not_null<Main::Session*> session);

struct VideoStreamDiamondsBoxArgs {
	std::shared_ptr<ChatHelpers::Show> show;
	std::vector<Data::MessageReactionsTopPaid> top;
	int min = 0;
	int current = 0;
	bool sending = false;
	bool admin = false;
	Fn<void(int)> save;
	QString name;
};

void VideoStreamDiamondsBox(
	not_null<Ui::GenericBox*> box,
	VideoStreamDiamondsBoxArgs &&args);

[[nodiscard]] object_ptr<Ui::BoxContent> MakeVideoStreamDiamondsBox(
	VideoStreamDiamondsBoxArgs &&args);

} // namespace Calls::Group
