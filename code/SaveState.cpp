void CopyStateToBuffer(ProgramState *curr, buffer *buf, u32 saveV = global_saveVersion)
{
    //NOTE: Copy Init Page
    InitPage *init = curr->Init;
    
    //4
    ls_bufferAddDWord(buf, init->Mobs.selectedIndex);
    //8
    ls_bufferAddDWord(buf, init->Allies.selectedIndex);
    
    //8+(4*PARTY_NUM)
    for(u32 i = 0; i < party_count; i++)
    { ls_bufferAddUTF32(buf, init->PlayerInit[i].text); }
    
    //20 + [(4*IDX_COUNT) + 20]*ally_count = 20 + (64*ally_count) = 532
    for(u32 i = 0; i < ally_count; i++)
    {
        InitField *ally = init->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_bufferAddUTF32(buf, ally->editFields[j].text); }
        
        ls_bufferAddUTF32(buf, ally->maxLife.text);
        ls_bufferAddDWord(buf, ally->compendiumIdx);
        ls_bufferAddDWord(buf, ally->ID);
    }
    
    //532 + (64*mob_count) = 2.068
    for(u32 i = 0; i < mob_count; i++)
    {
        InitField *mob = init->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_bufferAddUTF32(buf, mob->editFields[j].text); }
        
        ls_bufferAddUTF32(buf, mob->maxLife.text);
        ls_bufferAddDWord(buf, mob->compendiumIdx);
        ls_bufferAddDWord(buf, mob->ID);
    }
    
    //2.068 + (36*ORDER_NUM) = 3.328
    for(u32 i = 0; i < order_count; i++)
    {
        Order *order = init->OrderFields + i;
        
        ls_bufferAddUTF32(buf, order->field.text);
        
        ls_bufferAddDWord(buf, order->field.currValue);
        ls_bufferAddDWord(buf, order->field.maxValue);
        ls_bufferAddDWord(buf, order->field.minValue);
        ls_bufferAddDouble(buf, order->field.currPos);
        
        //Status Conditions, we store the check isActive status
        for(s32 statusIdx = 0; statusIdx < STATUS_COUNT; statusIdx++)
        { ls_bufferAddDWord(buf, order->status[statusIdx].check.isActive); }
        
        //TODO: Probably can be removed
        ls_bufferAddData32(buf, &order->field.lColor, sizeof(Color));
        ls_bufferAddData32(buf, &order->field.rColor, sizeof(Color));
        
        ls_bufferAddDWord(buf, order->compendiumIdx);
        ls_bufferAddDWord(buf, order->ID);
    }
    
    //3.328 + 24 = 3.352
    ls_bufferAddDWord(buf, init->turnsInRound);
    ls_bufferAddDWord(buf, init->orderAdjust);
    
    ls_bufferAddUTF32(buf, init->RoundCounter.text);
    ls_bufferAddDWord(buf, init->roundCount);
    
    ls_bufferAddUTF32(buf, init->Current.text);
    ls_bufferAddDWord(buf, init->currIdx);
    
    //3.352 + (24*COUNTER_NUM) = 3.568
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *counter = init->Counters + i;
        
        ls_bufferAddUTF32(buf, counter->name.text);
        ls_bufferAddUTF32(buf, counter->rounds.text);
        
        ls_bufferAddDWord(buf, counter->roundsLeft);
        ls_bufferAddDWord(buf, counter->startIdxInOrder);
        ls_bufferAddDWord(buf, counter->turnCounter);
        ls_bufferAddDWord(buf, counter->isActive);
    }
    
    //3.568 + (20*THROWER_NUM) = 3.728
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrowBox *thrower = init->Throwers + i;
        
        ls_bufferAddUTF32(buf, thrower->name.text);
        ls_bufferAddUTF32(buf, thrower->toHit.text);
        ls_bufferAddUTF32(buf, thrower->hitRes.text);
        ls_bufferAddUTF32(buf, thrower->damage.text);
        ls_bufferAddUTF32(buf, thrower->dmgRes.text);
    }
    
    //3.728 + 28 = 3.756 bytes in an Undo State
    ls_bufferAddUTF32(buf, init->GeneralThrower.name.text);
    ls_bufferAddUTF32(buf, init->GeneralThrower.toHit.text);
    ls_bufferAddUTF32(buf, init->GeneralThrower.hitRes.text);
    ls_bufferAddUTF32(buf, init->GeneralThrower.damage.text);
    ls_bufferAddUTF32(buf, init->GeneralThrower.dmgRes.text);
    
    ls_bufferAddDWord(buf, init->EncounterSel.selectedIndex);
    
    //NOTE: Copy General Info
    ls_bufferAddDWord(buf, curr->inBattle);
    ls_bufferAddDWord(buf, addID);
}

void CopyStateFromBufferV6(ProgramState *curr, buffer *buf, u32 saveV = global_saveVersion)
{
    //NOTE: Copy Init Page
    InitPage *init = curr->Init;
    
    init->Mobs.selectedIndex   = (s32)ls_bufferReadDWord(buf);
    init->Allies.selectedIndex = (s32)ls_bufferReadDWord(buf);
    
    for(u32 i = 0; i < party_count; i++)
    { ls_bufferReadIntoUTF32(buf, &init->PlayerInit[i].text); }
    
    for(u32 i = 0; i < ally_count; i++)
    {
        InitField *ally = init->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_bufferReadIntoUTF32(buf, &ally->editFields[j].text); }
        
        if(ally->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(ally->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            ally->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &ally->maxLife.text);
        ally->compendiumIdx = ls_bufferReadDWord(buf);
        ally->ID            = ls_bufferReadDWord(buf);
    }
    
    for(u32 i = 0; i < mob_count; i++)
    {
        InitField *mob = init->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_bufferReadIntoUTF32(buf, &mob->editFields[j].text); }
        
        if(mob->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(mob->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            mob->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &mob->maxLife.text);
        mob->compendiumIdx = ls_bufferReadDWord(buf);
        mob->ID            = ls_bufferReadDWord(buf);
    }
    
    for(u32 i = 0; i < order_count; i++)
    {
        Order *order = init->OrderFields + i;
        
        ls_bufferReadIntoUTF32(buf, &order->field.text);
        order->field.currValue = ls_bufferReadDWord(buf);
        order->field.maxValue  = ls_bufferReadDWord(buf);
        order->field.minValue  = ls_bufferReadDWord(buf);
        order->field.currPos   = ls_bufferReadDouble(buf);
        
        //Status Conditions, we store the check isActive status
        for(s32 statusIdx = 0; statusIdx < STATUS_COUNT; statusIdx++) // @V7
        { order->status[statusIdx].check.isActive = FALSE; }
        
        ls_bufferReadData32(buf, &order->field.lColor);
        ls_bufferReadData32(buf, &order->field.rColor);
        
        order->compendiumIdx = ls_bufferReadDWord(buf);
        order->ID            = ls_bufferReadDWord(buf);
    }
    
    init->turnsInRound = ls_bufferReadDWord(buf);
    init->orderAdjust = ls_bufferReadDWord(buf);
    
    ls_bufferReadIntoUTF32(buf, &init->RoundCounter.text);
    init->roundCount = ls_bufferReadDWord(buf);
    
    ls_bufferReadIntoUTF32(buf, &init->Current.text);
    init->currIdx = ls_bufferReadDWord(buf);
    
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *counter = init->Counters + i;
        
        ls_bufferReadIntoUTF32(buf, &counter->name.text);
        ls_bufferReadIntoUTF32(buf, &counter->rounds.text);
        
        counter->roundsLeft = ls_bufferReadDWord(buf);
        counter->startIdxInOrder = ls_bufferReadDWord(buf);
        counter->turnCounter = ls_bufferReadDWord(buf);
        counter->isActive = ls_bufferReadDWord(buf);
    }
    
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrowBox *thrower = init->Throwers + i;
        
        ls_bufferReadIntoUTF32(buf, &thrower->name.text);
        ls_bufferReadIntoUTF32(buf, &thrower->toHit.text);
        ls_bufferReadIntoUTF32(buf, &thrower->hitRes.text);
        ls_bufferReadIntoUTF32(buf, &thrower->damage.text);
        ls_bufferReadIntoUTF32(buf, &thrower->dmgRes.text);
    }
    
    ls_bufferReadIntoUTF32(buf, &init->GeneralThrower.name.text);
    ls_bufferReadIntoUTF32(buf, &init->GeneralThrower.toHit.text);
    ls_bufferReadIntoUTF32(buf, &init->GeneralThrower.hitRes.text);
    ls_bufferReadIntoUTF32(buf, &init->GeneralThrower.damage.text);
    ls_bufferReadIntoUTF32(buf, &init->GeneralThrower.dmgRes.text);
    
    
    init->EncounterSel.selectedIndex = ls_bufferReadDWord(buf);
    
    //NOTE: Copy General Info
    curr->inBattle = ls_bufferReadDWord(buf);
    addID          = ls_bufferReadDWord(buf);
}

void CopyStateFromBuffer(ProgramState *curr, buffer *buf, u32 saveV = global_saveVersion)
{
    //NOTE: Copy Init Page
    InitPage *init = curr->Init;
    
    init->Mobs.selectedIndex   = (s32)ls_bufferReadDWord(buf);
    init->Allies.selectedIndex = (s32)ls_bufferReadDWord(buf);
    
    for(u32 i = 0; i < party_count; i++)
    { ls_bufferReadIntoUTF32(buf, &init->PlayerInit[i].text); }
    
    for(u32 i = 0; i < ally_count; i++)
    {
        InitField *ally = init->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_bufferReadIntoUTF32(buf, &ally->editFields[j].text); }
        
        if(ally->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(ally->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            ally->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &ally->maxLife.text);
        ally->compendiumIdx = ls_bufferReadDWord(buf);
        ally->ID            = ls_bufferReadDWord(buf);
    }
    
    for(u32 i = 0; i < mob_count; i++)
    {
        InitField *mob = init->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_bufferReadIntoUTF32(buf, &mob->editFields[j].text); }
        
        if(mob->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(mob->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            mob->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &mob->maxLife.text);
        mob->compendiumIdx = ls_bufferReadDWord(buf);
        mob->ID            = ls_bufferReadDWord(buf);
    }
    
    for(u32 i = 0; i < order_count; i++)
    {
        Order *order = init->OrderFields + i;
        
        ls_bufferReadIntoUTF32(buf, &order->field.text);
        order->field.currValue = ls_bufferReadDWord(buf);
        order->field.maxValue  = ls_bufferReadDWord(buf);
        order->field.minValue  = ls_bufferReadDWord(buf);
        order->field.currPos   = ls_bufferReadDouble(buf);
        
        //Status Conditions, we store the check isActive status
        for(s32 statusIdx = 0; statusIdx < STATUS_COUNT; statusIdx++)
        { order->status[statusIdx].check.isActive = ls_bufferReadDWord(buf); }
        
        ls_bufferReadData32(buf, &order->field.lColor);
        ls_bufferReadData32(buf, &order->field.rColor);
        
        order->compendiumIdx = ls_bufferReadDWord(buf);
        order->ID            = ls_bufferReadDWord(buf);
    }
    
    init->turnsInRound = ls_bufferReadDWord(buf);
    init->orderAdjust = ls_bufferReadDWord(buf);
    
    ls_bufferReadIntoUTF32(buf, &init->RoundCounter.text);
    init->roundCount = ls_bufferReadDWord(buf);
    
    ls_bufferReadIntoUTF32(buf, &init->Current.text);
    init->currIdx = ls_bufferReadDWord(buf);
    
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *counter = init->Counters + i;
        
        ls_bufferReadIntoUTF32(buf, &counter->name.text);
        ls_bufferReadIntoUTF32(buf, &counter->rounds.text);
        
        counter->roundsLeft = ls_bufferReadDWord(buf);
        counter->startIdxInOrder = ls_bufferReadDWord(buf);
        counter->turnCounter = ls_bufferReadDWord(buf);
        counter->isActive = ls_bufferReadDWord(buf);
    }
    
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrowBox *thrower = init->Throwers + i;
        
        ls_bufferReadIntoUTF32(buf, &thrower->name.text);
        ls_bufferReadIntoUTF32(buf, &thrower->toHit.text);
        ls_bufferReadIntoUTF32(buf, &thrower->hitRes.text);
        ls_bufferReadIntoUTF32(buf, &thrower->damage.text);
        ls_bufferReadIntoUTF32(buf, &thrower->dmgRes.text);
    }
    
    ls_bufferReadIntoUTF32(buf, &init->GeneralThrower.name.text);
    ls_bufferReadIntoUTF32(buf, &init->GeneralThrower.toHit.text);
    ls_bufferReadIntoUTF32(buf, &init->GeneralThrower.hitRes.text);
    ls_bufferReadIntoUTF32(buf, &init->GeneralThrower.damage.text);
    ls_bufferReadIntoUTF32(buf, &init->GeneralThrower.dmgRes.text);
    
    
    init->EncounterSel.selectedIndex = ls_bufferReadDWord(buf);
    
    //NOTE: Copy General Info
    curr->inBattle = ls_bufferReadDWord(buf);
    addID          = ls_bufferReadDWord(buf);
}

b32 LoadStateV6(UIContext *c, buffer *buf)
{
    ls_arenaUse(globalArena);
    
    ls_bufferSeekBegin(buf);
    
    u32 fileVersion = ls_bufferReadDWord(buf);
    AssertMsg(fileVersion == 6, "Save File version is not 6. How could we have reached here?\n");
    if(fileVersion != 6) { ls_bufferDestroy(buf); return FALSE; }
    
    //NOTE: Deserialize party_count, max_mob_count, max_ally_count @V9-V*
    party_count = 4;
    mob_count   = 24;
    ally_count  = 8;
    order_count = party_count + mob_count + ally_count;
    
    if(party_count > MAX_PARTY_NUM) { party_count = MAX_PARTY_NUM; }
    if(mob_count   > MAX_MOB_NUM)   { mob_count   = MAX_MOB_NUM; }
    if(ally_count  > MAX_ALLY_NUM)  { ally_count  = MAX_ALLY_NUM; }
    if(order_count > MAX_ORDER_NUM) { order_count = MAX_ORDER_NUM; }
    
    //The party names are not being loaded, because they are not present
    // We are using the default values in the array in Init.h
    
    //NOTE: Random Stuff to de-serialize
    currentStyle = INIT_STYLE_PRANA; // @V7-V*
    currentTheme = THEME_DEFAULT;    // @V7-V*
    
    //NOTE: Custom Theme Colors. @V8-V*
    State.backgroundColor = RGBg(0x38);
    State.borderColor     = RGBg(0x22);
    State.menuBarColor    = RGBg(0x20);
    State.highliteColor   = RGBg(0x65);
    State.pressedColor    = RGBg(0x75);
    State.widgetColor     = RGBg(0x45);
    State.textColor       = RGBg(0xCC);
    State.invWidgetColor  = RGBg(0xBA);
    State.invTextColor    = RGBg(0x33);
    
    //NOTE: Init Starts Here
    addID                          = ls_bufferReadDWord(buf);
    State.inBattle                 = ls_bufferReadDWord(buf);
    State.encounters.numEncounters = ls_bufferReadDWord(buf);
    
    InitPage *Page = State.Init;
    
    //TODO: Make Encounter's memory management better.
    //      They are currently allocated here during loading.
    //Unserialize Encounters
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    {
        Encounter *curr = State.encounters.Enc + i;
        
        curr->name = ls_bufferReadUTF32(buf);
        
        curr->numMobs = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numMobs; j++)
        {
            EncounterInitEntry *e = curr->mob + j;
            
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { e->fields[k] = ls_bufferReadUTF32(buf); }
            
            e->compendiumIdx = ls_bufferReadDWord(buf);
        }
        
        curr->numAllies = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numAllies; j++)
        {
            EncounterInitEntry *e = curr->ally + j;
            
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { e->fields[k] = ls_bufferReadUTF32(buf); }
            
            e->compendiumIdx = ls_bufferReadDWord(buf);
        }
        
        for(u32 j = 0; j < THROWER_NUM; j++)
        {
            curr->throwerName[j]   = ls_bufferReadUTF32(buf);
            curr->throwerHit[j]    = ls_bufferReadUTF32(buf);
            curr->throwerDamage[j] = ls_bufferReadUTF32(buf);
        }
        
        ls_uiListBoxAddEntry(c, &Page->EncounterSel, curr->name);
    }
    
    //NOTE: UnSerialize Undo Chain
    {
        u32 maxUndos = ls_bufferReadDWord(buf);
        AssertMsg(maxUndos == MAX_UNDO_STATES, "Save File not converted properly. Max Undo States don't coincide\n");
        
        matchingUndoIdx = ls_bufferReadDWord(buf);
        distanceFromOld = ls_bufferReadDWord(buf);
        distanceFromNow = ls_bufferReadDWord(buf);
        
        for(u32 i = 0; i < MAX_UNDO_STATES; i++)
        {
            ProgramState *curr = UndoStates + i;
            CopyStateFromBufferV6(curr, buf);
        }
    }
    
    //NOTE: UnSerialize Player Initiative
    u32 currPartyNum = ls_bufferReadDWord(buf);
    u32 unserializePartyNum = currPartyNum < party_count ? currPartyNum : party_count;
    for(u32 i = 0; i < unserializePartyNum; i++)
    {
        ls_bufferReadIntoUTF32(buf, &Page->PlayerInit[i].text);
    }
    
    
    //NOTE: UnSerialize Mob Initiative
    s32 visibleMobs          = ls_bufferReadDWord(buf);
    Page->Mobs.selectedIndex = visibleMobs;
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        {
            ls_bufferReadIntoUTF32(buf, &f->editFields[j].text);
        }
        
        //NOTE: For the EXTRA editField, because it is multi-line 
        //      we need to count how many lines there are, and set it.
        if(f->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(f->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            f->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &f->maxLife.text);
        
        f->compendiumIdx = ls_bufferReadDWord(buf);
        f->ID            = ls_bufferReadDWord(buf);
    }
    
    
    //NOTE: UnSerialize Ally Initiative
    s32 visibleAllies          = ls_bufferReadDWord(buf);
    Page->Allies.selectedIndex = visibleAllies;
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        {
            ls_bufferReadIntoUTF32(buf, &f->editFields[j].text);
        }
        
        //NOTE: For the EXTRA editField, because it is multi-line 
        //      we need to count how many lines there are, and set it.
        if(f->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(f->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            f->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &f->maxLife.text);
        
        f->compendiumIdx = ls_bufferReadDWord(buf);
        f->ID            = ls_bufferReadDWord(buf);
    }
    
    //NOTE: Quick Exit if not in battle after the save.
    if(State.inBattle == FALSE) 
    { 
        ls_bufferDestroy(buf);
        return TRUE;
    }
    
    //NOTE: Set all Init Fields to ReadOnly because we are in battle
    for(u32 i = 0; i < unserializePartyNum; i++)
    { Page->PlayerInit[i].isReadonly = TRUE; }
    
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { f->editFields[j].isReadonly = TRUE; }
    }
    
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { f->editFields[j].isReadonly = TRUE; }
    }
    
    //NOTE: UnSerialize Order
    Page->orderAdjust = ls_bufferReadDWord(buf);
    s32 visibleOrder  = visibleMobs + visibleAllies + party_count - Page->orderAdjust;
    for(u32 i = 0; i < visibleOrder; i++)
    {
        Order *f = Page->OrderFields + i;
        
        ls_bufferReadIntoUTF32(buf, &f->field.text);
        
        f->pos.isReadonly = TRUE;
        
        //HP Slider
        f->field.maxValue = ls_bufferReadDWord(buf);
        f->field.minValue = ls_bufferReadDWord(buf);
        f->field.currPos  = ls_bufferReadDouble(buf);
        
        //Status Conditions, we store the check isActive status
        for(s32 statusIdx = 0; statusIdx < STATUS_COUNT; statusIdx++) // @V7
        { f->status[statusIdx].check.isActive = FALSE; }
        
        f->compendiumIdx  = ls_bufferReadDWord(buf);
        f->ID             = ls_bufferReadDWord(buf);
        
        s32 currVal = ls_uiSliderGetValue(c, &f->field);
        
        if(currVal == 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xFF, 0x97, 0x12, 0x99), c->widgetColor); }
        
        else if(currVal < 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xDD, 0x10, 0x20, 0x99), c->widgetColor); }
        
        else if(currVal > 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xF0, 0xFF, 0x3D, 0x99), c->widgetColor); }
    }
    
    Page->turnsInRound = ls_bufferReadDWord(buf);
    
    //NOTE: Current In Battle
    {
        Page->currIdx = ls_bufferReadDWord(buf);
        
        ls_bufferReadIntoUTF32(buf, &Page->Current.text);
    }
    
    
    //NOTE: Round Counter
    {
        Page->roundCount = ls_bufferReadDWord(buf);
        ls_bufferReadIntoUTF32(buf, &Page->RoundCounter.text);
    }
    
    //NOTE: Counters
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *C = Page->Counters + i;
        
        ls_bufferReadIntoUTF32(buf, &C->name.text);
        
        ls_bufferReadIntoUTF32(buf, &C->rounds.text);
        
        C->roundsLeft      = ls_bufferReadDWord(buf);
        C->isActive        = ls_bufferReadDWord(buf);
        C->turnCounter     = ls_bufferReadDWord(buf);
        C->startIdxInOrder = ls_bufferReadDWord(buf);
    }
    
    
    //NOTE: Throwers
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrowBox *f = Page->Throwers + i;
        
        ls_bufferReadIntoUTF32(buf, &f->name.text);
        ls_bufferReadIntoUTF32(buf, &f->toHit.text);
        ls_bufferReadIntoUTF32(buf, &f->hitRes.text);
        ls_bufferReadIntoUTF32(buf, &f->damage.text);
        ls_bufferReadIntoUTF32(buf, &f->dmgRes.text);
    }
    
    ls_bufferDestroy(buf);
    
    return TRUE;
}

b32 LoadStateV7(UIContext *c, buffer *buf)
{
    ls_arenaUse(globalArena);
    
    ls_bufferSeekBegin(buf);
    
    u32 fileVersion = ls_bufferReadDWord(buf);
    AssertMsg(fileVersion == 7, "Save File version is not 7. How could we have reached here?\n");
    if(fileVersion != 7) { ls_bufferDestroy(buf); return FALSE; }
    
    //NOTE: Deserialize party_count, max_mob_count, max_ally_count @V9-V*
    party_count = 4;
    mob_count   = 24;
    ally_count  = 8;
    order_count = party_count + mob_count + ally_count;
    
    if(party_count > MAX_PARTY_NUM) { party_count = MAX_PARTY_NUM; }
    if(mob_count   > MAX_MOB_NUM)   { mob_count   = MAX_MOB_NUM; }
    if(ally_count  > MAX_ALLY_NUM)  { ally_count  = MAX_ALLY_NUM; }
    if(order_count > MAX_ORDER_NUM) { order_count = MAX_ORDER_NUM; }
    
    //The party names are not being loaded, because they are not present
    // We are using the default values in the array in Init.h
    
    //NOTE: Random stuff to de-serialize
    currentStyle = (InitStyle)ls_bufferReadDWord(buf);
    currentTheme = (ProgramTheme)ls_bufferReadDWord(buf);
    
    //NOTE: Custom Theme Colors. @V8
    State.backgroundColor = RGBg(0x38);
    State.borderColor     = RGBg(0x22);
    State.menuBarColor    = RGBg(0x20);
    State.highliteColor   = RGBg(0x65);
    State.pressedColor    = RGBg(0x75);
    State.widgetColor     = RGBg(0x45);
    State.textColor       = RGBg(0xCC);
    State.invWidgetColor  = RGBg(0xBA);
    State.invTextColor    = RGBg(0x33);
    
    if(currentTheme == THEME_USER) { selectThemeProcs[THEME_DEFAULT](c, NULL); }
    else                           { selectThemeProcs[currentTheme](c, NULL); }
    
    //NOTE: Init Starts Here
    addID                          = ls_bufferReadDWord(buf);
    State.inBattle                 = ls_bufferReadDWord(buf);
    State.encounters.numEncounters = ls_bufferReadDWord(buf);
    
    InitPage *Page = State.Init;
    
    //TODO: Make Encounter's memory management better.
    //      They are currently allocated here during loading.
    //Unserialize Encounters
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    {
        Encounter *curr = State.encounters.Enc + i;
        
        curr->name = ls_bufferReadUTF32(buf);
        
        curr->numMobs = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numMobs; j++)
        {
            EncounterInitEntry *e = curr->mob + j;
            
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { e->fields[k] = ls_bufferReadUTF32(buf); }
            
            e->compendiumIdx = ls_bufferReadDWord(buf);
        }
        
        curr->numAllies = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numAllies; j++)
        {
            EncounterInitEntry *e = curr->ally + j;
            
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { e->fields[k] = ls_bufferReadUTF32(buf); }
            
            e->compendiumIdx = ls_bufferReadDWord(buf);
        }
        
        for(u32 j = 0; j < THROWER_NUM; j++)
        {
            curr->throwerName[j]   = ls_bufferReadUTF32(buf);
            curr->throwerHit[j]    = ls_bufferReadUTF32(buf);
            curr->throwerDamage[j] = ls_bufferReadUTF32(buf);
        }
        
        ls_uiListBoxAddEntry(c, &Page->EncounterSel, curr->name);
    }
    
    
    //NOTE: UnSerialize Undo Chain
    {
        u32 maxUndos = ls_bufferReadDWord(buf);
        AssertMsg(maxUndos == MAX_UNDO_STATES, "Save File not converted properly. Max Undo States don't coincide\n");
        
        matchingUndoIdx = ls_bufferReadDWord(buf);
        distanceFromOld = ls_bufferReadDWord(buf);
        distanceFromNow = ls_bufferReadDWord(buf);
        
        for(u32 i = 0; i < MAX_UNDO_STATES; i++)
        {
            ProgramState *curr = UndoStates + i;
            CopyStateFromBuffer(curr, buf);
        }
    }
    
    //NOTE: UnSerialize Player Initiative
    u32 currPartyNum = ls_bufferReadDWord(buf);
    u32 unserializePartyNum = currPartyNum < party_count ? currPartyNum : party_count;
    for(u32 i = 0; i < unserializePartyNum; i++)
    {
        ls_bufferReadIntoUTF32(buf, &Page->PlayerInit[i].text);
    }
    
    
    //NOTE: UnSerialize Mob Initiative
    s32 visibleMobs          = ls_bufferReadDWord(buf);
    Page->Mobs.selectedIndex = visibleMobs;
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        {
            ls_bufferReadIntoUTF32(buf, &f->editFields[j].text);
        }
        
        //NOTE: For the EXTRA editField, because it is multi-line 
        //      we need to count how many lines there are, and set it.
        if(f->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(f->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            f->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &f->maxLife.text);
        
        f->compendiumIdx = ls_bufferReadDWord(buf);
        f->ID            = ls_bufferReadDWord(buf);
    }
    
    
    //NOTE: UnSerialize Ally Initiative
    s32 visibleAllies          = ls_bufferReadDWord(buf);
    Page->Allies.selectedIndex = visibleAllies;
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        {
            ls_bufferReadIntoUTF32(buf, &f->editFields[j].text);
        }
        
        //NOTE: For the EXTRA editField, because it is multi-line 
        //      we need to count how many lines there are, and set it.
        if(f->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(f->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            f->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &f->maxLife.text);
        
        f->compendiumIdx = ls_bufferReadDWord(buf);
        f->ID            = ls_bufferReadDWord(buf);
    }
    
    //NOTE: Quick Exit if not in battle after the save.
    if(State.inBattle == FALSE) 
    { 
        ls_bufferDestroy(buf);
        return TRUE;
    }
    
    //NOTE: Set all Init Fields to ReadOnly because we are in battle
    for(u32 i = 0; i < unserializePartyNum; i++)
    { Page->PlayerInit[i].isReadonly = TRUE; }
    
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { f->editFields[j].isReadonly = TRUE; }
    }
    
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { f->editFields[j].isReadonly = TRUE; }
    }
    
    //NOTE: UnSerialize Order
    Page->orderAdjust = ls_bufferReadDWord(buf);
    s32 visibleOrder  = visibleMobs + visibleAllies + party_count - Page->orderAdjust;
    for(u32 i = 0; i < visibleOrder; i++)
    {
        Order *f = Page->OrderFields + i;
        
        ls_bufferReadIntoUTF32(buf, &f->field.text);
        
        f->pos.isReadonly = TRUE;
        
        //HP Slider
        f->field.maxValue = ls_bufferReadDWord(buf);
        f->field.minValue = ls_bufferReadDWord(buf);
        f->field.currPos  = ls_bufferReadDouble(buf);
        
        //Status Conditions, we store the check isActive status
        for(s32 statusIdx = 0; statusIdx < STATUS_COUNT; statusIdx++)
        { f->status[statusIdx].check.isActive = ls_bufferReadDWord(buf); }
        
        f->compendiumIdx  = ls_bufferReadDWord(buf);
        f->ID             = ls_bufferReadDWord(buf);
        
        s32 currVal = ls_uiSliderGetValue(c, &f->field);
        
        if(currVal == 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xFF, 0x97, 0x12, 0x99), c->widgetColor); }
        
        else if(currVal < 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xDD, 0x10, 0x20, 0x99), c->widgetColor); }
        
        else if(currVal > 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xF0, 0xFF, 0x3D, 0x99), c->widgetColor); }
    }
    
    Page->turnsInRound = ls_bufferReadDWord(buf);
    
    //NOTE: Current In Battle
    {
        Page->currIdx = ls_bufferReadDWord(buf);
        
        ls_bufferReadIntoUTF32(buf, &Page->Current.text);
    }
    
    
    //NOTE: Round Counter
    {
        Page->roundCount = ls_bufferReadDWord(buf);
        ls_bufferReadIntoUTF32(buf, &Page->RoundCounter.text);
    }
    
    //NOTE: Counters
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *C = Page->Counters + i;
        
        ls_bufferReadIntoUTF32(buf, &C->name.text);
        
        ls_bufferReadIntoUTF32(buf, &C->rounds.text);
        
        C->roundsLeft      = ls_bufferReadDWord(buf);
        C->isActive        = ls_bufferReadDWord(buf);
        C->turnCounter     = ls_bufferReadDWord(buf);
        C->startIdxInOrder = ls_bufferReadDWord(buf);
    }
    
    
    //NOTE: Throwers
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrowBox *f = Page->Throwers + i;
        
        ls_bufferReadIntoUTF32(buf, &f->name.text);
        ls_bufferReadIntoUTF32(buf, &f->toHit.text);
        ls_bufferReadIntoUTF32(buf, &f->hitRes.text);
        ls_bufferReadIntoUTF32(buf, &f->damage.text);
        ls_bufferReadIntoUTF32(buf, &f->dmgRes.text);
    }
    
    ls_bufferDestroy(buf);
    
    return TRUE;
}

b32 LoadStateV8(UIContext *c, buffer *buf)
{
    ls_arenaUse(globalArena);
    
    ls_bufferSeekBegin(buf);
    
    u32 fileVersion = ls_bufferReadDWord(buf);
    AssertMsg(fileVersion == 8, "Save File version is not 8. How could we have reached here?\n");
    if(fileVersion != 8) { ls_bufferDestroy(buf); return FALSE; }
    
    //NOTE: Deserialize party_count, max_mob_count, max_ally_count
    party_count = 4;
    mob_count   = 24;
    ally_count  = 8;
    order_count = party_count + mob_count + ally_count;
    
    if(party_count > MAX_PARTY_NUM) { party_count = MAX_PARTY_NUM; }
    if(mob_count   > MAX_MOB_NUM)   { mob_count   = MAX_MOB_NUM; }
    if(ally_count  > MAX_ALLY_NUM)  { ally_count  = MAX_ALLY_NUM; }
    if(order_count > MAX_ORDER_NUM) { order_count = MAX_ORDER_NUM; }
    
    //The party names are not being loaded, because they are not present
    // We are using the default values in the array in Init.h
    
    //NOTE: Random stuff to de-serialize
    currentStyle = (InitStyle)ls_bufferReadDWord(buf);
    currentTheme = (ProgramTheme)ls_bufferReadDWord(buf);
    
    //NOTE: Custom Theme Colors.
    State.backgroundColor.value = ls_bufferReadDWord(buf);
    State.borderColor.value     = ls_bufferReadDWord(buf);
    State.menuBarColor.value    = ls_bufferReadDWord(buf);
    State.highliteColor.value   = ls_bufferReadDWord(buf);
    State.pressedColor.value    = ls_bufferReadDWord(buf);
    State.widgetColor.value     = ls_bufferReadDWord(buf);
    State.textColor.value       = ls_bufferReadDWord(buf);
    State.invWidgetColor.value  = ls_bufferReadDWord(buf);
    State.invTextColor.value    = ls_bufferReadDWord(buf);
    
    selectThemeProcs[currentTheme](c, NULL);
    
    //NOTE: Init Starts Here
    addID                          = ls_bufferReadDWord(buf);
    State.inBattle                 = ls_bufferReadDWord(buf);
    State.encounters.numEncounters = ls_bufferReadDWord(buf);
    
    InitPage *Page = State.Init;
    
    //TODO: Make Encounter's memory management better.
    //      They are currently allocated here during loading.
    //Unserialize Encounters
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    {
        Encounter *curr = State.encounters.Enc + i;
        
        curr->name = ls_bufferReadUTF32(buf);
        
        curr->numMobs = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numMobs; j++)
        {
            EncounterInitEntry *e = curr->mob + j;
            
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { e->fields[k] = ls_bufferReadUTF32(buf); }
            
            e->compendiumIdx = ls_bufferReadDWord(buf);
        }
        
        curr->numAllies = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numAllies; j++)
        {
            EncounterInitEntry *e = curr->ally + j;
            
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { e->fields[k] = ls_bufferReadUTF32(buf); }
            
            e->compendiumIdx = ls_bufferReadDWord(buf);
        }
        
        for(u32 j = 0; j < THROWER_NUM; j++)
        {
            curr->throwerName[j]   = ls_bufferReadUTF32(buf);
            curr->throwerHit[j]    = ls_bufferReadUTF32(buf);
            curr->throwerDamage[j] = ls_bufferReadUTF32(buf);
        }
        
        ls_uiListBoxAddEntry(c, &Page->EncounterSel, curr->name);
    }
    
    
    //NOTE: UnSerialize Undo Chain
    {
        u32 maxUndos = ls_bufferReadDWord(buf);
        AssertMsg(maxUndos == MAX_UNDO_STATES, "Save File not converted properly. Max Undo States don't coincide\n");
        
        matchingUndoIdx = ls_bufferReadDWord(buf);
        distanceFromOld = ls_bufferReadDWord(buf);
        distanceFromNow = ls_bufferReadDWord(buf);
        
        for(u32 i = 0; i < MAX_UNDO_STATES; i++)
        {
            ProgramState *curr = UndoStates + i;
            CopyStateFromBuffer(curr, buf);
        }
    }
    
    //NOTE: UnSerialize Player Initiative
    u32 currPartyNum = ls_bufferReadDWord(buf);
    u32 unserializePartyNum = currPartyNum < party_count ? currPartyNum : party_count;
    for(u32 i = 0; i < unserializePartyNum; i++)
    {
        ls_bufferReadIntoUTF32(buf, &Page->PlayerInit[i].text);
    }
    
    
    //NOTE: UnSerialize Mob Initiative
    s32 visibleMobs          = ls_bufferReadDWord(buf);
    Page->Mobs.selectedIndex = visibleMobs;
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        {
            ls_bufferReadIntoUTF32(buf, &f->editFields[j].text);
        }
        
        //NOTE: For the EXTRA editField, because it is multi-line 
        //      we need to count how many lines there are, and set it.
        if(f->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(f->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            f->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &f->maxLife.text);
        
        f->compendiumIdx = ls_bufferReadDWord(buf);
        f->ID            = ls_bufferReadDWord(buf);
    }
    
    
    //NOTE: UnSerialize Ally Initiative
    s32 visibleAllies          = ls_bufferReadDWord(buf);
    Page->Allies.selectedIndex = visibleAllies;
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        {
            ls_bufferReadIntoUTF32(buf, &f->editFields[j].text);
        }
        
        //NOTE: For the EXTRA editField, because it is multi-line 
        //      we need to count how many lines there are, and set it.
        if(f->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(f->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            f->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &f->maxLife.text);
        
        f->compendiumIdx = ls_bufferReadDWord(buf);
        f->ID            = ls_bufferReadDWord(buf);
    }
    
    //NOTE: Quick Exit if not in battle after the save.
    if(State.inBattle == FALSE) 
    { 
        ls_bufferDestroy(buf);
        return TRUE;
    }
    
    //NOTE: Set all Init Fields to ReadOnly because we are in battle
    for(u32 i = 0; i < unserializePartyNum; i++)
    { Page->PlayerInit[i].isReadonly = TRUE; }
    
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { f->editFields[j].isReadonly = TRUE; }
    }
    
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { f->editFields[j].isReadonly = TRUE; }
    }
    
    //NOTE: UnSerialize Order
    Page->orderAdjust = ls_bufferReadDWord(buf);
    s32 visibleOrder  = visibleMobs + visibleAllies + party_count - Page->orderAdjust;
    for(u32 i = 0; i < visibleOrder; i++)
    {
        Order *f = Page->OrderFields + i;
        
        ls_bufferReadIntoUTF32(buf, &f->field.text);
        
        f->pos.isReadonly = TRUE;
        
        //HP Slider
        f->field.maxValue = ls_bufferReadDWord(buf);
        f->field.minValue = ls_bufferReadDWord(buf);
        f->field.currPos  = ls_bufferReadDouble(buf);
        
        //Status Conditions, we store the check isActive status
        for(s32 statusIdx = 0; statusIdx < STATUS_COUNT; statusIdx++)
        { f->status[statusIdx].check.isActive = ls_bufferReadDWord(buf); }
        
        f->compendiumIdx  = ls_bufferReadDWord(buf);
        f->ID             = ls_bufferReadDWord(buf);
        
        s32 currVal = ls_uiSliderGetValue(c, &f->field);
        
        if(currVal == 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xFF, 0x97, 0x12, 0x99), c->widgetColor); }
        
        else if(currVal < 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xDD, 0x10, 0x20, 0x99), c->widgetColor); }
        
        else if(currVal > 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xF0, 0xFF, 0x3D, 0x99), c->widgetColor); }
    }
    
    Page->turnsInRound = ls_bufferReadDWord(buf);
    
    //NOTE: Current In Battle
    {
        Page->currIdx = ls_bufferReadDWord(buf);
        
        ls_bufferReadIntoUTF32(buf, &Page->Current.text);
    }
    
    
    //NOTE: Round Counter
    {
        Page->roundCount = ls_bufferReadDWord(buf);
        ls_bufferReadIntoUTF32(buf, &Page->RoundCounter.text);
    }
    
    //NOTE: Counters
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *C = Page->Counters + i;
        
        ls_bufferReadIntoUTF32(buf, &C->name.text);
        
        ls_bufferReadIntoUTF32(buf, &C->rounds.text);
        
        C->roundsLeft      = ls_bufferReadDWord(buf);
        C->isActive        = ls_bufferReadDWord(buf);
        C->turnCounter     = ls_bufferReadDWord(buf);
        C->startIdxInOrder = ls_bufferReadDWord(buf);
    }
    
    
    //NOTE: Throwers
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrowBox *f = Page->Throwers + i;
        
        ls_bufferReadIntoUTF32(buf, &f->name.text);
        ls_bufferReadIntoUTF32(buf, &f->toHit.text);
        ls_bufferReadIntoUTF32(buf, &f->hitRes.text);
        ls_bufferReadIntoUTF32(buf, &f->damage.text);
        ls_bufferReadIntoUTF32(buf, &f->dmgRes.text);
    }
    
    ls_bufferDestroy(buf);
    
    return TRUE;
}

void SaveState(UIContext *c)
{
    (void)c;
    
    ls_arenaUse(saveArena);
    
    buffer state = ls_bufferInit(MBytes(1));
    buffer *buf = &state;
    
    ls_bufferAddDWord(buf, global_saveVersion);
    
    //NOTE: Serialize Party Count, Max Mob Count, Max Ally Count
    ls_bufferAddDWord(buf, party_count);
    ls_bufferAddDWord(buf, mob_count);
    ls_bufferAddDWord(buf, ally_count);
    
    for(s32 i = 0; i < party_count; i++)
    { ls_bufferAddUTF32(buf, State.PartyName[i].text); }
    
    //NOTE: Random stuff to serialize
    ls_bufferAddDWord(buf, currentStyle);
    ls_bufferAddDWord(buf, currentTheme);
    
    //NOTE: Add the custom theme colors
    ls_bufferAddDWord(buf, State.backgroundColor.value);
    ls_bufferAddDWord(buf, State.borderColor.value);
    ls_bufferAddDWord(buf, State.menuBarColor.value);
    ls_bufferAddDWord(buf, State.highliteColor.value);
    ls_bufferAddDWord(buf, State.pressedColor.value);
    ls_bufferAddDWord(buf, State.widgetColor.value);
    ls_bufferAddDWord(buf, State.textColor.value);
    ls_bufferAddDWord(buf, State.invWidgetColor.value);
    ls_bufferAddDWord(buf, State.invTextColor.value);
    
    //NOTE: Init Starts Here
    ls_bufferAddDWord(buf, addID);
    ls_bufferAddDWord(buf, State.inBattle);
    ls_bufferAddDWord(buf, State.encounters.numEncounters);
    
    InitPage *Page = State.Init;
    
    //NOTE Serialize Saved Encounters
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    {
        Encounter *curr = &State.encounters.Enc[i];
        
        ls_bufferAddUTF32(buf, curr->name);
        
        ls_bufferAddDWord(buf, curr->numMobs);
        for(u32 j = 0; j < curr->numMobs; j++)
        {
            EncounterInitEntry *e = curr->mob + j;
            
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { ls_bufferAddUTF32(buf, e->fields[k]); }
            
            ls_bufferAddDWord(buf, e->compendiumIdx);
        }
        
        ls_bufferAddDWord(buf, curr->numAllies);
        for(u32 j = 0; j < curr->numAllies; j++)
        {
            EncounterInitEntry *e = curr->ally + j;
            
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { ls_bufferAddUTF32(buf, e->fields[k]); }
            
            ls_bufferAddDWord(buf, e->compendiumIdx);
        }
        
        for(u32 j = 0; j < THROWER_NUM; j++)
        {
            ls_bufferAddUTF32(buf, curr->throwerName[j]);
            ls_bufferAddUTF32(buf, curr->throwerHit[j]);
            ls_bufferAddUTF32(buf, curr->throwerDamage[j]);
        }
        
    }
    
    //NOTE: Serialize Undo Chain
    {
        ls_bufferAddDWord(buf, MAX_UNDO_STATES);
        ls_bufferAddDWord(buf, matchingUndoIdx);
        ls_bufferAddDWord(buf, distanceFromOld);
        ls_bufferAddDWord(buf, distanceFromNow);
        
        for(u32 i = 0; i < MAX_UNDO_STATES; i++)
        {
            ProgramState *curr = UndoStates + i;
            CopyStateToBuffer(curr, buf);
        }
    }
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + party_count - Page->orderAdjust;
    
    //NOTE: Serialize Player Initiative
    ls_bufferAddDWord(buf, party_count);
    
    for(u32 i = 0; i < party_count; i++)
    { ls_bufferAddUTF32(buf, Page->PlayerInit[i].text); }
    
    
    //NOTE: Serialize Mob Initiative
    ls_bufferAddDWord(buf, visibleMobs);
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_bufferAddUTF32(buf, f->editFields[j].text); }
        
        ls_bufferAddUTF32(buf, f->maxLife.text);
        
        ls_bufferAddDWord(buf, f->compendiumIdx);
        ls_bufferAddDWord(buf, f->ID);
    }
    
    
    //NOTE: Serialize Ally Initiative
    ls_bufferAddDWord(buf, visibleAllies);
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_bufferAddUTF32(buf, f->editFields[j].text); }
        
        ls_bufferAddUTF32(buf, f->maxLife.text);
        
        ls_bufferAddDWord(buf, f->compendiumIdx);
        ls_bufferAddDWord(buf, f->ID);
    }
    
    
    //NOTE: Serialize Order
    //      visibleOrder is calculated from visibleMobs + visibleAllies + PARTY_NUM - orderAdjust,
    //      instead orderAdjust is not calculatable, so we store that instead
    ls_bufferAddDWord(buf, Page->orderAdjust);
    for(u32 i = 0; i < visibleOrder; i++)
    {
        Order *f = Page->OrderFields + i;
        
        ls_bufferAddUTF32(buf, f->field.text);
        
        //HP Slider
        ls_bufferAddDWord(buf,  f->field.maxValue);
        ls_bufferAddDWord(buf,  f->field.minValue);
        ls_bufferAddDouble(buf, f->field.currPos);
        
        //Status Conditions, we store the check isActive status
        for(s32 statusIdx = 0; statusIdx < STATUS_COUNT; statusIdx++)
        { ls_bufferAddDWord(buf, f->status[statusIdx].check.isActive); }
        
        ls_bufferAddDWord(buf, f->compendiumIdx);
        ls_bufferAddDWord(buf, f->ID);
    }
    
    ls_bufferAddDWord(buf, Page->turnsInRound);
    
    //NOTE: Current In Battle
    {
        ls_bufferAddDWord(buf, Page->currIdx);
        ls_bufferAddUTF32(buf, Page->Current.text);
    }
    
    
    //NOTE: Round Counter
    {
        ls_bufferAddDWord(buf, Page->roundCount);
        ls_bufferAddUTF32(buf, Page->RoundCounter.text);
    }
    
    
    //NOTE: Counters
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *C = Page->Counters + i;
        
        ls_bufferAddUTF32(buf, C->name.text);
        ls_bufferAddUTF32(buf, C->rounds.text);
        
        ls_bufferAddDWord(buf, C->roundsLeft);
        ls_bufferAddDWord(buf, C->isActive);
        ls_bufferAddDWord(buf, C->turnCounter);
        ls_bufferAddDWord(buf, C->startIdxInOrder);
    }
    
    
    //NOTE: Throwers
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrowBox *f = Page->Throwers + i;
        ls_bufferAddUTF32(buf, f->name.text);
        ls_bufferAddUTF32(buf, f->toHit.text);
        ls_bufferAddUTF32(buf, f->hitRes.text);
        ls_bufferAddUTF32(buf, f->damage.text);
        ls_bufferAddUTF32(buf, f->dmgRes.text);
    }
    
    //TODO: Actually use buf->cursor instead of buf->size??
    char outName[64] = {};
    ls_sprintf(outName, 64, "SaveFile", global_saveVersion);
    ls_writeFile(outName, (char *)buf->data, buf->size, FALSE);
    
    ls_bufferDestroy(buf);
    
    ls_arenaUse(globalArena);
    ls_arenaClear(saveArena);
    
    return;
}

typedef b32(*LoadFunc)(UIContext *c, buffer *buff);

const LoadFunc loadFunctions[] = { 0, 0, 0, 0, 0, LoadStateV6, LoadStateV7, LoadStateV8 };

b32 LoadState(UIContext *c)
{
    //NOTETODO: Changed this from saveArena. It was so stupid
    //          I am allocating a lot of data, on an arena that will be cleared at the end of the function
    //          It was working just out of pure luck
    //ls_arenaUse(saveArena);
    ls_arenaUse(globalArena);
    
    char fullPathBuff[128] = {};
    u32 len = ls_getFullPathName("SaveFile", fullPathBuff, 128);
    
    buffer state = {};
    buffer *buf = NULL;
    
    if(ls_fileExists(fullPathBuff) == FALSE) {
        
        //NOTE: Check legacy SaveFile_v6 version
        u32 len = ls_getFullPathName("SaveFile_v6", fullPathBuff, 128);
        if(ls_fileExists(fullPathBuff) == TRUE)
        {
            string path = ls_strConstant(fullPathBuff);
            state       = ls_bufferInitFromFile(path);
            buf         = &state;
            return loadFunctions[5](c, buf);
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        string path = ls_strConstant(fullPathBuff);
        state = ls_bufferInitFromFile(path);
        buf = &state;
    }
    
    ls_bufferSeekBegin(buf);
    
    //NOTE: Read Gloval Save Version
    u32 fileVersion = ls_bufferReadDWord(buf);
    if(fileVersion != global_saveVersion)
    {
        switch(fileVersion)
        {
            case 7: { return loadFunctions[6](c, buf); } break;
            case 8: { return loadFunctions[7](c, buf); } break;
            
            default: { ls_bufferDestroy(buf); return FALSE; }
        }
    }
    
    //NOTE: Deserialize party_count, max_mob_count, max_ally_count
    party_count = ls_bufferReadDWord(buf);
    mob_count   = ls_bufferReadDWord(buf);
    ally_count  = ls_bufferReadDWord(buf);
    order_count = party_count + mob_count + ally_count;
    
    for(s32 i = 0; i < party_count; i++)
    { ls_bufferReadIntoUTF32(buf, &State.PartyName[i].text); }
    
    //NOTE: Random stuff to de-serialize
    currentStyle = (InitStyle)ls_bufferReadDWord(buf);
    currentTheme = (ProgramTheme)ls_bufferReadDWord(buf);
    
    //NOTE: Custom Theme Colors.
    State.backgroundColor.value = ls_bufferReadDWord(buf);
    State.borderColor.value     = ls_bufferReadDWord(buf);
    State.menuBarColor.value    = ls_bufferReadDWord(buf);
    State.highliteColor.value   = ls_bufferReadDWord(buf);
    State.pressedColor.value    = ls_bufferReadDWord(buf);
    State.widgetColor.value     = ls_bufferReadDWord(buf);
    State.textColor.value       = ls_bufferReadDWord(buf);
    State.invWidgetColor.value  = ls_bufferReadDWord(buf);
    State.invTextColor.value    = ls_bufferReadDWord(buf);
    
    selectThemeProcs[currentTheme](c, NULL);
    
    //NOTE: Init Starts Here
    addID                          = ls_bufferReadDWord(buf);
    State.inBattle                 = ls_bufferReadDWord(buf);
    State.encounters.numEncounters = ls_bufferReadDWord(buf);
    
    InitPage *Page = State.Init;
    
    //TODO: Make Encounter's memory management better.
    //      They are currently allocated here during loading.
    //Unserialize Encounters
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    {
        Encounter *curr = State.encounters.Enc + i;
        
        curr->name = ls_bufferReadUTF32(buf);
        
        curr->numMobs = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numMobs; j++)
        {
            EncounterInitEntry *e = curr->mob + j;
            
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { e->fields[k] = ls_bufferReadUTF32(buf); }
            
            e->compendiumIdx = ls_bufferReadDWord(buf);
        }
        
        curr->numAllies = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numAllies; j++)
        {
            EncounterInitEntry *e = curr->ally + j;
            
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { e->fields[k] = ls_bufferReadUTF32(buf); }
            
            e->compendiumIdx = ls_bufferReadDWord(buf);
        }
        
        for(u32 j = 0; j < THROWER_NUM; j++)
        {
            curr->throwerName[j]   = ls_bufferReadUTF32(buf);
            curr->throwerHit[j]    = ls_bufferReadUTF32(buf);
            curr->throwerDamage[j] = ls_bufferReadUTF32(buf);
        }
        
        ls_uiListBoxAddEntry(c, &Page->EncounterSel, curr->name);
    }
    
    
    //NOTE: UnSerialize Undo Chain
    {
        u32 maxUndos = ls_bufferReadDWord(buf);
        AssertMsg(maxUndos == MAX_UNDO_STATES, "Save File not converted properly. Max Undo States don't coincide\n");
        
        matchingUndoIdx = ls_bufferReadDWord(buf);
        distanceFromOld = ls_bufferReadDWord(buf);
        distanceFromNow = ls_bufferReadDWord(buf);
        
        for(u32 i = 0; i < MAX_UNDO_STATES; i++)
        {
            ProgramState *curr = UndoStates + i;
            CopyStateFromBuffer(curr, buf);
        }
    }
    
    //NOTE: UnSerialize Player Initiative
    u32 currPartyNum = ls_bufferReadDWord(buf);
    u32 unserializePartyNum = currPartyNum < party_count ? currPartyNum : party_count;
    for(u32 i = 0; i < unserializePartyNum; i++)
    {
        ls_bufferReadIntoUTF32(buf, &Page->PlayerInit[i].text);
    }
    
    
    //NOTE: UnSerialize Mob Initiative
    s32 visibleMobs          = ls_bufferReadDWord(buf);
    Page->Mobs.selectedIndex = visibleMobs;
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        {
            ls_bufferReadIntoUTF32(buf, &f->editFields[j].text);
        }
        
        //NOTE: For the EXTRA editField, because it is multi-line 
        //      we need to count how many lines there are, and set it.
        if(f->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(f->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            f->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &f->maxLife.text);
        
        f->compendiumIdx = ls_bufferReadDWord(buf);
        f->ID            = ls_bufferReadDWord(buf);
    }
    
    
    //NOTE: UnSerialize Ally Initiative
    s32 visibleAllies          = ls_bufferReadDWord(buf);
    Page->Allies.selectedIndex = visibleAllies;
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        {
            ls_bufferReadIntoUTF32(buf, &f->editFields[j].text);
        }
        
        //NOTE: For the EXTRA editField, because it is multi-line 
        //      we need to count how many lines there are, and set it.
        if(f->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(f->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            f->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &f->maxLife.text);
        
        f->compendiumIdx = ls_bufferReadDWord(buf);
        f->ID            = ls_bufferReadDWord(buf);
    }
    
    //NOTE: Quick Exit if not in battle after the save.
    if(State.inBattle == FALSE) 
    { 
        ls_bufferDestroy(buf);
        //ls_arenaUse(globalArena);
        //ls_arenaClear(saveArena);
        return TRUE;
    }
    
    //NOTE: Set all Init Fields to ReadOnly because we are in battle
    for(u32 i = 0; i < unserializePartyNum; i++)
    { Page->PlayerInit[i].isReadonly = TRUE; }
    
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { f->editFields[j].isReadonly = TRUE; }
    }
    
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { f->editFields[j].isReadonly = TRUE; }
    }
    
    //NOTE: UnSerialize Order
    Page->orderAdjust = ls_bufferReadDWord(buf);
    s32 visibleOrder  = visibleMobs + visibleAllies + party_count - Page->orderAdjust;
    for(u32 i = 0; i < visibleOrder; i++)
    {
        Order *f = Page->OrderFields + i;
        
        ls_bufferReadIntoUTF32(buf, &f->field.text);
        
        f->pos.isReadonly = TRUE;
        
        //HP Slider
        f->field.maxValue = ls_bufferReadDWord(buf);
        f->field.minValue = ls_bufferReadDWord(buf);
        f->field.currPos  = ls_bufferReadDouble(buf);
        
        //Status Conditions, we store the check isActive status
        for(s32 statusIdx = 0; statusIdx < STATUS_COUNT; statusIdx++)
        { f->status[statusIdx].check.isActive = ls_bufferReadDWord(buf); }
        
        f->compendiumIdx  = ls_bufferReadDWord(buf);
        f->ID             = ls_bufferReadDWord(buf);
        
        s32 currVal = ls_uiSliderGetValue(c, &f->field);
        
        if(currVal == 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xFF, 0x97, 0x12, 0x99), c->widgetColor); }
        
        else if(currVal < 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xDD, 0x10, 0x20, 0x99), c->widgetColor); }
        
        else if(currVal > 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xF0, 0xFF, 0x3D, 0x99), c->widgetColor); }
    }
    
    Page->turnsInRound = ls_bufferReadDWord(buf);
    
    //NOTE: Current In Battle
    {
        Page->currIdx = ls_bufferReadDWord(buf);
        
        ls_bufferReadIntoUTF32(buf, &Page->Current.text);
    }
    
    
    //NOTE: Round Counter
    {
        Page->roundCount = ls_bufferReadDWord(buf);
        ls_bufferReadIntoUTF32(buf, &Page->RoundCounter.text);
    }
    
    //NOTE: Counters
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *C = Page->Counters + i;
        
        ls_bufferReadIntoUTF32(buf, &C->name.text);
        
        ls_bufferReadIntoUTF32(buf, &C->rounds.text);
        
        C->roundsLeft      = ls_bufferReadDWord(buf);
        C->isActive        = ls_bufferReadDWord(buf);
        C->turnCounter     = ls_bufferReadDWord(buf);
        C->startIdxInOrder = ls_bufferReadDWord(buf);
    }
    
    
    //NOTE: Throwers
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrowBox *f = Page->Throwers + i;
        
        ls_bufferReadIntoUTF32(buf, &f->name.text);
        ls_bufferReadIntoUTF32(buf, &f->toHit.text);
        ls_bufferReadIntoUTF32(buf, &f->hitRes.text);
        ls_bufferReadIntoUTF32(buf, &f->damage.text);
        ls_bufferReadIntoUTF32(buf, &f->dmgRes.text);
    }
    
    ls_bufferDestroy(buf);
    
    //ls_arenaUse(globalArena);
    //ls_arenaClear(saveArena);
    
    return TRUE;
}
