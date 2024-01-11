#ifndef _COMPENDIUM_H_P_H
#define _COMPENDIUM_H_P_H

//NOTE: Here's the bit mapping of the u64 HP
//
// [59..54]  [53..47]  [46..36]  [35..27]  [26..18]  [17..9]  [8..0]
//    ^         ^         ^         ^         ^         ^       ^
//    |         |         |         |         |         |     HD (6bit num, 3bit face)
//    |         |         |         |         |         |
//    |         |         |         |         |       HD (6bit num, 3bit face)
//    |         |         |         |         |
//    |         |         |         |       HD (6bit num, 3bit face)
//    |         |         |         |
//    |         |         |       HD (6bit num, 3bit face)
//    |         |         |
//    |         |       Flat value added to life
//    |         |
//    |       Option Type
//    |
//  Option Value
//
// Bit 63 is negative flat value
// Bit 62 says the creature has a barbarian `Ira`
//
// The 2 bits [61] [60] are currently unused

const s32 HP_MAX_DICE_COUNT     = 4;
const s32 HP_DIE_BITLEN         = 9;
const u64 HP_DIE_MASK           = 0x00000000000001FF;
const s32 HP_DIE_FACE_BITLEN    = 3;
const s32 HP_DIE_COUNT_BITLEN   = 6;
const u64 HP_DIE_COUNT_MASK     = 0x00000000000001FB;
const u64 HP_DIE_FACE_MASK      = 0x0000000000000007;

const s32 HP_OPTION_VAL_MAX     = 64;
const s32 HP_FLAT_OFFSET        = 36;
const u64 HP_FLAT_MASK          = 0x00007FF000000000;
const u64 HP_OPTION_TYPE_MASK   = 0x003F800000000000;
const s32 HP_OPTION_TYPE_OFFSET = 47;
const u64 HP_OPTION_VAL_MASK    = 0x0FC0000000000000;
const s32 HP_OPTION_VAL_OFFSET  = 54;

const u64 HP_FLAT_NEGATIVE_BIT  = 0x8000000000000000;
const u64 HP_IRA_BIT            = 0x4000000000000000;

const u64 HP_INVALID            = 0xFFFFFFFFFFFFFFFF;

enum HP_Die_Face
{
    HP_Die_Invalid = 0,
    
    HP_Die_D4 = 1,
    HP_Die_D6 = 2,
    HP_Die_D8 = 3,
    HP_Die_D10 = 4,
    HP_Die_D12 = 5,
    
    HP_Die_Face_Count,
};

const u16 dieFacesToInt[HP_Die_Face_Count] = {
    0,
    4, 6, 8, 10, 12,
};

const utf32 dieFaces[HP_Die_Face_Count] = {
    U"Die_Face_Invalid"_W,
    U"d4"_W, U"d6"_W, U"d8"_W, U"d10"_W, U"d12"_W
};

enum HP_Options_Type
{
    HP_Invalid = 0,
    
    HP_Guarigione_Rapida,
    HP_Guarigione_Rapida_Ghiaccio_Neve,
    HP_Guarigione_Rapida_Fuoco,
    HP_Guarigione_Rapida_Magma_Lava,
    HP_Guarigione_Rapida_Aridi,
    HP_Guarigione_Rapida_Acqua_Bollente_Vapore,
    HP_Guarigione_Rapida_Sotto_Acqua,
    HP_Guarigione_Rapida_Acqua_Salata,
    HP_Guarigione_Rapida_Tempesta_Vento,
    HP_Guarigione_Rapida_Umidi_Fangosi,
    HP_Guarigione_Rapida_Polverosi,
    HP_Guarigione_Rapida_Sotto_Terra,
    HP_Guarigione_Rapida_Ombre,
    
    HP_Guarigione_Rapida_Planare,
    
    HP_Guarigione_Sangue,
    
    HP_Rigenerazione,
    HP_Rigenerazione_Fuoco,
    HP_Rigenerazione_Elettricita,
    HP_Rigenerazione_Acido,
    HP_Rigenerazione_Freddo,
    HP_Rigenerazione_Terra,
    HP_Rigenerazione_Aria,
    HP_Rigenerazione_Acido_Mitico,
    HP_Rigenerazione_Acido_Sonoro,
    HP_Rigenerazione_Acido_Fuoco,
    HP_Rigenerazione_Acido_Fuoco_In_Acqua,
    HP_Rigenerazione_Acido_Freddo,
    HP_Rigenerazione_Acido_Freddo_Fuoco,
    HP_Rigenerazione_Contundente_Fuoco,
    HP_Rigenerazione_Terreno_Naturale,
    HP_Rigenerazione_Caos,
    HP_Rigenerazione_Caos_Magia,
    HP_Rigenerazione_Fuoco_Bene,
    HP_Rigenerazione_Bene,
    HP_Rigenerazione_Bene_Argento,
    HP_Rigenerazione_Bene_Ruggine,
    HP_Rigenerazione_Male,
    HP_Rigenerazione_Legale,
    HP_Rigenerazione_Mitico,
    HP_Rigenerazione_Deifico_Mitico,
    HP_Rigenerazione_Epico,
    HP_Rigenerazione_Bene_Epico_Mitico,
    HP_Rigenerazione_Bene_Deifico_Mitico,
    HP_Rigenerazione_Energia_Positiva,
    HP_Rigenerazione_Energia_Positiva_Ispirazione,
    HP_Rigenerazione_Energia_Negativa,
    HP_Rigenerazione_Freddo_Male,
    HP_Rigenerazione_Ferro_Freddo,
    HP_Rigenerazione_Adamantio,
    HP_Rigenerazione_No_Armi,
    HP_Rigenerazione_Armi_Maledette,
    HP_Rigenerazione_Armi_Epiche,
    HP_Rigenerazione_Pietrificazione,
    HP_Rigenerazione_Statuetta,
    HP_Rigenerazione_Artefatti_Malvagi,
    HP_Rigenerazione_Speciale,
    
    HP_Regen_Pazuzu,
    
    HP_Rigenerazione_Cera_Fuoco,
    
    HP_Rigenerazione_Pluviale_Acido_Fuoco,
    
    HP_Rigenerazione_Profana_Bene_Elettricita,
    
    HP_Ripararsi_Con_Rifiuti,
    
    HP_Terribile_Ringiovanimento,
    
    HP_Campo_Forza,
    
    HP_Vita_Falsata,
    
    HP_Raccolto_Dolore,
    
    HP_Divorare_Anima,
    
    HP_Estorsione,
    
    HP_Gioia_Sostenente,
    
    HP_Option_Type_Count,
};

const utf32 HpTypeToString[HP_Option_Type_Count] = {
    U"HP_Option_Type_Invalid"_W,
    
    U""_W, //Guarigione Rapida
    
    U"(ghiaccio, neve)"_W,
    U"(fuoco)"_W,
    U"(magma, lava)"_W,
    U"(ambiente arido)"_W,
    U"(acqua bollente, vapore)"_W,
    U"(sott'acqua)"_W,
    U"(acqua salata)"_W,
    U"(ambiente tempestoso, ventoso)"_W,
    U"(ambiente umido, fangoso)"_W,
    U"(ambiente polveroso)"_W,
    U"(sotto terra)"_W,
    U"(nelle ombre)"_W,
    
    U"Guarigione Rapida Planare "_W,
    U"Guarigione dal Sangue"_W,
    
    U""_W, //Rigenerazione
    
    U"fuoco"_W,
    U"elettricit\U000000E0"_W,
    U"acido"_W,
    U"freddo"_W,
    U"terra"_W,
    U"aria"_W,
    U"acido mitico"_W,
    U"acido e sonoro"_W,
    U"acido e fuoco"_W,
    U"acido e fuoco (solo a contatto con l'acqua)"_W,
    U"acido e freddo"_W,
    U"acido, freddo e fuoco"_W,
    U"contundente e fuoco"_W,
    U"terreno naturale"_W,
    U"caos"_W,
    U"caos e magia"_W,
    U"bene e fuoco"_W,
    U"bene"_W,
    U"argento e bene"_W,
    U"bene e ruggine"_W,
    U"male"_W,
    U"legale"_W,
    U"mitico"_W,
    U"deifico e mitico"_W,
    U"epico"_W,
    U"bene, epico e mitico"_W,
    U"bene, deifico e mitico"_W,
    U"energia positiva"_W,
    U"energia positiva e ispirazione"_W,
    U"energia negativa"_W,
    U"freddo e male"_W,
    U"ferro freddo"_W,
    U"adamantio"_W,
    U"colpo senz'armi e armi naturali"_W,
    U"armi maledette"_W,
    U"armi epiche"_W,
    U"pietrificazione"_W,
    U"nella statuetta"_W,
    U"artefatti malvagi"_W,
    U"speciale"_W,
    
    U"Rigenerazione (deifico o mitico)"_W,
    
    U"fuoco"_W,
    U"acido e fuoco"_W,
    U"bene e elettricit\U000000E0"_W,
    U"Ripararsi con i Rifiuti "_W,
    U"Terribile Ringiovanimento "_W,
    U""_W, //Campo di Forza
    U" Vita Falsata)"_W,
    
    U"Raccolto di Dolore"_W,
    
    U" (Divorare Anima)"_W,
    U"Estorsione"_W,
    U"Gioia Sostenente "_W,
};

void BuildHPFromPacked_t(CachedPageEntry *page, u64 entry, s32 totalHP);

#endif //_COMPENDIUM_H_P_H
