/* date = May 22nd 2020 11:51 am */

#ifndef _INIT_H
#define _INIT_H

const u32 PARTY_NUM = 5;
const char *PartyName[PARTY_NUM] = { "Lumbar", "Mogora", "Refonil", "Armitage", "Fanny" };

const u32 MOB_NUM = 24;
const char *MobName[MOB_NUM] = { "Enemy 1", "Enemy 2", "Enemy 3", "Enemy 4",
    "Enemy 5", "Enemy 6", "Enemy 7", "Enemy 8", "Enemy 9", "Enemy 10", "Enemy 11", "Enemy 12",
    "Enemy 13", "Enemy 14", "Enemy 15", "Enemy 16", "Enemy 17", "Enemy 18", "Enemy 19", 
    "Enemy 20", "Enemy 21", "Enemy 22", "Enemy 23", "Enemy 24", };

const char *Enemies[MOB_NUM + 1] = {
    "No Enemies", "1 Enemy", "2 Enemies", "3 Enemies", "4 Enemies", "5 Enemies", "6 Enemies",
    "7 Enemies", "8 Enemies", "9 Enemies", "10 Enemies", "11 Enemies", "12 Enemies",
    "13 Enemies", "14 Enemies", "15 Enemies", "16 Enemies", "17 Enemies", "18 Enemies", "19 Enemies",
    "20 Enemies", "21 Enemies", "22 Enemies", "23 Enemies", "24 Enemies"
};


const u32 ALLY_NUM = 6;
const char *AllyName[ALLY_NUM] = { "Ally 1", "Ally 2", "Ally 3", "Ally 4", "Ally 5", "Ally 6" };

const char *Allies[ALLY_NUM + 1] = {
    "No Allies", "1 Ally", "2 Allies", "3 Allies", "4 Allies", "5 Allies", "6 Allies"
};

const u32 ORDER_NUM = PARTY_NUM + MOB_NUM + ALLY_NUM;


const u32 COUNTER_NUM = 8;
const char *CounterNames[COUNTER_NUM] = { "Counter 1", "Counter 2", "Counter 3", "Counter 4", 
    "Counter 5", "Counter 6", "Counter 7", "Counter 8" };

struct tmp_order {
    s32 init;
    HWND nameBox;
    u32 fieldId;
    b32 isParty;
    b32 isMob;
};

struct InitField;
InitField *getInitById(InitField *f, u32 fieldSize, u64 id);
InitField *getInitByHWND(HWND handle);

#endif //_INIT_H
