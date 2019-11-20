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

@interface ViewController : NSViewController {
    
    // ---------------- //
    //  Initiative Tab  //
    // ---------------- //
    @public NSComboBox __strong *MobSelector;
    @public NSComboBox __strong *AllySelector;
    
    @public LabeledTextBox * __strong Heros[8];
    @public int PARTY_SIZE;

    @public BattleEntity * __strong Allies[4];
    @public int ALLY_SIZE;
    
    @public BattleEntity * __strong Mobs[16];
    @public int MOB_SIZE;
    
    @public OrderField * __strong Order[28];
    @public int ORDER_SIZE;
    
    @public NSTextField __strong *CurrentInTurn;
    @public NSTextField __strong *RoundCount;
    
    @public ActionButton __strong *Reset;
    @public ActionButton __strong *Map;
    @public ActionButton __strong *Roll;
    @public ActionButton __strong *Set;
    @public ActionButton __strong *Next;
    
    @public Counter * __strong Counters[4];
    @public int COUNTER_SIZE;
};

@end

