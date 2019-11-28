//
//  StatField.m
//  HyperPCMan
//
//  Created by Lowy on 27/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "StatField.h"

@implementation StatField

- (StatField *)initWithLabel:(NSString *)label frame:(NSRect)frame {
    
    Box = [[LabeledTextBox alloc] initLabeled:label labelDir:LABEL_LEFT frame:frame isEditable:false];
    Bonus = [[NSTextField alloc] initWithFrame:NSMakeRect(frame.origin.x + 40, frame.origin.y, 30, 20)];
    [Bonus setEditable:false];
    [Bonus setAlignment:NSTextAlignmentCenter];
    
    return self;
}

- (void)setIntValue:(int)v {
    [Box->Box setIntValue:v];
}

@end
