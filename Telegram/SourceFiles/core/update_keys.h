/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#include <QtCore/QByteArray>

namespace Core::Updates {

// The pinned root Ed25519 public key and the built-in key manifest with
// its detached root signature, embedded at build time from the files in
// Telegram/Resources/update/ through a generated header.

[[nodiscard]] QByteArray RootPublicKeyPem();
[[nodiscard]] QByteArray EmbeddedManifest();
[[nodiscard]] QByteArray EmbeddedManifestSignature();

} // namespace Core::Updates
