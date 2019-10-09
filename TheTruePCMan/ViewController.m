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
    CGPoint labelP; \
    switch(d) \
    { \
        case LABEL_UP: { labelP = CGPointMake(p.x, p.y + 30); } break; \
        case LABEL_DOWN: { labelP = CGPointMake(p.x, p.y - 60); } break; \
        case LABEL_LEFT: { labelP = CGPointMake(p.x - 40, p.y); } break; \
        case LABEL_RIGHT: { labelP = CGPointMake(p.x + 100, p.y); } break; \
        default: \
        { assert(false); } break; \
    } \
    [f setFrameOrigin:p]; [l setFrameOrigin:labelP]; \
}

/*void setFieldAndLabelT(NSTextField *f, NSTextField *l, CGPoint p, enum LabelDir d)
{
    CGPoint labelP;
    switch(d)
    {
        case LABEL_UP: { labelP = CGPointMake(p.x, p.y + 40); } break;
        case LABEL_DOWN: { labelP = CGPointMake(p.x, p.y - 60); } break;
        case LABEL_LEFT: { labelP = CGPointMake(p.x - 20, p.y); } break;
        case LABEL_RIGHT: { labelP = CGPointMake(p.x + 50, p.y); } break;
        default:
        { assert(false); } break;
    }
    [f setFrameOrigin:p]; [l setFrameOrigin:labelP];
}*/

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.

    _RaceSelector.autoresizingMask = 0;
    _ClassSelector.autoresizingMask = 0;
    _GenMethodSel.autoresizingMask = 0;
    
    setFieldAndLabel(_NameField, _NameLabel, CGPointMake(65, 810), LABEL_LEFT);
    setFieldAndLabel(_PlayerField, _PlayerLabel, CGPointMake(65, 780), LABEL_LEFT);
               
    setFieldAndLabel(_RaceSelector, _RaceLabel, CGPointMake(65, 750), LABEL_LEFT);
    setFieldAndLabel(_ClassSelector, _ClassLabel, CGPointMake(65, 720), LABEL_LEFT);
    
    setFieldAndLabel(_GenMethodSel, _GenMethodLabel, CGPointMake(180, 810), LABEL_UP);
    setFieldAndLabel(_STRField, _STRLabel, CGPointMake(300, 810), LABEL_UP);
    setFieldAndLabel(_DEXField, _DEXLabel, CGPointMake(300, 780), LABEL_UP);
    setFieldAndLabel(_CONField, _CONLabel, CGPointMake(300, 750), LABEL_UP);
    setFieldAndLabel(_INTField, _INTLabel, CGPointMake(300, 720), LABEL_UP);
    setFieldAndLabel(_WISField, _WISLabel, CGPointMake(300, 690), LABEL_UP);
    setFieldAndLabel(_CHAField, _CHALabel, CGPointMake(300, 660), LABEL_UP);
    
    [_RaceSelector removeAllItems];
    [_ClassSelector removeAllItems];
    
    NSArray *Classes = @[ @"Barbarian", @"Bard", @"Cleric", @"Druid", @"Fighter", @"Monk", @"Paladin", @"Ranger", @"Rogue", @"Sorcerer", @"Wizard" ];
    [_RaceSelector addItemsWithObjectValues:Classes];
    
    NSArray *Races = @[ @"Human", @"Elf", @"Half-Orc", @"Half-Elf", @"Gnome", @"Halfling", @"Dwarf"];
    [_ClassSelector addItemsWithObjectValues:Races];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
