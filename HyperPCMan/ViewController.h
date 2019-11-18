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

@interface ViewController : NSViewController {
    
    // ---------------- //
    //  Initiative Tab  //
    // ---------------- //
    @public LabeledTextBox * __strong Heros[8];
    @public int PARTY_SIZE;

    @public LabeledTextBox * __strong Allies[4];
    @public int ALLY_SIZE;
    
    @public LabeledTextBox * __strong Mobs[16];
    @public int MOB_SIZE;
    
    @public OrderField * __strong Order[28];
    @public int ORDER_SIZE;
    
    @public ActionButton __strong *Test;
    
    /*
    @public NSButton __strong *Reset;
    @public NSButton __strong *Map;
    @public NSButton __strong *Roll;
    @public NSButton __strong *Set;
    @public NSButton __strong *Next;*/
};

@end

