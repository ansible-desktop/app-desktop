# Ansible Desktop

Desktop client for the [Ansible][behappy] messaging service.

> **This project is a fork of [Telegram Desktop][tdesktop].** It is
> licensed under [GPL v3 with OpenSSL exception][license], the same
> terms as the upstream project. We are grateful to the Telegram Desktop
> Authors for their work — without it this fork would not exist.
>
> Ansible Desktop is **not affiliated with, endorsed by, or sponsored
> by Telegram FZ-LLC**. It connects to Ansible servers, not Telegram
> servers, and cannot be used to access Telegram accounts.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://github.com/ansible-desktop/app-desktop/blob/master/LICENSE)
[![Upstream](https://img.shields.io/badge/forked%20from-tdesktop-orange.svg)][tdesktop]

---

## What this is

Ansible Desktop is the official desktop client for the Ansible
messenger. It is built on top of the Telegram Desktop codebase
([tdesktop][tdesktop]) under GPL v3, with the following high-level
modifications:

- Networking layer rewritten to use the **MVSy 1.0** protocol and
  connect to Ansible backend servers (instead of MTProto 2.0 / Telegram
  DCs).
- Branding, visual identity, and product naming replaced throughout.
- Telegram-specific features removed where not applicable to Ansible
  (e.g., Telegram Premium subscriptions, Telegram Stars, Fragment
  integration, sponsored messages).
- Additional features added that are unique to Ansible.

The complete list of changes from upstream is tracked in
[`CHANGELOG.md`](CHANGELOG.md). The upstream changelog is preserved in
[`UPSTREAM_CHANGELOG.txt`](UPSTREAM_CHANGELOG.txt) for reference.

## Relationship to upstream

| | Telegram Desktop | Ansible Desktop |
|---|---|---|
| License | GPL v3 + OpenSSL exception | GPL v3 + OpenSSL exception (same) |
| Backend | Telegram DCs | Ansible servers (`mvsy.ansible.su`) |
| Protocol | MTProto 2.0 | MVSy 1.0 |
| Trademarks | Telegram, MTProto | Ansible |
| Account compatibility | Telegram accounts | Ansible accounts (separate system) |
| Source repository | [telegramdesktop/tdesktop][tdesktop] | [ansible-desktop/app-desktop](https://github.com/ansible-desktop/app-desktop) |

We do **not** merge updates from upstream automatically. The fork is
independently maintained.

## Downloads

The latest Ansible Desktop builds are available at:

- Windows / macOS / Linux: <https://ansible.su/desktop>
- Source release archives: <https://github.com/ansible-desktop/app-desktop/releases>

For Telegram Desktop (the upstream project) please visit
<https://desktop.telegram.org/>.

## Building from source

The build process is unchanged from upstream. Refer to:

- [docs/building-win.md](docs/building-win.md) — Windows
- [docs/building-mac.md](docs/building-mac.md) — macOS
- [docs/building-linux.md](docs/building-linux.md) — Linux

The build tooling (CMake, scripts) and third-party dependencies are
inherited from Telegram Desktop unchanged.

## Third-party

This project depends on the same third-party components as Telegram
Desktop. The full list is preserved here for transparency:

* Qt 6 ([LGPL](http://doc.qt.io/qt-6/lgpl.html))
* OpenSSL 3.2.1 ([Apache License 2.0](https://www.openssl.org/source/apache-license-2.0.txt))
* WebRTC ([New BSD License](https://github.com/desktop-app/tg_owt/blob/master/LICENSE))
* zlib ([zlib License](http://www.zlib.net/zlib_license.html))
* LZMA SDK 9.20 ([public domain](http://www.7-zip.org/sdk.html))
* liblzma ([public domain](http://tukaani.org/xz/))
* libopus codec ([BSD License](http://www.opus-codec.org/license/))
* FFmpeg ([LGPL](https://www.ffmpeg.org/legal.html))
* OpenAL Soft ([LGPL](https://kcat.strangesoft.net/openal.html))
* xxHash ([BSD License](https://github.com/Cyan4973/xxHash/blob/dev/LICENSE))
* Range-v3 ([Boost License](https://github.com/ericniebler/range-v3/blob/master/LICENSE.txt))

## License

Ansible Desktop is free software: you can redistribute it and/or modify
it under the terms of the **GNU General Public License v3** as published
by the Free Software Foundation, with the OpenSSL linking exception
inherited from upstream.

- Full license text: [LICENSE](LICENSE)
- Per-file copyright header: [LEGAL](LEGAL)
- Attribution and trademark notice: [NOTICE](NOTICE)

By contributing to this repository, you agree that your contributions
will be licensed under the same terms.

## Trademarks

"Telegram" and "MTProto" are trademarks of Telegram FZ-LLC. They are
used in this README and in source code copyright headers solely to
identify the upstream project from which this fork is derived, as
required by GPL §5(a). They are **not** used as trademarks of this
product.

"Ansible" is a trademark of the Ansible Desktop Authors.

## Contact

- General: <https://ansible.su>
- Source code questions: open an issue on this repository
- License compliance / DMCA: <legal@ansible.su>

[behappy]: https://ansible.su
[tdesktop]: https://github.com/telegramdesktop/tdesktop
[license]: https://github.com/ansible-desktop/app-desktop/blob/master/LICENSE
