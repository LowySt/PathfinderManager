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
        NSInteger i = [v.MobNumberSel indexOfSelectedItem];
        
        [v hideFields];
        
        switch (i) {

            case 16:
                [v.Mob16Field setHidden:false];
                [v.Mob16Bonus setHidden:false];
                [v.Mob16Label setHidden:false];
                [v.Order22Num setHidden:false];
                [v.Order22Field setHidden:false];
            case 15:
                [v.Mob15Field setHidden:false];
                [v.Mob15Bonus setHidden:false];
                [v.Mob15Label setHidden:false];
                [v.Order21Num setHidden:false];
                [v.Order21Field setHidden:false];
            case 14:
                [v.Mob14Field setHidden:false];
                [v.Mob14Bonus setHidden:false];
                [v.Mob14Label setHidden:false];
                [v.Order20Num setHidden:false];
                [v.Order20Field setHidden:false];
            case 13:
                [v.Mob13Field setHidden:false];
                [v.Mob13Bonus setHidden:false];
                [v.Mob13Label setHidden:false];
                [v.Order19Num setHidden:false];
                [v.Order19Field setHidden:false];
            case 12:
                [v.Mob12Field setHidden:false];
                [v.Mob12Bonus setHidden:false];
                [v.Mob12Label setHidden:false];
                [v.Order18Num setHidden:false];
                [v.Order18Field setHidden:false];
            case 11:
                [v.Mob11Field setHidden:false];
                [v.Mob11Bonus setHidden:false];
                [v.Mob11Label setHidden:false];
                [v.Order17Num setHidden:false];
                [v.Order17Field setHidden:false];
            case 10:
                [v.Mob10Field setHidden:false];
                [v.Mob10Bonus setHidden:false];
                [v.Mob10Label setHidden:false];
                [v.Order16Num setHidden:false];
                [v.Order16Field setHidden:false];
            case 9:
                [v.Mob9Field setHidden:false];
                [v.Mob9Bonus setHidden:false];
                [v.Mob9Label setHidden:false];
                [v.Order15Num setHidden:false];
                [v.Order15Field setHidden:false];
            case 8:
                [v.Mob8Field setHidden:false];
                [v.Mob8Bonus setHidden:false];
                [v.Mob8Label setHidden:false];
                [v.Order14Num setHidden:false];
                [v.Order14Field setHidden:false];
            case 7:
                [v.Mob7Field setHidden:false];
                [v.Mob7Bonus setHidden:false];
                [v.Mob7Label setHidden:false];
                [v.Order13Num setHidden:false];
                [v.Order13Field setHidden:false];
            case 6:
                [v.Mob6Field setHidden:false];
                [v.Mob6Bonus setHidden:false];
                [v.Mob6Label setHidden:false];
                [v.Order12Num setHidden:false];
                [v.Order12Field setHidden:false];
            case 5:
                [v.Mob5Field setHidden:false];
                [v.Mob5Bonus setHidden:false];
                [v.Mob5Label setHidden:false];
                [v.Order11Num setHidden:false];
                [v.Order11Field setHidden:false];
            case 4:
                [v.Mob4Field setHidden:false];
                [v.Mob4Bonus setHidden:false];
                [v.Mob4Label setHidden:false];
                [v.Order10Num setHidden:false];
                [v.Order10Field setHidden:false];
            case 3:
                [v.Mob3Field setHidden:false];
                [v.Mob3Bonus setHidden:false];
                [v.Mob3Label setHidden:false];
                [v.Order9Num setHidden:false];
                [v.Order9Field setHidden:false];
            case 2:
                [v.Mob2Field setHidden:false];
                [v.Mob2Bonus setHidden:false];
                [v.Mob2Label setHidden:false];
                [v.Order8Num setHidden:false];
                [v.Order8Field setHidden:false];
            case 1:
                [v.Mob1Field setHidden:false];
                [v.Mob1Bonus setHidden:false];
                [v.Mob1Label setHidden:false];
                [v.Order7Num setHidden:false];
                [v.Order7Field setHidden:false];
                break;
            case 0:
                break;
        }
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
    
    [v.Order1Num setDelegate:self]; [v.Order2Num setDelegate:self];
    [v.Order3Num setDelegate:self]; [v.Order4Num setDelegate:self];
    [v.Order5Num setDelegate:self]; [v.Order6Num setDelegate:self];
    [v.Order7Num setDelegate:self]; [v.Order8Num setDelegate:self];
    [v.Order9Num setDelegate:self]; [v.Order10Num setDelegate:self];
    [v.Order11Num setDelegate:self]; [v.Order12Num setDelegate:self];
    [v.Order13Num setDelegate:self]; [v.Order14Num setDelegate:self];
    [v.Order15Num setDelegate:self]; [v.Order16Num setDelegate:self];
    [v.Order17Num setDelegate:self]; [v.Order18Num setDelegate:self];
    [v.Order19Num setDelegate:self]; [v.Order20Num setDelegate:self];
    [v.Order21Num setDelegate:self]; [v.Order22Num setDelegate:self];
    
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