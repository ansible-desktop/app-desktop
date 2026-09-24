# -*- coding: utf-8 -*-
"""Сборка манифеста current2 для сервиса обновлений.

    python make_current.py --version 0.3.1 \
        --platform win64 --platform linux > current2

Формат снят с клиента (core/update_checker.cpp, ParseCommonMap):

    { "<платформа>": { "<канал>": { "released": <версия>, "link": "<путь>" } } }

  * платформа — ключ Platform::AutoUpdateKey(): win, win64, winarm, mac,
    armac, linux. Клиент ищет ТОЛЬКО свой ключ; нет его — «Update Error:
    platform not found», обновления нет;
  * канал — stable | beta | alpha. Обычная сборка смотрит только stable;
  * released — ЦЕЛОЕ AppVersion (0.3.1 -> 3003001), а не строка "0.3.1".
    Клиент обновляется строго при available > своей: равная не подходит;
  * link — путь, который ПРИКЛЕИВАЕТСЯ К ПРЕФИКСУ. Префикс уже
    https://ansible.su/ad, поэтому link начинается со слэша и полного
    адреса не содержит. Подстановка {version} в нём раскрывается клиентом.

🚨 Ключ "testing" вместо "released" отдаётся сборкам, запущенным в тестовом
режиме. Здесь он не нужен, но если появится — держать его НЕ ВЫШЕ released,
иначе обычные клиенты поедут на непроверенную сборку.
"""
import argparse
import json
import sys

#: имя файла пакета по платформе.
#: 🚨 Это формат v2 — тот, что РЕАЛЬНО создаёт упаковщик (packer.cpp,
#: V2FileName): ansible-update-{os}-{arch}-{версия}. Раньше здесь стояли
#: имена схемы v1 (tx64upd…), и манифест ссылался на файл, которого
#: упаковщик не создаёт ни при каких условиях.
#: Ключ платформы приходит от клиента (Platform::AutoUpdateKey), а os и arch
#: — то, чем их называет сам упаковщик (Core::Updates::OsName/ArchName).
PACK = {
    "win":    ("win", "x86"),
    "win64":  ("win", "x64"),
    "winarm": ("win", "arm"),
    "mac":    ("mac", "x64"),
    "armac":  ("mac", "arm"),
    "linux":  ("linux", "x64"),
}

#: суффикс канала — его упаковщик приклеивает к имени сам
SUFFIX = {"stable": "", "beta": "-beta"}


def app_version(text):
    """0.3.1 -> 3003001. Схема форка, та же, что в set_version.py."""
    parts = [int(x) for x in text.split(".")]
    if len(parts) != 3:
        raise ValueError("версия задаётся как 0.<minor>.<patch>")
    major, minor, patch = parts
    if major != 0:
        raise ValueError("схема рассчитана на мажор 0")
    return 3000000 + minor * 1000 + patch


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--version", required=True, help="например 0.3.1")
    p.add_argument("--platform", action="append", required=True,
                   choices=sorted(PACK), help="можно указывать несколько раз")
    p.add_argument("--channel", default="stable",
                   choices=sorted(SUFFIX))
    a = p.parse_args()

    version = app_version(a.version)
    out = {}
    for plat in a.platform:
        out[plat] = {
            a.channel: {
                "released": version,
                "link": "/%s/ansible-update-%s-%s-%d%s" % (
                plat, PACK[plat][0], PACK[plat][1], version, SUFFIX[a.channel]),
            }
        }
    json.dump(out, sys.stdout, ensure_ascii=False, indent=1, sort_keys=True)
    sys.stdout.write("\n")


if __name__ == "__main__":
    main()
