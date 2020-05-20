const u32 PARTY_NUM = 5;
char *PartyName[PARTY_NUM] = { "Player 1", "Player 2", "Player 3", "Player 4", "Player 5" };

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