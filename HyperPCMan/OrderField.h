//
//  OrderField.h
//  HyperPCMan
//
//  Created by Lowy on 17/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface OrderField : NSTextField {

    @public NSTextField __strong *Name;
    @public NSTextField __strong *Num;
};

- (OrderField *)initOrder:(CGPoint)p num:(int)num;

@end

NS_ASSUME_NONNULL_END
