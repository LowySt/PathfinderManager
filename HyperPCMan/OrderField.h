//
//  OrderField.h
//  HyperPCMan
//
//  Created by Lowy on 17/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ActionButton.h"

NS_ASSUME_NONNULL_BEGIN

@interface OrderField : NSTextField {

    @public NSTextField __strong *Name;
    @public NSTextField __strong *Num;
    
    @public ActionButton __strong *Remove;
};

- (OrderField *)initOrder:(CGPoint)p num:(int)num;

- (void)hide;
- (void)show;

@end

NS_ASSUME_NONNULL_END
