//
//  ViewController.h
//  TheTruePCMan
//
//  Created by Lowy on 09/10/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ViewController : NSViewController

- (void)hideFields;

@property (strong) IBOutlet NSView *MainView;

// PC Tab //

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

@property (weak) IBOutlet NSTextField *RaceTraitsField;

@property (weak) IBOutlet NSTabView *TabController;

// Feats Tab //

@property (weak) IBOutlet NSTableView *FeatsTable;

// Initiative Tab //

@property (weak) IBOutlet NSTextField *MobNumberLabel;
@property (weak) IBOutlet NSComboBox *MobNumberSel;

@property (weak) IBOutlet NSButton *InitiativeRollButton;

@property (weak) IBOutlet NSTextField *Mob1Label;
@property (weak) IBOutlet NSTextField *Mob1Field;
@property (weak) IBOutlet NSTextField *Mob1Bonus;

@property (weak) IBOutlet NSTextField *Mob2Label;
@property (weak) IBOutlet NSTextField *Mob2Field;
@property (weak) IBOutlet NSTextField *Mob2Bonus;

@property (weak) IBOutlet NSTextField *Mob3Label;
@property (weak) IBOutlet NSTextField *Mob3Field;
@property (weak) IBOutlet NSTextField *Mob3Bonus;

@property (weak) IBOutlet NSTextField *Mob4Label;
@property (weak) IBOutlet NSTextField *Mob4Field;
@property (weak) IBOutlet NSTextField *Mob4Bonus;

@property (weak) IBOutlet NSTextField *Mob5Label;
@property (weak) IBOutlet NSTextField *Mob5Field;
@property (weak) IBOutlet NSTextField *Mob5Bonus;

@property (weak) IBOutlet NSTextField *Mob6Label;
@property (weak) IBOutlet NSTextField *Mob6Field;
@property (weak) IBOutlet NSTextField *Mob6Bonus;

@property (weak) IBOutlet NSTextField *Mob7Label;
@property (weak) IBOutlet NSTextField *Mob7Field;
@property (weak) IBOutlet NSTextField *Mob7Bonus;

@property (weak) IBOutlet NSTextField *Mob8Label;
@property (weak) IBOutlet NSTextField *Mob8Field;
@property (weak) IBOutlet NSTextField *Mob8Bonus;

@property (weak) IBOutlet NSTextField *Mob9Label;
@property (weak) IBOutlet NSTextField *Mob9Field;
@property (weak) IBOutlet NSTextField *Mob9Bonus;

@property (weak) IBOutlet NSTextField *Mob10Label;
@property (weak) IBOutlet NSTextField *Mob10Field;
@property (weak) IBOutlet NSTextField *Mob10Bonus;

@property (weak) IBOutlet NSTextField *Mob11Label;
@property (weak) IBOutlet NSTextField *Mob11Field;
@property (weak) IBOutlet NSTextField *Mob11Bonus;

@property (weak) IBOutlet NSTextField *Mob12Label;
@property (weak) IBOutlet NSTextField *Mob12Field;
@property (weak) IBOutlet NSTextField *Mob12Bonus;

@property (weak) IBOutlet NSTextField *Mob13Label;
@property (weak) IBOutlet NSTextField *Mob13Field;
@property (weak) IBOutlet NSTextField *Mob13Bonus;

@property (weak) IBOutlet NSTextField *Mob14Label;
@property (weak) IBOutlet NSTextField *Mob14Field;
@property (weak) IBOutlet NSTextField *Mob14Bonus;

@property (weak) IBOutlet NSTextField *Mob15Label;
@property (weak) IBOutlet NSTextField *Mob15Field;
@property (weak) IBOutlet NSTextField *Mob15Bonus;

@property (weak) IBOutlet NSTextField *Mob16Label;
@property (weak) IBOutlet NSTextField *Mob16Field;
@property (weak) IBOutlet NSTextField *Mob16Bonus;

@property (weak) IBOutlet NSTextField *Hero1Label;
@property (weak) IBOutlet NSTextField *Hero1Field;

@property (weak) IBOutlet NSTextField *Hero2Label;
@property (weak) IBOutlet NSTextField *Hero2Field;

@property (weak) IBOutlet NSTextField *Hero3Label;
@property (weak) IBOutlet NSTextField *Hero3Field;

@property (weak) IBOutlet NSTextField *Hero4Label;
@property (weak) IBOutlet NSTextField *Hero4Field;

@property (weak) IBOutlet NSTextField *Hero5Label;
@property (weak) IBOutlet NSTextField *Hero5Field;

@property (weak) IBOutlet NSTextField *Hero6Label;
@property (weak) IBOutlet NSTextField *Hero6Field;

@property (weak) IBOutlet NSTextField *Order1Num;
@property (weak) IBOutlet NSTextField *Order1Field;

@property (weak) IBOutlet NSTextField *Order2Num;
@property (weak) IBOutlet NSTextField *Order2Field;

@property (weak) IBOutlet NSTextField *Order3Num;
@property (weak) IBOutlet NSTextField *Order3Field;

@property (weak) IBOutlet NSTextField *Order4Num;
@property (weak) IBOutlet NSTextField *Order4Field;

@property (weak) IBOutlet NSTextField *Order5Num;
@property (weak) IBOutlet NSTextField *Order5Field;

@property (weak) IBOutlet NSTextField *Order6Num;
@property (weak) IBOutlet NSTextField *Order6Field;

@property (weak) IBOutlet NSTextField *Order7Num;
@property (weak) IBOutlet NSTextField *Order7Field;

@property (weak) IBOutlet NSTextField *Order8Num;
@property (weak) IBOutlet NSTextField *Order8Field;

@property (weak) IBOutlet NSTextField *Order9Num;
@property (weak) IBOutlet NSTextField *Order9Field;

@property (weak) IBOutlet NSTextField *Order10Num;
@property (weak) IBOutlet NSTextField *Order10Field;

@property (weak) IBOutlet NSTextField *Order11Num;
@property (weak) IBOutlet NSTextField *Order11Field;

@property (weak) IBOutlet NSTextField *Order12Num;
@property (weak) IBOutlet NSTextField *Order12Field;

@property (weak) IBOutlet NSTextField *Order13Num;
@property (weak) IBOutlet NSTextField *Order13Field;

@property (weak) IBOutlet NSTextField *Order14Num;
@property (weak) IBOutlet NSTextField *Order14Field;

@property (weak) IBOutlet NSTextField *Order15Num;
@property (weak) IBOutlet NSTextField *Order15Field;

@property (weak) IBOutlet NSTextField *Order16Num;
@property (weak) IBOutlet NSTextField *Order16Field;

@property (weak) IBOutlet NSTextField *Order17Num;
@property (weak) IBOutlet NSTextField *Order17Field;

@property (weak) IBOutlet NSTextField *Order18Num;
@property (weak) IBOutlet NSTextField *Order18Field;

@property (weak) IBOutlet NSTextField *Order19Num;
@property (weak) IBOutlet NSTextField *Order19Field;

@property (weak) IBOutlet NSTextField *Order20Num;
@property (weak) IBOutlet NSTextField *Order20Field;

@property (weak) IBOutlet NSTextField *Order21Num;
@property (weak) IBOutlet NSTextField *Order21Field;

@property (weak) IBOutlet NSTextField *Order22Num;
@property (weak) IBOutlet NSTextField *Order22Field;

@property (weak) IBOutlet NSButton *SetOrderButton;

@property (weak) IBOutlet NSButton *AdvanceTurnButton;
@property (weak) IBOutlet NSTextField *CurrentTurnField;


@end

