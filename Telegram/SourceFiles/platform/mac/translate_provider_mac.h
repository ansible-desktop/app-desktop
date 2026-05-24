// This file is part of Ansible Desktop, a fork of Telegram Desktop,
// the official desktop application for the Telegram messaging service.
//
// For license and copyright information please follow this link:
// https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL
//
#pragma once

#include "translate_provider.h"

namespace Platform {

[[nodiscard]] std::unique_ptr<Ui::TranslateProvider>
CreateTranslateProvider();

[[nodiscard]] bool IsTranslateProviderAvailable();

} // namespace Platform
