void selectStyleDefault(UIContext *cxt, void *data)
{
    currentStyle = INIT_STYLE_DEFAULT;
    return;
}

void selectStylePrana(UIContext *cxt, void *data)
{
    currentStyle = INIT_STYLE_PRANA;
    return;
}

void CustomPlayerText(UIContext *cxt, void *data)
{
    InitPage *Page = State.Init;
    
    UITextBox *f = (UITextBox *)data;
    
    if(cxt->lastFocus != (u64 *)f) { ls_uiTextBoxClear(cxt, f); }
    
    if(KeyPress(keyMap::Enter))
    {
        UITextBox *lastPlayerBox = Page->PlayerInit + (PARTY_NUM - 1);
        
        if(f == lastPlayerBox) { ls_uiFocusChange(cxt, 0x0); return; }
        
        ls_uiFocusChange(cxt, (u64 *)(f + 1));
        return;
    }
    
    return;
}

void CustomInitFieldText(UIContext *cxt, void *data)
{
    InitPage *Page = State.Init;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    
    InitField *mobField  = &Page->MobFields[visibleMobs-1];
    InitField *allyField = &Page->AllyFields[visibleAllies-1];
    
    InitField *f   = (InitField *)data;
    
    UITextBox *name  = &f->name;
    UITextBox *bonus = &f->bonus;
    UITextBox *final = &f->final;
    
    if((cxt->lastFocus != (u64 *)name)  && (cxt->currentFocus == (u64 *)name))  { ls_uiTextBoxClear(cxt, &f->name);  }
    if((cxt->lastFocus != (u64 *)bonus) && (cxt->currentFocus == (u64 *)bonus)) { ls_uiTextBoxClear(cxt, &f->bonus); }
    if((cxt->lastFocus != (u64 *)final) && (cxt->currentFocus == (u64 *)final)) { ls_uiTextBoxClear(cxt, &f->final); }
    
    if(KeyPress(keyMap::Enter))
    {
        if(name == &mobField->name)  { ls_uiFocusChange(cxt, 0x0); return; }
        if(name == &allyField->name) { ls_uiFocusChange(cxt, 0x0); return; }
        
        if(cxt->currentFocus == (u64 *)name)
        {
            UITextBox *boxRef = &((f+1)->name);
            ls_uiTextBoxClear(cxt, boxRef);
            
            u64 *newFocus = (u64 *)boxRef;
            ls_uiFocusChange(cxt, newFocus);
            
            return;
        }
        else if(cxt->currentFocus == (u64 *)bonus)
        {
            UITextBox *boxRef = &((f+1)->bonus);
            ls_uiTextBoxClear(cxt, boxRef);
            
            u64 *newFocus = (u64 *)boxRef;
            ls_uiFocusChange(cxt, newFocus);
            
            return;
        }
        else if(cxt->currentFocus == (u64 *)final)
        {
            UITextBox *boxRef = &((f+1)->final);
            ls_uiTextBoxClear(cxt, boxRef);
            
            u64 *newFocus = (u64 *)boxRef;
            ls_uiFocusChange(cxt, newFocus);
            
            return;
        }
        
        return;
    }
    
    if(KeyPress(keyMap::DArrow))
    {
        if(name == &mobField->name)  { ls_uiFocusChange(cxt, 0x0); return; }
        if(name == &allyField->name) { ls_uiFocusChange(cxt, 0x0); return; }
        
        u32 numStrings = 0;
        unistring *words = ls_unistrSeparateByNumber(name->text, &numStrings);
        
        AssertMsg(words, "The returned words array was null.\n");
        if(!words) { ls_uiFocusChange(cxt, 0x0); return; }
        
        if(numStrings != 2) { ls_uiFocusChange(cxt, 0x0); return; }
        
        s64 newNumber = ls_unistrToInt(words[1]) + 1;
        ls_unistrFromInt_t(&words[1], newNumber);
        
        InitField *next = f + 1;
        
        ls_uiTextBoxClear(cxt, &next->name);
        ls_uiTextBoxClear(cxt, &next->bonus);
        
        ls_unistrSet(&next->name.text, words[0]);
        ls_unistrAppend(&next->name.text, words[1]);
        next->name.viewEndIdx = next->name.text.len;
        
        ls_unistrSet(&next->bonus.text, f->bonus.text);
        next->bonus.viewEndIdx = next->bonus.text.len;
        
        ls_uiFocusChange(cxt, (u64 *)&next->name);
        
        return;
    }
    
    return;
}

void SaveEncounterOnClick(UIContext *cxt, void *data)
{
    s32 visibleMobs   = State.Init->Mobs.selectedIndex;
    s32 visibleAllies = State.Init->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - State.Init->orderAdjust;
    
    u32 numEncounters = State.encounters.numEncounters;
    Encounter *curr = State.encounters.Enc + numEncounters;
    
    ls_unistrSet(&curr->name, State.Init->EncounterName.text);
    
    curr->numMobs = visibleMobs;
    curr->numAllies = visibleAllies;
    
    for(u32 i = 0; i < visibleMobs; i++)
    {
        ls_unistrSet(&curr->mobName[i], State.Init->MobFields[i].name.text);
        ls_unistrSet(&curr->mobBonus[i], State.Init->MobFields[i].bonus.text);
        ls_unistrSet(&curr->mobFinal[i], State.Init->MobFields[i].final.text);
        //ls_unistrSet(&curr->mobAC[i], State.Init->MobFields[i].AC.text);
    }
    
    for(u32 i = 0; i < visibleAllies; i++)
    {
        ls_unistrSet(&curr->allyName[i], State.Init->AllyFields[i].name.text);
        ls_unistrSet(&curr->allyBonus[i], State.Init->AllyFields[i].bonus.text);
        ls_unistrSet(&curr->allyFinal[i], State.Init->AllyFields[i].final.text);
        //ls_unistrSet(&curr->allyAC[i], State.Init->AllyFields[i].AC.text);
    }
    
    u32 throwerCount = 0;
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        if(State.Init->Throwers[i].name.text.len == 0)   { continue; }
        if(State.Init->Throwers[i].toHit.text.len == 0)  { continue; }
        if(State.Init->Throwers[i].damage.text.len == 0) { continue; }
        
        ls_unistrSet(&curr->throwerName[i], State.Init->Throwers[i].name.text);
        ls_unistrSet(&curr->throwerHit[i], State.Init->Throwers[i].toHit.text);
        ls_unistrSet(&curr->throwerDamage[i], State.Init->Throwers[i].damage.text);
        
        throwerCount += 1;
    }
    curr->numThrowers = throwerCount;
    
    
    State.encounters.numEncounters += 1;
    
    ls_uiListBoxAddEntry(cxt, &State.Init->EncounterSel, State.Init->EncounterName.text);
}

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
    Page->roundCount = 0;
    
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

void InitFieldInit(UIContext *cxt, InitField *f, s32 *currID, const char32_t *name)
{
    f->name.text          = ls_unistrFromUTF32(name);
    f->name.viewEndIdx    = f->name.text.len;
    f->name.preInput      = CustomInitFieldText;
    f->name.data          = f;
    
    f->bonus.text         = ls_unistrFromUTF32(U"0");
    f->bonus.viewEndIdx   = f->bonus.text.len;
    f->bonus.maxLen       = 2;
    f->bonus.preInput     = CustomInitFieldText;
    f->bonus.data         = f;
    
    f->final.text         = ls_unistrFromUTF32(U"0");
    f->final.viewEndIdx   = f->final.text.len;
    f->final.maxLen       = 2;
    f->final.preInput     = CustomInitFieldText;
    f->final.data         = f;
    
    f->extra.text         = ls_unistrAlloc(16);
    f->extra.viewEndIdx   = f->extra.text.len;
    
    f->maxLife.text       = ls_unistrFromUTF32(U"0");
    f->maxLife.viewEndIdx = f->maxLife.text.len;
    f->maxLife.maxLen     = 4;
    f->maxLife.preInput   = 0x0;
    f->maxLife.data       = 0x0;
    
    f->totalAC.text       = ls_unistrFromUTF32(U"0");
    f->totalAC.viewEndIdx = f->totalAC.text.len;
    f->totalAC.maxLen     = 2;
    f->totalAC.preInput   = 0x0;
    f->totalAC.data       = 0x0;
    
    f->touchAC.text       = ls_unistrFromUTF32(U"0");
    f->touchAC.viewEndIdx = f->touchAC.text.len;
    f->touchAC.maxLen     = 2;
    f->touchAC.preInput   = 0x0;
    f->touchAC.data       = 0x0;
    
    f->flatAC.text        = ls_unistrFromUTF32(U"0");
    f->flatAC.viewEndIdx  = f->flatAC.text.len;
    f->flatAC.maxLen      = 2;
    f->flatAC.preInput    = 0x0;
    f->flatAC.data        = 0x0;
    
    f->lowAC.text         = ls_unistrFromUTF32(U"0");
    f->lowAC.viewEndIdx   = f->lowAC.text.len;
    f->lowAC.maxLen       = 2;
    f->lowAC.preInput     = 0x0;
    f->lowAC.data         = 0x0;
    
    f->conSave.text       = ls_unistrFromUTF32(U"0");
    f->conSave.viewEndIdx = f->conSave.text.len;
    f->conSave.maxLen     = 2;
    f->conSave.preInput   = 0x0;
    f->conSave.data       = 0x0;
    
    f->dexSave.text       = ls_unistrFromUTF32(U"0");
    f->dexSave.viewEndIdx = f->dexSave.text.len;
    f->dexSave.maxLen     = 2;
    f->dexSave.preInput   = 0x0;
    f->dexSave.data       = 0x0;
    
    f->wisSave.text       = ls_unistrFromUTF32(U"0");
    f->wisSave.viewEndIdx = f->wisSave.text.len;
    f->wisSave.maxLen     = 2;
    f->wisSave.preInput   = 0x0;
    f->wisSave.data       = 0x0;
    
    f->addName.text       = ls_unistrAlloc(16);
    f->addInit.text       = ls_unistrAlloc(16);
    f->addInit.maxLen     = 2;
    
    f->addNew.style       = UIBUTTON_TEXT;
    f->addNew.name        = ls_unistrFromUTF32(U"+");
    f->addNew.onClick     = AddNewInitOnClick;
    f->addNew.data        = f;
    f->addNew.onHold      = 0x0;
    
    f->addConfirm.style   = UIBUTTON_TEXT;
    f->addConfirm.name    = ls_unistrFromUTF32(U"Ok");
    f->addConfirm.onClick = AddConfirmOnClick;
    f->addConfirm.data    = f;
    f->addConfirm.onHold  = 0x0;
    
    
    f->ID = *currID;
    *currID += 1;
}

void SetInitTab(UIContext *cxt)
{
    InitPage *Page = State.Init;
    
    for(u32 i = 0; i < MOB_NUM + 1; i++) { ls_uiListBoxAddEntry(cxt, &Page->Mobs, (char *)Enemies[i]); }
    for(u32 i = 0; i < ALLY_NUM + 1; i++) { ls_uiListBoxAddEntry(cxt, &Page->Allies, (char *)Allies[i]); }
    
    for(u32 i = 0; i < PARTY_NUM; i++) 
    { 
        UITextBox *f = Page->PlayerInit + i;
        f->text       = ls_unistrFromUTF32(U"0");
        f->viewEndIdx = f->text.len;
        f->maxLen     = 2;
        f->preInput   = CustomPlayerText;
        f->data       = f;
    }
    
    s32 currID = PARTY_NUM;
    for(u32 i = 0; i < MOB_NUM; i++)   
    { 
        InitField *f = Page->MobFields + i;
        
        InitFieldInit(cxt, f, &currID, MobName[i]);
    }
    
    for(u32 i = 0; i < ALLY_NUM; i++)  
    { 
        InitField *f = Page->AllyFields + i;
        
        InitFieldInit(cxt, f, &currID, AllyName[i]);
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
        
        f->remove.style   = UIBUTTON_TEXT;
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
        
        f->start.style   = UIBUTTON_TEXT;
        f->start.name    = ls_unistrFromUTF32(U"Start");
        f->start.onClick = StartCounterOnClick;
        f->start.data    = (void *)f;
        f->start.onHold  = 0x0;
        
        f->plusOne.style   = UIBUTTON_TEXT;
        f->plusOne.name    = ls_unistrFromUTF32(U"+1");
        f->plusOne.onClick = PlusOneCounterOnClick;
        f->plusOne.data    = (void *)f;
        f->plusOne.onHold  = 0x0;
        
        f->stop.style   = UIBUTTON_TEXT;
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
        
        f->throwDie.style   = UIBUTTON_TEXT;
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
        
        Page->GeneralThrower.throwDie.style   = UIBUTTON_TEXT;
        Page->GeneralThrower.throwDie.name    = ls_unistrFromUTF32(U"Go");
        Page->GeneralThrower.throwDie.onClick = ThrowDiceOnClick;
        Page->GeneralThrower.throwDie.data    = &Page->GeneralThrower.throwDie;
        Page->GeneralThrower.throwDie.onHold  = 0x0;
    }
    
    ls_uiListBoxAddEntry(cxt, &Page->EncounterSel, ls_unistrConstant(NoEncounterStr));
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    { ls_uiListBoxAddEntry(cxt, &Page->EncounterSel, State.encounters.Enc[i].name); }
    
    Page->EncounterName.text = ls_unistrAlloc(16);
    
    Page->Save.style   = UIBUTTON_TEXT;
    Page->Save.name    = ls_unistrFromUTF32(U"Save");
    Page->Save.onClick = SaveEncounterOnClick;
    Page->Save.data    = 0x0;
    Page->Save.onHold  = 0x0;
    
    
    Page->Current.text            = ls_unistrAlloc(16);
    Page->Current.isReadonly      = TRUE;
    
    Page->RoundCounter.text       = ls_unistrFromUTF32(U"0");
    Page->RoundCounter.viewEndIdx = Page->RoundCounter.text.len;
    Page->RoundCounter.isReadonly = TRUE;
    
    Page->Roll.style    = UIBUTTON_TEXT;
    Page->Roll.name     = ls_unistrFromUTF32(U"Roll");
    Page->Roll.onClick  = RollOnClick;
    Page->Roll.onHold   = 0x0;
    
    Page->Set.style     = UIBUTTON_TEXT;
    Page->Set.name      = ls_unistrFromUTF32(U"Set");
    Page->Set.onClick   = SetOnClick;
    Page->Set.onHold    = 0x0;
    
    Page->Reset.style   = UIBUTTON_TEXT;
    Page->Reset.name    = ls_unistrFromUTF32(U"Reset");
    Page->Reset.onClick = ResetOnClick;
    Page->Reset.onHold  = 0x0;
    
    Page->Next.style    = UIBUTTON_TEXT;
    Page->Next.name     = ls_unistrFromUTF32(U"Next");
    Page->Next.onClick  = NextOnClick;
    Page->Next.onHold   = 0x0;
}

void DrawInitExtra(UIContext *cxt, InitField *F, s32 baseX, s32 y)
{
    Color base = cxt->widgetColor;
    s32 x = baseX - 372; //276;
    
    ls_uiTextBox(cxt, &F->extra, x, y, 82, 20);   x += 96;
    
    cxt->widgetColor = ls_uiAlphaBlend(RGBA(0x1B, 0x18, 0x14, 150), base);
    ls_uiTextBox(cxt, &F->maxLife, x, y, 42, 20); x += 56;
    
    cxt->widgetColor = ls_uiAlphaBlend(RGBA(0x61, 0x3B, 0x09, 150), base);
    ls_uiTextBox(cxt, &F->totalAC, x, y, 26, 20); x += 30;
    
    cxt->widgetColor = ls_uiAlphaBlend(RGBA(0x9C, 0x43, 0x8B, 150), base);
    ls_uiTextBox(cxt, &F->touchAC, x, y, 26, 20); x += 30;
    
    cxt->widgetColor = ls_uiAlphaBlend(RGBA(0xD5, 0xCB, 0x35, 150), base);
    ls_uiTextBox(cxt, &F->flatAC,  x, y, 26, 20); x += 30;
    
    cxt->widgetColor = ls_uiAlphaBlend(RGBA(0x75, 0x46, 0x46, 150), base);
    ls_uiTextBox(cxt, &F->lowAC,   x, y, 26, 20); x += 40;
    
    cxt->widgetColor = ls_uiAlphaBlend(RGBA(0xDD, 0x10, 0x20, 150), base);
    ls_uiTextBox(cxt, &F->conSave, x, y, 26, 20); x += 30;
    
    cxt->widgetColor = ls_uiAlphaBlend(RGBA(0x2C, 0x80, 0x34, 150), base);
    ls_uiTextBox(cxt, &F->dexSave, x, y, 26, 20); x += 30;
    
    cxt->widgetColor = ls_uiAlphaBlend(RGBA(0x2C, 0x3D, 0x80, 150), base);
    ls_uiTextBox(cxt, &F->wisSave, x, y, 26, 20); x += 30;
    
    cxt->widgetColor = base;
}

void DrawInitField(UIContext *cxt, InitField *F, s32 baseX, s32 y)
{
    s32 w = 120;
    
    InitPage *Page = State.Init;
    
    s32 x = baseX;
    ls_uiTextBox(cxt, &F->name,    x         , y, w, 20);
    ls_uiTextBox(cxt, &F->bonus,   x + w     , y, 26, 20);
    ls_uiTextBox(cxt, &F->final,   x + w + 26, y, 26, 20);
}

void DrawOrderField(UIContext *cxt, Order *f, s32 xPos, s32 yPos)
{
    ls_uiSlider(cxt, &f->field, xPos + 50, yPos, 120, 20);
    ls_uiTextBox(cxt, &f->pos, xPos + 25, yPos, 32, 20);
    ls_uiButton(cxt, &f->remove, xPos, yPos, 20, 20);
}

void DrawDefaultStyle(UIContext *cxt)
{
    InitPage *Page = State.Init;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    
    // Party
    s32 yPos = 638;
    for(u32 i = 0; i < PARTY_NUM; i++)
    {
        ls_uiLabel(cxt, ls_unistrConstant(PartyName[i]), 650, yPos+6);
        ls_uiTextBox(cxt, Page->PlayerInit + i, 732, yPos, 32, 20);
        yPos -= 20;
    }
    
    // Allies
    yPos = 458;
    for(u32 i = 0; i < visibleAllies; i++)
    {
        DrawInitField(cxt, Page->AllyFields + i, 616, yPos);
        yPos -= 20;
    }
    
    // Enemies
    yPos = 638;
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        DrawInitField(cxt, Page->MobFields + i, 378, yPos);
        yPos -= 20;
    }
    
    // Order
    yPos = 638;
    for(u32 i = 0; i < visibleOrder; i += 2)
    {
        DrawOrderField(cxt, Page->OrderFields + i, 870, yPos);
        
        if((i+1) < visibleOrder)
        { DrawOrderField(cxt, Page->OrderFields + (i+1), 1056, yPos); }
        
        yPos -= 20;
    }
    
    // Counters
    if(!Page->InfoPane.isOpen)
    {
        yPos = 638;
        for(u32 i = 0; i < DEF_COUNTER_NUM; i++)
        {
            Counter *f = Page->Counters + i;
            
            ls_uiLabel(cxt, ls_unistrConstant(CounterNames[i]), 20, yPos+24);
            
            ls_uiTextBox(cxt, &f->name, 20, yPos, 100, 20);
            ls_uiTextBox(cxt, &f->rounds, 125, yPos, 36, 20);
            
            if(!f->isActive) { ls_uiButton(cxt, &f->start, 166, yPos, 48, 20); }
            else
            {
                ls_uiButton(cxt, &f->plusOne, 166, yPos, 48, 20);
                ls_uiButton(cxt, &f->stop, 113, yPos+22, 48, 20);
            }
            
            yPos -= 44;
        }
    }
    
    // Mob Info Left Pane
    ls_uiLPane(cxt, &Page->InfoPane, 0, 180, 360, 580);
    
    //Mob Info
    if(Page->InfoPane.isOpen)
    {
        u32 infoX = 2;
        u32 infoY = 742;
        for(u32 i = 0; i < visibleMobs; i++)
        {
            InitField *f = Page->MobFields + i;
            
            ls_uiLabel(cxt, f->name.text, infoX, infoY);
            
            Color base = cxt->widgetColor;
            
            cxt->widgetColor = ls_uiAlphaBlend(RGBA(0x61, 0x3B, 0x09, 150), base);
            ls_uiTextBox(cxt, &f->totalAC, infoX + 88, infoY-6, 26, 20);
            
            cxt->widgetColor = ls_uiAlphaBlend(RGBA(0x9C, 0x43, 0x8B, 150), base);
            ls_uiTextBox(cxt, &f->touchAC, infoX + 116, infoY-6, 26, 20);
            
            cxt->widgetColor = ls_uiAlphaBlend(RGBA(0xD5, 0xCB, 0x35, 150), base);
            ls_uiTextBox(cxt, &f->flatAC, infoX + 144, infoY-6, 26, 20);
            
            cxt->widgetColor = ls_uiAlphaBlend(RGBA(0x75, 0x46, 0x46, 150), base);
            ls_uiTextBox(cxt, &f->lowAC, infoX + 144, infoY-6, 26, 20);
            
            cxt->widgetColor = ls_uiAlphaBlend(RGBA(0x1B, 0x18, 0x14, 150), base);
            ls_uiTextBox(cxt, &f->maxLife, infoX + 182, infoY-6, 42, 20);
            
            cxt->widgetColor = base;
            
            infoY -= 24;
        }
    }
    
    // Dice Throwers
    s32 xPos = 20;
    yPos = 90;
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
    yPos = 42;
    // General Thrower
    {
        ls_uiTextBox(cxt, &Page->GeneralThrower.toHit,  xPos,       yPos, 168, 20);
        ls_uiTextBox(cxt, &Page->GeneralThrower.hitRes, xPos + 168, yPos, 36,  20);
        
        ls_uiButton(cxt, &Page->GeneralThrower.throwDie, xPos + 210, yPos, 48, 20);
    }
    
    
    
    ls_uiButton(cxt, &Page->Reset, 670, 678, 48, 20);
    
    
    if(!State.inBattle)
    {
        //TODO: Clicking on a ListBox Entry clicks also what's behind it.
        ls_uiListBox(cxt, &Page->Mobs,   406, 678, 100, 20);
        ls_uiListBox(cxt, &Page->Allies, 641, 498, 100, 20);
        
        ls_uiButton(cxt, &Page->Roll, 556, 678, 48, 20);
        ls_uiButton(cxt, &Page->Set,  780, 678, 48, 20);
        
        ls_uiListBox(cxt, &Page->EncounterSel,  480, 718, 120, 20);
        ls_uiTextBox(cxt, &Page->EncounterName, 624, 718, 100, 20);
        
        ls_uiButton(cxt, &Page->Save, 601, 740, 44, 20);
    }
    else
    {
        ls_uiTextBox(cxt, &Page->Current,      1010, 668, 100, 20);
        ls_uiTextBox(cxt, &Page->RoundCounter, 1230, 720, 30, 20);
        
        ls_uiButton(cxt, &Page->Next, 1036, 698, 48, 20);
        
        if(visibleAllies < ALLY_NUM)
        {
            InitField *f = Page->AllyFields + visibleAllies;
            
            u32 addY = 454 - (20*visibleAllies);
            if(!f->isAdding)
            {
                ls_uiButton(cxt, &f->addNew, 678, addY, 36, 20);
            }
            else
            {
                ls_uiTextBox(cxt, &f->addName, 616, addY+4, 120, 20);
                ls_uiTextBox(cxt, &f->addInit, 736, addY+4, 26, 20);
                
                ls_uiButton(cxt, &f->addConfirm, 762, addY+4, 26, 20);
            }
        }
        
        if(visibleMobs < MOB_NUM)
        {
            InitField *f = Page->MobFields + visibleMobs;
            
            u32 addY = 634 - (20*visibleMobs);
            if(!f->isAdding)
            {
                ls_uiButton(cxt, &f->addNew, 428, addY, 36, 20);
            }
            else
            {
                ls_uiTextBox(cxt, &f->addName, 378, addY+4, 120, 20);
                ls_uiTextBox(cxt, &f->addInit, 498, addY+4, 26, 20);
                
                ls_uiButton(cxt, &f->addConfirm, 524, addY+4, 26, 20);
            }
        }
    }
}

void DrawPranaStyle(UIContext *cxt)
{
    InitPage *Page = State.Init;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    
    // Party
    s32 yPos = 638;
    for(u32 i = 0; i < PARTY_NUM; i++)
    {
        ls_uiLabel(cxt, ls_unistrConstant(PartyName[i]), 650, yPos+6);
        ls_uiTextBox(cxt, Page->PlayerInit + i, 732, yPos, 32, 20);
        yPos -= 20;
    }
    
    // Allies
    yPos = 458;
    for(u32 i = 0; i < visibleAllies; i++)
    {
        DrawInitField(cxt, Page->AllyFields + i, 616, yPos);
        yPos -= 20;
    }
    
    // Enemies
    yPos = 638;
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        DrawInitField(cxt, f, 378, yPos);
        DrawInitExtra(cxt, f, 378, yPos);
        yPos -= 20;
    }
    
    // Order
    yPos = 638;
    for(u32 i = 0; i < visibleOrder; i += 2)
    {
        DrawOrderField(cxt, Page->OrderFields + i, 870, yPos);
        
        if((i+1) < visibleOrder)
        { DrawOrderField(cxt, Page->OrderFields + (i+1), 1056, yPos); }
        
        yPos -= 20;
    }
    
    // Counters
    yPos = 124;
    s32 xPos = 20;
    s32 xDiff = 0;
    for(u32 i = 0; i < PRANA_COUNTER_NUM; i++)
    {
        Counter *f = Page->Counters + i;
        
        if((i % 3) == 0) { yPos = 124; }
        if((i % 3) == 1) { yPos = 76; }
        if((i % 3) == 2) { yPos = 28; xDiff = 216; }
        
        ls_uiLabel(cxt, ls_unistrConstant(CounterNames[i]), xPos, yPos+24);
        
        ls_uiTextBox(cxt, &f->name, xPos, yPos, 100, 20);
        ls_uiTextBox(cxt, &f->rounds, xPos + 105, yPos, 36, 20);
        
        if(!f->isActive) { ls_uiButton(cxt, &f->start, xPos + 146, yPos, 48, 20); }
        else
        {
            ls_uiButton(cxt, &f->plusOne, xPos + 146, yPos, 48, 20);
            ls_uiButton(cxt, &f->stop, xPos + 93, yPos+22, 48, 20);
        }
        
        xPos += xDiff;
        xDiff = 0;
    }
    
    // Dice Throwers
    xPos = 512;
    yPos = 100;
    for(u32 i = 0; i < PRANA_THROWER_NUM; i++)
    {
        DiceThrow *f = Page->Throwers + i;
        
        if((i % 2) == 0) { yPos = 100; }
        if((i % 2) == 1) { yPos = 20; xDiff = 186; }
        
        
        ls_uiTextBox(cxt, &f->name,    xPos,      yPos + 40, 98, 20);
        ls_uiButton(cxt, &f->throwDie, xPos + 98, yPos + 40, 36, 20);
        
        ls_uiTextBox(cxt, &f->damage, xPos,       yPos + 20, 134, 20);
        ls_uiTextBox(cxt, &f->dmgRes, xPos + 134, yPos + 20, 36,  20);
        
        ls_uiTextBox(cxt, &f->toHit,  xPos,       yPos, 134, 20);
        ls_uiTextBox(cxt, &f->hitRes, xPos + 134, yPos, 36,  20);
        
        xPos += xDiff;
        xDiff = 0;
    }
    
    ls_uiButton(cxt, &Page->Reset, 670, 678, 48, 20);
    
    if(!State.inBattle)
    {
        //TODO: Clicking on a ListBox Entry clicks also what's behind it.
        ls_uiListBox(cxt, &Page->Mobs,   406, 678, 100, 20);
        ls_uiListBox(cxt, &Page->Allies, 641, 498, 100, 20);
        
        ls_uiButton(cxt, &Page->Roll, 556, 678, 48, 20);
        ls_uiButton(cxt, &Page->Set,  780, 678, 48, 20);
        
        ls_uiListBox(cxt, &Page->EncounterSel,  480, 718, 120, 20);
        ls_uiTextBox(cxt, &Page->EncounterName, 624, 718, 100, 20);
        
        ls_uiButton(cxt, &Page->Save, 601, 740, 44, 20);
    }
    else
    {
        ls_uiTextBox(cxt, &Page->Current,      1010, 668, 100, 20);
        ls_uiTextBox(cxt, &Page->RoundCounter, 1230, 720, 30, 20);
        
        ls_uiButton(cxt, &Page->Next, 1036, 698, 48, 20);
        
        if(visibleAllies < ALLY_NUM)
        {
            InitField *f = Page->AllyFields + visibleAllies;
            
            u32 addY = 454 - (20*visibleAllies);
            if(!f->isAdding)
            {
                ls_uiButton(cxt, &f->addNew, 678, addY, 36, 20);
            }
            else
            {
                ls_uiTextBox(cxt, &f->addName, 616, addY+4, 120, 20);
                ls_uiTextBox(cxt, &f->addInit, 736, addY+4, 26, 20);
                
                ls_uiButton(cxt, &f->addConfirm, 762, addY+4, 26, 20);
            }
        }
        
        if(visibleMobs < MOB_NUM)
        {
            InitField *f = Page->MobFields + visibleMobs;
            
            u32 addY = 634 - (20*visibleMobs);
            if(!f->isAdding)
            {
                ls_uiButton(cxt, &f->addNew, 428, addY, 36, 20);
            }
            else
            {
                ls_uiTextBox(cxt, &f->addName, 378, addY+4, 120, 20);
                ls_uiTextBox(cxt, &f->addInit, 498, addY+4, 26, 20);
                
                ls_uiButton(cxt, &f->addConfirm, 524, addY+4, 26, 20);
            }
        }
    }
    
    return;
}

void DrawInitTab(UIContext *cxt)
{
    switch(currentStyle)
    {
        case INIT_STYLE_DEFAULT: DrawDefaultStyle(cxt); return;
        case INIT_STYLE_PRANA:   DrawPranaStyle(cxt); return;
        
        default: AssertMsg(FALSE, "Unhandled Init Style\n"); return;
    }
    
    return;
}