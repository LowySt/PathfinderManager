/* date = May 22nd 2020 11:45 am */

#ifndef _STATE_GLOBALS_H
#define _STATE_GLOBALS_H

#define FILE_MENU_SAVE_IDX 0
#define FILE_MENU_LOAD_IDX 1

enum LabelAlign
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
    
    HWND WindowsArray[64];
    u32 numWindows;
};

struct FeatsPage
{
    ListBox *Feats;
    TextBox *FeatsDesc;
    ListBox *ChosenFeats;
    u32 ChosenFeatsIndices[64];
    u8 usedFeats;
    
    HWND WindowsArray[64];
    u32 numWindows;
};

//NOTE: Size in Bytes of an Encounter
const u32 sizeOfEncEntry  = 760;
const u32 sizeOfInitEntry = 36; // 32 Bytes Name, 4 Bytes Bonus

const u32 encounterOffset = KBytes(128);

struct Encounter
{
    u32 numMobs;
    char mobNames[MOB_NUM][32];
    u32  mobBonus[MOB_NUM];
    
    u32 numAllies;
    char allyNames[ALLY_NUM][32];
    u32  allyBonus[ALLY_NUM];
};

struct EncList
{
    void *data;  //NOTE: Fixed size of 32 Kbytes
    
    u32 numEncounters;
    Encounter Enc[64];
    
    b32 isInitialized;
};

struct Counter
{
    TextBox *Field;
    TextBox *Rounds;
    Button  *Start;
    
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
    
    AddInit New;
    
    u64     id;
    b32     isSelected;
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
    
    HWND WindowsArray[512];
    u32 numWindows;
};

struct ProgramState
{
    PCPage    *PC;
    FeatsPage *Feats;
    InitPage  *Init;
    
    void *StateData;
    
    b32  isInitialized;
    
    //State Management
    b32       inBattle;
    EncList   encounters;
    
    //Window management
    b32    isDragging;
    POINTS prevMousePos;
    POINTS currWindowPos;
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

const COLORREF whiteRGB      = RGB(255, 255, 255);

const HBRUSH testColor  = CreateSolidBrush(0x008B1FA7);


//TODO: Can I manage to do things using this?
Element ElementMap[512] = {};

#endif //_STATE_GLOBALS_H
