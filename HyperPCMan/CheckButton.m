//
//  CheckButton.m
//  HyperPCMan
//
//  Created by Lowy on 25/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "CheckButton.h"

@implementation CheckButton

- (void)DoAction {
    Action(self);
}

- (CheckButton *)initWithState:(bool)state frame:(NSRect)frame {
    
    Button = [NSButton checkboxWithTitle:@"" target:self action:NULL];
    if(state == true) {
        [Button setState:NSControlStateValueOn];
    }
    else {
        [Button setState:NSControlStateValueOff];
    }
    [Button setFrame:frame];
    
    return self;
}

- (CheckButton *)initWithAction:(NSRect)frame blk:(void(^)(CheckButton *s))blk {
    
    Action = blk;
    
    Button = [NSButton checkboxWithTitle:@"" target:self action:NSSelectorFromString(@"DoAction")];
    [Button setState:NSControlStateValueOn];
    [Button setFrame:frame];
    
    return self;
}

@end
