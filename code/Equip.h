#ifndef _EQUIP_H
#define _EQUIP_H

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

const s32 armorTableCount = 45;
Armor armorTable[armorTableCount] = {
    {{(u32 *)U"Armatura Cerimoniale di Seta", 28, 28}, +1, -1, 0, 0, ARMOR_LIGHT},
    {{(u32 *)U"Armatura Imbottita", 18, 18},           +1, +8, 0, 5, ARMOR_LIGHT},
    {{(u32 *)U"Haramaki", 8, 8},                       +1, -1, 0, 0, ARMOR_LIGHT},
    {{(u32 *)U"Imbottitura Trapuntata", 22, 22},       +1, +8, 0, 10, ARMOR_LIGHT},
    {{(u32 *)U"Armatura di Cuoio", 17, 17},            +2, +6, 0, 10, ARMOR_LIGHT},
    {{(u32 *)U"Corazza Lamellare", 17, 17},            +2, +4, 0, 5, ARMOR_LIGHT},
    {{(u32 *)U"Armatura da Parata", 18, 18},           +3, +5, -1, 15, ARMOR_LIGHT},
    {{(u32 *)U"Armatura di Cuoio Borchiato", 27, 27},  +3, +5, -1, 15, ARMOR_LIGHT},
    {{(u32 *)U"Armatura di Legno", 17, 17},            +3, +3, -1, 15, ARMOR_LIGHT},
    {{(u32 *)U"Armatura Lamellare (Cuoio)", 26, 26},   +4, +3, -2, 20, ARMOR_LIGHT},
    {{(u32 *)U"Giaco di Maglia", 15, 15},              +4, +4, -2, 20, ARMOR_LIGHT},
    
    {{(u32 *)U"Armatura di Pelle", 17, 17},             +4, +4, -3, 20, ARMOR_MEDIUM},
    {{(u32 *)U"Manto Corazzato", 15, 15},               +4, +3, -2, 20, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura Kikko", 14, 14},                +5, +4, -3, 20, ARMOR_MEDIUM},
    {{(u32 *)U"Armatura Lamellare (Corno)", 26, 26},    +5, +3, -4, 25, ARMOR_MEDIUM},
    {{(u32 *)U"Corazza a Scaglie", 17, 17},             +5, +3, -4, 25, ARMOR_MEDIUM},
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

#endif //_EQUIP_H
