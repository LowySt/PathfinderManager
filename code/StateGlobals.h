/* date = May 22nd 2020 11:45 am */

#ifndef _STATE_GLOBALS_H
#define _STATE_GLOBALS_H

const u32 global_saveVersion = 4;

const u32 WINARR_INIT_NUM  = 1024;
const u32 WINARR_PC_NUM    = 64;
const u32 WINARR_FEATS_NUM = 64;

const u32 ELEM_MAP_NUM = WINARR_INIT_NUM + WINARR_PC_NUM + WINARR_FEATS_NUM;

enum LabelAlign : u8
{
    LABEL_NULL,
    
    LABEL_UP,
    LABEL_DOWN,
    LABEL_LEFT,
    LABEL_RIGHT,
    
    LABEL_COUNT
};

struct dim
{
    u32 width;
    u32 height;
};

//TODO: Support Unicode in WindowCreation
struct TextBox
{
    HWND box;
    HWND label;
    
    u64 id;
};

struct ComboBox
{
    HWND box;
    HWND label;
    
    u64 id;
};

struct ListBox
{
    HWND box;
    HWND label;
    
    u64 id;
};

struct Button
{
    HWND box;
    
    union
    {
        TextBox *LinkedText;
    };
    u32 value;
    
    u64 id;
    
    b32 hasBackground;
};

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

struct Counter
{
    TextBox *Field;
    TextBox *Rounds;
    Button  *Start;
    
    Button  *PlusOne;
    Button  *Stop;
    
    u32 startIdxInOrder;
    u32 roundCounter;
    b32 isActive;
};

struct AddInit
{
    TextBox *Name;
    TextBox *Bonus;
    
    Button  *Add;
    Button  *Ok;
};

struct OrderField
{
    TextBox *Field;
    TextBox *Pos;
    
    Button  *Remove;
    
    u32 fieldId;
    b32 isMob;
    b32 isParty;
};

struct InitField
{
    TextBox *Name;
    TextBox *Bonus;
    TextBox *Final;
    
    TextBox *AC;
    TextBox *Touch;
    TextBox *NoDex;
    
    AddInit New;
    
    u64     id;
    b32     isSelected;
};

struct DiceThrow
{
    TextBox *Name;
    TextBox *ToHit;
    TextBox *HitRes;
    TextBox *Damage;
    TextBox *DmgRes;
    Button  *Throw;
};

struct InitPage
{
    ComboBox   *Mobs;
    ComboBox   *Allies;
    
    InitField  PlayerFields[PARTY_NUM];
    
    InitField  MobFields[MOB_NUM];
    u32        VisibleMobs;
    
    InitField  AllyFields[ALLY_NUM];
    u32        VisibleAllies;
    
    OrderField Order[ORDER_NUM];
    u32        VisibleOrder;
    u32        turnsInRound;
    
    TextBox    *Current;
    u32        currIdx;
    
    Button     *Roll;
    Button     *Set;
    Button     *Next;
    Button     *Reset;
    Button     *Save;
    
    ComboBox   *EncounterSel;
    TextBox    *EncounterName;
    
    Counter    Counters[COUNTER_NUM];
    TextBox    *RoundCounter;
    
    DiceThrow  Throwers[THROWER_NUM];
    DiceThrow  GeneralThrower;
    
    HWND WindowsArray[WINARR_INIT_NUM];
    u32 numWindows;
};

struct ProgramState
{
    PCPage    *PC;
    FeatsPage *Feats;
    InitPage  *Init;
    
    //State Management
    b32       inBattle;
    EncList   encounters;
    
    b32 isInitialized;
    
    //Window management
    b32    isDragging;
    POINTS prevMousePos;
    POINTS currWindowPos;
    
    b32 hasMouseClicked;
    
    u64 timePassed;
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
const COLORREF controlBkgRGB = RGB(0x56, 0x56, 0x56);

const HBRUSH appBkgBrush     = CreateSolidBrush(0x00383838); // 0x00 BB GG RR
const COLORREF appBkgRGB     = RGB(0x38, 0x38, 0x38);

const HBRUSH menuBkgBrush    = CreateSolidBrush(0x00787878);
const COLORREF menuBkgRGB    = RGB(0x78, 0x78, 0x78);

const HBRUSH whiteBrush      = CreateSolidBrush(0x00FFFFFF); // 0x00 BB GG RR
const COLORREF whiteRGB      = RGB(255, 255, 255);

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

Element ElementMap[ELEM_MAP_NUM] = {};

#endif //_STATE_GLOBALS_H
