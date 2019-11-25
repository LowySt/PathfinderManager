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
};

- (CheckButton *)initWithState:(bool)state frame:(NSRect)frame;

@end

NS_ASSUME_NONNULL_END
