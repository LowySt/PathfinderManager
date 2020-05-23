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
    
    u64     id;
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
    
    TextBox   *Current;
    u32       currIdx;
    
    Button     *Roll;
    Button     *Set;
    Button     *Next;
    Button     *Reset;
    
    HWND WindowsArray[256];
    u32 numWindows;
};

struct ProgramState
{
    PCPage    *PC;
    FeatsPage *Feats;
    InitPage  *Init;
    
    b32       inBattle;
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
PlayerChar pc = {};
ProgramState State = {};
const HBRUSH controlBkg = CreateSolidBrush(0x00565656); // 0x00 BB GG RR
const HBRUSH appBkg     = CreateSolidBrush(0x00383838); // 0x00 BB GG RR

//TODO: Can I manage to do things using this?
Element ElementMap[512] = {};

#endif //_STATE_GLOBALS_H
