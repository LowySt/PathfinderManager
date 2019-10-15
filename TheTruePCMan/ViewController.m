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
       
    NSInteger mobNum = [_MobNumberSel indexOfSelectedItem];
    NSInteger allyNum = [_AllyNumberSel indexOfSelectedItem];
    NSInteger num = mobNum + allyNum + PARTY_SIZE;
    if(newPos > num) { return; }

    NSMutableArray *order = [[NSMutableArray alloc] init];

    currentTurnIdx += 1;
    if(currentTurnIdx > num) { currentTurnIdx = 1; }
        
    struct OrderType h1;
    for(int i = 0; i < num; i++)
    {
        h1.name = [OrderFieldArr[i] stringValue];
        h1.v = [OrderNumArr[i] integerValue];
        [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
    }
    
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
    for(int i = 0; i < num; i++)
    {
        [order[i] getValue:&val];
        s = val.name;
        [OrderFieldArr[i] setStringValue:s];
    }
 
    [self setCurrentTurn];
}

- (void)rollInitiative {
    NSInteger mobNum = [_MobNumberSel indexOfSelectedItem];
    NSInteger allyNum = [_AllyNumberSel indexOfSelectedItem];
    int newRand;
    
    for(int i = 0; i < mobNum; i++)
    {
        newRand = arc4random_uniform(20) + 1;
        [MobFieldArr[i] setIntValue:(newRand + [MobBonusArr[i] intValue])];
    }
    
    for(int i = 0; i < allyNum; i++)
    {
        newRand = arc4random_uniform(20) + 1;
        [AllyFieldArr[i] setIntValue:(newRand + [AllyBonusArr[i] intValue])];
    }
}

- (void)setOrder {
    NSInteger mobNum = [_MobNumberSel indexOfSelectedItem];
    NSInteger allyNum = [_AllyNumberSel indexOfSelectedItem];
    NSInteger num = mobNum + allyNum;
    
    NSMutableArray *order = [[NSMutableArray alloc] init];
    
    currentTurnIdx = 1;
    
    struct OrderType h1;
    for(int i = 0; i < PARTY_SIZE; i++)
    {
        h1.name = [HeroLabelArr[i] stringValue];
        h1.v = [HeroFieldArr[i] integerValue];
        [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
    }
    
    for(int i = 0; i < MOB_SIZE; i++)
    {
        h1.name = [MobLabelArr[i] stringValue];
        h1.v = [MobFieldArr[i] integerValue];
        [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
    }

    for(int i = 0; i < ALLY_SIZE; i++)
    {
        h1.name = [AllyLabelArr[i] stringValue];
        h1.v = [AllyFieldArr[i] integerValue];
        [order addObject:[NSValue valueWithBytes:&h1 objCType:@encode(struct OrderType)]];
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
    for(int i = 0; i < (num + PARTY_SIZE); i++)
    {
        [order[i] getValue:&val];
        s = val.name;
        [OrderFieldArr[i] setStringValue:s];
        
        if(i == 0) { [_CurrentTurnField setStringValue:s]; }
    }
             
}

- (void)setCurrentTurn {
    //TODO: Convert currentTurnIdx to be in the range 0..n-1 instead of 0..n
    NSTextField *f = OrderFieldArr[currentTurnIdx - 1];
    [_CurrentTurnField setStringValue:[f stringValue]];
}

- (void)advanceTurn {
    //TODO: 0 based index?
    NSInteger mobNum = [_MobNumberSel indexOfSelectedItem];
    NSInteger allyNum = [_AllyNumberSel indexOfSelectedItem];
    NSInteger num = mobNum + allyNum + PARTY_SIZE;
    
    if(currentTurnIdx == num)
    { currentTurnIdx = 1; }
    else { currentTurnIdx += 1; }
    
    [self setCurrentTurn];
}

- (void)initialization {
    
    PARTY_SIZE = 8;
    MOB_SIZE = 16;
    ALLY_SIZE = 4;
    ORDER_SIZE = 28;
    
    OrderFieldArr[0] = _Order1Field; OrderFieldArr[1] = _Order2Field;
    OrderFieldArr[2] = _Order3Field; OrderFieldArr[3] = _Order4Field;
    OrderFieldArr[4] = _Order5Field; OrderFieldArr[5] = _Order6Field;
    OrderFieldArr[6] = _Order7Field; OrderFieldArr[7] = _Order8Field;
    OrderFieldArr[8] = _Order9Field; OrderFieldArr[9] = _Order10Field;
    OrderFieldArr[10] = _Order11Field; OrderFieldArr[11] = _Order12Field;
    OrderFieldArr[12] = _Order13Field; OrderFieldArr[13] = _Order14Field;
    OrderFieldArr[14] = _Order15Field; OrderFieldArr[15] = _Order16Field;
    OrderFieldArr[16] = _Order17Field; OrderFieldArr[17] = _Order18Field;
    OrderFieldArr[18] = _Order19Field; OrderFieldArr[19] = _Order20Field;
    OrderFieldArr[20] = _Order21Field; OrderFieldArr[21] = _Order22Field;
    OrderFieldArr[22] = _Order23Field; OrderFieldArr[23] = _Order24Field;
    OrderFieldArr[24] = _Order25Field; OrderFieldArr[25] = _Order26Field;
    OrderFieldArr[26] = _Order27Field; OrderFieldArr[27] = _Order28Field;
    
    OrderNumArr[0] = _Order1Num; OrderNumArr[1] = _Order2Num;
    OrderNumArr[2] = _Order3Num; OrderNumArr[3] = _Order4Num;
    OrderNumArr[4] = _Order5Num; OrderNumArr[5] = _Order6Num;
    OrderNumArr[6] = _Order7Num; OrderNumArr[7] = _Order8Num;
    OrderNumArr[8] = _Order9Num; OrderNumArr[9] = _Order10Num;
    OrderNumArr[10] = _Order11Num; OrderNumArr[11] = _Order12Num;
    OrderNumArr[12] = _Order13Num; OrderNumArr[13] = _Order14Num;
    OrderNumArr[14] = _Order15Num; OrderNumArr[15] = _Order16Num;
    OrderNumArr[16] = _Order17Num; OrderNumArr[17] = _Order18Num;
    OrderNumArr[18] = _Order19Num; OrderNumArr[19] = _Order20Num;
    OrderNumArr[20] = _Order21Num; OrderNumArr[21] = _Order22Num;
    OrderNumArr[22] = _Order23Num; OrderNumArr[23] = _Order24Num;
    OrderNumArr[24] = _Order25Num; OrderNumArr[25] = _Order26Num;
    OrderNumArr[26] = _Order27Num; OrderNumArr[27] = _Order28Num;
        
    MobLabelArr[0] = _Mob1Label; MobLabelArr[1] = _Mob2Label;
    MobLabelArr[2] = _Mob3Label; MobLabelArr[3] = _Mob4Label;
    MobLabelArr[4] = _Mob5Label; MobLabelArr[5] = _Mob6Label;
    MobLabelArr[6] = _Mob7Label; MobLabelArr[7] = _Mob8Label;
    MobLabelArr[8] = _Mob9Label; MobLabelArr[9] = _Mob10Label;
    MobLabelArr[10] = _Mob11Label; MobLabelArr[11] = _Mob12Label;
    MobLabelArr[12] = _Mob13Label; MobLabelArr[13] = _Mob14Label;
    MobLabelArr[14] = _Mob15Label; MobLabelArr[15] = _Mob16Label;
        
    MobFieldArr[0] = _Mob1Field; MobFieldArr[1] = _Mob2Field;
    MobFieldArr[2] = _Mob3Field; MobFieldArr[3] = _Mob4Field;
    MobFieldArr[4] = _Mob5Field; MobFieldArr[5] = _Mob6Field;
    MobFieldArr[6] = _Mob7Field; MobFieldArr[7] = _Mob8Field;
    MobFieldArr[8] = _Mob9Field; MobFieldArr[9] = _Mob10Field;
    MobFieldArr[10] = _Mob11Field; MobFieldArr[11] = _Mob12Field;
    MobFieldArr[12] = _Mob13Field; MobFieldArr[13] = _Mob14Field;
    MobFieldArr[14] = _Mob15Field; MobFieldArr[15] = _Mob16Field;
        
    MobBonusArr[0] = _Mob1Bonus; MobBonusArr[1] = _Mob2Bonus;
    MobBonusArr[2] = _Mob3Bonus; MobBonusArr[3] = _Mob4Bonus;
    MobBonusArr[4] = _Mob5Bonus; MobBonusArr[5] = _Mob6Bonus;
    MobBonusArr[6] = _Mob7Bonus; MobBonusArr[7] = _Mob8Bonus;
    MobBonusArr[8] = _Mob9Bonus; MobBonusArr[9] = _Mob10Bonus;
    MobBonusArr[10] = _Mob11Bonus; MobBonusArr[11] = _Mob12Bonus;
    MobBonusArr[12] = _Mob13Bonus; MobBonusArr[13] = _Mob14Bonus;
    MobBonusArr[14] = _Mob15Bonus; MobBonusArr[15] = _Mob16Bonus;
    
    HeroLabelArr[0] = _Hero1Label; HeroLabelArr[1] = _Hero2Label;
    HeroLabelArr[2] = _Hero3Label; HeroLabelArr[3] = _Hero4Label;
    HeroLabelArr[4] = _Hero5Label; HeroLabelArr[5] = _Hero6Label;
    HeroLabelArr[6] = _Hero7Label; HeroLabelArr[7] = _Hero8Label;
        
    HeroFieldArr[0] = _Hero1Field; HeroFieldArr[1] = _Hero2Field;
    HeroFieldArr[2] = _Hero3Field; HeroFieldArr[3] = _Hero4Field;
    HeroFieldArr[4] = _Hero5Field; HeroFieldArr[5] = _Hero6Field;
    HeroFieldArr[6] = _Hero7Field; HeroFieldArr[7] = _Hero8Field;
    
    AllyLabelArr[0] = _Ally1Label; AllyLabelArr[1] = _Ally2Label;
    AllyLabelArr[2] = _Ally3Label; AllyLabelArr[3] = _Ally4Label;

    AllyFieldArr[0] = _Ally1Field; AllyFieldArr[1] = _Ally2Field;
    AllyFieldArr[2] = _Ally3Field; AllyFieldArr[3] = _Ally4Field;
    
    AllyBonusArr[0] = _Ally1Bonus; AllyBonusArr[1] = _Ally2Bonus;
    AllyBonusArr[2] = _Ally3Bonus; AllyBonusArr[3] = _Ally4Bonus;
    
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
    
    setICF(_AllyNumberSel, _AllyNumberLabel, "Ally Count",
           CGPointMake(500, 840), LABEL_UP);
    
    setICF(_Hero1Field, _Hero1Label, "Gremag", CGPointMake(525, 810), LABEL_LEFT);
    setICF(_Hero2Field, _Hero2Label, "Federico", CGPointMake(525, 780), LABEL_LEFT);
    setICF(_Hero3Field, _Hero3Label, "Ken Shiro", CGPointMake(525, 750), LABEL_LEFT);
    setICF(_Hero4Field, _Hero4Label, "Sirion", CGPointMake(525, 720), LABEL_LEFT);
    setICF(_Hero5Field, _Hero5Label, "Albion", CGPointMake(525, 690), LABEL_LEFT);
    setICF(_Hero6Field, _Hero6Label, "Sdentato", CGPointMake(525, 660), LABEL_LEFT);
    setICF(_Hero7Field, _Hero7Label, "Zakkhyr", CGPointMake(525, 630), LABEL_LEFT);
    setICF(_Hero8Field, _Hero8Label, "Zoddak", CGPointMake(525, 600), LABEL_LEFT);
       
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

    setICF(_Ally1Bonus, _Ally1Label, "Ally 1", CGPointMake(525, 560), LABEL_LEFT);
    setICF(_Ally2Bonus, _Ally2Label, "Ally 2", CGPointMake(525, 530), LABEL_LEFT);
    setICF(_Ally3Bonus, _Ally3Label, "Ally 3", CGPointMake(525, 500), LABEL_LEFT);
    setICF(_Ally4Bonus, _Ally4Label, "Ally 4", CGPointMake(525, 470), LABEL_LEFT);
    
    setField(_Ally1Field, CGPointMake(565, 560));
    setField(_Ally2Field, CGPointMake(565, 530));
    setField(_Ally3Field, CGPointMake(565, 500));
    setField(_Ally4Field, CGPointMake(565, 470));
    
    mask0(_RaceSelector); mask0(_ClassSelector); mask0(_GenMethodSel);
    mask0(_STRBonus); mask0(_DEXBonus); mask0(_CONBonus); mask0(_INTBonus);
    mask0(_WISBonus); mask0(_CHABonus);
    
    mask0(_LvlField); mask0(_ExpField); mask0(_NextXPField); mask0(_XPCurveSel);
    mask0(_BABField); mask0(_FortField); mask0(_RefField); mask0(_WillField);
    mask0(_RaceTraitsField);

    mask0(_MobNumberSel); mask0(_InitiativeRollButton);
    mask0(_AllyNumberSel);
    
    mask0(_Mob1Field); mask0(_Mob2Field); mask0(_Mob3Field); mask0(_Mob4Field);
    mask0(_Mob5Field); mask0(_Mob6Field); mask0(_Mob7Field); mask0(_Mob8Field);
    mask0(_Mob9Field); mask0(_Mob10Field); mask0(_Mob11Field);
    mask0(_Mob12Field); mask0(_Mob13Field); mask0(_Mob14Field);
    mask0(_Mob15Field); mask0(_Mob16Field);
    
    mask0(_Hero1Field); mask0(_Hero2Field); mask0(_Hero3Field);
    mask0(_Hero4Field); mask0(_Hero5Field); mask0(_Hero6Field);
    
    mask0(_Ally1Field); mask0(_Ally2Field);
    mask0(_Ally3Field); mask0(_Ally4Field);
    
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
    
    mask0andFormat(_Order1Field, nf); mask0andFormat(_Order2Field, nf);
    mask0andFormat(_Order3Field, nf); mask0andFormat(_Order4Field, nf);
    mask0andFormat(_Order5Field, nf); mask0andFormat(_Order6Field, nf);
    mask0andFormat(_Order7Field, nf); mask0andFormat(_Order8Field, nf);
    mask0andFormat(_Order9Field, nf); mask0andFormat(_Order10Field, nf);
    mask0andFormat(_Order11Field, nf); mask0andFormat(_Order12Field, nf);
    mask0andFormat(_Order13Field, nf); mask0andFormat(_Order14Field, nf);
    mask0andFormat(_Order15Field, nf); mask0andFormat(_Order16Field, nf);
    mask0andFormat(_Order17Field, nf); mask0andFormat(_Order18Field, nf);
    mask0andFormat(_Order19Field, nf); mask0andFormat(_Order20Field, nf);
    mask0andFormat(_Order21Field, nf); mask0andFormat(_Order22Field, nf);
    mask0andFormat(_Order23Field, nf); mask0andFormat(_Order24Field, nf);
    mask0andFormat(_Order25Field, nf); mask0andFormat(_Order26Field, nf);
    mask0andFormat(_Order27Field, nf); mask0andFormat(_Order28Field, nf);
    //NOTE: This is the solution to fullscreen correct positioning.
    //[_Order1Field setAutoresizingMask:NSViewMinXMargin];
    
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
    [_AllyNumberSel removeAllItems];
     
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
    [_Order23Num setAlignment:NSTextAlignmentLeft];
    [_Order24Num setAlignment:NSTextAlignmentLeft];
    [_Order25Num setAlignment:NSTextAlignmentLeft];
    [_Order26Num setAlignment:NSTextAlignmentLeft];
    [_Order27Num setAlignment:NSTextAlignmentLeft];
    [_Order28Num setAlignment:NSTextAlignmentLeft];
    
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
    
    [_Ally1Field setAlignment:NSTextAlignmentLeft];
    [_Ally2Field setAlignment:NSTextAlignmentLeft];
    [_Ally3Field setAlignment:NSTextAlignmentLeft];
    [_Ally4Field setAlignment:NSTextAlignmentLeft];
    
    [_Ally1Bonus setAlignment:NSTextAlignmentLeft];
    [_Ally2Bonus setAlignment:NSTextAlignmentLeft];
    [_Ally3Bonus setAlignment:NSTextAlignmentLeft];
    [_Ally4Bonus setAlignment:NSTextAlignmentLeft];

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
    [_Order23Field setEditable:false];
    [_Order24Field setEditable:false];
    [_Order25Field setEditable:false];
    [_Order26Field setEditable:false];
    [_Order27Field setEditable:false];
    [_Order28Field setEditable:false];
    
    [_CurrentTurnField setEditable:false];
}

- (void)showOrderFields:(NSInteger)num {
    for(int i = 0; i < num + PARTY_SIZE; i++)
    {
        [OrderNumArr[i] setHidden:false];
        [OrderFieldArr[i] setHidden:false];
    }
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
    [_Order23Num setHidden:true]; [_Order23Field setHidden:true];
    [_Order24Num setHidden:true]; [_Order24Field setHidden:true];
    [_Order25Num setHidden:true]; [_Order25Field setHidden:true];
    [_Order26Num setHidden:true]; [_Order26Field setHidden:true];
    [_Order27Num setHidden:true]; [_Order27Field setHidden:true];
    [_Order28Num setHidden:true]; [_Order28Field setHidden:true];
    
    [_Ally1Field setHidden:true]; [_Ally2Field setHidden:true];
    [_Ally3Field setHidden:true]; [_Ally4Field setHidden:true];
    [_Ally1Label setHidden:true]; [_Ally2Label setHidden:true];
    [_Ally3Label setHidden:true]; [_Ally4Label setHidden:true];
    [_Ally1Bonus setHidden:true]; [_Ally2Bonus setHidden:true];
    [_Ally3Bonus setHidden:true]; [_Ally4Bonus setHidden:true];
    
}

- (void)hideAllies {
    for(int i = 0; i < ALLY_SIZE; i++)
    {
        [AllyLabelArr[i] setHidden:true];
        [AllyBonusArr[i] setHidden:true];
        [AllyFieldArr[i] setHidden:true];
    }
}

- (void)hideMobs {
    for(int i = 0; i < MOB_SIZE; i++)
    {
        [MobLabelArr[i] setHidden:true];
        [MobFieldArr[i] setHidden:true];
    }
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
    
    int yPos = 810;
    for(int i = 0; i < ORDER_SIZE; i++)
    {
        setField(OrderNumArr[i], CGPointMake(640, yPos));
        setField(OrderFieldArr[i], CGPointMake(680, yPos));
        yPos -= 25;
    }
    
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
    
    NSArray *AllyCount = @[@"No Allies", @"1 Ally", @"2 Allies", @"3 Allies", @"4 Allies"];
    [_AllyNumberSel addItemsWithObjectValues:AllyCount];
        
    NSArray *AB = @[ @"-5", @"-5", @"-4", @"-4", @"-3", @"-3", @"-2", @"-2", @"-1", @"-1", @"0", @"0", @"+1", @"+1", @"+2", @"+2", @"+3", @"+3", @"+4", @"+4", @"+5", @"+5", @"+6", @"+6", @"+7", @"+7", @"+8", @"+8", @"+9", @"+9", @"+10", @"+10", @"+11", @"+11", @"+12", @"+12", @"+13", @"+13", @"+14", @"+14", @"+15", @"+15", @"+16", @"+16", @"+17", @"+17"];
    _AbilityBonus = AB;

    [_RaceSelector selectItemAtIndex:0];
    [_ClassSelector selectItemAtIndex:0];
    [_GenMethodSel selectItemAtIndex:0];
    [_MobNumberSel selectItemAtIndex:0];
    [_AllyNumberSel selectItemAtIndex:0];
    
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
    
    [_Ally1Bonus setIntegerValue:0]; [_Ally2Bonus setIntegerValue:0];
    [_Ally3Bonus setIntegerValue:0]; [_Ally4Bonus setIntegerValue:0];
    
    for(int i = 0; i < ORDER_SIZE; i++)
    { [OrderNumArr[i] setIntegerValue:(i+1)]; }
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    // Update the view, if already loaded.
}

@end
