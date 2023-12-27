#ifndef _COMPENDIUM_ARCHETYPES_H
#define _COMPENDIUM_ARCHETYPES_H

const s32 MAX_ARCHETYPES = 4;

const char32_t *archetypeName[MAX_ARCHETYPES] = {
    U"Avanzato", U"Celestiale", U"Immondo", U"Gigante"
};

struct ArchetypeInfo
{
    UIButton   chooseArchetype;
    b32        isChoosingArchetype;
    
    UIButton   archetypes[MAX_ARCHETYPES];
    s32        archetypeIndex = -1;
};


typedef void (*ArchGSProc)(s32 *gs);
typedef void (*ArchASProc)(s32 *as);
typedef void (*ArchACProc)(s32 *ac);

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
    ArchGSProc gs;
    ArchASProc abilityScore;
    ArchACProc armorClass;
};

b32 CompendiumOpenArchetypeWindow(UIContext *c, void *user);
b32 CompendiumSelectArchetype(UIContext *c, void *user);

#endif //_COMPENDIUM_ARCHETYPES_H
