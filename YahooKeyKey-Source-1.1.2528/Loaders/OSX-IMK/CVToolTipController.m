// [AUTO_HEADER]

#import "CVToolTipController.h"

#import "CVFloatingBackground.h"

@implementation CVToolTipController

- (id)init {
  self = [super init];
  if (self != nil) {
    BOOL loaded = [NSBundle loadNibNamed:@"TooltipWindow" owner:self];
    NSAssert((loaded == YES), @"NIB did not load");
  }
  return self;
}
- (void)awakeFromNib {
  [[self window] setBackgroundColor:[NSColor colorWithCalibratedHue:0.16
                                                         saturation:0.22
                                                         brightness:0.97
                                                              alpha:1.00]];
  _lineHeight = [_messageTextField frame].size.height;
}
- (void)setWindowLocation:(NSPoint)point {
  NSPoint newPoint = point;
  if (point.y > 5) {
    newPoint.y -= 5;
  }
  [[self window] setFrameTopLeftPoint:newPoint];
}

- (void)autoSize {
  NSAttributedString *c = [_messageTextField attributedStringValue];
  int lines = [[[c string] componentsSeparatedByString:@"\n"] count];
  float h = _lineHeight * lines;

  NSRect rect = [c boundingRectWithSize:NSMakeSize(1600, h)
                                options:NSStringDrawingUsesLineFragmentOrigin];
  NSRect newTextRect = [_messageTextField frame];
  newTextRect.size.width = rect.size.width + 10;
  newTextRect.size.height = h;
  [_messageTextField setFrame:newTextRect];
  [_messageTextField setNeedsDisplay:YES];

  NSRect newWindowRect = [[self window] frame];
  newWindowRect.size.width = newTextRect.size.width + 10;
  newWindowRect.size.height = h + 10;
  [[self window] setFrame:newWindowRect display:YES];
}

- (void)showToolTip:(NSString *)toolTip atPoint:(NSPoint)point {
  [self setToolTip:toolTip];
  [self autoSize];
  [self setWindowLocation:point];
  [self show];
}
- (void)setToolTip:(NSString *)toolTip {
  [_messageTextField setStringValue:toolTip];
}
- (void)clear {
  [_messageTextField setStringValue:@""];
  [self hide];
}
- (void)show {
  [[self window] orderFront:self];
}
- (void)hide {
  [[self window] orderOut:self];
}

@end
