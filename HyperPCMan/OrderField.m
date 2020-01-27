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
    
    NSNumberFormatter *nf = [[NSNumberFormatter alloc] init];
    [nf setAllowsFloats:false];
    [nf setNumberStyle:NSNumberFormatterNoStyle];
    [nf setPartialStringValidationEnabled:true];
    
    NSNumber *min = [[NSNumber alloc] initWithInt:1];
    [nf setMinimum:min];
    
    Name = [[NSTextField alloc] initWithFrame:NSMakeRect(400, 400, 120, 20)];
    [Name setAlignment:NSTextAlignmentCenter];
    [Name setStringValue:@""];
    [Name setEditable:false];
    
    Num  = [[NSTextField alloc] initWithFrame:NSMakeRect(400, 400, 30, 20)];
    Num.formatter = nf;
    [Num setIntValue:num];
    [Num setAlignment:NSTextAlignmentCenter];
        
    CGPoint numP = CGPointMake(p.x - 40, p.y);
    
    [Name setFrameOrigin:p]; [Num setFrameOrigin:numP];
        
    NSRect buttonFrame = NSMakeRect(p.x - 60, p.y, 20, 20);
    Remove = [[ActionButton alloc] initXWithAction:buttonFrame blk:^void(){
        NSInteger arrIdx = [self->Num integerValue] - 1;
        NSInteger num = vc->orderNum;
        
        NSString *lookupName = [self->Name stringValue];
        
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
        } else if(vc->currentTurnIdx > arrIdx) {
            vc->currentTurnIdx -= 1;
        }
        
        //NOTE: Right now I don't waste time clearing the name... but maybe it could
        //      be useful if I add stuff like "Add enemies to battle" and other things...
        [vc->Order[num-1]->Name setHidden:true];
        [vc->Order[num-1]->Num setHidden:true];
        [vc->Order[num-1]->Remove->Button setHidden:true];
        
        vc->turnsInRound -= 1;
        vc->orderNum -= 1;
        
        assert(vc->currentTurnIdx < vc->orderNum);
                
        //TODO: I CAN'T HAVE THE SAME NAME BETWEEN TWO GUYS!
        //      NEED TO ADD AN IDENTIFIER SHARED BETWEEN ORDER AND ENTITIES!
        for(NSInteger i = 0; i < vc->allyNum; i++) {
            BattleEntity *a = vc->Allies[i];
            if([[a->Box->Label stringValue] isEqualToString:lookupName]) {
                if(vc->allyNum == 1) {
                    [a hide];
                    [vc->AddEntity[1]->Button setHidden:true];
                    [vc->AddEntity[0]->Button setHidden:false];
                }
                else if(i == (vc->allyNum-1)) {
                    [a clear];
                    [a hide];
                    [vc->AddEntity[i]->Button setHidden:false];
                    if(i+1 < ALLY_SIZE)
                    { [vc->AddEntity[i+1]->Button setHidden:true]; }
                }
                else {
                    BattleEntity *last = vc->Allies[vc->allyNum-1];
                    [a->Box->Label setStringValue:[last->Box->Label stringValue]];
                    [a->Box->Box setStringValue:[last->Box->Box stringValue]];
                    [a->Init setStringValue:[last->Init stringValue]];
                
                    [last clear];
                    [last hide];
                    if(vc->allyNum != ALLY_SIZE) {
                        [vc->AddEntity[vc->allyNum]->Button setHidden:true];
                    }
                    [vc->AddEntity[vc->allyNum-1]->Button setHidden:false];
                }
                vc->allyNum -= 1;
                return;
            }
        }
        
        for(NSInteger i = 0; i < vc->mobNum; i++) {
            BattleEntity *m = vc->Mobs[i];
            if([[m->Box->Label stringValue] isEqualToString:lookupName]) {
                if(vc->mobNum == 1) {
                    [m hide];
                    [vc->AddEntity[ALLY_SIZE + 1]->Button setHidden:true];
                    [vc->AddEntity[ALLY_SIZE + 0]->Button setHidden:false];
                }
                else if(i == (vc->mobNum-1)) {
                    [m clear];
                    [m hide];
                    [vc->AddEntity[ALLY_SIZE + i]->Button setHidden:false];
                    if(i+1 < MOB_SIZE)
                    { [vc->AddEntity[ALLY_SIZE + i+1]->Button setHidden:true]; }
                }
                else {
                    BattleEntity *last = vc->Mobs[vc->mobNum-1];
                    [m->Box->Label setStringValue:[last->Box->Label stringValue]];
                    [m->Box->Box setStringValue:[last->Box->Box stringValue]];
                    [m->Init setStringValue:[last->Init stringValue]];
                
                    [last clear];
                    [last hide];
                    if(vc->mobNum != MOB_SIZE) {
                        [vc->AddEntity[ALLY_SIZE + vc->mobNum]->Button setHidden:true];
                    }
                    [vc->AddEntity[ALLY_SIZE + (vc->mobNum-1)]->Button setHidden:false];
                }
                vc->mobNum -= 1;
                return;
            }
        }
                
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
