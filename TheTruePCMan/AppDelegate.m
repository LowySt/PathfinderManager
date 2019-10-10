//
//  AppDelegate.m
//  TheTruePCMan
//
//  Created by Lowy on 09/10/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)controlTextDidChange:(NSNotification *)obj {
   ViewController *v = (ViewController *) NSApplication.sharedApplication.orderedWindows.firstObject.contentViewController;
    
    NSTextField *t = obj.object;
    
    int abIdx = t.intValue;
    if(t.intValue > v.AbilityBonus.count || t.intValue < 0)
    { abIdx = 0; }
    NSString *ab = v.AbilityBonus[abIdx];

    if(t.identifier == v.STRField.identifier)
    { [v.STRBonus setStringValue:ab]; }
    else if(t.identifier == v.DEXField.identifier)
    { [v.DEXBonus setStringValue:ab]; }
    else if(t.identifier == v.CONField.identifier)
    { [v.CONBonus setStringValue:ab]; }
    else if(t.identifier == v.INTField.identifier)
    { [v.INTBonus setStringValue:ab]; }
    else if(t.identifier == v.WISField.identifier)
    { [v.WISBonus setStringValue:ab]; }
    else if(t.identifier == v.CHAField.identifier)
    { [v.CHABonus setStringValue:ab]; }
    
}

- (void)controlTextDidEndEditing:(NSNotification *)obj {
    ViewController *v = (ViewController *) NSApplication.sharedApplication.orderedWindows.firstObject.contentViewController;
    
    NSTextField *t = obj.object;
    
    if(t.identifier == v.LvlField.identifier)
    {
        setClassStats(v, (enum GameClass)[v.ClassSelector indexOfSelectedItem]);
    }

}

- (void)comboBoxSelectionDidChange:(NSNotification *)notification {
    ViewController *v = (ViewController *) NSApplication.sharedApplication.orderedWindows.firstObject.contentViewController;

    NSComboBox *t = notification.object;
            
    if(t.identifier == v.RaceSelector.identifier)
    {
        NSInteger i = [v.RaceSelector indexOfSelectedItem];
        
        switch(i)
        {
            case RACE_DWARF: [v.RaceTraitsField setStringValue:DwarfTraits]; break;
            case RACE_ELF: [v.RaceTraitsField setStringValue:ElfTraits]; break;
            case RACE_GNOME: [v.RaceTraitsField setStringValue:GnomeTraits]; break;
            case RACE_HALF_ELF: [v.RaceTraitsField setStringValue:HalfElfTraits]; break;
            case RACE_HALF_ORC: [v.RaceTraitsField setStringValue:HalfOrcTraits]; break;
            case RACE_HALFLING: [v.RaceTraitsField setStringValue:HalflingTraits]; break;
            case RACE_HUMAN: [v.RaceTraitsField setStringValue:HumanTraits]; break;
        }
    }
    else if(t.identifier == v.ClassSelector.identifier)
    {
        NSInteger i = [v.ClassSelector indexOfSelectedItem];
        
        switch(i)
        {
            case CLASS_BARBARIAN: setClassStats(v, CLASS_BARBARIAN); break;
            case CLASS_BARD: setClassStats(v, CLASS_BARD); break;
            case CLASS_CLERIC: setClassStats(v, CLASS_CLERIC); break;
            case CLASS_DRUID: setClassStats(v, CLASS_DRUID); break;
            case CLASS_FIGHTER: setClassStats(v, CLASS_FIGHTER); break;
            case CLASS_MONK: setClassStats(v, CLASS_MONK); break;
            case CLASS_PALADIN: setClassStats(v, CLASS_PALADIN); break;
            case CLASS_RANGER: setClassStats(v, CLASS_RANGER); break;
            case CLASS_ROGUE: setClassStats(v, CLASS_ROGUE); break;
            case CLASS_SORCERER: setClassStats(v, CLASS_SORCERER); break;
            case CLASS_WIZARD: setClassStats(v, CLASS_WIZARD); break;
        }
    }

}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
   ViewController *v = (ViewController *) NSApplication.sharedApplication.orderedWindows.firstObject.contentViewController;

    [v.NameField setDelegate:self];
    [v.PlayerField setDelegate:self];
    
    [v.STRField setDelegate:self];
    [v.DEXField setDelegate:self];
    [v.CONField setDelegate:self];
    [v.INTField setDelegate:self];
    [v.WISField setDelegate:self];
    [v.CHAField setDelegate:self];
    
    [v.RaceSelector setDelegate:self];
    [v.ClassSelector setDelegate:self];
    [v.GenMethodSel setDelegate:self];
    
    [v.LvlField setDelegate:self];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
