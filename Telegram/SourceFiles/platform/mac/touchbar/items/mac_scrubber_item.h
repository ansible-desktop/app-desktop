/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#pragma once

#import <AppKit/NSPopoverTouchBarItem.h>
#import <AppKit/NSTouchBar.h>

namespace Window {
class Controller;
} // namespace Window

API_AVAILABLE(macos(10.12.2))
@interface StickerEmojiPopover : NSPopoverTouchBarItem<NSTouchBarDelegate>
- (id)init:(not_null<Window::Controller*>)controller
	identifier:(NSTouchBarItemIdentifier)identifier;
@end
