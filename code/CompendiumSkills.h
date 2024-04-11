#ifndef _COMPENDIUM_SKILLS_H
#define _COMPENDIUM_SKILLS_H

//NOTE: Here's the bit mapping of the u32 Skill Entry
//
//  [31]   [30]    [29..7]  [6..0]
//    ^      ^        ^       ^
//    |      |        |     The first 7 bits indicate the Skill Type
//    |      |        |
//    |      |    Skill Value as a Signed Integer
//    |      |
//    |   Paren Bit
//    |        
//  Unused
//
// Bit 30 is the Paren Bit, which indicates wether we are inside of parenthes and need to concat multiple skills
// Bit 31 is currently unused, since no interning was necessary to include any possible skill.

enum SkillType
{
    Acrobazia,
    Saltare,
    Saltare2,
    
    Addestrare,
    Artigianato,
    
    Alchimia,
    Calligrafia,
    Carpenteria,
    Ceramica,
    CostruireArchi,
    CostruireNavi,
    CostruireTrappole,
    CostruireTrappole2,
    FabbricareArmature,
    FabbricareArmi,
    Ferramenta,
    IntessereCeste,
    LavorarePellami,
    LavoriInMuratura,
    Metallurgia,
    Pittura,
    Oreficeria,
    RilegareLibri,
    RiparareScarpe,
    Scultura,
    Tessitura,
    
    Artista,
    Camuffare,
    Cavalcare,
    Conoscenze,
    
    Conoscenze_Arcane,
    Conoscenze_Arcane2,
    Conoscenze_Dungeon,
    Conoscenze_Geografia,
    Conoscenze_Ingegneria,
    Conoscenze_Locali,
    Conoscenze_Locali2,
    Conoscenze_Natura,
    Conoscenze_Nobilt,
    Conoscenze_Piani,
    Conoscenze_Religioni,
    Conoscenze_Religioni2,
    Conoscenze_Storia,
    
    Diplomazia,
    Disattivare,
    Furtivit,
    Guarire,
    Intimidire,
    
    Intrattenere,
    
    Canto,
    Cantare,
    Commedia,
    Danza,
    Oratoria,
    Recitazione,
    Strumenti_Corda,
    Strumenti_Fiato,
    Strumenti_Percussione,
    Strumenti_Tastiera,
    
    Intuizione,
    Linguistica,
    Nuotare,
    Percezione,
    
    Professione,
    
    Allevatore,
    Architetto,
    Avvocato,
    Barcaiolo,
    Bibliotecario,
    Birraio,
    Boscaiolo,
    Cacciatore,
    Carovaniere,
    Conciatore,
    Conestabile,
    Contabile,
    Contadino,
    Cortigiano,
    Cuoco,
    Erborista,
    Farmacista,
    Fornaio,
    Giardiniere,
    Giocatore_Azzardo,
    Guida,
    Ingegnere,
    Levatrice,
    Locandiere,
    Macellaio,
    Marinaio,
    Mercante,
    Minatore,
    Mugnaio,
    Pastore,
    Pescatore,
    Scrivano,
    Siniscalco,
    Soldato,
    Stalliere,
    Taglialegna,
    
    Raggirare,
    Rapidit,
    Sapienza,
    Scalare,
    Sopravvivenza,
    
    SeguireTracce,
    
    Utilizzare,
    Valutare,
    Volare,
    
    Acqua,
    Foreste,
    Paludi,
    
    Ascoltare,
    Individuare,
    
    Tutte,
    Qualsiasi,
    UnaQualsiasi,
    UnoQualsiasi,
    
    SkillTypeCount
};

const u32 SKILL_BITS = 7;
const u32 SKILL_TYPE_MASK = 0x7F;
const s32 SKILL_SENTINEL_VALUE = -127;

const char32_t *SkillTypeToName[SkillType::SkillTypeCount] = {
    U"Acrobazia", U"Saltare", U"Saltare", U"Addestrare Animali", U"Artigianato", U"Alchimia", U"Calligrafia",
    U"Carpenteria", U"Ceramica", U"Costruire Archi", U"Costruire Navi", U"Costruire Trappole", U"Costruire Trappole",
    U"Fabbricare Armature", U"Fabbricare Armi", U"Ferramenta", U"Intessere Ceste", U"Lavorare Pellami",
    U"Lavori in Muratura", U"Metallurgia", U"Pittura", U"Oreficeria", U"Rilegare Libri", U"Riparare Scarpe",
    U"Scultura", U"Tessitura", U"Artista della Fuga", U"Camuffare", U"Cavalcare", U"Conoscenze",
    U"Arcane", U"Arcane", U"Dungeon", U"Geografia", U"Ingegneria", U"Locali",
    U"Locali", U"Natura", U"Nobilt\U000000E0", U"Piani", U"Religioni", U"Religioni",
    U"Storia", U"Diplomazia", U"Disattivare Congegni", U"Furtivit\U000000E0", U"Guarire", U"Intimidire",
    U"Intrattenere", U"Canto", U"Canto", U"Commedia", U"Danza", U"Oratoria",
    U"Recitazione", U"Strumenti a Corda", U"Strumenti a Fiato", U"Strumenti a Percussione", 
    U"Strumenti a Tastiera", U"Intuizione", U"Linguistica", U"Nuotare", U"Percezione", U"Professione", 
    U"Allevatore", U"Architetto", U"Avvocato", U"Barcaiolo", U"Bibliotecario", U"Birraio", U"Boscaiolo", 
    U"Cacciatore", U"Carovaniere", U"Conciatore", U"Conestabile", U"Contabile", U"Contadino", U"Cortigiano",
    U"Cuoco", U"Erborista", U"Farmacista", U"Fornaio", U"Giardiniere", U"Giocatore d'Azzardo",
    U"Guida", U"Ingegnere", U"Levatrice", U"Locandiere", U"Macellaio", U"Marinaio",
    U"Mercante", U"Minatore", U"Mugnaio", U"Pastore", U"Pescatore", U"Scrivano",
    U"Siniscalco", U"Soldato", U"Stalliere", U"Taglialegna", U"Raggirare", U"Rapidit\U000000E0 di Mano",
    U"Sapienza Magica", U"Scalare", U"Sopravvivenza", U"seguire tracce", U"Utilizzare Congegni Magici", U"Valutare",
    U"Volare", U"sott'acqua", U"nelle foreste", U"nelle paludi", U"Ascoltare", U"Individuare",
    U"tutte", U"qualsiasi", U"una qualsiasi", U"uno qualsiasi"
};

s32 BuildSkillFromPacked_t(u32 *entries, s32 index, utf32 *tmp);

#endif //_COMPENDIUM_SKILLS_H
