#ifndef _COMPENDIUM_ARCHETYPES_H
#define _COMPENDIUM_ARCHETYPES_H

//TODO: @ArchOptionalPrompt
//        Add an extra optional prompt for customization when choosing an archetype
//        For example, fleshwarped makes you choose a new movement speed among 4 different kinds.

const s32 MAX_CONCURRENT_ARCHETYPES = 4;
const s32 MAX_ARCHETYPES = 18;

const s32 RACE_SUBTYPES_COUNT = 61;
const utf32 raceSubtypes[RACE_SUBTYPES_COUNT] = {
    U"Agathion"_W, U"Androide"_W, U"Angelo"_W, U"Arconte"_W, U"Asura"_W, U"Azata"_W, U"Behemoth"_W, U"Boggard"_W,
    U"Caccia Selvaggia"_W, U"Calamità"_W, U"Changeling"_W, U"Daemon"_W, U"Derro"_W, U"Dhampir"_W, U"Demodand"_W,
    U"Demone"_W, U"Diavolo"_W, U"Div"_W, U"Elementale"_W, U"Elfo"_W, U"Eone"_W, U"Felinide"_W, U"Gigante"_W,
    U"Gnoll"_W, U"Gnomo"_W, U"Goblinoide"_W, U"Grande Antico"_W, U"Grigio"_W, U"Grippli"_W, U"Halfling"_W,
    U"Inevitabile"_W, U"Kaiju"_W, U"Kami"_W, U"Kasatha"_W, U"Kitsune"_W, U"Kyton"_W, U"Leshy"_W, U"Manasaputra"_W,
    U"Meccanico"_W, U"Nano"_W, U"Ombra Notturna"_W, U"Oni"_W, U"Orco"_W, U"Oscuro"_W, U"Psicopompo"_W, U"Protean"_W,
    U"Qlippoth"_W, U"Rakshasa"_W, U"Rattoide"_W, U"Rettile"_W, U"Robot"_W, U"Samsaran"_W, U"Skulk"_W, U"Strix"_W,
    U"Tengu"_W, U"Udeo"_W, U"Umano"_W, U"Vanara"_W, U"Vishkanya"_W, U"Wayang"_W, U"Yenaldooshi"_W,
};

const s32 ALIGNMENT_SUBTYPE_COUNT = 4;
const utf32 alignmentSubtypes[ALIGNMENT_SUBTYPE_COUNT] = {
    U"Buono"_W, U"Caotico"_W, U"Legale"_W, U"Malvagio"_W,
};

const char32_t *archetypeName[] = {
    U"Avanzato", U"Celestiale", U"Immondo", U"Entropico", U"Risoluto", 
    U"Gigante", U"Fungoide", U"d'Ombra", U"Carnideformato", U"Degenerato",
    
    U"Scheletro", U"Scheletro Insanguinato", U"Scheletro Bruciante",
    
    U"Zombi", U"Zombi Rapido",
    
    U"Vampiro",
    
    U"Micotico", U"Esoscheletro",
};

static_assert((sizeof(archetypeName) / sizeof(char32_t *)) == MAX_ARCHETYPES,
              "ArchetypeName is not exhaustive\n");

struct ArchetypeInfo
{
    UIButton   chooseArchetype;
    b32        isChoosingArchetype;
    
    UIButton   archetypes[MAX_ARCHETYPES];
    s32        archetypeIndex = -1;
};

typedef b32  (*ArchIsCompatible)(CachedPageEntry *);
typedef b32  (*ArchGSProc)(s32, s32 *, s32 *);
typedef void (*ArchASProc)(s32 *);
typedef b32  (*ArchACProc)(utf32 *, s32 *);
typedef void (*ArchSensesProc)(utf32 *);
typedef void (*ArchRDProc)(s32, utf32 *);
typedef void (*ArchResProc)(s32, u64, utf32 *);
typedef void (*ArchRIProc)(utf32, utf32 *);
typedef b32  (*ArchSpecAtkProc)(utf32 *);
typedef void (*ArchSizeProc)(utf32 *);
typedef void (*ArchMeleeProc)(CachedPageEntry*, utf32 *);
typedef b32  (*ArchAlignProc)(utf32 *);
typedef void (*ArchTypeProc)(utf32 *);
typedef void (*ArchSubTypeProc)(utf32 *);
typedef void (*ArchDVProc)(CachedPageEntry *, utf32 *, u64 *);
typedef b32  (*ArchSTProc)(s32, s32 *);
typedef void (*ArchDefCapProc)(utf32 *);
typedef void (*ArchSpeedProc)(utf32 *);
typedef void (*ArchImmProc)(utf32 *);
typedef void (*ArchBABProc)(utf32 *, s32);
typedef u32  (*ArchSkillsProc)(u32 skillEntry);
typedef void (*ArchTalentsProc)(CachedPageEntry *);
typedef void (*ArchEnvProc)(utf32 *);
typedef void (*ArchOrgProc)(utf32 *);
typedef void (*ArchTreasureProc)(utf32 *);
typedef void (*ArchSpecQualProc)(utf32 *);
typedef void (*ArchSpecCapProc)(utf32 *);
typedef void (*ArchLangProc)(utf32 *);
typedef void (*ArchAuraProc)(utf32 *);
typedef void (*ArchWeakProc)(utf32 *);

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

//TODO: Temporary. Will need to be moved somewhere more appropriate someday
enum SavingThrowType
{
    ST_CON,
    ST_DEX,
    ST_WIS,
    
    ST_COUNT,
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
    
    //TODONOTE: Should be normalized now. So this should not be necessary anymore
    U"Piccolissimo"_W, U"Minuto"_W, U"Minuscolo"_W, U"Piccolo"_W, U"Medio"_W,
    U"Grande"_W, U"Enorme"_W, U"Mastodontico"_W, U"Colossale"_W,
};


struct ArchetypeDiff
{
    utf32 nameStr;
    
    ArchIsCompatible isCompatible;
    
    ArchGSProc       gs;
    ArchASProc       abilityScores;
    ArchACProc       armorClass;
    ArchSensesProc   senses;
    ArchRDProc       rd;
    ArchResProc      resistances;
    ArchRIProc       ri;
    ArchSpecAtkProc  specialAtk;
    ArchSizeProc     size;
    ArchMeleeProc    melee;
    ArchAlignProc    align;
    ArchTypeProc     type;
    ArchSubTypeProc  subtype;
    ArchDVProc       DV;
    ArchSTProc       st;
    ArchDefCapProc   defCap;
    ArchSpeedProc    speed;
    ArchImmProc      immunities;
    ArchBABProc      bab;
    ArchSkillsProc   skills;
    ArchTalentsProc  talents;
    ArchEnvProc      env;
    ArchOrgProc      org;
    ArchTreasureProc treasure;
    ArchSpecQualProc specialQual;
    ArchSpecCapProc  specialCap;
    ArchLangProc     lang;
    ArchAuraProc     aura;
    ArchWeakProc     weak;
};

b32  CompendiumOpenArchetypeWindow(UIContext *c, void *user);
b32  CompendiumSelectArchetype(UIContext *c, void *user);


void CompendiumApplyAllArchetypeNames(utf32 *newName);
void CompendiumApplyAllArchetypeGS(u16 gsEntry, s32 hitDice, utf32 *newGS, utf32 *newPE);
void CompendiumApplyAllArchetypeAS(s32 as[AS_COUNT]);
b32  CompendiumApplyAllArchetypeAC(utf32 *size, s32 acDiff[AC_TYPES_COUNT]);
void CompendiumAddAllArchetypesToList(utf32 *archetypeList);
void CompendiumApplyAllArchetypeSenses(utf32 *old);
void CompendiumApplyAllArchetypeRD(s32 hitDice, utf32 *old);
void CompendiumApplyAllArchetypeResistances(s32 hitDice, u64 orig, utf32 *old);
void CompendiumApplyAllArchetypeRI(utf32 gs, utf32 *ri);
void CompendiumApplyAllArchetypeSpecAtk(utf32 *spec);
void CompendiumApplyAllArchetypeSize(utf32 *size);
void CompendiumApplyAllArchetypeMelee(CachedPageEntry *page, utf32 *melee);
void CompendiumApplyAllArchetypeAlign(utf32 *align);
void CompendiumApplyAllArchetypeTypes(utf32 *type);
void CompendiumApplyAllArchetypeSubTypes(utf32 *subtype);
void CompendiumApplyAllArchetypeDV(CachedPageEntry *page, utf32 *oldType, u64 *DV);
b32  CompendiumApplyAllArchetypeST(s32 DV, s32 st[ST_COUNT]);
void CompendiumApplyAllArchetypeDefCap(utf32 *old);
void CompendiumApplyAllArchetypeSpeed(utf32 *old);
void CompendiumApplyAllArchetypeImmunities(utf32 *old);
void CompendiumApplyAllArchetypeBAB(utf32 *old, s32 dv);
u32  CompendiumApplyAllArchetypeSkills(u32 skillEntry);
void CompendiumApplyAllArchetypeTalents(CachedPageEntry *);
void CompendiumApplyAllArchetypeEnv(utf32 *old);
void CompendiumApplyAllArchetypeOrg(utf32 *old);
void CompendiumApplyAllArchetypeTreasure(utf32 *old);
void CompendiumApplyAllArchetypeSpecQual(utf32 *old);
void CompendiumApplyAllArchetypeSpecCap(utf32 *old);
void CompendiumApplyAllArchetypeLang(utf32 *old);
void CompendiumApplyAllArchetypeAura(utf32 *old);
void CompendiumApplyAllArchetypeWeak(utf32 *old);

#endif //_COMPENDIUM_ARCHETYPES_H
