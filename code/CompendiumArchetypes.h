#ifndef _COMPENDIUM_ARCHETYPES_H
#define _COMPENDIUM_ARCHETYPES_H

const s32 MAX_CONCURRENT_ARCHETYPES = 4;
const s32 MAX_ARCHETYPES = 5;

const char32_t *archetypeName[MAX_ARCHETYPES] = {
    U"Avanzato", U"Celestiale", U"Immondo", U"Gigante", U"Scheletro"
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
typedef void (*ArchRDProc)(s32, utf32 *);
typedef void (*ArchResProc)(s32, u64, utf32 *);
typedef void (*ArchRIProc)(utf32, utf32 *);
typedef void (*ArchSpecAtkProc)(utf32 *);
typedef void (*ArchSizeProc)(utf32 *);
typedef void (*ArchMeleeProc)(utf32 *);

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

//TODO: Dodge and other AC types
enum ArmorClassType
{
    AC_ARMOR,
    AC_DODGE,
    AC_NATURAL,
    
    AC_TYPES_COUNT,
};

enum SizeType
{
    SIZE_FINE,
    SIZE_DIMINUTIVE,
    SIZE_TINY,
    SIZE_SMALL,
    SIZE_MEDIUM,
    SIZE_LARGE,
    SIZE_HUGE,
    SIZE_GARGANTUAN,
    SIZE_COLOSSAL,
    
    SIZE_COUNT,
};

//NOTETODO: Fucking golarion is not consistent. When Size is int-mapped it will not be a problem...
const utf32 sizeSet[SIZE_COUNT*2] =
{
    U"Piccolissima"_W, U"Minuta"_W, U"Minuscola"_W, U"Piccola"_W, U"Media"_W,
    U"Grande"_W, U"Enorme"_W, U"Mastodontica"_W, U"Colossale"_W,
    
    U"Piccolissimo"_W, U"Minuto"_W, U"Minuscolo"_W, U"Piccolo"_W, U"Medio"_W,
    U"Grande"_W, U"Enorme"_W, U"Mastodontico"_W, U"Colossale"_W,
};


struct ArchetypeDiff
{
    utf32 nameStr;
    
    ArchGSProc      gs;
    ArchASProc      abilityScores;
    ArchACProc      armorClass;
    ArchSensesProc  senses;
    ArchRDProc      rd;
    ArchResProc     resistances;
    ArchRIProc      ri;
    ArchSpecAtkProc specialAtk;
    ArchSizeProc    size;
    ArchMeleeProc   melee;
};

b32 CompendiumOpenArchetypeWindow(UIContext *c, void *user);
b32 CompendiumSelectArchetype(UIContext *c, void *user);
void CompendiumApplyAllArchetypeNames(utf32 *newName);
void CompendiumApplyAllArchetypeGS(u16 gsEntry, s32 hitDice, utf32 *newGS, utf32 *newPE);
void CompendiumApplyAllArchetypeAS(s32 as[AS_COUNT]);
void CompendiumApplyAllArchetypeAC(s32 acDiff[AC_TYPES_COUNT]);
void CompendiumAddAllArchetypesToList(utf32 *archetypeList);
void CompendiumApplyAllArchetypeSenses(utf32 *old);
void CompendiumApplyAllArchetypeRD(s32 hitDice, utf32 *old);
void CompendiumApplyAllArchetypeResistances(s32 hitDice, u64 orig, utf32 *old);
void CompendiumApplyAllArchetypeRI(utf32 gs, utf32 *ri);
void CompendiumApplyAllArchetypeSpecAtk(utf32 *spec);
void CompendiumApplyAllArchetypeSize(utf32 *size);
void CompendiumApplyAllArchetypeMelee(utf32 *melee);

#endif //_COMPENDIUM_ARCHETYPES_H
