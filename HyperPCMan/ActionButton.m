//
//  ActionButton.m
//  HyperPCMan
//
//  Created by Lowy on 17/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "ActionButton.h"

@implementation ActionButton

- (void)DoAction {
    Action();
}

- (ActionButton *)initWithAction:(NSRect)frame name:(NSString *)name blk:(void(^)(void))blk {
    
    Action = blk;
    
    Button = [NSButton buttonWithTitle:name target:self action:NSSelectorFromString(@"DoAction")];
    [Button setFrame:frame];

    return self;
}

- (ActionButton *)initXWithAction:(NSRect)frame blk:(void(^)(void))blk {
    
    Action = blk;
    
    Button = [NSButton buttonWithTitle:@"x" target:self action:NSSelectorFromString(@"DoAction")];
    [Button setBordered:false];
    [Button setFrame:frame];
    
    return self;
}

- (ActionButton *)initSymNoAction:(NSRect)frame sym:(NSString *)sym {

    Button = [NSButton buttonWithTitle:sym target:self action:nil];
    [Button setBordered:false];
    [Button setFrame:frame];
        
    return self;

}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
}

@end
