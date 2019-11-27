//
//  LabeledTextBox.h
//  HyperPCMan
//
//  Created by Lowy on 13/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

enum LabelDir {
    LABEL_UP = 0,
    LABEL_DOWN,
    LABEL_LEFT,
    LABEL_RIGHT,
    
    LABEL_COUNT
};


@interface LabeledTextBox : NSTextField {
    
    @public NSTextField __strong *Label;
    @public NSTextField __strong *Box;

};

- (LabeledTextBox *) initLabeled:(NSString *)label labelDir:(enum LabelDir)labelDir frame:(NSRect)frame isEditable:(BOOL)isEditable;

-(LabeledTextBox *) initEditableLabel:(NSString *)label labelDir:(enum LabelDir)labelDir frame:(NSRect)frame isEditable:(BOOL)isEditable boxSize:(NSSize)size;

@end

NS_ASSUME_NONNULL_END
