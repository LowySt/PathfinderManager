#ifndef _COMPENDIUM_IMMUNITIES_H
#define _COMPENDIUM_IMMUNITIES_H

const u64 IMM_SENTINEL_VALUE = 0xFFFFFFFFFFFFFFFF;
const s32 IMM_SINGLE_BIT_LEN = 8;
const s32 IMM_MAX_COUNT      = 8;
const u64 IMM_SINGLE_MAX_VAL = 255;

enum ImmunityType
{
    Immunity_Acid, Immunity_Fire, Immunity_Cold, Immunity_Elec, Immunity_Sound,
    Immunity_EnergyNegative, Immunity_EnergyPositive, Immunity_InfernalFire,
    Immunity_AcidAndFire, Immunity_FireAndCold, Immunity_ElecAndFire, Immunity_ColdAndElec,
    
    Immunity_Paralysis, Immunity_Sleep, Immunity_Fatigued, Immunity_Exhausted, 
    Immunity_Fear, Immunity_Sickened, Immunity_Nauseated, Immunity_Petrified,
    Immunity_Blind, Immunity_Stun, Immunity_Confusion, Immunity_Dazed,
    Immunity_Staggered, Immunity_Dazzled, Immunity_Deaf, Immunity_BlindAndDeaf,
    Immunity_DazzledAndPetrified, Immunity_Shaken, Immunity_Frightened,
    
    Immunity_Poison, Immunity_PoisonIngested, Immunity_Disease, Immunity_Magic,
    Immunity_Criticals, Immunity_SneakAtk, Immunity_GazeAtk, Immunity_WeaponDmg,
    Immunity_Curse, Immunity_Bleeding, Immunity_Bludgeoning, Immunity_FallAndCrush,
    Immunity_SmellAtk, Immunity_Precision, Immunity_PiercingAndSlashing, Immunity_BludgeoningAndPiercing,
    Immunity_Flanking, Immunity_NonLethalDamage, Immunity_Warpproof, Immunity_Piercing, Immunity_Slashing,
    Immunity_Divination, Immunity_PermanentWounds, Immunity_DiseaseAndPoison, Immunity_FatiguedAndExhausted,
    Immunity_DazedAndStun, Immunity_ParalysisAndPetrified, Immunity_BleedingAndPermanentWounds,
    Immunity_SleepAndStun, Immunity_PrecisionAndCriticals, Immunity_BloodDrainAndBleeding,
    Immunity_BloodDrainAndBleedingAndPoison, Immunity_Pressure,
    
    Immunity_MindEffects, Immunity_DeathEffects, Immunity_EmotionEffects, Immunity_GazeEffects,
    Immunity_ForceEffects, Immunity_GazeAndIllusionEffects, Immunity_CharmEffects,
    Immunity_CompulsionEffect, Immunity_MetamorphosisEffects, Immunity_MindRegressionEffects,
    Immunity_CreatureCountEffect, Immunity_MindEffectsAndGaze, Immunity_CompletionAndActivationSpellEffects,
    Immunity_SensesEffects, Immunity_FearAndMindEffects, Immunity_IllusionEffects, 
    Immunity_PainEffects, Immunity_ConfusionCrazyEffects, Immunity_AgingEffects,
    Immunity_ConfusionAndRegressionEffects, Immunity_Possession, Immunity_Rust, Immunity_IllusionSightEffects,
    Immunity_LightEffects, Immunity_CharmAndCompulsionEffect, Immunity_NecromancyEffects,
    Immunity_DeathAndAgingEffects, Immunity_NecromancyAndDeathEffects,
    
    Immunity_CharmeAndIllusionSpells, Immunity_RecallSpells, Immunity_DetectThoughtSpell,
    Immunity_DetectAlignmentSpell, Immunity_DetectLiesSpell, Immunity_SpellImmunitySpell,
    Immunity_DimensionalAnchorSpell, Immunity_CharmeMonsterSpell, Immunity_LightningBoltSpell,
    Immunity_FireballSpell, Immunity_SleepSpell,
    
    Immunity_ASDrain, Immunity_ASDmg, Immunity_LevelDrain, Immunity_ASDmgAndDrain,
    Immunity_LevelDrainAndASDmg, Immunity_LevelDrainAndASDmgAndDrain,
    
    Immunity_UndeadTraits, Immunity_OozeTraits, Immunity_SwarmTraits, Immunity_ConstructTraits,
    Immunity_PlantTraits, Immunity_ElementalTraits, Immunity_DragonTraits,
    
    Immunity_MagicMissile, Immunity_StyxWater, Immunity_CalmEmotions, Immunity_Convocation,
    Immunity_RuneMastery, Immunity_MagicControl, Immunity_Slowness, Immunity_TurnUndead,
    Immunity_EnergySpells, Immunity_Annihilation, Immunity_PlaneShift, Immunity_ChannelEnergyNonMithic,
    Immunity_Labyrinth, Immunity_MemoryLoss, Immunity_BloodDrain, Immunity_LethalGas,
    
    Immunity_ProtectFromEnergy,
    
    Immunity_MaxCount,
    
    //NOTE: This way, when all immunities are invalid, the entire packed value is invalid and viceversa
    Immunity_Invalid = 0xFF
};

const utf32 immunityToString[Immunity_MaxCount] = {
    U"Acido"_W, U"Fuoco"_W, U"Freddo"_W, U"Elettricit\U000000E0"_W, U"Sonoro"_W,
    U"Energia Negativa"_W, U"Energia Positiva"_W, U"Fuoco Infernale"_W,
    U"Acido, Fuoco"_W, U"Fuoco, Freddo"_W, U"Elettricit\U000000E0, Fuoco"_W, U"Elettricit\U000000E0, Freddo"_W,
    
    U"Paralisi"_W, U"Sonno"_W, U"Affaticamento"_W, U"Esaurimento"_W, U"Paura"_W, U"Infermit\U000000E0"_W,
    U"Nausea"_W, U"Pietrificazione"_W, U"Cecit\U000000E0"_W, U"Stordimento"_W, U"Confusione"_W, U"Frastornamento"_W,
    U"Barcollare"_W, U"Abbagliamento"_W, U"Sordit\U000000E0"_W, U"Cecit\U000000E0, Sordit\U000000E0"_W,
    U"Abbagliamento, Pietrificazione"_W, U"Scosso"_W, U"Spaventato"_W,
    
    U"Veleni"_W, U"Veleni ad Ingestione"_W, U"Malattie"_W, U"Magia"_W, U"Colpi Critici"_W, U"Attacchi Furtivi"_W,
    U"Sguardo"_W, U"Danno da Armi"_W, U"Maledizioni"_W, U"Sanguinamento"_W, U"Danno Contundente"_W,
    U"Danno da Caduta e Schiacciamento"_W, U"Attacchi basati sull'Olfatto"_W, U"Danni da Precisione"_W,
    U"Danni Perforanti e Taglienti"_W, U"Danni Contundenti e Perforanti"_W, U"Fiancheggiamento"_W,
    U"Danno Non Letale"_W, U"Non Deformante"_W, U"Danno Perforante"_W, U"Danno Tagliente"_W,
    U"Divinazioni"_W, U"Ferite Permanenti"_W, U"Malattie, Veleni"_W,
    U"Affaticamento, Esaurimento"_W, U"Frastornamento, Stordimento"_W, U"Paralisi, Pietrificazione"_W,
    U"Sanguinamento, Ferite Permanenti"_W, U"Sonno, Stordimento"_W, U"Danni da Precisione, Colpi Critici"_W,
    U"Risucchi di Sangue, Sanguinamento"_W, U"Risucchi di Sangue, Sanguinamento, Veleni"_W, U"Danno da Pressione"_W, 
    
    U"Effetti di Influenza Mentale"_W, U"Effetti di Morte"_W, U"Effetti di Emozione"_W,
    U"Effetti basati sulla Vista"_W, U"Effetti di Forza"_W, U"Effeti Visivi ed Illusioni"_W, U"Effetti di Charme"_W,
    U"Effetti di Compulsione"_W, U"Effetti di Metamorfosi"_W, U"Effetti di Demenza"_W,
    U"Effetti che bersagliano uno specifico numero di creature"_W, U"Effetti di Influenza Mentale e Visivi"_W,
    U"Effetti a Completamento di Incantesimo e ad Attivazione di Incantesimo"_W, U"Effetti basati sui Sensi"_W,
    U"Effetti di Paura ed Influenza Mentale"_W, U"Effetti di Illusione"_W, U"Effetti di Dolore"_W,
    U"Effetti di Confusione e Follia"_W, U"Invecchiamento"_W,
    U"Effetti di Confusione e Demenza"_W, U"Possessione"_W, U"Ruggine"_W,
    U"Effetti di Illusioni Visive"_W, U"Effetti di Luce"_W, U"Effetti di Charm e Compulsione"_W,
    U"Effetti di Necromanzia"_W, U"Effetti di Morte, Invecchiamento"_W, U"Effetti di Necromanzia e Morte"_W,
    
    U"Incantesimi di Charme e Illusioni"_W, U"Incantesimi di Richiamo"_W, U"Individuazione dei Pensieri"_W,
    U"Individuazione dell'Allineamento"_W, U"Rivela Bugie"_W, U"Immunit\U000000E0 agli Incantesimi"_W,
    U"Ancora Dimensionale"_W, U"Charme sui Mostri"_W, U"Fulmine"_W, U"Palla di Fuoco"_W, U"Incantesimo Sonno"_W,
    
    U"Risucchio di Caratteristiche"_W, U"Danni alle Caratteristiche"_W, U"Risucchio di Energia"_W, 
    U"Danni e Risucchio di Caratteristiche"_W, U"Risucchio di Energia, Danni alle Caratteristiche"_W,
    U"Risucchio di Energia, Danni e Risucchio di Caratteristiche"_W,
    
    U"Tratti dei Non Morti"_W, U"Tratti delle Melme"_W, U"Tratti degli Sciami"_W, U"Tratti dei Costrutti"_W,
    U"Tratti dei Vegetali"_W, U"Tratti degli Elementali"_W, U"Tratti dei Draghi"_W,
    
    U"Dardo Incantato"_W, U"Acque del Fiume Stige"_W, U"Calmare Emozioni"_W, U"Convocazione"_W,
    U"Padronanza delle Rune"_W, U"Controllo Magico"_W, U"Lentezza"_W, U"Scacciare Non Morti"_W,
    U"Incantesimi di Energia"_W, U"Annichilazione"_W, U"Spostamento Planare"_W,
    U"Incanalare Energia da Fonti Non Mitiche"_W, U"Labirinto"_W, U"Perdita di Memoria"_W,
    U"Risucchi di Sangue"_W, U"Gas e Vapori Letali"_W,
    
    //NOTE: Protect From Energy.
    // I decided to remove it's string, because it shouldn't be in immunities according to me.
    // It's based on a spell which doesn't give full immunity and isn't certain would be used.
    U""_W,
};

void BuildImmunityFromPacked_t(u64 val, utf32 *);

#endif //_COMPENDIUM_IMMUNITIES_H
