/*
Copyright (c) 2012, Yahoo! Inc.  All rights reserved.
Copyrights licensed under the New BSD License. See the accompanying LICENSE
file for terms.
*/
// [AUTO_HEADER]

#import <Cocoa/Cocoa.h>

@interface TakaoKeyboardLayoutPopUpButton : NSPopUpButton {
  NSMenu *_standardMenu;
  NSMenu *_realMenu;
}

- (void)changeMenu:(NSNotification *)notification;

@end
