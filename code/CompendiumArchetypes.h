#ifndef _COMPENDIUM_ARCHETYPES_H
#define _COMPENDIUM_ARCHETYPES_H

const s32 MAX_CONCURRENT_ARCHETYPES = 4;
const s32 MAX_ARCHETYPES = 4;

const char32_t *archetypeName[MAX_ARCHETYPES] = {
    U"Avanzato", U"Celestiale", U"Immondo", U"Gigante"
};

const s32 gsSetCount = 396;
const s32 rmIncreaseStride = 36;
const utf32 gsSet[gsSetCount] = {
    //NOTE: Standard GS without RM
    U"1/8"_W, U"1/6"_W, U"1/4"_W, U"1/3"_W, U"1/2"_W, U"1"_W, U"2"_W, U"3"_W, U"4"_W, U"5"_W, U"6"_W, U"7"_W,
    U"8"_W, U"9"_W, U"10"_W, U"11"_W, U"12"_W, U"13"_W, U"14"_W, U"15"_W, U"16"_W, U"17"_W, U"18"_W, U"19"_W,
    U"20"_W, U"21"_W, U"22"_W, U"23"_W, U"24"_W, U"25"_W, U"26"_W, U"27"_W, U"28"_W, U"29"_W, U"30"_W, U"31"_W,
    
    //NOTE: RM 1
    U"1/8/RM 1"_W, U"1/6/RM 1"_W, U"1/4/RM 1"_W, U"1/3/RM 1"_W, U"1/2/RM 1"_W, U"1/RM 1"_W, 
    U"2/RM 1"_W, U"3/RM 1"_W, U"4/RM 1"_W, U"5/RM 1"_W, U"6/RM 1"_W, U"7/RM 1"_W,
    U"8/RM 1"_W, U"9/RM 1"_W, U"10/RM 1"_W, U"11/RM 1"_W, U"12/RM 1"_W, U"13/RM 1"_W,
    U"14/RM 1"_W, U"15/RM 1"_W, U"16/RM 1"_W, U"17/RM 1"_W, U"18/RM 1"_W, U"19/RM 1"_W,
    U"20/RM 1"_W, U"21/RM 1"_W, U"22/RM 1"_W, U"23/RM 1"_W, U"24/RM 1"_W, U"25/RM 1"_W,
    U"26/RM 1"_W, U"27/RM 1"_W, U"28/RM 1"_W, U"29/RM 1"_W, U"30/RM 1"_W, U"31/RM 1"_W,
    
    //NOTE: RM 2
    U"1/8/RM 2"_W, U"1/6/RM 2"_W, U"1/4/RM 2"_W, U"1/3/RM 2"_W, U"1/2/RM 2"_W, U"1/RM 2"_W, 
    U"2/RM 2"_W, U"3/RM 2"_W, U"4/RM 2"_W, U"5/RM 2"_W, U"6/RM 2"_W, U"7/RM 2"_W,
    U"8/RM 2"_W, U"9/RM 2"_W, U"10/RM 2"_W, U"11/RM 2"_W, U"12/RM 2"_W, U"13/RM 2"_W,
    U"14/RM 2"_W, U"15/RM 2"_W, U"16/RM 2"_W, U"17/RM 2"_W, U"18/RM 2"_W, U"19/RM 2"_W,
    U"20/RM 2"_W, U"21/RM 2"_W, U"22/RM 2"_W, U"23/RM 2"_W, U"24/RM 2"_W, U"25/RM 2"_W,
    U"26/RM 2"_W, U"27/RM 2"_W, U"28/RM 2"_W, U"29/RM 2"_W, U"30/RM 2"_W, U"31/RM 2"_W,
    
    //NOTE: RM 3
    U"1/8/RM 3"_W, U"1/6/RM 3"_W, U"1/4/RM 3"_W, U"1/3/RM 3"_W, U"1/2/RM 3"_W, U"1/RM 3"_W, 
    U"2/RM 3"_W, U"3/RM 3"_W, U"4/RM 3"_W, U"5/RM 3"_W, U"6/RM 3"_W, U"7/RM 3"_W,
    U"8/RM 3"_W, U"9/RM 3"_W, U"10/RM 3"_W, U"11/RM 3"_W, U"12/RM 3"_W, U"13/RM 3"_W,
    U"14/RM 3"_W, U"15/RM 3"_W, U"16/RM 3"_W, U"17/RM 3"_W, U"18/RM 3"_W, U"19/RM 3"_W,
    U"20/RM 3"_W, U"21/RM 3"_W, U"22/RM 3"_W, U"23/RM 3"_W, U"24/RM 3"_W, U"25/RM 3"_W,
    U"26/RM 3"_W, U"27/RM 3"_W, U"28/RM 3"_W, U"29/RM 3"_W, U"30/RM 3"_W, U"31/RM 3"_W,
    
    //NOTE: RM 4
    U"1/8/RM 4"_W, U"1/6/RM 4"_W, U"1/4/RM 4"_W, U"1/3/RM 4"_W, U"1/2/RM 4"_W, U"1/RM 4"_W, 
    U"2/RM 4"_W, U"3/RM 4"_W, U"4/RM 4"_W, U"5/RM 4"_W, U"6/RM 4"_W, U"7/RM 4"_W,
    U"8/RM 4"_W, U"9/RM 4"_W, U"10/RM 4"_W, U"11/RM 4"_W, U"12/RM 4"_W, U"13/RM 4"_W,
    U"14/RM 4"_W, U"15/RM 4"_W, U"16/RM 4"_W, U"17/RM 4"_W, U"18/RM 4"_W, U"19/RM 4"_W,
    U"20/RM 4"_W, U"21/RM 4"_W, U"22/RM 4"_W, U"23/RM 4"_W, U"24/RM 4"_W, U"25/RM 4"_W,
    U"26/RM 4"_W, U"27/RM 4"_W, U"28/RM 4"_W, U"29/RM 4"_W, U"30/RM 4"_W, U"31/RM 4"_W,
    
    //NOTE: RM 5
    U"1/8/RM 5"_W, U"1/6/RM 5"_W, U"1/4/RM 5"_W, U"1/3/RM 5"_W, U"1/2/RM 5"_W, U"1/RM 5"_W, 
    U"2/RM 5"_W, U"3/RM 5"_W, U"4/RM 5"_W, U"5/RM 5"_W, U"6/RM 5"_W, U"7/RM 5"_W,
    U"8/RM 5"_W, U"9/RM 5"_W, U"10/RM 5"_W, U"11/RM 5"_W, U"12/RM 5"_W, U"13/RM 5"_W,
    U"14/RM 5"_W, U"15/RM 5"_W, U"16/RM 5"_W, U"17/RM 5"_W, U"18/RM 5"_W, U"19/RM 5"_W,
    U"20/RM 5"_W, U"21/RM 5"_W, U"22/RM 5"_W, U"23/RM 5"_W, U"24/RM 5"_W, U"25/RM 5"_W,
    U"26/RM 5"_W, U"27/RM 5"_W, U"28/RM 5"_W, U"29/RM 5"_W, U"30/RM 5"_W, U"31/RM 5"_W,
    
    //NOTE: RM 6
    U"1/8/RM 6"_W, U"1/6/RM 6"_W, U"1/4/RM 6"_W, U"1/3/RM 6"_W, U"1/2/RM 6"_W, U"1/RM 6"_W, 
    U"2/RM 6"_W, U"3/RM 6"_W, U"4/RM 6"_W, U"5/RM 6"_W, U"6/RM 6"_W, U"7/RM 6"_W,
    U"8/RM 6"_W, U"9/RM 6"_W, U"10/RM 6"_W, U"11/RM 6"_W, U"12/RM 6"_W, U"13/RM 6"_W,
    U"14/RM 6"_W, U"15/RM 6"_W, U"16/RM 6"_W, U"17/RM 6"_W, U"18/RM 6"_W, U"19/RM 6"_W,
    U"20/RM 6"_W, U"21/RM 6"_W, U"22/RM 6"_W, U"23/RM 6"_W, U"24/RM 6"_W, U"25/RM 6"_W,
    U"26/RM 6"_W, U"27/RM 6"_W, U"28/RM 6"_W, U"29/RM 6"_W, U"30/RM 6"_W, U"31/RM 6"_W,
    
    //NOTE: RM 7
    U"1/8/RM 7"_W, U"1/6/RM 7"_W, U"1/4/RM 7"_W, U"1/3/RM 7"_W, U"1/2/RM 7"_W, U"1/RM 7"_W, 
    U"2/RM 7"_W, U"3/RM 7"_W, U"4/RM 7"_W, U"5/RM 7"_W, U"6/RM 7"_W, U"7/RM 7"_W,
    U"8/RM 7"_W, U"9/RM 7"_W, U"10/RM 7"_W, U"11/RM 7"_W, U"12/RM 7"_W, U"13/RM 7"_W,
    U"14/RM 7"_W, U"15/RM 7"_W, U"16/RM 7"_W, U"17/RM 7"_W, U"18/RM 7"_W, U"19/RM 7"_W,
    U"20/RM 7"_W, U"21/RM 7"_W, U"22/RM 7"_W, U"23/RM 7"_W, U"24/RM 7"_W, U"25/RM 7"_W,
    U"26/RM 7"_W, U"27/RM 7"_W, U"28/RM 7"_W, U"29/RM 7"_W, U"30/RM 7"_W, U"31/RM 7"_W,
    
    //NOTE: RM 8
    U"1/8/RM 8"_W, U"1/6/RM 8"_W, U"1/4/RM 8"_W, U"1/3/RM 8"_W, U"1/2/RM 8"_W, U"1/RM 8"_W, 
    U"2/RM 8"_W, U"3/RM 8"_W, U"4/RM 8"_W, U"5/RM 8"_W, U"6/RM 8"_W, U"7/RM 8"_W,
    U"8/RM 8"_W, U"9/RM 8"_W, U"10/RM 8"_W, U"11/RM 8"_W, U"12/RM 8"_W, U"13/RM 8"_W,
    U"14/RM 8"_W, U"15/RM 8"_W, U"16/RM 8"_W, U"17/RM 8"_W, U"18/RM 8"_W, U"19/RM 8"_W,
    U"20/RM 8"_W, U"21/RM 8"_W, U"22/RM 8"_W, U"23/RM 8"_W, U"24/RM 8"_W, U"25/RM 8"_W,
    U"26/RM 8"_W, U"27/RM 8"_W, U"28/RM 8"_W, U"29/RM 8"_W, U"30/RM 8"_W, U"31/RM 8"_W,
    
    //NOTE: RM 9
    U"1/8/RM 9"_W, U"1/6/RM 9"_W, U"1/4/RM 9"_W, U"1/3/RM 9"_W, U"1/2/RM 9"_W, U"1/RM 9"_W, 
    U"2/RM 9"_W, U"3/RM 9"_W, U"4/RM 9"_W, U"5/RM 9"_W, U"6/RM 9"_W, U"7/RM 9"_W,
    U"8/RM 9"_W, U"9/RM 9"_W, U"10/RM 9"_W, U"11/RM 9"_W, U"12/RM 9"_W, U"13/RM 9"_W,
    U"14/RM 9"_W, U"15/RM 9"_W, U"16/RM 9"_W, U"17/RM 9"_W, U"18/RM 9"_W, U"19/RM 9"_W,
    U"20/RM 9"_W, U"21/RM 9"_W, U"22/RM 9"_W, U"23/RM 9"_W, U"24/RM 9"_W, U"25/RM 9"_W,
    U"26/RM 9"_W, U"27/RM 9"_W, U"28/RM 9"_W, U"29/RM 9"_W, U"30/RM 9"_W, U"31/RM 9"_W,
    
    //NOTE: RM 10
    U"1/8/RM 10"_W, U"1/6/RM 10"_W, U"1/4/RM 10"_W, U"1/3/RM 10"_W, U"1/2/RM 10"_W, U"1/RM 10"_W, 
    U"2/RM 10"_W, U"3/RM 10"_W, U"4/RM 10"_W, U"5/RM 10"_W, U"6/RM 10"_W, U"7/RM 10"_W,
    U"8/RM 10"_W, U"9/RM 10"_W, U"10/RM 10"_W, U"11/RM 10"_W, U"12/RM 10"_W, U"13/RM 10"_W,
    U"14/RM 10"_W, U"15/RM 10"_W, U"16/RM 10"_W, U"17/RM 10"_W, U"18/RM 10"_W, U"19/RM 10"_W,
    U"20/RM 10"_W, U"21/RM 10"_W, U"22/RM 10"_W, U"23/RM 10"_W, U"24/RM 10"_W, U"25/RM 10"_W,
    U"26/RM 10"_W, U"27/RM 10"_W, U"28/RM 10"_W, U"29/RM 10"_W, U"30/RM 10"_W, U"31/RM 10"_W,
};

const s32 rmSetCount = 10;
const utf32 rmSet[rmSetCount] = {
    ls_utf32Constant(U"RM 1"), ls_utf32Constant(U"RM 2"), ls_utf32Constant(U"RM 3"), ls_utf32Constant(U"RM 4"),
    ls_utf32Constant(U"RM 5"), ls_utf32Constant(U"RM 6"), ls_utf32Constant(U"RM 7"), ls_utf32Constant(U"RM 8"),
    ls_utf32Constant(U"RM 9"), ls_utf32Constant(U"RM 10")
};

const s32 peSetCount = 36;
const utf32 peSet[peSetCount] =
{
    ls_utf32Constant(U"50"), ls_utf32Constant(U"65"),
    ls_utf32Constant(U"100"), ls_utf32Constant(U"135"),
    ls_utf32Constant(U"200"), ls_utf32Constant(U"400"),
    ls_utf32Constant(U"600"), ls_utf32Constant(U"800"),
    ls_utf32Constant(U"1.200"), ls_utf32Constant(U"1.600"),
    ls_utf32Constant(U"2.400"), ls_utf32Constant(U"3.200"),
    ls_utf32Constant(U"4.800"), ls_utf32Constant(U"6.400"),
    ls_utf32Constant(U"9.600"), ls_utf32Constant(U"12.800"),
    ls_utf32Constant(U"19.200"), ls_utf32Constant(U"25.600"),
    ls_utf32Constant(U"38.400"), ls_utf32Constant(U"51.200"),
    ls_utf32Constant(U"76.800"), ls_utf32Constant(U"102.400"),
    ls_utf32Constant(U"153.600"), ls_utf32Constant(U"204.800"),
    ls_utf32Constant(U"307.200"), ls_utf32Constant(U"409.600"),
    ls_utf32Constant(U"614.400"), ls_utf32Constant(U"819.200"),
    ls_utf32Constant(U"1.228.800"), ls_utf32Constant(U"1.638.400"),
    ls_utf32Constant(U"2.457.600"), ls_utf32Constant(U"3.276.800"),
    ls_utf32Constant(U"4.915.200"), ls_utf32Constant(U"6.553.600"),
    ls_utf32Constant(U"9.830.400"), ls_utf32Constant(U"13.107.200")
};

struct ArchetypeInfo
{
    UIButton   chooseArchetype;
    b32        isChoosingArchetype;
    
    UIButton   archetypes[MAX_ARCHETYPES];
    s32        archetypeIndex = -1;
};

typedef void (*ArchGSProc)(s32, s32 *, s32 *);
typedef void (*ArchASProc)(s32 *);
typedef void (*ArchACProc)(s32 *);
typedef void (*ArchSensesProc)(utf32 *);

//TODO: Temporary. Will need to be moved somewhere more appropriate someday
enum AbilityScoreType
{
    AS_STR,
    AS_DEX,
    AS_CON,
    AS_INT,
    AS_WIS,
    AS_CHA,
    
    AS_COUNT,
};

enum ArmorClassType
{
    AC_ARMOR,
    AC_DODGE,
    AC_NATURAL,
    
    AC_TYPES_COUNT,
};

struct ArchetypeDiff
{
    utf32 nameStr;
    
    ArchGSProc     gs;
    ArchASProc     abilityScores;
    ArchACProc     armorClass;
    ArchSensesProc senses;
};

b32 CompendiumOpenArchetypeWindow(UIContext *c, void *user);
b32 CompendiumSelectArchetype(UIContext *c, void *user);
void CompendiumApplyAllArchetypeNames(utf32 *newName);
void CompendiumApplyAllArchetypeGS(utf32 oldGS, s32 hitDice, utf32 *newGS, utf32 *newPE);
void CompendiumApplyAllArchetypeAS(s32 as[AS_COUNT]);
void CompendiumAddAllArchetypesToList(utf32 *archetypeList);
void CompendiumApplyAllArchetypeSenses(utf32 *old);

#endif //_COMPENDIUM_ARCHETYPES_H
