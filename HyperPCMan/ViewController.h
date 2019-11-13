//
//  ViewController.h
//  HyperPCMan
//
//  Created by Lowy on 13/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "LabeledTextBox.h"

@interface ViewController : NSViewController {
    
    // ---------------- //
    //  Initiative Tab  //
    // ---------------- //
    @public LabeledTextBox * __strong Heros[8];
    @public int PARTY_SIZE;

    @public LabeledTextBox * __strong Allies[16];
    @public int ALLY_SIZE;
    
    @public LabeledTextBox * __strong Mobs[16];
    @public int MOB_SIZE;
};

@end

