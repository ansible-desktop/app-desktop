# This file is part of Ansible Desktop, a fork of Telegram Desktop,
# the official desktop application for the Telegram messaging service.
#
# For license and copyright information please follow this link:
# https://github.com/behappy-desktop/app-desktop/blob/master/LEGAL

# Ansible Desktop: пара выдана порталом my.ansible.su (workflow/APP_CREDENTIALS.md).
#
# 🚨 `CACHE STRING` — не украшение, а восстановление механизма upstream. Здесь
# стояло `set(TDESKTOP_API_ID 1)` / `set(TDESKTOP_API_HASH "stub")` обычными
# переменными, а обычная переменная в CMake ПЕРЕКРЫВАЕТ кэш-переменную, тогда
# как `-D` пишет именно в кэш. Из-за этого любой `-D TDESKTOP_API_ID=...` из
# воркфлоу молча игнорировался, и в бинарь ВСЕГДА уезжал api_id=1 / "stub".
# Отсюда же ложный вывод «десктоп собирается с 611335»: креды снапа Телеграма
# в win.yml и snapcraft.yaml есть, но до компилятора не доходили никогда.
#
# Вместе с этим форк удалил и upstream-овский `message(FATAL_ERROR ...)`, который
# отказывается собирать без кредов, — потому подмена и не замечалась месяцами.
# Гейт возвращён ниже.
#
# 🚨 `option(TDESKTOP_API_TEST)` НАМЕРЕННО НЕ восстанавливаем, хотя шесть наших
# воркфлоу до сих пор передают `-D TDESKTOP_API_TEST=ON`. У upstream эта ветка
# подставляет ТЕЛЕГРАМОВСКУЮ тестовую пару 17349; восстановить её значило бы
# начать собирать наш клиент под чужой личностью — ровно то, что мы чиним.
# Флаг остаётся мёртвым осознанно; чистить воркфлоу — отдельная задача.
set(TDESKTOP_API_ID "21000002" CACHE STRING "Provide 'api_id' for the Ansible API access.")
set(TDESKTOP_API_HASH "316a4ba0755f7ca055e5f55e5f15ebbc" CACHE STRING "Provide 'api_hash' for the Ansible API access.")

if (TDESKTOP_API_ID STREQUAL "0" OR TDESKTOP_API_ID STREQUAL "" OR TDESKTOP_API_HASH STREQUAL "")
    message(FATAL_ERROR
    " \n"
    " PROVIDE: -D TDESKTOP_API_ID=[API_ID] -D TDESKTOP_API_HASH=[API_HASH]\n"
    " \n"
    " > Get your own at https://my.ansible.su\n"
    " \n")
endif()

# BeHappy: always disable autoupdate and crash reports (no Telegram servers)
target_compile_definitions(Telegram PRIVATE TDESKTOP_DISABLE_AUTOUPDATE)
target_compile_definitions(Telegram PRIVATE TDESKTOP_DISABLE_CRASH_REPORTS)

if (DESKTOP_APP_USE_PACKAGED)
    target_compile_definitions(Telegram PRIVATE TDESKTOP_USE_PACKAGED)
endif()

if (DESKTOP_APP_SPECIAL_TARGET)
    target_compile_definitions(Telegram PRIVATE TDESKTOP_ALLOW_CLOSED_ALPHA)
endif()
