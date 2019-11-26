//
//  Counter.h
//  HyperPCMan
//
//  Created by Lowy on 18/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "ActionButton.h"
#import "LabeledTextBox.h"

NS_ASSUME_NONNULL_BEGIN

@interface Counter : NSObject {
    
    @public LabeledTextBox __strong *Field;
    @public NSTextField __strong *Count;
    @public ActionButton __strong *SetButton;
    
    bool isCounting;
    int  elapsedTurns;
};

- (Counter *)initWithFrame:(NSRect)frame name:(NSString *)name;

- (void)tick;
- (void)reset;

@end

NS_ASSUME_NONNULL_END
