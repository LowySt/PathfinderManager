//
//  ViewController.m
//  HyperPCMan
//
//  Created by Lowy on 13/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "ViewController.h"

@implementation ViewController

- (void)resetMobs {
    for(int i = 0; i < MOB_SIZE; i++) {
        [Mobs[i] hide];
        [Mobs[i] setName:EnemyNames[i]];
    }
}

- (void)resetAllies {
    for(int i = 0; i < ALLY_SIZE; i++) {
        [Allies[i] hide];
        [Allies[i] setName:AllyNames[i]];
    }
}

- (void)resetOrder {
    for(int i = 0; i < PARTY_SIZE; i++) { [Order[i] show]; }
    for(int i = PARTY_SIZE; i < ORDER_SIZE; i++) { [Order[i] hide]; }
}

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
