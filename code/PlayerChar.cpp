#include "PlayerChar.h"

const char *getASBonusStr(u8 v)
{
    if(v < 0 || v >= AS_BONUS_NUM) { return ASBonusTableString[AS_BONUS_STR_NUM - 1]; }
    return ASBonusTableString[v];
}

s32 getASBonusVal(s32 v)
{
    if(v < 0 || v >= AS_BONUS_NUM) { return 0; }
    return ASBonusTable[v];
}

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
    buffer buff = ls_bufferInit(KB(1));
    
    //SERIALIZE NAME
    ls_bufferAddString(&buff, pc->Name);
    
    //SERIALIZE PLAYER
    ls_bufferAddString(&buff, pc->Player);
    
    //SERIALIZE RACE
    ls_bufferAddByte(&buff, pc->Race);
    
    //SERIALIZE CLASS
    ls_bufferAddByte(&buff, pc->Class);
    
    //SERIALIZE ABILITY SCORES
    ls_bufferAddByte(&buff, pc->AbilityScores[ABILITY_STR]);
    ls_bufferAddByte(&buff, pc->AbilityScores[ABILITY_DEX]);
    ls_bufferAddByte(&buff, pc->AbilityScores[ABILITY_CON]);
    ls_bufferAddByte(&buff, pc->AbilityScores[ABILITY_INT]);
    ls_bufferAddByte(&buff, pc->AbilityScores[ABILITY_WIS]);
    ls_bufferAddByte(&buff, pc->AbilityScores[ABILITY_CHA]);
    
    //SERIALIZE LVL, XP, XP CURVE
    ls_bufferAddByte(&buff, pc->lvl);
    ls_bufferAddDWord(&buff, pc->xp);
    ls_bufferAddByte(&buff, pc->xpCurve);
    
    ls_writeFile((char *)"test.pcm", buff.data, buff.cursor, FALSE);
}

void LoadPC(PlayerChar *pc)
{
    buffer buff = ls_bufferInit(KB(1));
    
    buff.size = ls_readFile((char *)"F:\\ProgrammingProjects\\Lowy_No_VS\\PCMan\\test.pcm", (char **)(&buff.data), 0);
    buff.cursor = 0;
    
    pc->Name      = ls_bufferReadString(&buff);
    pc->Player    = ls_bufferReadString(&buff);
    
    u8 race  = ls_bufferReadByte(&buff);
    u8 c     = ls_bufferReadByte(&buff);
    
    pc->Race = RACE_HALF_ELF;
    pc->Class = CLASS_DRUID;
    
    pc->AbilityScores[ABILITY_STR] = ls_bufferReadByte(&buff);
    pc->AbilityScores[ABILITY_DEX] = ls_bufferReadByte(&buff);
    pc->AbilityScores[ABILITY_CON] = ls_bufferReadByte(&buff);
    pc->AbilityScores[ABILITY_INT] = ls_bufferReadByte(&buff);
    pc->AbilityScores[ABILITY_WIS] = ls_bufferReadByte(&buff);
    pc->AbilityScores[ABILITY_CHA] = ls_bufferReadByte(&buff);
    
    pc->lvl     = ls_bufferReadByte(&buff);
    pc->xp      = ls_bufferReadDWord(&buff);
    pc->xpCurve = (XPCurveIdx)ls_bufferReadByte(&buff);;
}