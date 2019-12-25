//
//  SetEntity.m
//  HyperPCMan
//
//  Created by Lowy on 24/12/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "SetEntity.h"

@implementation SetEntity

- (SetEntity *)initWithFrame:(NSRect)frame name:(NSString *)name blk:(void(^)(void))blk {
    
    NSNumberFormatter *nf = [[NSNumberFormatter alloc] init];
    [nf setAllowsFloats:false];
    [nf setNumberStyle:NSNumberFormatterNoStyle];
    [nf setPartialStringValidationEnabled:true];
    
    Box = [[LabeledTextBox alloc] initEditableLabel:name labelDir:LABEL_LEFT frame:frame isEditable:true boxSize:NSMakeSize(30, 20)];
    Box->Box.formatter = nf;
    [Box->Box setAlignment:NSTextAlignmentCenter];
    [Box->Box setStringValue:@""];
    
    [Box->Box setHidden:true];
    [Box->Label setHidden:true];

    NSRect actionRect = NSMakeRect(frame.origin.x + 26, frame.origin.y-5, 40, 30);
    Butt = [[ActionButton alloc] initWithAction:actionRect name:@"✓" blk:blk];
    [Butt->Button setBordered:false];

    [Butt->Button setHidden:true];
    
    return self;
}

- (void)hide {
    [Box->Box setHidden:true];
    [Box->Label setHidden:true];
    [Butt->Button setHidden:true];
}
- (void)show {
    [Box->Box setHidden:false];
    [Box->Label setHidden:false];
    [Butt->Button setHidden:false];
}

@end
