/* date = May 22nd 2020 11:45 am */

#ifndef _STATE_GLOBALS_H
#define _STATE_GLOBALS_H

const u32 global_saveVersion = 4;

#if 0
struct AbilityScores
{
    TextBox *Box[6];
    TextBox *Bonus[6];
    Button *Plus1[6];
    Button *Plus10[6];
};

struct PCPage
{
    HWND TabControl; //DEBUG
    
    TextBox *Name;
    TextBox *Player;
    ComboBox *Race;
    
    ComboBox *Class;
    b32 wasClassChosen;
    
    AbilityScores *Scores;
    
    ListBox *RacialTraits;
    
    TextBox *currLevel;
    TextBox *currXP;
    TextBox *nextLevelXP;
    ComboBox *XPCurve;
    XPCurveIdx xpIdx;
    
    TextBox *SavingThrows[3];
    TextBox *BaseAttackBonus;
    
    HWND WindowsArray[WINARR_PC_NUM];
    u32 numWindows;
};

struct FeatsPage
{
    ListBox *Feats;
    TextBox *FeatsDesc;
    ListBox *ChosenFeats;
    u32 ChosenFeatsIndices[64];
    u8 usedFeats;
    
    HWND WindowsArray[WINARR_FEATS_NUM];
    u32 numWindows;
};
#endif

struct Encounter
{
    char name[32];
    
    u32 numMobs;
    char mobNames[MOB_NUM][32];
    char mobBonus[MOB_NUM][8];
    char mobAC[MOB_NUM][8];
    
    u32 numAllies;
    char allyNames[ALLY_NUM][32];
    char allyBonus[ALLY_NUM][8];
    char allyAC[ALLY_NUM][8];
    
    char throwerNames[THROWER_NUM][32];
    char throwerHit[THROWER_NUM][64];
    char throwerDamage[THROWER_NUM][64];
};

struct EncList
{
    u32 numEncounters;
    Encounter Enc[64];
};

#if 0

struct AddInit
{
    TextBox *Name;
    TextBox *Bonus;
    
    Button  *Add;
    Button  *Ok;
};

#endif

struct InitField
{
    UITextBox name;
    UITextBox bonus;
    UITextBox final;
    
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
    u32 roundCounter;
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
    
#if 0
    Button     *Save;
    
    ComboBox   *EncounterSel;
    TextBox    *EncounterName;
#endif
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

struct Element
{
    void *ptr;
    
    b32 isButton;
    b32 isTextBox;
    b32 isComboBox;
    b32 isListBox;
};


HINSTANCE MainInstance;
HWND MainWindow;
PlayerChar pc = {};
ProgramState State = {};

const HBRUSH controlBkgBrush = CreateSolidBrush(0x00565656); // 0x00 BB GG RR
const COLORREF controlBkgRGB = win32RGB(0x56, 0x56, 0x56);

const HBRUSH appBkgBrush     = CreateSolidBrush(0x00383838); // 0x00 BB GG RR
const COLORREF appBkgRGB     = win32RGB(0x38, 0x38, 0x38);

const HBRUSH menuBkgBrush    = CreateSolidBrush(0x00787878);
const COLORREF menuBkgRGB    = win32RGB(0x78, 0x78, 0x78);

const HBRUSH whiteBrush      = CreateSolidBrush(0x00FFFFFF); // 0x00 BB GG RR
const COLORREF whiteRGB      = win32RGB(255, 255, 255);

const HBRUSH testColor  = CreateSolidBrush(0x008B1FA7);


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
