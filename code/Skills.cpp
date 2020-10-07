const char *DwarfTraitsList[] = { 
    "+2 CON, +2 WIS, -2 CHA",
    "Medium Size",
    "Speed: 6m, but don't get encumbered",
    "Darkvision: 18m Vision in the dark",
    "Defense Training: +4 Dodge to AC against Giants",
    "Greed: +2 on Appraise skill checks on non magical items with gemstones or metals",
    "Hatred: +1 on Attack Rolls against Orc/Goblin humanoids",
    "Hardy: +2 on Saving Throws agains poison, spells and spell-like abilities",
    "Stability: +4 to CMD when resisting falling or being pushed",
    "Stonecunning: +2 on Perception checks to notice unusual stonework. They receive a check when within 3m of them even when not looking",
    "Weapon Familiarity: Battleaxes, Heavy Picks, Warhammers, and any \"dwarven\" weapon.",
    "Languages: Common, Dwarven and can learn : Giant, Gnome, Goblin, Orc, Terran and Undercommon",
};

const char *ElfTraitsList[] = { 
    "+2 DEX, +2 INT, -2 CON",
    "Medium Size",
    "Speed: 9m",
    "Low-Light Vision: Twice as far as humans in dim light",
    "Elven Immunities: Elves are immune to magic sleep, and have +2 Saving Throw against enchantment spells and effects",
    "Elven Magic: +2 on Caster Level checks in spell resistance, +2 on Spellcraft Skill checks to identify magic properties",
    "Keen Sense: +2 on Perception Skill checks",
    "Weapon Familiarity: Proficient in longbows (and compositite), longswords, rapiers, shortbows(and composite), and any \"elven\" weapon",
    "Languages: Common, Elven and can learn : Celestial, Draconic, Gnoll, Gnome, Goblin, Orc and Sylvan",
};

const char *GnomeTraitsList[] = { 
    "+2 CON, +2 CHA, -2 STR",
    "Small Size: +1 AC, +1 Attack Roll, -1 CMB and CMD, +4 on Stealth Checks",
    "Speed: 6m",
    "Low-Light Vision: Twice as far as humans in dim light",
    "Defensive Training: +4 dodge bonus to AC against Giants",
    "Gnome Magic: +1 to DC of Illusion spells. With CHA>=11: 1/day dancing lights, ghost sound, prestidigitation, speak with animals (caster level = gnome level). DC is 10 + level + CHA mod.",
    "Hatred: +1 on Attack Rolls against reptilian and goblin humanoids.",
    "Illusion Resistance: +2 Saving Trhow against Illusions spells and effects",
    "Keen Senses: +2 on Perception skill checks",
    "Obsessive: +2 on a Craft or Profession skill of their choice.",
    "Weapon Familiarity: Any \"gnome\" weapon",
    "Languages: Common, Gnome and can learn : Draconic, Dwarven, Elven, Giant, Goblin, Orc",
};

const char *HalfElfTraitsList[] = { 
    "+2 to One Ability Score of choice",
    "Medium Size",
    "Speed: 9m",
    "Low-Light Vision: Twice as far as humans in dim light",
    "Adaptability: Half-Elves receive Skill Focus as a bonus feat at 1st LVL",
    "Elf Blood: Half-elves count as both elves and humans for any race effect.",
    "Elven Immunities: Immune to magic sleep and +2 saving throw against enchatment spells and effects",
    "Keen Senses: +2 on Perception skill checks",
    "Multitalented: Can choose 2 favored classes at first level",
    "Languages: Common, Elven and can learn any language they want expect secret languages.",
};

const char *HalfOrcTraitsList[] = {
    "+2 to One Ability Score of choice",
    "Medium Size",
    "Speed: 9m",
    "Darkvision: 18m Vision in the dark",
    "Intimidating: +2 on Intimidate skill checks",
    "Orc Blood: Half-orcs count as both orcs and humans for any race effect.",
    "Orc Ferocity: 1/day when Unconscious he can fight for 1 more round.",
    "Weapon Familiarity: Proficient with greataxes, falchions and any \"orc\" weapon",
    "Languages: Common, Orc and can learn: Abyssal, Draconic, Giant, Gnoll and Goblin",
};

const char *HalflingTraitsList[] = { 
    "+2 DEX, +2 CHA, -2 STR",
    "Small Size: +1 AC, +1 Attack Roll, -1 CMB and CMD, +4 on Stealth Checks",
    "Speed: 6m",
    "Fearless: +2 on all saving throws against fear. This stacks with Halfling Luck",
    "Halfling Luck: +2 on all saving throws",
    "Keen Senses: +2 on Perception skill checks",
    "Sure-Footed: +2 on Acrobatics and Climb skill checks",
    "Weapon Familiarity: Proficient with slings and any \"halfling\" weapon",
    "Languages: Common, Halfling and can learn : Dwarven, Elven, Gnome, Goblin",
};

const char *HumanTraitsList[] = {
    "+2 to One Ability Score of choice",
    "Medium Size",
    "Speed: 9m",
    "Bonus Feat: Select 1 extra feat at 1st level",
    "Skilled: Gain additional skill rank at 1st level and one additional rank on level up",
    "Languages: Common and can learn any language except secret ones, like Druidic."
};

const char **RaceTraits[] = 
{ 
    DwarfTraitsList, 
    ElfTraitsList, 
    GnomeTraitsList, 
    HalfElfTraitsList, 
    HalfOrcTraitsList,
    HalflingTraitsList,
    HumanTraitsList
};

u32 RaceTraitsArraySize[] = 
{
    ArraySize(DwarfTraitsList), 
    ArraySize(ElfTraitsList), 
    ArraySize(GnomeTraitsList), 
    ArraySize(HalfElfTraitsList), 
    ArraySize(HalfOrcTraitsList),
    ArraySize(HalflingTraitsList),
    ArraySize(HumanTraitsList)
};