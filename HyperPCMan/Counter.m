//
//  Counter.m
//  HyperPCMan
//
//  Created by Lowy on 18/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "Counter.h"

@implementation Counter

- (Counter *)initWithFrame:(NSRect)frame name:(NSString *)name {
    
    //TODO: Not using frame size right now!
    isCounting = false;
    
    CGPoint pos = CGPointMake(frame.origin.x, frame.origin.y);
    Field = [[LabeledTextBox alloc] initLabeled:name labelDir:LABEL_UP p:pos isEditable:true];
    Count = [[NSTextField alloc] initWithFrame:NSMakeRect(frame.origin.x + 84, frame.origin.y, 40, 20)];
    
    [Count setAlignment:NSTextAlignmentCenter];
    
    NSNumberFormatter *nf = [[NSNumberFormatter alloc] init];
    [nf setAllowsFloats:false];
    [nf setNumberStyle:NSNumberFormatterNoStyle];
    [nf setPartialStringValidationEnabled:true];
    Count.formatter = nf;
    
    SetButton = [[ActionButton alloc] initWithAction:NSMakeRect(frame.origin.x + 120, frame.origin.y - 3, 50, 24) name:@"Go" blk:^void(){
        self->isCounting = true;
    }];
    
    return self;
}

- (void)tick {
    elapsedTurns = 0;
    int currCount = [Count intValue];
    currCount -= 1;
    
    if(currCount == 0) {
        isCounting = false;
        [Count setStringValue:@""];
        [Field->Box setStringValue:@""];
    }
    else { [Count setIntValue:currCount]; }
}

/*
- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
}
*/
@end
