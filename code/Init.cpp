b32 RequestUndoOnClick(UIContext *c, void *data)
{
    undoRequest = TRUE;
    return FALSE;
}

b32 RequestRedoOnClick(UIContext *c, void *data)
{
    redoRequest = TRUE;
    return FALSE;
}

Order *GetOrderByID(s32 ID)
{
    s32 visibleMobs   = State.Init->Mobs.selectedIndex;
    s32 visibleAllies = State.Init->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - State.Init->orderAdjust;
    
    for(u32 i = 0; i < visibleOrder; i++)
    {
        Order *o = State.Init->OrderFields + i;
        if(o->ID == ID) { return o; }
    }
    
    return NULL;
}

InitField *GetInitFieldByID(s32 ID)
{
    s32 visibleMobs   = State.Init->Mobs.selectedIndex;
    s32 visibleAllies = State.Init->Allies.selectedIndex;
    
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *m = State.Init->MobFields + i;
        if(m->ID == ID) { return m; }
    }
    
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *a = State.Init->AllyFields + i;
        if(a->ID == ID) { return a; }
    }
    
    return NULL;
}

b32 selectStyleDefault(UIContext *c, void *data)
{
    currentStyle = INIT_STYLE_DEFAULT;
    return FALSE;
}

b32 selectStylePrana(UIContext *c, void *data)
{
    currentStyle = INIT_STYLE_PRANA;
    return FALSE;
}


b32 selectThemeDefault(UIContext *c, void *data)
{
    c->backgroundColor = RGBg(0x38);
    c->highliteColor   = RGBg(0x65);
    c->pressedColor    = RGBg(0x75);
    c->widgetColor     = RGBg(0x45);
    c->borderColor     = RGBg(0x22);
    c->textColor       = RGBg(0xCC);
    c->invWidgetColor  = RGBg(0xBA);
    c->invTextColor    = RGBg(0x33);
    
    return FALSE;
}

b32 selectThemeDarkNight(UIContext *c, void *data)
{
    c->backgroundColor = RGB(0x0C, 0x08, 0x0B);
    c->highliteColor   = RGB(0x14, 0x15, 0x13);
    c->pressedColor    = RGB(0x18, 0x20, 0x19);
    c->widgetColor     = RGB(0x1C, 0x18, 0x1B);
    c->borderColor     = RGB(0x60, 0x7B, 0x7D);
    c->textColor       = RGBg(0xCC);
    c->invWidgetColor  = RGB(0xE3, 0xE7, 0xE4);
    c->invTextColor    = RGB(0x7D, 0x71, 0x7D);
    
    return FALSE;
}


b32 CustomPlayerText(UIContext *c, void *data)
{
    Input *UserInput = &c->UserInput;
    b32 inputUse = FALSE;
    
    InitPage *Page = State.Init;
    
    UITextBox *f = (UITextBox *)data;
    
    if(c->lastFocus != (u64 *)f) { ls_uiTextBoxClear(c, f); inputUse = TRUE; }
    
    if(KeyPress(keyMap::Enter))
    {
        UITextBox *lastPlayerBox = Page->PlayerInit + (PARTY_NUM - 1);
        
        if(f == lastPlayerBox) { ls_uiFocusChange(c, 0x0); return TRUE; }
        
        ls_uiFocusChange(c, (u64 *)(f + 1));
        return TRUE;
    }
    
    return inputUse;
}

b32 CustomInitFieldText(UIContext *c, void *data)
{
    Input *UserInput = &c->UserInput;
    b32 inputUse = FALSE;
    
    CustomFieldTextHandler *f = (CustomFieldTextHandler *)data;
    
    InitPage *Page = State.Init;
    
    if((c->lastFocus != (u64 *)f->field) && (c->currentFocus == (u64 *)f->field))
    { ls_uiTextBoxClear(c, f->field); inputUse = TRUE; }
    
    InitField *lastMob  = State.Init->MobFields  + (MOB_NUM-1);
    InitField *lastAlly = State.Init->AllyFields + (ALLY_NUM-1);
    
    if(KeyPress(keyMap::Enter))
    {
        //NOTE: Clear up the '\n' printable key
        ClearPrintableKey();
        
        if(f->parent == lastMob)  { ls_uiFocusChange(c, 0x0); return inputUse; }
        if(f->parent == lastAlly) { ls_uiFocusChange(c, 0x0); return inputUse; }
        
        //NOTE: The maxLife field gets handled specially using the index 999
        if(f->if_idx == 999) {
            UITextBox *next = &((f->parent + 1)->maxLife);
            ls_uiTextBoxClear(c, next);
            ls_uiFocusChange(c, (u64 *)next);
            
            return TRUE;
        }
        
        AssertMsg(f->if_idx < INIT_FIELD_EDITFIELDS_NUM, "Out of bounds index\n");
        
        UITextBox *next = &((f->parent + 1)->editFields[f->if_idx]);
        ls_uiTextBoxClear(c, next);
        ls_uiFocusChange(c, (u64 *)next);
        
        return TRUE;
    }
    
    UITextBox *parentFields = f->parent->editFields;
    
    if(KeyPress(keyMap::DArrow))
    {
        if(f->parent == lastMob)  { ls_uiFocusChange(c, 0x0); return inputUse; }
        if(f->parent == lastAlly) { ls_uiFocusChange(c, 0x0); return inputUse; }
        
        u32 numStrings = 0;
        utf32 *words = ls_utf32SeparateByNumber(parentFields[IF_IDX_NAME].text, &numStrings);
        
        AssertMsg(words, "The returned words array was null.\n");
        if(!words) { ls_uiFocusChange(c, 0x0); return inputUse; }
        
        if(numStrings != 2) { ls_uiFocusChange(c, 0x0); return inputUse; }
        
        s64 newNumber = ls_utf32ToInt(words[1]) + 1;
        ls_utf32FromInt_t(&words[1], newNumber);
        
        InitField *next = f->parent + 1;
        UITextBox *nextFields = next->editFields;
        
        ls_uiTextBoxClear(c, &nextFields[IF_IDX_NAME]);
        ls_utf32Set(&nextFields[IF_IDX_NAME].text, words[0]);
        ls_utf32Append(&nextFields[IF_IDX_NAME].text, words[1]);
        nextFields[IF_IDX_NAME].viewEndIdx = nextFields[IF_IDX_NAME].text.len;
        
        //NOTE: We start at i == 1, because the name field has already been handled,
        //      We stop at COUNT-1 because the last field is 'final' which should not be modified by this routine.
        for(u32 i = 1; i < IF_IDX_COUNT-1; i++)
        {
            ls_uiTextBoxClear(c, &nextFields[i]);
            ls_uiTextBoxSet(c, &nextFields[i], parentFields[i].text);
        }
        
        ls_uiTextBoxClear(c, &next->maxLife);
        ls_uiTextBoxSet(c, &next->maxLife, f->parent->maxLife.text);
        
        //NOTE: The maxLife field gets handled specially using the index 999
        if(f->if_idx == 999) { ls_uiFocusChange(c, (u64 *)&(next->maxLife)); return inputUse; }
        
        AssertMsg(f->if_idx < INIT_FIELD_EDITFIELDS_NUM, "Out of bounds index\n");
        ls_uiFocusChange(c, (u64 *)&(nextFields[f->if_idx]));
        
        return TRUE;
    }
    
    return inputUse;
}


b32 CustomMobLifeField(UIContext *c, void *data)
{
    Input *UserInput = &c->UserInput;
    b32 inputUse = FALSE;
    
    MobLifeHandler *h = (MobLifeHandler *)data;
    UITextBox *f = h->parent;
    
    if(!State.inBattle) {
        CustomFieldTextHandler dummy = {h->mob, h->parent, 999};
        inputUse |= CustomInitFieldText(c, &dummy);
    };
    
    //NOTE: We lost focus, let's reset the box
    if((c->lastFocus != (u64 *)f) && h->isEditing) { h->isEditing = FALSE; }
    
    if(LeftClick && !h->isEditing && State.inBattle)
    { 
        ls_utf32Set(&h->previous, f->text);
        ls_uiTextBoxClear(c, f);
        
        h->isEditing = TRUE;
        inputUse = TRUE;
    }
    
    if(h->isEditing && KeyPress(keyMap::Enter))
    {
        Order *order = GetOrderByID(h->mob->ID);
        AssertMsg(order, "Could not find order by ID. Fucked up ID?\n");
        
        s32 diff = ls_utf32ToInt(f->text);
        
        ls_uiSliderChangeValueBy(c, &order->field, diff);
        
        s32 currVal = ls_uiSliderGetValue(c, &order->field);
        if(currVal == 0)
        { order->field.rColor = ls_uiAlphaBlend(RGBA(0xFF, 0x97, 0x12, 0x99), c->widgetColor); }
        
        else if(currVal < 0)
        { order->field.rColor = ls_uiAlphaBlend(RGBA(0xDD, 0x10, 0x20, 0x99), c->widgetColor); }
        
        else if(currVal > 0)
        { order->field.rColor = ls_uiAlphaBlend(RGBA(0xF0, 0xFF, 0x3D, 0x99), c->widgetColor); }
        
        ls_uiTextBoxClear(c, f);
        ls_uiTextBoxSet(c, f, h->previous);
        
        h->isEditing = FALSE;
        ls_uiFocusChange(c, NULL);
        
        inputUse = TRUE;
    }
    
    return inputUse;
}

b32 ChangeOrder(UIContext *c, void *data)
{
    Input *UserInput = &c->UserInput;
    b32 inputUse = FALSE;
    
    OrderHandler *h = (OrderHandler *)data;
    UITextBox *f = h->parent;
    
    AssertMsg(State.inBattle, "How did we even get here?\n");
    
    if(LeftClick && !h->isEditing)
    { 
        ls_utf32Set(&h->previous, f->text);
        ls_uiTextBoxClear(c, f);
        
        h->isEditing = TRUE;
        inputUse = TRUE;
    }
    
    
    if(h->isEditing && KeyPress(keyMap::Enter))
    {
        InitPage *Init = State.Init;
        
        s32 visibleMobs   = Init->Mobs.selectedIndex;
        s32 visibleAllies = Init->Allies.selectedIndex;
        s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Init->orderAdjust;
        
        Order *order = h->order;
        
        s32 newPosition = ls_utf32ToInt(f->text);
        s32 oldPosition = ls_utf32ToInt(h->previous);
        
        if((newPosition >= visibleOrder) || (newPosition == oldPosition) || (newPosition < 0))
        {
            ls_uiTextBoxClear(c, f);
            ls_uiTextBoxSet(c, f, h->previous);
            
            h->isEditing = FALSE;
            ls_uiFocusChange(c, NULL);
            return TRUE;
        }
        
        
        //-------------------------------
        //TODO: Horrible temp saving...
        //
        Order *oldOrder = &Init->OrderFields[oldPosition];
        
        utf32 oldName = {}; 
        ls_utf32Set(&oldName, oldOrder->field.text);
        
        s32 oldID = oldOrder->ID;
        
        s32 slideCurr = oldOrder->field.currValue;
        s32 slideMax = oldOrder->field.maxValue;
        s32 slideMin = oldOrder->field.minValue;
        f64 slidePos = oldOrder->field.currPos;
        //
        //TODO: Horrible temp saving...
        //-------------------------------
        
        //TODO: Make branchless by getting the diff (new-old) and obtaining either -1 or +1 for the j offset.
        //      Problem with <= / >= newPosition???
        if(newPosition > oldPosition)
        {
            for(u32 j = oldPosition; j <= newPosition; j++)
            {
                UISlider *curr = &Init->OrderFields[j].field;
                UISlider *next = &Init->OrderFields[j+1].field;
                
                ls_utf32Set(&curr->text, next->text);
                Init->OrderFields[j].ID = Init->OrderFields[j+1].ID;
                
                curr->currValue = next->currValue;
                curr->maxValue  = next->maxValue;
                curr->minValue  = next->minValue;
                curr->currPos   = next->currPos;
            }
        }
        
        if(newPosition < oldPosition)
        {
            for(u32 j = oldPosition; j > newPosition; j--)
            {
                UISlider *curr = &Init->OrderFields[j].field;
                UISlider *prev = &Init->OrderFields[j-1].field;
                
                ls_utf32Set(&curr->text, prev->text);
                Init->OrderFields[j].ID = Init->OrderFields[j-1].ID;
                
                curr->currValue = prev->currValue;
                curr->maxValue  = prev->maxValue;
                curr->minValue  = prev->minValue;
                curr->currPos   = prev->currPos;
            }
        }
        
        ls_utf32Set(&Init->OrderFields[newPosition].field.text, oldName);
        
        Init->OrderFields[newPosition].ID = oldID;
        Init->OrderFields[newPosition].field.currValue = slideCurr;
        Init->OrderFields[newPosition].field.maxValue  = slideMax;
        Init->OrderFields[newPosition].field.minValue  = slideMin;
        Init->OrderFields[newPosition].field.currPos   = slidePos;
        
        //NOTE: Reset the position to actual ordinal
        ls_uiTextBoxClear(c, f);
        ls_uiTextBoxSet(c, f, h->previous);
        
        //NOTE: Update "Current" TextBox to reflect change if there was any
        //NOTE:TODO: Do I actually like this? Do we want this?
        if(Init->currIdx == newPosition)
        { ls_uiTextBoxSet(c, &Init->Current, oldName); }
        
        ls_utf32Free(&oldName); //TODO: yuck... look upward at horrible temp saving.
        
        h->isEditing = FALSE;
        ls_uiFocusChange(c, NULL);
        
        return TRUE;
    }
    
    return inputUse;
}

b32 ResetOnClick(UIContext *, void *);
void OnEncounterSelect(UIContext *c, void *data)
{
    if(State.inBattle) return;
    
    UIListBox *b = (UIListBox *)data;
    
    u32 idx = b->selectedIndex;
    
    if(idx == 0) { ResetOnClick(c, 0); return; }
    
    Encounter *e = &State.encounters.Enc[idx-1];
    
    State.Init->Mobs.selectedIndex   = e->numMobs;
    State.Init->Allies.selectedIndex = e->numAllies;
    
    for(u32 i = 0; i < e->numMobs; i++)
    {
        InitField *m = State.Init->MobFields + i;
        EncounterInitEntry *entry = e->mob + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_uiTextBoxSet(c, &m->editFields[j], entry->fields[j]); }
        
        ls_uiTextBoxSet(c, &m->maxLife, entry->fields[MOB_INIT_ENC_FIELDS-1]);
        m->compendiumIdx = entry->compendiumIdx;
    }
    
    for(u32 i = 0; i < e->numAllies; i++)
    {
        InitField *a = State.Init->AllyFields + i;
        EncounterInitEntry *entry = e->ally + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_uiTextBoxSet(c, &a->editFields[j], entry->fields[j]); }
        
        ls_uiTextBoxSet(c, &a->maxLife, entry->fields[MOB_INIT_ENC_FIELDS-1]);
        a->compendiumIdx = entry->compendiumIdx;
    }
    
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrow *t = State.Init->Throwers + i;
        
        ls_uiTextBoxClear(c, &t->name);
        ls_uiTextBoxClear(c, &t->toHit);
        ls_uiTextBoxClear(c, &t->hitRes);
        ls_uiTextBoxClear(c, &t->damage);
        ls_uiTextBoxClear(c, &t->dmgRes);
        
        ls_utf32Set(&t->name.text,   e->throwerName[i]);   t->name.viewEndIdx   = t->name.text.len;
        ls_utf32Set(&t->toHit.text,  e->throwerHit[i]);    t->toHit.viewEndIdx  = t->toHit.text.len;
        ls_utf32Set(&t->damage.text, e->throwerDamage[i]); t->damage.viewEndIdx = t->damage.text.len;
    }
    
    //NOTE: This is to avoid the program never being reset. I don't think it's necessary,
    //      but for "security" reasons we'll have it.
    addID = 1000;
    
    return;
}

//TODO: When saving a new encounter, and no encounter is selected (Maybe only after a reset) the names get fucked.
//      It is only a display bug, and on program startup it is working
b32 SaveEncounterOnClick(UIContext *c, void *data)
{
    s32 visibleMobs   = State.Init->Mobs.selectedIndex;
    s32 visibleAllies = State.Init->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - State.Init->orderAdjust;
    
    u32 numEncounters = State.encounters.numEncounters;
    Encounter *curr = State.encounters.Enc + numEncounters;
    
    ls_utf32Set(&curr->name, State.Init->EncounterName.text);
    
    curr->numMobs = visibleMobs;
    curr->numAllies = visibleAllies;
    
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *currMob = State.Init->MobFields + i;
        EncounterInitEntry *e = curr->mob + i;
        
        for(u32 j = 0; j < MOB_INIT_ENC_FIELDS; j++)
        { ls_utf32Set(&e->fields[j], currMob->editFields[j].text); }
        
        ls_utf32Set(&e->fields[MOB_INIT_ENC_FIELDS-1], currMob->maxLife.text);
        e->compendiumIdx = currMob->compendiumIdx;
    }
    
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *currAlly = State.Init->AllyFields + i;
        EncounterInitEntry *e = curr->ally + i;
        
        for(u32 j = 0; j < MOB_INIT_ENC_FIELDS; j++)
        { ls_utf32Set(&e->fields[j], currAlly->editFields[j].text); }
        
        ls_utf32Set(&e->fields[MOB_INIT_ENC_FIELDS-1], currAlly->maxLife.text);
        e->compendiumIdx = currAlly->compendiumIdx;
    }
    
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        ls_utf32Set(&curr->throwerName[i],   State.Init->Throwers[i].name.text);
        ls_utf32Set(&curr->throwerHit[i],    State.Init->Throwers[i].toHit.text);
        ls_utf32Set(&curr->throwerDamage[i], State.Init->Throwers[i].damage.text);
    }
    
    State.encounters.numEncounters += 1;
    
    State.Init->EncounterSel.selectedIndex =
        ls_uiListBoxAddEntry(c, &State.Init->EncounterSel, State.Init->EncounterName.text);
    
    return FALSE;
}

b32 RemoveEncounterOnClick(UIContext *c, void *data)
{
    if(State.inBattle) return FALSE;
    
    b32 inputUse = FALSE;
    
    u32 idx = State.Init->EncounterSel.selectedIndex;
    if(idx == 0) { return FALSE; }
    
    u32 lastIdx = State.Init->EncounterSel.list.count-1;
    
    Encounter *selected = State.encounters.Enc + (idx-1);
    Encounter *last = State.encounters.Enc + (lastIdx-1);
    
    //NOTE:      When idx == lastIdx we just decrease the numEncounters
    //
    //TODO:      Is it fine to keep the old stuff allocated? ls_unistrSet only allocates if data is null
    //           So adding new things on it shouldn't leak memory (and it gets reset on program startup anyway)
    //           Also everything gets overwritten, so there should be no problem of old data hanging.
    
    if(idx != lastIdx)
    {
        //TODO: In this case I have to free memory, else I will leak
        //TODO: Should I change how Encounters are stored to avoid this annoyance?
        for(u32 i = 0; i < MOB_NUM; i++)
        {
            for(u32 j = 0; j < MOB_INIT_ENC_FIELDS; j++)
            { ls_utf32Free(&selected->mob[i].fields[j]); }
            selected->mob[i].compendiumIdx = -1;
        }
        
        for(u32 i = 0; i < ALLY_NUM; i++)
        {
            for(u32 j = 0; j < MOB_INIT_ENC_FIELDS; j++)
            { ls_utf32Free(&selected->ally[i].fields[j]); }
            selected->ally[i].compendiumIdx = -1;
        }
        
        for(u32 i = 0; i < THROWER_NUM; i++)
        {
            ls_utf32Free(&selected->throwerName[i]);
            ls_utf32Free(&selected->throwerHit[i]);
            ls_utf32Free(&selected->throwerDamage[i]);
        }
        
        ls_memcpy(last, selected, sizeof(Encounter));
    }
    
    State.encounters.numEncounters -= 1;
    ls_uiListBoxRemoveEntry(c, &State.Init->EncounterSel, idx);
    inputUse |= ResetOnClick(c, NULL);
    
    return inputUse;
}

void CheckAndFixCounterTurns();
void AddToOrder(s32 maxLife, utf32 name, s32 newID, s32 compendiumIdx);
b32 AddEncounterOnClick(UIContext *c, void *data)
{
    UIListBox *b = (UIListBox *)data;
    u32 idx      = b->selectedIndex;
    Encounter *e = &State.encounters.Enc[idx-1];
    
    s32 currMobs       = State.Init->Mobs.selectedIndex;
    s32 currAllies     = State.Init->Allies.selectedIndex;
    
    s32 newMobsCount   = State.Init->Mobs.selectedIndex + e->numMobs;
    s32 newAlliesCount = State.Init->Allies.selectedIndex + e->numAllies;
    
    if((newMobsCount > MOB_NUM) || (newAlliesCount > ALLY_NUM)) { return FALSE; }
    
    for(u32 i = currMobs, j = 0; i < newMobsCount; i++, j++)
    {
        InitField *m = State.Init->MobFields + i;
        EncounterInitEntry *entry = e->mob + j;
        
        for(u32 k = 0; k < IF_IDX_COUNT; k++)
        { ls_uiTextBoxSet(c, &m->editFields[k], entry->fields[k]); }
        
        ls_uiTextBoxSet(c, &m->maxLife, entry->fields[MOB_INIT_ENC_FIELDS-1]);
        m->compendiumIdx = entry->compendiumIdx;
        m->ID            = addID;
        
        State.Init->turnsInRound       += 1;
        CheckAndFixCounterTurns();
        
        AddToOrder(ls_utf32ToInt(m->maxLife.text), m->editFields[IF_IDX_NAME].text, addID, m->compendiumIdx);
        
        State.Init->Mobs.selectedIndex += 1;
        addID                          += 1;
    }
    
    for(u32 i = currAllies, j = 0; i < newAlliesCount; i++, j++)
    {
        InitField *a = State.Init->AllyFields + i;
        EncounterInitEntry *entry = e->ally + j;
        
        for(u32 k = 0; k < IF_IDX_COUNT; k++)
        { ls_uiTextBoxSet(c, &a->editFields[k], entry->fields[k]); }
        
        ls_uiTextBoxSet(c, &a->maxLife, entry->fields[MOB_INIT_ENC_FIELDS-1]);
        a->compendiumIdx = entry->compendiumIdx;
        a->ID            = addID;
        
        State.Init->turnsInRound         += 1;
        CheckAndFixCounterTurns();
        
        AddToOrder(ls_utf32ToInt(a->maxLife.text), a->editFields[IF_IDX_NAME].text, addID, a->compendiumIdx);
        
        State.Init->Allies.selectedIndex += 1;
        addID                            += 1;
    }
    
    //TODO: I'm not updating throwers. Need to work on this.
    
    return TRUE;
}

b32 ThrowDiceOnClick(UIContext *c, void *data)
{
    UIButton *f = (UIButton *)data;
    
    if(f == &State.Init->GeneralThrower.throwDie)
    {
        char toThrow[128] = {};
        s32 len = ls_utf32ToAscii_t(&State.Init->GeneralThrower.toHit.text, toThrow, 128);
        s32 result = (s32)diceRoll(toThrow, len);
        
        ls_utf32FromInt_t(&State.Init->GeneralThrower.hitRes.text, result);
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
        s32 len = ls_utf32ToAscii_t(&f->toHit.text, toThrow, 128);
        s32 result = (s32)diceRoll(toThrow, len);
        
        ls_utf32FromInt_t(&f->hitRes.text, result);
        f->hitRes.viewEndIdx = f->hitRes.text.len;
        
        char dmgThrow[128] = {};
        len = ls_utf32ToAscii_t(&f->damage.text, dmgThrow, 128);
        result = (s32)diceRoll(dmgThrow, len);
        
        ls_utf32FromInt_t(&f->dmgRes.text, result);
        f->dmgRes.viewEndIdx = f->dmgRes.text.len;
    }
    
    return TRUE;
}

s32 sortTmpOrder(void *a, void *b)
{
    tmp_order *ordA = (tmp_order *)a;
    tmp_order *ordB = (tmp_order *)b;
    
    if(ordA->init < ordB->init) { return -1; }
    else if(ordA->init == ordB->init) { return 0; }
    else { return 1; }
}

b32 RollOnClick(UIContext *c, void *data)
{
    InitPage *Page = State.Init;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        s32 finalVal = ls_utf32ToInt(f->editFields[IF_IDX_FINAL].text);
        if(finalVal != 0) { continue; }
        
        s32 die = pcg32_bounded(&pcg32_global, 20) + 1;
        s32 bonus = ls_utf32ToInt(f->editFields[IF_IDX_BONUS].text);
        
        ls_utf32Clear(&f->editFields[IF_IDX_FINAL].text);
        ls_utf32FromInt_t(&f->editFields[IF_IDX_FINAL].text, bonus + die);
        f->editFields[IF_IDX_FINAL].viewEndIdx = f->editFields[IF_IDX_FINAL].text.len;
    }
    
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        
        s32 finalVal = ls_utf32ToInt(f->editFields[IF_IDX_FINAL].text);
        if(finalVal != 0) { continue; }
        
        s32 die = pcg32_bounded(&pcg32_global, 20) + 1;
        s32 bonus = ls_utf32ToInt(f->editFields[IF_IDX_BONUS].text);
        
        ls_utf32Clear(&f->editFields[IF_IDX_FINAL].text);
        ls_utf32FromInt_t(&f->editFields[IF_IDX_FINAL].text, bonus + die);
        f->editFields[IF_IDX_FINAL].viewEndIdx = f->editFields[IF_IDX_FINAL].text.len;
    }
    
    return TRUE;
}

b32 SetOnClick(UIContext *c, void *data)
{
    AssertMsg(!State.inBattle, "Clicking this should be impossible while in Battle\n");
    
    InitPage *Page = State.Init;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    
    tmp_order ord[ORDER_NUM] = {};
    u32 idx = 0;
    
    //TODO: Already have HP as a number, why am I converting back and forth? @ConvertNation
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        ord[idx].init          = ls_utf32ToInt(f->editFields[IF_IDX_FINAL].text);
        ord[idx].name          = &f->editFields[IF_IDX_NAME].text;
        ord[idx].maxLife       = ls_utf32ToInt(f->maxLife.text);
        ord[idx].compendiumIdx = f->compendiumIdx;
        ord[idx].ID            = f->ID;
        
        idx += 1;
    }
    
    //TODO: Already have HP as a number, why am I converting back and forth? @ConvertNation
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        
        ord[idx].init          = ls_utf32ToInt(f->editFields[IF_IDX_FINAL].text);
        ord[idx].name          = &f->editFields[IF_IDX_NAME].text;
        ord[idx].maxLife       = ls_utf32ToInt(f->maxLife.text);
        ord[idx].compendiumIdx = f->compendiumIdx;
        ord[idx].ID            = f->ID;
        
        idx += 1;
    }
    
    for(u32 i = 0; i < PARTY_NUM; i++)
    {
        ord[idx].init          = ls_utf32ToInt(Page->PlayerInit[i].text);
        ord[idx].name          = (utf32 *)(PartyNameUTF32 + i);
        ord[idx].maxLife       = 0;
        ord[idx].compendiumIdx = -1;
        ord[idx].ID            = i;
        
        
        idx += 1;
    }
    
    ls_quicksortCustom(ord, sizeof(tmp_order), visibleOrder, sortTmpOrder);
    
    for(u32 i = 0, j = visibleOrder - 1; i < visibleOrder; i++, j--)
    {
        Order *f = Page->OrderFields + i;
        
        ls_utf32Set(&f->field.text, *ord[j].name);
        f->field.maxValue = ord[j].maxLife;
        f->compendiumIdx  = ord[j].compendiumIdx;
        f->ID             = ord[j].ID;
        
        if(i == 0) 
        { 
            ls_utf32Set(&Page->Current.text, *ord[j].name); 
            Page->Current.viewEndIdx = Page->Current.text.len;
        }
    }
    
    for(u32 i = 0; i < PARTY_NUM; i++)
    { Page->PlayerInit[i].isReadonly = TRUE; }
    
    for(u32 i = 0; i < MOB_NUM; i++)
    {
        InitField *f = Page->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { f->editFields[j].isReadonly = TRUE; }
    }
    
    for(u32 i = 0; i < ALLY_NUM; i++) { 
        Page->AllyFields[i].editFields[IF_IDX_NAME].isReadonly  = TRUE;
        Page->AllyFields[i].editFields[IF_IDX_BONUS].isReadonly = TRUE;
        Page->AllyFields[i].editFields[IF_IDX_FINAL].isReadonly = TRUE;
    }
    
    for(u32 i = 0; i < ORDER_NUM; i++)
    { Page->OrderFields[i].pos.isReadonly = FALSE; }
    
    Page->turnsInRound  = visibleOrder;
    Page->currIdx       = 0;
    State.inBattle      = TRUE;
    globalSelectedIndex = 0;
    
    return TRUE;
}

b32 ResetOnClick(UIContext *c, void *data)
{
    InitPage *Page = State.Init;
    
    Page->Mobs.selectedIndex   = 0;
    Page->Allies.selectedIndex = 0;
    State.Init->isAdding             = FALSE;
    globalSelectedIndex        = -1;
    
    State.inBattle = FALSE;
    
    utf32 zeroUTF32 = { (u32 *)U"0", 1, 1 };
    
    for(u32 i = 0; i < PARTY_NUM; i++) 
    { 
        ls_uiTextBoxClear(c, Page->PlayerInit + i);
        ls_utf32Set(&Page->PlayerInit[i].text, zeroUTF32);
        Page->PlayerInit[i].viewEndIdx = Page->PlayerInit[i].text.len;
    }
    
    s32 currID = PARTY_NUM;
    for(u32 i = 0; i < MOB_NUM; i++)   
    { 
        InitField *f = Page->MobFields + i;
        
        ls_uiTextBoxClear(c, &f->editFields[IF_IDX_NAME]);
        ls_uiTextBoxSet(c, &f->editFields[IF_IDX_NAME], ls_utf32Constant(MobName[i]));
        
        for(u32 j = 1; j < IF_IDX_COUNT; j++)
        {
            if(j == IF_IDX_EXTRA) { ls_uiTextBoxClear(c, &f->editFields[j]); continue; }
            
            ls_uiTextBoxClear(c, &f->editFields[j]);
            ls_uiTextBoxSet(c, &f->editFields[j], zeroUTF32);
        }
        
        ls_uiTextBoxClear(c, &f->maxLife);
        ls_uiTextBoxSet(c, &f->maxLife, zeroUTF32);
        
        f->compendiumIdx = -1;
        f->ID            = currID;
        currID          += -1;
    }
    
    for(u32 i = 0; i < ALLY_NUM; i++)  
    { 
        InitField *f = Page->AllyFields + i;
        
        ls_uiTextBoxClear(c, &f->editFields[IF_IDX_NAME]);
        ls_uiTextBoxSet(c, &f->editFields[IF_IDX_NAME], ls_utf32Constant(AllyName[i]));
        
        ls_uiTextBoxClear(c, &f->editFields[IF_IDX_BONUS]);
        ls_uiTextBoxSet(c, &f->editFields[IF_IDX_BONUS], zeroUTF32);
        
        ls_uiTextBoxClear(c, &f->editFields[IF_IDX_FINAL]);
        ls_uiTextBoxSet(c, &f->editFields[IF_IDX_FINAL], zeroUTF32);
        
        f->compendiumIdx = -1;
        f->ID            = currID;
        currID          += -1;
    }
    
    for(u32 i = 0; i < ORDER_NUM; i++)
    {
        Order *f = Page->OrderFields + i;
        
        f->pos.isReadonly = TRUE;
        
        ls_utf32Clear(&f->field.text);
        f->field.maxValue = 100;
        f->field.minValue = -30;
        f->field.currPos  = 1.0;
        f->compendiumIdx  = -1;
        f->ID             = -1;
    }
    
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *f = Page->Counters + i;
        
        ls_uiTextBoxClear(c, &f->name);
        ls_uiTextBoxClear(c, &f->rounds);
        f->roundsLeft      = 0;
        f->startIdxInOrder = 0;
        f->turnCounter     = 0;
        f->isActive        = FALSE;
    }
    
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrow *f = Page->Throwers + i;
        
        ls_uiTextBoxClear(c, &f->name);
        ls_uiTextBoxClear(c, &f->toHit);
        ls_uiTextBoxClear(c, &f->hitRes);
        ls_uiTextBoxClear(c, &f->damage);
        ls_uiTextBoxClear(c, &f->dmgRes);
    }
    
    // General Thrower
    {
        ls_uiTextBoxClear(c, &Page->GeneralThrower.toHit);
        ls_uiTextBoxClear(c, &Page->GeneralThrower.hitRes);
    }
    
    b32 StartAddingMob(UIContext *, void *);
    b32 StartAddingAlly(UIContext *, void *);
    ls_utf32Set(&Page->addNewMob.name, ls_utf32Constant(U"+"));
    Page->addNewMob.onClick = StartAddingMob;
    
    ls_utf32Set(&Page->addNewAlly.name, ls_utf32Constant(U"+"));
    Page->addNewAlly.onClick = StartAddingAlly;
    
    Page->EncounterSel.selectedIndex = 0;
    
    ls_utf32Clear(&Page->Current.text);
    
    ls_utf32Set(&Page->RoundCounter.text, zeroUTF32);
    Page->RoundCounter.viewEndIdx = Page->RoundCounter.text.len;
    Page->roundCount = 0;
    
    Page->orderAdjust = 0;
    
    addID = 1000;
    
    return TRUE;
}

b32 NextOnClick(UIContext *c, void *data)
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
        ls_utf32FromInt_t(&Page->RoundCounter.text, Page->roundCount);
        Page->RoundCounter.viewEndIdx = Page->RoundCounter.text.len;
    }
    
    //NOTE: Set the Current
    ls_utf32Set(&Page->Current.text, Page->OrderFields[Page->currIdx].field.text);
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
                    ls_uiTextBoxClear(c, &C->name);
                    ls_uiTextBoxClear(c, &C->rounds);
                    
                    C->turnCounter = 0;
                    C->isActive     = FALSE;
                    
                    continue;
                }
                
                ls_utf32FromInt_t(&C->rounds.text, C->roundsLeft);
                C->rounds.viewEndIdx = C->rounds.text.len;
                
                continue;
            }
            
            continue;
        }
    }
    
    return TRUE;
}

void CopyOrder(Order *From, Order *To)
{
    // Copy Slider
    To->field.isHot  = From->field.isHot;
    To->field.isHeld = From->field.isHeld;
    
    ls_utf32Set(&To->field.text, From->field.text);
    
    To->field.currValue = From->field.currValue;
    To->field.maxValue  = From->field.maxValue;
    To->field.minValue  = From->field.minValue;
    
    To->field.currPos   = From->field.currPos;
    
    To->field.style     = From->field.style;
    To->field.lColor    = From->field.lColor;
    To->field.rColor    = From->field.rColor;
    
    //NOTE: Don't need to copy the textbox, since the position of an order field is fixed.
    //NOTE: Don't need to copy the button
    
    To->compendiumIdx = From->compendiumIdx;
    To->ID            = From->ID;
}

void CopyInitField(UIContext *c, InitField *From, InitField *To)
{
    for(u32 i = 0; i < IF_IDX_COUNT; i++)
    { ls_uiTextBoxSet(c, &To->editFields[i], From->editFields[i].text); }
    
    ls_uiTextBoxSet(c, &To->maxLife, From->maxLife.text);
    
    To->compendiumIdx = From->compendiumIdx;
    To->ID            = From->ID;
}

b32 RemoveOrderOnClick(UIContext *c, void *data)
{
    if(State.inBattle == FALSE) { return FALSE; }
    
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
                CopyInitField(c, B, ally);
                
                ls_uiTextBoxClear(c, &B->editFields[IF_IDX_NAME]);
                ls_uiTextBoxClear(c, &B->editFields[IF_IDX_BONUS]);
                ls_uiTextBoxClear(c, &B->editFields[IF_IDX_FINAL]);
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
                CopyInitField(c, B, mob);
                
                for(u32 j = 0; j < IF_IDX_COUNT; j++)
                { ls_uiTextBoxClear(c, &B->editFields[j]); }
                
                ls_uiTextBoxClear(c, &B->maxLife);
                
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
    AssertMsg(Page->turnsInRound >= 0 && Page->turnsInRound <= 64, "Turns in Round is Fucked\n");
    
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
    
    return TRUE;
}

b32 StartCounterOnClick(UIContext *c, void *data)
{
    Counter *C = (Counter *)data;
    
    if(State.inBattle == FALSE) { return FALSE; }
    
    s32 val = ls_utf32ToInt(C->rounds.text);
    if(val <= 0) { return FALSE; }
    
    C->name.isReadonly   = TRUE;
    C->rounds.isReadonly = TRUE;
    
    C->isActive        = TRUE;
    C->turnCounter     = 0;
    C->roundsLeft      = val;
    C->startIdxInOrder = State.Init->currIdx;
    
    return TRUE;
}

b32 PlusOneCounterOnClick(UIContext *c, void *data)
{
    Counter *C = (Counter *)data;
    
    C->roundsLeft += 1;
    ls_utf32FromInt_t(&C->rounds.text, C->roundsLeft);
    
    return TRUE;
}

b32 StopCounterOnClick(UIContext *c, void *data)
{
    Counter *C = (Counter *)data;
    
    ls_uiTextBoxClear(c, &C->name);
    ls_uiTextBoxClear(c, &C->rounds);
    
    C->name.isReadonly   = FALSE;
    C->rounds.isReadonly = FALSE;
    
    C->isActive        = FALSE;
    C->turnCounter     = 0;
    C->roundsLeft      = 0;
    C->startIdxInOrder = 0;
    
    return TRUE;
}

void CheckAndFixCounterTurns()
{
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
}

b32 StartAddingMob(UIContext *c, void *data)
{
    //NOTE: We suppress Undo State Recording during the addition of a new Enemy/Ally because it will allow
    //      The entire new mob to be undone/redone in a single action.
    suppressingUndoRecord = TRUE;
    State.Init->isAdding  = TRUE;
    globalSelectedIndex   = State.Init->Mobs.selectedIndex;
    
    //Clear The Extra
    InitField *f = State.Init->MobFields + State.Init->Mobs.selectedIndex;
    for(u32 i = 0; i < IF_IDX_COUNT; i++) { ls_uiTextBoxClear(c, &f->editFields[i]); }
    ls_uiTextBoxClear(c, &f->maxLife);
    
    //Update the button!
    ls_utf32Set(&State.Init->addNewMob.name, ls_utf32Constant(U"Ok"));
    State.Init->addNewMob.onClick = AddMobOnClick;
    
    return FALSE;
}

b32 StartAddingAlly(UIContext *c, void *data)
{
    //NOTE: We suppress Undo State Recording during the addition of a new Enemy/Ally because it will allow
    //      The entire new mob to be undone/redone in a single action.
    suppressingUndoRecord = TRUE;
    State.Init->isAdding  = TRUE;
    globalSelectedIndex   = State.Init->Allies.selectedIndex + MOB_NUM;
    
    //Clear The Extra
    InitField *f = State.Init->AllyFields + State.Init->Allies.selectedIndex;
    for(u32 i = 0; i < IF_IDX_COUNT; i++) { ls_uiTextBoxClear(c, &f->editFields[i]); }
    ls_uiTextBoxClear(c, &f->maxLife);
    
    //Update the button!
    ls_utf32Set(&State.Init->addNewAlly.name, ls_utf32Constant(U"Ok"));
    State.Init->addNewAlly.onClick = AddAllyOnClick;
    
    return FALSE;
}

//TODO: Already have HP as a number, why am I converting back and forth? @ConvertNation
void AddToOrder(s32 maxLife, utf32 name, s32 newID, s32 compendiumIdx)
{
    s32 visibleMobs   = State.Init->Mobs.selectedIndex;
    s32 visibleAllies = State.Init->Allies.selectedIndex;
    s32 visibleOrder = visibleMobs + visibleAllies + PARTY_NUM - State.Init->orderAdjust;
    Order *o = State.Init->OrderFields + visibleOrder;
    
    ls_utf32Set(&o->field.text, name);
    o->field.maxValue = maxLife;
    o->compendiumIdx  = compendiumIdx;
    
    //NOTE: Because IDs in the InitFields get overwritten when removing from order 
    //      we can't reliably re-use them, unless we create a system to dispense Unique IDs.
    //      So for simplicy we are just starting from 1000 and every single Added Init will just the next one.
    //      AddID is reset during ResetOnClick
    o->ID = newID;
}

//TODO: Right now we are not allowing to give an Initiative Value and put the new 
//      Field in the right place in Order.
//      If we allow it, the counter checker WILL HAVE to probably fix C->startIdxInOrder to work.
b32 AddMobOnClick(UIContext *c, void *data)
{
    s32 visibleMobs = State.Init->Mobs.selectedIndex;
    
    if(visibleMobs == MOB_NUM) { return FALSE; }
    
    State.Init->turnsInRound += 1;
    CheckAndFixCounterTurns();
    
    InitField *f = State.Init->MobFields + visibleMobs;
    for(u32 i = 0; i < IF_IDX_COUNT; i++)
    { 
        f->editFields[i].isReadonly  = TRUE;
    }
    f->ID = addID;
    
    //TODO: Already have HP as a number, why am I converting back and forth? @ConvertNation
    AddToOrder(ls_utf32ToInt(f->maxLife.text), f->editFields[IF_IDX_NAME].text, addID, f->compendiumIdx);
    
    State.Init->Mobs.selectedIndex += 1;
    addID                          += 1;
    State.Init->isAdding            = FALSE;
    globalSelectedIndex             = -1;
    
    //Update the button!
    ls_utf32Set(&State.Init->addNewMob.name, ls_utf32Constant(U"+"));
    State.Init->addNewMob.onClick = StartAddingMob;
    
    //NOTE: We suppress Undo State Recording during the addition of a new Enemy/Ally because it will allow
    //      The entire new mob to be undone/redone in a single action.
    suppressingUndoRecord = FALSE;
    
    return TRUE;
}

b32 AddAllyOnClick(UIContext *c, void *data)
{
    s32 visibleAllies = State.Init->Allies.selectedIndex;
    
    if(visibleAllies == ALLY_NUM) { return FALSE; }
    
    State.Init->turnsInRound         += 1;
    CheckAndFixCounterTurns();
    
    InitField *f = State.Init->AllyFields + visibleAllies;
    for(u32 i = 0; i < IF_IDX_COUNT; i++)
    { 
        f->editFields[i].isReadonly  = TRUE;
    }
    f->ID = addID;
    
    //TODO: Already have HP as a number, why am I converting back and forth? @ConvertNation
    AddToOrder(ls_utf32ToInt(f->maxLife.text), f->editFields[IF_IDX_NAME].text, addID, f->compendiumIdx);
    
    State.Init->Allies.selectedIndex += 1;
    addID                            += 1;
    State.Init->isAdding              = FALSE;
    globalSelectedIndex               = -1;
    
    //Update the button!
    ls_utf32Set(&State.Init->addNewAlly.name, ls_utf32Constant(U"+"));
    State.Init->addNewAlly.onClick = StartAddingAlly;
    
    //NOTE: We suppress Undo State Recording during the addition of a new Enemy/Ally because it will allow
    //      The entire new mob to be undone/redone in a single action.
    suppressingUndoRecord = FALSE;
    
    return TRUE;
}

void InitFieldInit(UIContext *c, InitField *f, s32 *currID, const char32_t *name)
{
    //TODO: I hate the init fields, and how we have a different CustomFieldTextHandler for every piece
    //      of the init fields.
    CustomFieldTextHandler *textHandler = 
        (CustomFieldTextHandler *)ls_alloc(sizeof(CustomFieldTextHandler)*INIT_FIELD_EDITFIELDS_NUM);
    for(u32 i = 0; i < IF_IDX_COUNT; i++)
    {
        textHandler[i].parent = f;
        textHandler[i].field = f->editFields + i;
        textHandler[i].if_idx = i;
    }
    
    utf32 zeroUTF32 = { (u32 *)U"0", 1, 1 };
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_NAME], ls_utf32Constant(name));
    f->editFields[IF_IDX_NAME].preInput     = CustomInitFieldText;
    f->editFields[IF_IDX_NAME].data         = &textHandler[0];
    f->editFields[IF_IDX_NAME].isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_BONUS], zeroUTF32);
    f->editFields[IF_IDX_BONUS].maxLen       = 3; //NOTE: Allow for sign!
    f->editFields[IF_IDX_BONUS].preInput     = CustomInitFieldText;
    f->editFields[IF_IDX_BONUS].data         = &textHandler[1];
    f->editFields[IF_IDX_BONUS].isSingleLine = TRUE;
    
    f->editFields[IF_IDX_EXTRA].text         = ls_utf32Alloc(16);
    f->editFields[IF_IDX_EXTRA].viewEndIdx   = 0;//f->editFields[IF_IDX_EXTRA].text.len;
    f->editFields[IF_IDX_EXTRA].preInput     = NULL; //CustomInitFieldText;
    f->editFields[IF_IDX_EXTRA].data         = NULL; //&textHandler[2];
    f->editFields[IF_IDX_EXTRA].isSingleLine = FALSE;
    
    MobLifeHandler *handler = (MobLifeHandler *)ls_alloc(sizeof(MobLifeHandler));
    handler->parent   = &f->maxLife;
    handler->mob      = f;
    handler->previous = ls_utf32Alloc(16);
    
    ls_uiTextBoxSet(c, &f->maxLife, zeroUTF32);
    f->maxLife.maxLen       = 4;
    f->maxLife.preInput     = CustomMobLifeField;
    f->maxLife.data         = handler;
    f->maxLife.isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_TOTALAC], zeroUTF32);
    f->editFields[IF_IDX_TOTALAC].maxLen       = 2;
    f->editFields[IF_IDX_TOTALAC].preInput     = CustomInitFieldText;
    f->editFields[IF_IDX_TOTALAC].data         = &textHandler[3];
    f->editFields[IF_IDX_TOTALAC].isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_TOUCHAC], zeroUTF32);
    f->editFields[IF_IDX_TOUCHAC].maxLen       = 2;
    f->editFields[IF_IDX_TOUCHAC].preInput     = CustomInitFieldText;
    f->editFields[IF_IDX_TOUCHAC].data         = &textHandler[4];
    f->editFields[IF_IDX_TOUCHAC].isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_FLATAC], zeroUTF32);
    f->editFields[IF_IDX_FLATAC].maxLen       = 2;
    f->editFields[IF_IDX_FLATAC].preInput     = CustomInitFieldText;
    f->editFields[IF_IDX_FLATAC].data         = &textHandler[5];
    f->editFields[IF_IDX_FLATAC].isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_LOWAC], zeroUTF32);
    f->editFields[IF_IDX_LOWAC].maxLen       = 2;
    f->editFields[IF_IDX_LOWAC].preInput     = CustomInitFieldText;
    f->editFields[IF_IDX_LOWAC].data         = &textHandler[6];
    f->editFields[IF_IDX_LOWAC].isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_CONSAVE], zeroUTF32);
    f->editFields[IF_IDX_CONSAVE].maxLen       = 2;
    f->editFields[IF_IDX_CONSAVE].preInput     = CustomInitFieldText;
    f->editFields[IF_IDX_CONSAVE].data         = &textHandler[7];
    f->editFields[IF_IDX_CONSAVE].isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_DEXSAVE], zeroUTF32);
    f->editFields[IF_IDX_DEXSAVE].maxLen       = 2;
    f->editFields[IF_IDX_DEXSAVE].preInput     = CustomInitFieldText;
    f->editFields[IF_IDX_DEXSAVE].data         = &textHandler[8];
    f->editFields[IF_IDX_DEXSAVE].isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_WISSAVE], zeroUTF32);
    f->editFields[IF_IDX_WISSAVE].maxLen       = 2;
    f->editFields[IF_IDX_WISSAVE].preInput     = CustomInitFieldText;
    f->editFields[IF_IDX_WISSAVE].data         = &textHandler[9];
    f->editFields[IF_IDX_WISSAVE].isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_FINAL], zeroUTF32);
    f->editFields[IF_IDX_FINAL].maxLen       = 2;
    f->editFields[IF_IDX_FINAL].preInput     = CustomInitFieldText;
    f->editFields[IF_IDX_FINAL].data         = &textHandler[10];
    f->editFields[IF_IDX_FINAL].isSingleLine = TRUE;
    
    f->compendiumIdx      = -1;
    
    f->ID = *currID;
    *currID += 1;
}

void SetInitTab(UIContext *c, ProgramState *PState)
{
    InitPage *Page = PState->Init;
    
    globalSelectedIndex = -1;
    
    for(u32 i = 0; i < MOB_NUM + 1; i++) { ls_uiListBoxAddEntry(c, &Page->Mobs, (char *)Enemies[i]); }
    for(u32 i = 0; i < ALLY_NUM + 1; i++) { ls_uiListBoxAddEntry(c, &Page->Allies, (char *)Allies[i]); }
    
    for(u32 i = 0; i < PARTY_NUM; i++) 
    { 
        UITextBox *f = Page->PlayerInit + i;
        ls_uiTextBoxSet(c, f, ls_utf32Constant(U"0"));
        f->maxLen       = 2;
        f->preInput     = CustomPlayerText;
        f->data         = f;
        f->isSingleLine = TRUE;
    }
    
    s32 currID = PARTY_NUM;
    for(u32 i = 0; i < MOB_NUM; i++)   
    { 
        InitField *f = Page->MobFields + i;
        
        InitFieldInit(c, f, &currID, MobName[i]);
    }
    
    for(u32 i = 0; i < ALLY_NUM; i++)  
    { 
        InitField *f = Page->AllyFields + i;
        
        InitFieldInit(c, f, &currID, AllyName[i]);
    }
    
    ls_uiButtonInit(&Page->addNewMob, UIBUTTON_TEXT, U"+", StartAddingMob, NULL, NULL);
    ls_uiButtonInit(&Page->addNewAlly, UIBUTTON_TEXT, U"+", StartAddingAlly, NULL, NULL);
    
    for(u32 i = 0; i < ORDER_NUM; i++)
    {
        Order *f = Page->OrderFields + i;
        
        Color lColor = ls_uiAlphaBlend(RGBA(0x10, 0xDD, 0x20, 0x99), c->widgetColor);
        Color rColor = ls_uiAlphaBlend(RGBA(0xF0, 0xFF, 0x3D, 0x99), c->widgetColor);
        f->field     = ls_uiSliderInit(NULL, 100, -30, 1.0, SL_BOX, lColor, rColor);
        
        OrderHandler *orderHandler = (OrderHandler *)ls_alloc(sizeof(OrderHandler));
        orderHandler->parent = &f->pos;
        orderHandler->order  = f;
        
        //TODO: Cannot use a constant string here, because it HAS to be modifiable to change the order
        f->pos.text         = ls_utf32FromInt(i);
        f->pos.viewEndIdx   = f->pos.text.len;
        f->pos.maxLen       = 2;
        f->pos.preInput     = ChangeOrder;
        f->pos.data         = orderHandler;
        f->pos.isReadonly   = TRUE;
        f->pos.isSingleLine = TRUE;
        
        ls_uiButtonInit(&f->remove, UIBUTTON_TEXT, ls_utf32Constant(U"X"), RemoveOrderOnClick, NULL, (void *)((u64)i));
        
        f->compendiumIdx  = -1;
    }
    
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *f = Page->Counters + i;
        
        f->name.text        = ls_utf32Alloc(16);
        f->name.isSingleLine = TRUE;
        
        f->rounds.text         = ls_utf32Alloc(16);
        f->rounds.maxLen       = 2;
        f->rounds.isSingleLine = TRUE;
        
        ls_uiButtonInit(&f->start, UIBUTTON_TEXT, ls_utf32Constant(U"Start"), StartCounterOnClick, NULL, (void *)f);
        ls_uiButtonInit(&f->plusOne, UIBUTTON_TEXT, ls_utf32Constant(U"+1"), PlusOneCounterOnClick, NULL, (void *)f);
        ls_uiButtonInit(&f->stop, UIBUTTON_TEXT, ls_utf32Constant(U"Stop"), StopCounterOnClick, NULL, (void *)f);
    }
    
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrow *f = Page->Throwers + i;
        
        f->name.text   = ls_utf32Alloc(32);
        f->toHit.text  = ls_utf32Alloc(32);
        f->hitRes.text = ls_utf32Alloc(16);
        f->damage.text = ls_utf32Alloc(32);
        f->dmgRes.text = ls_utf32Alloc(16);
        
        f->name.isSingleLine   = TRUE;
        f->toHit.isSingleLine  = TRUE;
        f->damage.isSingleLine = TRUE;
        
        f->hitRes.maxLen       = 4;
        f->hitRes.isReadonly   = TRUE;
        f->hitRes.isSingleLine = TRUE;
        
        f->dmgRes.maxLen       = 4;
        f->dmgRes.isReadonly   = TRUE;
        f->dmgRes.isSingleLine = TRUE;
        
        ls_uiButtonInit(&f->throwDie, UIBUTTON_TEXT, ls_utf32Constant(U"Go"), ThrowDiceOnClick, NULL, &f->throwDie);
    }
    
    {
        // General Thrower
        Page->GeneralThrower.name.text     = ls_utf32Alloc(32);
        Page->GeneralThrower.toHit.text    = ls_utf32Alloc(32);
        Page->GeneralThrower.hitRes.text   = ls_utf32Alloc(16);
        Page->GeneralThrower.damage.text   = ls_utf32Alloc(32);
        Page->GeneralThrower.dmgRes.text   = ls_utf32Alloc(16);
        
        Page->GeneralThrower.name.isSingleLine  = TRUE;
        Page->GeneralThrower.toHit.isSingleLine = TRUE;
        
        Page->GeneralThrower.hitRes.maxLen       = 4;
        Page->GeneralThrower.hitRes.isReadonly   = TRUE;
        Page->GeneralThrower.hitRes.isSingleLine = TRUE;
        
        Page->GeneralThrower.dmgRes.maxLen       = 4;
        Page->GeneralThrower.dmgRes.isReadonly   = TRUE;
        Page->GeneralThrower.dmgRes.isSingleLine = TRUE;
        
        ls_uiButtonInit(&Page->GeneralThrower.throwDie, UIBUTTON_TEXT, ls_utf32Constant(U"Go"), ThrowDiceOnClick,
                        NULL, &Page->GeneralThrower.throwDie);
    }
    
    //Encounter Selector
    {
        Page->EncounterSel.onSelect = OnEncounterSelect;
        Page->EncounterSel.data     = &Page->EncounterSel;
        ls_uiListBoxAddEntry(c, &Page->EncounterSel, ls_utf32Constant(NoEncounterStr));
        for(u32 i = 0; i < PState->encounters.numEncounters; i++)
        { ls_uiListBoxAddEntry(c, &Page->EncounterSel, PState->encounters.Enc[i].name); }
        
        Page->EncounterName.text         = ls_utf32Alloc(16);
        Page->EncounterName.isSingleLine = TRUE;
    }
    
    ls_uiButtonInit(&Page->SaveEnc, UIBUTTON_TEXT, ls_utf32Constant(U"Save"), SaveEncounterOnClick, NULL, NULL);
    ls_uiButtonInit(&Page->RemoveEnc, UIBUTTON_TEXT, ls_utf32Constant(U"X"), RemoveEncounterOnClick, NULL, NULL);
    ls_uiButtonInit(&Page->AddEnc, UIBUTTON_TEXT, ls_utf32Constant(U"<-"), AddEncounterOnClick, NULL, &Page->EncounterSel);
    
    Page->Current.text         = ls_utf32Alloc(16);
    Page->Current.isReadonly   = TRUE;
    Page->Current.isSingleLine = TRUE;
    
    Page->RoundCounter.text         = ls_utf32FromUTF32(U"0");
    Page->RoundCounter.viewEndIdx   = Page->RoundCounter.text.len;
    Page->RoundCounter.isReadonly   = TRUE;
    Page->RoundCounter.isSingleLine = TRUE;
    
    ls_uiButtonInit(&Page->Roll, UIBUTTON_TEXT, ls_utf32Constant(U"Roll"), RollOnClick, NULL, NULL);
    ls_uiButtonInit(&Page->Set, UIBUTTON_TEXT, ls_utf32Constant(U"Set"), SetOnClick, NULL, NULL);
    ls_uiButtonInit(&Page->Reset, UIBUTTON_TEXT, ls_utf32Constant(U"Reset"), ResetOnClick, NULL, NULL);
    ls_uiButtonInit(&Page->Next, UIBUTTON_TEXT, ls_utf32Constant(U"Next"), NextOnClick, NULL, NULL);
    ls_uiButtonInit(&Page->Undo, UIBUTTON_TEXT, ls_utf32Constant(U"<-"), RequestUndoOnClick, NULL, NULL);
    ls_uiButtonInit(&Page->Redo, UIBUTTON_TEXT, ls_utf32Constant(U"->"), RequestRedoOnClick, NULL, NULL);
}

b32 DrawInitExtra(UIContext *c, InitField *F, s32 baseX, s32 y)
{
    b32 inputUse = FALSE;
    
    Color base = c->widgetColor;
    s32 x = baseX;
    
    ls_uiLabel(c, U"Nome", x-45, y+5);
    inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_NAME], x, y, 156, 20);
    
    c->widgetColor = ls_uiAlphaBlend(RGBA(0x1B, 0x18, 0x14, 150), base);
    ls_uiLabel(c, U"PF", x+257, y+5);
    inputUse |= ls_uiTextBox(c, &F->maxLife, x+282, y, 42, 20);
    
    y -= 40;
    c->widgetColor = ls_uiAlphaBlend(RGBA(0x61, 0x3B, 0x09, 150), base);
    ls_uiLabel(c, U"CA", x-30, y+5);
    inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_TOTALAC], x, y, 26, 20); x += 130;
    
    c->widgetColor = ls_uiAlphaBlend(RGBA(0x9C, 0x43, 0x8B, 150), base);
    ls_uiLabel(c, U"Contatto", x-76, y+5);
    inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_TOUCHAC], x, y, 26, 20); x += 152;
    
    c->widgetColor = ls_uiAlphaBlend(RGBA(0xD5, 0xCB, 0x35, 150), base);
    ls_uiLabel(c, U"Impreparato", x-100, y+5);
    inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_FLATAC],  x, y, 26, 20); x += 100;
    
    c->widgetColor = ls_uiAlphaBlend(RGBA(0x75, 0x46, 0x46, 150), base);
    ls_uiLabel(c, U"Base", x-46, y+5);
    inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_LOWAC],   x, y, 26, 20); x += 140;
    
    y -= 40;
    x  = baseX;
    c->widgetColor = ls_uiAlphaBlend(RGBA(0xDD, 0x10, 0x20, 150), base);
    ls_uiLabel(c, U"Tempra", x-62, y+5);
    inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_CONSAVE], x, y, 26, 20); x += 130;
    
    c->widgetColor = ls_uiAlphaBlend(RGBA(0x2C, 0x80, 0x34, 150), base);
    ls_uiLabel(c, U"Riflessi", x-76, y+5);
    inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_DEXSAVE], x, y, 26, 20); x += 152;
    
    c->widgetColor = ls_uiAlphaBlend(RGBA(0x2C, 0x3D, 0x80, 150), base);
    ls_uiLabel(c, U"Volont\U000000E0", x-70, y+5);
    inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_WISSAVE], x, y, 26, 20); x += 130;
    
    c->widgetColor = base;
    
    y -= 380;
    x  = baseX;
    inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_EXTRA], x, y, 408, 360); x += 96;
    
    return inputUse;
}

b32 DrawInitField(UIContext *c, InitField *F, s32 baseX, s32 y, u32 posIdx, s32 w)
{
    b32 inputUse = FALSE;
    
    s32 x = baseX;
    if(globalSelectedIndex != posIdx) { inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_NAME], x, y, w, 20); }
    else { ls_uiLabel(c, F->editFields[IF_IDX_NAME].text, x+4, y+5); }
    inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_BONUS], x + w     , y, 26, 20);
    inputUse |= ls_uiTextBox(c, &F->editFields[IF_IDX_FINAL], x + w + 26, y, 26, 20);
    
    Input *UserInput = &c->UserInput;
    if(RightClickIn(x, y, w+26, 19)) { globalSelectedIndex = (s32)posIdx; }
    
    return inputUse;
}

b32 DrawOrderField(UIContext *c, Order *f, s32 xPos, s32 yPos, u32 posIdx)
{
    b32 inputUse = FALSE;
    
    Color original = c->borderColor;
    if(posIdx == State.Init->currIdx) c->borderColor = RGB(0xBB, 0, 0);
    
    inputUse |= ls_uiSlider(c, &f->field, xPos + 50, yPos, 166, 20);
    
    c->borderColor = original;
    
    if(inputUse)
    {
        s32 currVal = ls_uiSliderGetValue(c, &f->field);
        
        if(currVal == 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xFF, 0x97, 0x12, 0x99), c->widgetColor); }
        
        else if(currVal < 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xDD, 0x10, 0x20, 0x99), c->widgetColor); }
        
        else if(currVal > 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xF0, 0xFF, 0x3D, 0x99), c->widgetColor); }
    }
    
    inputUse |= ls_uiTextBox(c, &f->pos, xPos + 25, yPos, 25, 20);
    inputUse |= ls_uiButton(c, &f->remove, xPos, yPos, 20, 20);
    
    Input *UserInput = &c->UserInput;
    if(RightClickIn(xPos + 50, yPos, 120, 20)) globalSelectedIndex = (s32)posIdx;
    
    return inputUse;
}

b32 DrawDefaultStyle(UIContext *c)
{
    InitPage *Page = State.Init;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    
    b32 inputUse = FALSE;
    
    //NOTE: Z Layer 1 Input
    {
        if(!State.inBattle)
        {
            inputUse |= ls_uiListBox(c, &Page->EncounterSel,  480, 718, 120, 20);
            inputUse |= ls_uiTextBox(c, &Page->EncounterName, 624, 718, 100, 20);
            
            inputUse |= ls_uiButton(c, &Page->SaveEnc, 601, 740, 44, 20);
            inputUse |= ls_uiButton(c, &Page->RemoveEnc, 455, 718, 24, 20);
            
            inputUse |= ls_uiListBox(c, &Page->Mobs,   406, 678, 100, 20);
            inputUse |= ls_uiListBox(c, &Page->Allies, 641, 498, 100, 20);
            
            inputUse |= ls_uiButton(c, &Page->Roll, 556, 678, 48, 20);
            inputUse |= ls_uiButton(c, &Page->Set,  780, 678, 48, 20);
        }
    }
    
    //NOTE: Z Layer 0 Input
    
    // Party
    s32 yPos = 638;
    for(u32 i = 0; i < PARTY_NUM; i++)
    {
        ls_uiLabel(c, PartyName[i], 650, yPos+6);
        inputUse |= ls_uiTextBox(c, Page->PlayerInit + i, 732, yPos, 32, 20);
        yPos -= 20;
    }
    
    // Allies
    yPos = 458;
    for(u32 i = 0; i < visibleAllies; i++)
    {
        inputUse |= DrawInitField(c, Page->AllyFields + i, 616, yPos, i, 136);
        yPos -= 20;
    }
    
    // Enemies
    yPos = 638;
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        inputUse |= DrawInitField(c, Page->MobFields + i, 378, yPos, i, 136);
        yPos -= 20;
    }
    
    // Order
    yPos = 638;
    for(u32 i = 0; i < visibleOrder; i += 2)
    {
        inputUse |= DrawOrderField(c, Page->OrderFields + i, 870, yPos, i);
        
        if((i+1) < visibleOrder)
        { inputUse |= DrawOrderField(c, Page->OrderFields + (i+1), 1056, yPos, i); }
        
        yPos -= 20;
    }
    
    // Dice Throwers
    s32 xPos = 20;
    yPos = 90;
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrow *f = Page->Throwers + i;
        
        inputUse |= ls_uiTextBox(c, &f->name,   xPos,       yPos,      120, 20);
        inputUse |= ls_uiTextBox(c, &f->toHit,  xPos,       yPos + 24, 138, 20);
        inputUse |= ls_uiTextBox(c, &f->hitRes, xPos + 138, yPos + 24, 36,  20);
        inputUse |= ls_uiTextBox(c, &f->damage, xPos,       yPos + 48, 138, 20);
        inputUse |= ls_uiTextBox(c, &f->dmgRes, xPos + 138, yPos + 48, 36,  20);
        
        inputUse |= ls_uiButton(c, &f->throwDie, xPos + 126, yPos, 48, 20);
        
        xPos += 212;
    }
    
    xPos = 20;
    yPos = 42;
    // General Thrower
    {
        inputUse |= ls_uiTextBox(c, &Page->GeneralThrower.toHit,  xPos,       yPos, 168, 20);
        inputUse |= ls_uiTextBox(c, &Page->GeneralThrower.hitRes, xPos + 168, yPos, 36,  20);
        
        inputUse |= ls_uiButton(c, &Page->GeneralThrower.throwDie, xPos + 210, yPos, 48, 20);
    }
    
    inputUse |= ls_uiButton(c, &Page->Reset, 670, 678, 48, 20);
    
    if(State.inBattle)
    {
        inputUse |= ls_uiTextBox(c, &Page->Current,      1010, 668, 100, 20);
        inputUse |= ls_uiTextBox(c, &Page->RoundCounter, 1230, 720, 30, 20);
        
        inputUse |= ls_uiButton(c, &Page->Next, 1036, 698, 48, 20);
    }
    
    return inputUse;
}

b32 DrawPranaStyle(UIContext *c)
{
    InitPage *Page = State.Init;
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    
    b32 inputUse = FALSE;
    
    ls_uiSelectFontByFontSize(c, FS_SMALL);
    
    s32 yPos = 780;
    //NOTE: Z Layer 1 Input
    {
        if(!State.inBattle)
        {
            inputUse |= ls_uiListBox(c, &Page->EncounterSel,  495, yPos, 120, 20, 2);
            inputUse |= ls_uiTextBox(c, &Page->EncounterName, 644, yPos, 120, 20);
            
            inputUse |= ls_uiButton(c, &Page->SaveEnc, 617, yPos+22, 44, 20);
            inputUse |= ls_uiButton(c, &Page->RemoveEnc, 455, yPos, 24, 20);
            
            inputUse |= ls_uiListBox(c, &Page->Mobs,     50, yPos-65, 100, 20, 1);
            inputUse |= ls_uiListBox(c, &Page->Allies, 1094, yPos-225, 100, 20, 1);
            
            inputUse |= ls_uiButton(c, &Page->Roll, 536, yPos-40, 48, 20);
            inputUse |= ls_uiButton(c, &Page->Set,  698, yPos-40, 48, 20);
            inputUse |= ls_uiButton(c, &Page->Reset, 616, yPos-40, 48, 20);
        }
        else
        {
            inputUse |= ls_uiListBox(c, &Page->EncounterSel,  565, yPos, 120, 20, 2);
            
            if(Page->EncounterSel.selectedIndex > 0) 
            { inputUse |= ls_uiButton(c, &Page->AddEnc, 520, yPos, 24, 20); }
        }
        
        //NOTE: We hijack the globals to know when to show the buttons.
        if(distanceFromOld != 0) { inputUse |= ls_uiButton(c, &Page->Undo, 1180, yPos+22, 24, 20); }
        if(distanceFromNow != 0) { inputUse |= ls_uiButton(c, &Page->Redo, 1220, yPos+22, 24, 20); }
    }
    
    
    yPos = 678;
    //NOTE: Z Layer 0 Input
    if(!State.inBattle)
    {
        // Party
        for(u32 i = 0; i < PARTY_NUM; i++)
        {
            ls_uiLabel(c, PartyName[i], 1109, yPos+6);
            inputUse |= ls_uiTextBox(c, Page->PlayerInit + i, 1194, yPos, 32, 20);
            yPos -= 20;
        }
        
        // Enemies
        yPos = 678;
        for(u32 i = 0; i < visibleMobs; i++)
        {
            inputUse |= DrawInitField(c, Page->MobFields + i, 18, yPos, i, 162);
            yPos -= 20;
        }
        
        yPos = 678;
        for(u32 i = 0; i < visibleAllies; i++)
        {
            inputUse |= DrawInitField(c, Page->AllyFields + i, 1072, yPos-160, i+MOB_NUM, 136);
            yPos -= 20;
        }
        
        yPos = 678;
        if(State.Init->isAdding)
        {
            AssertMsgF(globalSelectedIndex >= 0, "Selected Index %d is not set\n", globalSelectedIndex);
            AssertMsgF(globalSelectedIndex <= visibleAllies+MOB_NUM, "Selected Index %d is out of bounds\n", globalSelectedIndex);
            
            InitField *f = 0;
            if(globalSelectedIndex >= MOB_NUM) { f = Page->AllyFields + (globalSelectedIndex - MOB_NUM); }
            else                               { f = Page->MobFields + globalSelectedIndex; }
            
            inputUse |= DrawInitExtra(c, f, 436, yPos);
        }
        else if(globalSelectedIndex >= 0)
        {
            AssertMsgF(globalSelectedIndex < visibleAllies+MOB_NUM, "Selected Index %d is out of bounds\n", globalSelectedIndex);
            
            InitField *f = 0;
            if(globalSelectedIndex >= MOB_NUM) { f = Page->AllyFields + (globalSelectedIndex - MOB_NUM); }
            else                               { f = Page->MobFields + globalSelectedIndex; }
            
            if(f->compendiumIdx == -1)
            {
                inputUse |= DrawInitExtra(c, f, 436, yPos);
            }
            else
            { 
                static UIScrollableRegion initViewScroll = { 260, 218, 780, 478, 0, 0, 998, 218 };
                
                if(mainCachedPage.pageIndex != f->compendiumIdx)
                { 
                    PageEntry pEntry = compendium.codex.pages[f->compendiumIdx];
                    CachePage(pEntry, f->compendiumIdx, &mainCachedPage);
                    initViewScroll = { 260, 218, 780, 478, 0, 0, 998, 218};
                }
                
                ls_uiStartScrollableRegion(c, &initViewScroll);
                initViewScroll.minY = DrawPage(c , &mainCachedPage, 260, 676, 998, 218);
                ls_uiEndScrollableRegion(c);
                
                ls_uiRect(c, 260, 218, 780, 478, RGBg(0x33), RGBg(0x11));
            }
        }
        
        //Add New
        if(visibleMobs <= MOB_NUM)
        {
            if(!(State.Init->isAdding && globalSelectedIndex >= MOB_NUM))
            { inputUse |= ls_uiButton(c, &Page->addNewMob, 180, 715, 25, 20); }
        }
        
        if(visibleAllies <= ALLY_NUM)
        {
            if(!(State.Init->isAdding && globalSelectedIndex < MOB_NUM))
            { inputUse |= ls_uiButton(c, &Page->addNewAlly, 1224, 780-225, 25, 20); }
        }
    }
    else
    {
        inputUse |= ls_uiTextBox(c, &Page->Current,      952, 668, 170, 20);
        inputUse |= ls_uiTextBox(c, &Page->RoundCounter, 1150, 698, 30, 20);
        inputUse |= ls_uiButton(c, &Page->Next, 1016, 698, 48, 20);
        
        // Order
        yPos = 638;
        for(u32 i = 0; i < visibleOrder; i += 2)
        {
            inputUse |= DrawOrderField(c, Page->OrderFields + i, 815, yPos, i);
            
            if((i+1) < visibleOrder)
            { inputUse |= DrawOrderField(c, Page->OrderFields + (i+1), 1049, yPos, i+1); }
            
            yPos -= 20;
        }
        
        yPos = 678;
        if(State.Init->isAdding)
        {
            AssertMsg(globalSelectedIndex >= 0, "Selected Index is not set\n");
            AssertMsg(globalSelectedIndex <= visibleAllies+MOB_NUM, "Selected Index is out of bounds\n");
            
            InitField *f = 0;
            if(globalSelectedIndex >= MOB_NUM) { f = Page->AllyFields + (globalSelectedIndex - MOB_NUM); }
            else                               { f = Page->MobFields + globalSelectedIndex; }
            
            inputUse |= DrawInitExtra(c, f, 66, yPos);
        }
        else if(globalSelectedIndex >= 0)
        {
            AssertMsg(globalSelectedIndex < visibleOrder, "Selected Order Index is out of bounds\n");
            Order *ord = Page->OrderFields + globalSelectedIndex;
            InitField *f = GetInitFieldByID(ord->ID);
            
            if(ord->compendiumIdx == -1) {
                if(f) inputUse |= DrawInitExtra(c, f, 66, yPos);
            }
            else
            { 
                static UIScrollableRegion viewScroll = { 40, 218, 760, 478, 0, 0, 758, 218};
                
                if(mainCachedPage.pageIndex != ord->compendiumIdx)
                { 
                    PageEntry pEntry = compendium.codex.pages[ord->compendiumIdx];
                    CachePage(pEntry, ord->compendiumIdx, &mainCachedPage);
                    viewScroll = { 40, 218, 760, 478, 0, 0, 758, 218};
                }
                
                ls_uiStartScrollableRegion(c, &viewScroll);
                viewScroll.minY = DrawPage(c, &mainCachedPage, 40, 676, 758, 218);
                ls_uiEndScrollableRegion(c);
                
                ls_uiRect(c, 40, 218, 760, 478, RGBg(0x33), RGBg(0x11));
                
                if(f) {
                    //NOTE: Also draw the life box so that we can use it to add/remove HP
                    Color base = c->widgetColor;
                    c->widgetColor = ls_uiAlphaBlend(RGBA(0x1B, 0x18, 0x14, 150), base);
                    ls_uiLabel(c, U"PF", 307, 720);
                    inputUse |= ls_uiTextBox(c, &f->maxLife, 332, 715, 118, 20);
                    c->widgetColor = base;
                }
            }
        }
        inputUse |= ls_uiButton(c, &Page->Reset, 1212, 718, 48, 20);
        
        //Add New
        if(visibleMobs <= MOB_NUM)
        {
            if(!(State.Init->isAdding && globalSelectedIndex >= MOB_NUM))
            {
                ls_uiLabel(c, U"Add Enemy", 30, 720);
                inputUse |= ls_uiButton(c, &Page->addNewMob, 116, 715, 25, 20);
            }
        }
        
        if(visibleAllies <= ALLY_NUM)
        {
            if(!(State.Init->isAdding && globalSelectedIndex < MOB_NUM))
            {
                ls_uiLabel(c, U"Add Ally", 157, 720);
                inputUse |= ls_uiButton(c, &Page->addNewAlly, 235, 715, 25, 20);
            }
        }
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
        
        ls_uiLabel(c, ls_utf32Constant(CounterNames[i]), xPos, yPos+24);
        
        inputUse |= ls_uiTextBox(c, &f->name, xPos, yPos, 100, 20);
        inputUse |= ls_uiTextBox(c, &f->rounds, xPos + 105, yPos, 36, 20);
        
        if(!f->isActive) { inputUse |= ls_uiButton(c, &f->start, xPos + 146, yPos, 48, 20); }
        else
        {
            inputUse |= ls_uiButton(c, &f->plusOne, xPos + 146, yPos, 48, 20);
            inputUse |= ls_uiButton(c, &f->stop, xPos + 93, yPos+22, 48, 20);
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
        
        
        inputUse |= ls_uiTextBox(c, &f->name,    xPos,      yPos + 40, 98, 20);
        inputUse |= ls_uiButton(c, &f->throwDie, xPos + 98, yPos + 40, 36, 20);
        
        inputUse |= ls_uiTextBox(c, &f->damage, xPos,       yPos + 20, 134, 20);
        inputUse |= ls_uiTextBox(c, &f->dmgRes, xPos + 134, yPos + 20, 36,  20);
        
        inputUse |= ls_uiTextBox(c, &f->toHit,  xPos,       yPos, 134, 20);
        inputUse |= ls_uiTextBox(c, &f->hitRes, xPos + 134, yPos, 36,  20);
        
        xPos += xDiff;
        xDiff = 0;
    }
    
    return inputUse;
}

b32 DrawInitTab(UIContext *c)
{
    switch(currentStyle)
    {
        case INIT_STYLE_DEFAULT: return DrawDefaultStyle(c); break;
        case INIT_STYLE_PRANA:   return DrawPranaStyle(c); break;
        
        default: AssertMsg(FALSE, "Unhandled Init Style\n"); return  FALSE;
    }
    
    AssertMsg(FALSE, "Unreachable code\n");
    return FALSE;
}