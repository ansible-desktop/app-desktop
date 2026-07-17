# Локальная сборка десктопа (быстрый цикл разработки)

Идея: собирать **локально** (первый билд ~1–1.5 ч, дальше правка → пересборка **1–3 мин**, т.к.
пересобираются только изменённые файлы), проверять, и уже потом пушить в `master`/тег `v*` на
полноценный CI-релиз. Продуктовый CI (`release_win.yml`) не трогаем.

> Совпадает с CI: **Visual Studio 2022**, Windows SDK **10.0.26100.0**, Qt6, Ninja. (Upstream-гайд
> `building-win.md` упоминает VS2026 — нам не нужен, CI на 2022, берём 2022.)

Обозначения: **`<BuildPath>`** — пустая папка под сборку (например `D:\TBuild`); **`<repo>`** —
клон этого репозитория внутри неё (`<BuildPath>\app-desktop`).

## 1. Поставить тулчейн (один раз, нужен админ)
В PowerShell **от администратора** запусти бутстрап из репозитория:
```powershell
<repo>\docs\dev-bootstrap-win.ps1
```
Ставит через winget: VS2022 Community + ворклоад «Desktop development with C++», Windows 11 SDK
26100, VC x64, а также Strawberry Perl, Go, 7-Zip. Python 3.10+/Git должны уже стоять. NASM ставит
сам `prepare.py`. Что уже есть — winget пропустит.

## 2. Разложить папки сборки
`prepare.py` кладёт зависимости в `Libraries` и `ThirdParty` **рядом** с репозиторием:
```
<BuildPath>\
  app-desktop\      (этот репозиторий)
  Libraries\
  ThirdParty\
```
```cmd
mkdir <BuildPath>\Libraries
mkdir <BuildPath>\ThirdParty
```

## 3. Открыть терминал сборки (важно — именно так)
tdesktop собирается из «x64 Native Tools» с тулсетом 14.44 (Win7-совместимый v143):
```
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" -vcvars_ver=14.44
```
Дальше все команды — в этом окне (это `cmd`, не PowerShell).

## 4. Собрать зависимости (один раз, ~1–1.5 ч)
```cmd
cd /d <BuildPath>
app-desktop\Telegram\build\prepare\win.bat
```
Качает/собирает Qt/OpenSSL/FFmpeg/… в `Libraries`. Долго, но **однократно** (потом кэш).
Упадёт на отсутствующем инструменте (perl/go/…) — `winget install <id>` и перезапусти win.bat.

## 5. Сконфигурировать + собрать приложение
```cmd
cd /d <BuildPath>\app-desktop\Telegram
configure.bat x64 qt6 -D TDESKTOP_API_TEST=ON -D CMAKE_CONFIGURATION_TYPES=Debug
cmake --build ..\out --config Debug --parallel
```
- `TDESKTOP_API_TEST=ON` — как в CI, без реальных api_id/api_hash.
- Первая сборка приложения ~20–30 мин. Результат: `..\out\Debug\Ansible.exe`.

## 6. 🔁 Быстрый цикл (ради чего всё затевалось)
Правишь `.cpp`/`.h` → в том же терминале:
```cmd
cmake --build ..\out --config Debug --parallel
```
Пересобираются **только изменённые** единицы трансляции → **1–3 мин**. Запуск: `..\out\Debug\Ansible.exe`.

Альтернатива — открыть `..\out\Telegram.slnx` в Visual Studio 2022 и жать Build (то же инкрементально, с дебагом).

## 7. Убедился локально → в прод
```cmd
git add -A && git commit -m "…" && git push origin master
git tag -a vX.Y.Z <sha> -m "…" && git push origin vX.Y.Z   :: → release_win.yml соберёт полный релиз
```

---
## Заметки
- **Ошибки компиляции ловятся локально за минуты**, а не за часы в CI — то, что нужно.
- Хочешь 1:1 с CI (чтобы не было «локально ок, в CI красный») — добавь `-D CMAKE_COMPILE_WARNING_AS_ERROR=ON`.
- Ресурсы (иконки/лэнг/`.style`) — просто пересобрать; codegen отработает.
- Для других разработчиков есть быстрый **CI-путь**: тег `dev-*` или ручной запуск воркфлоу
  **“Dev Windows (fast).”** (`.github/workflows/dev_win.yml`) — Windows x64 Debug + sccache, ~10 мин на тёплом кэше.
