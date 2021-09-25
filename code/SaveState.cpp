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
        
        ls_bufferAddUnistring(buf, curr->name);
        ls_bufferAddDWord(buf, curr->numMobs);
        
        for(u32 j = 0; j < curr->numMobs; j++)
        {
            ls_bufferAddUnistring(buf, curr->mobName[j]);
            ls_bufferAddUnistring(buf, curr->mobBonus[j]);
            ls_bufferAddUnistring(buf, curr->mobFinal[j]);
            //ls_bufferAddUnistring(buf, curr->mobAC[j]);
        }
        
        ls_bufferAddDWord(buf, curr->numAllies);
        for(u32 j = 0; j < curr->numAllies; j++)
        {
            ls_bufferAddUnistring(buf, curr->allyName[j]);
            ls_bufferAddUnistring(buf, curr->allyBonus[j]);
            ls_bufferAddUnistring(buf, curr->allyFinal[j]);
            //ls_bufferAddUnistring(buf, curr->allyAC[j]);
        }
        
        ls_bufferAddDWord(buf, curr->numThrowers);
        for(u32 j = 0; j < curr->numThrowers; j++)
        {
            ls_bufferAddUnistring(buf, curr->throwerName[j]);
            ls_bufferAddUnistring(buf, curr->throwerHit[j]);
            ls_bufferAddUnistring(buf, curr->throwerDamage[j]);
        }
        
    }
    
#if 0
    
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
    
    
#endif
    
    char outName[64] = {};
    ls_sprintf(outName, "SaveFile_v%d", global_saveVersion);
    ls_writeFile(outName, (char *)buf->data, buf->size, FALSE);
    
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
    if(fileVersion != global_saveVersion) { return FALSE; }
    
    State.inBattle                 = ls_bufferReadDWord(buf);
    State.encounters.numEncounters = ls_bufferReadDWord(buf);
    
    InitPage *Page = State.Init;
    
    for(u32 i = 0; i < State.encounters.numEncounters; i++)
    {
        Encounter *curr = State.encounters.Enc + i;
        
        //TODO: Remove this when everything is checked to be working properly.
        AssertMsg(curr->name.data == NULL, "We don't want to leak memory\n");
        curr->name = ls_bufferReadUnistring(buf);
        
        curr->numMobs = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numMobs; j++)
        {
            curr->mobName[j]  = ls_bufferReadUnistring(buf);
            curr->mobBonus[j] = ls_bufferReadUnistring(buf);
            curr->mobFinal[j] = ls_bufferReadUnistring(buf);
            //curr->mobAC[j]  = ls_bufferReadUnistring(buf);
        }
        
        curr->numAllies = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numAllies; j++)
        {
            curr->allyName[j]  = ls_bufferReadUnistring(buf);
            curr->allyBonus[j] = ls_bufferReadUnistring(buf);
            curr->allyFinal[j] = ls_bufferReadUnistring(buf);
            //curr->allyAC[j]    = ls_bufferReadUnistring(buf);
        }
        
        curr->numThrowers = ls_bufferReadDWord(buf);
        for(u32 j = 0; j < curr->numThrowers; j++)
        {
            curr->throwerName[j]   = ls_bufferReadUnistring(buf);
            curr->throwerHit[j]    = ls_bufferReadUnistring(buf);
            curr->throwerDamage[j] = ls_bufferReadUnistring(buf);
        }
        
        ls_uiListBoxAddEntry(cxt, &Page->EncounterSel, curr->name);
    }
    
    
    return TRUE;
    
#if 0
    
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
    
    ShowActiveCounters(page->Counters, COUNTER_NUM);
    
    ShowOrder(page->Order, page->VisibleOrder);
    
    HideElem(page->EncounterSel->box); HideElem(page->EncounterSel->label);
    HideElem(page->EncounterName->box); HideElem(page->Save->box);
    
    HideElem(page->Set->box);    HideElem(page->Roll->box);
    HideElem(page->Mobs->box);   HideElem(page->Mobs->label);
    HideElem(page->Allies->box); HideElem(page->Allies->label);
    ShowElem(page->Next->box);   ShowElem(page->RoundCounter->box);
    
    return TRUE;
    
#endif
}
