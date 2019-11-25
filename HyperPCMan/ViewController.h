//
//  ViewController.h
//  HyperPCMan
//
//  Created by Lowy on 13/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "LabeledTextBox.h"
#import "OrderField.h"
#import "ActionButton.h"
#import "Counter.h"
#import "BattleEntity.h"
#import "CheckButton.h"

@interface ViewController : NSViewController {
    
    // ---------------- //
    //  Initiative Tab  //
    // ---------------- //
    @public NSComboBox __strong *MobSelector;
    @public NSComboBox __strong *AllySelector;
    
    @public BattleEntity * __strong Heros[PARTY_SIZE];
    @public CheckButton * __strong InBattle[PARTY_SIZE];

    @public BattleEntity * __strong Allies[ALLY_SIZE];
    @public BattleEntity * __strong Mobs[MOB_SIZE];
    @public OrderField * __strong Order[ORDER_SIZE];
        
    @public NSTextField __strong *CurrentInTurn;
    @public NSTextField __strong *RoundCount;
    
    @public ActionButton __strong *Reset;
    @public ActionButton __strong *Map;
    @public ActionButton __strong *Roll;
    @public ActionButton __strong *Set;
    @public ActionButton __strong *Next;
    
    @public Counter * __strong Counters[4];
};

- (void)resetMobs;
- (void)resetAllies;
- (void)resetOrder;

@end

