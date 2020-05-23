/* date = May 22nd 2020 11:48 am */

#ifndef _PLAYER_CHAR_H
#define _PLAYER_CHAR_H

#define AS_BONUS_NUM     46
#define AS_BONUS_STR_NUM 47

s32 ASBonusTable[AS_BONUS_NUM]
{
    -5, -5, -4, -4, -3, -3, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2,
    3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
    12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17
};

char *ASBonusTableString[AS_BONUS_STR_NUM]
{
    "-5", "-5", "-4", "-4", "-3", "-3", "-2", "-2", "-1", "-1", "0", "0", "+1", "+1",
    "+2", "+2", "+3", "+3", "+4", "+4", "+5", "+5", "+6", "+6", "+7", "+7", "+8", "+8",
    "+9", "+9", "+10", "+10", "+11", "+11", "+12", "+12", "+13", "+13", "+14","+14",
    "+15", "+15", "+16", "+16", "+17", "+17", "-999"
};

enum XPCurveIdx : u8
{
    XP_FAST,
    XP_MEDIUM,
    XP_SLOW
};

char *FastXPCurve[] =
{
    "0", "1300", "3300", "6000", "10000", "15000", "23000", "34000", "50000", "71000",
    "105000", "145000", "210000", "295000", "425000", "600000", "850000", "1200000",
    "1700000", "2400000", "0"
};

char *MediumXPCurve[] =
{
    "0", "2000", "5000", "9000", "15000", "23000", "35000", "51000", "75000", "105000",
    "155000", "220000", "315000", "445000", "635000", "890000", "1300000", "1800000",
    "2550000", "3600000", "0"
};

char *SlowXPCurve[] =
{
    "0", "3000", "75000", "14000", "23000", "35000", "53000", "77000", "115000", "160000",
    "235000", "330000", "475000", "665000", "955000", "1350000", "1900000", "2700000",
    "3850000", "5350000", "0"
};

char *XPCurvesString[] =
{
    "Fast",
    "Medium",
    "Slow"
};

char **XPCurvesArr[] =
{
    FastXPCurve,
    MediumXPCurve,
    SlowXPCurve
};

char *AbilityGenMethods[] =
{
    "Classic",
    "Dice Pool",
    "Heroic",
    "Purchase",
    "Standard"
};

enum GenMethod : u8
{
    GEN_INVALID,
    
    GEN_STANDARD,
    GEN_CLASSIC,
    GEN_HEROIC,
    GEN_DICE_POOL,
    GEN_PURCHASE,
    
    GEN_COUNT
};

enum AbilityScore : u8
{
    ABILITY_INVALID = 127,
    
    ABILITY_STR = 0,
    ABILITY_DEX,
    ABILITY_CON,
    ABILITY_INT,
    ABILITY_WIS,
    ABILITY_CHA,
    
    ABILITY_COUNT
};

char *AS_string[6] = { "STR", "DEX", "CON", "INT", "WIS", "CHA" };

char *Races[] =
{
    "Human", "Elf", "Half-Orc", "Half-Elf",
    "Gnome", "Halfling", "Dwarf"
};

enum GameRace : u8
{
    RACE_INVALID = 99,
    
    RACE_DWARF = 0,
    RACE_ELF,
    RACE_GNOME,
    RACE_HALF_ELF,
    RACE_HALF_ORC,
    RACE_HALFLING,
    RACE_HUMAN,
    
    RACE_COUNT
};

struct PlayerChar
{
    string    Name;
    string    Player;
    GameRace  Race;
    GameClass Class;
    
    //NOTE: I'm not really sure this should be here...
    GenMethod GenMethod;
    
    u8 AbilityScores[6];
    
    u8 lvl;
    u32 xp;
    XPCurveIdx xpCurve;
};


#endif //_PLAYER_CHAR_H
