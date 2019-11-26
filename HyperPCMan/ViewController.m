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
        [Mobs[i]->Box->Box setStringValue:@"0"];
        [Mobs[i]->Init setStringValue:@""];
    }
}

- (void)resetAllies {
    for(int i = 0; i < ALLY_SIZE; i++) {
        [Allies[i] hide];
        [Allies[i] setName:AllyNames[i]];
        [Allies[i]->Box->Box setStringValue:@"0"];
        [Allies[i]->Init setStringValue:@""];
    }
}

- (void)resetOrder {
    for(int i = 0; i < ORDER_SIZE; i++) {
        if(i < PARTY_SIZE) { [Order[i] show]; }
        else { [Order[i] hide]; }
        [Order[i]->Name setStringValue:@""];
    }
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
