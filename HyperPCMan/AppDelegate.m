//
//  AppDelegate.m
//  HyperPCMan
//
//  Created by Lowy on 13/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (BOOL)control:(NSControl *)control textView:(NSTextView *)textView doCommandBySelector:(SEL)commandSelector {
    ViewController *v = mainVC;
    
    if (commandSelector == @selector(insertNewline:)) {
    // Do something against ENTER key
        NSInteger allyNum = mainVC->allyNum;
        NSInteger mobNum = mainVC->mobNum;
    
        for(int i = 0; i < allyNum; i++) {
            void *LabelField = (__bridge void *)(v->Allies[i]->Box->Label);
            void *BonusField = (__bridge void *)(v->Allies[i]->Box->Box);
            void *InitField = (__bridge void *)(v->Allies[i]->Init);
            
            if((__bridge void *)control == LabelField) {
                if((i+1) == allyNum) { return YES; }
                [v->Allies[i+1]->Box->Label setStringValue:@""];
                [control.window makeFirstResponder:v->Allies[i+1]->Box->Label];
                return YES;
            }
            
            if((__bridge void *)control == BonusField) {
                if((i+1) == allyNum) { return YES; }
                [v->Allies[i+1]->Box->Box setStringValue:@""];
                [control.window makeFirstResponder:v->Allies[i+1]->Box->Box];
                return YES;
            }

            if((__bridge void *)control == InitField) {
                if((i+1) == allyNum) { return YES; }
                [v->Allies[i+1]->Init setStringValue:@""];
                [control.window makeFirstResponder:v->Allies[i+1]->Init];
                return YES;
            }
        }
        
        for(int i = 0; i < mobNum; i++) {
            void *LabelField = (__bridge void *)(v->Mobs[i]->Box->Label);
            void *BonusField = (__bridge void *)(v->Mobs[i]->Box->Box);
            void *InitField = (__bridge void *)(v->Mobs[i]->Init);
            
            if((__bridge void *)control == LabelField) {
                if((i+1) == mobNum) { return YES; }
                [v->Mobs[i+1]->Box->Label setStringValue:@""];
                [control.window makeFirstResponder:v->Mobs[i+1]->Box->Label];
                return YES;
            }
            
            if((__bridge void *)control == BonusField) {
                if((i+1) == mobNum) { return YES; }
                [v->Mobs[i+1]->Box->Box setStringValue:@""];
                [control.window makeFirstResponder:v->Mobs[i+1]->Box->Box];
                return YES;
            }

            if((__bridge void *)control == InitField) {
                if((i+1) == mobNum) { return YES; }
                [v->Mobs[i+1]->Init setStringValue:@""];
                [control.window makeFirstResponder:v->Mobs[i+1]->Init];
                return YES;
            }
        }
        
        for(int i = 0; i < PARTY_SIZE; i++) {
            void *InitField = (__bridge void *)(v->Heros[i]->Box->Box);
            if((__bridge void *)control == InitField) {
                if((i + 1) == PARTY_SIZE) { return YES; }
                [control.window makeFirstResponder:v->Heros[i+1]->Box->Box];
                return YES;
            }
        }
        
        [control.window makeFirstResponder:control.window];
        return YES;
    }
    
    // return true if the action was handled; otherwise false
    return NO;
}
  
- (void)controlTextDidEndEditing:(NSNotification *)obj {
    ViewController *v = mainVC;
    NSTextField *t = obj.object;
    void *tp = (__bridge void *)t;

    for(NSInteger i = 0; i < v->orderNum; i++) {
        if(tp == (__bridge void *)v->Order[i]->Num)
        {
            NSInteger newPos = [v->Order[i]->Num integerValue];
            [v changePos:(i+1) newPos:newPos];
            [v->Order[i]->Num setIntegerValue:(i+1)];
        }
    }
}

- (void)comboBoxSelectionDidChange:(NSNotification *)notification {
    
    NSComboBox *t = notification.object;
    
    if(t.identifier == mainVC->MobSelector.identifier)
    {
        mainVC->currentTurnIdx = 0;
        
        NSInteger num = [mainVC->MobSelector indexOfSelectedItem];
        mainVC->mobNum = num; [mainVC resetMobs];
        for(int i = 0; i < num; i++) { [mainVC->Mobs[i] show]; }
               
        NSInteger allyNum = [mainVC->AllySelector indexOfSelectedItem];
        mainVC->orderNum = num + allyNum + PARTY_SIZE - mainVC->notInBattle;
        [mainVC showNOrder:mainVC->orderNum];
    }
    else if(t.identifier == mainVC->AllySelector.identifier)
    {
        mainVC->currentTurnIdx = 0;

        NSInteger num = [mainVC->AllySelector indexOfSelectedItem];
        mainVC->allyNum = num;
        [mainVC resetAllies];
        for(int i = 0; i < num; i++) { [mainVC->Allies[i] show]; }
        
        NSInteger mobNum = [mainVC->MobSelector indexOfSelectedItem];
        mainVC->orderNum = num + mobNum + PARTY_SIZE - mainVC->notInBattle;
        [mainVC showNOrder:mainVC->orderNum];
    }
    else if(t == mainVC->PCSelector) {
        NSInteger index = [mainVC->PCSelector indexOfSelectedItem];
        for(NSInteger i = 0; i < STATS_NUM; i++) {
            int v = mainVC->Party[index]->Stats[i];
            [mainVC->Stats[i] setIntValue:v];
            [mainVC->Stats[i]->Bonus setIntValue:ASBonus[v]];
        }
        for(NSInteger i = 0; i < ST_NUM; i++) {
            [mainVC->SavingThrows[i]->Box setIntValue:mainVC->Party[index]->SavingThrows[i]];
        }
        for(NSInteger i = 0; i < SKILL_NUM; i++) {
            [mainVC->Skills[i]->Box setIntValue:mainVC->Party[index]->Skills[i].Value];
            if(mainVC->Party[index]->Skills[i].markSet == SKILL_MARK_UNSET) {
                [mainVC->hasLevels[i]->Button setTitle:@"O"];
            }
            else if(mainVC->Party[index]->Skills[i].markSet == 0) {
                [mainVC->hasLevels[i]->Button setTitle:@"X"];
            }
            else if(mainVC->Party[index]->Skills[i].markSet == 1) {
                [mainVC->hasLevels[i]->Button setTitle:@"✓"];
            }
        }
        //TODO: Account for multiple bonuses
        [mainVC->miscAttack[0]->Box setIntValue:mainVC->Party[index]->AC];
        [mainVC->miscAttack[1]->Box setIntValue:mainVC->Party[index]->AttackBonus.Bonus[0]];
        [mainVC->miscAttack[2]->Box setIntValue:mainVC->Party[index]->CMB];
        [mainVC->miscAttack[3]->Box setIntValue:mainVC->Party[index]->CMD];
    }
}

- (void)SetupInitTab:(NSTabViewItem *)item {

    mainVC->battleOngoing = false;
    mainVC->orderNum = PARTY_SIZE;
    
    int yPos = 720;
    for(int i = 0; i < PARTY_SIZE; i++) {
    
        //TODO: Remove the formatter from hero. Maybe make it a BattleEntity
        //      as well?
        BattleEntity *Hero = [[BattleEntity alloc]initHero:NSMakeRect(525, yPos, 30, 20) name:HeroNames[i]];
                
        CheckButton *InBattle = [[CheckButton alloc] initWithAction:NSMakeRect(560, yPos, 20, 20) blk:^void(CheckButton *s){
            ViewController *v = self->mainVC;
            if([s->Button state] == NSControlStateValueOff) { v->notInBattle += 1; v->orderNum -= 1; }
            else if([s->Button state] == NSControlStateValueOn) { v->notInBattle -= 1; v->orderNum += 1; }
            
            [v showNOrder:v->orderNum];
        }];
                
        [Hero->Box->Box setDelegate:self];
        [[item view] addSubview:Hero->Box->Box];
        [[item view] addSubview:Hero->Box->Label];
        [[item view] addSubview:InBattle->Button];
        mainVC->Heros[i] = Hero;
        mainVC->InBattle[i] = InBattle;
        yPos -= 30;
    }
    
    for(int i = 0; i < ALLY_SIZE; i++) {
    
        BattleEntity *Ally = [[BattleEntity alloc] initWithFrame:NSMakeRect(525, yPos, 70, 20) name:AllyNames[i]];
        
        SetEntity *set = [[SetEntity alloc] initWithFrame:NSMakeRect(525, yPos, 70, 20) name:@"" blk:^void(){
            ViewController *v = self->mainVC;
            NSInteger newAllyNum = v->allyNum+1;
            assert(newAllyNum <= ALLY_SIZE);
            [v->NewAllies[v->allyNum] hide];
            
            [v showNOrder:(v->orderNum+1)];
            NSString *newName = [v->NewAllies[v->allyNum]->Box->Label stringValue];
            [v->Order[v->orderNum]->Name setStringValue:newName];
            [v->Order[v->orderNum]->Num setIntegerValue:(v->orderNum+1)];
            
            v->turnsInRound += 1;
            [v changePos:(v->orderNum+1) newPos:(v->currentTurnIdx+1+1)];
                       
            v->allyNum = newAllyNum; [v->Allies[newAllyNum-1] show]; v->orderNum += 1;
            
            [v->Allies[v->allyNum-1]->Box->Label setStringValue:newName];
            if(newAllyNum < ALLY_SIZE) { [v->AddEntity[v->allyNum]->Button setHidden:false]; }
        }];
        
        ActionButton *ab = [[ActionButton alloc] initWithAction:NSMakeRect(525, yPos, 40, 20) name:@"+" blk:^void(){
            ViewController *vc = self->mainVC;
            if(vc->battleOngoing == false) { return; }
            
            NSInteger idx = vc->allyNum;
            [vc->NewAllies[idx] show];
            [vc->AddEntity[idx]->Button setHidden:true];
            //TODO: Maybe add boolean to allow other components to know a new entity is being added.
        }];
        [ab->Button setHidden:true];
        
        [Ally->Box->Label setDelegate:self];
        [Ally->Box->Box setDelegate:self];
        [Ally->Init setDelegate:self];
        [set->Box->Label setDelegate:self];
        [set->Box->Box setDelegate:self];
        [[item view] addSubview:Ally->Box->Box];
    	[[item view] addSubview:Ally->Box->Label];
        [[item view] addSubview:Ally->Init];
        [[item view] addSubview:ab->Button];
        [[item view] addSubview:set->Box->Box];
        [[item view] addSubview:set->Box->Label];
        [[item view] addSubview:set->Butt->Button];
        mainVC->NewAllies[i] = set;
        mainVC->AddEntity[i] = ab;
        mainVC->Allies[i] = Ally;
        yPos -= 30;
    }
 
    yPos = 720;
    for(int i = 0; i < MOB_SIZE; i++) {

        BattleEntity *Mob = [[BattleEntity alloc] initWithFrame:NSMakeRect(365, yPos, 70, 20) name:EnemyNames[i]];
        
        SetEntity *set = [[SetEntity alloc] initWithFrame:NSMakeRect(365, yPos, 70, 20) name:@"" blk:^void(){
            ViewController *v = self->mainVC;
            NSInteger newMobNum = v->mobNum+1;
            assert(newMobNum <= MOB_SIZE);
            [v->NewMobs[v->mobNum] hide];
            
            [v showNOrder:(v->orderNum+1)];
            NSString *newName = [v->NewMobs[v->mobNum]->Box->Label stringValue];
            [v->Order[v->orderNum]->Name setStringValue:newName];
            [v->Order[v->orderNum]->Num setIntegerValue:(v->orderNum+1)];
            
            v->turnsInRound += 1;
            [v changePos:(v->orderNum+1) newPos:(v->currentTurnIdx+1+1)];
                       
            v->mobNum = newMobNum; [v->Mobs[newMobNum-1] show]; v->orderNum += 1;
            
            [v->Mobs[v->mobNum-1]->Box->Label setStringValue:newName];
            if(newMobNum < MOB_SIZE) { [v->AddEntity[v->mobNum+ALLY_SIZE]->Button setHidden:false]; }
        }];
        
        ActionButton *ab = [[ActionButton alloc] initWithAction:NSMakeRect(365, yPos, 40, 20) name:@"+" blk:^void(){
            ViewController *vc = self->mainVC;
            if(vc->battleOngoing == false) { return; }
            
            NSInteger idx = vc->mobNum;
            [vc->NewMobs[idx] show];
            [vc->AddEntity[idx+ALLY_SIZE]->Button setHidden:true];
            //TODO: Maybe add boolean to allow other components to know a new entity is being added.
        }];
        [ab->Button setHidden:true];
        
        [Mob->Box->Label setDelegate:self];
        [Mob->Box->Box setDelegate:self];
        [Mob->Init setDelegate:self];
        [set->Box->Label setDelegate:self];
        [set->Box->Box setDelegate:self];
        [[item view] addSubview:Mob->Box->Box];
        [[item view] addSubview:Mob->Box->Label];
        [[item view] addSubview:Mob->Init];
        [[item view] addSubview:ab->Button];
        [[item view] addSubview:set->Box->Box];
        [[item view] addSubview:set->Box->Label];
        [[item view] addSubview:set->Butt->Button];
        mainVC->NewMobs[i] = set;
        mainVC->AddEntity[i+ALLY_SIZE] = ab;
        mainVC->Mobs[i] = Mob;
        yPos -= 22;
    }
    
    yPos = 720;
    bool left = true;
    for(int i = 0; i < ORDER_SIZE; i++) {
        CGPoint p;
        if(left == true) { p = CGPointMake(680, yPos); left = false; }
        else { p = CGPointMake(870, yPos); left = true; yPos -= 22; }
        
        OrderField *Order = [[OrderField alloc] initOrder:p num:i+1 vc:mainVC];
        
        [Order->Num setDelegate:self];
        [[item view] addSubview:Order->Name];
        [[item view] addSubview:Order->Num];
        [[item view] addSubview:Order->Remove->Button];
        mainVC->Order[i] = Order;
    }
    
    mainVC->CurrentInTurn = [[NSTextField alloc] initWithFrame:NSMakeRect(760, 750, 120, 20)];
    [mainVC->CurrentInTurn setAlignment:NSTextAlignmentCenter];
    [mainVC->CurrentInTurn setEditable:false];
    [mainVC->CurrentInTurn setHidden:true];
    [[item view] addSubview:mainVC->CurrentInTurn];
    
    mainVC->RoundCount = [[NSTextField alloc] initWithFrame:NSMakeRect(1100, 790, 30, 20)];
    [mainVC->RoundCount setAlignment:NSTextAlignmentCenter];
    [mainVC->RoundCount setEditable:false];
    [[item view] addSubview:mainVC->RoundCount];
    
    yPos = 780;
    ActionButton *Reset = [[ActionButton alloc] initWithAction:NSMakeRect(20, yPos, 80, 24) name:@"Reset" blk:^void(){
        ViewController *vc = self->mainVC;
        [vc->AddEntity[vc->allyNum]->Button setHidden:true];
        [vc->AddEntity[vc->mobNum+ALLY_SIZE]->Button setHidden:true];
        [vc resetMobs]; [vc resetAllies]; [vc resetOrder];
        [vc->MobSelector selectItemAtIndex:0];
        [vc->AllySelector selectItemAtIndex:0];
        [vc->RoundCount setStringValue:@""];
        [vc->CurrentInTurn setStringValue:@""];
        [vc->MobSelector setHidden:false];
        [vc->AllySelector setHidden:false];
        [vc->Roll->Button setHidden:false];
        [vc->Next->Button setHidden:true];
        [vc->CurrentInTurn setHidden:true];
        [vc->Set->Button setHidden:false];
        vc->currentTurnIdx = 0;
        vc->battleOngoing = false;
        for(int i = 0; i < COUNTER_SIZE; i++) { [vc->Counters[i] reset]; }
        for(int i = 0; i < PARTY_SIZE; i++) {
            [vc->Heros[i]->Box->Box setStringValue:@""];
            [vc->InBattle[i]->Button setState:NSControlStateValueOn];
        }
        for(int i = 0; i < MOB_SIZE; i++) {
            [vc->NewMobs[i]->Box->Box setHidden:true];
            [vc->NewMobs[i]->Box->Label setHidden:true];
            [vc->NewMobs[i]->Butt->Button setHidden:true];
        }
        for(int i = 0; i < ALLY_SIZE; i++) {
            [vc->NewAllies[i]->Box->Box setHidden:true];
            [vc->NewAllies[i]->Box->Label setHidden:true];
            [vc->NewAllies[i]->Butt->Button setHidden:true];
        }
        [vc resetOrder];
    }];
        
    ActionButton *Map = [[ActionButton alloc] initWithAction:NSMakeRect(100, yPos, 80, 24) name:@"Map" blk:^void(){
        NSLog(@"Map Button!");
    }];
    
    ActionButton *Next = [[ActionButton alloc] initWithAction:NSMakeRect(780, 770, 80, 24) name:@"Next" blk:^void(){
        ViewController *v = self->mainVC;
        NSInteger num = v->orderNum;
        
        if(v->mobNum == 0 && v->allyNum == 0) { return; }
        
        //TODO: Maybe bug when removing from order and currentTurnIdx gets >= than num?
        //      Probably not because currentTurnIdx gets managed when removing from order
        if(v->currentTurnIdx == (num-1)) {
            v->currentTurnIdx = 0;
            [v->RoundCount setIntValue:([v->RoundCount intValue]+1)];
        } else {
            v->currentTurnIdx += 1;
        }

        //NOTE: We need numberOfTurnsI >= turnsInRound, rather then ==, because if
        //      turnsInRound was modified while the counter was on the boundary, it
        //      ends up in a wrong state!
        for(int i = 0; i < COUNTER_SIZE; i++) {
            Counter *curr = v->Counters[i];
            if(curr->isCounting == true) {
                curr->elapsedTurns += 1;
                if(curr->elapsedTurns >= v->turnsInRound) { [curr tick]; }
            }
        }
        
        [v->CurrentInTurn setStringValue:[v->Order[v->currentTurnIdx]->Name stringValue]];
    }];
    [Next->Button setHidden:true];
    
    ActionButton *Roll = [[ActionButton alloc] initWithAction:NSMakeRect(420, 750, 80, 24) name:@"Roll" blk:^void(){
        
        ViewController *v = self->mainVC;
        int newRand;
        for(int i = 0; i < v->mobNum; i++) {
            newRand = arc4random_uniform(20) + 1;
            [v->Mobs[i]->Init setIntValue:(newRand + [v->Mobs[i]->Box->Box intValue])];
        }
        
        for(int i = 0; i < self->mainVC->allyNum; i++) {
            newRand = arc4random_uniform(20) + 1;
            [v->Allies[i]->Init setIntValue:(newRand + [v->Allies[i]->Box->Box intValue])];
        }
    }];
    
    ActionButton *Set = [[ActionButton alloc] initWithAction:NSMakeRect(770, 740, 80, 24) name:@"Set" blk:^void(){
        ViewController *v = self->mainVC;
        
        v->currentTurnIdx = 0;
        v->turnsInRound = v->orderNum;
        v->battleOngoing = true;
        [v->MobSelector setHidden:true];
        [v->AllySelector setHidden:true];
        [v->Roll->Button setHidden:true];
        [v->Next->Button setHidden:false];
        [v->CurrentInTurn setHidden:false];
        [v->RoundCount setStringValue:@""];
       
        NSMutableArray *orderArr = [[NSMutableArray alloc] init];
        for(int i = 0; i < PARTY_SIZE; i++) {
            if([v->InBattle[i]->Button state] == NSControlStateValueOn) {
                [orderArr addObject:v->Heros[i]];
            }
        }
        for(int i = 0; i < v->mobNum; i++) { [orderArr addObject:v->Mobs[i]]; }
        for(int i = 0; i < v->allyNum; i++) { [orderArr addObject:v->Allies[i]]; }
        
        [orderArr sortUsingComparator:^(BattleEntity *a, BattleEntity *b) {
            if(a->isHero == true) {
                if(b->isHero == true) {
                    if([a->Box->Box intValue] > [b->Box->Box intValue])
                    { return (NSComparisonResult)NSOrderedAscending; }
                    if([a->Box->Box intValue] < [b->Box->Box intValue])
                    { return (NSComparisonResult)NSOrderedDescending; }
                    return (NSComparisonResult)NSOrderedSame;
                }
                if([a->Box->Box intValue] > [b->Init intValue])
                { return (NSComparisonResult)NSOrderedAscending; }
                if([a->Box->Box intValue] < [b->Init intValue])
                { return (NSComparisonResult)NSOrderedDescending; }
                return (NSComparisonResult)NSOrderedSame;
            }
            if(b->isHero == true) {
                if([b->Box->Box intValue] > [a->Init intValue])
                { return (NSComparisonResult)NSOrderedAscending; }
                if([b->Box->Box intValue] < [a->Init intValue])
                { return (NSComparisonResult)NSOrderedDescending; }
                return (NSComparisonResult)NSOrderedSame;
            }
            if([a->Init intValue] > [b->Init intValue])
            { return (NSComparisonResult)NSOrderedAscending; }
            if([a->Init intValue] < [b->Init intValue])
            { return (NSComparisonResult)NSOrderedDescending; }
            return (NSComparisonResult)NSOrderedSame;
        }];
        
        for(int i = 0; i < v->orderNum; i++) {
            BattleEntity *b = orderArr[i];
            [v->Order[i]->Name setStringValue:[b->Box->Label stringValue]];
            [v->Order[i]->Num setIntValue:(i+1)];
            
            if(i == 0) { [v->CurrentInTurn setStringValue:[b->Box->Label stringValue]]; }
        }
        
        //Show Enemy and Ally Adding Buttons:
        if(v->allyNum < ALLY_SIZE)
        { [v->AddEntity[v->allyNum]->Button setHidden:false]; }
        if(v->mobNum < MOB_SIZE)
        { [v->AddEntity[v->mobNum+ALLY_SIZE]->Button setHidden:false]; }
        
        [v->Set->Button setHidden:true];
    }];
       
    
    [[item view] addSubview:Reset->Button]; [[item view] addSubview:Map->Button];
    [[item view] addSubview:Roll->Button]; [[item view] addSubview:Set->Button];
    [[item view] addSubview:Next->Button];
    mainVC->Reset = Reset; mainVC->Map = Map;
    mainVC->Roll = Roll; mainVC->Set = Set;
    mainVC->Next = Next;
    
    yPos = 730;
    for(int i = 0; i < COUNTER_SIZE; i++) {
        
        Counter *C = [[Counter alloc] initWithFrame:NSMakeRect(20, yPos, 80, 20) name:counterNames[i]];
        [[item view] addSubview:C->Field->Box];
        [[item view] addSubview:C->Field->Label];
        [[item view] addSubview:C->Count];
        [[item view] addSubview:C->SetButton->Button];
        
        mainVC->Counters[i] = C;
        
        yPos -= 40;
    }

    NSArray *MobSelValues = @[@"No Enemies", @"1 Enemy", @"2 Enemies", @"3 Enemies", @"4 Enemies", @"5 Enemies", @"6 Enemies", @"7 Enemies", @"8 Enemies", @"9 Enemies", @"10 Enemies", @"11 Enemies", @"12 Enemies", @"13 Enemies", @"14 Enemies", @"15 Enemies", @"16 Enemies", @"17 Enemies", @"18 Enemies", @"19 Enemies", @"20 Enemies", @"21 Enemies", @"22 Enemies", @"23 Enemies", @"24 Enemies"];
    assert(MobSelValues.count == MOB_SIZE+1);
    NSComboBox *MobSel = [[NSComboBox alloc] initWithFrame:NSMakeRect(315, 750, 100, 25)];
    [MobSel addItemsWithObjectValues:MobSelValues];
    [MobSel selectItemAtIndex:0];
    MobSel.identifier = @"MobSel";
    [[item view] addSubview:MobSel];
    mainVC->MobSelector = MobSel;
    [mainVC->MobSelector setDelegate:self];
    
    NSArray *AllySelValues = @[@"No Allies", @"1 Ally", @"2 Allies", @"3 Allies", @"4 Allies"];
    assert(AllySelValues.count == ALLY_SIZE+1);
    NSComboBox *AllySel = [[NSComboBox alloc] initWithFrame:NSMakeRect(500, 750, 100, 25)];
    [AllySel addItemsWithObjectValues:AllySelValues];
    [AllySel selectItemAtIndex:0];
    AllySel.identifier = @"AllySel";
    [[item view] addSubview:AllySel];
    mainVC->AllySelector = AllySel;
    [mainVC->AllySelector setDelegate:self];
}

- (void)SetupPartyTab:(NSTabViewItem *)item {
    //TODO: Add Error Specifying no Party.pc file was found in the application folder!
    NSString *appParentDirectory = [[[NSBundle mainBundle] bundlePath] stringByDeletingLastPathComponent];
    NSString *sourcePath = [appParentDirectory stringByAppendingString:@"/Party.pc"];
    NSMutableArray *pcNames = [[NSMutableArray alloc] init];
    int language = 99;
    /*
    NSString *initMessage = @"Directory: ";
    NSString *message = [initMessage stringByAppendingString:sourcePath];
    
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:message];
    [alert setInformativeText:@"Informative text."];
    [alert addButtonWithTitle:@"Cancel"];
    [alert addButtonWithTitle:@"Ok"];
    [alert runModal];
    */
    NSInteger __block partyIdx = 0;
    NSString * __strong *Stat = nil;
    NSString * __strong *STNames = nil;
    NSString * __strong *SkillNames = nil;
    NSString * __strong *miscNames = nil;
    bool *levelRequiredSkill = nil;
    
    NSData *d = [NSData dataWithContentsOfFile:sourcePath];
    if(d == nil) { assert(false); }
    else {
              
        NSInteger dataIdx = 0;
        char *data = (char *)[d bytes];
                
        //-- Entity Setup --//
        NSInteger sizeOfParty = data[dataIdx++];
        
        for(NSInteger i = 0; i < sizeOfParty; i++) {
            Entity *e = [[Entity alloc] init];
            
            if((language == 0 && data[dataIdx] == 1) ||
               (language == 1 && data[dataIdx] == 0)) {
                //TODO: Report Error!!!
                assert(false);
            }
            language = data[dataIdx++];
            
            switch(language) {
                case 0: {
                    levelRequiredSkill = levelRequiredSkillENG;
                } break;
                case 1: {
                    levelRequiredSkill = levelRequiredSkillIT;
                } break;
            }

            uint8_t nameLen = data[dataIdx++];
            e->name = [[NSString alloc] initWithBytes:(data + dataIdx) length:nameLen encoding:NSUTF8StringEncoding];
            dataIdx += nameLen;
            
            NSString *pcName = [[NSString alloc] initWithString:e->name];
            [pcNames addObject:pcName];
            
            for(NSInteger j = 0; j < STATS_NUM; j++) { e->Stats[j] = data[dataIdx++]; }
            for(NSInteger j = 0; j < ST_NUM; j++) { e->SavingThrows[j] = data[dataIdx++]; }
            
            e->AC = data[dataIdx++];
            
            NSInteger howManyBABs = data[dataIdx++]; e->AttackBonus.num = (int)howManyBABs;
            for(NSInteger j = 0; j < howManyBABs; j++) {
                e->AttackBonus.Bonus[j] = data[dataIdx++];
            }
            e->CMB = data[dataIdx++]; e->CMD = data[dataIdx++];
                        
            for(NSInteger j = 0; j < SKILL_NUM; j++) {
                if(levelRequiredSkill[j] == true) {
                    e->Skills[j].markSet = data[dataIdx++];
                } else {
                    e->Skills[j].markSet = SKILL_MARK_UNSET;
                }
                e->Skills[j].Value = data[dataIdx++];
            }
            mainVC->Party[partyIdx] = e; partyIdx += 1;
        }
               
    }
 
    NSComboBox *pcs = [[NSComboBox alloc] initWithFrame:NSMakeRect(40, 700, 100, 26)];
    [pcs setDelegate:self];
    [pcs removeAllItems];
    [pcs addItemsWithObjectValues:pcNames];
    [pcs selectItemAtIndex:0];
    [[item view] addSubview:pcs];
    mainVC->PCSelector = pcs;
    
    switch(language) {
        case 0: {
            Stat = StatENG;
            STNames = STNamesENG;
            miscNames = miscNamesENG;
            SkillNames = SkillNamesENG;
            levelRequiredSkill = levelRequiredSkillENG;
        } break;
        case 1: {
            Stat = StatIT;
            STNames = STNamesIT;
            miscNames = miscNamesIT;
            SkillNames = SkillNamesIT;
            levelRequiredSkill = levelRequiredSkillIT;
        } break;
    }
    
    int yPos = 670;
    for(NSInteger i = 0; i < STATS_NUM; i++) {
        StatField *f = [[StatField alloc] initWithLabel:Stat[i] frame:NSMakeRect(60, yPos, 40, 20)];
        yPos -= 30;
                
        [[item view] addSubview:f->Box->Box];
        [[item view] addSubview:f->Box->Label];
        [[item view] addSubview:f->Bonus];
        mainVC->Stats[i] = f;
    }
    
    for(NSInteger i = 0; i < ST_NUM; i++) {
        LabeledTextBox *st = [[LabeledTextBox alloc] initLabeled:STNames[i] labelDir:LABEL_LEFT frame:NSMakeRect(60, yPos, 40, 20) isEditable:false];
        yPos -= 30;
        [st->Box setAlignment:NSTextAlignmentCenter];
        
        [[item view] addSubview:st->Box];
        [[item view] addSubview:st->Label];
        mainVC->SavingThrows[i] = st;
    }
    
    for(NSInteger i = 0; i < MISC_NUM; i++) {
        LabeledTextBox *m = [[LabeledTextBox alloc] initLabeled:miscNames[i] labelDir:LABEL_LEFT frame:NSMakeRect(60, yPos, 40, 20) isEditable:false];
        yPos -= 30;
        [m->Box setAlignment:NSTextAlignmentCenter];
        [[item view] addSubview:m->Box]; [[item view] addSubview:m->Label];
        mainVC->miscAttack[i] = m;
    }
         
    yPos = 670;
    int xPos = 340;
    for(NSInteger i = 0; i < SKILL_NUM; i++) {
        LabeledTextBox *Skill = [[LabeledTextBox alloc] initLabeled:SkillNames[i] labelDir:LABEL_LEFT frame:NSMakeRect(xPos, yPos, 40, 20) isEditable:false];
        [Skill->Box setAlignment:NSTextAlignmentCenter];
        
        ActionButton *mark = [[ActionButton alloc] initSymNoAction:NSMakeRect(xPos+45, yPos, 20, 22) sym:@"O"];
        yPos -= 20;
        
        if(yPos < 320) { yPos = 670; xPos = xPos + 45 + 210; }
        
        [[item view] addSubview:Skill->Box]; [[item view] addSubview:Skill->Label];
        [[item view] addSubview:mark->Button];
        mainVC->Skills[i] = Skill;
        mainVC->hasLevels[i] = mark;
    }
    
    NSInteger index = [mainVC->PCSelector indexOfSelectedItem];
    for(NSInteger i = 0; i < STATS_NUM; i++) {
        int v = mainVC->Party[index]->Stats[i];
        [mainVC->Stats[i] setIntValue:v];
        [mainVC->Stats[i]->Bonus setIntValue:ASBonus[v]];
    }
    for(NSInteger i = 0; i < ST_NUM; i++) {
        [mainVC->SavingThrows[i]->Box setIntValue:mainVC->Party[index]->SavingThrows[i]];
    }
    for(NSInteger i = 0; i < SKILL_NUM; i++) {
        [mainVC->Skills[i]->Box setIntValue:mainVC->Party[index]->Skills[i].Value];
        if(mainVC->Party[index]->Skills[i].markSet == SKILL_MARK_UNSET) {
            [mainVC->hasLevels[i]->Button setTitle:@"O"];
        }
        else if(mainVC->Party[index]->Skills[i].markSet == 0) {
            [mainVC->hasLevels[i]->Button setTitle:@"X"];
        }
        else if(mainVC->Party[index]->Skills[i].markSet == 1) {
            [mainVC->hasLevels[i]->Button setTitle:@"✓"];
        }
    }
    //TODO: Account for multiple bonuses
    [mainVC->miscAttack[0]->Box setIntValue:mainVC->Party[index]->AC];
    [mainVC->miscAttack[1]->Box setIntValue:mainVC->Party[index]->AttackBonus.Bonus[0]];
    [mainVC->miscAttack[2]->Box setIntValue:mainVC->Party[index]->CMB];
    [mainVC->miscAttack[3]->Box setIntValue:mainVC->Party[index]->CMD];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    mainVC = (ViewController *) NSApplication.sharedApplication.orderedWindows.firstObject.contentViewController;
    
    mainVC->currentTurnIdx = 0;
    
    NSWindow *MainWindow = [NSApp windows][0];
    //NOTE: Originally 1280x960
    [MainWindow setFrame:NSMakeRect(300, 160, 1280, 800) display:true];
    
    NSTabView __strong *MainTabView = [[NSTabView alloc] initWithFrame:NSMakeRect(0, 0, 1280, 780)];
    NSTabViewItem *item0 = [[NSTabViewItem alloc] init];
    [item0 setLabel:@"PC"];
    NSTabViewItem *item1 = [[NSTabViewItem alloc] init];
    [item1 setLabel:@"Feats"];
    NSTabViewItem *item2 = [[NSTabViewItem alloc] init];
    [item2 setLabel:@"Init"];
    NSTabViewItem *item3 = [[NSTabViewItem alloc] init];
    [item3 setLabel:@"Party"];

    //NOTE: This is the solution to fullscreen correct positioning.
    //      Maybe not...
    int resizeFlags = (NSViewMinXMargin | NSViewMaxXMargin | NSViewMinYMargin |
                       NSViewHeightSizable | NSViewWidthSizable);
    [MainTabView setAutoresizingMask:resizeFlags];
    
    [self SetupInitTab:item2];
    [mainVC resetOrder];
    
    [self SetupPartyTab:item3];
    
    NSArray *tabViewItems = [[NSArray alloc] initWithObjects:item0, item1, item2, item3, nil];
    [MainTabView setTabViewItems:tabViewItems];
    [MainTabView selectTabViewItemAtIndex:2];
    [[MainWindow contentView] addSubview:MainTabView];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
