//
//  AppDelegate.h
//  TheTruePCMan
//
//  Created by Lowy on 09/10/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ViewController.h"
#import "RaceTraits.h"
#import "Class.h"

@interface AppDelegate : NSObject <NSApplicationDelegate, NSTextFieldDelegate, NSComboBoxDelegate, NSTableViewDataSource, NSTableViewDelegate>

@property (strong) NSArray *nameArray;

@end
