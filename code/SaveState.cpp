void SaveState()
{
    buffer state = ls_bufferInit(MBytes(1));
    buffer *buf = &state;
    
    ls_bufferAddDWord(buf, global_saveVersion);
    ls_bufferAddDWord(buf, State.inBattle);
    ls_bufferAddDWord(buf, State.encounters.numEncounters);
    
    InitPage *Page = State.Init;
    
    //NOTE Serialize Saved Encounters
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    {
        Encounter *curr = &State.encounters.Enc[i];
        
        ls_bufferAddUnistring(buf, curr->name);
        ls_bufferAddDWord(buf, curr->numMobs);
        
        for(u32 j = 0; j < curr->numMobs; j++)
        {
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { ls_bufferAddUnistring(buf, curr->mob[j][k]); }
        }
        
        ls_bufferAddDWord(buf, curr->numAllies);
        for(u32 j = 0; j < curr->numAllies; j++)
        {
            ls_bufferAddUnistring(buf, curr->allyName[j]);
            ls_bufferAddUnistring(buf, curr->allyBonus[j]);
            ls_bufferAddUnistring(buf, curr->allyFinal[j]);
        }
        
        for(u32 j = 0; j < THROWER_NUM; j++)
        {
            ls_bufferAddUnistring(buf, curr->throwerName[j]);
            ls_bufferAddUnistring(buf, curr->throwerHit[j]);
            ls_bufferAddUnistring(buf, curr->throwerDamage[j]);
        }
        
    }
    
    
    s32 visibleMobs   = Page->Mobs.selectedIndex;
    s32 visibleAllies = Page->Allies.selectedIndex;
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    
    //NOTE: Serialize Player Initiative
    for(u32 i = 0; i < PARTY_NUM; i++)
    { ls_bufferAddUnistring(buf, Page->PlayerInit[i].text); }
    
    
    //NOTE: Serialize Mob Initiative
    ls_bufferAddDWord(buf, visibleMobs);
    for(u32 i = 0; i < visibleMobs; i++)
    {
        InitField *f = Page->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_bufferAddUnistring(buf, f->editFields[j].text); }
        
        ls_bufferAddUnistring(buf, f->maxLife.text);
        
        ls_bufferAddDWord(buf, f->ID);
    }
    
    
    //NOTE: Serialize Ally Initiative
    ls_bufferAddDWord(buf, visibleAllies);
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        
        ls_bufferAddUnistring(buf, f->editFields[IF_IDX_NAME].text);
        ls_bufferAddUnistring(buf, f->editFields[IF_IDX_BONUS].text);
        ls_bufferAddUnistring(buf, f->editFields[IF_IDX_FINAL].text);
        
        ls_bufferAddDWord(buf, f->ID);
    }
    
    
    //NOTE: Serialize Order
    ls_bufferAddDWord(buf, Page->orderAdjust);
    for(u32 i = 0; i < visibleOrder; i++)
    {
        Order *f = Page->OrderFields + i;
        
        ls_bufferAddUnistring(buf, f->field.text);
        
        ls_bufferAddDWord(buf, f->field.maxValue);
        ls_bufferAddDWord(buf, f->field.minValue);
        ls_bufferAddDWord(buf, f->field.currPos);
        
        ls_bufferAddDWord(buf, f->ID);
    }
    
    ls_bufferAddDWord(buf, Page->turnsInRound);
    
    
    //NOTE: Current In Battle
    {
        ls_bufferAddDWord(buf, Page->currIdx);
        ls_bufferAddUnistring(buf, Page->Current.text);
    }
    
    
    //NOTE: Round Counter
    {
        ls_bufferAddDWord(buf, Page->roundCount);
        ls_bufferAddUnistring(buf, Page->RoundCounter.text);
    }
    
    
    //NOTE: Counters
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *C = Page->Counters + i;
        
        ls_bufferAddUnistring(buf, C->name.text);
        ls_bufferAddUnistring(buf, C->rounds.text);
        
        ls_bufferAddDWord(buf, C->roundsLeft);
        ls_bufferAddDWord(buf, C->isActive);
        ls_bufferAddDWord(buf, C->turnCounter);
        ls_bufferAddDWord(buf, C->startIdxInOrder);
    }
    
    
    //NOTE: Throwers
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrow *f = Page->Throwers + i;
        ls_bufferAddUnistring(buf, f->name.text);
        ls_bufferAddUnistring(buf, f->toHit.text);
        ls_bufferAddUnistring(buf, f->hitRes.text);
        ls_bufferAddUnistring(buf, f->damage.text);
        ls_bufferAddUnistring(buf, f->dmgRes.text);
    }
    
    char outName[64] = {};
    ls_sprintf(outName, "SaveFile_v%d", global_saveVersion);
    ls_writeFile(outName, (char *)buf->data, buf->size, FALSE);
    
    ls_bufferDestroy(buf);
    
    return;
}

b32 LoadState(UIContext *cxt)
{
    //TODO: Check for previous file versions and convert them to newer version!
    char fullPathBuff[128] = {};
    char outName[64] = {};
    ls_sprintf(outName, "SaveFile_v%d", global_saveVersion);
    
    u32 len = ls_getFullPathName(outName, fullPathBuff, 128);
    
    if(ls_fileExists(fullPathBuff) == FALSE) { return FALSE; }
    
    
    string path = ls_strConstant(fullPathBuff);
    buffer state = ls_bufferInitFromFile(path);
    buffer *buf = &state;
    
    
    u32 fileVersion = ls_bufferReadDWord(buf);
    if(fileVersion != global_saveVersion) { ls_bufferDestroy(buf); return FALSE; }
    
    State.inBattle                 = ls_bufferReadDWord(buf);
    State.encounters.numEncounters = ls_bufferReadDWord(buf);
    
    InitPage *Page = State.Init;
    
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    {
        Encounter *curr = State.encounters.Enc + i;
        
        curr->name = ls_bufferReadUnistring(buf);
        
        curr->numMobs = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numMobs; j++)
        {
            for(u32 k = 0; k < MOB_INIT_ENC_FIELDS; k++)
            { curr->mob[j][k] = ls_bufferReadUnistring(buf); }
        }
        
        curr->numAllies = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numAllies; j++)
        {
            curr->allyName[j]  = ls_bufferReadUnistring(buf);
            curr->allyBonus[j] = ls_bufferReadUnistring(buf);
            curr->allyFinal[j] = ls_bufferReadUnistring(buf);
        }
        
        for(u32 j = 0; j < THROWER_NUM; j++)
        {
            curr->throwerName[j]   = ls_bufferReadUnistring(buf);
            curr->throwerHit[j]    = ls_bufferReadUnistring(buf);
            curr->throwerDamage[j] = ls_bufferReadUnistring(buf);
        }
        
        ls_uiListBoxAddEntry(cxt, &Page->EncounterSel, curr->name);
    }
    
    //NOTE: Quick Exit if not in battle after the save.
    if(State.inBattle == FALSE) { return TRUE; }
    
    
    //NOTE: UnSerialize Player Initiative
    for(u32 i = 0; i < PARTY_NUM; i++)
    {
        ls_bufferReadIntoUnistring(buf, &Page->PlayerInit[i].text);
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
            ls_bufferReadIntoUnistring(buf, &f->editFields[j].text);
            f->editFields[j].viewEndIdx = f->editFields[j].text.len;
        }
        
        ls_bufferReadIntoUnistring(buf, &f->maxLife.text);
        f->maxLife.viewEndIdx = f->maxLife.text.len;
        
        f->ID = ls_bufferReadDWord(buf);
    }
    
    
    //NOTE: UnSerialize Ally Initiative
    s32 visibleAllies          = ls_bufferReadDWord(buf);
    Page->Allies.selectedIndex = visibleAllies;
    for(u32 i = 0; i < visibleAllies; i++)
    {
        InitField *f = Page->AllyFields + i;
        
        ls_bufferReadIntoUnistring(buf, &f->editFields[IF_IDX_NAME].text);
        f->editFields[IF_IDX_NAME].viewEndIdx = f->editFields[IF_IDX_NAME].text.len;
        
        ls_bufferReadIntoUnistring(buf, &f->editFields[IF_IDX_BONUS].text);
        f->editFields[IF_IDX_BONUS].viewEndIdx = f->editFields[IF_IDX_BONUS].text.len;
        
        ls_bufferReadIntoUnistring(buf, &f->editFields[IF_IDX_FINAL].text);
        f->editFields[IF_IDX_FINAL].viewEndIdx = f->editFields[IF_IDX_FINAL].text.len;
        
        f->ID = ls_bufferReadDWord(buf);
    }
    
    
    //NOTE: UnSerialize Order
    Page->orderAdjust = ls_bufferReadDWord(buf);
    s32 visibleOrder  = visibleMobs + visibleAllies + PARTY_NUM - Page->orderAdjust;
    for(u32 i = 0; i < visibleOrder; i++)
    {
        Order *f = Page->OrderFields + i;
        
        ls_bufferReadIntoUnistring(buf, &f->field.text);
        
        f->field.maxValue = ls_bufferReadDWord(buf);
        f->field.minValue = ls_bufferReadDWord(buf);
        f->field.currPos  = ls_bufferReadDWord(buf);
        
        f->ID             = ls_bufferReadDWord(buf);
    }
    
    Page->turnsInRound = ls_bufferReadDWord(buf);
    
    
    //NOTE: Current In Battle
    {
        Page->currIdx = ls_bufferReadDWord(buf);
        
        ls_bufferReadIntoUnistring(buf, &Page->Current.text);
        Page->Current.viewEndIdx = Page->Current.text.len;
    }
    
    
    //NOTE: Round Counter
    {
        Page->roundCount = ls_bufferReadDWord(buf);
        ls_bufferReadIntoUnistring(buf, &Page->RoundCounter.text);
        Page->RoundCounter.viewEndIdx = Page->RoundCounter.text.len;
    }
    
    
    //NOTE: Counters
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *C = Page->Counters + i;
        
        ls_bufferReadIntoUnistring(buf, &C->name.text);
        C->name.viewEndIdx = C->name.text.len;
        
        ls_bufferReadIntoUnistring(buf, &C->rounds.text);
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
        
        ls_bufferReadIntoUnistring(buf, &f->name.text);   f->name.viewEndIdx   = f->name.text.len;
        ls_bufferReadIntoUnistring(buf, &f->toHit.text);  f->toHit.viewEndIdx  = f->toHit.text.len;
        ls_bufferReadIntoUnistring(buf, &f->hitRes.text); f->hitRes.viewEndIdx = f->hitRes.text.len;
        ls_bufferReadIntoUnistring(buf, &f->damage.text); f->damage.viewEndIdx = f->damage.text.len;
        ls_bufferReadIntoUnistring(buf, &f->dmgRes.text); f->dmgRes.viewEndIdx = f->dmgRes.text.len;
    }
    
    ls_bufferDestroy(buf);
    
    return TRUE;
}
