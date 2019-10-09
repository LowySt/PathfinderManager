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
    
    int index = t.intValue;
    if(t.intValue > v.AbilityBonus.count || t.intValue < 0)
    { index = 0; }
    NSString *s = v.AbilityBonus[index];

    if(t.identifier == v.STRField.identifier)
    { [v.STRBonus setStringValue:s]; }
    else if(t.identifier == v.DEXField.identifier)
    { [v.DEXBonus setStringValue:s]; }
    else if(t.identifier == v.CONField.identifier)
    { [v.CONBonus setStringValue:s]; }
    else if(t.identifier == v.INTField.identifier)
    { [v.INTBonus setStringValue:s]; }
    else if(t.identifier == v.WISField.identifier)
    { [v.WISBonus setStringValue:s]; }
    else if(t.identifier == v.CHAField.identifier)
    { [v.CHABonus setStringValue:s]; }
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
   ViewController *v = (ViewController *) NSApplication.sharedApplication.orderedWindows.firstObject.contentViewController;
    
    [v.STRField setDelegate:self];
    [v.DEXField setDelegate:self];
    [v.CONField setDelegate:self];
    [v.INTField setDelegate:self];
    [v.WISField setDelegate:self];
    [v.CHAField setDelegate:self];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
