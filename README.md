# Ansible Desktop

Desktop client for the [Ansible](https://ansible.su) messaging service —
Windows, macOS and Linux.

> **This project is a fork of [Telegram Desktop](https://github.com/telegramdesktop/tdesktop).**
> It is licensed under [GPL v3 with the OpenSSL exception](LICENSE), the
> same terms as the upstream project. We are grateful to the Telegram
> Desktop Authors for their work — without it this fork would not exist.
>
> Ansible Desktop is **not affiliated with, endorsed by, or sponsored by
> Telegram FZ-LLC**. It connects to Ansible servers, not Telegram
> servers, and cannot be used to access Telegram accounts.

[![License: GPLv3](https://img.shields.io/badge/License-GPLv3%20%2B%20OpenSSL-blue.svg)](LICENSE)
[![Upstream](https://img.shields.io/badge/forked%20from-tdesktop%207.2.8-orange.svg)](https://github.com/telegramdesktop/tdesktop)

---

## What this is

Ansible Desktop is the desktop client for the Ansible messenger, built on
the Telegram Desktop codebase under GPL v3. Current version **0.3.0**,
rebased onto upstream **7.2.8** (API layer 229).

What differs from upstream:

- **Servers.** The DC list, the RSA public key used for the handshake and
  the `api_id` / `api_hash` pair all point at Ansible infrastructure. The
  client cannot reach Telegram servers and cannot sign in to a Telegram
  account.
- **Branding and naming.** Product name, identifiers, installer ids, file
  associations and user-visible strings are ours.
- **Own namespace.** Links are `asme.su`, the deep-link scheme is `as://`,
  animated stickers are `.ass` (`x-ansible-stikers`). Telegram's `t.me`,
  `tg://` and `.tgs` are **not** recognised — deliberately, this is not a
  compatibility layer.
- **Translation keys are ours.** Key names carry no upstream product
  words: `telegram` → `ansible`, `stars` → `diamonds`, `credits` →
  `diamonds`. Translations themselves are served by the Ansible backend,
  not bundled beyond the built-in English.
- **Crash reports come to us.** The reporter is kept — it is a useful
  feature — but it posts to `api.ansible.su/crash` instead of upstream.

The upstream changelog is not carried in this repository; our own history
starts in [`changelog.txt`](changelog.txt).

## Relationship to upstream

| | Telegram Desktop | Ansible Desktop |
|---|---|---|
| License | GPL v3 + OpenSSL exception | same |
| Backend | Telegram DCs | Ansible servers |
| Accounts | Telegram accounts | Ansible accounts (separate system) |
| Links / scheme | `t.me`, `tg://` | `asme.su`, `as://` |
| Animated stickers | `.tgs` | `.ass` |
| Source repository | [telegramdesktop/tdesktop](https://github.com/telegramdesktop/tdesktop) | [ansible-desktop/app-desktop](https://github.com/ansible-desktop/app-desktop) |
| Versioning | upstream `X.Y.Z` | our `0.Y.Z`, see below |

Upstream changes are **not** merged automatically. Moving to a newer
upstream base is a deliberate, separate piece of work.

## Versioning

The displayed version is `0.<minor>.<patch>`. Internally tdesktop also
keeps an integer `AppVersion`, which selects the on-disk data format and
drives update comparisons — for a `0.x.y` release it is `3000000` plus
`minor * 1000` plus `patch`, so 0.3.0 is `3003000`.

🚨 **Never edit a version by hand.** It lives in five files at once
(`Ansible/build/version`, `Ansible/SourceFiles/core/version.h`, both
`Ansible/Resources/winrc/*.rc` and the UWP manifest). Use the script,
which patches all of them and refuses values that would make the app
misread its own stored data:

```
python Ansible/build/set_version.py 0.3.1
```

## Downloads and updating

Builds are published as GitHub Actions artifacts on this repository.
See [`docs/upgrading.md`](docs/upgrading.md) for how to move from an
earlier build and what is preserved.

🚨 **In-app auto-update does not work yet.** The client checks
`https://ansible.su/ad/current2`, and nothing is published there at the
moment — the request 404s and the app stays on its current version
without telling you. Update by downloading a new build.

## Building from source

Build instructions are inherited from upstream and kept current:

- [Windows](docs/building-win.md)
- [macOS](docs/building-mac.md)
- [Linux](docs/building-linux.md)
- [API credentials](docs/api_credentials.md)

## Branches and CI

| branch | what runs on push |
|---|---|
| `master` | the full matrix — Windows, Linux, macOS, macOS Packaged, Snap |
| `dev` | one fast Windows x64 build (`sccache`, minutes on a warm cache) |

Day-to-day work happens on `dev`; `master` receives it by pull request.
The workflow files are identical on both branches — each build is bound
to its branch by an explicit `branches:` list, so nothing diverges on
merge and a new branch never raises the four-platform matrix by accident.

## License

Ansible Desktop is free software: you can redistribute it and/or modify
it under the terms of the **GNU General Public License v3** with the
OpenSSL linking exception, as published by the Free Software Foundation.

- Full license text: [LICENSE](LICENSE)
- Attribution and copyright notice: [LEGAL](LEGAL)

By contributing to this repository, you agree that your contributions
will be licensed under the same terms.

## Trademarks

"Telegram" is a trademark of Telegram FZ-LLC. It is used in this README,
in `LEGAL` and in source file headers solely to identify the upstream
project from which this fork is derived, as required by the GPL. It is
**not** used as a trademark of this product.

"Ansible" is a trademark of the Ansible Desktop Authors.

## Contact

- General: <https://ansible.su>
- Source code questions: open an issue on this repository
- License compliance: <legal@ansible.su>
