/* date = May 22nd 2020 11:45 am */

#ifndef _STATE_GLOBALS_H
#define _STATE_GLOBALS_H

const u32 global_saveVersion = 0;

struct Encounter
{
    unistring name;
    
    u32 numMobs;
    unistring mobName[MOB_NUM];
    unistring mobBonus[MOB_NUM];
    unistring mobFinal[MOB_NUM];
    //unistring mobAC[MOB_NUM];
    
    u32 numAllies;
    unistring allyName[ALLY_NUM];
    unistring allyBonus[ALLY_NUM];
    unistring allyFinal[ALLY_NUM];
    //unistring allyAC[ALLY_NUM];
    
    u32 numThrowers;
    unistring throwerName[THROWER_NUM];
    unistring throwerHit[THROWER_NUM];
    unistring throwerDamage[THROWER_NUM];
};

struct EncList
{
    u32 numEncounters;
    Encounter Enc[64];
};

struct InitField
{
    UITextBox name;
    UITextBox bonus;
    UITextBox final;
    
    UITextBox addName;
    UITextBox addInit;
    
    UIButton  addNew;
    UIButton  addConfirm;
    b32 isAdding;
    
    s32 ID;
};

struct Counter
{
    UITextBox name;
    UITextBox rounds;
    s32 roundsLeft;
    
    UIButton  start;
    UIButton  plusOne;
    UIButton  stop;
    
    u32 startIdxInOrder;
    u32 turnCounter;
    b32 isActive;
};

struct Order
{
    UISlider  field;
    UITextBox pos;
    
    UIButton  remove;
    
    s32 ID;
};

struct DiceThrow
{
    UITextBox name;
    UITextBox toHit;
    UITextBox hitRes;
    UITextBox damage;
    UITextBox dmgRes;
    
    UIButton  throwDie;
};

struct InitPage
{
    UIListBox  Mobs;
    UIListBox  Allies;
    
    UITextBox  PlayerInit[PARTY_NUM];
    
    InitField  AllyFields[ALLY_NUM];
    InitField  MobFields[MOB_NUM];
    
    UIButton   Roll;
    UIButton   Set;
    UIButton   Reset;
    UIButton   Next;
    
    Order      OrderFields[ORDER_NUM];
    u32        turnsInRound;
    s32        orderAdjust;
    
    UITextBox  RoundCounter;
    u32        roundCount;
    
    UITextBox  Current;
    u32        currIdx;
    
    Counter    Counters[COUNTER_NUM];
    
    DiceThrow  Throwers[THROWER_NUM];
    DiceThrow  GeneralThrower;
    
    
    UIListBox  EncounterSel;
    UITextBox  EncounterName;
    UIButton   Save;
};

struct ProgramState
{
#if 0
    PCPage    *PC;
    FeatsPage *Feats;
#endif
    
    InitPage  *Init;
    
    //State Management
    b32       inBattle;
    EncList   encounters;
    
    b32 isInitialized;
    
    //Window management
    u32 windowWidth;
    u32 windowHeight;
    
    b32    isDragging;
    POINTS prevMousePos;
    POINTS currWindowPos;
    
    b32 hasMouseClicked;
    
    u64 timePassed;
    u64 dt = 0;
};


HINSTANCE MainInstance;
HWND MainWindow;
PlayerChar pc = {};
ProgramState State = {};


const HBRUSH appBkgBrush     = CreateSolidBrush(0x00383838); // 0x00 BB GG RR
const COLORREF appBkgRGB     = win32RGB(0x38, 0x38, 0x38);

const HBRUSH menuBkgBrush    = CreateSolidBrush(0x00787878);
const COLORREF menuBkgRGB    = win32RGB(0x78, 0x78, 0x78);

const HBRUSH whiteBrush      = CreateSolidBrush(0x00FFFFFF); // 0x00 BB GG RR
const COLORREF whiteRGB      = win32RGB(255, 255, 255);


//---- MENU ----//

const u32 FILE_MENU_SAVE_IDX = 0;
const u32 FILE_MENU_LOAD_IDX = 1;

const u32 MENU_FILE_ITEM_ID = 76;
const u32 MENU_CLOSE_APP_ID = 77;
const u32 MENU_FILE_IDX      = 0;
const u32 MENU_CLOSE_APP_IDX = 1;

HMENU MenuBar;
HMENU SubMenu;

#endif //_STATE_GLOBALS_H
