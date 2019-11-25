//
//  OrderField.m
//  HyperPCMan
//
//  Created by Lowy on 17/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "OrderField.h"

@implementation OrderField

- (OrderField *)initOrder:(CGPoint)p num:(int)num {
    
    Name = [[NSTextField alloc] initWithFrame:NSMakeRect(400, 400, 120, 20)];
    [Name setAlignment:NSTextAlignmentCenter];
    [Name setStringValue:@""];
    
    Num  = [[NSTextField alloc] initWithFrame:NSMakeRect(400, 400, 30, 20)];
    [Num setIntValue:num];
    [Num setAlignment:NSTextAlignmentCenter];
        
    CGPoint numP = CGPointMake(p.x - 40, p.y);
    
    [Name setFrameOrigin:p]; [Num setFrameOrigin:numP];
    
    //NOTE:TODO: Something fucky happening. Slowdown HERE!
    NSRect buttonFrame = NSMakeRect(p.x - 60, p.y, 20, 20);
    Remove = [[ActionButton alloc] initXWithAction:buttonFrame blk:^void(){
        NSLog(@"Remove This Order!");
    }];
    
    [Name setHidden:true];
    [Num setHidden:true];
    [Remove->Button setHidden:true];
    
    return self;
}

- (void)hide {
    [Name setHidden:true];
    [Num setHidden:true];
    [Remove->Button setHidden:true];
}

- (void)show {
    [Name setHidden:false];
    [Num setHidden:false];
    [Remove->Button setHidden:false];
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
}

@end
