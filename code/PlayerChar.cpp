char *ASBonusTable[]
{
    "-5", "-5", "-4", "-4", "-3", "-3", "-2", "-2", "-1", "-1", "0", "0", "+1", "+1", 
    "+2", "+2", "+3", "+3", "+4", "+4", "+5", "+5", "+6", "+6", "+7", "+7", "+8", "+8",
    "+9", "+9", "+10", "+10", "+11", "+11", "+12", "+12", "+13", "+13", "+14","+14",
    "+15", "+15", "+16", "+16", "+17", "+17"
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

GameRace getRaceFromString(string s)
{
    if(s == "Dwarf")
    { return RACE_DWARF; }
    else if(s == "Elf")
    { return RACE_ELF; }
    else if(s == "Gnome")
    { return RACE_GNOME; }
    else if(s == "Half-Elf")
    { return RACE_HALF_ELF; }
    else if(s == "Half-Orc")
    { return RACE_HALF_ORC; }
    else if(s == "Halfling")
    { return RACE_HALFLING; }
    else if(s == "Human")
    { return RACE_HUMAN; }
    
    Assert(FALSE);
    
    return RACE_INVALID;
}

GameClass getClassFromString(string s)
{
    if(s == "Barbarian")     { return CLASS_BARBARIAN; }
    else if(s == "Bard")     { return CLASS_BARD; }
    else if(s == "Cleric")   { return CLASS_CLERIC; }
    else if(s == "Druid")    { return CLASS_DRUID; }
    else if(s == "Fighter")  { return CLASS_FIGHTER; }
    else if(s == "Monk")     { return CLASS_MONK; }
    else if(s == "Paladin")  { return CLASS_PALADIN; }
    else if(s == "Ranger")   { return CLASS_RANGER; }
    else if(s == "Rogue")    { return CLASS_ROGUE; }
    else if(s == "Sorcerer") { return CLASS_SORCERER; }
    else if(s == "Wizard")   { return CLASS_WIZARD; }
    
    Assert(FALSE);
    
    return CLASS_INVALID;
}

GenMethod getGenMethodFromString(string s)
{
    if(s == "Standard")       { return GEN_STANDARD; }
    else if(s == "Classic")   { return GEN_CLASSIC; }
    else if(s == "Heroic")    { return GEN_HEROIC; }
    else if(s == "Dice Pool") { return GEN_DICE_POOL; }
    else if(s == "Purchase")  { return GEN_PURCHASE; }
    
    Assert(FALSE);
    
    return GEN_INVALID;
}

void SerializePC(PlayerChar *pc)
{
    dataBuffer buff;
    
    //SERIALIZE NAME
    buff += (string)pc->Name;
    
    //SERIALIZE PLAYER
    buff += (string)pc->Player;
    
    //SERIALIZE RACE
    buff += (u8)pc->Race;
    
    //SERIALIZE CLASS
    buff += (u8)pc->Class;
    
    //SERIALIZE GEN METHOD
    buff += (u8)pc->GenMethod;
    
    //SERIALIZE ABILITY SCORES
    buff += (u8)pc->AbilityScores[ABILITY_STR];
    buff += (u8)pc->AbilityScores[ABILITY_DEX];
    buff += (u8)pc->AbilityScores[ABILITY_CON];
    buff += (u8)pc->AbilityScores[ABILITY_INT];
    buff += (u8)pc->AbilityScores[ABILITY_WIS];
    buff += (u8)pc->AbilityScores[ABILITY_CHA];
    
    //SERIALIZE LVL, XP, XP CURVE
    buff += (u8)pc->lvl;
    buff += (u32)pc->xp;
    buff += (u8)pc->xpCurve;
    
    string Path = ls_strInit("test.pcm");
    ls_writeFile(&Path, buff.data, buff.used, FALSE);
}

void LoadPC(PlayerChar *pc)
{
    dataBuffer buff = 0;
    
    string Path = ls_strInit("F:\\ProgrammingProjects\\Lowy_No_VS\\PCMan\\test.pcm");
    buff.size = ls_readFile(&Path, (char **)(&buff.data), 0);
    buff.used = buff.size;
    
    buff.readPos = 0;
    
    pc->Name      = buff.readString();
    pc->Player    = buff.readString();
    
    u8 race  = buff.readU8();
    u8 c    = buff.readU8(); 
    u8 g    = buff.readU8();
    
    
    pc->Race = RACE_HALF_ELF;
    pc->Class = CLASS_DRUID;
    pc->GenMethod = GEN_CLASSIC; 
    
    pc->AbilityScores[ABILITY_STR] = buff.readU8();
    pc->AbilityScores[ABILITY_DEX] = buff.readU8();
    pc->AbilityScores[ABILITY_CON] = buff.readU8();
    pc->AbilityScores[ABILITY_INT] = buff.readU8();
    pc->AbilityScores[ABILITY_WIS] = buff.readU8();
    pc->AbilityScores[ABILITY_CHA] = buff.readU8();
    
    pc->lvl     = buff.readU8();
    pc->xp      = buff.readU32();
    pc->xpCurve = (XPCurveIdx)buff.readU8();
}