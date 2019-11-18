//
//  ActionButton.h
//  HyperPCMan
//
//  Created by Lowy on 17/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface ActionButton : NSButton {
    @public NSButton __strong *Button;
    
    void(^Action)(void);
};

- (void)DoAction;

- (ActionButton *)initWithAction:(NSRect)frame name:(NSString *)name blk:(void(^)(void))blk;

@end

NS_ASSUME_NONNULL_END
