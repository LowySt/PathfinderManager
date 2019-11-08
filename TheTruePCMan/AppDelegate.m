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

- (BOOL)control:(NSControl *)control textView:(NSTextView *)textView doCommandBySelector:(SEL)commandSelector {
    
    ViewController *v = (ViewController *) NSApplication.sharedApplication.orderedWindows.firstObject.contentViewController;
    
    if (commandSelector == @selector(insertNewline:)) {
        // Do something against ENTER key
        NSInteger allyNum = [v.AllyNumberSel indexOfSelectedItem];
        NSInteger mobNum = [v.MobNumberSel indexOfSelectedItem];
        
        for(int i = 0; i < allyNum; i++) {
            
            //NOTE: WHAT THE ACTUAL FUCK!?
            if([control.identifier isEqual:v->AllyLabelArr[i].identifier]) {
                if((i + 1) == allyNum) { return YES; }
                [v->AllyLabelArr[i+1] setStringValue:@""];
                [control.window makeFirstResponder:v->AllyLabelArr[i+1]];
                return YES;
            }
            
            if([control.identifier isEqual:v->AllyBonusArr[i].identifier]) {
                if((i + 1) == allyNum) { return YES; }
                [v->AllyBonusArr[i+1] setStringValue:@""];
                [control.window makeFirstResponder:v->AllyBonusArr[i+1]];
                return YES;
            }
            
            if([control.identifier isEqual:v->AllyFieldArr[i].identifier]) {
                if((i + 1) == allyNum) { return YES; }
                [v->AllyFieldArr[i+1] setStringValue:@""];
                [control.window makeFirstResponder:v->AllyFieldArr[i+1]];
                return YES;
            }
        }

        for(int i = 0; i < mobNum; i++) {
            
            if([control.identifier isEqual:v->MobLabelArr[i].identifier]) {
                if((i + 1) == mobNum) { return YES; }
                [v->MobLabelArr[i+1] setStringValue:@""];
                [control.window makeFirstResponder:v->MobLabelArr[i+1]];
                return YES;
            }
            
            if([control.identifier isEqual:v->MobBonusArr[i].identifier]) {
                if((i + 1) == mobNum) { return YES; }
                [v->MobBonusArr[i+1] setStringValue:@""];
                [control.window makeFirstResponder:v->MobBonusArr[i+1]];
                return YES;
            }
            
            if([control.identifier isEqual:v->MobFieldArr[i].identifier]) {
                if((i + 1) == mobNum) { return YES; }
                [v->MobFieldArr[i+1] setStringValue:@""];
                [control.window makeFirstResponder:v->MobFieldArr[i+1]];
                return YES;
            }

        }
        
        for(int i = 0; i < v->PARTY_SIZE; i++) {
            
            if([control.identifier isEqual:v->HeroFieldArr[i].identifier]) {
                if((i + 1) == v->PARTY_SIZE) { return YES; }
                [control.window makeFirstResponder:v->HeroFieldArr[i+1]];
                return YES;
            }
        }
        
        [control.window makeFirstResponder:control.window];
        return YES;
    }
    
    // return true if the action was handled; otherwise false
    return NO;
}

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
        setXPCurve(v, (enum XPCurveIdx)[v.XPCurveSel indexOfSelectedItem]);
    }
    
    else if(t.identifier == v.Order1Num.identifier)
      {
          int newPos = [v.Order1Num intValue];
          [v changePos:1 newPos:newPos];
          [v.Order1Num setStringValue:@"1"];
      }
      else if(t.identifier == v.Order2Num.identifier)
      {
          int newPos = [v.Order2Num intValue];
          [v changePos:2 newPos:newPos];
          [v.Order2Num setStringValue:@"2"];
      }
      else if(t.identifier == v.Order3Num.identifier)
      {
          int newPos = [v.Order3Num intValue];
          [v changePos:3 newPos:newPos];
          [v.Order3Num setStringValue:@"3"];
      }
      else if(t.identifier == v.Order4Num.identifier)
      {
          int newPos = [v.Order4Num intValue];
          [v changePos:4 newPos:newPos];
          [v.Order4Num setStringValue:@"4"];
      }
      else if(t.identifier == v.Order5Num.identifier)
      {
          int newPos = [v.Order5Num intValue];
          [v changePos:5 newPos:newPos];
          [v.Order5Num setStringValue:@"5"];
      }
      else if(t.identifier == v.Order6Num.identifier)
      {
          int newPos = [v.Order6Num intValue];
          [v changePos:6 newPos:newPos];
          [v.Order6Num setStringValue:@"6"];
      }
      else if(t.identifier == v.Order7Num.identifier)
      {
          int newPos = [v.Order7Num intValue];
          [v changePos:7 newPos:newPos];
          [v.Order7Num setStringValue:@"7"];
      }
      else if(t.identifier == v.Order8Num.identifier)
      {
          int newPos = [v.Order8Num intValue];
          [v changePos:8 newPos:newPos];
          [v.Order8Num setStringValue:@"8"];
      }
    else if(t.identifier == v.Order9Num.identifier)
    {
        int newPos = [v.Order9Num intValue];
        [v changePos:9 newPos:newPos];
        [v.Order9Num setStringValue:@"9"];
    }
    else if(t.identifier == v.Order10Num.identifier)
    {
        int newPos = [v.Order10Num intValue];
        [v changePos:10 newPos:newPos];
        [v.Order10Num setStringValue:@"10"];
    }
    else if(t.identifier == v.Order11Num.identifier)
    {
        int newPos = [v.Order11Num intValue];
        [v changePos:11 newPos:newPos];
        [v.Order11Num setStringValue:@"11"];
    }
    else if(t.identifier == v.Order12Num.identifier)
    {
        int newPos = [v.Order12Num intValue];
        [v changePos:12 newPos:newPos];
        [v.Order12Num setStringValue:@"12"];
    }
    else if(t.identifier == v.Order13Num.identifier)
    {
        int newPos = [v.Order13Num intValue];
        [v changePos:13 newPos:newPos];
        [v.Order13Num setStringValue:@"13"];
    }
    else if(t.identifier == v.Order14Num.identifier)
    {
        int newPos = [v.Order14Num intValue];
        [v changePos:14 newPos:newPos];
        [v.Order14Num setStringValue:@"14"];
    }
    else if(t.identifier == v.Order15Num.identifier)
    {
        int newPos = [v.Order15Num intValue];
        [v changePos:15 newPos:newPos];
        [v.Order15Num setStringValue:@"15"];
    }
    else if(t.identifier == v.Order16Num.identifier)
    {
        int newPos = [v.Order16Num intValue];
        [v changePos:16 newPos:newPos];
        [v.Order16Num setStringValue:@"16"];
    }
    else if(t.identifier == v.Order17Num.identifier)
    {
        int newPos = [v.Order17Num intValue];
        [v changePos:17 newPos:newPos];
        [v.Order17Num setStringValue:@"17"];
    }
    else if(t.identifier == v.Order18Num.identifier)
    {
        int newPos = [v.Order18Num intValue];
        [v changePos:18 newPos:newPos];
        [v.Order18Num setStringValue:@"18"];
    }
    else if(t.identifier == v.Order19Num.identifier)
    {
        int newPos = [v.Order19Num intValue];
        [v changePos:19 newPos:newPos];
        [v.Order19Num setStringValue:@"19"];
    }
    else if(t.identifier == v.Order20Num.identifier)
    {
        int newPos = [v.Order20Num intValue];
        [v changePos:20 newPos:newPos];
        [v.Order20Num setStringValue:@"20"];
    }
    else if(t.identifier == v.Order21Num.identifier)
    {
        int newPos = [v.Order21Num intValue];
        [v changePos:21 newPos:newPos];
        [v.Order21Num setStringValue:@"21"];
    }
    else if(t.identifier == v.Order22Num.identifier)
    {
        int newPos = [v.Order22Num intValue];
        [v changePos:22 newPos:newPos];
        [v.Order22Num setStringValue:@"22"];
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
            case CLASS_WIZARD:
                setClassStats(v, CLASS_WIZARD);
                break;
        }
    }
    else if(t.identifier == v.XPCurveSel.identifier)
    {
        NSInteger i = [v.XPCurveSel indexOfSelectedItem];
        
        switch(i)
        {
            case XP_FAST: setXPCurve(v, XP_FAST); break;
            case XP_MEDIUM: setXPCurve(v, XP_MEDIUM); break;
            case XP_SLOW: setXPCurve(v, XP_SLOW); break;
        }
    }
    else if(t.identifier == v.MobNumberSel.identifier)
    {
        [v setCurrentTurnIdx:1];
        NSInteger num = [v.MobNumberSel indexOfSelectedItem];
        
        [v hideMobs];
        [v setMobLabels];
        
        for(int i = 0; i < num; i++)
        {
            [v->MobFieldArr[i] setHidden:false];
            [v->MobBonusArr[i] setHidden:false];
            [v->MobLabelArr[i] setHidden:false];
            [v->SymbolCheckArr[0][i] setHidden:false];
        }
        
        NSInteger allyNum = [v.AllyNumberSel indexOfSelectedItem];
        
        [v showOrderFields:(num + allyNum)];
    }
    else if(t.identifier == v.AllyNumberSel.identifier)
    {
        [v setCurrentTurnIdx:1];
        NSInteger num = [v.AllyNumberSel indexOfSelectedItem];
        
        [v hideAllies];
        [v setAllyLabels];
        
        for(int i = 0; i < num; i++)
        {
            [v->AllyFieldArr[i] setHidden:false];
            [v->AllyBonusArr[i] setHidden:false];
            [v->AllyLabelArr[i] setHidden:false];
        }
        
        NSInteger mobNum = [v.MobNumberSel indexOfSelectedItem];
        
        [v showOrderFields:(num + mobNum)];
    }

}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
    return _nameArray.count;
}

- (NSView *)tableView:(NSTableView *)tableView
   viewForTableColumn:(NSTableColumn *)tableColumn
                  row:(NSInteger)row {
          
    // Get an existing cell with the MyView identifier if it exists
    NSTextField *result = [tableView makeViewWithIdentifier:@"MyView" owner:self];
 
    // There is no existing cell to reuse so create a new one
    if (result == nil) {
 
         // Create the new NSTextField with a frame of the {0,0} with the width of the table.
         // Note that the height of the frame is not really relevant, because the row height will modify the height.
         result = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 300, 20)];
 
         // The identifier of the NSTextField instance is set to MyView.
         // This allows the cell to be reused.
         result.identifier = @"MyView";
      }
 
      // result is now guaranteed to be valid, either as a reused cell
      // or as a new cell, so set the stringValue of the cell to the
      // nameArray value at row
      result.stringValue = [self.nameArray objectAtIndex:row];
 
      // Return the result
      return result;
 
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    ViewController *v = (ViewController *) NSApplication.sharedApplication.orderedWindows.firstObject.contentViewController;

    setClassStats(v, CLASS_BARBARIAN);
    [v.RaceTraitsField setStringValue:DwarfTraits];
    
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
    [v.XPCurveSel setDelegate:self];
    
    [v.LvlField setDelegate:self];
    
    [v.FeatsTable setDelegate:self];
    [v.MobNumberSel setDelegate:self];
    [v.AllyNumberSel setDelegate:self];
        
    for(int i = 0; i < v->MOB_SIZE; i++) {
        [v->MobLabelArr[i] setDelegate:self];
        [v->MobBonusArr[i] setDelegate:self];
        [v->MobFieldArr[i] setDelegate:self];
    }
    
    for(int i = 0; i < v->ALLY_SIZE; i++) {
        [v->AllyLabelArr[i] setDelegate:self];
        [v->AllyBonusArr[i] setDelegate:self];
        [v->AllyFieldArr[i] setDelegate:self];
    }
    
    for(int i = 0; i < v->PARTY_SIZE; i++)
    { [v->HeroFieldArr[i] setDelegate:self]; }
    
    for(int i = 0; i < v->ORDER_SIZE; i++)
    { [v->OrderNumArr[i] setDelegate:self]; }
       
    
    /*
    DrawView *view = [[DrawView alloc] init];
    [view setPath];
    
    NSWindowController *dw = [[NSStoryboard mainStoryboard] instantiateControllerWithIdentifier:@"DrawWin"];
    [dw showWindow:self];
    NSWindow *drawWin = dw.window;
    [drawWin.contentView addSubview:view];
    [view.superview setNeedsDisplay:true];
    
    [view setTranslatesAutoresizingMaskIntoConstraints:false];
        
    NSDictionary *d = @{@"view" : view};
    
    [drawWin.contentView addConstraint:[NSLayoutConstraint constraintsWithVisualFormat:@"H: |[view]|"
    options:NSLayoutFormatAlignmentMask metrics:nil
    views:d][0]];
   [drawWin.contentView addConstraint:[NSLayoutConstraint constraintsWithVisualFormat:@"V: |[view]|"
    options:NSLayoutFormatAlignmentMask metrics:nil
    views:d][0]];*/
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
