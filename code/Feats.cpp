//TODO: Add Tables to Feats Descriptions.

const char *FeatNames[] =
{
    "Acrobatic",
    "Acrobatic Steps",
    "Agile Maneuvers (Combat)",
    "Alertness",
    "Alignment Channel",
    "Animal Affinity",
    "Arcane Armor Mastery (Combat)",
    "Aracan Armor Training (Combat)",
    "Arcane Strike (Combat)",
    "Armor Proficiency, Heavy (Combat)",
    "Armor Proficiency, Light (Combat)",
    "Armor Proficiency, Medium (Combat)",
    "Athletic",
    "Augment Summoning",
    
    "Bleeding Critical (Combat, Critical)",
    "Blind-Fight (Combat)",
    "Blinding Critical (Combat, Critical)",
    "Brew Potion (Item Creation)",
    
    "Catch Off-Guard (Combat)",
    "Channel Smite (Combat)",
    "Cleave (Combat)",
    "Combat Casting",
    "Combat Expertise (Combat)",
    "Combat Reflexes (Combat)",
    "Command Undead",
    "Craft Magic Arms and Armor (Item Creation)",
    "Craft Rod (Item Creation)",
    "Craft Staff (Item Creation)",
    "Craft Wand (Item Creation)",
    "Craft Wondrous Item (Item Creation)",
    "Critical Focus (Combat)",
    "Critical Mastery (Combat)",
    
    "Dazzling Display (Combat)",
    "Deadly Aim (Combat)",
    "Deadly Stroke (Combat)",
    "Deafening Critical (Combat, Critical)",
    "Deceitful",
    "Defensive Combat Training (Combat)",
    "Deflect Arrows (Combat)",
    "Deft Hands",
    "Diehard",
    "Disruptive (Combat)",
    "Dodge (Combat)",
    "Double Slice (Combat)",
    
    "Elemental Channel",
    "Empower Spell (Metamagic)",
    "Endurance",
    "Enlarge Spell (Metamagic)",
    "Eschew Materials",
    "Exhausting Critical (Combat, Critical)",
    "Exotic Weapon Proficiency (Combat)",
    "Extend Spell (Metamagic)",
    "Extra Channel",
    "Extra Ki",
    "Extra Lay On Hands",
    "Extra Mercy",
    "Extra Performance",
    "Extra Rage",
    
    "Far Shot (Combat)",
    "Fleet",
    "Forge Ring (Item Creation)",
    
    "Gorgon's Fist (Combat)",
    "Great Cleave (Combat)",
    "Great Fortitude",
    "Greater Bull Rush (Combat)",
    "Greater Disarm (Combat)",
    "Greater Feint (Combat)",
    "Greater Grapple (Combat)",
    "Greater Overrun (Combat)",
    "Greater Penetrating Strike (Combat)",
    "Greater Shield Focus (Combat)",
    "Greater Spell Focus",
    "Greater Spell Penetration",
    "Greater Sunder (Combat)",
    "Greater Trip (Combat)",
    "Greater Two-Weapon Fighting (Combat)",
    "Greater Vital Strike (Combat)",
    "Greater Weapon Focus (Combat)",
    "Greater Weapon Specialization (Combat)",
    
    "Heighten Spell (Metamagic)",
    
    "Improved Bull Rush (Combat)",
    "Improved Channel",
    "Improved Counterspell",
    "Improved Critical (Combat)",
    "Improved Disarm (Combat)",
    "Improved Familiar",
    "Improved Feint (Combat)",
    "Improved Grapple (Combat)",
    "Improved Great Fortitude",
    "Improved Initiative (Combat)",
    "Improved Iron Will",
    "Improved Lightning Reflexes",
    "Improved Overrun (Combat)",
    "Improved Precise Shot (Combat)",
    "Improved Shield Bash (Combat)",
    "Improved Sunder (Combat)",
    "Improved Trip (Combat)",
    "Improved Two-Weapon Fighting (Combat)",
    "Improved Unarmed Strike (Combat)",
    "Improved Vital Strike (Combat)",
    "Improved Weapon Mastery (Combat)",
    "Intimidating Prowess (Combat)",
    "Iron Will",
    
    "Leadership",
    "Lightning Reflexes",
    "Lightning Stance (Combat)",
    "Lunge (Combat)",
    
    "Magical Aptitude",
    "Manyshot (Combat)",
    "Martial Weapon Proficiency (Combat)",
    "Master Craftsman",
    "Maximize Spell (Metamagic)",
    "Medusa's Wrath (Combat)",
    "Mobility (Combat)",
    "Mounted Archery (Combat)",
    "Mounted Combat (Combat)",
    
    "Natural Spell",
    "Nimble Moves",
    
    "Penetrating Strike (Combat)",
    "Persuasive",
    "Pinpoint Targeting (Combat)",
    "Point-Blank Shot (Combat)",
    "Power Attack (Combat)",
    "Precise Shot (Combat)",
    
    "Quick Draw (Combat)",
    "Quicken Spell (Metamagic)",
    
    "Rapid Reload (Combat)",
    "Rapid Shot (Combat)",
    "Ride-By Attack (Combat)",
    "Run",
    
    "Scorpion Style (Combat)",
    "Scribe Scroll (Item Creation)",
    "Selective Channeling",
    "Self Sufficient",
    "Shatter Defenses (Combat)",
    "Shield Focus (Combat)",
    "Shield Master (Combat)",
    "Shield Proficiency (Combat)",
    "Shield Slam (Combat)",
    "Shot on the Run (Combat)",
    "Sickening Critical (Combat, Critical)",
    "Silent Spell (Metamagic)",
    "Simple Weapon Proficiency (Combat)",
    "Skill Focus",
    "Snatch Arrows (Combat)",
    "Spell Focus",
    "Spell Mastery",
    "Spell Penetration",
    "Spellbreaker (Combat)",
    "Spirited Charge (Combat)",
    "Spring Attack (Combat)",
    "Staggering Critical (Combat, Critical)",
    "Stand Skill (Combat)",
    "Stealthy",
    "Step Up (Combat)",
    "Still Spell (Metamagic)",
    "Strike Back (Combat)",
    "Stunning Critical (Combat, Critical)",
    "Stunning Fist (Combat)",
    
    "Throw Anything (Combat)",
    "Tiring Critical (Combat, Critical)",
    "Toughness",
    "Tower Shield Proficiency (Combat)",
    "Trample (Combat)",
    "Turn Undead",
    "Two-Weapon Defense (Combat)",
    "Two-Weapon Fighting (Combat)",
    "Two-Weapon Rend (Combat)",
    
    "Unseat (Combat)",
    
    "Vital Strike (Combat)",
    
    "Weapon Finesse (Combat)",
    "Weapon Focus (Combat)",
    "Weapon Specialization (Combat)",
    "Whirlwind Attack (Combat)",
    "Widen Spell (Metamagic)",
    "Wind Stance (Combat)"
};

//" ’ "

const char *FeatsDesc[]=
{
    "You are skilled at leaping, jumping and flying.\r\n\r\nBenefit: You get a +2 bonus on all Acrobatics and Fly skill checks.\r\nIf you have 10 or more ranks in one of these skills, the bonus increases to +4 for that skill.",
    
    "You can easily move over and through obstacles.\r\n\r\nPrerequisites: Dex 15, Nimble Moves.\r\n\r\nBenefit: Whenever you move, you may move through up to 15 feet of difficult terrain each round as if it were normal terrain.The effects of this feat stack with those provided by Nimble Moves (allowing you to move normally through a total of 20 feet of difficult terrain each round).",
    
    "You've learned to use your quickness in place of brute force when performing combat maneuvers.\r\n\r\nBenefit:  You  add  your  Dexterity  bonus  to  your  base attack  bonus  and  size  bonus  when  determining  your Combat Maneuver Bonus (see Chapter 8) instead of your Strength bonus.\r\n\r\nNormal:  You  add  your  Strength  bonus  to  your  base attack  bonus  and  size  bonus  when  determining  your Combat Maneuver Bonus.",
    
    "You often notice things that others might miss.\r\n\r\nBenefit:  You  get  a  +2  bonus  on  Perception  and  Sense Motive skill checks. If you have 10 or more ranks in one of these skills, the bonus increases to +4 for that skill.",
    
    "Choose chaos, evil, good, or law. You can channel divine energy to affect outsiders that possess this subtype.\r\n\r\nPrerequisites: Ability to channel energy.\r\n\r\nBenefit: Instead of its normal effect, you can choose to have your ability to channel energy heal or harm outsiders of  the  chosen  alignment  subtype.  You  must  make  this choice  each  time  you  channel  energy.  If  you  choose  to heal or harm creatures of the chosen alignment subtype, your channel energy has no effect on other creatures. The amount of damage healed or dealt and the DC to halve the damage is otherwise unchanged.\r\n\r\nSpecial: You can gain this feat multiple times. Its effects do not stack. Each time you take this feat, it applies to a new  alignment  subtype.  Whenever  you  channel  energy, you must choose which type to effect.",
    
    "You are skilled at working with animals and mounts.\r\n\r\nBenefit: You get a +2 bonus on all Handle Animal and Ride skill checks. If you have 10 or more ranks in one of these skills, the bonus increases to +4 for that skill.",
    
    "You  have  mastered  the  ability  to  cast  spells  while wearing armor.\r\n\r\nPrerequisites:  Arcane  Armor  Training,  Medium  Armor Proficiency, caster level 7th.\r\n\r\nBenefit: As a swift action, reduce the arcane spell failure chance due to the armor you are wearing by 20% for any spells you cast this round. This bonus replaces, and does not stack with, the bonus granted by Arcane Armor Training.",
    
    "You have learned how to cast spells while wearing armor.\r\n\r\nPrerequisites: Light Armor Proficiency, caster level 3rd.\r\n\r\nBenefit: As a swift action, reduce the arcane spell failure chance due to the armor you are wearing by 10% for any spells you cast this round.",
    
    "You  draw  upon  your  arcane  power  to  enhance  your weapons with magical energy.\r\n\r\nPrerequisite: Ability to cast arcane spells.\r\n\r\nBenefit: As a swift action, you can imbue your weapons with a fraction of your power. For 1 round, your weapons deal +1 damage and are treated as magic for the purpose of overcoming damage reduction. For every five caster levels you possess, this bonus increases by +1, to a maximum of +5 at 20th level.",
    
    "You are skilled at wearing heavy armor.\r\n\r\nPrerequisites:  Light  Armor  Proficiency,  Medium Armor Proficiency.\r\n\r\nBenefit: See Armor Proficiency, Light.\r\n\r\nNormal: See Armor Proficiency, Light.\r\n\r\nSpecial: Fighters and paladins automatically have Heavy Armor Proficiency as a bonus feat. They need not select it.",
    
    "You are skilled at wearing light armor.\r\n\r\nBenefit:  When  you  wear  a  type  of  armor  with  which you  are  proficient,  the  armor  check  penalty  for  that armor  applies  only  to  Dexterity and  Strength-based  skill checks.\r\n\r\nNormal: A character who is wearing armor with which he  is  not  proficient  applies  its  armor  check  penalty  to a ttack rolls and to all skill checks that involve moving.\r\n\r\nSpecial:  All  characters  except  monks,  sorcerers,  and wizards automatically have Light Armor Proficiency as a bonus feat. They need not select it.",
    
    "You are skilled at wearing medium armor.\r\n\r\nPrerequisite: Light Armor Proficiency.\r\n\r\nBenefit: See Armor Proficiency, Light.\r\n\r\nNormal: See Armor Proficiency, Light.\r\n\r\nSpecial: Barbarians, clerics, druids, fighters, paladins, and  rangers  automatically  have  Medium  Armor Proficiency as a bonus feat. They need not select it.",
    
    "You possess inherent physical prowess.\r\n\r\nBenefit: You get a +2 bonus on Climb and Swim skill checks. If you have 10 or more ranks in one of these skills, the bonus increases to +4 for that skill.",
    
    "Your summoned creatures are more powerful and robust.\r\n\r\nPrerequisite: Spell Focus (conjuration).\r\n\r\nBenefit: Each creature you conjure with any summon spell gains a +4 enhancement bonus to Strength and Constitution for the duration of the spell that s ummoned it.",
    
    "Your critical hits cause opponents to bleed profusely.\r\n\r\nPrerequisites: Critical Focus, base attack bonus +11.\r\n\r\nBenefit: Whenever you score a critical hit with a slashing or piercing weapon, your opponent takes 2d6 points of bleed damage (see Appendix 2) each round on his turn, in addition to the damage dealt by the critical hit. Bleed damage can be stopped by a DC 15 Heal skill check or through any magical healing. The effects of this feat stack.\r\n\r\nSpecial: You can only apply the effects of one critical feat to a given critical hit unless you possess Critical Mastery.",
    
    "You  are  skilled  at  attacking  opponents  that  you  cannot clearly perceive.\r\n\r\nBenefit:  In  melee,  every  time  you  miss  because  of concealment  (see  Chapter  8),  you  can  reroll  your  miss chance percentile roll one time to see if you actually hit.An  invisible  attacker  gets  no  advantages  related  to h itting you in melee. That is, you don't lose your Dexterity bonus  to  Armor  Class,  and  the  attacker  doesn't  get  the usual +2 bonus for being invisible. The invisible attacker's bonuses do still apply for ranged attacks, however.You do not need to make Acrobatics skill checks to move at full speed while blinded.\r\n\r\nNormal:  Regular  attack  roll  modifiers  for  invisible attackers  trying  to  hit  you  apply,  and  you  lose  your Dexterity bonus to AC. The speed reduction for darkness and poor visibility also applies.\r\n\r\nSpecial:  The  Blind-Fight  feat  is  of  no  use  against  a character who is the subject of a blink spell.",
    
    "Your critical hits blind your opponents.\r\n\r\nPrerequisites: Critical Focus, base attack bonus +15.\r\n\r\nBenefit:  Whenever  you  score  a  critical  hit,  your opponent  is  permanently  blinded.  A  successful  Fortitude save  reduces  this  to  dazzled  for  1d4  rounds.  The  DC  of this Fortitude save is equal to 10 + your base attack bonus. This  feat  has  no  effect  on  creatures  that  do  not  rely on  eyes  for  sight  or  creatures  with  more  than  two  eyes (although  multiple  critical  hits  might  cause  blindness, at the GM's discretion). Blindness can be cured by heal, regeneration, remove blindness, or similar abilities.\r\n\r\nSpecial: You can only apply the effects of one critical feat to a given critical hit unless you possess Critical Mastery.",
    
    "You can create magic potions.\r\n\r\nPrerequisite: Caster level 3rd.\r\n\r\nBenefit: You can create a potion of any 3rd-level or lower spell that you know and that targets one or more creatures or objects. Brewing a potion takes 2 hours if its base price is 250 gp or less, otherwise brewing a potion takes 1 day for each 1,000 gp in its base price. When you create a potion, you set the caster level, which must be sufficient to cast the spell in question and no higher than your own level. To brew a potion, you must use up raw materials costing one half this base price. See the magic item creation rules in Chapter 15 for more information.When  you  create  a  potion,  you  make  any  choices  that you would normally make when casting the spell. Whoever drinks the potion is the target of the spell.",
    
    "Foes are surprised by your skilled use of unorthodox and improvised weapons.\r\n\r\nBenefit:  You  do  not  suffer  any  penalties  for  using  an improvised melee weapon. Unarmed opponents are flat-footed against any attacks you make with an improvised melee weapon.\r\n\r\nNormal: You take a –4 penalty on attack rolls made with an improvised weapon.",
    
    "You  can  channel  your  divine  energy  through  a  melee weapon you wield.\r\n\r\nPrerequisite: Channel energy class feature.\r\n\r\nBenefit: Before you make a melee attack roll, you can choose  to  spend  one  use  of  your  channel  energy  ability as a swift action. If you channel positive energy and you hit  an  undead  creature,  that  creature  takes  an  amount of additional damage equal to the damage dealt by your channel  positive  energy  ability.  If  you  channel  negative energy and you hit a living creature, that creature takes an amount of additional damage equal to the damage dealt by your channel negative energy ability. Your target can make a  Will  save,  as  normal,  to  halve  this  additional  damage. If  your  attack  misses,  the  channel  energy  ability  is  still expended with no effect.",
    
    "You can strike two adjacent foes with a single swing.\r\n\r\nPrerequisites: Str 13, Power Attack, base attack bonus +1.\r\n\r\nBenefit: As a standard action, you can make a single attack at your full base attack bonus against a foe within reach. If you hit, you deal damage normally and can make an additional attack (using your full base attack bonus) against a foe that is adjacent to the first and also within reach.  You  can  only  make  one  additional  attack  per round with this feat. When you use this feat, you take a –2 penalty to your Armor Class until your next turn.",
    
    "You are adept at spellcasting when threatened or distracted.\r\n\r\nBenefit:  You  get  a  +4  bonus  on  concentration  checks made to cast a spell or use a spell-like ability when casting on the defensive or while grappled.",
    
    "You can increase your defense at the expense of your accuracy.\r\n\r\nPrerequisite: Int 13.\r\n\r\nBenefit: You can choose to take a –1 penalty on melee attack  rolls  and  combat  maneuver  checks  to  gain  a  +1 dodge bonus to your Armor Class. When your base attack bonus  reaches  +4,  and  every  +4  thereafter,  the  penalty increases by –1 and the dodge bonus increases by +1. You can only choose to use this feat when you declare that you are making an attack or a full-attack action with a melee weapon. The effects of this feat last until your next turn.",
    
    "You can make additional attacks of opportunity.\r\n\r\nBenefit: You may make a number of additional attacks of opportunity per round equal to your Dexterity bonus. With this feat, you may also make attacks of opportunity while flat-footed.\r\n\r\nNormal: A character without this feat can make only one attack of opportunity per round and can't make attacks of opportunity while flat-footed.\r\n\r\nSpecial: The Combat Reflexes feat does not allow a rogue to use her opportunist ability more than once per round.",
    
    "Using  foul  powers  of  necromancy,  you  can  command undead creatures, making them into your servants.\r\n\r\nPrerequisites: Channel negative energy class feature.\r\n\r\nBenefit:  As  a  standard  action,  you  can  use  one  of  your uses of channel negative energy to enslave undead within 30 feet. Undead receive a Will save to negate the effect. The DC for this Will save is equal to 10 + 1/2 your cleric level + your Charisma modifier. Undead that fail their saves fall under your control, obeying your commands to the best of their ability, as if under the effects of control undead. Intelligent undead receive a new saving throw each day to resist your command. You can control any number of undead, so long as their total Hit Dice do not exceed your cleric level. If you use channel energy in this way, it has no other effect (it does not heal or harm nearby creatures). If an undead creature is under the control of another creature, you must make an opposed Charisma check whenever your orders conflict.",
    
    "You can create magic armor, shields, and weapons.\r\n\r\nPrerequisite: Caster level 5th.\r\n\r\nBenefit: You can create magic weapons, armor, or shields. Enhancing a weapon, suit of armor, or shield takes 1 day for  each  1,000  gp  in  the  price  of  its  magical  features.  To enhance a weapon, suit of armor, or shield, you must use up raw materials costing half of this total price. See the magic item creation rules in Chapter 15 for more information.The weapon, armor, or shield to be enhanced must be a masterwork item that you provide. Its cost is not included in the above cost.You can also mend a broken or destroyed magic weapon, suit of armor, or shield if it is one that you could make. Doing so costs half the raw materials and half the time it would take to craft that item in the first place.",
    
    "You can create magic rods.\r\n\r\nPrerequisite: Caster level 9th.\r\n\r\nBenefit:  You  can  create  magic  rods.  Crafting  a  rod takes 1 day for each 1,000 gp in its base price. To craft a rod, you must use up raw materials costing half of its base price. See the magic item creation rules in Chapter 15 for more information.",
    
    "You can create magic staves.\r\n\r\nPrerequisite: Caster level 11th.\r\n\r\nBenefit: You can create any staff whose prerequisites you meet. Crafting a staff takes 1 day for each 1,000 gp in its base price. To craft a staff, you must use up raw materials costing half of its base price. A newly created staff has 10 charges. See the magic item creation rules in Chapter 15 for more information.",
    
    "You can create magic wands.\r\n\r\nPrerequisite: Caster level 5th.\r\n\r\nBenefit: You can create a wand of any 4th-level or lower spell that you know. Crafting a wand takes 1 day for each 1,000 gp in its base price. To craft a wand, you must use up raw materials costing half of this base price. A newly created wand has 50 charges. See the magic item creation rules in Chapter 15 for more information.",
    
    "You can create wondrous items, a type of magic item.\r\n\r\nPrerequisite: Caster level 3rd.\r\n\r\nBenefit: You can create a wide variety of magic wondrous items. Crafting a wondrous item takes 1 day for each 1,000 gp in its price. To create a wondrous item, you must use up raw materials costing half of its base price. See the magic item creation rules in Chapter 15 for more information.You can also mend a broken or destroyed wondrous item if it is one that you could make. Doing so costs half the raw materials and half the time it would take to craft that item.",
    
    "You are trained in the art of causing pain.\r\n\r\nPrerequisites: Base attack bonus +9.\r\n\r\nBenefit: You receive a +4 circumstance bonus on attack rolls made to confirm critical hits.",
    
    "Your critical hits cause two additional effects.\r\n\r\nPrerequisites: Critical Focus, any two critical feats, 14th-level fighter.\r\n\r\nBenefit: When you score a critical hit, you can apply the effects of two critical feats in addition to the damage dealt.\r\n\r\nNormal:  You  can  only apply  the  effects  of  one critical feat to a given critical hit in addition to the damage dealt.",
    
    "Your skill with your favored weapon can frighten enemies.\r\n\r\nPrerequisite:  Weapon  Focus,  proficiency  with  the selected weapon.\r\n\r\nBenefit: While wielding the weapon in which you have Weapon  Focus,  you  can  perform  a  bewildering  show  of prowess as a full-round action. Make an Intimidate check to demoralize all foes within 30 feet who can see your display.",
    
    "You  can  make  exceptionally  deadly  ranged  attacks  by pinpointing a foe's weak spot, at the expense of making the attack less likely to succeed.\r\n\r\nPrerequisites: Dex 13, base attack bonus +1.\r\n\r\nBenefit:  You  can  choose  to  take  a  –1  penalty  on  all ranged attack rolls to gain a +2 bonus on all ranged damage rolls. When your base attack bonus reaches +4, and every +4 thereafter, the penalty increases by –1 and the bonus to damage increases by +2. You must choose to use this feat before making an attack roll and its effects last until your next turn. The bonus damage does not apply to touch attacks or effects that do not deal hit point damage.",
    
    "With a well-placed strike, you can bring a swift and painful end to most foes.\r\n\r\nPrerequisites: Dazzling Display, Greater Weapon Focus, Shatter  Defenses,  Weapon  Focus,  proficiency  with  the selected weapon, base attack bonus +11.\r\n\r\nBenefit: As a standard action, make a single attack with the weapon for which you have Greater Weapon Focus against a stunned or flat-footed opponent. If you hit, you deal double the normal damage and the target takes 1 point of Constitution bleed (see Appendix 2). The additional damage and bleed is not multiplied on a critical hit.",
    
    "Your critical hits cause enemies to lose their hearing.\r\n\r\nPrerequisites: Critical Focus, base attack bonus +13.\r\n\r\nBenefit:  Whenever  you  score  a  critical  hit  against an  opponent,  the  victim  is  permanently  deafened.  A successful Fortitude save reduces the deafness to 1 round. The DC of this Fortitude save is equal to 10 + your base attack bonus. This feat has no effect on deaf creatures. This  deafness  can  be  cured  by  heal,  regeneration,  remove deafness, or a similar ability.\r\n\r\nSpecial: You can only apply the effects of one critical feat to a given critical hit unless you possess Critical Mastery.",
    
    "You are skilled at deceiving others, both with the spoken word and with physical disguises.\r\n\r\nBenefit: You get a +2 bonus on all Bluff and Disguise skill checks. If you have 10 or more ranks in one of these skills, the bonus increases to +4 for that skill.",
    
    "You  excel  at  defending  yourself  from  all  manner  of combat maneuvers.\r\n\r\nBenefit:  You  treat  your  total  Hit  Dice  as  your base  attack  bonus  when  calculating  your  Combat Maneuver Defense (see Chapter 8). ",
    
    "You  can  knock  arrows  and  other  projectiles  off  course, preventing them from hitting you.\r\n\r\nPrerequisites: Dex 13, Improved Unarmed Strike.\r\n\r\nBenefit: You must have at least one hand free (holding nothing) to use this feat. Once per round when you would normally be hit with an attack from a ranged weapon, you may deflect it so that you take no damage from it. You must be  aware  of  the  attack  and  not  flat-footed.  Attempting to  deflect  a  ranged  attack  doesn't  count  as  an  action. Unusually massive ranged weapons (such as boulders or ballista  bolts)  and  ranged  attacks  generated  by  natural attacks or spell effects can't be deflected.",
    
    "You have exceptional manual dexterity.\r\n\r\nBenefit: You get a +2 bonus on Disable Device and Sleight of Hand skill checks. If you have 10 or more ranks in one of these skills, the bonus increases to +4 for that skill.",
    
    "You are especially hard to kill. Not only do your wounds automatically stabilize when grievously injured, but you can remain conscious and continue to act even at death's door.\r\n\r\nPrerequisite: Endurance.\r\n\r\nBenefit: When your hit point total is below 0, but you are not dead, you automatically stabilize. You do not need to make a Constitution check each round to avoid losing additional hit points. You may choose to act as if you were disabled, rather than dying. You must make this decision as soon as you are reduced to negative hit points (even if it isn't your turn). If you do not choose to act as if you were disabled, you immediately fall unconscious.When using this feat, you are staggered. You can take a  move  action  without  further  injuring  yourself,  but  if you  perform  any  standard  action  (or  any  other  action deemed as strenuous, including some swift actions, such as casting a quickened spell) you take 1 point of damage after completing the act. If your negative hit points are equal  to  or  greater  than  your  Constitution  score,  you immediately die.\r\n\r\nNormal: A character without this feat who is reduced to negative hit points is unconscious and dying.",
    
    "Your training makes it difficult for enemy spellcasters to safely cast spells near you.\r\n\r\nPrerequisites: 6th-level fighter.\r\n\r\nBenefit: The DC to cast spells defensively increases by +4  for  all  enemies  that  are  within  your  threatened  area. This increase to casting spells defensively only applies if you are aware of the enemy's location and are capable of taking an attack of opportunity. If you can only take one attack of opportunity per round and have already used that attack, this increase does not apply.",
    
    "Your  training  and  reflexes  allow  you  to  react  swiftly  to avoid an opponents' attacks.\r\n\r\nPrerequisite: Dex 13.\r\n\r\nBenefit:  You  gain  a  +1  dodge  bonus  to  your AC. A condition that makes you lose your Dex bonus to AC also makes you lose the benefits of this feat.",
    
    "Your  off-hand  weapon  while  dual-wielding  strikes  with greater power.\r\n\r\nPrerequisite: Dex 15, Two-Weapon Fighting.\r\n\r\nBenefit: Add your Strength bonus to damage rolls made with your off-hand weapon.\r\n\r\nNormal: You normally add only half of your Strength modifier to damage rolls made with a weapon wielded in your off-hand.",
    
    "Choose one elemental subtype, such as air, earth, fire, or water. You can channel your divine energy to harm or heal outsiders that possess your chosen elemental subtype.\r\n\r\nPrerequisites: Channel energy class feature.\r\n\r\nBenefit: Instead of its normal effect, you can choose to have your ability to channel energy heal or harm outsiders of  your  chosen  elemental  subtype.  You  must  make  this choice each time you channel energy. If you choose to heal or harm creatures of your elemental subtype, your channel energy  has  no  affect  on  other  creatures.  The  amount  of damage healed or dealt and the DC to halve the damage is otherwise unchanged.\r\n\r\nSpecial: You can gain this feat multiple times. Its effects do not stack. Each time you take this feat, it applies to a new elemental subtype.",
    
    "You can increase the power of your spells, causing them to deal more damage.\r\n\r\nBenefit: All variable, numeric effects of an empowered spell  are  increased  by  half,  including  bonuses  to  those dice rolls.Saving throws and opposed rolls are not affected, nor are spells without random variables. An empowered spell uses up a spell slot two levels higher than the spell's actual level.",
    
    "Harsh conditions or long exertions do not easily tire you.\r\n\r\nBenefit:  You gain a +4 bonus  on  the  following checks and saves: Swim checks made to resist nonlethal damage from  exhaustion;  Constitution  checks  made  to  continue running;  Constitution  checks  made  to  avoid  nonlethal damage from a forced march; Constitution checks made to  hold  your  breath;  Constitution  checks  made  to  avoid nonlethal  damage  from  starvation  or  thirst;  Fortitude saves  made  to  avoid  nonlethal  damage  from  hot  or  cold environments; and Fortitude saves made to resist damage from suffocation.You  may  sleep  in  light  or  medium  armor  without becoming fatigued.\r\n\r\nNormal:  A  character  without  this  feat  who  sleeps  in medium or heavier armor is fatigued the next day.",
    
    "You can increase the range of your spells.\r\n\r\nBenefit:  You  can  alter  a  spell  with  a  range  of  close, medium, or long to increase its range by 100%. An enlarged spell with a range of close now has a range of 50 ft. + 5 ft./level, while medium-range spells have a range of 200 ft. + 20 ft./level and long-range spells have a range of 800 ft. + 80 ft./level. An enlarged spell uses up a spell slot one level higher than the spell's actual level.Spells whose ranges are not defined by distance, as well as spells whose ranges are not close, medium, or long, do not benefit from this feat.",
    
    "You can cast many spells without needing to utilize minor material components.\r\n\r\nBenefit:  You  can  cast  any  spell  with  a  material component  costing  1  gp  or  less  without  needing  that component. The casting of the spell still provokes attacks of opportunity as normal. If the spell requires a material component that costs more than 1 gp, you must have the material component on hand to cast the spell, as normal.",
    
    "Your critical hits cause opponents to become exhausted.\r\n\r\nPrerequisites:  Critical  Focus,  Tiring  Critical,  base attack bonus +15.\r\n\r\nBenefit:  When  you  score  a  critical  hit  on  a  foe,  your target immediately becomes exhausted. This feat has no effect on exhausted creatures.\r\n\r\nSpecial:  You  can  only  apply  the  effects  of  one  critical feat to a given critical hit unless you possess the Critical Mastery feat.",
    
    "Choose  one  type  of  exotic  weapon,  such  as  the  spiked chain  or  whip.  You  understand  how  to  use  that  type  of exotic weapon in combat, and can utilize any special tricks or qualities that exotic weapon might allow.\r\n\r\nPrerequisite: Base attack bonus +1.\r\n\r\nBenefit: You make attack rolls with the weapon normally.\r\n\r\nNormal: A character who uses a weapon with which he is not proficient takes a –4 penalty on attack rolls.\r\n\r\nSpecial: You can gain Exotic Weapon Proficiency multiple times. Each time you take the feat, it applies to a new type of exotic weapon. ",
    
    "You can make your spells last twice as long.\r\n\r\nBenefit: An extended spell lasts twice as long as normal. A spell with a duration of concentration, instantaneous, or  permanent  is  not  affected  by  this  feat.  An  extended spell uses up a spell slot one level higher than the spell's actual level.",
    
    "You can channel divine energy more often.\r\n\r\nPrerequisite: Channel energy class feature.\r\n\r\nBenefit: You can channel energy two additional times per day.\r\n\r\nSpecial: If a paladin with the ability to channel positive energy  takes  this  feat,  she  can  use  lay  on  hands  four additional times a day, but only to channel positive energy.",
    
    "You can use your ki pool more times per day than most.\r\n\r\nPrerequisite: Ki pool class feature.\r\n\r\nBenefit: Your ki pool increases by 2.\r\n\r\nSpecial:  You  can  gain  Extra  Ki  multiple  times.  Its effects stack.",
    
    "You can use your lay on hands ability more often.\r\n\r\nPrerequisite: Lay on hands class feature.\r\n\r\nBenefit:  You  can  use  your  lay  on  hands  ability  two additional times per day.\r\n\r\nSpecial:  You  can  gain  Extra  Lay  On  Hands  multiple times. Its effects stack.",
    
    "Your lay on hands ability adds an additional mercy.\r\n\r\nPrerequisites:  Lay  on  hands  class feature, mercy class feature.\r\n\r\nBenefit:  Select  one  additional  mercy  for  which you qualify. When you use lay on hands to heal damage to one target, it also receives the additional effects of this mercy.\r\n\r\nSpecial: You can gain this feat multiple times. Its effects do not stack. Each time you take this feat, select a new mercy.",
    
    "You can use your bardic performance ability more often than normal.\r\n\r\nPrerequisite: Bardic performance class feature.\r\n\r\nBenefit: You can use bardic performance for 6 additional rounds per day.\r\n\r\nSpecial:  You  can  gain  Extra  Performance  multiple times. Its effects stack.",
    
    "You can use your rage ability more than normal.\r\n\r\nPrerequisite: Rage class feature.\r\n\r\nBenefit: You can rage for 6 additional rounds per day.\r\n\r\nSpecial: You  can gain Extra Rage multiple times. Its effects stack.",
    
    "You are more accurate at longer ranges.\r\n\r\nPrerequisites: Point-Blank Shot.\r\n\r\nBenefit: You only suffer a –1 penalty per full range increment between you and your target when using a ranged weapon.\r\n\r\nNormal: You suffer a –2 penalty per full range increment between you and your target.",
    
    "You are faster than most.\r\n\r\nBenefit: While you are wearing light or no armor, your base speed increases by 5 feet. You lose the benefits of this feat if you carry a medium or heavy load.\r\n\r\nSpecial: You can take this feat multiple times. The effects stack.",
    
    "You can create magic rings.\r\n\r\nPrerequisite: Caster level 7th.\r\n\r\nBenefit:  You  can  create  magic  rings.  Crafting  a  ring takes 1 day for each 1,000 gp in its base price. To craft a ring,  you  must  use  up  raw  materials  costing  half  of  the base price. See the magic item creation rules in Chapter 15 for more information.You can also mend a broken ring if it is one that you could make. Doing so costs half the raw materials and half the time it would take to forge that ring in the first place.",
    
    "With one well-placed blow, you leave your target reeling.\r\n\r\nPrerequisites:  Improved  Unarmed  Strike, Scorpion Style, base attack bonus +6.\r\n\r\nBenefit: As a standard action, make a single unarmed melee attack against a foe whose speed is reduced (such as from Scorpion Style). If the attack hits, you deal damage normally and the target is staggered until the end of your next turn unless it makes a Fortitude saving throw (DC 10 + 1/2 your character level + your Wis modifier). This feat has no effect on targets that are staggered.",
    
    "You can strike many adjacent foes with a single blow.\r\n\r\nPrerequisites: Str 13, Cleave, Power Attack, base attack bonus +4.\r\n\r\nBenefit: As a standard action, you can make a single attack at your full base attack bonus against a foe within reach. If you hit, you deal damage normally and can make an additional attack (using your full base attack bonus) against a foe that is adjacent to the previous foe and also within reach. If you hit, you can continue to make attacks against foes adjacent to the previous foe, so long as they are within your reach. You cannot attack an individual foe more than once during this attack action. When you use this feat, you take a –2 penalty to your Armor Class until your next turn.",
    
    "You are resistant to poisons, diseases, and other maladies.\r\n\r\nBenefit: You get a +2 bonus on all Fortitude saving throws.",
    
    "Your bull rush attacks throw enemies off balance.\r\n\r\nPrerequisites: Improved Bull Rush, Power Attack, base attack bonus +6, Str 13.\r\n\r\nBenefit: You receive a +2 bonus on checks made to bull rush a foe. This bonus stacks with the bonus granted by Improved Bull Rush. Whenever you bull rush an opponent, his movement provokes attacks of opportunity from all of your allies (but not you).\r\n\r\nNormal: Creatures moved by bull rush do not provoke attacks of opportunity.",
    
    "You can knock weapons far from an enemy’s grasp.\r\n\r\nPrerequisites:  Combat  Expertise, Improved Disarm, base attack bonus +6, Int 13.\r\n\r\nBenefit:  You  receive  a  +2  bonus  on  checks  made  to disarm a foe. This bonus stacks with the bonus granted by Improved Disarm. Whenever you successfully disarm an  opponent,  the  weapon  lands  15  feet  away  from  its previous wielder, in a random direction.\r\n\r\nNormal: Disarmed weapons and gear land at the feet of the disarmed creature.",
    
    "You are skilled at making foes overreact to your attacks.\r\n\r\nPrerequisites: Combat Expertise, Improved Feint, base attack bonus +6, Int 13.\r\n\r\nBenefit: Whenever you use feint to cause an opponent to lose his Dexterity bonus, he loses that bonus until the beginning  of  your  next  turn,  in  addition  to  losing  his Dexterity bonus against your next attack.\r\n\r\nNormal: A creature you feint loses its Dexterity bonus against your next attack.",
    
    "Maintaining a grapple is second nature to you.\r\n\r\nPrerequisites:  Improved  Grapple,  Improved  Unarmed Strike, base attack bonus +6, Dex 13.\r\n\r\nBenefit:  You  receive  a  +2  bonus  on  checks  made  to grapple a foe. This bonus stacks with the bonus granted by Improved Grapple. Once you have grappled a creature, maintaining the grapple is a move action. This feat allows you  to  make  two  grapple  checks  each  round  (to  move, harm, or pin your opponent), but you are not required to make two checks. You only need to succeed at one of these checks to maintain the grapple.\r\n\r\nNormal: Maintaining a grapple is a standard action.",
    
    "Enemies must dive to avoid your dangerous move.\r\n\r\nPrerequisites:  Improved  Overrun,  Power  Attack,  base attack bonus +6, Str 13.Benefit:  You  receive  a  +2  bonus  on  checks  made  to overrun a foe. This bonus stacks with the bonus granted by Improved Overrun. Whenever you overrun opponents, they  provoke  attacks  of  opportunity  if  they  are  knocked prone by your overrun.\r\n\r\nNormal: Creatures knocked prone by your overrun do not provoke an attack of opportunity.",
    
    "Your attacks penetrate the defenses of most foes.\r\n\r\nPrerequisites: Penetrating Strike, Weapon Focus, 16th-level fighter.\r\n\r\nBenefit: Your attacks made with weapons selected with Weapon Focus ignore up to 10 points of damage reduction. This amount is reduced to 5 points for damage reduction without a type (such as DR 10/-).",
    
    "You are skilled at deflecting blows with your shield.\r\n\r\nPrerequisites:  Shield  Focus, Shield  Proficiency, 8th-level fighter.\r\n\r\nBenefit: Increase the AC  bonus granted by any shield you are using by 1. This  bonus  stacks  with the bonus granted by Shield Focus.",
    
    "Choose a school of magic to which you have already applied the Spell Focus feat. Any spells you cast of this school are very hard to resist.\r\n\r\nPrerequisite: Spell Focus.\r\n\r\nBenefit: Add  +1  to  the  Difficulty Class for all saving throws against spells from the school of magic you select. This bonus stacks with the bonus from Spell Focus.\r\n\r\nSpecial: You can  gain this feat multiple times. Its effects do not stack. Each time you take the feat, it applies to a new school to which you  already have applied the Spell Focus feat.",
    
    "Your  spells  break  through  spell  resistance  much  more easily than most.\r\n\r\nPrerequisite: Spell Penetration.\r\n\r\nBenefit:  You  get  a  +2  bonus  on  caster  level  checks (1d20 + caster level) made to overcome a creature’s spell resistance.  This  bonus  stacks  with  the  one  from  Spell Penetration.",
    
    "Your  devastating  strikes  cleave  through  weapons  and armor and into their wielders, damaging both item and wielder alike in a single terrific strike.\r\n\r\nPrerequisites:  Improved  Sunder, Power Attack, base attack bonus +6, Str 13.\r\n\r\nBenefit:  You  receive  a  +2  bonus  on  checks  made  to sunder an item. This bonus stacks with the bonus granted by Improved Sunder. Whenever you sunder to destroy a weapon,  shield,  or  suit  of  armor,  any  excess  damage  is applied to the item’s wielder. No damage is transferred if you decide to leave the item with 1 hit point.",
    
    "You can make free attacks on foes that you knock down.\r\n\r\nPrerequisites: Combat  Expertise, Improved Trip, base attack bonus +6, Int 13.\r\n\r\nBenefit: You receive a +2 bonus on checks made to trip a foe. This bonus stacks with the bonus granted by Improved Trip. Whenever  you  successfully  trip  an  opponent,  that opponent provokes attacks of opportunity.\r\n\r\nNormal: Creatures do not provoke attacks of opportunity from being tripped.",
    
    "You are incredibly skilled at fighting with two weapons at the same time.\r\n\r\nPrerequisites: Dex 19, Improved Two-Weapon Fighting, Two-Weapon Fighting, base attack bonus +11.\r\n\r\nBenefit:  You  get  a  third  attack  with  your  off-hand weapon, albeit at a –10 penalty.",
    
    "You can make a single attack that deals incredible damage.\r\n\r\nPrerequisites: Improved Vital Strike, Vital Strike, base attack bonus +16.\r\n\r\nBenefit:  When  you  use  the  attack  action,  you  can make one attack at your highest base attack bonus that deals additional damage. Roll the weapon’s damage dice for the attack four times and add the results together before adding bonuses from Strength, weapon abilities (such as flaming),  precision-based  damage  (such as sneak attack), and other damage bonuses. These extra weapon damage dice are not multiplied on a critical hit, but are added to the total.",
    
    "Choose one type of weapon (including u narmed strike or grapple) for which you have already selected Weapon Focus. You are a master at your chosen weapon.\r\n\r\nPrerequisites: Proficiency  with  selected  weapon, Weapon Focus with selected weapon, 8th-level fighter.\r\n\r\nBenefit: You gain a +1 bonus on attack rolls you make using  the  selected  weapon.  This  bonus  stacks  with other  bonuses  on  attack  rolls,  including  those  from Weapon Focus.\r\n\r\nSpecial: You can gain Greater Weapon Focus multiple times. Its effects do not stack. Each time you take the feat, it applies to a new type of weapon.",
    
    "Choose one type of weapon (including u narmed strike or grapple) for which you possess the Weapon Specialization feat.  Your  attacks  with  the  chosen  weapon  are  more devastating than normal.\r\n\r\nPrerequisites:  Proficiency  with  selected  weapon, Greater  Weapon  Focus  with  selected  weapon,  Weapon Focus with selected weapon, Weapon Specialization with selected weapon, 12th-level fighter.\r\n\r\nBenefit:  You  gain  a  +2  bonus  on  all  damage  rolls  you make using the selected weapon. This bonus to damage stacks with other damage roll bonuses, including any you gain from Weapon Specialization.\r\n\r\nSpecial:  You  can  gain  Greater  Weapon  Specialization multiple times. Its effects do not stack. Each time you take the feat, it applies to a new type of weapon.",
    
    "You can cast spells as if they were a higher level.\r\n\r\nBenefit:  A  heightened  spell  has  a  higher  spell  level than  normal  (up  to  a  maximum  of  9th  level).  Unlike other metamagic feats, Heighten Spell actually increases the effective level of the spell that it modifies. All effects dependent  on  spell  level  (such  as  saving  throw  DCs and  ability  to  penetrate  a  lesser  globe  of  invulnerability) are  calculated  according  to  the  heightened  level.  The heightened  spell  is  as  difficult  to  prepare  and  cast  as  a spell of its effective level. ",
    
    "You are skilled at pushing your foes around.\r\n\r\nPrerequisite: Str 13, Power Attack, base attack bonus +1.\r\n\r\nBenefit:  You  do  not  provoke  an  attack  of  opportunity when  performing  a  bull  rush  combat  maneuver. In addition, you receive a +2 bonus on checks made to bull rush  a  foe.  You  also  receive  a  +2  bonus  to  your  Combat Maneuver  Defense  whenever  an  opponent  tries  to  bull rush you.\r\n\r\nNormal:  You  provoke  an  attack  of  opportunity  when performing a bull rush combat maneuver.",
    
    "Your channeled energy is harder to resist.\r\n\r\nPrerequisite: Channel energy class feature.\r\n\r\nBenefit: Add 2 to the DC of saving throws made to resist the effects of your channel energy ability. ",
    
    "You  are  skilled  at  countering  the  spells  of  others  using similar spells.\r\n\r\nBenefit: When counterspelling, you may use a spell of the  same  school  that  is  one  or  more  spell  levels  higher than the target spell.\r\n\r\nNormal: Without this feat, you may counter a spell only with the same spell or with a spell specifically designated as countering the target spell.",
    
    "Attacks made with your chosen weapon are quite deadly.\r\n\r\nPrerequisite: Proficient with weapon, base attack bonus +8.\r\n\r\nBenefit:  When  using  the  weapon  you  selected,  your threat range is doubled.\r\n\r\nSpecial: You can gain Improved Critical multiple times. The  effects  do  not  stack.  Each  time  you  take  the  feat,  it applies to a new type of weapon. This  effect  doesn’t  stack  with  any  other  effect  that expands the threat range of a weapon.",
    
    "You are skilled at knocking weapons from a foe’s grasp.\r\n\r\nPrerequisite: Int 13, Combat Expertise.\r\n\r\nBenefit:  You  do  not  provoke  an  attack  of  opportunity when performing a disarm combat maneuver. In addition, you receive a +2 bonus on checks made to disarm a foe. You also receive a +2 bonus to your Combat Maneuver Defense whenever an opponent tries to disarm you.\r\n\r\nNormal:  You  provoke  an  attack  of  opportunity  when performing a disarm combat maneuver.",
    
    "This  feat  allows  you  to  acquire  a  powerful  familiar,  but only when you could normally acquire a new familiar.\r\n\r\nPrerequisites:  Ability  to  acquire  a  new  familiar, compatible alignment, sufficiently high level (see below).\r\n\r\nBenefit:  When  choosing  a  familiar,  the  creatures  listed below are also available to you (see the Pathfinder RPG Bestiary for statistics on these creatures). You may choose a familiar with an alignment up to one step away on each alignment axis (lawful through chaotic, good through evil). (TODO: Add Familiar Table). Improved familiars otherwise use the rules for regular familiars,  with  two  exceptions:  if  the  creature’s  type  is something other than animal, its type does not change; and improved familiars do not gain the ability to speak with other creatures of their kind (although many of them already have the ability to communicate).",
    
    "You are skilled at fooling your opponents in combat.\r\n\r\nPrerequisites: Int 13, Combat Expertise.\r\n\r\nBenefit: You can make a Bluff check to feint in combat as a move action.\r\n\r\nNormal: Feinting in combat is a standard action.",
    
    "You are skilled at grappling opponents.\r\n\r\nPrerequisite: Dex 13, Improved Unarmed Strike.\r\n\r\nBenefit:  You  do  not  provoke  an  attack  of  opportunity when performing a grapple combat maneuver. In addition, you receive a +2 bonus on checks made to grapple a foe. You also receive a +2 bonus to your Combat Maneuver Defense whenever an opponent tries to grapple you.\r\n\r\nNormal:  You  provoke  an  attack  of  opportunity  when performing a grapple combat maneuver.",
    
    "You  can  draw  upon  an  inner  reserve  to  resist  diseases, poisons, and other grievous harm.\r\n\r\nPrerequisites: Great Fortitude.\r\n\r\nBenefit: Once per day, you may reroll a Fortitude save. You must decide to use this ability before the results are revealed. You must take the second roll, even if it is worse.",
    
    "Your quick reflexes allow you to react rapidly to danger.\r\n\r\nBenefit: You get a +4 bonus on initiative checks.",
    
    "Your clarity of thought allows you to resist mental attacks.\r\n\r\nPrerequisites: Iron Will.\r\n\r\nBenefit: Once per day, you may reroll a Will save. You must  decide  to  use  this  ability  before  the  results  are revealed. You must take the second roll, even if it is worse.",
    
    "You have a knack for avoiding danger all around you.\r\n\r\nPrerequisites: Lightning Reflexes.\r\n\r\nBenefit:  Once  per  day,  you  may  reroll  a  Reflex  save. You must decide to use this ability before the results are revealed. You must take the second roll, even if it is worse.",
    
    "You are skilled at running down your foes.\r\n\r\nPrerequisite: Str 13, Power Attack, base attack bonus +1.\r\n\r\nBenefit: You do not provoke an attack of opportunity when performing an overrun combat maneuver. In addition, you receive a +2 bonus on checks made to overrrun a foe. You also receive a +2 bonus to your Combat Maneuver Defense whenever an opponent tries to overrun you. Targets of your overrun attempt may not choose to avoid you.\r\n\r\nNormal:  You  provoke  an  attack  of  opportunity  when performing an overrun combat maneuver.",
    
    "Your ranged attacks ignore anything but total concealment and cover.\r\n\r\nPrerequisites:  Dex  19,  Point-Blank  Shot,  Precise  Shot, base attack bonus +11.\r\n\r\nBenefit:  Your  ranged  attacks  ignore  the  AC  bonus g ranted to targets by anything less than total cover, and the miss chance granted to targets by anything less than total  concealment.  Total  cover  and  total  c oncealment provide their normal benefits against your ranged attacks.\r\n\r\nNormal: See the normal rules on the effects of cover and concealment in Chapter 8.",
    
    "You can protect yourself with your shield, even if you use it to attack.\r\n\r\nPrerequisite: Shield Proficiency.\r\n\r\nBenefit: When you perform a shield bash, you may still apply the shield’s shield bonus to your AC.\r\n\r\nNormal: Without this feat, a character that performs a shield bash loses the shield’s shield bonus to AC until his next turn (see Chapter 6).",
    
    "You are skilled at damaging your foes’ weapons and armor.\r\n\r\nPrerequisite: Str 13, Power Attack, base attack bonus +1.\r\n\r\nBenefit: You do not provoke an attack of opportunity when  performing  a  sunder  combat  maneuver. In addition,  you  receive  a  +2  bonus  on  checks  made  to sunder  an  item.  You  also  receive  a  +2  bonus  to  your Combat Maneuver Defense whenever an opponent tries to sunder your gear.\r\n\r\nNormal:  You  provoke  an  attack  of  opportunity  when performing a sunder combat maneuver.",
    
    "You are skilled at sending your opponents to the ground.\r\n\r\nPrerequisite: Int 13, Combat Expertise.\r\n\r\nBenefit: You do not provoke an attack of opportunity when performing a trip combat maneuver. In addition, you receive a +2 bonus on checks made to trip a foe. You also receive a +2 bonus to your Combat Maneuver Defense whenever an opponent tries to trip you.\r\n\r\nNormal:  You  provoke  an  attack  of  opportunity  when performing a trip combat maneuver.",
    
    "You are skilled at fighting with two weapons.\r\n\r\nPrerequisites: Dex 17, Two-Weapon Fighting, base attack bonus +6.\r\n\r\nBenefit: In addition to the standard single extra attack you get with an off-hand weapon, you get a second attack with it, albeit at a –5 penalty.\r\n\r\nNormal:  Without  this  feat,  you  can  only  get  a  single extra attack with an off-hand weapon.",
    
    "You are skilled at fighting while unarmed.\r\n\r\nBenefit:  You  are  considered  to  be  armed  even  when unarmed—you  do  not  provoke  attacks  of  opportunity when  you  attack  foes  while  unarmed.  Your  unarmed strikes can deal lethal or nonlethal damage, at your choice.\r\n\r\nNormal: Without this feat, you are considered unarmed when attacking with an unarmed strike, and you can deal only nonlethal damage with such an attack.",
    
    "You can make a single attack that deals a large amount of damage.\r\n\r\nPrerequisites: Vital Strike, base attack bonus +11.\r\n\r\nBenefit:  When  you  use  the  attack  action,  you  can make  one  attack  at  your  highest  base  attack  bonus  that deals additional damage. Roll the weapon’s damage dice for  the  attack  three  times  and  add  the  results  together before  adding  bonuses  from  Strength,  weapon  abilities (such  as  flaming),  precision-based  damage,  and  other damage bonuses. These extra weapon damage dice are not multiplied on a critical hit, but are added to the total.",
    
    "You can turn nearly any object into a deadly weapon, from a razor-sharp chair leg to a sack of flour.\r\n\r\nPrerequisites:  Catch  Off-Guard  or  Throw  Anything, base attack bonus +8.\r\n\r\nBenefit: You do not suffer any penalties for using an improvised  weapon.  Increase  the  amount  of  damage dealt  by  the  improvised  weapon  by  one  step  (for example, 1d4 becomes 1d6) to a maximum of 1d8 (2d6 if the improvised weapon is two-handed). The improvised weapon  has  a  critical  threat  range  of  19–20,  with  a critical multiplier of ×2.",
    
    "Your physical might is intimidating to others.\r\n\r\nBenefit: Add your Strength modifier to Intimidate skill checks in addition to your Charisma modifier.",
    
    "You are more resistant to mental effects.\r\n\r\nBenefit: You get a +2 bonus on all Will saving throws.",
    
    "You attract followers to your cause and a companion to join you on your adventures.\r\n\r\nPrerequisite: Character level 7th.\r\n\r\nBenefits: This feat enables you to attract a loyal cohort and a number of devoted subordinates who assist you. A cohort is generally an NPC with class levels, while followers are typically lower level NPCs. See Table 5–2 for what level of cohort and how many followers you can recruit.\r\n\r\nLeadership  Modifiers:  Several  factors  can  affect  your Leadership  score,  causing  it  to  vary  from  the  base  score (character level + Cha modifier). Your reputation (from the point of view of the cohort or follower you are trying to attract) raises or lowers your Leadership score: (TODO: Leader Reputation Table).Other  modifiers  may  apply  when  you  try  to  attract  a cohort, as listed below. (TODO: Leader Modifier Table). Followers have different priorities from cohorts. When you try to attract a follower, use the following modifiers.  (TODO: Follower Modifier Table).\r\n\r\nLeadership Score: Your base Leadership score equals your level plus your Charisma modifier. In order to take into account negative Charisma modifiers, this table allows for very low Leadership scores, but you must still be 7th level or  higher  in  order  to  gain  the  Leadership  feat.  Outside factors can affect your Leadership score, as detailed above.\r\n\r\nCohort Level: You can attract a cohort of up to this level. Regardless of your Leadership score, you can only recruit a  cohort  who  is  two  or  more  levels  lower  than  yourself. The  cohort  should  be  equipped  with  gear  appropriate for its level (see Chapter 14). A cohort can be of any race or  class.  The  cohort’s  alignment  may  not  be  opposed  to your alignment on either the law/chaos or good/evil axis, and you take a –1 penalty to your Leadership score if you recruit a cohort of an alignment different from your own.A  cohort  does  not  count  as  a  party  member  when determining  the  party’s  XP.  Instead,  divide  the  cohort’s level  by  your  level.  Multiply  this  result  by  the  total  XP awarded to you, then add that number of experience points to the cohort’s total. If a cohort gains enough XP to bring it to a level one lower  than  your  level,  the  cohort  does  not  gain  the  new level—its new XP total is 1 less than the amount needed to attain the next level.\r\n\r\nNumber of  Followers  by  Level:  You  can  lead  up  to  the indicated number of characters of each level. Followers are similar to cohorts, except they’re generally low-level NPCs. Because they’re usually 5 or more levels behind you, they’re rarely effective in combat. Followers  don’t  earn experience  and  thus  don’t  gain levels.  When  you  gain  a  new  level,  consult  Table  5–2  to determine if you acquire more followers, some of whom may  be  higher  level  than  the  existing  followers. Don’t consult the table to see if your cohort gains levels, however, because cohorts earn experience on their own.",
    
    "You have faster reflexes than normal.\r\n\r\nBenefit: You get a +2 bonus on all Reflex saving throws.",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
    "",
    
};