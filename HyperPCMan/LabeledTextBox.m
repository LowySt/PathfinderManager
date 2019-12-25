//
//  LabeledTextBox.m
//  HyperPCMan
//
//  Created by Lowy on 13/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "LabeledTextBox.h"

@implementation LabeledTextBox

- (LabeledTextBox *)initLabeled:(NSString *)label labelDir:(enum LabelDir)labelDir frame:(NSRect)frame isEditable:(BOOL)isEditable {

    Box   = [[NSTextField alloc] initWithFrame:frame];
    Label = [NSTextField labelWithString:label];
    [Label sizeToFit];
    
    CGSize fR = Box.frame.size;
    CGSize lR = Label.frame.size;
    CGPoint p = CGPointMake(frame.origin.x, frame.origin.y);
    CGPoint labelP;
    switch(labelDir)
    {
      case LABEL_UP: { labelP = CGPointMake(p.x, p.y + fR.height); } break;
      case LABEL_DOWN: { labelP = CGPointMake(p.x, p.y - fR.height); } break;
      case LABEL_LEFT: { labelP = CGPointMake(p.x - lR.width, p.y); } break;
      case LABEL_RIGHT: { labelP = CGPointMake(p.x + fR.width, p.y); } break;
      default: { assert(false); } break;
    }
    
    [Box setFrameOrigin:p]; [Label setFrameOrigin:labelP];
        
    if(!isEditable) { [Box setEditable:false]; }
    
    return self;
}

- (LabeledTextBox *)initEditableLabel:(NSString *)label labelDir:(enum LabelDir)labelDir frame:(NSRect)f isEditable:(BOOL)isEditable boxSize:(NSSize)size {
    
    Box   = [[NSTextField alloc]
             initWithFrame:NSMakeRect(400, 400, size.width, size.height)];
    Label = [[NSTextField alloc] init];
    [Label setFrameSize:NSMakeSize(f.size.width, f.size.height)];
    [Label setStringValue:label];
            
    CGSize fR = Box.frame.size;
    CGSize lR = Label.frame.size;
    CGPoint p = CGPointMake(f.origin.x, f.origin.y);
    CGPoint labelP;
    switch(labelDir)
    {
      case LABEL_UP: { labelP = CGPointMake(p.x, p.y + fR.height); } break;
      case LABEL_DOWN: { labelP = CGPointMake(p.x, p.y - fR.height); } break;
      case LABEL_LEFT: { labelP = CGPointMake(p.x - lR.width, p.y); } break;
      case LABEL_RIGHT: { labelP = CGPointMake(p.x + fR.width, p.y); } break;
      default: { assert(false); } break;
    }
    
    [Box setFrameOrigin:p]; [Label setFrameOrigin:labelP];
    
    if(!isEditable) { [Box setEditable:false]; }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
}

@end
