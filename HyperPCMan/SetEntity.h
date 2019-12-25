//
//  SetEntity.h
//  HyperPCMan
//
//  Created by Lowy on 24/12/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LabeledTextBox.h"
#import "ActionButton.h"

NS_ASSUME_NONNULL_BEGIN

@interface SetEntity : NSObject {
    
    @public LabeledTextBox __strong *Box;
    @public ActionButton __strong *Butt;
}

- (SetEntity *)initWithFrame:(NSRect)frame name:(NSString *)name blk:(void(^)(void))blk;
- (void)hide;
- (void)show;

@end

NS_ASSUME_NONNULL_END
