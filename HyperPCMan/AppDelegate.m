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

- (void)SetupInitTab:(NSTabViewItem *)item {
    
    NSArray *HeroNames = @[ @"Gremag", @"Federico", @"Ken Shiro", @"Sirion", @"Albion", @"Sdentato", @"Dresdam", @"Zoddak"];
           
    NSArray *AllyNames = @[@"Ally 1", @"Ally 2", @"Ally 3", @"Ally 4"];
    
    NSArray *EnemyNames = @[ @"Enemy 1", @"Enemy 2", @"Enemy 3", @"Enemy 4", @"Enemy 5", @"Enemy 6", @"Enemy 7", @"Enemy 8", @"Enemy 9", @"Enemy 10", @"Enemy 11", @"Enemy 12", @"Enemy 13", @"Enemy 14", @"Enemy 15", @"Enemy 16"];
    
    NSArray *counterNames = @[@"Counter 1", @"Counter 2",
                              @"Counter 3", @"Counter 4"];
            
    int yPos = 750;
    for(int i = 0; i < mainViewController->PARTY_SIZE; i++) {
    
        //TODO: Remove the formatter from hero. Maybe make it a BattleEntity
        //      as well?
        NSNumberFormatter *nf = [[NSNumberFormatter alloc] init];
        [nf setAllowsFloats:false];
        [nf setNumberStyle:NSNumberFormatterNoStyle];
        [nf setPartialStringValidationEnabled:true];
        
        LabeledTextBox *Hero = [[LabeledTextBox alloc]
                            initLabeled:HeroNames[i] labelDir:LABEL_LEFT
                            p:CGPointMake(525, yPos) isEditable:true];
        [Hero->Box setFrameSize:NSMakeSize(30, 20)];
        Hero->Box.formatter = nf;
    
        [[item view] addSubview:Hero->Box];
        [[item view] addSubview:Hero->Label];
        mainViewController->Heros[i] = Hero;
        yPos -= 30;
    }
        
    for(int i = 0; i < mainViewController->ALLY_SIZE; i++) {
    
        BattleEntity *Ally = [[BattleEntity alloc] initWithFrame:NSMakeRect(525, yPos, 70, 20) name:AllyNames[i]];
                   
        [[item view] addSubview:Ally->Box->Box];
    	[[item view] addSubview:Ally->Box->Label];
        [[item view] addSubview:Ally->Init];
        mainViewController->Allies[i] = Ally;
        yPos -= 30;
    }
 
    yPos = 750;
    for(int i = 0; i < mainViewController->MOB_SIZE; i++) {

        BattleEntity *Mob = [[BattleEntity alloc] initWithFrame:NSMakeRect(365, yPos, 70, 20) name:EnemyNames[i]];
        [[item view] addSubview:Mob->Box->Box];
        [[item view] addSubview:Mob->Box->Label];
        [[item view] addSubview:Mob->Init];
        mainViewController->Mobs[i] = Mob;
        yPos -= 30;
    }
    
    yPos = 750;
    for(int i = 0; i < mainViewController->ORDER_SIZE; i++) {
        OrderField *Order = [[OrderField alloc]
                             initOrder:CGPointMake(680, yPos) num:i+1];
        
        [[item view] addSubview:Order->Name];
        [[item view] addSubview:Order->Num];
        mainViewController->Order[i] = Order;
        yPos -= 22;
    }
    
    mainViewController->CurrentInTurn = [[NSTextField alloc] initWithFrame:NSMakeRect(820, 750, 120, 20)];
    [mainViewController->CurrentInTurn setAlignment:NSTextAlignmentCenter];
    [mainViewController->CurrentInTurn setEditable:false];
    [[item view] addSubview:mainViewController->CurrentInTurn];
    
    mainViewController->RoundCount = [[NSTextField alloc] initWithFrame:NSMakeRect(1200, 820, 30, 20)];
    [mainViewController->RoundCount setAlignment:NSTextAlignmentCenter];
    [mainViewController->RoundCount setEditable:false];
    [[item view] addSubview:mainViewController->RoundCount];
    
    ActionButton *Reset = [[ActionButton alloc] initWithAction:NSMakeRect(20, 810, 80, 24) name:@"Reset" blk:^void(){
        NSLog(@"Reset Button!");
    }];
        
    ActionButton *Map = [[ActionButton alloc] initWithAction:NSMakeRect(100, 810, 80, 24) name:@"Map" blk:^void(){
        NSLog(@"Map Button!");
    }];
        
    ActionButton *Roll = [[ActionButton alloc] initWithAction:NSMakeRect(420, 780, 80, 24) name:@"Roll" blk:^void(){
        NSLog(@"Roll Button!");
    }];
    
    ActionButton *Set = [[ActionButton alloc] initWithAction:NSMakeRect(695, 780, 80, 24) name:@"Set" blk:^void(){
        NSLog(@"Set Button!");
    }];
    
    ActionButton *Next = [[ActionButton alloc] initWithAction:NSMakeRect(840, 780, 80, 24) name:@"Next" blk:^void(){
        NSLog(@"Next Button!");
    }];
    
    [[item view] addSubview:Reset->Button]; [[item view] addSubview:Map->Button];
    [[item view] addSubview:Roll->Button]; [[item view] addSubview:Set->Button];
    [[item view] addSubview:Next->Button];
    mainViewController->Reset = Reset; mainViewController->Map = Map;
    mainViewController->Roll = Roll; mainViewController->Set = Set;
    mainViewController->Next = Next;
    
    yPos = 760;
    for(int i = 0; i < mainViewController->COUNTER_SIZE; i++) {
        
        Counter *C = [[Counter alloc] initWithFrame:NSMakeRect(20, yPos, 80, 20) name:counterNames[i]];
        [[item view] addSubview:C->Field->Box];
        [[item view] addSubview:C->Field->Label];
        [[item view] addSubview:C->Count];
        [[item view] addSubview:C->SetButton->Button];
        
        mainViewController->Counters[i] = C;
        
        yPos -= 40;
    }

    NSArray *MobSelValues = @[@"No Enemies", @"1 Enemy", @"2 Enemies", @"3 Enemies", @"4 Enemies", @"5 Enemies", @"6 Enemies", @"7 Enemies", @"8 Enemies", @"9 Enemies", @"10 Enemies", @"11 Enemies", @"12 Enemies", @"13 Enemies", @"14 Enemies", @"15 Enemies", @"16 Enemies"];
    NSComboBox *MobSel = [[NSComboBox alloc] initWithFrame:NSMakeRect(315, 780, 100, 25)];
    [MobSel addItemsWithObjectValues:MobSelValues];
    [MobSel selectItemAtIndex:0];
    [[item view] addSubview:MobSel];
    mainViewController->MobSelector = MobSel;
    
    NSArray *AllySelValues = @[@"No Allies", @"1 Ally", @"2 Allies", @"3 Allies", @"4 Allies"];
    NSComboBox *AllySel = [[NSComboBox alloc] initWithFrame:NSMakeRect(500, 780, 100, 25)];
    [AllySel addItemsWithObjectValues:AllySelValues];
    [AllySel selectItemAtIndex:0];
    [[item view] addSubview:AllySel];
    mainViewController->AllySelector = AllySel;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    mainViewController = (ViewController *) NSApplication.sharedApplication.orderedWindows.firstObject.contentViewController;
    
    mainViewController->PARTY_SIZE = 8;
    mainViewController->ALLY_SIZE  = 4;
    mainViewController->MOB_SIZE   = 16;
    mainViewController->ORDER_SIZE = 28;
    mainViewController->COUNTER_SIZE = 4;
    
    NSWindow *MainWindow = [NSApp windows][0];
    [MainWindow setFrame:NSMakeRect(300, 160, 1280, 960) display:true];
    
    NSTabView __strong *MainTabView = [[NSTabView alloc] initWithFrame:NSMakeRect(0, 0, 1280, 890)];
    NSTabViewItem *item0 = [[NSTabViewItem alloc] init];
    [item0 setLabel:@"PC"];
    NSTabViewItem *item1 = [[NSTabViewItem alloc] init];
    [item1 setLabel:@"Feats"];
    NSTabViewItem *item2 = [[NSTabViewItem alloc] init];
    [item2 setLabel:@"Init"];
    NSTabViewItem *item3 = [[NSTabViewItem alloc] init];
    [item3 setLabel:@"Party"];

    [self SetupInitTab:item2];
    
    NSArray *tabViewItems = [[NSArray alloc] initWithObjects:item0, item1, item2, item3, nil];
    [MainTabView setTabViewItems:tabViewItems];
    [MainTabView selectTabViewItemAtIndex:2];
    [[MainWindow contentView] addSubview:MainTabView];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
