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

#: имя файла пакета по платформе — так его называет упаковщик
#: (Ansible/SourceFiles/_other/packer.cpp)
PACK = {
    "win": "tupdate%d",
    "win64": "tx64upd%d",
    "winarm": "tarm64upd%d",
    "mac": "tmacupd%d",
    "armac": "tarmacupd%d",
    "linux": "tlinuxupd%d",
}


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
                   choices=["stable", "beta", "alpha"])
    a = p.parse_args()

    version = app_version(a.version)
    out = {}
    for plat in a.platform:
        out[plat] = {
            a.channel: {
                "released": version,
                "link": "/%s/%s" % (plat, PACK[plat] % version),
            }
        }
    json.dump(out, sys.stdout, ensure_ascii=False, indent=1, sort_keys=True)
    sys.stdout.write("\n")


if __name__ == "__main__":
    main()
