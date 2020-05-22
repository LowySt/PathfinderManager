const u32 PARTY_NUM = 5;
char *PartyName[PARTY_NUM] = { "Kenshiro", "Nathaniel", "Player 3", "Player 4", "Player 5" };

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
};

static s32 order_partition(tmp_order *a, s32 low, s32 high)
{
    // pivot (Element to be placed at right position)
    s32 pivot = a[high].init;
    
    // Index of smaller element
    s32 i = (low - 1);
    
    for (u32 j = low; j <= high - 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (a[j].init <= pivot)
        {
            i++;    // increment index of smaller element
            tmp_order oldI = a[i];
            a[i] = a[j];
            a[j] = oldI;
        }
    }
    
    tmp_order oldV = a[i+1];
    a[i+1] = a[high];
    a[high] = oldV;
    
    return (i + 1);
}

static void order_helper(tmp_order *a, s32 low, s32 high)
{
    if (low < high)
    {
        /* pi is partitioning index, a[pi] is now
        at right place */
        u32 pi = order_partition(a, low, high);
        
        order_helper(a, low, pi - 1);  // Before pi
        order_helper(a, pi + 1, high); // After pi
    }
}

void order_ascending(tmp_order *ord, u32 size)
{
    order_helper(ord, 0, ((s32)size)-1);
}