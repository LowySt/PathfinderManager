/* date = May 22nd 2020 11:51 am */

#ifndef _INIT_H
#define _INIT_H

const u32 PARTY_NUM = 5;
char *PartyName[PARTY_NUM] = { "Kenshiro", "Nathaniel", "Refonil", "Armitage", "Fanny" };

const u32 MOB_NUM = 16;
char *MobName[MOB_NUM] = { "Enemy 1", "Enemy 2", "Enemy 3", "Enemy 4",
    "Enemy 5", "Enemy 6", "Enemy 7", "Enemy 8", "Enemy 9", "Enemy 10", "Enemy 11", "Enemy 12",
    "Enemy 13", "Enemy 14", "Enemy 15", "Enemy 16" };

char *Enemies[MOB_NUM + 1] = {
    "No Enemies", "1 Enemy", "2 Enemies", "3 Enemies", "4 Enemies", "5 Enemies", "6 Enemies",
    "7 Enemies", "8 Enemies", "9 Enemies", "10 Enemies", "11 Enemies", "12 Enemies",
    "13 Enemies", "14 Enemies", "15 Enemies", "16 Enemies"
};


const u32 ALLY_NUM = 4;
char *AllyName[ALLY_NUM] = { "Ally 1", "Ally 2", "Ally 3", "Ally 4" };

char *Allies[ALLY_NUM + 1] = {
    "No Allies", "1 Ally", "2 Allies", "3 Allies", "4 Allies"
};

const u32 ORDER_NUM = PARTY_NUM + MOB_NUM + ALLY_NUM;


struct tmp_order {
    s32 init;
    HWND nameBox;
    u32 fieldId;
    b32 isParty;
    b32 isMob;
};

struct InitField;
InitField *getInitById(InitField *f, u32 fieldSize, u64 id);

#endif //_INIT_H
