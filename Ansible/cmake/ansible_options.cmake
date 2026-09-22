# This file is part of Ansible Desktop, a fork of Telegram Desktop,
# the official desktop application for the Telegram messaging service.
#
# For license and copyright information please follow this link:
# https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL

# Ansible Desktop: пара выдана порталом my.ansible.su 2026-08-14,
# реестр — workflow/APP_CREDENTIALS.md, таблица dev_apps (владелец uid 777).
#
# 🚨 CACHE STRING обязателен. Обычная переменная в CMake ПЕРЕКРЫВАЕТ
# кэш-переменную, а -D пишет именно в кэш. Форк когда-то заменил эти строки
# обычным set(TDESKTOP_API_ID 1) — и любой -D TDESKTOP_API_ID=... из
# воркфлоу молча игнорировался: в бинарь месяцами уезжал api_id=1.
# Механизм апстрима здесь цел, задача — не сломать его снова.
#
# Пара не секрет: она уезжает в бинарь и достаётся через strings. У
# апстрима она лежит здесь же открытым текстом, у Android — в
# BuildVars.java, у iOS — в appstore-configuration.json. Контроль
# серверный: sessions.api_id, бан и троттлинг по api_id.
#
# 🚨 option(TDESKTOP_API_TEST) УДАЛЁН намеренно. У апстрима эта ветка
# подставляет ТЕЛЕГРАМОВСКУЮ тестовую пару 17349 — восстановить её значило
# бы собирать наш клиент под чужой личностью. Часть воркфлоу до сих пор
# передаёт -D TDESKTOP_API_TEST=ON; флаг теперь просто ни на что не влияет.
set(TDESKTOP_API_ID "21000002" CACHE STRING "Provide 'api_id' for the Ansible API access.")
set(TDESKTOP_API_HASH "316a4ba0755f7ca055e5f55e5f15ebbc" CACHE STRING "Provide 'api_hash' for the Ansible API access.")

if (TDESKTOP_API_ID STREQUAL "0" OR TDESKTOP_API_ID STREQUAL "" OR TDESKTOP_API_HASH STREQUAL "")
    message(FATAL_ERROR
    " \n"
    " PROVIDE: -D TDESKTOP_API_ID=[API_ID] -D TDESKTOP_API_HASH=[API_HASH]\n"
    " \n"
    " > Get your own api_id at https://my.ansible.su\n"
    " ")
endif()

if (DESKTOP_APP_DISABLE_AUTOUPDATE)
    target_compile_definitions(Telegram PRIVATE TDESKTOP_DISABLE_AUTOUPDATE)
endif()

if (DESKTOP_APP_DISABLE_CRASH_REPORTS)
    target_compile_definitions(Telegram PRIVATE TDESKTOP_DISABLE_CRASH_REPORTS)
endif()

if (DESKTOP_APP_USE_PACKAGED)
    target_compile_definitions(Telegram PRIVATE TDESKTOP_USE_PACKAGED)
endif()

if (DESKTOP_APP_SPECIAL_TARGET)
    target_compile_definitions(Telegram PRIVATE TDESKTOP_ALLOW_CLOSED_ALPHA)
endif()

option(DESKTOP_APP_DISABLE_SWIFT6 "Disable local on-device translation (build without Swift 6 on macOS)." OFF)
if (DESKTOP_APP_DISABLE_SWIFT6)
    target_compile_definitions(Telegram PRIVATE TDESKTOP_DISABLE_SWIFT6)
endif()

set(TDESKTOP_UPDATE_CHANNEL "stable" CACHE STRING "Compile-time update channel (stable, beta, canary-public, canary-private).")
set(TDESKTOP_CANARY_COUNTER "0" CACHE STRING "Per-channel canary build counter, required positive for canary channels.")
set(TDESKTOP_CANARY_COMMIT "" CACHE STRING "Short commit hash shown in the canary version string.")
set(TDESKTOP_CANARY_PUBLIC_CHANNEL "" CACHE STRING "Public canary channel username (canary-public builds).")
set(TDESKTOP_CANARY_PRIVATE_CHANNEL_ID "0" CACHE STRING "Private canary channel numeric id (canary-private builds).")
set(TDESKTOP_CANARY_METADATA_MSG_ID "0" CACHE STRING "Fixed metadata message id in the canary channel.")

# CI passes these straight from repository variables, an unset variable
# arrives as an empty string and must mean "not configured", not an
# empty macro body.
foreach(numeric_option
    TDESKTOP_CANARY_COUNTER
    TDESKTOP_CANARY_PRIVATE_CHANNEL_ID
    TDESKTOP_CANARY_METADATA_MSG_ID)
    if (${numeric_option} STREQUAL "")
        set(${numeric_option} 0)
    elseif (NOT ${numeric_option} MATCHES "^[0-9]+$")
        message(FATAL_ERROR "${numeric_option} must be a non-negative integer, got '${${numeric_option}}'.")
    endif()
endforeach()

if (TDESKTOP_UPDATE_CHANNEL STREQUAL "stable")
    set(tdesktop_update_channel_value 0)
elseif (TDESKTOP_UPDATE_CHANNEL STREQUAL "beta")
    set(tdesktop_update_channel_value 1)
elseif (TDESKTOP_UPDATE_CHANNEL STREQUAL "canary-public")
    set(tdesktop_update_channel_value 2)
elseif (TDESKTOP_UPDATE_CHANNEL STREQUAL "canary-private")
    set(tdesktop_update_channel_value 3)
else()
    message(FATAL_ERROR "Bad TDESKTOP_UPDATE_CHANNEL '${TDESKTOP_UPDATE_CHANNEL}'")
endif()

if (tdesktop_update_channel_value GREATER 1)
    if (TDESKTOP_CANARY_COUNTER LESS_EQUAL 0)
        message(FATAL_ERROR "Canary channels require a positive TDESKTOP_CANARY_COUNTER.")
    endif()
elseif (NOT TDESKTOP_CANARY_COUNTER EQUAL 0)
    message(FATAL_ERROR "TDESKTOP_CANARY_COUNTER requires a canary TDESKTOP_UPDATE_CHANNEL.")
endif()

target_compile_definitions(Telegram
PRIVATE
    TDESKTOP_UPDATE_CHANNEL=${tdesktop_update_channel_value}
    TDESKTOP_CANARY_COUNTER=${TDESKTOP_CANARY_COUNTER}
    TDESKTOP_CANARY_PRIVATE_CHANNEL_ID=${TDESKTOP_CANARY_PRIVATE_CHANNEL_ID}
    TDESKTOP_CANARY_METADATA_MSG_ID=${TDESKTOP_CANARY_METADATA_MSG_ID}
)
if (NOT TDESKTOP_CANARY_COMMIT STREQUAL "")
    target_compile_definitions(Telegram PRIVATE TDESKTOP_CANARY_COMMIT=${TDESKTOP_CANARY_COMMIT})
endif()
if (NOT TDESKTOP_CANARY_PUBLIC_CHANNEL STREQUAL "")
    target_compile_definitions(Telegram PRIVATE TDESKTOP_CANARY_PUBLIC_CHANNEL=${TDESKTOP_CANARY_PUBLIC_CHANNEL})
endif()
