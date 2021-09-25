void ThrowDiceOnClick(UIContext *cxt, void *data)
{
    UIButton *f = (UIButton *)data;
    
    if(f == &State.Init->GeneralThrower.throwDie)
    {
        char toThrow[128] = {};
        s32 len = ls_unistrToAscii_t(&State.Init->GeneralThrower.toHit.text, toThrow, 128);
        s32 result = (s32)diceRoll(toThrow, len);
        
        ls_unistrFromInt_t(&State.Init->GeneralThrower.hitRes.text, result);
        State.Init->GeneralThrower.hitRes.viewEndIdx = State.Init->GeneralThrower.hitRes.text.len;
    }
    else
    {
        u32 idx = 9999;
        for(u32 i = 0; i < THROWER_NUM; i++) {
            if(f == &State.Init->Throwers[i].throwDie) { idx = i; }
        }
        AssertMsg(idx != 9999, "Couldn't find thrower\n");
        
        DiceThrow *f = State.Init->Throwers + idx;
        
        char toThrow[128] = {};
        s32 len = ls_unistrToAscii_t(&f->toHit.text, toThrow, 128);
        s32 result = (s32)diceRoll(toThrow, len);
        
        ls_unistrFromInt_t(&f->hitRes.text, result);
        f->hitRes.viewEndIdx = f->hitRes.text.len;
        
        char dmgThrow[128] = {};
        len = ls_unistrToAscii_t(&f->damage.text, dmgThrow, 128);
        result = (s32)diceRoll(dmgThrow, len);
        
        ls_unistrFromInt_t(&f->dmgRes.text, result);
        f->dmgRes.viewEndIdx = f->dmgRes.text.len;
    }
}

s32 sortTmpOrder(void *a, void *b)
{
    tmp_order *ordA = (tmp_order *)a;
    tmp_order *ordB = (tmp_order *)b;
    
    if(ordA->init < ordB->init) { return -1; }
    else if(ordA->init == ordB->init) { return 0; }
    else { return 1; }
}

void RollOnClick(UIContext *cxt, void *data)
{
    InitPage *Page = State.Init;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        s32 finalVal = ls_unistrToInt(f->final.text);
        if(finalVal != 0) { continue; }
        
        s32 die = pcg32_bounded(&pcg32_global, 20) + 1;
        s32 bonus = ls_unistrToInt(f->bonus.text);
        
        ls_unistrClear(&f->final.text);
        ls_unistrFromInt_t(&f->final.text, bonus + die);
        f->final.viewEndIdx = f->final.text.len;
    }
    
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        
        s32 finalVal = ls_unistrToInt(f->final.text);
        if(finalVal != 0) { continue; }
        
        s32 die = pcg32_bounded(&pcg32_global, 20) + 1;
        s32 bonus = ls_unistrToInt(f->bonus.text);
        
        ls_unistrClear(&f->final.text);
        ls_unistrFromInt_t(&f->final.text, bonus + die);
        f->final.viewEndIdx = f->final.text.len;
    }
}

void SetOnClick(UIContext *cxt, void *data)
{
    AssertMsg(!State.inBattle, "Clicking this should be impossible while in Battle\n");
    
    InitPage *Page = State.Init;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    
    tmp_order ord[ORDER_NUM] = {};
    u32 idx = 0;
    
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        ord[idx].init = ls_unistrToInt(f->final.text);
        ord[idx].name = &f->name.text;
        ord[idx].ID   = f->ID;
        
        idx += 1;
    }
    
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        
        ord[idx].init = ls_unistrToInt(f->final.text);
        ord[idx].name = &f->name.text;
        ord[idx].ID   = f->ID;
        
        idx += 1;
    }
    
    for(u32 i = 0; i < PARTY_NUM; i++)
    {
        ord[idx].init = ls_unistrToInt(Page->PlayerInit[i].text);
        ord[idx].name = (unistring *)(PartyNameUTF32 + i);
        ord[idx].ID   = i;
        
        idx += 1;
    }
    
    ls_quicksortCustom(ord, sizeof(tmp_order), visibleOrder, sortTmpOrder);
    
    for(u32 i = 0, j = visibleOrder - 1; i < visibleOrder; i++, j--)
    {
        Order *f = Page->OrderFields + i;
        
        ls_unistrSet(&f->field.text, *ord[j].name);
        f->ID = ord[j].ID;
        
        //TODO: Make this just a reference to Order[i].field.text ??
        if(i == 0) 
        { 
            ls_unistrSet(&Page->Current.text, *ord[j].name); 
            Page->Current.viewEndIdx = Page->Current.text.len;
        }
    }
    
    for(u32 i = 0; i < PARTY_NUM; i++)
    { Page->PlayerInit[i].isReadonly = TRUE; }
    
    for(u32 i = 0; i < MOB_NUM; i++)  {
        Page->MobFields[i].name.isReadonly  = TRUE;
        Page->MobFields[i].bonus.isReadonly = TRUE;
        Page->MobFields[i].final.isReadonly = TRUE;
    }
    
    for(u32 i = 0; i < ALLY_NUM; i++) { 
        Page->AllyFields[i].name.isReadonly  = TRUE;
        Page->AllyFields[i].bonus.isReadonly = TRUE;
        Page->AllyFields[i].final.isReadonly = TRUE;
    }
    
    Page->turnsInRound = visibleOrder;
    
    State.inBattle = TRUE;
}

void ResetOnClick(UIContext *cxt, void *data)
{
    InitPage *Page = State.Init;
    
    Page->Mobs.selectedIndex   = 0;
    Page->Allies.selectedIndex = 0;
    State.inBattle = FALSE;
    
    unistring zeroUTF32 = { (u32 *)U"0", 1, 1 };
    
    for(u32 i = 0; i < PARTY_NUM; i++) 
    { 
        ls_uiTextBoxClear(cxt, Page->PlayerInit + i);
        ls_unistrSet(&Page->PlayerInit[i].text, zeroUTF32);
        Page->PlayerInit[i].viewEndIdx = Page->PlayerInit[i].text.len;
    }
    
    s32 currID = PARTY_NUM;
    for(u32 i = 0; i < MOB_NUM; i++)   
    { 
        InitField *f = Page->MobFields + i;
        
        ls_uiTextBoxClear(cxt, &f->name);
        ls_unistrSet(&f->name.text, ls_unistrConstant(MobName[i]));
        f->name.viewEndIdx = f->name.text.len;
        
        ls_uiTextBoxClear(cxt, &f->bonus);
        ls_unistrSet(&f->bonus.text ,zeroUTF32);
        f->bonus.viewEndIdx = f->bonus.text.len;
        
        ls_uiTextBoxClear(cxt, &f->final);
        ls_unistrSet(&f->final.text, zeroUTF32);
        f->final.viewEndIdx = f->final.text.len;
        
        f->ID = currID;
        currID += 1;
    }
    
    for(u32 i = 0; i < ALLY_NUM; i++)  
    { 
        InitField *f = Page->AllyFields + i;
        
        ls_uiTextBoxClear(cxt, &f->name);
        ls_unistrSet(&f->name.text, ls_unistrConstant(AllyName[i]));
        f->name.viewEndIdx = f->name.text.len;
        
        ls_uiTextBoxClear(cxt, &f->bonus);
        ls_unistrSet(&f->bonus.text ,zeroUTF32);
        f->bonus.viewEndIdx = f->bonus.text.len;
        
        ls_uiTextBoxClear(cxt, &f->final);
        ls_unistrSet(&f->final.text, zeroUTF32);
        f->final.viewEndIdx = f->final.text.len;
        
        f->ID = currID;
        currID += 1;
    }
    
    for(u32 i = 0; i < ORDER_NUM; i++)
    {
        Order *f = Page->OrderFields + i;
        
        ls_unistrClear(&f->field.text);
        f->field.currPos = 1.0;
        f->ID = -1;
    }
    
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *f = Page->Counters + i;
        
        ls_uiTextBoxClear(cxt, &f->name);
        ls_uiTextBoxClear(cxt, &f->rounds);
        f->roundsLeft      = 0;
        f->startIdxInOrder = 0;
        f->turnCounter     = 0;
        f->isActive        = FALSE;
    }
    
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrow *f = Page->Throwers + i;
        
        ls_uiTextBoxClear(cxt, &f->name);
        ls_uiTextBoxClear(cxt, &f->toHit);
        ls_uiTextBoxClear(cxt, &f->hitRes);
        ls_uiTextBoxClear(cxt, &f->damage);
        ls_uiTextBoxClear(cxt, &f->dmgRes);
    }
    
    // General Thrower
    {
        ls_uiTextBoxClear(cxt, &Page->GeneralThrower.toHit);
        ls_uiTextBoxClear(cxt, &Page->GeneralThrower.hitRes);
    }
    
    ls_unistrClear(&Page->Current.text);
    
    ls_unistrSet(&Page->RoundCounter.text, zeroUTF32);
    Page->RoundCounter.viewEndIdx = Page->RoundCounter.text.len;
    
    Page->orderAdjust = 0;
    
    addID = 1000;
}

void NextOnClick(UIContext *cxt, void *data)
{
    InitPage *Page = State.Init;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    
    Page->currIdx = (Page->currIdx + 1) % visibleOrder;
    
    //NOTE: Increase the round
    if(Page->currIdx == 0)
    {
        Page->roundCount += 1;
        ls_unistrFromInt_t(&Page->RoundCounter.text, Page->roundCount);
        Page->RoundCounter.viewEndIdx = Page->RoundCounter.text.len;
    }
    
    //NOTE: Set the Current
    ls_unistrSet(&Page->Current.text, Page->OrderFields[Page->currIdx].field.text);
    Page->Current.viewEndIdx = Page->Current.text.len;
    
    //NOTE: Advance the Counters
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *C = Page->Counters + i;
        
        if(C->isActive == TRUE)
        {
            C->turnCounter += 1;
            
            if(C->turnCounter >= Page->turnsInRound)
            { 
                C->turnCounter = 0;
                C->roundsLeft -= 1;
                
                AssertMsg(C->roundsLeft >= 0, "Rounds Left became negative");
                
                if(C->roundsLeft == 0)
                {
                    ls_uiTextBoxClear(cxt, &C->name);
                    ls_uiTextBoxClear(cxt, &C->rounds);
                    
                    C->turnCounter = 0;
                    C->isActive     = FALSE;
                    
                    continue;
                }
                
                ls_unistrFromInt_t(&C->rounds.text, C->roundsLeft);
                C->rounds.viewEndIdx = C->rounds.text.len;
                
                continue;
            }
            
            continue;
        }
    }
}

void CopyOrder(Order *From, Order *To)
{
    // Copy Slider
    To->field.isHot  = From->field.isHot;
    To->field.isHeld = From->field.isHeld;
    
    ls_unistrSet(&To->field.text, From->field.text);
    
    To->field.currValue = From->field.currValue;
    To->field.maxValue  = From->field.maxValue;
    To->field.minValue  = From->field.minValue;
    
    To->field.currPos   = From->field.currPos;
    
    To->field.style     = From->field.style;
    To->field.lColor    = From->field.lColor;
    To->field.rColor    = From->field.rColor;
    
    //NOTE: Don't need to copy the textbox, since the position of an order field is fixed.
    //NOTE: Don't need to copy the button
    
    To->ID = From->ID;
}

void CopyInitField(InitField *From, InitField *To)
{
    ls_unistrSet(&To->name.text, From->name.text);
    ls_unistrSet(&To->bonus.text, From->bonus.text);
    ls_unistrSet(&To->final.text, From->final.text);
    
    To->ID = From->ID;
}

void RemoveOrderOnClick(UIContext *cxt, void *data)
{
    if(State.inBattle == FALSE) { return; }
    
    u32 index = (u32)((u64)data);
    
    InitPage *Page = State.Init;
    Order *f = Page->OrderFields + index;
    
    s32 removeID = f->ID;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    
    if(removeID < PARTY_NUM)
    {
        Page->orderAdjust += 1;
        
        for(u32 i = index; i < (visibleOrder-1); i++)
        {
            Order *A = Page->OrderFields + i;
            Order *B = Page->OrderFields + (i+1);
            
            CopyOrder(B, A);
        }
        
        goto exit;
    }
    else
    {
        for(u32 i = 0; i < ALLY_NUM; i++) 
        {
            InitField *ally = Page->AllyFields + i;
            if(removeID == ally->ID) 
            {
                //NOTE: We remove the ally from the allies list by moving the last one in its place
                InitField *B = Page->AllyFields + (visibleAllies - 1);
                CopyInitField(B, ally);
                
                //TODO: Is clearing for new additions a waste of CPU time? The user won't see them anyway
                ls_uiTextBoxClear(cxt, &B->name);
                ls_uiTextBoxClear(cxt, &B->bonus);
                ls_uiTextBoxClear(cxt, &B->final);
                B->ID = 0;
                
                Page->Allies.selectedIndex -= 1;
                
                for(u32 i = index; i < (visibleOrder-1); i++)
                {
                    Order *A = Page->OrderFields + i;
                    Order *B = Page->OrderFields + (i+1);
                    
                    CopyOrder(B, A);
                }
                
                goto exit;
            }
        }
        
        for(u32 i = 0; i < MOB_NUM; i++)
        {
            InitField *mob = Page->MobFields + i;
            if(removeID == mob->ID)
            {
                //NOTE: We remove the Mob from the mobs list by moving the last one in its place
                InitField *B = Page->MobFields + (visibleMobs - 1);
                CopyInitField(B, mob);
                
                //TODO: Is clearing for new additions a waste of CPU time? The user won't see them anyway
                ls_uiTextBoxClear(cxt, &B->name);
                ls_uiTextBoxClear(cxt, &B->bonus);
                ls_uiTextBoxClear(cxt, &B->final);
                B->ID = 0;
                
                Page->Mobs.selectedIndex -= 1;
                
                for(u32 i = index; i < (visibleOrder-1); i++)
                {
                    Order *A = Page->OrderFields + i;
                    Order *B = Page->OrderFields + (i+1);
                    
                    CopyOrder(B, A);
                }
                
                goto exit;
            }
        }
    }
    
    
    exit:
    
    Page->turnsInRound -= 1;
    
    //NOTE: We won't move the 'Current' field if you remove the 'Current' from the order.
    //      Because of that, Counters will be one count extra on the first lap after the remove.
    //      So we decrease them by one.
    if(Page->currIdx == index)
    {
        for(u32 i = 0; i < COUNTER_NUM; i++)
        {
            if(Page->Counters[i].isActive) 
            { Page->Counters[i].turnCounter -= 1; }
        }
    }
    
    //NOTE: Because we push everything up in the order (they go towards index 0)
    //      If the current index is larger then the removed idx, the current will have been
    //      pushed up, hence we need to readjust it.
    if(Page->currIdx >= index) { Page->currIdx -= 1; }
    
    return;
}

void StartCounterOnClick(UIContext *cxt, void *data)
{
    Counter *C = (Counter *)data;
    
    if(State.inBattle == FALSE) { return; }
    
    s32 val = ls_unistrToInt(C->rounds.text);
    if(val <= 0) { return; }
    
    C->name.isReadonly   = TRUE;
    C->rounds.isReadonly = TRUE;
    
    C->isActive        = TRUE;
    C->turnCounter     = 0;
    C->roundsLeft      = val;
    C->startIdxInOrder = State.Init->currIdx;
    
    return;
}

void PlusOneCounterOnClick(UIContext *cxt, void *data)
{
    Counter *C = (Counter *)data;
    
    C->roundsLeft += 1;
    ls_unistrFromInt_t(&C->rounds.text, C->roundsLeft);
    
    return;
}

void StopCounterOnClick(UIContext *cxt, void *data)
{
    Counter *C = (Counter *)data;
    
    ls_uiTextBoxClear(cxt, &C->name);
    ls_uiTextBoxClear(cxt, &C->rounds);
    
    C->name.isReadonly   = FALSE;
    C->rounds.isReadonly = FALSE;
    
    C->isActive        = FALSE;
    C->turnCounter     = 0;
    C->roundsLeft      = 0;
    C->startIdxInOrder = 0;
}

void AddNewInitOnClick(UIContext *cxt, void *data)
{
    InitField *f = (InitField *)data;
    f->isAdding = TRUE;
}

//TODO: Right now we are not allowing to give an Initiative Value and put the new 
//      Field in the right place in Order.
//      If we allow it, the counter checker WILL HAVE to probably fix C->startIdxInOrder to work.
void AddConfirmOnClick(UIContext *cxt, void *data)
{
    InitField *f = (InitField *)data;
    
    s32 visibleMobs   = State.Init->Mobs.selectedIndex;
    s32 visibleAllies = State.Init->Allies.selectedIndex;
    s32 visibleOrder = visibleMobs + visibleAllies + PARTY_NUM - State.Init->orderAdjust;
    
    //NOTETODO: This is a really dumb way to determine if it's an ally or a mob...
    if(State.Init->AllyFields <= f && f <= (State.Init->AllyFields + visibleAllies))
    { State.Init->Allies.selectedIndex += 1; }
    else
    { State.Init->Mobs.selectedIndex += 1; }
    
    State.Init->turnsInRound += 1;
    
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *C = State.Init->Counters + i;
        if(C->isActive)
        { 
            AssertMsg(State.Init->turnsInRound >= C->startIdxInOrder, "startIdxInOrder is not realiable anymore\n");
            
            //NOTE: This counter missed counting the new addition this round,
            //      So we manually count up by one.
            if(State.Init->currIdx < C->startIdxInOrder)
            { C->turnCounter += 1; }
        }
    }
    
    Order *o = State.Init->OrderFields + visibleOrder;
    
    ls_unistrSet(&o->field.text, f->addName.text);
    ls_unistrSet(&f->name.text, f->addName.text);
    f->name.viewEndIdx = f->name.text.len;
    
    f->name.isReadonly  = TRUE;
    f->bonus.isReadonly = TRUE;
    f->final.isReadonly = TRUE;
    
    //NOTE: Because IDs in the InitFields get overwritten when removing from order 
    //      we can't reliably re-use them, unless we create a system to dispense Unique IDs.
    //      So for simplicy we are just starting from 1000 and every single Added Init will just the next one.
    //      AddID is reset during ResetOnClick
    f->ID = addID;
    o->ID = addID;
    addID += 1;
    
    f->isAdding = FALSE;
}

void SetInitTab(UIContext *cxt)
{
    InitPage *Page = State.Init;
    
    for(u32 i = 0; i < MOB_NUM + 1; i++) { ls_uiListBoxAddEntry(cxt, &Page->Mobs, (char *)Enemies[i]); }
    for(u32 i = 0; i < ALLY_NUM + 1; i++) { ls_uiListBoxAddEntry(cxt, &Page->Allies, (char *)Allies[i]); }
    
    for(u32 i = 0; i < PARTY_NUM; i++) 
    { 
        Page->PlayerInit[i].text       = ls_unistrFromUTF32(U"0");
        Page->PlayerInit[i].viewEndIdx = Page->PlayerInit[i].text.len;
        Page->PlayerInit[i].maxLen     = 2;
    }
    
    s32 currID = PARTY_NUM;
    for(u32 i = 0; i < MOB_NUM; i++)   
    { 
        InitField *f = Page->MobFields + i;
        
        f->name.text        = ls_unistrFromUTF32(MobName[i]);
        f->name.viewEndIdx  = f->name.text.len;
        
        f->bonus.text       = ls_unistrFromUTF32(U"0");
        f->bonus.viewEndIdx = f->bonus.text.len;
        f->bonus.maxLen     = 2;
        
        f->final.text       = ls_unistrFromUTF32(U"0");
        f->final.viewEndIdx = f->final.text.len;
        f->final.maxLen     = 2;
        
        f->ID = currID;
        currID += 1;
        
        f->addName.text     = ls_unistrAlloc(16);
        f->addInit.text     = ls_unistrAlloc(16);
        f->addInit.maxLen   = 2;
        
        f->addNew.name      = ls_unistrFromUTF32(U"+");
        f->addNew.onClick   = AddNewInitOnClick;
        f->addNew.data      = f;
        f->addNew.onHold    = 0x0;
        
        f->addConfirm.name    = ls_unistrFromUTF32(U"Ok");
        f->addConfirm.onClick = AddConfirmOnClick;
        f->addConfirm.data    = f;
        f->addConfirm.onHold  = 0x0;
    }
    
    for(u32 i = 0; i < ALLY_NUM; i++)  
    { 
        InitField *f = Page->AllyFields + i;
        
        f->name.text        = ls_unistrFromUTF32(AllyName[i]);
        f->name.viewEndIdx  = f->name.text.len;
        
        f->bonus.text       = ls_unistrFromUTF32(U"0");
        f->bonus.viewEndIdx = f->bonus.text.len;
        f->bonus.maxLen     = 2;
        
        f->final.text       = ls_unistrFromUTF32(U"0");
        f->final.viewEndIdx = f->final.text.len;
        f->final.maxLen     = 2;
        
        f->ID = currID;
        currID += 1;
        
        f->addName.text     = ls_unistrAlloc(16);
        f->addInit.text     = ls_unistrAlloc(16);
        f->addInit.maxLen   = 2;
        
        f->addNew.name      = ls_unistrFromUTF32(U"+");
        f->addNew.onClick   = AddNewInitOnClick;
        f->addNew.data      = f;
        f->addNew.onHold    = 0x0;
        
        f->addConfirm.name    = ls_unistrFromUTF32(U"Ok");
        f->addConfirm.onClick = AddConfirmOnClick;
        f->addConfirm.data    = f;
        f->addConfirm.onHold  = 0x0;
    }
    
    for(u32 i = 0; i < ORDER_NUM; i++)
    {
        Order *f = Page->OrderFields + i;
        
        Color lColor      = ls_uiAlphaBlend(RGBA(0x10, 0xDD, 0x20, 0x99), cxt->widgetColor);
        Color rColor      = ls_uiAlphaBlend(RGBA(0xDD, 0x10, 0x20, 0x99), cxt->widgetColor);
        f->field          = ls_uiSliderInit(NULL, 100, -50, 1.0, SL_BOX, lColor, rColor);
        
        f->pos.text       = ls_unistrFromInt(i);
        f->pos.viewEndIdx = f->pos.text.len;
        f->pos.maxLen     = 2;
        
        f->remove.name    = ls_unistrFromUTF32(U"X");
        f->remove.onClick = RemoveOrderOnClick;
        f->remove.data    = (void *)((u64)i);
        f->remove.onHold  = 0x0;
    }
    
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *f = Page->Counters + i;
        
        f->name.text     = ls_unistrAlloc(16);
        f->rounds.text   = ls_unistrAlloc(16);
        f->rounds.maxLen = 2;
        
        f->start.name    = ls_unistrFromUTF32(U"Start");
        f->start.onClick = StartCounterOnClick;
        f->start.data    = (void *)f;
        f->start.onHold  = 0x0;
        
        f->plusOne.name    = ls_unistrFromUTF32(U"+1");
        f->plusOne.onClick = PlusOneCounterOnClick;
        f->plusOne.data    = (void *)f;
        f->plusOne.onHold  = 0x0;
        
        f->stop.name    = ls_unistrFromUTF32(U"Stop");
        f->stop.onClick = StopCounterOnClick;
        f->stop.data    = (void *)f;
        f->stop.onHold  = 0x0;
    }
    
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrow *f = Page->Throwers + i;
        
        f->name.text   = ls_unistrAlloc(32);
        f->toHit.text  = ls_unistrAlloc(32);
        f->hitRes.text = ls_unistrAlloc(16);
        f->damage.text = ls_unistrAlloc(32);
        f->dmgRes.text = ls_unistrAlloc(16);
        
        f->hitRes.maxLen = 4;
        f->hitRes.isReadonly = TRUE;
        
        f->dmgRes.maxLen = 4;
        f->dmgRes.isReadonly = TRUE;
        
        f->throwDie.name    = ls_unistrFromUTF32(U"Go");
        f->throwDie.onClick = ThrowDiceOnClick;
        f->throwDie.data    = &f->throwDie;
        f->throwDie.onHold  = 0x0;
    }
    
    {
        // General Thrower
        Page->GeneralThrower.toHit.text  = ls_unistrAlloc(32);
        Page->GeneralThrower.hitRes.text = ls_unistrAlloc(16);
        
        Page->GeneralThrower.hitRes.maxLen = 4;
        Page->GeneralThrower.hitRes.isReadonly = TRUE;
        
        Page->GeneralThrower.throwDie.name    = ls_unistrFromUTF32(U"Go");
        Page->GeneralThrower.throwDie.onClick = ThrowDiceOnClick;
        Page->GeneralThrower.throwDie.data    = &Page->GeneralThrower.throwDie;
        Page->GeneralThrower.throwDie.onHold  = 0x0;
    }
    
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    { ls_uiListBoxAddEntry(cxt, &Page->EncounterSel, State.encounters.Enc[i].name); }
    
    Page->EncounterName.text = ls_unistrAlloc(16);
    Page->Save.name    = ls_unistrFromUTF32(U"Save");
    Page->Save.onClick = 0x0;
    Page->Save.data    = 0x0;
    Page->Save.onHold  = 0x0;
    
    
    Page->Current.text            = ls_unistrAlloc(16);
    Page->Current.isReadonly      = TRUE;
    
    Page->RoundCounter.text       = ls_unistrFromUTF32(U"0");
    Page->RoundCounter.viewEndIdx = Page->RoundCounter.text.len;
    Page->RoundCounter.isReadonly = TRUE;
    
    Page->Roll.name     = ls_unistrFromUTF32(U"Roll");
    Page->Roll.onClick  = RollOnClick;
    Page->Roll.onHold   = 0x0;
    
    Page->Set.name      = ls_unistrFromUTF32(U"Set");
    Page->Set.onClick   = SetOnClick;
    Page->Set.onHold    = 0x0;
    
    Page->Reset.name    = ls_unistrFromUTF32(U"Reset");
    Page->Reset.onClick = ResetOnClick;
    Page->Reset.onHold  = 0x0;
    
    Page->Next.name     = ls_unistrFromUTF32(U"Next");
    Page->Next.onClick  = NextOnClick;
    Page->Next.onHold   = 0x0;
}

void DrawInitField(UIContext *cxt, InitField *F, s32 x, s32 y)
{
    s32 w = 120;
    
    InitPage *Page = State.Init;
    
    ls_uiTextBox(cxt, &F->name,  x         , y, w, 20);
    ls_uiTextBox(cxt, &F->bonus, x + w     , y, 32, 20);
    ls_uiTextBox(cxt, &F->final, x + w + 32, y, 32, 20);
}

void DrawOrderField(UIContext *cxt, Order *f, s32 xPos, s32 yPos)
{
    ls_uiSlider(cxt, &f->field, xPos + 50, yPos, 120, 20);
    ls_uiTextBox(cxt, &f->pos, xPos + 25, yPos, 32, 20);
    ls_uiButton(cxt, &f->remove, xPos, yPos, 20, 20);
}

void DrawInitTab(UIContext *cxt)
{
    InitPage *Page = State.Init;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    
    // Party
    s32 yPos = 658;
    for(u32 i = 0; i < PARTY_NUM; i++)
    {
        ls_uiLabel(cxt, ls_unistrConstant(PartyName[i]), 580, yPos+6);
        ls_uiTextBox(cxt, Page->PlayerInit + i, 662, yPos, 32, 20);
        yPos -= 20;
    }
    
    // Allies
    yPos = 478;
    for(u32 i = 0; i < visibleAllies; i++)
    {
        DrawInitField(cxt, Page->AllyFields + i, 546, yPos);
        yPos -= 20;
    }
    
    // Enemies
    yPos = 658;
    for(u32 i = 0; i < visibleMobs; i++)
    {
        DrawInitField(cxt, Page->MobFields + i, 296, yPos);
        yPos -= 20;
    }
    
    // Order
    yPos = 658;
    for(u32 i = 0; i < visibleOrder; i += 2)
    {
        DrawOrderField(cxt, Page->OrderFields + i, 770, yPos);
        
        if((i+1) < visibleOrder)
        { DrawOrderField(cxt, Page->OrderFields + (i+1), 956, yPos); }
        
        yPos -= 20;
    }
    
    // Counters
    yPos = 658;
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *f = Page->Counters + i;
        
        ls_uiLabel(cxt, ls_unistrConstant(CounterNames[i]), 20, yPos+24);
        
        ls_uiTextBox(cxt, &f->name, 20, yPos, 100, 20);
        ls_uiTextBox(cxt, &f->rounds, 125, yPos, 36, 20);
        
        if(!f->isActive) { ls_uiButton(cxt, &f->start, 166, yPos, 48, 20); }
        else
        {
            ls_uiButton(cxt, &f->plusOne, 166, yPos, 48, 20);
            ls_uiButton(cxt, &f->stop, 85, yPos+22, 48, 20);
        }
        
        yPos -= 44;
    }
    
    // Dice Throwers
    u32 xPos = 20;
    yPos = 110;
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrow *f = Page->Throwers + i;
        
        ls_uiTextBox(cxt, &f->name,   xPos,       yPos,      120, 20);
        ls_uiTextBox(cxt, &f->toHit,  xPos,       yPos + 24, 138, 20);
        ls_uiTextBox(cxt, &f->hitRes, xPos + 138, yPos + 24, 36,  20);
        ls_uiTextBox(cxt, &f->damage, xPos,       yPos + 48, 138, 20);
        ls_uiTextBox(cxt, &f->dmgRes, xPos + 138, yPos + 48, 36,  20);
        
        ls_uiButton(cxt, &f->throwDie, xPos + 126, yPos, 48, 20);
        
        xPos += 212;
    }
    
    xPos = 20;
    yPos = 62;
    // General Thrower
    {
        ls_uiTextBox(cxt, &Page->GeneralThrower.toHit,  xPos,       yPos, 168, 20);
        ls_uiTextBox(cxt, &Page->GeneralThrower.hitRes, xPos + 168, yPos, 36,  20);
        
        ls_uiButton(cxt, &Page->GeneralThrower.throwDie, xPos + 210, yPos, 48, 20);
    }
    
    
    
    ls_uiButton(cxt, &Page->Reset, 600, 698, 48, 20);
    
    
    if(!State.inBattle)
    {
        //TODO: Clicking on a ListBox Entry clicks also what's behind it.
        ls_uiListBox(cxt, &Page->Mobs, 336, 698, 100, 20);
        ls_uiListBox(cxt, &Page->Allies, 570, 518, 100, 20);
        
        ls_uiButton(cxt, &Page->Roll, 486, 698, 48, 20);
        ls_uiButton(cxt, &Page->Set, 710, 698, 48, 20);
        
        ls_uiListBox(cxt, &Page->EncounterSel, 500, 738, 100, 20);
        ls_uiTextBox(cxt, &Page->EncounterName, 624, 738, 100, 20);
        
        ls_uiButton(cxt, &Page->Save, 601, 760, 44, 20);
    }
    else
    {
        ls_uiTextBox(cxt, &Page->Current, 870, 688, 100, 20);
        ls_uiTextBox(cxt, &Page->RoundCounter, 1180, 740, 30, 20);
        
        ls_uiButton(cxt, &Page->Next, 900, 718, 48, 20);
        
        if(visibleAllies < ALLY_NUM)
        {
            InitField *f = Page->AllyFields + visibleAllies;
            
            u32 addY = 474 - (20*visibleAllies);
            if(!f->isAdding)
            {
                ls_uiButton(cxt, &f->addNew, 606, addY, 36, 20);
            }
            else
            {
                ls_uiTextBox(cxt, &f->addName, 546, addY+4, 120, 20);
                ls_uiTextBox(cxt, &f->addInit, 666, addY+4, 32, 20);
                
                ls_uiButton(cxt, &f->addConfirm, 698, addY+4, 32, 20);
            }
        }
        
        if(visibleMobs < MOB_NUM)
        {
            InitField *f = Page->MobFields + visibleMobs;
            
            u32 addY = 654 - (20*visibleMobs);
            if(!f->isAdding)
            {
                ls_uiButton(cxt, &f->addNew, 356, addY, 36, 20);
            }
            else
            {
                ls_uiTextBox(cxt, &f->addName, 296, addY+4, 120, 20);
                ls_uiTextBox(cxt, &f->addInit, 416, addY+4, 32, 20);
                
                ls_uiButton(cxt, &f->addConfirm, 448, addY+4, 32, 20);
            }
        }
    }
}