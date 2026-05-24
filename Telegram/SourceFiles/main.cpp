/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#include "core/launcher.h"

int main(int argc, char *argv[]) {
	const auto launcher = Core::Launcher::Create(argc, argv);
	return launcher ? launcher->exec() : 1;
}
