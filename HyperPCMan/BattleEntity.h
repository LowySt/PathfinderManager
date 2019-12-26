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
#define MOB_SIZE     24
#define ORDER_SIZE   PARTY_SIZE + ALLY_SIZE + MOB_SIZE
#define COUNTER_SIZE 8

NSString static * _Nonnull __strong EnemyNames[MOB_SIZE] = {@"Enemy 1", @"Enemy 2", @"Enemy 3", @"Enemy 4", @"Enemy 5", @"Enemy 6", @"Enemy 7", @"Enemy 8", @"Enemy 9", @"Enemy 10", @"Enemy 11", @"Enemy 12", @"Enemy 13", @"Enemy 14", @"Enemy 15", @"Enemy 16", @"Enemy 17", @"Enemy 18", @"Enemy 19", @"Enemy 20", @"Enemy 21", @"Enemy 22", @"Enemy 23", @"Enemy 24"};

NSString static *_Nonnull __strong HeroNames[PARTY_SIZE] = {@"Gremag", @"Federico", @"Ken Shiro", @"Sirion", @"Israfel", @"Juliet", @"Dubhe", @"Dresdam", @"Zoddak"};

NSString static *_Nonnull __strong AllyNames[ALLY_SIZE] = {@"Ally 1", @"Ally 2", @"Ally 3", @"Ally 4"};

NSString static *_Nonnull __strong counterNames[COUNTER_SIZE] = {@"Counter 1", @"Counter 2", @"Counter 3", @"Counter 4", @"Counter 5", @"Counter 6", @"Counter 7", @"Counter 8"};

@interface BattleEntity : NSObject {
  
    @public LabeledTextBox __strong *Box;
    @public NSTextField __strong *Init;
    
    @public bool isHero;
};

- (BattleEntity *)initWithFrame:(NSRect)frame name:(NSString *)name;

- (BattleEntity *)initHero:(NSRect)frame name:(NSString *)name;

- (void)clear;
- (void)hide;
- (void)show;

- (void)setName:(NSString *)name;

@end

NS_ASSUME_NONNULL_END
