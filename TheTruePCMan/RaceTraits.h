//
//  RaceTraits.m
//  TheTruePCMan
//
//  Created by Lowy on 10/10/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Foundation/Foundation.h>

enum GameRace
{
    RACE_DWARF = 0,
    RACE_ELF,
    RACE_GNOME,
    RACE_HALF_ELF,
    RACE_HALF_ORC,
    RACE_HALFLING,
    RACE_HUMAN,
    
    RACE_COUNT
};

//Test

NSString *DwarfTraits = @"+2 CON, +2 WIS, -2 CHA\r\n"
"Medium Size\r\n"
"Speed: 6m, but don't get encumbered\r\n"
"Darkvision: 18m Vision in the dark\r\n"
"Defense Training: +4 Dodge to AC against Giants\r\n"
"Greed: +2 on Appraise skill checks on non magical items with gemstones or metals\r\n"
"Hatred: +1 on Attack Rolls against Orc/Goblin humanoids\r\n"
"Hardy: +2 on Saving Throws agains poison, spells and spell-like abilities\r\n"
"Stability: +4 to CMD when resisting falling or being pushed\r\n"
"Stonecunning: +2 on Perception checks to notice unusual stonework. They receive a check when within 3m of them even when not looking\r\n"
"Weapon Familiarity: Battleaxes, Heavy Picks, Warhammers, and any \"dwarven\" weapon.\r\n"
"Languages: Common, Dwarven and can learn : Giant, Gnome, Goblin, Orc, Terran and Undercommon\r\n";

NSString *ElfTraits = @"+2 DEX, +2 INT, -2 CON\r\n"
    "Medium Size\r\n"
    "Speed: 9m\r\n"
    "Low-Light Vision: Twice as far as humans in dim light\r\n"
    "Elven Immunities: Elves are immune to magic sleep, and have +2 Saving Throw against enchantment spells and effects\r\n"
    "Elven Magic: +2 on Caster Level checks in spell resistance, +2 on Spellcraft Skill checks to identify magic properties\r\n"
    "Keen Sense: +2 on Perception Skill checks\r\n"
    "Weapon Familiarity: Proficient in longbows (and compositite), longswords, rapiers, shortbows(and composite), and any \"elven\" weapon\r\n"
    "Languages: Common, Elven and can learn : Celestial, Draconic, Gnoll, Gnome, Goblin, Orc and Sylvan\r\n";

NSString *GnomeTraits = @"+2 CON, +2 CHA, -2 STR\r\n"
    "Small Size: +1 AC, +1 Attack Roll, -1 CMB and CMD, +4 on Stealth Checks\r\n"
    "Speed: 6m\r\n"
    "Low-Light Vision: Twice as far as humans in dim light\r\n"
    "Defensive Training: +4 dodge bonus to AC against Giants\r\n"
    "Gnome Magic: +1 to DC of Illusion spells. With CHA>=11: 1/day dancing lights, ghost sound, prestidigitation, speak with animals (caster level = gnome level). DC is 10 + level + CHA mod.\r\n"
    "Hatred: +1 on Attack Rolls against reptilian and goblin humanoids.\r\n"
    "Illusion Resistance: +2 Saving Trhow against Illusions spells and effects\r\n"
    "Keen Senses: +2 on Perception skill checks\r\n"
    "Obsessive: +2 on a Craft or Profession skill of their choice.\r\n"
    "Weapon Familiarity: Any \"gnome\" weapon\r\n"
    "Languages: Common, Gnome and can learn : Draconic, Dwarven, Elven, Giant, Goblin, Orc\r\n";

NSString *HalfElfTraits = @"+2 to One Ability Score of choice\r\n"
    "Medium Size\r\n"
    "Speed: 9m\r\n"
    "Low-Light Vision: Twice as far as humans in dim light\r\n"
    "Adaptability: Half-Elves receive Skill Focus as a bonus feat at 1st LVL\r\n"
    "Elf Blood: Half-elves count as both elves and humans for any race effect.\r\n"
    "Elven Immunities: Immune to magic sleep and +2 saving throw against enchatment spells and effects\r\n"
    "Keen Senses: +2 on Perception skill checks\r\n"
    "Multitalented: Can choose 2 favored classes at first level\r\n"
    "Languages: Common, Elven and can learn any language they want expect secret languages.\r\n";

NSString *HalfOrcTraits = @"+2 to One Ability Score of choice\r\n"
    "Medium Size\r\n"
    "Speed: 9m\r\n"
    "Darkvision: 18m Vision in the dark\r\n"
    "Intimidating: +2 on Intimidate skill checks\r\n"
    "Orc Blood: Half-orcs count as both orcs and humans for any race effect.\r\n"
    "Orc Ferocity: 1/day when Unconscious he can fight for 1 more round.\r\n"
    "Weapon Familiarity: Proficient with greataxes, falchions and any \"orc\" weapon\r\n"
    "Languages: Common, Orc and can learn: Abyssal, Draconic, Giant, Gnoll and Goblin\r\n";

NSString *HalflingTraits = @"+2 DEX, +2 CHA, -2 STR\r\n"
    "Small Size: +1 AC, +1 Attack Roll, -1 CMB and CMD, +4 on Stealth Checks\r\n"
    "Speed: 6m\r\n"
    "Fearless: +2 on all saving throws against fear. This stacks with Halfling Luck\r\n"
    "Halfling Luck: +2 on all saving throws\r\n"
    "Keen Senses: +2 on Perception skill checks\r\n"
    "Sure-Footed: +2 on Acrobatics and Climb skill checks\r\n"
    "Weapon Familiarity: Proficient with slings and any \"halfling\" weapon\r\n"
    "Languages: Common, Halfling and can learn : Dwarven, Elven, Gnome, Goblin\r\n";

NSString *HumanTraits = @"+2 to One Ability Score of choice\r\n"
    "Medium Size\r\n"
    "Speed: 9m\r\n"
    "Bonus Feat: Select 1 extra feat at 1st level\r\n"
    "Skilled: Gain additional skill rank at 1st level and one additional rank on level up\r\n"
    "Languages: Common and can learn any language except secret ones, like Druidic";

