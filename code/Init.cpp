
inline void SwitchInitAddToSelect(InitField *f, u32 idx, s32 max)
{
    if(idx < max) { 
        ShowWindow(f[idx].New.Add->box, SW_HIDE);
        
        ShowWindow(f[idx].New.Name->box, SW_SHOW);
        ShowWindow(f[idx].New.Bonus->box, SW_SHOW);
        ShowWindow(f[idx].New.Ok->box, SW_SHOW);
    }
}

inline void HideInitFieldAdd(InitField *f, u32 idx, s32 max)
{
    if(idx < max) { ShowWindow(f[idx].New.Add->box, SW_HIDE); }
}

inline void HideInitField(InitField *f, s32 n)
{
    for(u32 i = 0; i < n; i++)
    {
        if(f[i].Name != 0) //NOTE: Assuming this is not a party member
        { 
            ShowWindow(f[i].Name->box, SW_HIDE);
            ShowWindow(f[i].Final->box, SW_HIDE);
            ShowWindow(f[i].Bonus->box, SW_HIDE);
            
            ShowWindow(f[i].New.Name->box, SW_HIDE);
            ShowWindow(f[i].New.Bonus->box, SW_HIDE);
            ShowWindow(f[i].New.Add->box, SW_HIDE);
            ShowWindow(f[i].New.Ok->box, SW_HIDE);
        }
        else {
            ShowWindow(f[i].Bonus->box, SW_HIDE);
            ShowWindow(f[i].Bonus->label, SW_HIDE);
        }
    }
}

inline void HideOrder(OrderField *o, s32 n)
{
    for(u32 i = 0; i < n; i++)
    {
        ShowWindow(o[i].Field->box, SW_HIDE);
        ShowWindow(o[i].Pos->box, SW_HIDE);
        ShowWindow(o[i].Remove->box, SW_HIDE);
    }
}

inline void HideElem(HWND box) { ShowWindow(box, SW_HIDE); }
inline void ShowElem(HWND box) { ShowWindow(box, SW_SHOW); }

inline void HideInitElem(InitPage *p) {
    HideInitField(p->MobFields, MOB_NUM);
    ShowElem(p->MobFields[0].New.Add->box);
    
    HideInitField(p->AllyFields, ALLY_NUM);
    ShowElem(p->AllyFields[0].New.Add->box);
    
    HideOrder(p->Order, ORDER_NUM);
    
    HideElem(p->RoundCounter->box);
}

inline void ShowInitFieldAdd(InitField *f, u32 idx, s32 max)
{
    if(idx < max) { ShowWindow(f[idx].New.Add->box, SW_SHOW); }
}

inline void ShowInitField(InitField *f, s32 n, s32 max)
{
    u32 i = 0; 
    for(; i < n; i++)
    {
        if(f[i].Name  != 0) //NOTE: Assuming this is not a party member
        { 
            ShowWindow(f[i].Name->box, SW_SHOW);
            ShowWindow(f[i].Final->box, SW_SHOW);
            ShowWindow(f[i].Bonus->box, SW_SHOW);
        }
        else {
            ShowWindow(f[i].Bonus->box, SW_SHOW);
            ShowWindow(f[i].Bonus->label, SW_SHOW);
        }
    }
    
    if(i < max) { ShowWindow(f[i].New.Add->box, SW_SHOW); }
}

inline void ShowOrder(OrderField *o, s32 n)
{
    for(u32 i = 0; i < n; i++)
    {
        ShowWindow(o[i].Field->box, SW_SHOW);
        ShowWindow(o[i].Pos->box, SW_SHOW);
        ShowWindow(o[i].Remove->box, SW_SHOW);
    }
}

inline void ShowInitElem(InitPage *p) {
    ShowInitField(p->MobFields, MOB_NUM, MOB_NUM);
    ShowInitField(p->AllyFields, ALLY_NUM, ALLY_NUM);
    ShowOrder(p->Order, ORDER_NUM);
}

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

b32 InitTabOnComboSelect(u32 commandID, HWND handle)
{
    InitPage  *Init = State.Init;
    
    if(commandID == Init->Mobs->id)
    {
        s32 idx = ComboBox_GetCurSel(handle);
        Assert(idx != CB_ERR);
        State.Init->VisibleMobs  = idx;
        State.Init->VisibleOrder = PARTY_NUM + State.Init->VisibleAllies + idx;
        
        HideInitField(State.Init->MobFields, MOB_NUM);
        ShowInitField(State.Init->MobFields, idx, MOB_NUM);
        
        HideOrder(State.Init->Order, ORDER_NUM);
        ShowOrder(State.Init->Order, State.Init->VisibleOrder);
        
        return TRUE;
    }
    else if(commandID == Init->Allies->id)
    {
        s32 idx = ComboBox_GetCurSel(handle);
        Assert(idx != CB_ERR);
        Init->VisibleAllies = idx;
        Init->VisibleOrder = PARTY_NUM + Init->VisibleMobs + idx;
        
        HideInitField(Init->AllyFields, ALLY_NUM);
        ShowInitField(Init->AllyFields, idx, ALLY_NUM);
        
        HideOrder(Init->Order, ORDER_NUM);
        ShowOrder(Init->Order, State.Init->VisibleOrder);
        
        return TRUE;
    }
    else if(commandID == Init->EncounterSel->id)
    {
        s32 idx = ComboBox_GetCurSel(handle);
        Assert(idx != CB_ERR);
        
        Encounter *Curr = &State.encounters.Enc[idx];
        Init->VisibleMobs   = Curr->numMobs;
        Init->VisibleAllies = Curr->numAllies;
        Init->VisibleOrder  = PARTY_NUM + Curr->numMobs + Curr->numAllies;
        
        ComboBox_SetCurSel(Init->Mobs->box, Curr->numMobs);
        ComboBox_SetCurSel(Init->Allies->box, Curr->numAllies);
        
        HideInitField(Init->MobFields, MOB_NUM);
        ShowInitField(Init->MobFields, Curr->numMobs, MOB_NUM);
        
        HideInitField(Init->AllyFields, ALLY_NUM);
        ShowInitField(Init->AllyFields, Curr->numAllies, ALLY_NUM);
        
        HideOrder(Init->Order, ORDER_NUM);
        ShowOrder(Init->Order, Init->VisibleOrder);
        
        for(u32 i = 0; i < Init->VisibleMobs; i++)
        {
            Edit_SetText(Init->MobFields[i].Name->box, Curr->mobNames[i]);
            
            char bonus[8] = {};
            ls_itoa_t(Curr->mobBonus[i], bonus, 8);
            
            Edit_SetText(Init->MobFields[i].Bonus->box, bonus);
        }
        
        for(u32 i = 0; i < Init->VisibleAllies; i++)
        {
            Edit_SetText(Init->AllyFields[i].Name->box, Curr->allyNames[i]);
            
            char bonus[8] = {};
            ls_itoa_t(Curr->allyBonus[i], bonus, 8);
            
            Edit_SetText(Init->AllyFields[i].Bonus->box, bonus);
        }
        
        return TRUE;
    }
    
    return FALSE;
}

ComboBox *FillEncounters(HWND h, HWND *wA, const char *label, s32 x, s32 y, u32 width, u32 height, u64 id)
{
    buffer buff = ls_bufferViewIntoPtr(State.encounters.data, KBytes(64));
    
    u32 numOfEncounters = ls_bufferReadDWord(&buff);
    State.encounters.numEncounters = numOfEncounters;
    
    char **encounterNames = (char **)ls_alloc(sizeof(char *) * numOfEncounters);
    for(u32 i = 0; i < numOfEncounters; i++) { encounterNames[i] = (char *)ls_alloc(sizeof(char) * 32); }
    
    u32 cursorStart = buff.cursor;
    for(u32 i = 0; i < numOfEncounters; i++)
    {
        Encounter *CurrEnc = &State.encounters.Enc[i];
        ls_bufferReadData(&buff, encounterNames[i], 32);
        
        u32 numOfMobs = ls_bufferReadDWord(&buff);
        CurrEnc->numMobs = numOfMobs;
        
        for(u32 j = 0; j < numOfMobs; j++)
        {
            char nameBuff[32] = {};
            ls_bufferReadData(&buff, nameBuff, 32);
            
            u32  mobBonus = ls_bufferReadDWord(&buff);
            
            ls_memcpy(nameBuff, CurrEnc->mobNames[j], 32);
            CurrEnc->mobBonus[j] = mobBonus;
        }
        
        if(numOfMobs < MOB_NUM) { 
            u32 paddingBytes = (MOB_NUM - numOfMobs) * sizeOfInitEntry;
            ls_bufferAdvanceCursor(&buff, paddingBytes);
        }
        
        u32 numOfAllies = ls_bufferReadDWord(&buff);
        CurrEnc->numAllies = numOfAllies;
        
        for(u32 j = 0; j < numOfAllies; j++)
        {
            char nameBuff[32] = {};
            ls_bufferReadData(&buff, nameBuff, 32);
            
            u32  allyBonus = ls_bufferReadDWord(&buff);
            
            ls_memcpy(nameBuff, CurrEnc->allyNames[j], 32);
            CurrEnc->allyBonus[j] = allyBonus;
        }
        
        if(numOfAllies < ALLY_NUM) { 
            u32 paddingBytes = (ALLY_NUM - numOfAllies) * sizeOfInitEntry;
            ls_bufferAdvanceCursor(&buff, paddingBytes);
        }
        u32 cursorEnd = buff.cursor;
        Assert(cursorEnd - cursorStart == sizeOfEncEntry);
        cursorStart = cursorEnd;
    }
    
    u32 stupidComboBoxNumberOfItemsWhichDeterminesHeight = numOfEncounters;
    if(numOfEncounters == 1)
    { stupidComboBoxNumberOfItemsWhichDeterminesHeight = 2; }
    
    ComboBox *Result = AddUnsortedComboBox(h, wA, label, LABEL_UP, x, y, width, height, id,
                                           stupidComboBoxNumberOfItemsWhichDeterminesHeight);
    AddAllComboBoxItems(Result->box, encounterNames, numOfEncounters);
    
    return Result;
}

//TODO: Init is improperly Initialized. Can't "Set" at program start without other commands prior.
//      Can't add new enemies or allies if no other enemy/ally selection was previously made.
void DrawInitTab(HWND WinH, u64 *ElementId)
{
    InitPage *Page = State.Init;
    HWND *wA = Page->WindowsArray + Page->numWindows;
    
    Page->Mobs = AddUnsortedComboBox(WinH, wA, "Enemies", LABEL_UP,
                                     276, 102, 100, 20, (*ElementId)++, ArraySize(Enemies));;
    wA += 2;
    
    AddAllComboBoxItems(Page->Mobs->box, Enemies, ArraySize(Enemies));
    
    Page->Allies = AddUnsortedComboBox(WinH, wA, "Allies", LABEL_UP,
                                       520, 282, 100, 20, (*ElementId)++, ArraySize(Allies));;
    wA += 2;
    AddAllComboBoxItems(Page->Allies->box, Allies, ArraySize(Allies));
    
    Page->numWindows += 4;
    
    // Party Fields
    s32 yPos = 142;
    for(u32 i = 0; i < PARTY_NUM; i++)
    {
        Page->PlayerFields[i] = AddInitField(WinH, &wA, PartyName[i], 560, yPos, ElementId, i, TRUE);
        yPos += 20;
        Page->numWindows += 2;
    }
    
    // Ally Fields
    yPos += 70;
    for(u32 i = 0; i < ALLY_NUM; i++)
    {
        Page->AllyFields[i] = AddInitField(WinH, &wA, AllyName[i], 480, yPos, ElementId, i, FALSE);
        yPos += 20;
        Page->numWindows += 7;
    }
    
    // Mob Fields
    yPos = 142;
    for(u32 i = 0; i < MOB_NUM; i++)
    {
        Page->MobFields[i] = AddInitField(WinH, &wA, MobName[i], 246, yPos, ElementId, i, FALSE);
        yPos += 20;
        Page->numWindows += 7;
    }
    
    //ORDER
    yPos = 142;
    for(u32 i = 0; i < ORDER_NUM; i += 2)
    {
        Page->Order[i]   = AddOrderField(WinH, &wA, 720, yPos, i, ElementId);
        if((i+1) < ORDER_NUM) { 
            Page->Order[i+1] = AddOrderField(WinH, &wA, 880, yPos, i+1, ElementId); 
            Page->numWindows += 3;
        }
        Page->numWindows += 3;
        yPos += 20;
    }
    
    //TODO: Should this be initialized here???
    Page->VisibleOrder = PARTY_NUM;
    
    Page->Current = AddStaticUnlabeledTextBox(WinH, wA, 820, 112, 100, 20, (*ElementId)++); wA += 1;
    
    Page->Roll  = AddButton(WinH, wA, "Roll",  406, 102, 45, 20, (*ElementId)++); wA += 1;
    Page->Set   = AddButton(WinH, wA, "Set",   630, 102, 45, 20, (*ElementId)++); wA += 1;
    Page->Next  = AddButton(WinH, wA, "Next",  850, 82, 45, 20, (*ElementId)++); wA += 1;
    Page->Reset = AddButton(WinH, wA, "Reset", 520, 102, 45, 20, (*ElementId)++); wA += 1;
    Page->Save  = AddButton(WinH, wA, "Save",  620, 42, 45, 20, (*ElementId)++); wA += 1;
    
    Page->RoundCounter = AddValueBox(WinH, wA, 0, LABEL_NULL, 1, 1180, 60, 30, 20, (*ElementId)++); wA += 1;
    
    Page->numWindows += 7;
    
    yPos = 142;
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Page->Counters[i] = AddCounter(WinH, &wA, CounterNames[i], 20, yPos, ElementId);
        yPos += 40;
        Page->numWindows += 4;
    }
    
    Page->EncounterName = AddTextBox(WinH, wA, 0, LABEL_NULL, 594, 62, 100, 20, (*ElementId)++); wA += 1;
    Page->numWindows += 1;
    
    //EncounterSelection
    if(State.encounters.isInitialized == TRUE)
    {
        Page->EncounterSel = FillEncounters(WinH, wA, "Encounters", 470, 60, 100, 20, (*ElementId)++);
        wA += 2;
        
        Page->numWindows += 2;
    }
    
    return;
}
