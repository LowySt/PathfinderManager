//
//  ViewController.m
//  TheTruePCMan
//
//  Created by Lowy on 09/10/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "ViewController.h"
#import "Feats.h"
#import <stdlib.h>
#import <objc/runtime.h>

enum LabelDir {
    LABEL_UP = 0,
    LABEL_DOWN,
    LABEL_LEFT,
    LABEL_RIGHT,
    
    LABEL_COUNT
};

#define setFieldAndLabel(f, l, p, d) { \
    CGSize fR = f.frame.size; \
    CGSize lR = l.frame.size; \
    CGPoint labelP; \
    switch(d) \
    { \
        case LABEL_UP: { labelP = CGPointMake(p.x, p.y + fR.height); } break; \
        case LABEL_DOWN: { labelP = CGPointMake(p.x, p.y - fR.height); } break; \
        case LABEL_LEFT: { labelP = CGPointMake(p.x - lR.width, p.y); } break; \
        case LABEL_RIGHT: { labelP = CGPointMake(p.x + fR.width, p.y); } break; \
        default: \
        { assert(false); } break; \
    } \
    [f setFrameOrigin:p]; [l setFrameOrigin:labelP]; \
}

#define setField(f, p) [f setFrameOrigin:p]
#define mask0(f) f.autoresizingMask = 0
#define mask0andFormat(f, t) mask0(f); f.formatter = t
#define setICF(f, l, n, p, lp) [l setStringValue:@n]; \
[l sizeToFit]; setFieldAndLabel(f, l, p, lp)

struct OrderType
{
    NSString *name;
    NSInteger v;
};

@implementation ViewController

- (void)setCurrentTurnIdx:(int)v {
    currentTurnIdx = v;
}

- (void)changePos:(int)oldPos newPos:(int)newPos {
    
    if(oldPos == newPos) { return; }
    if(newPos > 22) { return; }
    if(newPos > [_MobNumberSel indexOfSelectedItem] + 6) { return; }
    
    NSInteger num = [_MobNumberSel indexOfSelectedItem] + 6;
    NSMutableArray *order = [[NSMutableArray alloc] init];

    currentTurnIdx += 1;
    if(currentTurnIdx > num) { currentTurnIdx = 1; }
        
    struct OrderType h1;
    switch(num)
    {
        case 22:
            h1.name = [_Order22Field stringValue];
            h1.v = [_Order22Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 21:
            h1.name = [_Order21Field stringValue];
            h1.v = [_Order21Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 20:
            h1.name = [_Order20Field stringValue];
            h1.v = [_Order20Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 19:
            h1.name = [_Order19Field stringValue];
            h1.v = [_Order19Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 18:
            h1.name = [_Order18Field stringValue];
            h1.v = [_Order18Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 17:
            h1.name = [_Order17Field stringValue];
            h1.v = [_Order17Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 16:
            h1.name = [_Order16Field stringValue];
            h1.v = [_Order16Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 15:
            h1.name = [_Order15Field stringValue];
            h1.v = [_Order15Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 14:
            h1.name = [_Order14Field stringValue];
            h1.v = [_Order14Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 13:
            h1.name = [_Order13Field stringValue];
            h1.v = [_Order13Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 12:
            h1.name = [_Order12Field stringValue];
            h1.v = [_Order12Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 11:
            h1.name = [_Order11Field stringValue];
            h1.v = [_Order11Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 10:
            h1.name = [_Order10Field stringValue];
            h1.v = [_Order10Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 9:
            h1.name = [_Order9Field stringValue];
            h1.v = [_Order9Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 8:
            h1.name = [_Order8Field stringValue];
            h1.v = [_Order8Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 7:
            h1.name = [_Order7Field stringValue];
            h1.v = [_Order7Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
            h1.name = [_Order6Field stringValue];
            h1.v = [_Order6Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
            h1.name = [_Order5Field stringValue];
            h1.v = [_Order5Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
            h1.name = [_Order4Field stringValue];
            h1.v = [_Order4Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
            h1.name = [_Order3Field stringValue];
            h1.v = [_Order3Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
            h1.name = [_Order2Field stringValue];
            h1.v = [_Order2Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
            h1.name = [_Order1Field stringValue];
            h1.v = [_Order1Num integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
            break;
    }
    
    order = [[[order reverseObjectEnumerator] allObjects] mutableCopy];
    
    int currIdx = newPos - 1;
    int newIdx = oldPos - 1;
    NSObject *curr = [order objectAtIndex:newIdx];
    
    int range;
    if(newPos > oldPos)
    {
        int untouched = newPos - oldPos - 1;
        range = (int)num - untouched;
    }
    else { range = oldPos - newPos + 1; }
    
    for(int i = 0; i < range; i++)
    {
        NSObject *tmp = [order objectAtIndex:currIdx];
        [order replaceObjectAtIndex:currIdx withObject:curr];
        
        if((currIdx + 1) > (num - 1))
        { newIdx = 0; }
        else { newIdx = currIdx + 1; }
                
        curr = tmp;
        currIdx = newIdx;
    }
    
    struct OrderType val;
    NSString *s;
    switch(num)
    {
        case 22:
            [order[21] getValue:&val];
            s = val.name;
            [_Order22Field setStringValue:s];
        case 21:
            [order[20] getValue:&val];
            s = val.name;
            [_Order21Field setStringValue:s];
        case 20:
            [order[19] getValue:&val];
            s = val.name;
            [_Order20Field setStringValue:s];
        case 19:
            [order[18] getValue:&val];
            s = val.name;
            [_Order19Field setStringValue:s];
        case 18:
            [order[17] getValue:&val];
            s = val.name;
            [_Order18Field setStringValue:s];
        case 17:
            [order[16] getValue:&val];
            s = val.name;
            [_Order17Field setStringValue:s];
        case 16:
            [order[15] getValue:&val];
            s = val.name;
            [_Order16Field setStringValue:s];
        case 15:
            [order[14] getValue:&val];
            s = val.name;
            [_Order15Field setStringValue:s];
        case 14:
            [order[13] getValue:&val];
            s = val.name;
            [_Order14Field setStringValue:s];
        case 13:
            [order[12] getValue:&val];
            s = val.name;
            [_Order13Field setStringValue:s];
        case 12:
            [order[11] getValue:&val];
            s = val.name;
            [_Order12Field setStringValue:s];
        case 11:
            [order[10] getValue:&val];
            s = val.name;
            [_Order11Field setStringValue:s];
        case 10:
            [order[9] getValue:&val];
            s = val.name;
            [_Order10Field setStringValue:s];
        case 9:
            [order[8] getValue:&val];
            s = val.name;
            [_Order9Field setStringValue:s];
        case 8:
            [order[7] getValue:&val];
            s = val.name;
            [_Order8Field setStringValue:s];
        case 7:
            [order[6] getValue:&val];
            s = val.name;
            [_Order7Field setStringValue:s];
            [order[5] getValue:&val];
            s = val.name;
            [_Order6Field setStringValue:s];
            [order[4] getValue:&val];
            s = val.name;
            [_Order5Field setStringValue:s];
            [order[3] getValue:&val];
            s = val.name;
            [_Order4Field setStringValue:s];
            [order[2] getValue:&val];
            s = val.name;
            [_Order3Field setStringValue:s];
            [order[1] getValue:&val];
            s = val.name;
            [_Order2Field setStringValue:s];
            [order[0] getValue:&val];
            s = val.name;
            [_Order1Field setStringValue:s];
            [_CurrentTurnField setStringValue:s];
            break;
    }
    
    [self setCurrentTurn];
}

- (void)rollInitiative {
    NSInteger num = [_MobNumberSel indexOfSelectedItem];
    int newRand;
    
    switch(num)
    {
        case 16:
            newRand = arc4random_uniform(20) + 1;
            [_Mob16Field setIntValue:(newRand + [_Mob16Bonus intValue])];
        case 15:
            newRand = arc4random_uniform(20) + 1;
            [_Mob15Field setIntValue:(newRand + [_Mob15Bonus intValue])];
        case 14:
            newRand = arc4random_uniform(20) + 1;
            [_Mob14Field setIntValue:(newRand + [_Mob14Bonus intValue])];
        case 13:
            newRand = arc4random_uniform(20) + 1;
            [_Mob13Field setIntValue:(newRand + [_Mob13Bonus intValue])];
        case 12:
            newRand = arc4random_uniform(20) + 1;
            [_Mob12Field setIntValue:(newRand + [_Mob12Bonus intValue])];
        case 11:
            newRand = arc4random_uniform(20) + 1;
            [_Mob11Field setIntValue:(newRand + [_Mob11Bonus intValue])];
        case 10:
            newRand = arc4random_uniform(20) + 1;
            [_Mob10Field setIntValue:(newRand + [_Mob10Bonus intValue])];
        case 9:
            newRand = arc4random_uniform(20) + 1;
            [_Mob9Field setIntValue:(newRand + [_Mob9Bonus intValue])];
        case 8:
            newRand = arc4random_uniform(20) + 1;
            [_Mob8Field setIntValue:(newRand + [_Mob8Bonus intValue])];
        case 7:
            newRand = arc4random_uniform(20) + 1;
            [_Mob7Field setIntValue:(newRand + [_Mob7Bonus intValue])];
        case 6:
            newRand = arc4random_uniform(20) + 1;
            [_Mob6Field setIntValue:(newRand + [_Mob6Bonus intValue])];
        case 5:
            newRand = arc4random_uniform(20) + 1;
            [_Mob5Field setIntValue:(newRand + [_Mob5Bonus intValue])];
        case 4:
            newRand = arc4random_uniform(20) + 1;
            [_Mob4Field setIntValue:(newRand + [_Mob4Bonus intValue])];
        case 3:
            newRand = arc4random_uniform(20) + 1;
            [_Mob3Field setIntValue:(newRand + [_Mob3Bonus intValue])];
        case 2:
            newRand = arc4random_uniform(20) + 1;
            [_Mob2Field setIntValue:(newRand + [_Mob2Bonus intValue])];
        case 1:
            newRand = arc4random_uniform(20) + 1;
            [_Mob1Field setIntValue:(newRand + [_Mob1Bonus intValue])];
    }
}

- (void)setOrder {
    NSInteger num = [_MobNumberSel indexOfSelectedItem];
        
    NSMutableArray *order = [[NSMutableArray alloc] init];
    
    currentTurnIdx = 1;
    
    struct OrderType h1;
    h1.name = [_Hero1Label stringValue];
    h1.v = [_Hero1Field integerValue];
    [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
    h1.name = [_Hero2Label stringValue];
    h1.v = [_Hero2Field integerValue];
    [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
    h1.name = [_Hero3Label stringValue];
    h1.v = [_Hero3Field integerValue];
    [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
    h1.name = [_Hero4Label stringValue];
    h1.v = [_Hero4Field integerValue];
    [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
    h1.name = [_Hero5Label stringValue];
    h1.v = [_Hero5Field integerValue];
    [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
    h1.name = [_Hero6Label stringValue];
    h1.v = [_Hero6Field integerValue];
    [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];

        
    switch (num) {
            
        case 16:
            h1.name = [_Mob16Label stringValue];
            h1.v = [_Mob16Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 15:
            h1.name = [_Mob15Label stringValue];
            h1.v = [_Mob15Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 14:
            h1.name = [_Mob14Label stringValue];
            h1.v = [_Mob14Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 13:
            h1.name = [_Mob13Label stringValue];
            h1.v = [_Mob13Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 12:
            h1.name = [_Mob12Label stringValue];
            h1.v = [_Mob12Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 11:
            h1.name = [_Mob11Label stringValue];
            h1.v = [_Mob11Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 10:
            h1.name = [_Mob10Label stringValue];
            h1.v = [_Mob10Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 9:
            h1.name = [_Mob9Label stringValue];
            h1.v = [_Mob9Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 8:
            h1.name = [_Mob8Label stringValue];
            h1.v = [_Mob8Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 7:
            h1.name = [_Mob7Label stringValue];
            h1.v = [_Mob7Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 6:
            h1.name = [_Mob6Label stringValue];
            h1.v = [_Mob6Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 5:
            h1.name = [_Mob5Label stringValue];
            h1.v = [_Mob5Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 4:
            h1.name = [_Mob4Label stringValue];
            h1.v = [_Mob4Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 3:
            h1.name = [_Mob3Label stringValue];
            h1.v = [_Mob3Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 2:
            h1.name = [_Mob2Label stringValue];
            h1.v = [_Mob2Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
        case 1:
            h1.name = [_Mob1Label stringValue];
            h1.v = [_Mob1Field integerValue];
            [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
            break;
    }
    
    [order sortUsingComparator:^(id obj1, id obj2) {
        struct OrderType a; [obj1 getValue:&a];
        struct OrderType b; [obj2 getValue:&b];
        
        if (a.v > b.v) {
            return (NSComparisonResult)NSOrderedAscending;
        }
     
        if (a.v < b.v) {
            return (NSComparisonResult)NSOrderedDescending;
        }
        return (NSComparisonResult)NSOrderedSame;
    }];
    
    struct OrderType val;
    NSString *s;
    switch(num+6)
    {
        case 22:
            [order[21] getValue:&val];
            s = val.name;
            [_Order22Field setStringValue:s];
        case 21:
            [order[20] getValue:&val];
            s = val.name;
            [_Order21Field setStringValue:s];
        case 20:
            [order[19] getValue:&val];
            s = val.name;
            [_Order20Field setStringValue:s];
        case 19:
            [order[18] getValue:&val];
            s = val.name;
            [_Order19Field setStringValue:s];
        case 18:
            [order[17] getValue:&val];
            s = val.name;
            [_Order18Field setStringValue:s];
        case 17:
            [order[16] getValue:&val];
            s = val.name;
            [_Order17Field setStringValue:s];
        case 16:
            [order[15] getValue:&val];
            s = val.name;
            [_Order16Field setStringValue:s];
        case 15:
            [order[14] getValue:&val];
            s = val.name;
            [_Order15Field setStringValue:s];
        case 14:
            [order[13] getValue:&val];
            s = val.name;
            [_Order14Field setStringValue:s];
        case 13:
            [order[12] getValue:&val];
            s = val.name;
            [_Order13Field setStringValue:s];
        case 12:
            [order[11] getValue:&val];
            s = val.name;
            [_Order12Field setStringValue:s];
        case 11:
            [order[10] getValue:&val];
            s = val.name;
            [_Order11Field setStringValue:s];
        case 10:
            [order[9] getValue:&val];
            s = val.name;
            [_Order10Field setStringValue:s];
        case 9:
            [order[8] getValue:&val];
            s = val.name;
            [_Order9Field setStringValue:s];
        case 8:
            [order[7] getValue:&val];
            s = val.name;
            [_Order8Field setStringValue:s];
        case 7:
            [order[6] getValue:&val];
            s = val.name;
            [_Order7Field setStringValue:s];
            [order[5] getValue:&val];
            s = val.name;
            [_Order6Field setStringValue:s];
            [order[4] getValue:&val];
            s = val.name;
            [_Order5Field setStringValue:s];
            [order[3] getValue:&val];
            s = val.name;
            [_Order4Field setStringValue:s];
            [order[2] getValue:&val];
            s = val.name;
            [_Order3Field setStringValue:s];
            [order[1] getValue:&val];
            s = val.name;
            [_Order2Field setStringValue:s];
            [order[0] getValue:&val];
            s = val.name;
            [_Order1Field setStringValue:s];
            [_CurrentTurnField setStringValue:s];
            break;
    }
         
}

- (void) setCurrentTurn {
    
    //TODO: Convert currentTurnIdx to be in the range 0..n-1 instead of 0..n
    NSTextField *f = OrderArr[currentTurnIdx - 1];
    [_CurrentTurnField setStringValue:[f stringValue]];
}

- (void)advanceTurn {
    NSInteger num = [_MobNumberSel indexOfSelectedItem] + 1 + 6;
    
    if(currentTurnIdx == (num - 1))
    { currentTurnIdx = 1; }
    else { currentTurnIdx += 1; }
    
    [self setCurrentTurn];
}

- (void)initialization {
    
    size_t tfs = sizeof(NSTextField *);

    OrderArr = (NSTextField * __strong*)malloc(tfs*22);
    OrderArr[0] = _Order1Field; OrderArr[1] = _Order2Field;
    OrderArr[2] = _Order3Field; OrderArr[3] = _Order4Field;
    OrderArr[4] = _Order5Field; OrderArr[5] = _Order6Field;
    OrderArr[6] = _Order7Field; OrderArr[7] = _Order8Field;
    OrderArr[8] = _Order9Field; OrderArr[9] = _Order10Field;
    OrderArr[10] = _Order11Field; OrderArr[11] = _Order12Field;
    OrderArr[12] = _Order13Field; OrderArr[13] = _Order14Field;
    OrderArr[14] = _Order15Field; OrderArr[15] = _Order16Field;
    OrderArr[16] = _Order17Field; OrderArr[17] = _Order18Field;
    OrderArr[18] = _Order19Field; OrderArr[19] = _Order20Field;
    OrderArr[20] = _Order21Field; OrderArr[21] = _Order22Field;
    
    currentTurnIdx = 1;
    
    [[_TabController tabViewItemAtIndex:0] setLabel:@"PC"];
    [[_TabController tabViewItemAtIndex:1] setLabel:@"Feats"];
    [[_TabController tabViewItemAtIndex:2] setLabel:@"Init"];
    
    [_InitiativeRollButton setTitle:@"Roll"];
    [_SetOrderButton setTitle:@"Set"];
    [_AdvanceTurnButton setTitle:@"Next"];
    setField(_InitiativeRollButton, CGPointMake(420, 836));
        
    setICF(_MobNumberSel, _MobNumberLabel, "Enemy Count",
           CGPointMake(315, 840), LABEL_UP);
    
    setICF(_Hero1Field, _Hero1Label, "Gremag", CGPointMake(525, 810), LABEL_LEFT);
    setICF(_Hero2Field, _Hero2Label, "Ken Shiro", CGPointMake(525, 780), LABEL_LEFT);
    setICF(_Hero3Field, _Hero3Label, "Sirion", CGPointMake(525, 750), LABEL_LEFT);
    setICF(_Hero4Field, _Hero4Label, "Albion", CGPointMake(525, 720), LABEL_LEFT);
    setICF(_Hero5Field, _Hero5Label, "Zakkhyr", CGPointMake(525, 690), LABEL_LEFT);
    setICF(_Hero6Field, _Hero6Label, "Zoddak", CGPointMake(525, 660), LABEL_LEFT);
    
    setICF(_Mob1Bonus, _Mob1Label, "Enemy 1", CGPointMake(365, 810), LABEL_LEFT);
    setICF(_Mob2Bonus, _Mob2Label, "Enemy 2", CGPointMake(365, 780), LABEL_LEFT);
    setICF(_Mob3Bonus, _Mob3Label, "Enemy 3", CGPointMake(365, 750), LABEL_LEFT);
    setICF(_Mob4Bonus, _Mob4Label, "Enemy 4", CGPointMake(365, 720), LABEL_LEFT);
    setICF(_Mob5Bonus, _Mob5Label, "Enemy 5", CGPointMake(365, 690), LABEL_LEFT);
    setICF(_Mob6Bonus, _Mob6Label, "Enemy 6", CGPointMake(365, 660), LABEL_LEFT);
    setICF(_Mob7Bonus, _Mob7Label, "Enemy 7", CGPointMake(365, 630), LABEL_LEFT);
    setICF(_Mob8Bonus, _Mob8Label, "Enemy 8", CGPointMake(365, 600), LABEL_LEFT);
    setICF(_Mob9Bonus, _Mob9Label, "Enemy 9", CGPointMake(365, 570), LABEL_LEFT);
    setICF(_Mob10Bonus, _Mob10Label, "Enemy 10", CGPointMake(365, 540), LABEL_LEFT);
    setICF(_Mob11Bonus, _Mob11Label, "Enemy 11", CGPointMake(365, 510), LABEL_LEFT);
    setICF(_Mob12Bonus, _Mob12Label, "Enemy 12", CGPointMake(365, 480), LABEL_LEFT);
    setICF(_Mob13Bonus, _Mob13Label, "Enemy 13", CGPointMake(365, 450), LABEL_LEFT);
    setICF(_Mob14Bonus, _Mob14Label, "Enemy 14", CGPointMake(365, 420), LABEL_LEFT);
    setICF(_Mob15Bonus, _Mob15Label, "Enemy 15", CGPointMake(365, 390), LABEL_LEFT);
    setICF(_Mob16Bonus, _Mob16Label, "Enemy 16", CGPointMake(365, 360), LABEL_LEFT);

    setField(_Mob1Field, CGPointMake(405, 810));
    setField(_Mob2Field, CGPointMake(405, 780));
    setField(_Mob3Field, CGPointMake(405, 750));
    setField(_Mob4Field, CGPointMake(405, 720));
    setField(_Mob5Field, CGPointMake(405, 690));
    setField(_Mob6Field, CGPointMake(405, 660));
    setField(_Mob7Field, CGPointMake(405, 630));
    setField(_Mob8Field, CGPointMake(405, 600));
    setField(_Mob9Field, CGPointMake(405, 570));
    setField(_Mob10Field, CGPointMake(405, 540));
    setField(_Mob11Field, CGPointMake(405, 510));
    setField(_Mob12Field, CGPointMake(405, 480));
    setField(_Mob13Field, CGPointMake(405, 450));
    setField(_Mob14Field, CGPointMake(405, 420));
    setField(_Mob15Field, CGPointMake(405, 390));
    setField(_Mob16Field, CGPointMake(405, 360));

    
    mask0(_RaceSelector); mask0(_ClassSelector); mask0(_GenMethodSel);
    mask0(_STRBonus); mask0(_DEXBonus); mask0(_CONBonus); mask0(_INTBonus);
    mask0(_WISBonus); mask0(_CHABonus);
    
    mask0(_LvlField); mask0(_ExpField); mask0(_NextXPField); mask0(_XPCurveSel);
    mask0(_BABField); mask0(_FortField); mask0(_RefField); mask0(_WillField);
    mask0(_RaceTraitsField);

    mask0(_MobNumberSel); mask0(_InitiativeRollButton);
    
    mask0(_Mob1Field); mask0(_Mob2Field); mask0(_Mob3Field); mask0(_Mob4Field);
    mask0(_Mob5Field); mask0(_Mob6Field); mask0(_Mob7Field); mask0(_Mob8Field);
    mask0(_Mob9Field); mask0(_Mob10Field); mask0(_Mob11Field); mask0(_Mob12Field);
    mask0(_Mob13Field); mask0(_Mob14Field); mask0(_Mob15Field); mask0(_Mob16Field);
    
    mask0(_Hero1Field); mask0(_Hero2Field); mask0(_Hero3Field); mask0(_Hero4Field);
    mask0(_Hero5Field); mask0(_Hero6Field);
       
    NSNumberFormatter *nf = [[NSNumberFormatter alloc] init];
    [nf setAllowsFloats:false];
    [nf setNumberStyle:NSNumberFormatterNoStyle];
    [nf setPartialStringValidationEnabled:true];
    
    _STRField.formatter = nf;
    _DEXField.formatter = nf;
    _CONField.formatter = nf;
    _INTField.formatter = nf;
    _WISField.formatter = nf;
    _CHAField.formatter = nf;
    
    mask0andFormat(_Mob1Bonus, nf); mask0andFormat(_Mob2Bonus, nf);
    mask0andFormat(_Mob3Bonus, nf); mask0andFormat(_Mob4Bonus, nf);
    mask0andFormat(_Mob5Bonus, nf); mask0andFormat(_Mob6Bonus, nf);
    mask0andFormat(_Mob7Bonus, nf); mask0andFormat(_Mob8Bonus, nf);
    mask0andFormat(_Mob9Bonus, nf); mask0andFormat(_Mob10Bonus, nf);
    mask0andFormat(_Mob11Bonus, nf); mask0andFormat(_Mob12Bonus, nf);
    mask0andFormat(_Mob13Bonus, nf); mask0andFormat(_Mob14Bonus, nf);
    mask0andFormat(_Mob15Bonus, nf); mask0andFormat(_Mob16Bonus, nf);
    
    mask0andFormat(_Order1Field, nf); mask0andFormat(_Order2Field, nf); mask0andFormat(_Order3Field, nf);
    mask0andFormat(_Order4Field, nf); mask0andFormat(_Order5Field, nf); mask0andFormat(_Order6Field, nf);
    mask0andFormat(_Order7Field, nf); mask0andFormat(_Order8Field, nf); mask0andFormat(_Order9Field, nf);
    mask0andFormat(_Order10Field, nf); mask0andFormat(_Order11Field, nf); mask0andFormat(_Order12Field, nf);
    mask0andFormat(_Order13Field, nf); mask0andFormat(_Order14Field, nf); mask0andFormat(_Order15Field, nf);
    mask0andFormat(_Order16Field, nf); mask0andFormat(_Order17Field, nf); mask0andFormat(_Order18Field, nf);
    mask0andFormat(_Order19Field, nf); mask0andFormat(_Order20Field, nf); mask0andFormat(_Order21Field, nf);
    mask0andFormat(_Order22Field, nf);
        
    NSNumber *minLevel = [[NSNumber alloc] initWithInt:1];
    NSNumber *maxLevel = [[NSNumber alloc] initWithInt:20];
    NSNumberFormatter *lvlF = [[NSNumberFormatter alloc] init];
    [lvlF setAllowsFloats:false];
    [lvlF setNumberStyle:NSNumberFormatterNoStyle];
    [lvlF setMinimum:minLevel];
    [lvlF setMaximum:maxLevel];
    [lvlF setPartialStringValidationEnabled:true];
       
    _LvlField.formatter = lvlF;
      
    [ self setAllUneditable ];
    [ self setAllAlignments ];
    
    [_NameLabel sizeToFit];
    [_PlayerLabel sizeToFit];
    [_RaceLabel sizeToFit];
    [_ClassLabel sizeToFit];
    
    [_GenMethodLabel sizeToFit];
    [_STRLabel sizeToFit];
    [_DEXLabel sizeToFit];
    [_CONLabel sizeToFit];
    [_INTLabel sizeToFit];
    [_WISLabel sizeToFit];
    [_CHALabel sizeToFit];
    
    [_LvlLabel sizeToFit];
    [_ExpLabel sizeToFit];
    [_NextXPLabel sizeToFit];
    [_XPCurveLabel sizeToFit];
    
    [_BABLabel sizeToFit];
    [_FortLabel sizeToFit];
    [_RefLabel sizeToFit];
    [_WillLabel sizeToFit];
        
    [_RaceSelector removeAllItems];
    [_ClassSelector removeAllItems];
    [_GenMethodSel removeAllItems];
    [_XPCurveSel removeAllItems];
    [_MobNumberSel removeAllItems];
     
}

- (void)setAllAlignments {
    [_NameField setAlignment:NSTextAlignmentLeft];
    [_PlayerField setAlignment:NSTextAlignmentLeft];
    [_RaceSelector setAlignment:NSTextAlignmentLeft];
    [_ClassSelector setAlignment:NSTextAlignmentLeft];
    [_GenMethodSel setAlignment:NSTextAlignmentLeft];
    
    [_STRField setAlignment:NSTextAlignmentLeft];
    [_DEXField setAlignment:NSTextAlignmentLeft];
    [_CONField setAlignment:NSTextAlignmentLeft];
    [_INTField setAlignment:NSTextAlignmentLeft];
    [_WISField setAlignment:NSTextAlignmentLeft];
    [_CHAField setAlignment:NSTextAlignmentLeft];
    
    [_STRBonus setAlignment:NSTextAlignmentCenter];
    [_DEXBonus setAlignment:NSTextAlignmentCenter];
    [_CONBonus setAlignment:NSTextAlignmentCenter];
    [_INTBonus setAlignment:NSTextAlignmentCenter];
    [_WISBonus setAlignment:NSTextAlignmentCenter];
    [_CHABonus setAlignment:NSTextAlignmentCenter];
    
    [_LvlField setAlignment:NSTextAlignmentLeft];
    [_ExpField setAlignment:NSTextAlignmentLeft];
    [_NextXPField setAlignment:NSTextAlignmentLeft];
    [_XPCurveSel setAlignment:NSTextAlignmentLeft];
    
    [_BABField setAlignment:NSTextAlignmentLeft];
    [_FortField setAlignment:NSTextAlignmentLeft];
    [_RefField setAlignment:NSTextAlignmentLeft];
    [_WillField setAlignment:NSTextAlignmentLeft];
    
    [_RaceTraitsField setAlignment:NSTextAlignmentLeft];
    
    [_Order1Field setAlignment:NSTextAlignmentLeft];
    [_Order2Field setAlignment:NSTextAlignmentLeft];
    [_Order3Field setAlignment:NSTextAlignmentLeft];
    [_Order4Field setAlignment:NSTextAlignmentLeft];
    [_Order5Field setAlignment:NSTextAlignmentLeft];
    [_Order6Field setAlignment:NSTextAlignmentLeft];
    [_Order7Field setAlignment:NSTextAlignmentLeft];
    [_Order8Field setAlignment:NSTextAlignmentLeft];
    [_Order9Field setAlignment:NSTextAlignmentLeft];
    [_Order10Field setAlignment:NSTextAlignmentLeft];
    [_Order11Field setAlignment:NSTextAlignmentLeft];
    [_Order12Field setAlignment:NSTextAlignmentLeft];
    [_Order13Field setAlignment:NSTextAlignmentLeft];
    [_Order14Field setAlignment:NSTextAlignmentLeft];
    [_Order15Field setAlignment:NSTextAlignmentLeft];
    [_Order16Field setAlignment:NSTextAlignmentLeft];
    [_Order17Field setAlignment:NSTextAlignmentLeft];
    [_Order18Field setAlignment:NSTextAlignmentLeft];
    [_Order19Field setAlignment:NSTextAlignmentLeft];
    [_Order20Field setAlignment:NSTextAlignmentLeft];
    [_Order21Field setAlignment:NSTextAlignmentLeft];
    [_Order22Field setAlignment:NSTextAlignmentLeft];
    
    [_Order1Num setAlignment:NSTextAlignmentLeft];
    [_Order2Num setAlignment:NSTextAlignmentLeft];
    [_Order3Num setAlignment:NSTextAlignmentLeft];
    [_Order4Num setAlignment:NSTextAlignmentLeft];
    [_Order5Num setAlignment:NSTextAlignmentLeft];
    [_Order6Num setAlignment:NSTextAlignmentLeft];
    [_Order7Num setAlignment:NSTextAlignmentLeft];
    [_Order8Num setAlignment:NSTextAlignmentLeft];
    [_Order9Num setAlignment:NSTextAlignmentLeft];
    [_Order10Num setAlignment:NSTextAlignmentLeft];
    [_Order11Num setAlignment:NSTextAlignmentLeft];
    [_Order12Num setAlignment:NSTextAlignmentLeft];
    [_Order13Num setAlignment:NSTextAlignmentLeft];
    [_Order14Num setAlignment:NSTextAlignmentLeft];
    [_Order15Num setAlignment:NSTextAlignmentLeft];
    [_Order16Num setAlignment:NSTextAlignmentLeft];
    [_Order17Num setAlignment:NSTextAlignmentLeft];
    [_Order18Num setAlignment:NSTextAlignmentLeft];
    [_Order19Num setAlignment:NSTextAlignmentLeft];
    [_Order20Num setAlignment:NSTextAlignmentLeft];
    [_Order21Num setAlignment:NSTextAlignmentLeft];
    [_Order22Num setAlignment:NSTextAlignmentLeft];
    
    [_Mob1Field setAlignment:NSTextAlignmentLeft];
    [_Mob2Field setAlignment:NSTextAlignmentLeft];
    [_Mob3Field setAlignment:NSTextAlignmentLeft];
    [_Mob4Field setAlignment:NSTextAlignmentLeft];
    [_Mob5Field setAlignment:NSTextAlignmentLeft];
    [_Mob6Field setAlignment:NSTextAlignmentLeft];
    [_Mob7Field setAlignment:NSTextAlignmentLeft];
    [_Mob8Field setAlignment:NSTextAlignmentLeft];
    [_Mob9Field setAlignment:NSTextAlignmentLeft];
    [_Mob10Field setAlignment:NSTextAlignmentLeft];
    [_Mob11Field setAlignment:NSTextAlignmentLeft];
    [_Mob12Field setAlignment:NSTextAlignmentLeft];
    [_Mob13Field setAlignment:NSTextAlignmentLeft];
    [_Mob14Field setAlignment:NSTextAlignmentLeft];
    [_Mob15Field setAlignment:NSTextAlignmentLeft];
    [_Mob16Field setAlignment:NSTextAlignmentLeft];
    
    [_Mob1Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob2Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob3Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob4Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob5Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob6Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob7Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob8Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob9Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob10Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob11Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob12Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob13Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob14Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob15Bonus setAlignment:NSTextAlignmentLeft];
    [_Mob16Bonus setAlignment:NSTextAlignmentLeft];
    
    [_Hero1Field setAlignment:NSTextAlignmentLeft];
    [_Hero2Field setAlignment:NSTextAlignmentLeft];
    [_Hero3Field setAlignment:NSTextAlignmentLeft];
    [_Hero4Field setAlignment:NSTextAlignmentLeft];
    [_Hero5Field setAlignment:NSTextAlignmentLeft];
    [_Hero6Field setAlignment:NSTextAlignmentLeft];
}

- (void)setAllUneditable {
    [_RaceSelector setEditable:false];
    [_ClassSelector setEditable:false];
    [_GenMethodSel setEditable:false];
    [_XPCurveSel setEditable:false];

    [_STRBonus setEditable:false];
    [_DEXBonus setEditable:false];
    [_CONBonus setEditable:false];
    [_INTBonus setEditable:false];
    [_WISBonus setEditable:false];
    [_CHABonus setEditable:false];
    
    [_NextXPField setEditable:false];
    [_BABField setEditable:false];
    [_FortField setEditable:false];
    [_RefField setEditable:false];
    [_WillField setEditable:false];
    
    [_RaceTraitsField setEditable:false];
    
    [_Order1Field setEditable:false];
    [_Order2Field setEditable:false];
    [_Order3Field setEditable:false];
    [_Order4Field setEditable:false];
    [_Order5Field setEditable:false];
    [_Order6Field setEditable:false];
    [_Order7Field setEditable:false];
    [_Order8Field setEditable:false];
    [_Order9Field setEditable:false];
    [_Order10Field setEditable:false];
    [_Order11Field setEditable:false];
    [_Order12Field setEditable:false];
    [_Order13Field setEditable:false];
    [_Order14Field setEditable:false];
    [_Order15Field setEditable:false];
    [_Order16Field setEditable:false];
    [_Order17Field setEditable:false];
    [_Order18Field setEditable:false];
    [_Order19Field setEditable:false];
    [_Order20Field setEditable:false];
    [_Order21Field setEditable:false];
    [_Order22Field setEditable:false];
    
    [_CurrentTurnField setEditable:false];
}

- (void)hideFields {
    
    [_Mob1Bonus setHidden:true]; [_Mob2Bonus setHidden:true];
    [_Mob3Bonus setHidden:true]; [_Mob4Bonus setHidden:true];
    [_Mob5Bonus setHidden:true]; [_Mob6Bonus setHidden:true];
    [_Mob7Bonus setHidden:true]; [_Mob8Bonus setHidden:true];
    [_Mob9Bonus setHidden:true]; [_Mob10Bonus setHidden:true];
    [_Mob11Bonus setHidden:true]; [_Mob12Bonus setHidden:true];
    [_Mob13Bonus setHidden:true]; [_Mob14Bonus setHidden:true];
    [_Mob15Bonus setHidden:true]; [_Mob16Bonus setHidden:true];
        
    [_Mob1Field setHidden:true]; [_Mob1Label setHidden:true];
    [_Mob2Field setHidden:true]; [_Mob2Label setHidden:true];
    [_Mob3Field setHidden:true]; [_Mob3Label setHidden:true];
    [_Mob4Field setHidden:true]; [_Mob4Label setHidden:true];
    [_Mob5Field setHidden:true]; [_Mob5Label setHidden:true];
    [_Mob6Field setHidden:true]; [_Mob6Label setHidden:true];
    [_Mob7Field setHidden:true]; [_Mob7Label setHidden:true];
    [_Mob8Field setHidden:true]; [_Mob8Label setHidden:true];
    [_Mob9Field setHidden:true]; [_Mob9Label setHidden:true];
    [_Mob10Field setHidden:true]; [_Mob10Label setHidden:true];
    [_Mob11Field setHidden:true]; [_Mob11Label setHidden:true];
    [_Mob12Field setHidden:true]; [_Mob12Label setHidden:true];
    [_Mob13Field setHidden:true]; [_Mob13Label setHidden:true];
    [_Mob14Field setHidden:true]; [_Mob14Label setHidden:true];
    [_Mob15Field setHidden:true]; [_Mob15Label setHidden:true];
    [_Mob16Field setHidden:true]; [_Mob16Label setHidden:true];
        
    [_Order7Num setHidden:true]; [_Order7Field setHidden:true];
    [_Order8Num setHidden:true]; [_Order8Field setHidden:true];
    [_Order9Num setHidden:true]; [_Order9Field setHidden:true];
    [_Order10Num setHidden:true]; [_Order10Field setHidden:true];
    [_Order11Num setHidden:true]; [_Order11Field setHidden:true];
    [_Order12Num setHidden:true]; [_Order12Field setHidden:true];
    [_Order13Num setHidden:true]; [_Order13Field setHidden:true];
    [_Order14Num setHidden:true]; [_Order14Field setHidden:true];
    [_Order15Num setHidden:true]; [_Order15Field setHidden:true];
    [_Order16Num setHidden:true]; [_Order16Field setHidden:true];
    [_Order17Num setHidden:true]; [_Order17Field setHidden:true];
    [_Order18Num setHidden:true]; [_Order18Field setHidden:true];
    [_Order19Num setHidden:true]; [_Order19Field setHidden:true];
    [_Order20Num setHidden:true]; [_Order20Field setHidden:true];
    [_Order21Num setHidden:true]; [_Order21Field setHidden:true];
    [_Order22Num setHidden:true]; [_Order22Field setHidden:true];
        
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [ self initialization ];
    
    setFieldAndLabel(_NameField, _NameLabel, CGPointMake(65, 810), LABEL_LEFT);
    setFieldAndLabel(_PlayerField, _PlayerLabel, CGPointMake(65, 780), LABEL_LEFT);
               
    setFieldAndLabel(_RaceSelector, _RaceLabel, CGPointMake(65, 750), LABEL_LEFT);
    setFieldAndLabel(_ClassSelector, _ClassLabel, CGPointMake(65, 720), LABEL_LEFT);
    
    setFieldAndLabel(_GenMethodSel, _GenMethodLabel, CGPointMake(180, 807), LABEL_UP);
    setFieldAndLabel(_STRField, _STRLabel, CGPointMake(300, 810), LABEL_UP);
    setFieldAndLabel(_DEXField, _DEXLabel, CGPointMake(300, 770), LABEL_UP);
    setFieldAndLabel(_CONField, _CONLabel, CGPointMake(300, 730), LABEL_UP);
    setFieldAndLabel(_INTField, _INTLabel, CGPointMake(300, 690), LABEL_UP);
    setFieldAndLabel(_WISField, _WISLabel, CGPointMake(300, 650), LABEL_UP);
    setFieldAndLabel(_CHAField, _CHALabel, CGPointMake(300, 610), LABEL_UP);
    setField(_STRBonus, CGPointMake(400, 810));
    setField(_DEXBonus, CGPointMake(400, 770));
    setField(_CONBonus, CGPointMake(400, 730));
    setField(_INTBonus, CGPointMake(400, 690));
    setField(_WISBonus, CGPointMake(400, 650));
    setField(_CHABonus, CGPointMake(400, 610));
    
    setFieldAndLabel(_LvlField, _LvlLabel, CGPointMake(480, 810), LABEL_UP);
    setFieldAndLabel(_ExpField, _ExpLabel, CGPointMake(540, 810), LABEL_UP);
    setFieldAndLabel(_NextXPField, _NextXPLabel, CGPointMake(620, 810), LABEL_UP);
    setFieldAndLabel(_XPCurveSel, _XPCurveLabel, CGPointMake(700, 807), LABEL_UP);
    
    setFieldAndLabel(_BABField, _BABLabel, CGPointMake(540, 730), LABEL_LEFT);
    setFieldAndLabel(_FortField, _FortLabel, CGPointMake(540, 690), LABEL_LEFT);
    setFieldAndLabel(_RefField, _RefLabel, CGPointMake(540, 650), LABEL_LEFT);
    setFieldAndLabel(_WillField, _WillLabel, CGPointMake(540, 610), LABEL_LEFT);
         
    [_RaceTraitsField setFrameSize:NSMakeSize(960, 320)];
    setField(_RaceTraitsField, CGPointMake(65, 220));
    
    setField(_Order1Num, CGPointMake(640, 810));
    setField(_Order1Field, CGPointMake(680, 810));
    setField(_Order2Num, CGPointMake(640, 780));
    setField(_Order2Field, CGPointMake(680, 780));
    setField(_Order3Num, CGPointMake(640, 750));
    setField(_Order3Field, CGPointMake(680, 750));
    setField(_Order4Num, CGPointMake(640, 720));
    setField(_Order4Field, CGPointMake(680, 720));
    setField(_Order5Num, CGPointMake(640, 690));
    setField(_Order5Field, CGPointMake(680, 690));
    setField(_Order6Num, CGPointMake(640, 660));
    setField(_Order6Field, CGPointMake(680, 660));
    setField(_Order7Num, CGPointMake(640, 630));
    setField(_Order7Field, CGPointMake(680, 630));
    setField(_Order8Num, CGPointMake(640, 600));
    setField(_Order8Field, CGPointMake(680, 600));
    setField(_Order9Num, CGPointMake(640, 570));
    setField(_Order9Field, CGPointMake(680, 570));
    setField(_Order10Num, CGPointMake(640, 540));
    setField(_Order10Field, CGPointMake(680, 540));
    setField(_Order11Num, CGPointMake(640, 510));
    setField(_Order11Field, CGPointMake(680, 510));
    setField(_Order12Num, CGPointMake(640, 480));
    setField(_Order12Field, CGPointMake(680, 480));
    setField(_Order13Num, CGPointMake(640, 450));
    setField(_Order13Field, CGPointMake(680, 450));
    setField(_Order14Num, CGPointMake(640, 420));
    setField(_Order14Field, CGPointMake(680, 420));
    setField(_Order15Num, CGPointMake(640, 390));
    setField(_Order15Field, CGPointMake(680, 390));
    setField(_Order16Num, CGPointMake(640, 360));
    setField(_Order16Field, CGPointMake(680, 360));
    setField(_Order17Num, CGPointMake(640, 330));
    setField(_Order17Field, CGPointMake(680, 330));
    setField(_Order18Num, CGPointMake(640, 300));
    setField(_Order18Field, CGPointMake(680, 300));
    setField(_Order19Num, CGPointMake(640, 270));
    setField(_Order19Field, CGPointMake(680, 270));
    setField(_Order20Num, CGPointMake(640, 240));
    setField(_Order20Field, CGPointMake(680, 240));
    setField(_Order21Num, CGPointMake(640, 210));
    setField(_Order21Field, CGPointMake(680, 210));
    setField(_Order22Num, CGPointMake(640, 180));
    setField(_Order22Field, CGPointMake(680, 180));
    
    setField(_SetOrderButton, CGPointMake(730, 840));
    setField(_AdvanceTurnButton, CGPointMake(970, 840));
    setField(_CurrentTurnField, CGPointMake(900, 810));
    
    [self hideFields];

    NSArray *Races = @[ @"Dwarf", @"Elf", @"Gnome", @"Half-Elf", @"Half-Orc", @"Halfling", @"Human"];
    [_RaceSelector addItemsWithObjectValues:Races];
    
    NSArray *Classes = @[ @"Barbarian", @"Bard", @"Cleric", @"Druid", @"Fighter", @"Monk", @"Paladin", @"Ranger", @"Rogue", @"Sorcerer", @"Wizard" ];
    [_ClassSelector addItemsWithObjectValues:Classes];
            
    NSArray *GenMethods = @[ @"Classic", @"Dice Pool", @"Heroic", @"Purchase", @"Standard"];
    [_GenMethodSel addItemsWithObjectValues:GenMethods];

    NSArray *XPCurves = @[ @"Fast", @"Medium", @"Slow" ];
    [_XPCurveSel addItemsWithObjectValues:XPCurves];
    
    NSArray *EnemyCount = @[ @"No Enemies", @"1 Enemy", @"2 Enemies", @"3 Enemies", @"4 Enemies", @"5 Enemies", @"6 Enemies", @"7 Enemies", @"8 Enemies", @"9 Enemies", @"10 Enemies", @"11 Enemies", @"12 Enemies", @"13 Enemies", @"14 Enemies", @"15 Enemies", @"16 Enemies"];
    [_MobNumberSel addItemsWithObjectValues:EnemyCount];
        
    NSArray *AB = @[ @"-5", @"-5", @"-4", @"-4", @"-3", @"-3", @"-2", @"-2", @"-1", @"-1", @"0", @"0", @"+1", @"+1", @"+2", @"+2", @"+3", @"+3", @"+4", @"+4", @"+5", @"+5", @"+6", @"+6", @"+7", @"+7", @"+8", @"+8", @"+9", @"+9", @"+10", @"+10", @"+11", @"+11", @"+12", @"+12", @"+13", @"+13", @"+14", @"+14", @"+15", @"+15", @"+16", @"+16", @"+17", @"+17"];
    _AbilityBonus = AB;

    [_RaceSelector selectItemAtIndex:0];
    [_ClassSelector selectItemAtIndex:0];
    [_GenMethodSel selectItemAtIndex:0];
    [_MobNumberSel selectItemAtIndex:0];
    
    [_LvlField setStringValue:@"1"];
    [_ExpField setStringValue:@"0"];
    [_NextXPField setIntegerValue:1300];
    [_XPCurveSel selectItemAtIndex:0];
        
    [_InitiativeRollButton setAction:NSSelectorFromString(@"rollInitiative")];
    [_SetOrderButton setAction:NSSelectorFromString(@"setOrder")];
    [_AdvanceTurnButton setAction:NSSelectorFromString(@"advanceTurn")];
    
    [_Mob1Bonus setIntegerValue:0]; [_Mob2Bonus setIntegerValue:0];
    [_Mob3Bonus setIntegerValue:0]; [_Mob4Bonus setIntegerValue:0];
    [_Mob5Bonus setIntegerValue:0]; [_Mob6Bonus setIntegerValue:0];
    [_Mob7Bonus setIntegerValue:0]; [_Mob8Bonus setIntegerValue:0];
    [_Mob9Bonus setIntegerValue:0]; [_Mob10Bonus setIntegerValue:0];
    [_Mob11Bonus setIntegerValue:0]; [_Mob12Bonus setIntegerValue:0];
    [_Mob13Bonus setIntegerValue:0]; [_Mob14Bonus setIntegerValue:0];
    [_Mob15Bonus setIntegerValue:0]; [_Mob16Bonus setIntegerValue:0];
    
    [_Order1Num setIntegerValue:1]; [_Order2Num setIntegerValue:2];
    [_Order3Num setIntegerValue:3]; [_Order4Num setIntegerValue:4];
    [_Order5Num setIntegerValue:5]; [_Order6Num setIntegerValue:6];
    [_Order7Num setIntegerValue:7]; [_Order8Num setIntegerValue:8];
    [_Order9Num setIntegerValue:9]; [_Order10Num setIntegerValue:10];
    [_Order11Num setIntegerValue:11]; [_Order12Num setIntegerValue:12];
    [_Order13Num setIntegerValue:13]; [_Order14Num setIntegerValue:14];
    [_Order15Num setIntegerValue:15]; [_Order16Num setIntegerValue:16];
    [_Order17Num setIntegerValue:17]; [_Order18Num setIntegerValue:18];
    [_Order19Num setIntegerValue:19]; [_Order20Num setIntegerValue:20];
    [_Order21Num setIntegerValue:21]; [_Order22Num setIntegerValue:22];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    // Update the view, if already loaded.
}

@end
