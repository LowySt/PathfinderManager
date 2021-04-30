void SaveState()
{
    buffer state = ls_bufferInit(MBytes(1));
    buffer *buf = &state;
    
    ls_bufferAddDWord(buf, global_saveVersion);
    ls_bufferAddDWord(buf, State.inBattle);
    ls_bufferAddDWord(buf, State.encounters.numEncounters);
    
    InitPage *page = State.Init;
    
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    {
        Encounter *curr = &State.encounters.Enc[i];
        
        ls_bufferAddData(buf, curr->name, 32);
        ls_bufferAddDWord(buf, curr->numMobs);
        for(u32 j = 0; j < curr->numMobs; j++)
        {
            ls_bufferAddData(buf, curr->mobNames[j], 32);
            ls_bufferAddDWord(buf, curr->mobBonus[j]);
        }
        
        ls_bufferAddDWord(buf, curr->numAllies);
        for(u32 j = 0; j < curr->numAllies; j++)
        {
            ls_bufferAddData(buf, curr->allyNames[j], 32);
            ls_bufferAddDWord(buf, curr->allyBonus[j]);
        }
        
        for(u32 j = 0; j < THROWER_NUM; j++)
        {
            ls_bufferAddData(buf, curr->throwerNames[j], 64);
            ls_bufferAddData(buf, curr->throwerHit[j], 64);
            ls_bufferAddData(buf, curr->throwerDamage[j], 64);
        }
    }
    
    //NOTE: Serialize Player Initiative
    for(u32 i = 0; i < PARTY_NUM; i++)
    {
        char text[32] = {};
        s32 len = Edit_GetText(page->PlayerFields[i].Bonus->box, text, 32);
        ls_bufferAddData(buf, text, len);
    }
    
    //NOTE: Serialize Mob Initiative
    ls_bufferAddDWord(buf, page->VisibleMobs);
    for(u32 i = 0; i < page->VisibleMobs; i++)
    {
        char text[32] = {};
        
        s32 len = Edit_GetText(page->MobFields[i].Name->box, text, 32);
        ls_bufferAddData(buf, text, len);
        
        len = Edit_GetText(page->MobFields[i].Bonus->box, text, 32);
        ls_bufferAddData(buf, text, len);
        
        len = Edit_GetText(page->MobFields[i].Final->box, text, 32);
        ls_bufferAddData(buf, text, len);
    }
    
    
    //NOTE: Serialize Ally Initiative
    ls_bufferAddDWord(buf, page->VisibleAllies);
    for(u32 i = 0; i < page->VisibleAllies; i++)
    {
        char text[32] = {};
        
        s32 len = Edit_GetText(page->AllyFields[i].Name->box, text, 32);
        ls_bufferAddData(buf, text, len);
        
        len = Edit_GetText(page->AllyFields[i].Bonus->box, text, 32);
        ls_bufferAddData(buf, text, len);
        
        len = Edit_GetText(page->AllyFields[i].Final->box, text, 32);
        ls_bufferAddData(buf, text, len);
    }
    
    
    //NOTE: Serialize Order
    ls_bufferAddDWord(buf, page->VisibleOrder);
    for(u32 i = 0; i < page->VisibleOrder; i++)
    {
        char text[32] = {};
        
        s32 len = Edit_GetText(page->Order[i].Field->box, text, 32);
        ls_bufferAddData(buf, text, len);
        
        len = Edit_GetText(page->Order[i].Pos->box, text, 32);
        ls_bufferAddData(buf, text, len);
        
        ls_bufferAddDWord(buf, page->Order[i].fieldId);
        ls_bufferAddDWord(buf, page->Order[i].isMob);
        ls_bufferAddDWord(buf, page->Order[i].isParty);
    }
    
    ls_bufferAddDWord(buf, page->turnsInRound);
    
    //NOTE: Current In Battle
    {
        ls_bufferAddDWord(buf, page->currIdx);
        
        char text[32] = {};
        s32 len = Edit_GetText(page->Current->box, text, 32);
        ls_bufferAddData(buf, text, len);
    }
    
#if 0
    //NOTE: Current Encounter
    {
        char text[32] = {};
        s32 len = Edit_GetText(page->EncounterName->box, text, 32);
        ls_bufferAddData(buf, text, len);
    }
#endif
    
    //NOTE: Round Counter
    {
        char text[32] = {};
        s32 len = Edit_GetText(page->RoundCounter->box, text, 32);
        ls_bufferAddData(buf, text, len);
    }
    
    //NOTE: Counters
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *c = &page->Counters[i];
        
        ls_bufferAddDWord(buf, c->isActive);
        ls_bufferAddDWord(buf, c->roundCounter);
        
        char text[32] = {};
        
        s32 len = Edit_GetText(page->Counters[i].Field->box, text, 32);
        ls_bufferAddData(buf, text, len);
        
        len = Edit_GetText(page->Counters[i].Rounds->box, text, 32);
        ls_bufferAddData(buf, text, len);
    }
    
    ls_writeFile((char *)"testState", (char *)buf->data, buf->size, FALSE);
    
    return;
}

b32 LoadState()
{
    char fullPathBuff[128] = {};
    u32 len = ls_getFullPathName((char *)"testState", fullPathBuff, 128);
    
    if(ls_fileExists(fullPathBuff) == FALSE) { return FALSE; }
    
#if __GNUG__
    string filePath = {
        (char *)"testState",
        (sizeof("testState")/sizeof("testState"[0]))-1,
        (sizeof("testState")/sizeof("testState"[0]))-1
    };
#else
    string filePath = ls_strConst("testState");
#endif
    
    buffer state = ls_bufferInitFromFile(filePath);
    buffer *buf = &state;
    
    u32 fileVersion = ls_bufferReadDWord(buf);
    if(fileVersion != global_saveVersion) { return FALSE; }
    
    State.inBattle = ls_bufferReadDWord(buf);
    State.encounters.numEncounters = ls_bufferReadDWord(buf);
    
    InitPage *page = State.Init;
    
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    {
        Encounter *curr = &State.encounters.Enc[i];
        
        ls_bufferReadData(buf, curr->name);
        
        curr->numMobs = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numMobs; j++)
        {
            ls_bufferReadData(buf, curr->mobNames[j]);
            curr->mobBonus[j] = ls_bufferReadDWord(buf);
        }
        
        curr->numAllies = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numAllies; j++)
        {
            ls_bufferReadData(buf, curr->allyNames[j]);
            curr->allyBonus[j] = ls_bufferReadDWord(buf);
        }
        
        for(u32 j = 0; j < THROWER_NUM; j++)
        {
            ls_bufferReadData(buf, curr->throwerNames[j]);
            ls_bufferReadData(buf, curr->throwerHit[j]);
            ls_bufferReadData(buf, curr->throwerDamage[j]);
        }
        
        const u32 baseHeight = 20;
        u32 height = 0;
        if(State.encounters.numEncounters == 1) { height = baseHeight*3; }
        else { height = baseHeight*(State.encounters.numEncounters + 1); }
        SetWindowPos(State.Init->EncounterSel->box, NULL, NULL, NULL, 100, height, SWP_NOMOVE | SWP_NOZORDER);
        
        ComboBox_InsertString(State.Init->EncounterSel->box, -1, curr->name);
    }
    
    if(!State.inBattle) 
    { 
        ComboBox_SetCurSel(page->Mobs->box, 0);
        ComboBox_SetCurSel(page->Allies->box, 0);
        return FALSE; 
    }
    
    //NOTE: UnSerialize Player Initiative
    for(u32 i = 0; i < PARTY_NUM; i++)
    {
        char text[32] = {};
        ls_bufferReadData(buf, text);
        Edit_SetText(page->PlayerFields[i].Bonus->box, text);
    }
    
    
    //NOTE: UnSerialize Mob Initiative
    page->VisibleMobs = ls_bufferReadDWord(buf);
    for(u32 i = 0; i < page->VisibleMobs; i++)
    {
        char text1[32] = {};
        char text2[32] = {};
        char text3[32] = {};
        
        ls_bufferReadData(buf, text1);
        Edit_SetText(page->MobFields[i].Name->box, text1);
        
        ls_bufferReadData(buf, text2);
        Edit_SetText(page->MobFields[i].Bonus->box, text2);
        
        ls_bufferReadData(buf, text3);
        Edit_SetText(page->MobFields[i].Final->box, text3);
    }
    
    
    //NOTE: UnSerialize Ally Initiative
    page->VisibleAllies = ls_bufferReadDWord(buf);
    for(u32 i = 0; i < page->VisibleAllies; i++)
    {
        char text1[32] = {};
        char text2[32] = {};
        char text3[32] = {};
        
        ls_bufferReadData(buf, text1);
        Edit_SetText(page->AllyFields[i].Name->box, text1);
        
        ls_bufferReadData(buf, text2);
        Edit_SetText(page->AllyFields[i].Bonus->box, text2);
        
        ls_bufferReadData(buf, text3);
        Edit_SetText(page->AllyFields[i].Final->box, text3);
    }
    
    //NOTE: UnSerialize Order
    page->VisibleOrder = ls_bufferReadDWord(buf);
    for(u32 i = 0; i < page->VisibleOrder; i++)
    {
        char text1[32] = {};
        char text2[32] = {};
        
        ls_bufferReadData(buf, text1);
        Edit_SetText(page->Order[i].Field->box, text1);
        
        ls_bufferReadData(buf, text2);
        Edit_SetText(page->Order[i].Pos->box, text2);
        
        page->Order[i].fieldId = ls_bufferReadDWord(buf);
        page->Order[i].isMob   = ls_bufferReadDWord(buf);
        page->Order[i].isParty = ls_bufferReadDWord(buf);
    }
    
    page->turnsInRound = ls_bufferReadDWord(buf);
    
    
    //NOTE: Current In Battle
    {
        page->currIdx = ls_bufferReadDWord(buf);
        
        char text[32] = {};
        ls_bufferReadData(buf, text);
        Edit_SetText(page->Current->box, text);
    }
    
#if 0
    //NOTE: Current Encounter
    {
        char text[32] = {};
        ls_bufferReadData(buf, text);
        Edit_SetText(page->EncounterName->box, text);
    }
#endif
    
    //NOTE: Round Counter
    {
        char text[32] = {};
        ls_bufferReadData(buf, text);
        Edit_SetText(page->RoundCounter->box, text);
    }
    
    //NOTE: Counters
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *c = &page->Counters[i];
        
        c->isActive     = ls_bufferReadDWord(buf);
        c->roundCounter = ls_bufferReadDWord(buf);
        
        char text1[32] = {};
        char text2[32] = {};
        
        ls_bufferReadData(buf, text1);
        Edit_SetText(page->Counters[i].Field->box, text1);
        
        ls_bufferReadData(buf, text2);
        Edit_SetText(page->Counters[i].Rounds->box, text2);
    }
    
    
    for(u32 i = 0; i < PARTY_NUM; i++) {
        Edit_SetReadOnly(page->PlayerFields[i].Bonus->box, TRUE);
    }
    
    for(u32 i = 0; i < MOB_NUM; i++)  { 
        Edit_SetReadOnly(page->MobFields[i].Name->box, TRUE);
        Edit_SetReadOnly(page->MobFields[i].Bonus->box, TRUE);
        Edit_SetReadOnly(page->MobFields[i].Final->box, TRUE);
    }
    
    for(u32 i = 0; i < ALLY_NUM; i++) { 
        Edit_SetReadOnly(page->AllyFields[i].Name->box, TRUE);
        Edit_SetReadOnly(page->AllyFields[i].Bonus->box, TRUE);
        Edit_SetReadOnly(page->AllyFields[i].Final->box, TRUE);
    }
    
    ComboBox_SetCurSel(page->Mobs->box, page->VisibleMobs);
    HideInitField(page->MobFields, MOB_NUM);
    ShowInitField(page->MobFields, page->VisibleMobs, MOB_NUM);
    ShowInitFieldAdd(page->MobFields, page->VisibleMobs, MOB_NUM);
    
    ComboBox_SetCurSel(page->Allies->box, page->VisibleAllies);
    HideInitField(page->AllyFields, ALLY_NUM);
    ShowInitField(page->AllyFields, page->VisibleAllies, ALLY_NUM);
    ShowInitFieldAdd(page->AllyFields, page->VisibleAllies, ALLY_NUM);
    
    ShowOrder(page->Order, page->VisibleOrder);
    
    HideElem(page->EncounterSel->box); HideElem(page->EncounterSel->label);
    HideElem(page->EncounterName->box); HideElem(page->Save->box);
    
    HideElem(page->Set->box);    HideElem(page->Roll->box);
    HideElem(page->Mobs->box);   HideElem(page->Mobs->label);
    HideElem(page->Allies->box); HideElem(page->Allies->label);
    ShowElem(page->Next->box);   ShowElem(page->RoundCounter->box);
    
    return TRUE;
}