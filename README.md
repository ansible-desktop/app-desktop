# Ansible Desktop

<img src="Ansible/Resources/art/icon128.png" width="80" alt="Значок Ansible Desktop">

Настольное приложение мессенджера Ansible для **Windows, Linux и macOS**. Это независимый **форк Telegram Desktop** с интерфейсом на Qt. Для входа нужна учётная запись Ansible. [Исходный проект Telegram Desktop](https://github.com/telegramdesktop/tdesktop).

## Установка и обновление

Сборки репозитория доступны в GitHub Actions. Если Ansible Desktop уже установлен, перед переходом на новую сборку посмотрите, как клиент сохраняет профиль и настройки. [Сборки](https://github.com/ansible-desktop/app-desktop/actions) · [Инструкция по обновлению](docs/upgrading.md).

## Сборка из исходников

Подготовьте инструменты для своей платформы, затем настройте параметры API приложения. Для macOS предусмотрены отдельные инструкции для обычной сборки и Mac App Store.

| Платформа | Инструкция |
| --- | --- |
| Windows | [Сборка для Windows](docs/building-win.md) |
| Linux | [Сборка для Linux](docs/building-linux.md) |
| macOS | [Сборка для Mac](docs/building-mac.md) · [Mac App Store](docs/building-mas.md) |

[Параметры API](docs/api_credentials.md) · [История изменений](changelog.txt). Нативный клиент для Mac на Swift и AppKit разрабатывается отдельно. [Ansible для macOS](https://github.com/ansible-ios/app-macos).

## Лицензия и авторство

Ansible Desktop распространяется по GNU GPL версии 3 с исключением для связывания с OpenSSL. Права авторов Telegram Desktop и условия сторонних компонентов сохраняются. [Лицензия](LICENSE) · [Сведения об авторах и компонентах](LEGAL).

[Сайт Ansible](https://ansible.su) · [Сообщить о проблеме](https://github.com/ansible-desktop/app-desktop/issues)
