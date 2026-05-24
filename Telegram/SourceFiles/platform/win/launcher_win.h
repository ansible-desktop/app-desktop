/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include "core/launcher.h"

namespace Platform {

class Launcher : public Core::Launcher {
public:
	Launcher(int argc, char *argv[]);

private:
	std::optional<QStringList> readArgumentsHook(
		int argc,
		char *argv[]) const override;

	bool launchUpdater(UpdaterLaunch action) override;

	bool launch(
		const QString &operation,
		const QString &binaryPath,
		const QStringList &argumentsList);

};

} // namespace Platform
