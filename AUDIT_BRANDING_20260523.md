# Аудит остаточного Telegram-брендинга в BeHappy Desktop

**Дата:** 2026-05-23
**Скоуп:** `F:\test_cl\split\ansible-desktop\app-desktop\` (= `ansible-desktop/app-desktop`, форк `tdesktop`)
**HEAD:** `a560f6d feat(mtproto): swap built-in DC1 RSA public key`
**Тип:** Read-only — никаких изменений в исходниках, только этот отчёт.

---

## 1. TL;DR

- **Ребрендинг ~30% сделан.** Прежние коммиты (`a591ec9`, `0e616d6`, `b7c0f99`, `fd85f3b`) поменяли: иконки приложения (16-1024px), `AppName/AppFile/AppNameOld` в `core/version.h`, MTProto-конфиг, RU-лангпак (частично), LEGAL/NOTICE/README/CHANGELOG. Этого недостаточно для prod-релиза под брендом BeHappy.
- **🔴 Критично:** на Windows бинарь именуется `Telegram.exe` (зашит в `CMakeLists.txt:7`, `setup.iss:5`, `Telegram.rc`, `winget.yml`, ~20 build-скриптах). Папка данных = `%APPDATA%\Telegram Desktop\` (потому что `AppName` = `"BeHappy Desktop"`, НО `psAppDataPath()` собирает её динамически, и **установленный бинарь, который видит юзер, был собран до a591ec9** — пересборка устранит этот симптом).
- **🔴 AppUserModelID `Telegram.TelegramDesktop.<hash>`** — зашит в `windows_app_user_model_id.cpp:29,31`. Это идентификатор для Windows-уведомлений и pin-to-taskbar. Влияет на toast notifications, иконки в task-bar grouping.
- **🔴 Linux/macOS bundle identifier** = `org.telegram.desktop` (`CMakeLists.txt:2077,2097`, `lib/xdg/org.telegram.desktop.{desktop,metainfo.xml,service}`). При установке `.deb`/`.snap`/`.dmg` BeHappy будет конфликтовать с настоящим Telegram Desktop пользователя.
- **🔴 UWP/Store манифест** (`AppxManifest.xml`) полностью telegram-овский: `TelegramMessengerLLP.TelegramDesktop`, `DisplayName="Telegram Desktop"`, `TaskId="TelegramStartupTask"`. **Блокер для Microsoft Store.**
- **🔴 322 строки** `lng_*` в `lang.strings` (EN) и **373** в `lang_ru.strings` упоминают "Telegram" — включая `lng_intro_about` ("Welcome to the official Telegram Desktop app"), `lng_about_text3_faq`, `lng_open_from_tray` ("Open Telegram"), весь раздел Premium/Stars/Sponsored.
- **🟡 26 hard-coded URL'ов** на `t.me/`, `telegram.org`, `core.telegram.org`, `desktop.telegram.org` в исходниках; **27 URL'ов** в lang.strings.
- **🟡 Telegram FZ-LLC** прописан как Publisher: `setup.iss:3`, `Telegram.rc:63`, `Updater.rc:54`, `updater_win.cpp:312`, `AppxManifest.xml:10,16`, и **в codesign-командах** `build.sh:330-335`. При подписи build'ом BeHappy эти codesign-команды сфейлятся (не тот dev-cert).
- **🟢 2459 файлов** в `Telegram/SourceFiles/` начинаются с шапки "This file is part of Telegram Desktop, the official desktop application for the Telegram messaging service. For license… https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL". Это **легально корректно** (форк, GPL §6), но один скриптовый sed-replace заметно почистит атрибуцию и подтвердит, что файлы — производная работа BeHappy.
- **Оценка работы:** **80-120 часов** на полный clean-up. Самое долгое — `lang.strings` (≈8-16 ч) и систематический проход по hard-coded "Telegram Desktop" в .cpp/.h (≈6-10 ч). Иконки `intro_plane*` (paper-plane Telegram'а) — ≈4 ч (дизайнер + замена 15 PNG/SVG).

---

## 2. Условные обозначения серьёзности

- 🔴 **Критично** — пользователь сразу видит, или это блокер магазина приложений / трейдмарк-конфликт / конфликт с настоящим Telegram'ом у юзера на машине.
- 🟡 **Средне** — видно саппорту/в bug-репортах/в URL bar/при экспорте; шиппится, но не на главном экране.
- 🟢 **Косметика** — внутренние комментарии, копирайт-шапки, имена переменных. Не блокеры, но желательно почистить.

---

## 3. Категория 1. Видимые текстовые строки

### 3.1. `lang.strings` (EN, источник)

Файл: `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/langs/lang.strings`
**Всего ключей:** 7496. **Содержат "Telegram":** 320 уникальных ключей, 322 матча.

#### 🔴 Самые видимые (rendered в окне приложения)

| Ключ | Где видно | Текущее значение | Действие |
|------|-----------|------------------|----------|
| `lng_intro_about` (`lang.strings:379`) | Welcome-экран | "Welcome to the official **Telegram Desktop** app.\nIt's fast and secure." | Заменить на BeHappy-описание |
| `lng_open_from_tray` (`lang.strings:31`) | Системный трей | "Open Telegram" | "Open BeHappy" |
| `lng_quit_from_tray` (`lang.strings:33`) | Системный трей | "Quit Telegram" | "Quit BeHappy" |
| `lng_tray_icon_text` (`lang.strings:34`) | Tray tooltip | "**Telegram** is still running here…" | Заменить "Telegram" → "BeHappy" |
| `lng_settings_auto_start` (`lang.strings:674`) | Settings | "Launch **Telegram** when system starts" | Заменить |
| `lng_settings_add_sendto` (`lang.strings:678`) | Settings | "Place **Telegram** in \"Send to\" menu" | Заменить |
| `lng_settings_auto_start_disabled_uwp` (`lang.strings:676`) | Settings | "…Please enable **Telegram Desktop** in the Startup Apps Settings." | Заменить |
| `lng_passcode_about*` (`lang.strings:1166-1169`) | Settings → Privacy | "…log out of **Telegram Desktop** and log in again." | Заменить |
| `lng_proxy_unsupported` (`lang.strings:340`) | Proxy box | "Your **Telegram Desktop** version doesn't support this proxy type…" | Заменить |
| `lng_theme_no_desktop` (`lang.strings:1090`) | Theme apply error | "Sorry, this theme doesn't include a version for **Telegram Desktop**." | Заменить |
| `lng_about_text3_faq` (`lang.strings:5586`) | About → FAQ link text | "**Telegram** FAQ" | "BeHappy FAQ" |
| `lng_about_text1_api` (`lang.strings:5583`) | About → API link text | "**Telegram** API" | "BeHappy API" (или удалить раздел) |
| `lng_new_version_wrap` (`lang.strings:5661`) | Update toast | "**Telegram Desktop** was updated to version {version}…" | Заменить |
| `lng_outdated_now` / `lng_outdated_soon` (`lang.strings:7068-7069`) | Outdated warning | "…**Telegram Desktop** will stop updating on {date}." | Заменить |
| `lng_group_call_mac_access` (`lang.strings:6028`) | Group call error | "**Telegram Desktop** does not have access to system wide keyboard input…" | Заменить |
| `lng_group_call_mac_screencast_access` (`lang.strings:6031`) | Group call error | "**Telegram Desktop** does not have access to screen recording…" | Заменить |
| `lng_terms_delete_warning` (`lang.strings:6670`) | Delete-account confirm | "…delete your **Telegram** account and all the data you store in the **Telegram** cloud…" | Заменить (особенно важно — диалог об удалении!) |
| `lng_passport_app_out_of_date` / `lng_passport_*` | Passport | Telegram Passport flow | Удалить весь раздел (BeHappy не использует Passport) |
| `lng_language_not_ready_about` (`lang.strings:6916`) | Language box | "…this custom language pack doesn't contain data for **Telegram Desktop**…" | Заменить |
| `lng_settings_passkeys_unsigned_error` (`lang.strings:1328`) | Passkeys | "…Please use an official signed version of **Telegram Desktop**." | Заменить |
| `lng_bot_share_location_unavailable` (`lang.strings:2866`) | Bot location | "…currently unavailable in **Telegram Desktop**." | Заменить |

#### 🔴 URL'ы на Telegram-инфраструктуру внутри lang.strings (27 шт.)

| Ключ | Строка | URL | Куда уйдёт |
|------|--------|-----|------------|
| `lng_settings_faq_link` (`lang.strings:1298`) | "Ask a Question" → URL | `https://telegram.org/faq#general-questions` | Должна быть behappy-FAQ |
| `lng_settings_suggestion_phone_number_about_link` (`lang.strings:916`) | | `https://telegram.org/faq#q-i-have-a-new-phone-number-what-do-i-do` | Заменить или удалить |
| `lng_profile_bot_privacy_url` (`lang.strings:1604`) | | `https://telegram.org/privacy-tpa` | Заменить |
| `lng_star_ref_tos_url` (`lang.strings:2038`) | Star Stars TOS | `https://telegram.org/tos/mini-apps` | Удалить весь Stars/Star Ref раздел |
| `lng_group_invite_subscription_about_url` (`lang.strings:2788`) | | `https://telegram.org/tos/stars` | Удалить |
| `lng_business_about_sponsored_url` (`lang.strings:3152`) | | `https://ads.telegram.org` | Удалить |
| `lng_credits_summary_options_about_url` (`lang.strings:3167`) | | `https://telegram.org/tos/stars` | Удалить |
| `lng_credits_box_out_about_link` (`lang.strings:3216`) | | `https://telegram.org/tos/stars` | Удалить |
| `lng_chatbots_info_url` (`lang.strings:3452`) | | `https://telegram.org/blog/telegram-business#chatbots-for-business` | Удалить |
| `lng_paid_about_link_url` (`lang.strings:5206`) | | `https://telegram.org/blog/telegram-stars` | Удалить |
| `lng_translate_cocoon_text_link` (`lang.strings:5278`) | | `t.me/cocoon?direct` | Заменить (BeHappy translate provider?) |
| `lng_sponsored_info_description1_url` (`lang.strings:7281`) | | `https://telegram.org/privacy#5-6-no-ads-based-on-user-data` | Удалить весь Sponsored раздел |
| `lng_telegram_features_url` (`lang.strings:7308`) | "What's new" button → URL | `https://t.me/TelegramTips` | Заменить на BeHappy-канал |
| `lng_mini_apps_tos_url` (`lang.strings:7314`) | | `https://telegram.org/tos/mini-apps` | Заменить |
| `lng_channel_earn_learn_coin_link` (`lang.strings:7688`) | | `https://telegram.org/blog/monetization-for-channels` | Удалить |
| `lng_bot_earn_balance_about_url` (`lang.strings:7706`) | | `https://telegram.org/tos/stars` | Удалить |
| `lng_popular_apps_info_url` (`lang.strings:7802`) | | `https://core.telegram.org/bots/webapps#launching-the-main-mini-app` | Заменить (если есть BeHappy WebApps доки) |

#### 🟡 Premium/Stars/Boosts/Sponsored (≈118 ключей)

В `lang.strings` 118 строк с "**Telegram Premium**" / "**Telegram Stars**" / "**Telegram FZ-LLC**". Эти фичи в BeHappy **не реализованы** (см. `CHANGELOG.md:23-25`, `README.md:29-31`). Рекомендация: либо
- (a) удалить из lang.strings все ключи `lng_premium_*`, `lng_credits_*`, `lng_stars_*`, `lng_sponsored_*`, `lng_giveaway_*`, `lng_prize_*`, `lng_chatbots_premium_*`, `lng_boost_*`, ИЛИ
- (b) оставить ключи (так как C++ код их читает через `tr::lng_*` и удаление приведёт к panic), но заменить упоминания "Telegram Premium" на "BeHappy Premium" (если планируется) или нейтральные "Premium subscription" / удалить кнопки.

Решение зависит от продуктовой стратегии. Если **Premium НЕ планируется** — UI уже не рендерит эти строки (после удаления соответствующих секций в C++), и достаточно сделать строки безопасными (replace "Telegram Premium" → "Premium", чтобы fallback не показал чужой бренд при regression).

#### Полный список 320 ключей с упоминаниями "Telegram" в `lang.strings`

<details>
<summary>Распахнуть полный список (320 keys)</summary>

```
lng_about_text1_api, lng_about_text3_faq, lng_accounts_limit2, lng_action_gift_displayed_self,
lng_action_gift_premium_about, lng_action_giveaway_results#one/other, lng_action_giveaway_results_credits#one/other,
lng_action_giveaway_results_credits_some, lng_action_giveaway_results_some, lng_action_giveaway_started,
lng_action_giveaway_started_group, lng_action_suggested_photo/me, lng_action_suggested_video/me,
lng_action_user_registered, lng_age_verify_here, lng_age_verify_mobile, lng_ai_compose_flood_link,
lng_allow_bot_pass, lng_animated_emoji_text, lng_auction_preview_learn_gifts, lng_bad_photo,
lng_blocked_list_confirm_text, lng_boost_available_in_toast#one/other, lng_boost_channel_gifting,
lng_boost_error_gifted_text(_group), lng_boost_error_premium_text(_group), lng_boost_group_gifting,
lng_boost_need_more_again#one/other, lng_boost_need_more_text#one/other, lng_boost_reassign_gift#one/other,
lng_boosts_get_boosts_subtext(_group), lng_boosts_prepaid_giveaway_quantity#one/other,
lng_bot_no_scan_qr, lng_bot_no_share_story, lng_bot_share_location_unavailable,
lng_business_about_sponsored(_link), lng_business_title, lng_call_box_about, lng_call_status_group_invite,
lng_caption_limit2#one/other, lng_change_phone_error, lng_channel_earn_about(_bot),
lng_channel_earn_learn_bot_in_about, lng_channel_earn_learn_coin_about, lng_channel_earn_learn_in_about,
lng_channel_earn_learn_in_subtitle, lng_channels_limit2#one/other, lng_chat_link_qr_about,
lng_chatbots_add_about, lng_chatbots_not_supported, lng_clear_payment_info_confirm,
lng_cloud_password_about_recover, lng_cloud_password_create, lng_cloud_password_passport_losing,
lng_code_call, lng_code_called, lng_code_calling, lng_code_from_telegram,
lng_confcall_create_link_description, lng_confcall_join_text(_inviter), lng_confcall_link_about,
lng_confirm_phone_about, lng_contact_not_joined, lng_credits_box_history_entry_gift_in/out_about,
lng_credits_currency_summary_about, lng_credits_gift_title, lng_credits_purchase_blocked,
lng_credits_small_balance_fallback, lng_credits_summary_about, lng_credits_summary_title,
lng_dialogs_suggestions_premium_annual_about/title, lng_dialogs_suggestions_premium_restore_about,
lng_dialogs_suggestions_premium_upgrade_about/title, lng_disable_sharing_unlock,
lng_download_path_default_radio, lng_edit_lastseen_subscribe, lng_edit_privacy_phone_number_contacts,
lng_edit_privacy_phone_number_warning, lng_edit_privacy_premium_status, lng_effect_premium_link,
lng_error_start_minimized_passcoded, lng_factcheck_links, lng_file_size_limit2,
lng_filter_chats_limit2#one/other, lng_filter_links_limit2#one/other, lng_filter_pin_limit2#one/other,
lng_filter_shared_limit2#one/other, lng_filters_limit2#one/other, lng_group_about3,
lng_group_invite_about_permanent_channel/group, lng_group_invite_qr_about, lng_intro_about,
lng_intro_qr_step1, lng_intro_qr_title, lng_language_not_ready_about, lng_lastseen_premium_button,
lng_limit_download/upload_subscribe_link, lng_links_limit2#one/other, lng_mac_menu_show,
lng_manage_peer_antispam_about, lng_manage_peer_reactions_paid_about, lng_menu_not_contact,
lng_message_unsupported, lng_messages_privacy_premium_about/button/link, lng_mini_apps_disclaimer_text,
lng_new_contact_about_status_link, lng_new_version_wrap, lng_no_conversations_subtitle,
lng_no_mic_permission, lng_open_from_tray, lng_outdated_now, lng_outdated_soon,
lng_passcode_about/2/3, lng_passcode_winhello_unlock, lng_passport_app_out_of_date,
lng_passport_restart_sure, lng_passport_title, lng_passport_use_existing_email/phone,
lng_payments_warning_body, lng_peer_gifts_notify_enabled, lng_posts_subtitle_empty,
lng_premium_emoji_status_about, lng_premium_gift_about/terms/title, lng_premium_gifts_terms,
lng_premium_more_about, lng_premium_stories_about_mobile, lng_premium_success,
lng_premium_summary_about_no_ads, lng_premium_summary_about_profile_badge,
lng_premium_summary_bottom_about/subtitle, lng_premium_summary_subtitle_business,
lng_premium_summary_subtitle_gift(#one/other), lng_premium_summary_subtitle_gift_days(#one/other),
lng_premium_summary_subtitle_gift_days_me(#one/other), lng_premium_summary_subtitle_gift_me(#one/other),
lng_premium_summary_subtitle_gifts, lng_premium_summary_title, lng_premium_summary_top_about,
lng_premium_summary_user_about/title, lng_premium_unavailable, lng_prize_duration,
lng_prize_gift_duration, lng_prize_unclaimed_duration, lng_prizes_about#one/other,
lng_prizes_admins(_group)#one/other, lng_prizes_end_when_finish, lng_prizes_how_when_finish,
lng_prizes_results_about#one/other, lng_profile_accept_photo/video_sure, lng_profile_suggest_sure,
lng_profile_unofficial_warning, lng_proxy_about, lng_proxy_sponsor_about/warning,
lng_proxy_unsupported, lng_quit_from_tray, lng_readtime_premium_button, lng_schedule_repeat_promo_link,
lng_screen_reader_bar_text, lng_screen_reader_confirm_text, lng_send_as_premium_required_link,
lng_send_non_premium_message_toast_link, lng_sessions_about_apps, lng_sessions_other_desc,
lng_settings_add_sendto, lng_settings_ask_sure, lng_settings_auto_start(_disabled_uwp),
lng_settings_cloud_login_email_about, lng_settings_cloud_login_email_box_about,
lng_settings_cloud_password_start_about, lng_settings_color_link_name,
lng_settings_disconnect_all_sure, lng_settings_events_joined, lng_settings_export_data,
lng_settings_faq, lng_settings_features, lng_settings_generic_subscribe_link,
lng_settings_logged_in_description/title, lng_settings_passkeys_unsigned_error,
lng_settings_peer_to_peer_about, lng_settings_privacy_premium_link,
lng_settings_suggestion_phone_number_about/change, lng_settings_use_applewatch_about,
lng_settings_use_systempwd_about, lng_settings_use_touchid_about, lng_settings_use_winhello_about,
lng_settings_username_about, lng_signin_sure_reset, lng_similar_channels_premium_all_link,
lng_sponsored_info_description1_linked, lng_sponsored_info_description2,
lng_sponsored_revenued_footer_bot_description, lng_sponsored_revenued_footer_description,
lng_sponsored_revenued_footer_search_description, lng_sponsored_revenued_info1_bot_description,
lng_sponsored_revenued_info1_description, lng_sponsored_revenued_info1_search_description,
lng_sponsored_revenued_info2_bot_description, lng_sponsored_revenued_info2_description,
lng_sponsored_revenued_info3_search_description, lng_sponsored_revenued_subtitle,
lng_star_ref_created_text, lng_star_ref_launch_about, lng_star_ref_updated_text,
lng_stars_about_gifts_telegram, lng_stars_rating_about(_your), lng_stars_title_gifts_telegram,
lng_stealth_mode_unlock_about, lng_stories_unsupported, lng_sure_save_language,
lng_telegram_features_url, lng_terms_delete_warning, lng_terms_signup_sorry,
lng_terms_update_sorry, lng_theme_editor_need_unlock, lng_theme_no_desktop,
lng_todo_premium_link, lng_translate_cocoon_efficient_text, lng_tray_icon_text,
lng_unconfirmed_auth_denied_warning, lng_update_telegram, lng_username_description1,
lng_verification_codes_about
```
</details>

### 3.2. `lang_ru.strings` (RU)

Файл: `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/langs/lang_ru.strings`
**Всего:** 8984 строк. **Содержит "Telegram":** 373 матча.

Аналогично EN: `lng_intro_about` (`lang_ru.strings:479`) = "Добро пожаловать в **Telegram для ПК**…" — заменить. RU-лангпак был ПОЛНОСТЬЮ скопирован коммитом `a591ec9` из официального Telegram-перевода без замены трейдмарка (см. сообщение коммита: "lang.strings branding renames will be ported as targeted edits later" — но **edits так и не сделаны**).

### 3.3. Видимые "Telegram" в коде (вне lang.strings)

Файлы, где `u"Telegram*"` или `L"Telegram*"` zashито хардкодом:

| Файл:строка | Что | Severity | Действие |
|-------------|-----|----------|----------|
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/about_box.cpp:66` | `box->setTitle(u"Telegram Desktop"_q);` — заголовок диалога About | 🔴 | `u"BeHappy Desktop"_q` или `AppName.utf16()` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/intro/intro_start.cpp:27` | `setTitleText(rpl::single(u"Telegram Desktop"_q));` — заголовок intro-экрана | 🔴 | Тот же fix |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/window/main_window.cpp:850` | `setTitle((user.isEmpty() ? u"Telegram"_q : user) + added);` — заголовок главного окна Windows когда нет активного чата | 🔴 | `AppName.utf16()` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/window/notifications_manager_default.cpp:1006` | `TextWithEntities{ u"Telegram Desktop"_q }` — fallback title для notification c hide-name-and-photo | 🔴 | `TextWithEntities{ AppName.utf16() }` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/settings/sections/settings_notifications.cpp:495` | `auto notifyTitle = st::msgNameFont->elided(u"Telegram Desktop"_q, …)` — превью уведомления в Settings → Notifications | 🔴 | `AppName.utf16()` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/window/window_main_menu.cpp:387-388` | `u"Telegram Desktop"_q,` + `u"https://desktop.telegram.org"_q` — Burger menu → "Telegram Desktop" link | 🔴 | Заменить на behappy |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/_other/updater_win.cpp:310,314,494` | `wsprintf(nameStr, L"Telegram Desktop"); wsprintf(icongroupStr, L"Telegram Desktop"); …static const WCHAR *_programName = L"Telegram Desktop";` — апдейтер пишет ярлык в Start Menu как "Telegram Desktop" | 🔴 | `L"BeHappy Desktop"` (но апдейтер всё равно отключён через `TDESKTOP_DISABLE_AUTOUPDATE`) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/_other/updater_win.cpp:312` | `wsprintf(publisherStr, L"Telegram FZ-LLC");` | 🔴 | Заменить |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/_other/updater_win.cpp:319` | `const WCHAR *appURL = L"https://desktop.telegram.org";` | 🔴 | Заменить |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/click_handler_types.cpp:137` | `"PRODID:-//Telegram Desktop//EN\r\n"` — vCard PRODID при экспорте контактов | 🟡 | `AppName` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/export/output/export_output_abstract.cpp:170` | `session.applicationName = "Telegram Desktop";` — имя приложения в JSON/HTML экспорте чатов | 🟡 | `AppName.utf8()` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/crash_report_window.cpp:440,458` | `QDesktopServices::openUrl(u"https://desktop.telegram.org"_q);` + dialog title `u"Telegram Crash Report"_q` + file extension `.telegramcrash` | 🟡 | Заменить URL+extension (`telegramcrash` встречается ещё в файловых ассоциациях ОС) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/sandbox.cpp:320` | `LOG(("This is the only instance of Telegram, starting server and app..."));` | 🟢 | Косметика |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/launcher.cpp:416,424` | `DEBUG_LOG(("Telegram finished, result: %1"))`, `DEBUG_LOG(("Sandbox Info: executing Telegram because of restart."))` | 🟢 | Косметика |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/api/api_updates.cpp:516` | `LOG(("API Error: updates.differenceTooLong is not supported by Telegram Desktop!"));` | 🟢 | Косметика |

`AppName` / `AppFile` уже определены как BeHappy в `core/version.h:23-24`, так что замена везде = одинаковое поведение и поддержка ребрендинга в одном месте.

---

## 4. Категория 2. Идентификаторы приложения (AppUserModelID, MSIX, .plist, .desktop, URL-схемы)

### 4.1. Windows — AppUserModelID

**Главный найденный leak (точно совпадает с симптомом юзера):**

| Файл:строка | Текущее значение | Что это |
|-------------|------------------|---------|
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/win/windows_app_user_model_id.cpp:29` | `const WCHAR AppUserModelIdBase[] = L"Telegram.TelegramDesktop.Store";` (UWP) | Базовое имя для group id |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/win/windows_app_user_model_id.cpp:31` | `const WCHAR AppUserModelIdBase[] = L"Telegram.TelegramDesktop";` (non-store) | Это значение ЛОГгируется на старте: `LOG(("AppUserModelID: %1"))` (`specific_win.cpp:378`) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/win/windows_app_user_model_id.cpp:348-349` | `const auto installed = u"Telegram Desktop/Telegram.lnk"_q;` + `const auto old = u"Telegram Win (Unofficial)/Telegram.lnk"_q;` | Поиск ярлыка по фиксированному имени |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/win/windows_app_user_model_id.cpp:216,348,361,370` | `Telegram.lnk`, `TelegramAlpha.lnk` | Имена .lnk файлов в Start Menu |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/win/specific_win.cpp:323,326` | `userDesktopLnk = ... + "\\Telegram.lnk";` `commonDesktopLnk = ... + "\\Telegram.lnk";` | Desktop shortcut path |

🔴 **Severity:** Critical. AppUserModelID использует Windows для: (a) group icons in taskbar, (b) jump lists, (c) toast notifications, (d) pin-to-start. Сейчас BeHappy будет группироваться с настоящим Telegram'ом если они оба установлены. Менять на `L"BeHappy.BeHappyDesktop"` и пересобирать.

**Замечание:** в `Id()` (`windows_app_user_model_id.cpp:469-500`) для portable-builds к base добавляется хеш `instanceHash()` — отсюда `.<hash>` в логе юзера. Логика правильная, надо менять только base-string.

### 4.2. Windows — MSIX / UWP (Microsoft Store)

Файл `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/uwp/AppX/AppxManifest.xml`:

```xml
<Identity Name="TelegramMessengerLLP.TelegramDesktop"
  ProcessorArchitecture="ARCHITECTURE"
  Publisher="CN=536BC709-8EE1-4478-AF22-F0F0F26FF64A"
  Version="6.7.8.0" />
<Properties>
  <DisplayName>Telegram Desktop</DisplayName>
  <PublisherDisplayName>Telegram Messenger LLP</PublisherDisplayName>
  <Description>Telegram Desktop official messenger</Description>
  ...
</Properties>
...
<Application Id="Telegram.TelegramDesktop.Store" Executable="Telegram.exe" ...>
  <uap:VisualElements ... DisplayName="Telegram Desktop" Description="Telegram Desktop official messenger" />
  ...
  <desktop:StartupTask TaskId="TelegramStartupTask" Enabled="false" DisplayName="Telegram Desktop" />
```

🔴 **Severity:** Critical (блокер для Microsoft Store). 100% Telegram-брендинг + чужой Publisher CN. Microsoft не пропустит. Все 8 полей надо заменить.

| Поле | Действие |
|------|----------|
| `Identity Name` | `BeHappyApp.BeHappyDesktop` |
| `Publisher` (CN) | Подписать своим cert'ом из Microsoft Partner Center |
| `DisplayName` (Properties + VisualElements) | `BeHappy Desktop` |
| `PublisherDisplayName` | BeHappy реальное юр-лицо |
| `Description` | "BeHappy Desktop" |
| `Application Id` | `BeHappy.BeHappyDesktop.Store` |
| `Executable` | `BeHappy.exe` (требует ребилда — см. категорию 6) |
| `TaskId` | `BeHappyStartupTask` |

### 4.3. Windows — .rc и .manifest

| Файл:строка | Что | Severity |
|-------------|-----|----------|
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/winrc/Telegram.rc:63` | `VALUE "CompanyName", "Telegram FZ-LLC"` | 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/winrc/Telegram.rc:64` | `VALUE "FileDescription", "Telegram Desktop"` | 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/winrc/Telegram.rc:65` | `VALUE "FileVersion", "6.7.8.0"` — UPSTREAM version, не 0.1.1! | 🟡 (auto-fill из `Telegram/build/version` — нужно проверить, реально ли это пересобирается) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/winrc/Telegram.rc:66` | `VALUE "LegalCopyright", "Copyright (C) 2014-2026"` | 🟡 — добавить BeHappy copyright |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/winrc/Telegram.rc:67` | `VALUE "ProductName", "Telegram Desktop"` | 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/winrc/Updater.rc:54,55,58` | то же самое для Updater.exe | 🔴 (хотя апдейтер отключён) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/winrc/Telegram.manifest` | Только windowsSettings (DPI). Без бренда. | ✅ ok |

**Эти поля попадают в свойства exe-файла** — пользователь видит их в Properties → Details в Explorer'е, а Windows SmartScreen + AV сравнивают `CompanyName` с подписью.

### 4.4. macOS — Info.plist + bundle id

Файл `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Telegram.plist`:

| Строка | Поле | Текущее | Действие |
|--------|------|---------|----------|
| `8` | `CFBundleGetInfoString` | `Telegram Desktop messaging app` | 🔴 заменить |
| `14, 37` | `CFBundleIdentifier`, `CFBundleURLName` | `@bundle_identifier_plist@` — берётся из CMake | См. `CMakeLists.txt:2077,2097` ниже |
| `40-41` | `CFBundleURLSchemes` | `tg`, `tonsite` | 🟡 решить: оставить `tg` для обратной совместимости deep-links / сменить |
| `72-74` | NSMicrophoneUsageDescription, NSCameraUsageDescription, NSLocationUsageDescription | "We need access to your microphone…" | 🟢 нейтральные, можно оставить, но желательно `BeHappy needs access…` |

Файл `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/CMakeLists.txt`:

| Строка | Что | Severity |
|--------|-----|----------|
| `2077` | `set(bundle_identifier "org.telegram.desktop")` (Mac App Store) | 🔴 |
| `2095, 2097` | `set(bundle_identifier "com.tdesktop.Telegram$<...>")` (non-MAS) | 🔴 |
| `2078` | `set(bundle_entitlements "Telegram Lite.entitlements")` | 🔴 (имя файла + содержимое) |
| `2079` | `set(output_name "Telegram Lite")` | 🔴 |
| `2099-2100` | `set(bundle_entitlements "Telegram.entitlements")` + `set(output_name "Telegram")` | 🔴 |

Файлы `Telegram/Telegram/Telegram.entitlements`, `Telegram/Telegram/Telegram Lite.entitlements`, `Telegram/Telegram/Breakpad.entitlements` — внутри только Apple security keys (`com.apple.security.device.audio-input` и т.п.), бренд не указан. **Но имена файлов** надо менять синхронно с `output_name` (иначе CMake не найдёт после переименования).

`Telegram.plist:18` `CFBundleName` берётся из `@output_name@` = `"Telegram"` или `"Telegram Lite"`. После замены `output_name` → `"BeHappy"` всё подтянется. **Но `output_name="Telegram"` хардкодом сидит в build-скриптах и .github/workflows/ — см. категорию 6.**

### 4.5. Linux — .desktop + AppStream metainfo + D-Bus service

Файлы `lib/xdg/org.telegram.desktop.*`:

`F:/test_cl/split/ansible-desktop/app-desktop/lib/xdg/org.telegram.desktop.desktop`:
```ini
Name=Telegram                                  # 🔴 строка 2
Comment=New era of messaging                   # 🔴 (нейтрально, но переписать)
TryExec=Telegram                               # 🔴 строка 4 (если бинарь = Telegram)
Exec=Telegram -- %U                            # 🔴 строка 5
Icon=org.telegram.desktop                      # 🔴 строка 6
StartupWMClass=TelegramDesktop                 # 🔴 строка 8 (должна совпадать с Qt setApplicationName!)
MimeType=x-scheme-handler/tg;...               # 🟡 строка 11 (URL scheme)
Keywords=tg;chat;im;messaging;messenger;sms;tdesktop;  # 🔴 строка 12
Exec=Telegram -quit                            # 🔴 строка 20 (Quit action)
```

`F:/test_cl/split/ansible-desktop/app-desktop/lib/xdg/org.telegram.desktop.metainfo.xml` — **полный telegram-описательный XML на 116 строк**, включая:
- `<id>org.telegram.desktop</id>` (строка 3)
- `<name>Telegram</name>` (строка 6)
- 10+ параграфов `<description>` рассказывающих про Telegram FZ-LLC, MTProto, "world's top 10 most downloaded apps" и т.д.
- `<developer><name>Telegram FZ-LLC</name>` (строка 26)
- 6 ссылок на `desktop.telegram.org`, `telegram.org/faq`, `translations.telegram.org`, `github.com/telegramdesktop/tdesktop` (строки 27-32)
- 5 `<screenshot>` URL'ов на `raw.githubusercontent.com/telegramdesktop/tdesktop/dev/docs/assets/*.png` (строки 38-56)
- `<binary>Telegram</binary>` + `<dbus>org.telegram.desktop</dbus>` (строки 111-112)

🔴 **Severity:** Critical. Это файл, который GNOME Software / KDE Discover / Flathub / Snap Store используют для отображения карточки приложения. Сейчас он на 100% Telegram-овский. **Все три файла надо переписать с нуля + переименовать (`org.behappy.desktop.*`).**

`F:/test_cl/split/ansible-desktop/app-desktop/lib/xdg/org.telegram.desktop.service`:
```ini
Name=org.telegram.desktop                                       # 🔴
Exec=@CMAKE_INSTALL_FULL_BINDIR@/Telegram                       # 🔴
```

Также: `Telegram/SourceFiles/platform/linux/specific_linux.cpp` использует `u"org.telegram.desktop._%1"_q` (строка 741, 745) как D-Bus name + `u":/misc/org.telegram.desktop.desktop"_q` (строка 237) — references к .desktop name. Менять синхронно.

### 4.6. URL-схемы (`tg://`, `tonsite://`, `telegram://`)

| Файл:строка | Что | Severity |
|-------------|-----|----------|
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/application.cpp:1889-1898` | Регистрация URL-scheme `tg` с описанием `"Telegram Link"`, `shortAppName = u"tdesktop"_q` | 🟡 решить |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/application.cpp:1900-1909` | Регистрация URL-scheme `tonsite` с описанием `"TonSite Link"` | 🟡 не нужна BeHappy (TON выпилили) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/local_url_handlers.cpp` | 12+ мест с `u"tg://resolve"`, `u"tg://join"`, и т.д. | 🟡 в продукт-плане есть выбор: оставить `tg://` для совместимости с экспортированными ссылками OR перейти на `bh://` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/api/api_single_message_search.cpp:41,225` | `if (check.startsWith(u"tg://resolve"_q, Qt::CaseInsensitive))` | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/inline_bots/bot_attach_web_view.cpp:971` | `const auto prefix = u"tg://resolve?"_q;` | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/local_url_handlers.cpp:575,2033` | `if (domain == u"telegrampassport"_q)` — Telegram Passport flow | 🟡 (passport удалён из BeHappy, но обработчик остался) |

**Рекомендация:** оставить `tg://` как working URL scheme (60+ мест), но переименовать `protocolName = u"Telegram Link"_q` (`core/application.cpp:1893,1894`) → `u"BeHappy Link"_q` + `shortAppName = u"tdesktop"_q` → `u"behappydesktop"_q`. Это поменяет только описание в Windows-registry / Linux-mimeapps, но не сломает уже-существующие `tg://`-ссылки.

`tonsite://` (Open Network TON) использовался Telegram-Fragment'ом — для BeHappy не нужен, можно убрать всю секцию (`application.cpp:1900-1909`, `Telegram.plist:41`).

---

## 5. Категория 3. Имя папки данных

**Симптом юзера:** `%APPDATA%\Telegram Desktop\`.

**Root cause analysis:**

- `Telegram/SourceFiles/core/version.h:23`: `constexpr auto AppName = "BeHappy Desktop"_cs;` — поменяно коммитом `a591ec9`.
- `Telegram/SourceFiles/platform/win/specific_win.cpp:244`: `return appData.absolutePath() + '/' + AppName.utf16() + '/';` — **корректно** использует `AppName` (т.е. после ребилда станет `%APPDATA%\BeHappy Desktop\`).
- `Telegram/SourceFiles/platform/win/specific_win.cpp:255`: `psAppDataPathOld()` использует `AppNameOld.utf16()` = `"BeHappy Desktop"` тоже — что неверно для миграции. **Должно быть `AppNameOld = "Telegram Desktop"`**, чтобы при первом запуске нового бинарника старая папка `%APPDATA%\Telegram Desktop\` была найдена и данные перекатились (см. как в upstream). Сейчас `AppName == AppNameOld == "BeHappy Desktop"` — миграции не будет, пользователи с реальным Telegram-account'ом сольют его, что НЕ страшно (BeHappy на другом backend'е), но если у юзера БЫЛ запущен старый BeHappy build с `AppName="Telegram Desktop"`, его данные потеряются.

**Гипотеза, почему симптом всё ещё есть:**
1. Самое вероятное: **развёрнутый бинарь был собран ДО `a591ec9`** (т.е. со старым AppName="Telegram Desktop"). Пересборка устранит симптом.
2. Альтернатива: у юзера старая папка с данными из настоящего Telegram, и установщик их не трогает (правильное поведение upstream'а).

| Файл:строка | Уточнение | Действие |
|-------------|-----------|----------|
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/version.h:22` | `constexpr auto AppNameOld = "BeHappy Desktop"_cs;` | 🟡 поменять на `"Telegram Desktop"` для миграции; либо явно ввести `AppNameMigrateFrom` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/launcher.cpp:338` | `QApplication::setApplicationName(u"TelegramDesktop"_q);` | 🟡 поменять на `u"BeHappyDesktop"_q`. Qt-uses for `QStandardPaths`, `QSettings`, и крах-логи. **На Windows реально влияет:** если код где-то вызывает `QStandardPaths::writableLocation(AppLocalDataLocation)`, оно ВКЛЮЧАЕТ `applicationName` в путь (по умолчанию `%LOCALAPPDATA%/<orgName>/<appName>/`) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/launcher.cpp:216` | `cExeDir() + u"TelegramForcePortable"_q;` — маркер portable-режима | 🟡 переименовать, но **сломает обратную совместимость** для existing portable-installs (если они есть) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/launcher.cpp:237-238` | `MoveLegacyAlphaFolder(u"TelegramAlpha_data"_q, u"alpha"_q)` + `MoveLegacyAlphaFolder(u"TelegramBeta_data"_q, u"beta"_q)` | 🟢 legacy migration, можно оставить как есть |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/win/specific_win.cpp:242` | `return appData.absolutePath() + u"/Telegram Desktop UWP/"_q;` (только OS_WIN_STORE) | 🔴 для UWP-builds менять на `"BeHappy Desktop UWP"` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/linux/specific_linux.cpp:689-700` | Linux ищет `~/.TelegramDesktop/` для миграции legacy data | 🟢 можно оставить (миграция со старого Linux-tdesktop) |

**Миграционный план для existing BeHappy users (если такие были):** написать спец-логику в `Application::start()`, которая проверит наличие `%APPDATA%\Telegram Desktop\tdata\settings*` и `%APPDATA%\BeHappy Desktop\tdata\settings*`, и если ТОЛЬКО первая существует — скопирует/переедет. Без этого юзеры будут "потерять" свои чаты при upgrade.

---

## 6. Категория 4. Иконки и изображения

### 6.1. Что уже заменено (по содержимому коммитов `0e616d6`, `b7c0f99`, `a591ec9`)

**Заменены на BeHappy-логотипы (бесконечность + speech bubble):**
- `Telegram/Resources/art/icon{16,32,48,64,128,256,512}{@2x}.png` (12 files)
- `Telegram/Resources/art/icon_round512@2x.png`
- `Telegram/Resources/art/icon256.ico` (Windows multi-res .ico, замена в a591ec9)
- `Telegram/Resources/art/icon_green.png`, `iconbig_green.png` (alpha-channel green variants)
- `Telegram/Resources/icons/mac_tray_icon{,@2x,@3x}.png` (mac tray PNGs)
- `Telegram/Resources/icons/tray_monochrome{,_attention,_mute}.svg` (Linux/Win tray SVGs — speech bubble + infinity, verified by reading content)
- `Telegram/Telegram/Images.xcassets/AppIcon.appiconset/icon{16,32,128,256,512}{@2x}.png` (10 macOS Mac App Store icons)
- `Telegram/Telegram/Images.xcassets/Icon.iconset/icon_{16x16,32x32,128x128,256x256,512x512}{@2x}.png` (10 macOS .iconset icons)

### 6.2. 🔴 Что НЕ заменено и всё ещё показывает Telegram paper-plane

| Файл | Где используется | Действие |
|------|------------------|----------|
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/icons/intro_plane_inner.png` (+ `@2x`, `@3x`) | Welcome-экран при первом запуске, анимация bubble с самолётиком | 🔴 заменить (intro animation) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/icons/intro_plane_outer.png` (+ `@2x`, `@3x`) | Тот же экран | 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/icons/intro_plane_top.png` (+ `@2x`, `@3x`) | Тот же экран | 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/icons/intro_plane_trace.png` (+ `@2x`, `@3x`) | Тот же экран (след самолёта) | 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/icons/intro_qr_plane.png` (+ `@2x`, `@3x`) | QR-login экран — самолётик в углу | 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/icons/plane_white.svg` | **Подтверждено по содержимому:** classic Telegram paper-plane SVG (см. path coordinates). Используется в `Telegram/SourceFiles/ui/boxes/peer_qr_box.cpp:887` — на QR-коде для chat link | 🔴 заменить или удалить из QR-генератора |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/icons/folders/folders_airplane.png` (+ `@2x`, `@3x`) | Иконка папки "Travel" | 🟡 нейтральная (просто самолёт, не Telegram-logo). Можно оставить. |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/art/affiliate_logo.png` | Affiliate program promo | 🟡 проверить визуально — название "affiliate_logo" подозрительное |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/art/business_logo.png` | "Telegram Business" promo | 🟡 Business feature выпилен → проверить |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/art/logo_256.png`, `logo_256_no_margin.png` | Splash / Welcome cover (большие хедер-логотипы) | 🟡 имя `logo_*` подозрительное, проверить визуально (МОЖЕТ быть BeHappy-логотип ИЛИ исходный Telegram-логотип) |

**📋 Список ALL PNG/SVG/TIFF файлов в `Telegram/Resources/art/`** (на случай ручной визуальной инспекции):
```
affiliate_logo.png, bg_initial.jpg, bg_thumbnail.png, business_logo.png,
cocoon.webp, icon128.png, icon128@2x.png, icon16.png, icon16@2x.png,
icon256.ico, icon256.png, icon256@2x.png, icon32.png, icon32@2x.png,
icon48.png, icon48@2x.png, icon512.png, icon512@2x.png, icon64.png,
icon64@2x.png, icon_green.png, icon_round512@2x.png, iconbig_green.png,
logo_256.png, logo_256_no_margin.png, mac_setup.tiff,
round_placeholder.jpg, themeimage.jpg, verified_bg.webp, verified_fg.webp,
background.tgv
```

⚠️ `mac_setup.tiff` — это macOS DMG-installer background картинка. С большой вероятностью на ней изображены: Telegram-логотип + стрелка "Drag to Applications". 🔴 **Critical если делается DMG-релиз.**

⚠️ `cocoon.webp` — некий "cocoon" mascot/branding для Telegram Translate. Удалить (BeHappy не использует cocoon).

⚠️ `verified_bg.webp`, `verified_fg.webp` — фоны для "Verified by Telegram" значка. 🟡 проверить (в BeHappy нет верификации центральным сервером Telegram).

### 6.3. UWP/Store иконки

| Файл | Содержание | Действие |
|------|------------|----------|
| `Telegram/Resources/uwp/AppX/Assets/logo/logo.scale-{100,125,150,200,400}.png` (5 files) | Microsoft Store store-listing logo | 🔴 заменить (вероятно Telegram-логотип) |
| `Telegram/Resources/uwp/AppX/Assets/logo150/logo150.scale-*.png` (5) | Square150x150Logo (tile средний) | 🔴 заменить |
| `Telegram/Resources/uwp/AppX/Assets/logo44/logo44.scale-*.png` (5) + `logo44.targetsize-*.png` (30) | Square44x44Logo (icon во всех scales) | 🔴 заменить 35 файлов |

**Коммит `b7c0f99` / `0e616d6` UWP assets НЕ трогали** — только `art/icon*.png` и macOS iconset. UWP store icons остались Telegram'овские. Поскольку UWP/MSIX обновлять надо в связке с `AppxManifest.xml` (категория 2.2), это один большой кластер.

### 6.4. Theme files

Файлы `*.tdesktop-theme`:
- `Telegram/Resources/day-blue.tdesktop-theme`
- `Telegram/Resources/day-custom-base.tdesktop-theme`
- `Telegram/Resources/night.tdesktop-theme`
- `Telegram/Resources/night-custom-base.tdesktop-theme`
- `Telegram/Resources/night-green.tdesktop-theme`

🟡 Сам формат `.tdesktop-theme` — это zip-архив с `colors.tdesktop-palette` + опциональным `background.jpg/png`. Названия файлов содержат `tdesktop` (расширение зашито в код: `Telegram/SourceFiles/core/mime_type.cpp:59-118` + `Telegram/SourceFiles/data/data_document.cpp:1793` + `boxes/background_box.cpp:243`). Менять расширение → ломать совместимость со всеми существующими user themes. **Рекомендация: оставить как есть.**

---

## 7. Категория 5. URL'ы и домены, hard-coded в исходниках

Не считая lang.strings (раздел 3.1), в `.cpp`/`.h`/`.mm`/`.style` найдено **26 hard-coded URL'ов** на Telegram-инфраструктуру:

| Файл:строка | URL | Severity | Действие |
|-------------|-----|----------|----------|
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/about_box.cpp:39` | `https://core.telegram.org/api` | 🔴 в About-диалоге | заменить или удалить ссылку |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/about_box.cpp:48` | `https://github.com/telegramdesktop/tdesktop/blob/master/LICENSE` | 🔴 в About | `https://github.com/ansible-desktop/tdesktop/blob/master/LICENSE` (или указать оба) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/about_box.cpp:52` | `https://github.com/telegramdesktop/tdesktop` | 🔴 в About | `https://github.com/ansible-desktop/tdesktop` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/about_box.cpp:85` | `https://tdesktop.com/` (Alpha-download URL) | 🟡 alpha disabled — но строка остаётся в бинаре |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/about_box.cpp:135` | `https://telegram.org/faq` (returned by `telegramFaqLink()`) | 🔴 — FAQ ссылка |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/application.cpp:1185` | `https://telegramdesktop.github.io/tdesktop/changelog/` — changelog URL | 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/update_checker.cpp:1647` | `https://desktop.telegram.org` | 🟡 update checker disabled, но строка остаётся |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/crash_report_window.cpp:440` | `https://desktop.telegram.org` (в crash dialog) | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/window/window_main_menu.cpp:388` | `https://desktop.telegram.org` (Burger menu link) | 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/_other/updater_win.cpp:319` | `https://desktop.telegram.org` (updater) | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/iv/iv_prepare.cpp:765` | `https://t.me/` + username (для генерации share-ссылок на чужого юзера) | 🟡 это logical link, должен указывать на behappy.chat |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/dialogs/ui/dialogs_suggestions.cpp:2962` | `https://t.me/botfather` | 🟡 BotFather только в Telegram'е, в BeHappy свой |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/dialogs/dialogs_top_bar_suggestion.cpp:342` | `https://t.me/premiumbot?start=status` | 🟡 (Premium не используется) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/settings/sections/settings_premium.cpp:1871` | `https://t.me/$" + slug` | 🟡 (Premium удалён) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/sticker_set_box.cpp:887` | `https://t.me/stickers` | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/peers/create_managed_bot_box.cpp:354` | `https://t.me/botfather?start=deletebot` | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/peers/edit_peer_reactions.cpp:943` | `https://telegram.org/tos/stars` | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/choose_filter_box.cpp:492` | `text.startsWith(u"https://t.me/"_q)` (validation) | 🟡 — должна быть `behappy.chat` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/language_box.cpp:515` | `https://t.me/setlanguage/` + lang.id | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/chat_helpers/message_field.cpp:543` | `url.startsWith(u"t.me/"_q) \|\| url.startsWith(u"https://t.me/"_q)` (link-detection regex) | 🟡 internalLinksDomain в `mtproto_config.h` стоит `behappy.chat`, но это hard-coded check не использует config |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/chat_helpers/message_field.cpp:1553` | `tr::link(u"@SpamBot"_q, info.appealUrl)` — антиспам Telegram'а | 🟡 (если BeHappy свой SpamBot — заменить) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/history/history_item.cpp:5895` | `u"internal:url:https://t.me/c/%1/%2"_q` (deep-link на topic) | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/history/view/history_view_element.cpp:1667` | то же | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/history/admin_log/history_admin_log_item.cpp:751` | то же | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/history/history_item_helpers.cpp:1310,1329` | `u"https://desktop.telegram.org"_q` + `QByteArray("https://desktop.telegram.org")` | 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/main/main_session.cpp:74` | `// Like 'https://telegram.me/' or 'https://t.me/'.` (комментарий) | 🟢 |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/export/output/export_output_html.cpp:171` | `const auto domain = QByteArray("https://telegram.org/");` (в HTML-export шапке) | 🟡 экспорт чатов |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/data/data_passkey_deserialize.cpp:26` | `obj["origin"] = "https://telegram.org";` — origin для passkey/webauthn | 🔴 (passkey будет работать только для telegram.org) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/ui/boxes/about_cocoon_box.cpp:188` | `u"tg://resolve?domain="_q + username` | 🟡 (cocoon — Telegram-feature, в BeHappy не используется) |

**Best practice:** хардкод `https://t.me/` в 8+ местах надо вытащить в `Telegram/SourceFiles/mtproto/mtproto_config.h` через тот же механизм `internalLinksDomain` (уже = `https://behappy.chat/`). Сейчас config есть, но не везде используется.

**Telegram-controlled IPs:** ✅ ни одного. `mtproto_dc_options.cpp:35-49` уже использует BeHappy DC1 `5.129.243.207:10443` (см. коммит `30a6fc8`). RSA keys в `kPublicRSAKeys[]` — собственные BeHappy ключи (commit `a560f6d`).

---

## 8. Категория 6. Build / CI / Installer

### 8.1. GitHub Actions

| Файл | Что | Severity |
|------|-----|----------|
| `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/winget.yml:10,12,16,18` | `uses: telegramdesktop/winget-releaser@main` + `identifier: Telegram.TelegramDesktop` (Beta тоже) | 🔴 публикует BeHappy под Telegram-identifier в winget. Удалить файл или поменять identifier |
| `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/win.yml:156,166,170,177,188,192` | `ARTIFACT_NAME="Telegram"` + suffixes (`Telegram x64`, `Telegram x64 6.7`, и т.д.) | 🟡 артефакты в Actions UI будут называться "Telegram x64" |
| `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/win.yml:205,208,229` | Build step name = "Telegram Desktop build.", `cd ...\Telegram`, `move Telegram.exe artifact/` | 🟡 — но это идёт из CMake (`add_executable(Telegram ...)`), см. ниже |
| `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/linux.yml:59` | `IMAGE_TAG: tdesktop:centos_env` | 🟢 build-internal |
| `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/linux.yml:106,113,115` | `name: Telegram Desktop build.` + `ARTIFACT_NAME=Telegram` | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/linux.yml:120,123` | `-v $PWD:/usr/src/tdesktop` + `/usr/src/tdesktop/Telegram/build/docker/centos_env/build.sh` | 🟢 (mount path, internal) |
| `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/mac.yml:96,105,107` | `name: Telegram Desktop build.` + `ARTIFACT_NAME=Telegram` | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/mac_packaged.yml:153` | `name: Telegram Desktop build.` | 🟡 |
| `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/snap.yml:71,77` | `name: Telegram Desktop snap build.` + `artifact_name=$(echo telegram-desktop_*.snap)` | 🟡 |

### 8.2. CMake (где зашит target name "Telegram")

| Файл:строка | Что |
|-------------|-----|
| `F:/test_cl/split/ansible-desktop/app-desktop/CMakeLists.txt:19` | `project(Telegram ...)` |
| `F:/test_cl/split/ansible-desktop/app-desktop/CMakeLists.txt:22` | `DESCRIPTION "Official Telegram Desktop messenger"` 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/CMakeLists.txt:23` | `HOMEPAGE_URL "https://desktop.telegram.org"` 🔴 |
| `F:/test_cl/split/ansible-desktop/app-desktop/CMakeLists.txt:36` | `VS_STARTUP_PROJECT Telegram` |
| `F:/test_cl/split/ansible-desktop/app-desktop/CMakeLists.txt:66` | `add_subdirectory(Telegram)` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/CMakeLists.txt:7` | `add_executable(Telegram WIN32 MACOSX_BUNDLE)` 🔴 **это даёт `Telegram.exe`** |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/CMakeLists.txt:2100` | `set(output_name "Telegram")` (non-MAS macOS) 🔴 |

**Action:** Менять `add_executable(Telegram ...)` → `add_executable(BeHappy ...)`, и `output_name "Telegram"` → `"BeHappy"`. **Это каскадно поменяет** все ссылки в CI (`Telegram.exe` → `BeHappy.exe`, `Telegram.app` → `BeHappy.app`). Но `cmake target name` "Telegram" встречается в 30+ местах внутри `Telegram/CMakeLists.txt` (`target_link_libraries(Telegram ...)`, `set_target_properties(Telegram ...)`) — это RENAME operation, не одну строчку.

### 8.3. Inno Setup installer (Windows)

Файл `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/build/setup.iss`:

```
1:  #define MyAppShortName "Telegram"            🔴 → "BeHappy"
2:  #define MyAppName "Telegram Desktop"         🔴 → "BeHappy Desktop"
3:  #define MyAppPublisher "Telegram FZ-LLC"     🔴 → BeHappy юр-лицо
4:  #define MyAppURL "https://desktop.telegram.org"  🔴 → behappy URL
5:  #define MyAppExeName "Telegram.exe"          🔴 → "BeHappy.exe"
6:  #define MyAppId "53F49750-6209-4FBF-9CA8-7A333C87D1ED"  🔴 → СГЕНЕРИРОВАТЬ НОВЫЙ GUID (этот совпадает с настоящим Telegram'ом!)
```

🔴 **Critical:** `MyAppId` — это Setup ID, по которому Windows определяет "это та же программа или другая". Сейчас он = upstream Telegram's GUID. Это значит: если у юзера установлен настоящий Telegram Desktop, и он запустит BeHappy-installer, Windows перезатрёт Telegram'у файлы или предложит "Repair". **Нужен НОВЫЙ GUID** (`{? GenerateGuid()}`).

Также: `setup.iss:73` `Source: "{#ReleasePath}\Telegram.exe"` — после переименования EXE надо менять.

### 8.4. Mac code signing

Файл `Telegram/build/build.sh:330-335`:
```bash
codesign --force --deep --timestamp --options runtime --sign "Developer ID Application: Telegram FZ-LLC (C67CF9S4VU)" "$ReleasePath/$BundleName"
codesign --force --timestamp --options runtime --sign "3rd Party Mac Developer Application: Telegram FZ-LLC (C67CF9S4VU)" ...
productbuild --sign "3rd Party Mac Developer Installer: Telegram FZ-LLC (C67CF9S4VU)" ...
```

🔴 **Critical:** Эти команды подписываются ключом Telegram FZ-LLC (TeamID `C67CF9S4VU`). У BeHappy этого ключа НЕТ → сборка macOS-релиза **технически невозможна** без своего Apple Developer ID. Action: записать свой TeamID или использовать env-vars (`$APPLE_TEAM_ID`).

### 8.5. Snap

Файл `F:/test_cl/split/ansible-desktop/app-desktop/snap/snapcraft.yaml`:
```yaml
1:  name: telegram-desktop                              🔴
2:  adopt-info: telegram                                🔴
3:  icon: Telegram/Resources/art/icon512@2x.png         ✅ (icon уже BeHappy)
...
11:   telegram-desktop:                                 🔴
12:     command: usr/bin/telegram-desktop               🔴
13:     common-id: org.telegram.desktop                 🔴
14:     desktop: usr/share/applications/org.telegram.desktop.desktop  🔴
15:     autostart: telegram-desktop_telegram-desktop.desktop  🔴
...
35:   telegram:                                         🔴
39:     parse-info: [usr/share/metainfo/org.telegram.desktop.metainfo.xml]  🔴
55:       - -DTDESKTOP_API_ID=611335                    🔴 (Telegram's API ID — должен быть BeHappy)
56:       - -DTDESKTOP_API_HASH=d524b414d21f4d37f08684c1df41ac9c  🔴 (Telegram's API hash)
73:       mv "$CRAFT_PART_INSTALL"/usr/bin/{Telegram,telegram-desktop}  🔴
75:       APP_ID=org.telegram.desktop                   🔴
```

🔴 **Critical:** Snap-релиз будет публиковаться под именем `telegram-desktop` в Snap Store — это **прямой конфликт** с реальным `telegram-desktop` snap от Canonical / Telegram.

### 8.6. Docker

`F:/test_cl/split/ansible-desktop/app-desktop/Telegram/build/docker/centos_env/Dockerfile` (используется в CI Linux build): тег по умолчанию `tdesktop:centos_env`. 🟢 internal build, можно оставить, но удобнее переименовать в `behappy:centos_env`.

### 8.7. Scripts (other)

| Файл:строка | Что | Severity |
|-------------|-----|----------|
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/build/release.py:252,284,301,310` | `requests.get(url + 'repos/telegramdesktop/tdesktop/releases/...')` — публикует GitHub releases в репозиторий `telegramdesktop/tdesktop`. **НЕ работает для BeHappy** (нет доступа к чужому репо) | 🔴 — но release.py явно не используется в текущей CI (есть свой `master_updater.yml`); рекомендуется удалить или переписать |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/build/build.sh:16`, `deploy.sh:12`, `mac_store_upload.sh:12`, `release.sh:17`, `updates.sh:12` | `echo "https://github.com/telegramdesktop/tdesktop/#build-instructions"` (usage error message) | 🟢 |
| `F:/test_cl/split/ansible-desktop/app-desktop/scripts/merge_translations.py:13-26` | BEHAPPY_EN_OVERRIDES + BEHAPPY_RU_OVERRIDES — текущий перевод только 4 ключа (`lng_open_from_tray`, `lng_quit_from_tray`, `lng_tray_icon_text`, `lng_intro_qr_step1`) | 🔴 этот скрипт **должен** покрыть все 322 EN/373 RU Telegram-mentioning ключа — сейчас покрывает 4 |

---

## 9. Категория 7. Copyright / Legal / README / CHANGELOG

### 9.1. Top-level legal files (уже сделаны коммитом `fd85f3b`)

| Файл | Состояние |
|------|-----------|
| `F:/test_cl/split/ansible-desktop/app-desktop/LEGAL` | ✅ полный фрак-attribution: "BeHappy Desktop is a derivative work based on Telegram Desktop", copyrights обоих, trademark notice. **GPL §6 compliant.** |
| `F:/test_cl/split/ansible-desktop/app-desktop/NOTICE` | ✅ 79 строк full attribution, GPL §6 source availability, trademark notice |
| `F:/test_cl/split/ansible-desktop/app-desktop/README.md` | ✅ перерасписан как fork-of-tdesktop |
| `F:/test_cl/split/ansible-desktop/app-desktop/CHANGELOG.md` | ✅ упоминает fork и инициальные изменения |
| `F:/test_cl/split/ansible-desktop/app-desktop/UPSTREAM_CHANGELOG.txt` | ✅ preserved (был `changelog.txt`) |
| `F:/test_cl/split/ansible-desktop/app-desktop/LICENSE` | ✅ unchanged (GPL v3 + OpenSSL exception, как и упомянуто в commit `fd85f3b`) |

### 9.2. Per-source-file copyright headers (🟡 не сделано)

Все `.cpp`/`.h`/`.mm`/`.style`/`.cmake` файлы в `Telegram/SourceFiles/`, `Telegram/cmake/`, `Telegram/CMakeLists.txt`, `CMakeLists.txt` начинаются с шапки:

```cpp
/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
```

**Количество файлов:** 2459 в `Telegram/SourceFiles/` + 12 в `cmake/` + 2 в корне ≈ **2473 файла**.

Коммит `fd85f3b` явно отметил это в commit message: "Per-file copyright headers in source still need update in a follow-up." — TODO висит, не выполнен.

**Рекомендация (один скриптовый sed-replace):**

```bash
# Заменить шапку на:
/*
This file is part of BeHappy Desktop, a fork of Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/tdesktop/blob/master/LEGAL
*/
```

🟢 Низкая срочность (legal compliance уже OK через top-level `LEGAL`+`NOTICE`), но желательно сделать одним коммитом для prod-релиза.

### 9.3. Documentation

| Файл | Состояние |
|------|-----------|
| `F:/test_cl/split/ansible-desktop/app-desktop/AGENTS.md` | 🟡 строка 1: `# Agent Guide for Telegram Desktop` — переписать на BeHappy. Строки 26-29, 65 ссылаются на "Telegram Desktop" |
| `F:/test_cl/split/ansible-desktop/app-desktop/CLAUDE.md` | ✅ просто pointer на AGENTS.md |
| `F:/test_cl/split/ansible-desktop/app-desktop/REVIEW.md` | 🟡 проверить |
| `F:/test_cl/split/ansible-desktop/app-desktop/docs/building-{linux,mac,win}.md` | 🟡 содержит ссылки на upstream tdesktop |
| `F:/test_cl/split/ansible-desktop/app-desktop/docs/api_credentials.md` | 🟡 проверить, нужен ли для BeHappy (API stubbed) |

---

## 10. Категория 8. Код-комментарии + внутренние символы

### 10.1. Логи (логируются в `log.txt` → могут попасть в bug-репорты)

🟡 **Visible to support:**

| Файл:строка | Что |
|-------------|-----|
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/win/specific_win.cpp:378` | `LOG(("AppUserModelID: %1").arg(appUserModelId));` — **именно эта строка** ловится в логе юзера = `Telegram.TelegramDesktop.<hash>` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/api/api_updates.cpp:516` | `LOG(("API Error: updates.differenceTooLong is not supported by Telegram Desktop!"));` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/sandbox.cpp:320` | `LOG(("This is the only instance of Telegram, starting server and app..."));` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/launcher.cpp:416` | `DEBUG_LOG(("Telegram finished, result: %1"))` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/launcher.cpp:424` | `DEBUG_LOG(("Sandbox Info: executing Telegram because of restart."))` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/mtproto/mtproto_dc_options.cpp:759,795` | `LOG(("MTP Error: in .tdesktop-endpoints expected ...")) ` |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/win/specific_win.cpp:477-478` | `L"Telegram autorun link.\n" "You can disable autorun in Telegram settings.")` — описание .lnk shortcut в autorun (видно в MSConfig) |
| `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/win/specific_win.cpp:711-712` | `L"Telegram send to link.\n" "You can disable send to menu item in Telegram settings.")` — описание ярлыка в SendTo |

### 10.2. Внутренние символы (только developer-facing)

🟢 **Low priority — нет необходимости менять для prod:**
- `tdesktop::` cmake-namespace в `Telegram/CMakeLists.txt:58-73` (target prefix для submodules: `tdesktop::lib_tgcalls`, `tdesktop::td_export`, и т.д.). Если переименовывать, надо ещё и в самих submodules — лучше не трогать.
- Identifier `TDESKTOP_*` defines: `TDESKTOP_DISABLE_AUTOUPDATE`, `TDESKTOP_API_ID`, `TDESKTOP_API_HASH`, `TDESKTOP_ALPHA_VERSION`, `TDESKTOP_USE_PACKAGED`, `TDESKTOP_REQUESTED_ALPHA_VERSION`, `TDESKTOP_ALLOW_CLOSED_ALPHA` — все internal compile-time флаги, видны только в build-time.
- Имена namespace'ов `Platform::`, `Core::`, и т.д. — uns specific to Telegram.
- File extensions `.tdesktop-theme`, `.tdesktop-palette`, `.tdesktop-endpoints` — менять = ломать user themes (см. раздел 6.4).

### 10.3. Подсчёт по grep

- **`Telegram\b` в исходниках вне copyright-шапки:** ≈ 137 матчей (после фильтра шапок).
- **`tdesktop\b` в исходниках:** ≈ 80 матчей (значительная часть — file extensions `.tdesktop-*`).
- **Всё в `Telegram/SourceFiles/`** (2459 файлов с шапками): "This file is part of Telegram Desktop" — **2459 матчей** (все файлы).

---

## 11. Сравнение с прошлыми ребрендинг-коммитами

### Что **уже** сделано прошлыми коммитами

| Commit | Что сделал |
|--------|-----------|
| **`7da05ed`** (Initial: BeHappy Desktop Client, 7da05ed) | Точка отсчёта — копия tdesktop. |
| **`9220392`** + **`d728017`** | Переключили git submodules на `ansible-desktop` org (32/32). |
| **`fd85f3b`** chore(legal): GPL v3 compliance | ✅ LEGAL + NOTICE + README + CHANGELOG переписаны. Per-file headers ОСТАВЛЕНЫ TODO. |
| **`0e616d6`** Replace app icons with BeHappy logo | ✅ 26 PNG/.ico/.iconset файлов (art/icon*, AppIcon.appiconset, Icon.iconset). |
| **`b7c0f99`** feat: replace Telegram icons with BeHappy branding | ✅ 18 файлов: tray SVG, mac tray PNGs, green icon variants, AppIcon.appiconset (повторно). |
| **`a591ec9`** port: BeHappy customizations | ✅ `core/version.h` (AppName/AppFile/AppNameOld), `config.h` (ApiId stub, internalLinksDomain, webFileDcId), `cmake/telegram_options.cmake`, mtproto DC config, intro.style tweaks, `Resources/langs/lang_ru.strings` (8984 строки RU, **но не переведены telegram→behappy**), `Resources/art/icon256.ico`, `scripts/*translations.py`. **Явно пропущено:** "skipped wholesale copies of history_widget.cpp / data_channel.cpp / history_view_top_bar_widget.cpp / lang.strings to avoid undoing tdesktop upstream improvements; the channel crash fix and lang.strings branding renames will be ported as targeted edits later." — **Эти targeted edits НЕ сделаны.** |
| **`e289201`** fix: QR code too small | minor, не относится к брендингу. |
| **`a560f6d`** + **`30a6fc8`** | MTProto RSA + DC IP swap → BeHappy infrastructure |
| **`fe054c9`**, **`e517f51`** | Version bumps |

### Что прошлые коммиты **пропустили** (= скоуп этого аудита)

1. **`lang.strings` (EN)** — 322 матча с "Telegram" в 320 ключах. *(Из commit `a591ec9`: "lang.strings branding renames will be ported as targeted edits later" — не сделано.)*
2. **`lang_ru.strings`** — 373 матча. *(скрипт `merge_translations.py` покрывает только 4 ключа.)*
3. **UWP assets**: 40+ PNG'ов в `Telegram/Resources/uwp/AppX/Assets/logo*/` — коммит `b7c0f99` НЕ трогал, всё ещё Telegram-логотипы.
4. **AppxManifest.xml** — 100% Telegram-овский (раздел 4.2).
5. **AppUserModelID** в `windows_app_user_model_id.cpp:29,31` — `L"Telegram.TelegramDesktop"`.
6. **macOS bundle identifier** в `CMakeLists.txt:2077,2097` — `org.telegram.desktop` / `com.tdesktop.Telegram`.
7. **Linux .desktop + AppStream metainfo + D-Bus service** в `lib/xdg/org.telegram.desktop.*` — 100% Telegram-овские.
8. **Inno Setup `setup.iss`** — Publisher, AppName, MyAppId (!), URL'ы.
9. **Windows .rc и Updater.rc** — CompanyName, ProductName.
10. **Plane icons** (`intro_plane_*`, `intro_qr_plane`, `plane_white.svg`) — классический Telegram paper-plane.
11. **Mac DMG installer background** `mac_setup.tiff` — вероятно с Telegram-логотипом.
12. **20+ build/CI скриптов** — `ARTIFACT_NAME="Telegram"`, `repo: telegramdesktop/tdesktop`, codesign Telegram-cert'ом.
13. **`snap/snapcraft.yaml`** — `name: telegram-desktop` (конфликт в Snap Store).
14. **26 hard-coded URL'ов** в .cpp файлах на `telegram.org`/`t.me`/`desktop.telegram.org`/`core.telegram.org`.
15. **18 hard-coded "Telegram Desktop"** строк в .cpp файлах (about box title, intro start title, main window title, updater, notifications, vCard, JSON export).
16. **Per-source-file copyright headers** в 2459 файлах — explicit TODO коммита `fd85f3b`.

### Что-то ли уже сделано, что упомянуто в README/CHANGELOG, но в коде НЕ отражено

- **`CHANGELOG.md:27`** говорит "Telegram-specific deep links (`tg://`, `t.me`) — Removed". В коде **НЕ удалены** (см. раздел 4.6 и 7).
- **`CHANGELOG.md:23-26`** говорит Premium / Stars / Fragment / Sponsored — "Removed". В коде секции UI **присутствуют** (`Telegram/SourceFiles/settings/sections/settings_premium.cpp`, `dialogs/dialogs_top_bar_suggestion.cpp:342`, и т.д.); lang.strings 118 ключей про Premium/Stars **присутствуют**. **README/CHANGELOG расходятся с реальностью кода.**

---

## 12. Приоритизированный чек-лист

Порядок: сверху вниз = критичность убывает. Эстимейты — для разработчика, знакомого с tdesktop-кодом (часы).

### 🔴 Phase A. Блокеры для prod-релиза под брендом BeHappy (≈30-40 ч)

| # | Кластер | Файлы | Описание | Эстимейт |
|---|---------|-------|----------|----------|
| A1 | **Renamer for Windows binary + AppUserModelID** | `Telegram/CMakeLists.txt:7,2100`, `Telegram/SourceFiles/platform/win/windows_app_user_model_id.cpp:29,31`, `Telegram/SourceFiles/platform/win/specific_win.cpp:323,326,477-478,711-712`, `Telegram/SourceFiles/_other/updater_win.cpp:310,312,314,319,494`, `Telegram/Resources/winrc/Telegram.rc:63-67`, `Telegram/Resources/winrc/Updater.rc:54-58`, `Telegram/build/setup.iss` (целиком, **+ новый GUID!**) | `Telegram.exe` → `BeHappy.exe` + AppUserModelID → `BeHappy.BeHappyDesktop` + Inno Setup полностью переписать (новый AppId, MyAppName, Publisher, URL'ы, MyAppExeName). Пересборка → юзер видит `BeHappy.exe` и `%APPDATA%\BeHappy Desktop\`. | 6-10 ч |
| A2 | **Linux .desktop + AppStream metainfo + D-Bus** | `lib/xdg/org.telegram.desktop.{desktop,metainfo.xml,service}` (rename + content rewrite), `Telegram/SourceFiles/platform/linux/specific_linux.cpp:237,355,376,385,444,693,741,745`, `Telegram/Resources/qrc/...` (qrc reference) | Переименовать всю тройку XDG-файлов → `org.behappy.desktop.*`, **полностью переписать** metainfo.xml (новый name, summary, description, developer, screenshots, branding colors, URLs, keywords, provides), `StartupWMClass=BeHappyDesktop`. | 4-6 ч |
| A3 | **macOS bundle identifier + output_name + entitlements rename** | `Telegram/CMakeLists.txt:2077-2114`, `Telegram/Telegram.plist:8`, `Telegram/Telegram/{Telegram,Telegram Lite,Breakpad}.entitlements` (rename files), `Telegram/build/build.sh:330-335` (codesign cert) | `org.telegram.desktop` → `org.behappy.desktop`, `com.tdesktop.Telegram` → `com.behappy.BeHappy`, output_name `Telegram`/`Telegram Lite` → `BeHappy`/`BeHappy Lite`, переименовать `.entitlements` файлы + поменять codesign Identity на свой Apple Developer ID. | 4-6 ч |
| A4 | **AppxManifest + UWP icons (Microsoft Store)** | `Telegram/Resources/uwp/AppX/AppxManifest.xml` (целиком), `Telegram/Resources/uwp/AppX/Assets/logo*/` (40+ PNG'ов) | Получить свой Microsoft Publisher CN, заменить Identity / DisplayName / Publisher / TaskId / Application Id, поменять Executable на `BeHappy.exe`, заменить все 40 store-icons. | 6-8 ч (без учёта дизайна icons) |
| A5 | **plane_white.svg + intro_plane_*.png + intro_qr_plane.png (Telegram paper-plane)** | `Telegram/Resources/icons/plane_white.svg`, `intro_plane_{inner,outer,top,trace}{,@2x,@3x}.png`, `intro_qr_plane{,@2x,@3x}.png` — итого 13 файлов | Заказать у дизайнера BeHappy-analogue paper-plane (или другой intro-animation), нарезать 1x/2x/3x для PNG + SVG, заменить. | 4-6 ч (без учёта дизайн-работы) |
| A6 | **lang.strings: top 30 most-visible ключей** | `Telegram/Resources/langs/lang.strings` (key list — см. таблицу в разделе 3.1) | Заменить вручную ключи `lng_intro_about`, `lng_open_from_tray`, `lng_quit_from_tray`, `lng_tray_icon_text`, `lng_settings_auto_start`, `lng_settings_add_sendto`, `lng_settings_auto_start_disabled_uwp`, `lng_passcode_about*`, `lng_proxy_unsupported`, `lng_theme_no_desktop`, `lng_about_text1_api`, `lng_about_text3_faq`, `lng_new_version_wrap`, `lng_outdated_now`, `lng_outdated_soon`, `lng_group_call_mac_access`, `lng_group_call_mac_screencast_access`, `lng_terms_delete_warning`, `lng_passport_*`, `lng_language_not_ready_about`, `lng_settings_passkeys_unsigned_error`, `lng_bot_share_location_unavailable`, `lng_message_unsupported`, `lng_intro_qr_step1`, `lng_intro_qr_title`, `lng_settings_logged_in_*`, `lng_settings_faq*`, `lng_settings_faq_link`, `lng_settings_features`. Заодно — для каждого синхронить `lang_ru.strings`. | 6-10 ч |

### 🟡 Phase B. Видимое в bug-репортах / при экспорте / saving (≈20-30 ч)

| # | Кластер | Действие | Эстимейт |
|---|---------|----------|----------|
| B1 | Hard-coded "Telegram Desktop" в .cpp | Заменить на `AppName.utf16()` или `AppName.utf8()` все 18 мест из таблицы 3.3 (`about_box.cpp:66`, `intro_start.cpp:27`, `main_window.cpp:850`, `notifications_manager_default.cpp:1006`, `settings_notifications.cpp:495`, `window_main_menu.cpp:387`, и т.д.). | 3-4 ч |
| B2 | Hard-coded URLs (`telegram.org`, `t.me`, `desktop.telegram.org`) в .cpp | Заменить 26 URL'ов из раздела 7 на BeHappy-эквиваленты. Часть удалить (Premium/Stars/Fragment), часть пропустить через `internalLinksDomain`. | 3-5 ч |
| B3 | Lang strings Premium/Stars/Sponsored cleanup | 118 ключей: заменить `Telegram Premium` → `Premium` / удалить разделы (если фичи реально не используются). Sync RU. | 4-6 ч |
| B4 | Lang strings все остальные ~150 Telegram-ссылок | Bulk replace: `Telegram` → `BeHappy`, `Telegram Desktop` → `BeHappy Desktop`, `telegram.org/*` → `ansible.su/*` (или удалить ссылку). Visual review каждой. Sync RU. | 6-10 ч |
| B5 | URL'ы в lang.strings (27) | Удалить ссылки на `telegram.org/tos`, `telegram.org/privacy`, `telegram.org/blog`, `ads.telegram.org` (нет BeHappy-аналогов). Заменить `telegram.org/faq` → `ansible.su/faq`. | 1-2 ч |
| B6 | CI workflow renames | `.github/workflows/{win,mac,linux,snap,mac_packaged}.yml` — заменить `ARTIFACT_NAME="Telegram"` → `"BeHappy"`, build-step names, и `IMAGE_TAG: tdesktop:centos_env` → `behappy:centos_env`. | 1-2 ч |
| B7 | Snap snapcraft.yaml | `snap/snapcraft.yaml` — переименовать `name: telegram-desktop` → `ansible-desktop`, `APP_ID=org.telegram.desktop` → `org.behappy.desktop`, удалить hardcoded `TDESKTOP_API_ID=611335`/`TDESKTOP_API_HASH=...` (это Telegram's API key — НЕЛЬЗЯ публиковать в snap под чужим брендом). | 1-2 ч |
| B8 | mac DMG background (`mac_setup.tiff`) + любые `affiliate_logo`/`business_logo`/`logo_256*` если на них Telegram-логотип | Manual visual inspection → пересоздать. | 2-4 ч (без дизайна) |
| B9 | Log messages | 8 строк (раздел 10.1) — заменить `"Telegram"` → `"BeHappy"` в LOG()/DEBUG_LOG()/wsprintf() | 1 ч |
| B10 | `data_passkey_deserialize.cpp:26` (passkey origin = telegram.org) | 🔴 если passkey-фича работает — заменить на `behappy.chat`. | 0.5 ч |
| B11 | Crash report extension `.telegramcrash` | `core/crash_report_window.cpp:458` — заменить на `.behappycrash` (или `.crash`) — менять и file association, и в exception handler. | 0.5 ч |
| B12 | Delete winget.yml OR re-target | `.github/workflows/winget.yml` — удалить целиком ИЛИ заменить identifier на BeHappy + поменять action `telegramdesktop/winget-releaser@main` на свой fork. | 0.5 ч |
| B13 | release.py хардкод | `Telegram/build/release.py:252,284,301,310` — либо переписать под BeHappy GitHub releases, либо удалить файл (CI его не использует). | 1 ч |

### 🟢 Phase C. Косметика / legal cleanup (≈15-20 ч)

| # | Кластер | Действие | Эстимейт |
|---|---------|----------|----------|
| C1 | Per-file copyright headers (2459 файлов) | Один скриптовый sed-replace: `This file is part of Telegram Desktop, the official desktop application for the Telegram messaging service.` → `This file is part of BeHappy Desktop, a fork of Telegram Desktop.` + URL `github.com/telegramdesktop/tdesktop` → `github.com/ansible-desktop/tdesktop`. **Одна команда:** `find Telegram/ -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.mm" -o -name "*.style" -o -name "*.cmake" \) -exec sed -i ...`. | 1-2 ч |
| C2 | `Telegram` folder rename | Структура папок (`Telegram/SourceFiles/`, `Telegram/Resources/`, `Telegram/CMakeLists.txt`) — оставить как есть (переименование сломает thousands of include paths, не оправдано). | 0 ч (do nothing) |
| C3 | `AGENTS.md`, `REVIEW.md`, `docs/building-*.md` rebrand | Заменить "Telegram Desktop" → "BeHappy Desktop" в текстах документации, актуализировать build-instructions URL'ы. | 2-3 ч |
| C4 | Cleanup `tonsite` scheme (TON не используется в BeHappy) | `core/application.cpp:1900-1909`, `Telegram.plist:41` — удалить tonsite registration. | 1 ч |
| C5 | `tdesktop::` cmake namespace | Оставить (рискованно менять, тянет за собой submodules) | 0 ч |
| C6 | Lang-helper scripts `scripts/merge_translations.py` | Дополнить `BEHAPPY_EN_OVERRIDES` / `BEHAPPY_RU_OVERRIDES` всеми ключами из Phase A6 + Phase B3,B4 — чтобы reproducible was on каждом upstream-merge. | 3-5 ч (по мере дописывания) |
| C7 | `cocoon.tgs`, `cocoon.webp`, `about_cocoon_box.cpp` (cocoon translate brand) | Удалить (BeHappy не использует Cocoon). | 1-2 ч |

### Полная оценка работы

- **Phase A (critical):** 30-46 часов
- **Phase B (visible):** 21-39 часов
- **Phase C (cosmetic):** 8-13 часов

**Итого: 60-100 человеко-часов** для полного клин-апа без учёта работы дизайнера на новые иконки/анимации/Mac DMG-background. С учётом дизайнерской работы — добавить +20-40 часов.

---

## 13. References

### Файлы, цитированные в отчёте

- `F:/test_cl/split/ansible-desktop/app-desktop/AGENTS.md`
- `F:/test_cl/split/ansible-desktop/app-desktop/CHANGELOG.md`
- `F:/test_cl/split/ansible-desktop/app-desktop/CLAUDE.md`
- `F:/test_cl/split/ansible-desktop/app-desktop/CMakeLists.txt`
- `F:/test_cl/split/ansible-desktop/app-desktop/LEGAL`
- `F:/test_cl/split/ansible-desktop/app-desktop/NOTICE`
- `F:/test_cl/split/ansible-desktop/app-desktop/README.md`
- `F:/test_cl/split/ansible-desktop/app-desktop/lib/xdg/org.telegram.desktop.desktop`
- `F:/test_cl/split/ansible-desktop/app-desktop/lib/xdg/org.telegram.desktop.metainfo.xml`
- `F:/test_cl/split/ansible-desktop/app-desktop/lib/xdg/org.telegram.desktop.service`
- `F:/test_cl/split/ansible-desktop/app-desktop/snap/snapcraft.yaml`
- `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/winget.yml`
- `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/win.yml`
- `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/linux.yml`
- `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/mac.yml`
- `F:/test_cl/split/ansible-desktop/app-desktop/.github/workflows/snap.yml`
- `F:/test_cl/split/ansible-desktop/app-desktop/scripts/merge_translations.py`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Telegram.plist`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/CMakeLists.txt`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/winrc/Telegram.rc`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/winrc/Updater.rc`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/winrc/Telegram.manifest`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/uwp/AppX/AppxManifest.xml`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/langs/lang.strings`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/langs/lang_ru.strings`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/icons/plane_white.svg`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/icons/tray_monochrome.svg`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Resources/qrc/telegram/telegram.qrc`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/version.h`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/config.h`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/launcher.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/application.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/local_url_handlers.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/sandbox.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/click_handler_types.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/core/crash_report_window.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/boxes/about_box.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/intro/intro_start.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/intro/intro.style`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/window/main_window.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/window/window_main_menu.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/window/notifications_manager_default.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/settings/sections/settings_notifications.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/export/output/export_output_abstract.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/export/output/export_output_html.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/data/data_passkey_deserialize.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/_other/updater_win.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/api/api_updates.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/mtproto/mtproto_dc_options.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/mtproto/mtproto_config.h`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/win/windows_app_user_model_id.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/win/specific_win.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/SourceFiles/platform/linux/specific_linux.cpp`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/cmake/telegram_options.cmake`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/build/setup.iss`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/build/build.sh`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/build/release.py`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/build/version`
- `F:/test_cl/split/ansible-desktop/app-desktop/Telegram/Telegram/{Telegram,Telegram Lite,Breakpad}.entitlements`

### Связанные коммиты (для cross-reference)

- `a591ec9 port: BeHappy customizations from old bh-desktop` — последний big-batch rebrand (видимая часть).
- `0e616d6 Replace app icons with BeHappy logo` — 26 PNG/.ico/.iconset.
- `b7c0f99 feat: replace Telegram icons with BeHappy branding` — 18 SVG/PNG (cherry-pick из `2472246d`).
- `fd85f3b chore(legal): GPL v3 compliance for fork attribution` — LEGAL, NOTICE, README, CHANGELOG.
- `7da05ed Initial: BeHappy Desktop Client (based on tdesktop)` — fork origin.
- `30a6fc8 feat(mtproto): point single built-in DC to new server IP` — BeHappy DC.
- `a560f6d feat(mtproto): swap built-in DC1 RSA public key to the rotated server key` — текущий HEAD, BeHappy RSA.

---

**End of audit.**
