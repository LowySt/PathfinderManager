//
//  OrderField.m
//  HyperPCMan
//
//  Created by Lowy on 17/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "OrderField.h"
#import "ViewController.h"

@implementation OrderField

- (OrderField *)initOrder:(CGPoint)p num:(int)num vc:(ViewController *)vc {
    
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
        NSInteger arrIdx = [self->Num integerValue] - 1;
        NSInteger mobs = [vc->MobSelector indexOfSelectedItem];
        NSInteger allies = [vc->AllySelector indexOfSelectedItem];
        NSInteger num = mobs + allies + PARTY_SIZE - vc->notInBattle - vc->removed;
        
        for(NSInteger i = arrIdx; i < num-1; i++) {
            NSString *new = [vc->Order[i+1]->Name stringValue];
            [vc->Order[i]->Name setStringValue:new];
        }
        if(vc->currentTurnIdx == arrIdx) {
            if(arrIdx == num-1) {
                vc->currentTurnIdx = 0;
                [vc->CurrentInTurn setStringValue:[vc->Order[0]->Name stringValue]];
            } else {
                [vc->CurrentInTurn setStringValue:[vc->Order[arrIdx]->Name stringValue]];
            }
        }
        //NOTE: Right now I don't waste time clearing the name... but maybe it could
        //      be useful if I add stuff like "Add enemies to battle" and other things...
        [vc->Order[num-1]->Name setHidden:true];
        [vc->Order[num-1]->Num setHidden:true];
        [vc->Order[num-1]->Remove->Button setHidden:true];
        
        vc->turnsInRound -= 1;
        vc->removed += 1;
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
