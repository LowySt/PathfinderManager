//
//  Entity.h
//  HyperPCMan
//
//  Created by Lowy on 27/11/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "StatField.h"

NS_ASSUME_NONNULL_BEGIN

#define ST_NUM 3
#define MISC_NUM 3
#define SKILL_MARK_UNSET 99

typedef struct {
    int Bonus[4];
    int num;
} BAB;

typedef struct {
    int Value;
    int AS;
    int Extra;
    
    int markSet;
} Skill;

typedef enum {
    ACROBATICS = 0,
    APPRAISE,
    BLUFF,
    CLIMB,
    CRAFT,
    DIPLOMACY,
    DISABLE_DEVICE,
    DISGUISE,
    ESCAPE_ARTIST,
    FLY,
    HANDLE_ANIMAL,
    HEAL,
    INTIMIDATE,
    INTUITION,
    KNOWLEDGE_ARCANA,
    KNOWLEDGE_DUNGEONEERING,
    KNOWLEDGE_ENGINEERING,
    KNOWLEDGE_GEOGRAPHY,
    KNOWLEDGE_HISTORY,
    KNOWLEDGE_LOCAL,
    KNOWLEDGE_NATURE,
    KNOWLEDGE_NOBILITY,
    KNOWLEDGE_PLANES,
    KNOWLEDGE_RELIGION,
    LINGUISTICS,
    PERCEPTION,
    PERFORM,
    PROFESSION,
    RIDE,
    SLEIGHT_OF_HAND,
    SPELLCRAFT,
    STEALTH,
    SURVIVAL,
    SWIM,
    USE_MAGIC_DEVICE,
    SKILL_NUM
} SkillEnums;

static NSString * _Nonnull __strong miscNamesENG[MISC_NUM] = {
        @"BAB", @"CMB", @"CMD"
};
static NSString * _Nonnull __strong miscNamesIT[MISC_NUM] = {
        @"BAB", @"BMC", @"DMC"
};

static NSString  * _Nonnull __strong SkillNamesENG[SKILL_NUM] = {
        @"Acrobatics", @"Appraise", @"Bluff", @"Climb", @"Craft",
        @"Diplomacy", @"Disable Device*", @"Disguise", @"Escape Artist",
        @"Fly", @"Handle Animal*", @"Heal", @"Intimidate", @"Intuition",
        @"Knowledge(Arcana)*", @"Knowledge(Dungeoneering)*", @"Knowledge(Engineering)*",
        @"Knowledge(Geography)*", @"Knowledge(History)*", @"Knowledge(Local)*",
        @"Knowledge(Nature)*", @"Knowledge(Nobility)*", @"Knowledge(Planes)*",
        @"Knowledge(Religion)*", @"Linguistics*", @"Perception", @"Perform",
        @"Profession*", @"Ride", @"Sleight of Hand*", @"Spellcraft*",
        @"Stealth", @"Survival", @"Swim", @"Use Magic Device*"
};

static NSString * _Nonnull __strong SkillNamesIT[SKILL_NUM] = {
        @"Acrobazia", @"Addestrare Animali*", @"Artigianato", @"Artista Della Fuga",
        @"Camuffare", @"Cavalcare", @"Conoscenze(Arcane)*", @"Conoscenze(Dungeon)*",
        @"Conoscenze(Geografia)*", @"Conoscenze(Ingegneria)*", @"Conoscenze(Locali)*",
        @"Conoscenze(Natura)*", @"Conoscenze(Nobiltà)*", @"Conoscenze(Piani)*",
        @"Conoscenze(Religioni)*", @"Conoscenze(Storia)*", @"Diplomazia",
        @"Disattivare Congegni*", @"Furtività", @"Guarire", @"Intimidire", @"Intrattenere",
        @"Intuizione", @"Linguistica*", @"Nuotare", @"Percezione", @"Professione*",
        @"Raggirare",  @"Rapidità Di Mano*", @"Sapienza Magica*", @"Scalare",
        @"Sopravvivenza", @"Utilizzare Congegni Magici*", @"Valutare", @"Volare"
};

static bool levelRequiredSkillENG[SKILL_NUM] = {
        false, false, false, false, false, false, true, false, false, false, true,
        false, false, false, true, true, true, true, true, true, true, true, true,
        true, true, false, false, true, false, true, true, false, false, false, true
};

static bool levelRequiredSkillIT[SKILL_NUM] = {
        false, true, false, false, false, false, true, true, true, true, true, true,
        true, true, true, true, false, true, false, false, false, false, false, true,
        false, false, true, false, true, true, false, false, true, false, false
};

static int ASBonus[] = {
    -5, -5, -4, -4, -3, -3, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2,
    3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
    12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17
};

static NSString * _Nonnull __strong STNamesENG[ST_NUM] = {@"Fortitude", @"Reflexes", @"Will"};
static NSString * _Nonnull __strong STNamesIT[ST_NUM] = {@"Tempra", @"Riflessi", @"Volontà"};

@interface Entity : NSObject {
    @public NSString *name;
    @public int      Stats[STATS_NUM];
    @public int      SavingThrows[ST_NUM];
    @public BAB      AttackBonus;
    @public int      CMB;
    @public int      CMD;
    @public Skill    Skills[SKILL_NUM];
};

@end

NS_ASSUME_NONNULL_END
