//
//  DrawView.m
//  TheTruePCMan
//
//  Created by Lowy on 13/10/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "DrawView.h"

@implementation DrawView

- (void) setPath {
    path = [[NSBezierPath alloc] init ];
}

- (void) mouseDown:(NSEvent *)event {
    [path moveToPoint:[event locationInWindow]];
    //path.move(to: convert(event.locationInWindow, from: nil))
    [self setNeedsDisplay:true];
    //needsDisplay = true
}

- (void) mouseDragged:(NSEvent *)event {
    [path lineToPoint:[event locationInWindow]];
    //path.line(to: convert(event.locationInWindow, from: nil))
    [self setNeedsDisplay:true];
    //needsDisplay = true
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
    [[NSColor whiteColor] set];
    //NSColor.black.set()
        
    [path setLineJoinStyle:NSRoundLineJoinStyle];
    //path.lineJoinStyle = .roundLineJoinStyle
    [path setLineCapStyle:NSRoundLineCapStyle];
    //path.lineCapStyle = .roundLineCapStyle
    [path setLineWidth:10.0];
    //path.lineWidth = 10.0
    [path stroke];
    //path.stroke()

}

@end
