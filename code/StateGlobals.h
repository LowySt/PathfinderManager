/* date = May 22nd 2020 11:45 am */

#ifndef _STATE_GLOBALS_H
#define _STATE_GLOBALS_H

const u32 global_saveVersion = 3;

enum InitFieldIndex
{
    IF_IDX_NAME = 0,
    IF_IDX_BONUS,
    IF_IDX_EXTRA,
    IF_IDX_TOTALAC,
    IF_IDX_TOUCHAC,
    IF_IDX_FLATAC,
    IF_IDX_LOWAC,
    IF_IDX_CONSAVE,
    IF_IDX_DEXSAVE,
    IF_IDX_WISSAVE,
    
    //NOTE: Final HAS to be the last entry because 
    //      CustomInitFieldText depends on its position
    IF_IDX_FINAL,
    
    IF_IDX_COUNT
};

const u32 MOB_INIT_ENC_FIELDS = IF_IDX_COUNT+1;

struct Encounter
{
    unistring name;
    
    u32 numMobs;
    unistring mob[MOB_NUM][MOB_INIT_ENC_FIELDS];
    
    u32 numAllies;
    unistring allyName[ALLY_NUM];
    unistring allyBonus[ALLY_NUM];
    unistring allyFinal[ALLY_NUM];
    
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
    //NOTE: This makes referring to other boxes in the same InitField much easier
    //      For things like: CustomInitFieldText Handler.
    UITextBox editFields[IF_IDX_COUNT];
    
    UITextBox maxLife;
    
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
    
    UILPane    InfoPane;
    
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
    InitPage  *Init;
    
    //State Management
    b32       inBattle;
    EncList   encounters;
    
    b32 isInitialized;
    
    //Window management
    u32 windowWidth;
    u32 windowHeight;
    
    b32    isDragging;
    POINT  prevMousePos;
    POINTS currWindowPos;
    
    b32 hasMouseClicked;
    
    u64 timePassed;
    u64 dt = 0;
};


HINSTANCE MainInstance;
HWND MainWindow;
PlayerChar pc = {};
ProgramState State = {};


#endif //_STATE_GLOBALS_H
