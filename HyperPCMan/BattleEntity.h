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

#define PARTY_SIZE   9
#define ALLY_SIZE    4
#define MOB_SIZE     16
#define ORDER_SIZE   29
#define COUNTER_SIZE 4

@interface BattleEntity : NSObject {
  
    @public LabeledTextBox __strong *Box;
    @public NSTextField __strong *Init;
    
};

- (BattleEntity *)initWithFrame:(NSRect)frame name:(NSString *)name;

@end

NS_ASSUME_NONNULL_END
