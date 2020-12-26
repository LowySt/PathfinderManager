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

inline InitField *getInitById(InitField *f, u32 fieldSize, u64 id)
{
    for(u32 i = 0; i < fieldSize; i++) { if(f[i].id == id) { return &f[i]; } }
    return 0x0;
}

inline InitField *getInitByHWND(HWND handle)
{
    if(State.isInitialized == FALSE) { return 0x0; }
    
    //NOTETODO: Is this what I want for InitField selection?
    InitPage *InitPage = State.Init;
    
    for(u32 i = 0; i < MOB_NUM; i++)
    {
        if(InitPage->MobFields[i].Name->box == handle)
        { return &InitPage->MobFields[i]; }
        if(InitPage->MobFields[i].Name->label == handle)
        { return &InitPage->MobFields[i]; }
        if(InitPage->MobFields[i].Bonus->box == handle)
        { return &InitPage->MobFields[i]; }
        if(InitPage->MobFields[i].Bonus->label == handle)
        { return &InitPage->MobFields[i]; }
        if(InitPage->MobFields[i].Final->box == handle)
        { return &InitPage->MobFields[i]; }
        if(InitPage->MobFields[i].Final->label == handle)
        { return &InitPage->MobFields[i]; }
    }
    
    for(u32 i = 0; i < ALLY_NUM; i++)
    {
        if(InitPage->AllyFields[i].Name->box == handle)
        { return &InitPage->AllyFields[i]; }
        if(InitPage->AllyFields[i].Name->label == handle)
        { return &InitPage->AllyFields[i]; }
        if(InitPage->AllyFields[i].Bonus->box == handle)
        { return &InitPage->AllyFields[i]; }
        if(InitPage->AllyFields[i].Bonus->label == handle)
        { return &InitPage->AllyFields[i]; }
        if(InitPage->AllyFields[i].Final->box == handle)
        { return &InitPage->AllyFields[i]; }
        if(InitPage->AllyFields[i].Final->label == handle)
        { return &InitPage->AllyFields[i]; }
    }
    
    return 0x0;
}