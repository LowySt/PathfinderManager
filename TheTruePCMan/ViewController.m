//
//  ViewController.m
//  TheTruePCMan
//
//  Created by Lowy on 09/10/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "ViewController.h"

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

@implementation ViewController

- (void)initialization {
    _RaceSelector.autoresizingMask = 0;
    _ClassSelector.autoresizingMask = 0;
    _GenMethodSel.autoresizingMask = 0;
    _STRBonus.autoresizingMask = 0;
    _DEXBonus.autoresizingMask = 0;
    _CONBonus.autoresizingMask = 0;
    _INTBonus.autoresizingMask = 0;
    _WISBonus.autoresizingMask = 0;
    _CHABonus.autoresizingMask = 0;
    _LvlField.autoresizingMask = 0;
    _ExpField.autoresizingMask = 0;
    _NextXPField.autoresizingMask = 0;
    _XPCurveSel.autoresizingMask = 0;
    _BABField.autoresizingMask = 0;
    _FortField.autoresizingMask = 0;
    _RefField.autoresizingMask = 0;
    _WillField.autoresizingMask = 0;
    _RaceTraitsField.autoresizingMask = 0;
        
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
    
    NSNumber *minLevel = [[NSNumber alloc] initWithInt:1];
    NSNumber *maxLevel = [[NSNumber alloc] initWithInt:20];
    NSNumberFormatter *lvlF = [[NSNumberFormatter alloc] init];
    [lvlF setAllowsFloats:false];
    [lvlF setNumberStyle:NSNumberFormatterNoStyle];
    [lvlF setMinimum:minLevel];
    [lvlF setMaximum:maxLevel];
    [lvlF setPartialStringValidationEnabled:true];
       
    _LvlField.formatter = lvlF;
    
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
    setFieldAndLabel(_NextXPField, _NextXPLabel, CGPointMake(600, 810), LABEL_UP);
    setFieldAndLabel(_XPCurveSel, _XPCurveLabel, CGPointMake(660, 807), LABEL_UP);
    
    setFieldAndLabel(_BABField, _BABLabel, CGPointMake(540, 730), LABEL_LEFT);
    setFieldAndLabel(_FortField, _FortLabel, CGPointMake(540, 690), LABEL_LEFT);
    setFieldAndLabel(_RefField, _RefLabel, CGPointMake(540, 650), LABEL_LEFT);
    setFieldAndLabel(_WillField, _WillLabel, CGPointMake(540, 610), LABEL_LEFT);
            
    setField(_RaceTraitsField, CGPointMake(65, 220));
    
    NSArray *Races = @[ @"Dwarf", @"Elf", @"Gnome", @"Half-Elf", @"Half-Orc", @"Halfling", @"Human"];
    [_RaceSelector addItemsWithObjectValues:Races];
    
    NSArray *Classes = @[ @"Barbarian", @"Bard", @"Cleric", @"Druid", @"Fighter", @"Monk", @"Paladin", @"Ranger", @"Rogue", @"Sorcerer", @"Wizard" ];
    [_ClassSelector addItemsWithObjectValues:Classes];
            
    NSArray *GenMethods = @[ @"Classic", @"Dice Pool", @"Heroic", @"Purchase", @"Standard"];
    [_GenMethodSel addItemsWithObjectValues:GenMethods];

    NSArray *XPCurves = @[ @"Fast", @"Medium", @"Slow" ];
    [_XPCurveSel addItemsWithObjectValues:XPCurves];
    
    NSArray *AB = @[ @"-5", @"-5", @"-4", @"-4", @"-3", @"-3", @"-2", @"-2", @"-1", @"-1", @"0", @"0", @"+1", @"+1", @"+2", @"+2", @"+3", @"+3", @"+4", @"+4", @"+5", @"+5", @"+6", @"+6", @"+7", @"+7", @"+8", @"+8", @"+9", @"+9", @"+10", @"+10", @"+11", @"+11", @"+12", @"+12", @"+13", @"+13", @"+14", @"+14", @"+15", @"+15", @"+16", @"+16", @"+17", @"+17"];
    _AbilityBonus = AB;
    
    [_LvlField setStringValue:@"1"];
    [_ExpField setStringValue:@"0"];
    [_XPCurveSel selectItemAtIndex:0];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    // Update the view, if already loaded.
}

@end
