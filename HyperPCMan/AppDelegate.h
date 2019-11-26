//
//  AppDelegate.h
//  HyperPCMan
//
//  Created by Lowy on 13/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "ViewController.h"
#import "LabeledTextBox.h"
#import "ActionButton.h"
#import "Counter.h"
#import "BattleEntity.h"
#import "CheckButton.h"

@interface AppDelegate : NSObject <NSApplicationDelegate, NSComboBoxDelegate, NSTextFieldDelegate> {
    ViewController *mainVC;
};


@end

