void CopyStateToBuffer(ProgramState *curr, buffer *buf, u32 saveV = global_saveVersion)
{
    //NOTE: Copy Init Page
    InitPage *init = curr->Init;
    
    //4
    ls_bufferAddDWord(buf, init->Mobs.selectedIndex);
    //8
    ls_bufferAddDWord(buf, init->Allies.selectedIndex);
    
    //8+(4*PARTY_NUM)
    for(u32 i = 0; i < PARTY_NUM; i++)
    { ls_bufferAddUTF32(buf, init->PlayerInit[i].text); }
    
    //20 + [(4*IDX_COUNT) + 20]*ALLY_NUM = 20 + (64*ALLY_NUM) = 532
    for(u32 i = 0; i < ALLY_NUM; i++)
    {
        InitField *ally = init->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_bufferAddUTF32(buf, ally->editFields[j].text); }
        
        ls_bufferAddUTF32(buf, ally->maxLife.text);
        ls_bufferAddDWord(buf, ally->compendiumIdx);
        ls_bufferAddDWord(buf, ally->ID);
    }
    
    //532 + (64*MOB_NUM) = 2.068
    for(u32 i = 0; i < MOB_NUM; i++)
    {
        InitField *mob = init->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_bufferAddUTF32(buf, mob->editFields[j].text); }
        
        ls_bufferAddUTF32(buf, mob->maxLife.text);
        ls_bufferAddDWord(buf, mob->compendiumIdx);
        ls_bufferAddDWord(buf, mob->ID);
    }
    
    //2.068 + (36*ORDER_NUM) = 3.328
    for(u32 i = 0; i < ORDER_NUM; i++)
    {
        Order *order = init->OrderFields + i;
        
        ls_bufferAddUTF32(buf, order->field.text);
        ls_bufferAddDWord(buf, order->field.currValue);
        ls_bufferAddDWord(buf, order->field.maxValue);
        ls_bufferAddDWord(buf, order->field.minValue);
        ls_bufferAddDouble(buf, order->field.currPos);
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
        DiceThrow *thrower = init->Throwers + i;
        
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

void CopyStateFromBuffer(ProgramState *curr, buffer *buf, u32 saveV = global_saveVersion)
{
    //NOTE: Copy Init Page
    InitPage *init = curr->Init;
    
    init->Mobs.selectedIndex   = (s32)ls_bufferReadDWord(buf);
    init->Allies.selectedIndex = (s32)ls_bufferReadDWord(buf);
    
    for(u32 i = 0; i < PARTY_NUM; i++)
    { ls_bufferReadIntoUTF32(buf, &init->PlayerInit[i].text); }
    
    for(u32 i = 0; i < ALLY_NUM; i++)
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
    
    for(u32 i = 0; i < MOB_NUM; i++)
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
    
    for(u32 i = 0; i < ORDER_NUM; i++)
    {
        Order *order = init->OrderFields + i;
        
        ls_bufferReadIntoUTF32(buf, &order->field.text);
        order->field.currValue = ls_bufferReadDWord(buf);
        order->field.maxValue  = ls_bufferReadDWord(buf);
        order->field.minValue  = ls_bufferReadDWord(buf);
        order->field.currPos   = ls_bufferReadDouble(buf);
        
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
        DiceThrow *thrower = init->Throwers + i;
        
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

//NOTETODO: Easier to add ls_buffer utilities to reserve/remove blocks of data inside the buffer
//          at certain indeces and just fill those up accordingly.
buffer ConvertSaveToV4(buffer *buff)
{
    buffer V4 = ls_bufferInit(buff->size);
    buffer *out = &V4;
    
    u32 fileVersion = ls_bufferReadDWord(buff);
    AssertMsg(fileVersion == 3, "How did we get here? Only v3 saves should arrive here.\n");
    
    ls_bufferAddDWord(out, 4); //NOTE: New fileVersion is 4!!
    
    //NOTE: Setup Constants!
    const u32 MOB_INIT_ENC_FIELDS_V3 = 12;
    const u32 MOB_INIT_ENC_FIELDS_V4 = 12;
    
    const u32 IF_IDX_COUNT_V3 = 11;
    const u32 IF_IDX_COUNT_V4 = 11;
    
    const u32 PARTY_NUM_V3 = 3;
    const u32 PARTY_NUM_V4 = 3;
    
    const u32 COUNTER_NUM_V3    = 9;
    const u32 COUNTER_NUM_V4    = 9;
    
    const u32 THROWER_NUM_V3    = 8;
    const u32 THROWER_NUM_V4    = 8;
    
    const u32 MAX_UNDO_STATES_V4 = 32;
    
    ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //State.inBattle
    
    u32 numEncounters = ls_bufferReadDWord(buff);
    ls_bufferAddDWord(out, numEncounters);
    
    //NOTE: Encounters
    for(u32 i = 0; i < numEncounters; i++)
    {
        utf32 name = ls_bufferReadUTF32(buff);
        ls_bufferAddUTF32(out, name);
        ls_utf32Free(&name);
        
        u32 numMobs = ls_bufferReadDWord(buff);
        ls_bufferAddDWord(out, numMobs);
        
        for(u32 j = 0; j < numMobs; j++)
        {
            
            //NOTE: Discrepancy in non-variable data has to be taken into account.
            u32 encounterFields = MOB_INIT_ENC_FIELDS_V3;
            if(encounterFields > MOB_INIT_ENC_FIELDS_V4) { encounterFields = MOB_INIT_ENC_FIELDS_V4; }
            
            for(u32 k = 0; k < encounterFields; k++) 
            { 
                utf32 mobField = ls_bufferReadUTF32(buff);
                ls_bufferAddUTF32(out, mobField); 
                ls_utf32Free(&mobField);
            }
            
            // NOTE: Make sure the newer version has all data properly padded if older is missing it.
            if(MOB_INIT_ENC_FIELDS_V3 < MOB_INIT_ENC_FIELDS_V4)
            {
                u32 diff = MOB_INIT_ENC_FIELDS_V4 - MOB_INIT_ENC_FIELDS_V3;
                for(u32 k = 0; k < diff; k++) 
                { ls_bufferAddUTF32(out, {NULL, 0, 0}); }
                
            }
        }
        
        u32 numAllies = ls_bufferReadDWord(buff);
        ls_bufferAddDWord(out, numAllies);
        
        for(u32 j = 0; j < numAllies; j++)
        {
            utf32 name  = ls_bufferReadUTF32(buff);
            utf32 bonus = ls_bufferReadUTF32(buff);
            utf32 final = ls_bufferReadUTF32(buff);
            
            ls_bufferAddUTF32(out, name);
            ls_bufferAddUTF32(out, bonus);
            ls_bufferAddUTF32(out, final);
            
            ls_utf32Free(&name);
            ls_utf32Free(&bonus);
            ls_utf32Free(&final);
        }
        
        //NOTE: Discrepancy in non-variable data has to be taken into account.
        u32 throwerNum = THROWER_NUM_V3;
        if(throwerNum > THROWER_NUM_V4) { throwerNum = THROWER_NUM_V4; }
        
        for(u32 j = 0; j < throwerNum; j++)
        {
            utf32 name  = ls_bufferReadUTF32(buff);
            utf32 hit   = ls_bufferReadUTF32(buff);
            utf32 dmg   = ls_bufferReadUTF32(buff);
            
            ls_bufferAddUTF32(out, name);
            ls_bufferAddUTF32(out, hit);
            ls_bufferAddUTF32(out, dmg);
            
            ls_utf32Free(&name);
            ls_utf32Free(&hit);
            ls_utf32Free(&dmg);
        }
        
        // NOTE: Make sure the newer version has all data properly padded if older is missing it.
        if(THROWER_NUM_V3 < THROWER_NUM_V4)
        {
            u32 diff = THROWER_NUM_V4 - THROWER_NUM_V3;
            for(u32 k = 0; k < diff; k++) 
            { 
                ls_bufferAddUTF32(out, {NULL, 0, 0});
                ls_bufferAddUTF32(out, {NULL, 0, 0});
                ls_bufferAddUTF32(out, {NULL, 0, 0});
            }
            
        }
    }
    
    // ------------------ //
    //   V4 UNIQUE DATA   //
    
    //NOTE: Undo Chain
    {
        const u32 MIN_SIZE_UNDO_STATE_V4 = 3756;
        
        ls_bufferAddDWord(out, MAX_UNDO_STATES_V4);
        ls_bufferAddDWord(out, 0);  // matchingUndoIdx
        ls_bufferAddDWord(out, 0);  // distanceFromOld
        ls_bufferAddDWord(out, 0);  // distanceFromNow
        
        for(u32 i = 0; i < MAX_UNDO_STATES_V4; i++)
        { ls_bufferZeroPad(out, MIN_SIZE_UNDO_STATE_V4); }
    }
    
    //   V4 UNIQUE DATA   //
    // ------------------ //
    
    //NOTE: This time there's not discrepancy in 
    //          PARTY_NUM_V3 vs PARTY_NUM_V4, so we don't need to do checks
    
    //NOTE: Player Initiative
    for(u32 i = 0; i < PARTY_NUM_V3; i++)
    {
        utf32 init = ls_bufferReadUTF32(buff);
        ls_bufferAddUTF32(out, init);
        ls_utf32Free(&init);
    }
    
    //NOTE: Mob Initiative
    s32 visibleMobs = ls_bufferReadDWord(buff);
    ls_bufferAddDWord(out, visibleMobs);
    
    for(u32 i = 0; i < visibleMobs; i++)
    {
        
        //NOTE: Discrepancy in non-variable data has to be taken into account.
        u32 encounterFields = IF_IDX_COUNT_V3;
        if(encounterFields > IF_IDX_COUNT_V4) { encounterFields = IF_IDX_COUNT_V4; }
        
        for(u32 k = 0; k < encounterFields; k++) 
        { 
            utf32 mobField = ls_bufferReadUTF32(buff);
            ls_bufferAddUTF32(out, mobField); 
            ls_utf32Free(&mobField);
        }
        
        // NOTE: Make sure the newer version has all data properly padded if older is missing it.
        if(IF_IDX_COUNT_V3 < IF_IDX_COUNT_V4)
        {
            u32 diff = IF_IDX_COUNT_V4 - IF_IDX_COUNT_V3;
            for(u32 k = 0; k < diff; k++) 
            { ls_bufferAddUTF32(out, {NULL, 0, 0}); }
            
        }
        
        utf32 maxLife = ls_bufferReadUTF32(buff);
        ls_bufferAddUTF32(out, maxLife);
        ls_utf32Free(&maxLife);
        
        ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //f->ID
    }
    
    
    //NOTE: Ally Initiative
    s32 visibleAllies = ls_bufferReadDWord(buff);
    ls_bufferAddDWord(out, visibleAllies);
    
    for(u32 i = 0; i < visibleAllies; i++)
    {
        utf32 name  = ls_bufferReadUTF32(buff);
        utf32 bonus = ls_bufferReadUTF32(buff);
        utf32 final = ls_bufferReadUTF32(buff);
        
        ls_bufferAddUTF32(out, name);
        ls_bufferAddUTF32(out, bonus);
        ls_bufferAddUTF32(out, final);
        
        ls_utf32Free(&name);
        ls_utf32Free(&bonus);
        ls_utf32Free(&final);
        
        ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //f->ID
    }
    
    //NOTE: Order
    
    u32 orderAdjust = ls_bufferReadDWord(buff);
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM_V3 - orderAdjust;
    for(u32 i = 0; i < visibleOrder; i++)
    {
        
        utf32 name = ls_bufferReadUTF32(buff);
        ls_bufferAddUTF32(out, name);
        ls_utf32Free(&name);
        
        ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //f->field.maxValue
        ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //f->field.minValue
        
        //NOTE: In V3 it was saved as float, but it really should be a Double!!
        f64 currPos = (f64)ls_bufferReadFloat(buff);
        ls_bufferAddDouble(out, currPos); //f->field.currPos
        
        ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //f->ID
    }
    
    
    ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //Page->turnsInRound
    
    
    //NOTE: Current In Battle
    {
        ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //Page->currIdx
        
        utf32 current = ls_bufferReadUTF32(buff);
        ls_bufferAddUTF32(out, current);
        ls_utf32Free(&current);
    }
    
    
    //NOTE: Round Counter
    {
        ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //Page->roundCount
        
        utf32 counter = ls_bufferReadUTF32(buff);
        ls_bufferAddUTF32(out, counter);
        ls_utf32Free(&counter);
    }
    
    //NOTE: There's no difference between COUNTER_NUM_V3 and COUNTER_NUM_V4
    //NOTE: Counters
    for(u32 i = 0; i < COUNTER_NUM_V3; i++)
    {
        utf32 name = ls_bufferReadUTF32(buff);
        ls_bufferAddUTF32(out, name);
        ls_utf32Free(&name);
        
        utf32 rounds = ls_bufferReadUTF32(buff);
        ls_bufferAddUTF32(out, rounds);
        ls_utf32Free(&rounds);
        
        
        ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //C->roundsLeft
        ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //C->isActive
        ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //C->turnCounter
        ls_bufferAddDWord(out, ls_bufferReadDWord(buff)); //C->startIdxInOrder
    }
    
    //NOTE: There's not difference between THROWER_NUM_V3 and THROWER_NUM_V4
    //NOTE: Throwers
    for(u32 i = 0; i < THROWER_NUM_V3; i++)
    {
        utf32 name   = ls_bufferReadUTF32(buff);
        utf32 toHit  = ls_bufferReadUTF32(buff);
        utf32 hitRes = ls_bufferReadUTF32(buff);
        utf32 damage = ls_bufferReadUTF32(buff);
        utf32 dmgRes = ls_bufferReadUTF32(buff);
        
        ls_bufferAddUTF32(out, name);
        ls_bufferAddUTF32(out, toHit);
        ls_bufferAddUTF32(out, hitRes);
        ls_bufferAddUTF32(out, damage);
        ls_bufferAddUTF32(out, dmgRes);
        
        ls_utf32Free(&name);
        ls_utf32Free(&toHit);
        ls_utf32Free(&hitRes);
        ls_utf32Free(&damage);
        ls_utf32Free(&dmgRes);
    }
    
    ls_bufferDestroy(buff);
    
    return V4;
}

buffer ConvertSaveToNewVersion(buffer *oldSave, u32 oldVersion)
{
    buffer currentSaveBuffer = *oldSave;
    
    for(u32 i = oldVersion; i < global_saveVersion; i++)
    {
        switch(i)
        {
            case 3: 
            {
                buffer v4Buff = ConvertSaveToV4(&currentSaveBuffer);
                currentSaveBuffer = v4Buff;
            } break;
            
            default: 
            {
                AssertMsg(FALSE, "No Available Conversion Function\n"); break;
            }
        }
    }
    
    return currentSaveBuffer;
}

void SaveState(UIContext *c)
{
    (void)c;
    
    ls_arenaUse(saveArena);
    
    buffer state = ls_bufferInit(MBytes(1));
    buffer *buf = &state;
    
    ls_bufferAddDWord(buf, global_saveVersion);
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
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    
    //NOTE: Serialize Player Initiative
    ls_bufferAddDWord(buf, PARTY_NUM);
    
    for(u32 i = 0; i < PARTY_NUM; i++)
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
        
        ls_bufferAddDWord(buf,  f->field.maxValue);
        ls_bufferAddDWord(buf,  f->field.minValue);
        ls_bufferAddDouble(buf, f->field.currPos);
        
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
        DiceThrow *f = Page->Throwers + i;
        ls_bufferAddUTF32(buf, f->name.text);
        ls_bufferAddUTF32(buf, f->toHit.text);
        ls_bufferAddUTF32(buf, f->hitRes.text);
        ls_bufferAddUTF32(buf, f->damage.text);
        ls_bufferAddUTF32(buf, f->dmgRes.text);
    }
    
    //TODO: Actually use buf->cursor instead of buf->size??
    char outName[64] = {};
    ls_sprintf(outName, 64, "SaveFile_v%d", global_saveVersion);
    ls_writeFile(outName, (char *)buf->data, buf->size, FALSE);
    
    ls_bufferDestroy(buf);
    
    ls_arenaUse(globalArena);
    ls_arenaClear(saveArena);
    
    return;
}

b32 LoadState(UIContext *cxt)
{
    ls_arenaUse(saveArena);
    
    char fullPathBuff[128] = {};
    char outName[64] = {};
    ls_sprintf(outName, 64, "SaveFile_v%d", global_saveVersion);
    
    u32 len = ls_getFullPathName(outName, fullPathBuff, 128);
    
    buffer state = {};
    buffer *buf = NULL;
    
    if(ls_fileExists(fullPathBuff) == FALSE) { return FALSE; }
    else
    {
        string path = ls_strConstant(fullPathBuff);
        state = ls_bufferInitFromFile(path);
        buf = &state;
    }
    
    ls_bufferSeekBegin(buf);
    
    u32 fileVersion = ls_bufferReadDWord(buf);
    if(fileVersion != global_saveVersion) { ls_bufferDestroy(buf); return FALSE; }
    
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
        
        ls_uiListBoxAddEntry(cxt, &Page->EncounterSel, curr->name);
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
    u32 partyNum = ls_bufferReadDWord(buf);
    u32 unserializePartyNum = partyNum < PARTY_NUM ? partyNum : PARTY_NUM;
    for(u32 i = 0; i < unserializePartyNum; i++)
    {
        ls_bufferReadIntoUTF32(buf, &Page->PlayerInit[i].text);
        Page->PlayerInit[i].viewEndIdx = Page->PlayerInit[i].text.len;
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
            f->editFields[j].viewEndIdx = f->editFields[j].text.len;
        }
        
        //NOTE: For the EXTRA editField, because it is multi-line 
        //      we need to count how many lines there are, and set it.
        if(f->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(f->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            f->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &f->maxLife.text);
        f->maxLife.viewEndIdx = f->maxLife.text.len;
        
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
            f->editFields[j].viewEndIdx = f->editFields[j].text.len;
        }
        
        //NOTE: For the EXTRA editField, because it is multi-line 
        //      we need to count how many lines there are, and set it.
        if(f->editFields[IF_IDX_EXTRA].text.len > 0)
        {
            s32 newlines = ls_utf32CountOccurrences(f->editFields[IF_IDX_EXTRA].text, (u32)'\n');
            f->editFields[IF_IDX_EXTRA].lineCount = newlines + 1;
        }
        
        ls_bufferReadIntoUTF32(buf, &f->maxLife.text);
        f->maxLife.viewEndIdx = f->maxLife.text.len;
        
        f->compendiumIdx = ls_bufferReadDWord(buf);
        f->ID            = ls_bufferReadDWord(buf);
    }
    
    //NOTE: Quick Exit if not in battle after the save.
    if(State.inBattle == FALSE) 
    { 
        ls_arenaUse(globalArena);
        ls_arenaClear(saveArena);
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
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    for(u32 i = 0; i < visibleOrder; i++)
    {
        Order *f = Page->OrderFields + i;
        
        ls_bufferReadIntoUTF32(buf, &f->field.text);
        
        f->pos.isReadonly = TRUE;
        
        f->field.maxValue = ls_bufferReadDWord(buf);
        f->field.minValue = ls_bufferReadDWord(buf);
        f->field.currPos  = ls_bufferReadDouble(buf);
        
        f->compendiumIdx  = ls_bufferReadDWord(buf);
        f->ID             = ls_bufferReadDWord(buf);
        
        s32 currVal = ls_uiSliderGetValue(cxt, &f->field);
        
        if(currVal == 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xFF, 0x97, 0x12, 0x99), cxt->widgetColor); }
        
        else if(currVal < 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xDD, 0x10, 0x20, 0x99), cxt->widgetColor); }
        
        else if(currVal > 0)
        { f->field.rColor = ls_uiAlphaBlend(RGBA(0xF0, 0xFF, 0x3D, 0x99), cxt->widgetColor); }
    }
    
    Page->turnsInRound = ls_bufferReadDWord(buf);
    
    //NOTE: Current In Battle
    {
        Page->currIdx = ls_bufferReadDWord(buf);
        
        ls_bufferReadIntoUTF32(buf, &Page->Current.text);
        Page->Current.viewEndIdx = Page->Current.text.len;
    }
    
    
    //NOTE: Round Counter
    {
        Page->roundCount = ls_bufferReadDWord(buf);
        ls_bufferReadIntoUTF32(buf, &Page->RoundCounter.text);
        Page->RoundCounter.viewEndIdx = Page->RoundCounter.text.len;
    }
    
    //NOTE: Counters
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *C = Page->Counters + i;
        
        ls_bufferReadIntoUTF32(buf, &C->name.text);
        C->name.viewEndIdx = C->name.text.len;
        
        ls_bufferReadIntoUTF32(buf, &C->rounds.text);
        C->rounds.viewEndIdx = C->rounds.text.len;
        
        C->roundsLeft      = ls_bufferReadDWord(buf);
        C->isActive        = ls_bufferReadDWord(buf);
        C->turnCounter     = ls_bufferReadDWord(buf);
        C->startIdxInOrder = ls_bufferReadDWord(buf);
    }
    
    
    //NOTE: Throwers
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrow *f = Page->Throwers + i;
        
        ls_bufferReadIntoUTF32(buf, &f->name.text);   f->name.viewEndIdx   = f->name.text.len;
        ls_bufferReadIntoUTF32(buf, &f->toHit.text);  f->toHit.viewEndIdx  = f->toHit.text.len;
        ls_bufferReadIntoUTF32(buf, &f->hitRes.text); f->hitRes.viewEndIdx = f->hitRes.text.len;
        ls_bufferReadIntoUTF32(buf, &f->damage.text); f->damage.viewEndIdx = f->damage.text.len;
        ls_bufferReadIntoUTF32(buf, &f->dmgRes.text); f->dmgRes.viewEndIdx = f->dmgRes.text.len;
    }
    
    ls_bufferDestroy(buf);
    
    ls_arenaUse(globalArena);
    ls_arenaClear(saveArena);
    
    return TRUE;
}
