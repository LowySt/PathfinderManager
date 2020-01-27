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
    orderNum = PARTY_SIZE;
    for(int i = 0; i < ORDER_SIZE; i++) {
        if(i < PARTY_SIZE) { [Order[i] show]; }
        else { [Order[i] hide]; }
        [Order[i]->Name setStringValue:@""];
    }
}

- (void)showNOrder:(NSInteger)n {
    for(int i = 0; i < ORDER_SIZE; i++) {
        if(i < n) { [Order[i] show]; }
        else { [Order[i] hide]; }
    }
}

- (void)changePos:(NSInteger)p newPos:(NSInteger)newP {
    NSInteger num = orderNum;
    
    if(p == newP) { return; }
    if(newP > num) { return; }

    NSInteger currIdx = p - 1;
    NSInteger newIdx = currIdx - 1;
    NSInteger range;
    NSString *moved = [Order[currIdx]->Name stringValue];
    
    if(newP > p) {
        NSInteger untouched = newP - p - 1;
        range = num - untouched;
        if(currentTurnIdx <= (p-1) || currentTurnIdx >= (newP - 1)) {
            currentTurnIdx += 1;
            if(currentTurnIdx > num) { currentTurnIdx = 1; }
        }
    } else {
        range = p - newP + 1;
        if(currentTurnIdx <= (p-1) && currentTurnIdx >= (newP - 1)) {
            currentTurnIdx += 1;
            if(currentTurnIdx > num) { currentTurnIdx = 1; }
        }
    }
    
    for(NSInteger i = range; i > 0; i--) {
        if(newIdx < 0) { newIdx = num - 1; }
        [Order[currIdx]->Name setStringValue:[Order[newIdx]->Name stringValue]];
        if(currIdx - 1 < 0) { currIdx = num - 1; }
        else { currIdx -= 1; }
        newIdx = currIdx - 1;
    }
    [Order[newP-1]->Name setStringValue:moved];
   
    [CurrentInTurn setStringValue:[Order[currentTurnIdx]->Name stringValue]];
}

bool isToken(char c) {
    switch(c)
    {
        case '(': return true;
        case ')': return true;
        case 'd': return true;
        case '+': return true;
        case '-': return true;
        case '*': return true;
    }
    
    return false;
}

NSString *getSubstringUntilChar(const char *s, char c) {
    int length = 0;
    char *at = (char *)s;
    while(*at != c) { length += 1; at += 1; }
    
    NSString *result = [[NSString alloc] initWithBytes:s length:length encoding:NSUTF8StringEncoding];
    return result;
}

- (NSInteger)calcThrow:(NSString *)s {
    
    NSInteger result = 0;
    const char *cStr = [s cStringUsingEncoding:NSUTF8StringEncoding];
    char *At = (char *)cStr;
    while(*At != 0)
    {
        char c = *At;
        if(isToken(c)) {
            if(c == '(') {
                NSString *subStr = getSubstringUntilChar(At+1, ')'); At += 1;
                NSInteger partialResult = [self calcThrow:subStr];
                result += partialResult;
                At += [subStr length];
                At += 1; // )
            }
        }
        else {
            
        }
        
        At += 1;
    }
        
    return 0;
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
