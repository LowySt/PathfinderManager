/* date = May 22nd 2020 11:51 am */
#ifndef _INIT_H
#define _INIT_H

utf32 ConstNumberStrings[32] = { {(u32 *)U"0",1,1} , {(u32 *)U"1",1,1}, {(u32 *)U"2",1,1}, {(u32 *)U"3",1,1},
    {(u32 *)U"4",1,1}, {(u32 *)U"5",1,1}, {(u32 *)U"6",1,1}, {(u32 *)U"7",1,1}, {(u32 *)U"8",1,1},
    {(u32 *)U"9",1,1}, {(u32 *)U"10",2,2}, {(u32 *)U"11",2,2}, {(u32 *)U"12",2,2}, {(u32 *)U"13",2,2},
    {(u32 *)U"14",2,2}, {(u32 *)U"15",2,2}, {(u32 *)U"16",2,2}, {(u32 *)U"17",2,2}, {(u32 *)U"18",2,2},
    {(u32 *)U"19",2,2}, {(u32 *)U"20",2,2}, {(u32 *)U"21",2,2}, {(u32 *)U"22",2,2}, {(u32 *)U"23",2,2},
    {(u32 *)U"24",2,2}, {(u32 *)U"25",2,2}, {(u32 *)U"26",2,2}, {(u32 *)U"27",2,2}, {(u32 *)U"28",2,2},
    {(u32 *)U"29",2,2}, {(u32 *)U"30",2,2}, {(u32 *)U"31",2,2} };

const u32 PARTY_NUM = 4;
const char32_t *PartyName[PARTY_NUM] = { U"Efrea", U"Sigismondo", U"Alice", U"Bruce" };
const utf32 PartyNameUTF32[PARTY_NUM] = {
    {(u32 *)PartyName[0], 5, 5},
    {(u32 *)PartyName[1], 10, 10},
    {(u32 *)PartyName[2], 5, 5},
    {(u32 *)PartyName[3], 5, 5},
};

const u32 MOB_NUM = 24;
const char32_t *MobName[MOB_NUM] = { U"Enemy 1", U"Enemy 2", U"Enemy 3", U"Enemy 4",
    U"Enemy 5", U"Enemy 6", U"Enemy 7", U"Enemy 8", U"Enemy 9", U"Enemy 10", U"Enemy 11", U"Enemy 12",
    U"Enemy 13", U"Enemy 14", U"Enemy 15", U"Enemy 16", U"Enemy 17", U"Enemy 18", U"Enemy 19", 
    U"Enemy 20", U"Enemy 21", U"Enemy 22", U"Enemy 23", U"Enemy 24" };

const char *Enemies[MOB_NUM + 1] = {
    "No Enemies", "1 Enemy", "2 Enemies", "3 Enemies", "4 Enemies", "5 Enemies", "6 Enemies",
    "7 Enemies", "8 Enemies", "9 Enemies", "10 Enemies", "11 Enemies", "12 Enemies",
    "13 Enemies", "14 Enemies", "15 Enemies", "16 Enemies", "17 Enemies", "18 Enemies", "19 Enemies",
    "20 Enemies", "21 Enemies", "22 Enemies", "23 Enemies", "24 Enemies"
};


const u32 ALLY_NUM = 8;
const char32_t *AllyName[ALLY_NUM] = { U"Ally 1", U"Ally 2", U"Ally 3", U"Ally 4",
    U"Ally 5", U"Ally 6", U"Ally 7", U"Ally 8"
};

const char *Allies[ALLY_NUM + 1] = {
    "No Allies", "1 Ally", "2 Allies", "3 Allies", "4 Allies", "5 Allies", "6 Allies", "7 Allies", "8 Allies"
};

const u32 ORDER_NUM = PARTY_NUM + MOB_NUM + ALLY_NUM;

const u32 COUNTER_NUM       = 9;
const u32 DEF_COUNTER_NUM   = 8;
const u32 PRANA_COUNTER_NUM = 6;
const char32_t *CounterNames[COUNTER_NUM] = { U"Counter 1", U"Counter 2", U"Counter 3", U"Counter 4",
    U"Counter 5", U"Counter 6", U"Counter 7", U"Counter 8", U"Counter 9" };


const u32 THROWER_NUM       = 8;
const u32 DEF_THROWER_NUM   = 6;
const u32 PRANA_THROWER_NUM = 8;

const char32_t *NoEncounterStr = U"No Selection";

struct tmp_order
{
    s32    init;
    utf32 *name;
    s32    maxLife;
    
    s32    compendiumIdx;
    s32    ID;
};

const u32 INIT_FIELD_EDITFIELDS_NUM = 11;

struct InitField;
struct MobLifeHandler
{
    InitField *mob;
    UITextBox *parent;
    
    utf32      previous;
    b32        isEditing;
};

struct Order;
struct OrderHandler
{
    Order     *order;
    UITextBox *parent;
    
    utf32      previous;
    b32        isEditing;
};

struct CustomFieldTextHandler
{
    InitField *parent;
    UITextBox *field;
    
    u32 if_idx;
};

static s32 addID = 1000;

enum InitStyle
{
    INIT_STYLE_DEFAULT,
    INIT_STYLE_PRANA
};

static InitStyle currentStyle = INIT_STYLE_PRANA;

enum Ruleset
{
    RULESET_DEFAULT,
    RULESET_PRANA
};

static Ruleset currentRuleset = RULESET_PRANA;

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

struct EncounterInitEntry
{
    utf32 fields[MOB_INIT_ENC_FIELDS];
    s32 compendiumIdx;
};

struct Encounter
{
    utf32 name;
    
    u32 numMobs;
    EncounterInitEntry mob[MOB_NUM];
    
    u32 numAllies;
    EncounterInitEntry ally[ALLY_NUM];
    
    utf32 throwerName[THROWER_NUM];
    utf32 throwerHit[THROWER_NUM];
    utf32 throwerDamage[THROWER_NUM];
};

struct EncList
{
    u32 numEncounters;
    Encounter Enc[64];
};

const s32 STATUS_TOOLTIP_DT = 350;

enum StatusType
{
    STATUS_ABBAGLIATO = 0,   // -1 TxC e Percezione (su vista)
    STATUS_ACCECATO,         // -2 CA, lose DEX to CA, -4 on Skills on STR/DEX and against Perception 
    STATUS_ACCOVACCIATO,     // -2 CA, lose DEX bonus
    STATUS_AFFASCINATO,      // -4 Skill checks as a reaction (ex Perception) TODO
    STATUS_AFFATICATO,       // -2 STR/DEX
    STATUS_ASSORDATO,        // -4 Init
    STATUS_BARCOLLANTE,      // No numeric change
    STATUS_CONFUSO,          // No numeric change
    STATUS_ESAUSTO,          // -6 STR/DEX
    STATUS_IMMOBILIZZATO,    // Flat footed (CA/DMC), -4 CA NOTE EXCEPT SCHIVARE PRODIGIOSO
    STATUS_IMPREPARATO,      // Flat footed (CA/DMC)        NOTE EXCEPT SCHIVARE PRODIGIOSO
    STATUS_INDIFESO,         // DEX -> 0
    STATUS_INFERMO,          // -2 TxC/Damage/SavingThrows/SkillChecks/AbilityChecks
    STATUS_INTRALCIATO,      // -2 TxC e -4 DEX
    STATUS_INVISIBILE,       // +2 TxC
    STATUS_LOTTA,            // -4 DEX, -2 TxC/(BMC except for freeing/fighting)
    STATUS_NAUSEATO,         // No numeric change
    STATUS_PANICO,           // -2 TxC/SavingThrows/Checks
    STATUS_PARALIZZATO,      // STR/DEX -> 0
    STATUS_PIETRIFICATO,     // DEX -> 0
    STATUS_PRONO,            // -4 TxC, TODO +4 CA Ranged / -4 CA Melee
    STATUS_SANGUINANTE,      // No numeric change
    STATUS_SCOSSO,           // -2 TxC/SavingThrows/Checks
    STATUS_SPAVENTATO,       // -2 TxC/SavingThrows/Checks
    STATUS_STORDITO,         // -2 CA, lose DEX Bonus to CA
    
    //NOTE: CAREFUL! For serialization ease new status conditions HAVE to be added after the last one!
    STATUS_COUNT
};

const s32 MAX_STATUS = 32;

struct Status
{
    StatusType type;
    UICheck check;
};

struct InitField
{
    //NOTE: This makes referring to other boxes in the same InitField much easier
    //      For things like: CustomInitFieldText Handler.
    UITextBox editFields[IF_IDX_COUNT];
    UITextBox maxLife;
    
    s32 compendiumIdx;
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
    
    Status status[MAX_STATUS];
    
    s32 compendiumIdx;
    s32 ID;
};

struct DiceThrowBox
{
    UITextBox name;
    UITextBox toHit;
    UITextBox hitRes;
    UITextBox damage;
    UITextBox dmgRes;
    
    UIButton  throwDie;
};

s32 globalSelectedIndex = -1;
//b32 globalIsAdding = FALSE;

struct InitPage
{
    UIListBox    Mobs;
    UIListBox    Allies;
    
    UITextBox    PlayerInit[PARTY_NUM];
    
    InitField    AllyFields[ALLY_NUM];
    InitField    MobFields[MOB_NUM];
    
    UIButton     addNewMob;
    UIButton     addNewAlly;
    b32          isAdding;
    
    UIButton     Roll;
    UIButton     Set;
    UIButton     Reset;
    UIButton     Next;
    
    Order        OrderFields[ORDER_NUM];
    s32          turnsInRound;
    s32          orderAdjust;
    
    UITextBox    RoundCounter;
    u32          roundCount;
    
    UITextBox    Current;
    u32          currIdx;
    
    Counter      Counters[COUNTER_NUM];
    
    DiceThrowBox Throwers[THROWER_NUM];
    DiceThrowBox GeneralThrower;
    
    UIListBox    EncounterSel;
    UITextBox    EncounterName;
    UIButton     SaveEnc;
    UIButton     RemoveEnc;
    UIButton     AddEnc;
    
    UIButton     Undo;
    UIButton     Redo;
    
    
    b32          isShowingStatusTooltip;
    s32          tooltipCurrentDT;
    s32          tooltipMouseX;
    s32          tooltipMouseY;
};

#endif //_INIT_H
