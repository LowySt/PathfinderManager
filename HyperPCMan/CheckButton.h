//
//  CheckButton.h
//  HyperPCMan
//
//  Created by Lowy on 25/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface CheckButton : NSButton {
    @public NSButton __strong *Button;
    
    void(^Action)(CheckButton *s);
};
- (void)DoAction;

- (CheckButton *)initWithState:(bool)state frame:(NSRect)frame;
- (CheckButton *)initWithAction:(NSRect)frame blk:(void(^)(CheckButton *))blk;
@end

NS_ASSUME_NONNULL_END
