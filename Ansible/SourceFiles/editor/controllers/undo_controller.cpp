/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#include "editor/controllers/undo_controller.h"

namespace Editor {
namespace {
using EnableRequest = UndoController::EnableRequest;
} // namespace

UndoController::UndoController() {
}

void UndoController::setCanPerformChanges(
		rpl::producer<EnableRequest> &&command) {
	std::move(
		command
	) | rpl::start_to_stream(_enable, _lifetime);
}

void UndoController::setPerformRequestChanges(rpl::producer<Undo> &&command) {
	std::move(
		command
	) | rpl::start_to_stream(_perform, _lifetime);
}

rpl::producer<EnableRequest> UndoController::canPerformChanges() const {
	return _enable.events();
}

rpl::producer<Undo> UndoController::performRequestChanges() const {
	return _perform.events();
}

} // namespace Editor
