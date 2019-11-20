//
//  BattleEntity.h
//  HyperPCMan
//
//  Created by Lowy on 20/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LabeledTextBox.h"

NS_ASSUME_NONNULL_BEGIN

@interface BattleEntity : NSObject {
  
    @public LabeledTextBox __strong *Box;
    @public NSTextField __strong *Init;
    
};

- (BattleEntity *)initWithFrame:(NSRect)frame name:(NSString *)name;

@end

NS_ASSUME_NONNULL_END
