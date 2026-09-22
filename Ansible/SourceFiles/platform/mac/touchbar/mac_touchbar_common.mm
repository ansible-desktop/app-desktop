/*
This file is part of Ansible Desktop, a fork of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/ansible-desktop/app-desktop/blob/master/LEGAL
*/
#include "platform/mac/touchbar/mac_touchbar_common.h"

#include "base/platform/mac/base_utilities_mac.h"

#import <AppKit/NSTextField.h>

namespace TouchBar {

int WidthFromString(NSString *s) {
	return (int)ceil(
		[[NSTextField labelWithString:s] frame].size.width) * 1.2;
}

NSImage *CreateNSImageFromStyleIcon(const style::icon &icon, int size) {
	auto instance = icon.instance(QColor(255, 255, 255, 255), 100);
	instance.setDevicePixelRatio(style::DevicePixelRatio());
	NSImage *image = Platform::Q2NSImage(instance);
	[image setSize:NSMakeSize(size, size)];
	return image;
}

} // namespace TouchBar
