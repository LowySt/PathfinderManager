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

@property (weak) IBOutlet NSTextField *DEXLabel;
@property (weak) IBOutlet NSTextField *DEXField;

@property (weak) IBOutlet NSTextField *CONLabel;
@property (weak) IBOutlet NSTextField *CONField;

@property (weak) IBOutlet NSTextField *INTLabel;
@property (weak) IBOutlet NSTextField *INTField;

@property (weak) IBOutlet NSTextField *WISLabel;
@property (weak) IBOutlet NSTextField *WISField;

@property (weak) IBOutlet NSTextField *CHALabel;
@property (weak) IBOutlet NSTextField *CHAField;

@end

