//
//  ViewController.h
//  TheTruePCMan
//
//  Created by Lowy on 09/10/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ViewController : NSViewController

@property (strong) IBOutlet NSView *MainView;

@property (strong) NSArray *AbilityBonus;

@property (weak) IBOutlet NSTextField *NameLabel;
@property (weak) IBOutlet NSTextField *NameField;

@property (weak) IBOutlet NSTextField *PlayerLabel;
@property (weak) IBOutlet NSTextField *PlayerField;

@property (weak) IBOutlet NSTextField *RaceLabel;
@property (weak) IBOutlet NSComboBox *RaceSelector;
@property (weak) IBOutlet NSTextField *ClassLabel;
@property (weak) IBOutlet NSComboBox *ClassSelector;

@property (weak) IBOutlet NSTextField *GenMethodLabel;
@property (weak) IBOutlet NSComboBox *GenMethodSel;

@property (weak) IBOutlet NSTextField *STRLabel;
@property (weak) IBOutlet NSTextField *STRField;
@property (weak) IBOutlet NSTextField *STRBonus;

@property (weak) IBOutlet NSTextField *DEXLabel;
@property (weak) IBOutlet NSTextField *DEXField;
@property (weak) IBOutlet NSTextField *DEXBonus;

@property (weak) IBOutlet NSTextField *CONLabel;
@property (weak) IBOutlet NSTextField *CONField;
@property (weak) IBOutlet NSTextField *CONBonus;

@property (weak) IBOutlet NSTextField *INTLabel;
@property (weak) IBOutlet NSTextField *INTField;
@property (weak) IBOutlet NSTextField *INTBonus;

@property (weak) IBOutlet NSTextField *WISLabel;
@property (weak) IBOutlet NSTextField *WISField;
@property (weak) IBOutlet NSTextField *WISBonus;

@property (weak) IBOutlet NSTextField *CHALabel;
@property (weak) IBOutlet NSTextField *CHAField;
@property (weak) IBOutlet NSTextField *CHABonus;

@property (weak) IBOutlet NSTextField *LvlLabel;
@property (weak) IBOutlet NSTextField *LvlField;

@property (weak) IBOutlet NSTextField *ExpLabel;
@property (weak) IBOutlet NSTextField *ExpField;

@property (weak) IBOutlet NSTextField *NextXPLabel;
@property (weak) IBOutlet NSTextField *NextXPField;

@property (weak) IBOutlet NSTextField *XPCurveLabel;
@property (weak) IBOutlet NSComboBox *XPCurveSel;

@property (weak) IBOutlet NSTextField *BABLabel;
@property (weak) IBOutlet NSTextField *BABField;

@property (weak) IBOutlet NSTextField *FortLabel;
@property (weak) IBOutlet NSTextField *FortField;

@property (weak) IBOutlet NSTextField *RefLabel;
@property (weak) IBOutlet NSTextField *RefField;

@property (weak) IBOutlet NSTextField *WillLabel;
@property (weak) IBOutlet NSTextField *WillField;

@property (weak) IBOutlet NSScrollView *RacialTraitsField;


@end

