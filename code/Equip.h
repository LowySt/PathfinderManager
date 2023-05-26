#ifndef _EQUIP_H
#define _EQUIP_H

struct DiceThrow
{
    //NOTETODO: 0, 0 indicates something special???
    s8 numDice;
    s8 numFaces;
};

struct CritRange
{
    // Range is the distance from 20, so 19-20/x2 would be {range: 1, mul: 2}
    s8 range;
    s8 mul;
};

enum DamageTypeFlag : u8
{
    DMG_BLUDGEONING = 1,
    DMG_SLASHING    = 2,
    DMG_PIERCING    = 4,
    
    DMG_OTHER       = 128
};

enum ArmorType : s32
{
    ARMOR_UNDEFINED,
    
    ARMOR_LIGHT,
    ARMOR_MEDIUM,
    ARMOR_HEAVY,
    ARMOR_SHIELD,
    
    ARMOR_COUNT
};

struct Armor
{
    utf32     name;
    s32       armorBonus;
    s32       dexMax;    //NOTE: A value of -1 means there is no maximum
    s32       penalty;
    s32       failChance;
    ArmorType type;
};

const s32 armorTableCount = 36;
Armor armorTable[armorTableCount] = {
    {{(u32 *)U"Armatura Cerimoniale di Seta", 28, 28}, +1, -1, 0, 0, ARMOR_LIGHT},
    {{(u32 *)U"Armatura Imbottita", 18, 18},           +1, +8, 0, 5, ARMOR_LIGHT},
    {{(u32 *)U"Haramaki", 8, 8},                       +1, -1, 0, 0, ARMOR_LIGHT},
    {{(u32 *)U"Imbottitura Trapuntata", 22, 22},       +1, +8, 0, 10, ARMOR_LIGHT},
    {{(u32 *)U"Armatura di Cuoio", 17, 17},            +2, +6, 0, 10, ARMOR_LIGHT},
    {{(u32 *)U"Corazza Lamellare", 17, 17},            +2, +4, 0, 5, ARMOR_LIGHT},
    {{(u32 *)U"Armatura da Parata", 18, 18},           +3, +5, -1, 15, ARMOR_LIGHT},
    {{(u32 *)U"Armatura di Cuoio Borchiato", 27, 27},  +3, +5, -1, 15, ARMOR_LIGHT},
    {{(u32 *)U"Corazza di Cuoio Borchiato", 26, 26},   +3, +5, -1, 15, ARMOR_LIGHT},//NOTE: Doppio di Sopra
    {{(u32 *)U"Armatura di Legno", 17, 17},            +3, +3, -1, 15, ARMOR_LIGHT},
    {{(u32 *)U"Armatura Lamellare (Cuoio)", 26, 26},   +4, +3, -2, 20, ARMOR_LIGHT},
    {{(u32 *)U"Giaco di Maglia", 15, 15},              +4, +4, -2, 20, ARMOR_LIGHT},
    
    {{(u32 *)U"Armatura di Pelle", 17, 17},             +4, +4, -3, 20, ARMOR_MEDIUM},
    {{(u32 *)U"Manto Corazzato", 15, 15},               +4, +3, -2, 20, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura Kikko", 14, 14},                +5, +4, -3, 20, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura Lamellare (Corno)", 26, 26},    +5, +3, -4, 25, ARMOR_MEDIUM},
    {{(u32 *)U"Corazza a Scaglie", 17, 17},             +5, +3, -4, 25, ARMOR_MEDIUM},
    {{(u32 *)U"Corazze a Scaglie", 17, 17},             +5, +3, -4, 25, ARMOR_MEDIUM},//NOTE: Doppio di Sopra
    {{(u32 *)U"Do-Maru", 7, 7},                         +5, +4, -4, 25, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura a Dischi", 17, 17},             +6, +2, -5, 30, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura della Montagna", 23, 23},       +6, +3, -4, 30, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura Lamellare (Acciaio)", 28, 28},  +6, +3, -5, 25, ARMOR_MEDIUM},
    {{(u32 *)U"Cotta di Maglia", 15, 15},               +6, +2, -5, 30, ARMOR_MEDIUM},
    {{(u32 *)U"Corazza di Piastre", 18, 18},            +6, +3, -4, 25, ARMOR_MEDIUM},
    {{(u32 *)U"Corazza di Piastre Flessibile", 29, 29}, +6, +3, -4, 25, ARMOR_MEDIUM},
    
    {{(u32 *)U"Armatura Lamellare (Ferro)", 26, 26},  +7, +0, -7, 40, ARMOR_HEAVY},
    {{(u32 *)U"Corazza a Strisce", 17, 17},           +7, +0, -7, 40, ARMOR_HEAVY},
    {{(u32 *)U"Corazza di Bande", 16, 16},            +7, +1, -6, 35, ARMOR_HEAVY},
    {{(u32 *)U"Kusari Gusoku", 13, 13},               +7, +1, -7, 35, ARMOR_HEAVY},
    {{(u32 *)U"Tatami-Do", 9, 9},                     +7, +3, -6, 35, ARMOR_HEAVY},
    {{(u32 *)U"Armatura Lamellare (Pietra)", 27, 27}, +8, +0, -7, 40, ARMOR_HEAVY},
    {{(u32 *)U"Mezza Armatura", 14, 14},              +8, +0, -7, 40, ARMOR_HEAVY},
    {{(u32 *)U"Mezza Armatura Flessibile", 25, 25},   +8, +0, -7, 40, ARMOR_HEAVY},
    {{(u32 *)U"O-Yoroi", 7, 7},                       +8, +2, -6, 35, ARMOR_HEAVY},
    {{(u32 *)U"Armatura Completa", 17, 17},           +9, +1, -6, 35, ARMOR_HEAVY},
    {{(u32 *)U"Corazza di Pietra", 17, 17},           +9, +1, -6, 35, ARMOR_HEAVY},
};

const s32 shieldTableCount = 11;
Armor shieldTable[shieldTableCount] = {
    {{(u32 *)U"Buckler", 7, 7},                           +1, -1, -1, 5, ARMOR_SHIELD},
    {{(u32 *)U"Klar", 4, 4},                              +1, -1, -1, 5, ARMOR_SHIELD},
    {{(u32 *)U"Madu (Cuoio)", 12, 12},                    +1, -1, -2, 5, ARMOR_SHIELD},
    {{(u32 *)U"Madu (Metallo)", 14, 14},                  +1, -1, -2, 5, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Leggero di Legno", 22, 22},          +1, -1, -1, 5, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Leggero di Metallo", 24, 24},        +1, -1, -1, 5, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Rapido Leggero di Legno", 29, 29},   +1, -1, -2, 5, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Rapido Leggero di Metallo", 31, 31}, +1, -1, -2, 5, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Pesante di Legno", 22, 22},          +2, -1, -2, 15, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Pesante di Metallo", 24, 24},        +2, -1, -2, 15, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Torre", 11, 11},                     +4, +2, -10, 50, ARMOR_SHIELD},
};

Armor armorTablePrana[armorTableCount] = {
    {{(u32 *)U"Armatura Cerimoniale di Seta", 28, 28}, +1, -1, 0, 0, ARMOR_LIGHT},
    {{(u32 *)U"Armatura Imbottita", 18, 18},           +2, +8, 0, 5, ARMOR_LIGHT},
    {{(u32 *)U"Haramaki", 8, 8},                       +1, -1, 0, 0, ARMOR_LIGHT},
    {{(u32 *)U"Imbottitura Trapuntata", 22, 22},       +2, +8, 0, 10, ARMOR_LIGHT},
    {{(u32 *)U"Armatura di Cuoio", 17, 17},            +3, +6, 0, 10, ARMOR_LIGHT},
    {{(u32 *)U"Corazza Lamellare", 17, 17},            +3, +4, 0, 5, ARMOR_LIGHT},
    {{(u32 *)U"Armatura da Parata", 18, 18},           +4, +5, -1, 15, ARMOR_LIGHT},
    {{(u32 *)U"Armatura di Cuoio Borchiato", 27, 27},  +4, +5, -1, 15, ARMOR_LIGHT},
    {{(u32 *)U"Corazza di Cuoio Borchiato", 26, 26},   +4, +5, -1, 15, ARMOR_LIGHT},//NOTE: Doppio di Sopra
    {{(u32 *)U"Armatura di Legno", 17, 17},            +4, +3, -1, 15, ARMOR_LIGHT},
    {{(u32 *)U"Armatura Lamellare (Cuoio)", 26, 26},   +5, +3, -2, 20, ARMOR_LIGHT},
    {{(u32 *)U"Giaco di Maglia", 15, 15},              +5, +4, -2, 20, ARMOR_LIGHT},
    
    {{(u32 *)U"Armatura di Pelle", 17, 17},             +6, +4, -3, 20, ARMOR_MEDIUM},
    {{(u32 *)U"Manto Corazzato", 15, 15},               +6, +3, -2, 20, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura Kikko", 14, 14},                +7, +4, -3, 20, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura Lamellare (Corno)", 26, 26},    +7, +3, -4, 25, ARMOR_MEDIUM},
    {{(u32 *)U"Corazza a Scaglie", 17, 17},             +7, +3, -4, 25, ARMOR_MEDIUM},
    {{(u32 *)U"Corazze a Scaglie", 17, 17},             +7, +3, -4, 25, ARMOR_MEDIUM},//NOTE: Doppio di Sopra
    {{(u32 *)U"Do-Maru", 7, 7},                         +7, +4, -4, 25, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura a Dischi", 17, 17},             +8, +2, -5, 30, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura della Montagna", 23, 23},       +8, +3, -4, 30, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura Lamellare (Acciaio)", 28, 28},  +8, +3, -5, 25, ARMOR_MEDIUM},
    {{(u32 *)U"Cotta di Maglia", 15, 15},               +8, +2, -5, 30, ARMOR_MEDIUM},
    {{(u32 *)U"Corazza di Piastre", 18, 18},            +8, +3, -4, 25, ARMOR_MEDIUM},
    {{(u32 *)U"Corazza di Piastre Flessibile", 29, 29}, +8, +3, -4, 25, ARMOR_MEDIUM},
    
    {{(u32 *)U"Armatura Lamellare (Ferro)", 26, 26},  +9, +0, -7, 40, ARMOR_HEAVY},
    {{(u32 *)U"Corazza a Strisce", 17, 17},           +9, +0, -7, 40, ARMOR_HEAVY},
    {{(u32 *)U"Corazza di Bande", 16, 16},            +9, +1, -6, 35, ARMOR_HEAVY},
    {{(u32 *)U"Kusari Gusoku", 13, 13},               +9, +1, -7, 35, ARMOR_HEAVY},
    {{(u32 *)U"Tatami-Do", 9, 9},                     +9, +3, -6, 35, ARMOR_HEAVY},
    {{(u32 *)U"Armatura Lamellare (Pietra)", 27, 27}, +10, +0, -7, 40, ARMOR_HEAVY},
    {{(u32 *)U"Mezza Armatura", 14, 14},              +10, +0, -7, 40, ARMOR_HEAVY},
    {{(u32 *)U"Mezza Armatura Flessibile", 25, 25},   +10, +0, -7, 40, ARMOR_HEAVY},
    {{(u32 *)U"O-Yoroi", 7, 7},                       +11, +2, -6, 35, ARMOR_HEAVY},
    {{(u32 *)U"Armatura Completa", 17, 17},           +11, +1, -6, 35, ARMOR_HEAVY},
    {{(u32 *)U"Corazza di Pietra", 17, 17},           +12, +1, -6, 35, ARMOR_HEAVY},
};

Armor shieldTablePrana[shieldTableCount] = {
    {{(u32 *)U"Buckler", 7, 7},                           +1, -1, -1, 5, ARMOR_SHIELD},
    {{(u32 *)U"Klar", 4, 4},                              +1, -1, -1, 5, ARMOR_SHIELD},
    {{(u32 *)U"Madu (Cuoio)", 12, 12},                    +2, -1, -2, 5, ARMOR_SHIELD},
    {{(u32 *)U"Madu (Metallo)", 14, 14},                  +3, -1, -2, 5, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Leggero di Legno", 22, 22},          +2, -1, -1, 5, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Leggero di Metallo", 24, 24},        +3, -1, -1, 5, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Rapido Leggero di Legno", 29, 29},   +2, -1, -2, 5, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Rapido Leggero di Metallo", 31, 31}, +3, -1, -2, 5, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Pesante di Legno", 22, 22},          +4, -1, -2, 15, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Pesante di Metallo", 24, 24},        +5, -1, -2, 15, ARMOR_SHIELD},
    {{(u32 *)U"Scudo Torre", 11, 11},                     +6, +2, -10, 50, ARMOR_SHIELD},
};

enum RangedWeaponCat
{
    RANGED_CAT_UNDEFINED,
    
    RANGED_CAT_SIMPLE,
    RANGED_CAT_WAR,
    RANGED_CAT_EXOTIC,
    
    RANGED_CAT_COUNT,
};

enum RangedWeaponType
{
    RANGED_UNDEFINED,
    
    RANGED_THROW,
    RANGED_SPREAD,
    RANGED_AIM,
    RANGED_AIM_STR,
    RANGED_AIM_STR_BON,
    
    RANGED_COUNT
};

struct RangedWeapon
{
    utf32            name;
    DiceThrow        smallDmg;
    DiceThrow        mediumDmg;
    CritRange        crit;
    s32              range; //NOTE: In Squares of 1,5m each!
    
    DamageTypeFlag   dmgType;
    RangedWeaponCat  cat;
    RangedWeaponType type;
};

const s32 rangedWeaponCount = 43;
RangedWeapon rangedTable[rangedWeaponCount] = {
    
    //NOTE: Simple Weapons
    { ls_utf32Constant(U"Balestra Leggera"), {1, 6}, {1, 8}, {1, 2}, 16, DMG_PIERCING, RANGED_CAT_SIMPLE, RANGED_AIM },
    { ls_utf32Constant(U"Balestra leggera"), {1, 6}, {1, 8}, {1, 2}, 16, DMG_PIERCING, RANGED_CAT_SIMPLE, RANGED_AIM },
    { ls_utf32Constant(U"Balestra Pesante"), {1, 8}, {1, 10}, {1, 2}, 24, DMG_PIERCING, RANGED_CAT_SIMPLE, RANGED_AIM },
    { ls_utf32Constant(U"Balestra Subacquea Leggera"), {1, 6}, {1, 8}, {1, 2}, 16, DMG_PIERCING, RANGED_CAT_SIMPLE, RANGED_AIM },
    { ls_utf32Constant(U"Balestra Subacquea Pesante"), {1, 8}, {1, 10}, {1, 2}, 24, DMG_PIERCING, RANGED_CAT_SIMPLE, RANGED_AIM },
    { ls_utf32Constant(U"Cerbottana"), {1, 1}, {1, 2}, {0, 2}, 4, DMG_PIERCING, RANGED_CAT_SIMPLE, RANGED_THROW },
    { ls_utf32Constant(U"Dardo"), {1, 3}, {1, 4}, {0, 2}, 4, DMG_PIERCING, RANGED_CAT_SIMPLE, RANGED_THROW },
    { ls_utf32Constant(U"Fionda"), {1, 3}, {1, 4}, {0, 2}, 10, DMG_BLUDGEONING, RANGED_CAT_SIMPLE, RANGED_THROW },
    { ls_utf32Constant(U"Giavellotto"), {1, 4}, {1, 6}, {0, 2}, 6, DMG_PIERCING, RANGED_CAT_SIMPLE, RANGED_THROW },
    { ls_utf32Constant(U"Giavellotti"), {1, 4}, {1, 6}, {0, 2}, 6, DMG_PIERCING, RANGED_CAT_SIMPLE, RANGED_THROW },
    { ls_utf32Constant(U"Lancia"), {1, 6}, {1, 8}, {0, 3}, 4, DMG_PIERCING, RANGED_CAT_SIMPLE, RANGED_THROW },
    { ls_utf32Constant(U"Pugnale"), {1, 3}, {1, 4}, {1, 2}, 2, DMG_PIERCING, RANGED_CAT_SIMPLE, RANGED_THROW },
    
    
    //NOTE: War Weapons
    { ls_utf32Constant(U"Amentum"), {1, 4}, {1, 6}, {0, 2}, 10, DMG_PIERCING, RANGED_CAT_WAR, RANGED_THROW },
    { ls_utf32Constant(U"Arco Corto"), {1, 4}, {1, 6}, {0, 3}, 12, DMG_PIERCING, RANGED_CAT_WAR, RANGED_AIM_STR },
    { ls_utf32Constant(U"Arco corto"), {1, 4}, {1, 6}, {0, 3}, 12, DMG_PIERCING, RANGED_CAT_WAR, RANGED_AIM_STR },
    { ls_utf32Constant(U"Arco Corto Composito"), {1, 4}, {1, 6}, {0, 3}, 14, DMG_PIERCING, RANGED_CAT_WAR, RANGED_AIM_STR_BON },
    { ls_utf32Constant(U"Arco Lungo"), {1, 6}, {1, 8}, {0, 3}, 20, DMG_PIERCING, RANGED_CAT_WAR, RANGED_AIM_STR },
    { ls_utf32Constant(U"Archi Lunghi"), {1, 6}, {1, 8}, {0, 3}, 20, DMG_PIERCING, RANGED_CAT_WAR, RANGED_AIM_STR },
    { ls_utf32Constant(U"Arco Lungo Composito"), {1, 6}, {1, 8}, {0, 3}, 22, DMG_PIERCING, RANGED_CAT_WAR, RANGED_AIM_STR_BON },
    { ls_utf32Constant(U"Chakram (O)"), {1, 6}, {1, 8}, {0, 2}, 6, DMG_SLASHING, RANGED_CAT_WAR, RANGED_THROW },
    { ls_utf32Constant(U"Hunga Munga"), {1, 4}, {1, 6}, {0, 2}, 3, DMG_PIERCING, RANGED_CAT_WAR, RANGED_THROW },
    { ls_utf32Constant(U"Pilum"), {1, 6}, {1, 8}, {0, 2}, 4, DMG_PIERCING, RANGED_CAT_WAR, RANGED_THROW },
    { ls_utf32Constant(U"Propulsore (P)"), {1, 4}, {1, 6}, {0, 2}, 10, DMG_PIERCING, RANGED_CAT_WAR, RANGED_THROW },
    { ls_utf32Constant(U"Tubo della Polvere Velenosa (O)"), {0, 0}, {0, 0}, {0, 0}, 0, DMG_OTHER, RANGED_CAT_WAR, RANGED_UNDEFINED },
    { ls_utf32Constant(U"Tubo Lancia Frecce"), {1, 3}, {1, 4}, {0, 2}, 8, DMG_PIERCING, RANGED_CAT_WAR, RANGED_UNDEFINED },
    { ls_utf32Constant(U"Tridente"), {1, 6}, {1, 8}, {0, 2}, 2, DMG_PIERCING, RANGED_CAT_WAR, RANGED_THROW },
    { ls_utf32Constant(U"Ascia da Lancio"), {1, 4}, {1, 6}, {0, 2}, 2, DMG_SLASHING, RANGED_CAT_WAR, RANGED_THROW },
    { ls_utf32Constant(U"Ascie da Lancio"), {1, 4}, {1, 6}, {0, 2}, 2, DMG_SLASHING, RANGED_CAT_WAR, RANGED_THROW },
    
    //NOTE: Exotic Weapons
    { ls_utf32Constant(U"Balestra a Mano"), {1, 3}, {1, 4}, {1, 2}, 6, DMG_PIERCING, RANGED_CAT_EXOTIC, RANGED_AIM },
    { ls_utf32Constant(U"Balestra a Ripetizione a Mano"), {1, 3}, {1, 4}, {1, 2}, 6, DMG_PIERCING, RANGED_CAT_EXOTIC, RANGED_AIM },
    { ls_utf32Constant(U"Balestra a Ripetizione Leggera"), {1, 6}, {1, 8}, {1, 2}, 16, DMG_PIERCING, RANGED_CAT_EXOTIC, RANGED_AIM },
    { ls_utf32Constant(U"Balestra a Ripetizione Pesante"), {1, 8}, {1, 10}, {1, 2}, 24, DMG_PIERCING, RANGED_CAT_EXOTIC, RANGED_AIM },
    { ls_utf32Constant(U"Balestra Doppia"), {1, 6}, {1, 8}, {1, 2}, 16, DMG_PIERCING, RANGED_CAT_EXOTIC, RANGED_AIM },
    { ls_utf32Constant(U"Bastone Fionda Halfling"), {1, 6}, {1, 8}, {0, 3}, 16, DMG_BLUDGEONING, RANGED_CAT_EXOTIC, RANGED_THROW },
    { ls_utf32Constant(U"Bolas"), {1, 3}, {1, 4}, {0, 2}, 2, DMG_BLUDGEONING, RANGED_CAT_EXOTIC, RANGED_THROW },
    { ls_utf32Constant(U"Boomerang"), {1, 4}, {1, 6}, {0, 2}, 6, DMG_BLUDGEONING, RANGED_CAT_EXOTIC, RANGED_THROW },
    { ls_utf32Constant(U"Kestros"), {1, 6}, {1, 8}, {0, 3}, 10, DMG_PIERCING, RANGED_CAT_EXOTIC, RANGED_THROW },
    { ls_utf32Constant(U"Lazo"), {}, {}, {}, 0, DMG_OTHER, RANGED_CAT_EXOTIC, RANGED_UNDEFINED},
    { ls_utf32Constant(U"Rete"), {}, {}, {}, 2, DMG_OTHER, RANGED_CAT_EXOTIC, RANGED_UNDEFINED},
    { ls_utf32Constant(U"Rete da Cattura"), {}, {}, {}, 2, DMG_PIERCING, RANGED_CAT_EXOTIC, RANGED_UNDEFINED},
    { ls_utf32Constant(U"Scudo da Lancio"), {1, 4}, {1, 6}, {0, 2}, 4, DMG_BLUDGEONING, RANGED_CAT_EXOTIC, RANGED_THROW },
    { ls_utf32Constant(U"Sheng Biao"), {1, 3}, {1, 4}, {0, 2}, 4, DMG_PIERCING, RANGED_CAT_EXOTIC, RANGED_THROW },
    { ls_utf32Constant(U"Shuriken"), {1, 1}, {1, 2}, {0, 2}, 2, DMG_PIERCING, RANGED_CAT_EXOTIC, RANGED_THROW },
};

#endif //_EQUIP_H
