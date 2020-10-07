string getText(HWND hwnd);
void UpdateSavingThrows();
void HideElem(HWND box);
void ShowElem(HWND box);
void HideOrder(OrderField *o, s32 n);
void ShowOrder(OrderField *o, s32 n);
void HideInitField(InitField *f, s32 n);
void HideInitFieldAdd(InitField *f, u32 idx, s32 max);
void ShowInitFieldAdd(InitField *f, u32 idx, s32 max);

//TODO: I'd like to add functionality for removing Ability Scores.
//TODO: Start using the ElementMap to stop doing the stupid for loops everywhere
void OnButton(u32 commandID, u32 notificationCode, HWND handle)
{
    InitPage  *Init = State.Init;
    FeatsPage *Feats = State.Feats;
    PCPage    *PCPage   = State.PC;
    
    Button *clicked = (Button *)ElementMap[commandID].ptr;
    
    if(clicked->LinkedText != 0x0) {
        TextBox *text = clicked->LinkedText;
        
        AbilityScores *S = PCPage->Scores;
        
        string s = getText(text->box);
        u32 val = ls_stoi(s);
        val += clicked->value;
        
        if(S->Plus1[ABILITY_STR]->id ||
           S->Plus10[ABILITY_STR]->id)
        {
            Edit_SetText(S->Bonus[ABILITY_STR]->box, getASBonusStr(val));
            pc.AbilityScores[ABILITY_STR] = val;
        }
        else if(S->Plus1[ABILITY_DEX]->id ||
                S->Plus10[ABILITY_DEX]->id) 
        {
            Edit_SetText(S->Bonus[ABILITY_DEX]->box, getASBonusStr(val));
            pc.AbilityScores[ABILITY_DEX] = val;
        } 
        else if(S->Plus1[ABILITY_CON]->id ||
                S->Plus10[ABILITY_CON]->id)
        {
            Edit_SetText(S->Bonus[ABILITY_CON]->box, getASBonusStr(val));
            pc.AbilityScores[ABILITY_CON] = val;
        } 
        else if(S->Plus1[ABILITY_INT]->id ||
                S->Plus10[ABILITY_INT]->id)
        {
            Edit_SetText(S->Bonus[ABILITY_INT]->box, getASBonusStr(val));
            pc.AbilityScores[ABILITY_INT] = val;
        } 
        else if(S->Plus1[ABILITY_WIS]->id ||
                S->Plus10[ABILITY_WIS]->id)
        {
            Edit_SetText(S->Bonus[ABILITY_WIS]->box, getASBonusStr(val));
            pc.AbilityScores[ABILITY_WIS] = val;
        } 
        else if(S->Plus1[ABILITY_CHA]->id ||
                S->Plus10[ABILITY_CHA]->id)
        {
            Edit_SetText(S->Bonus[ABILITY_CHA]->box, getASBonusStr(val));
            pc.AbilityScores[ABILITY_CHA] = val;
        } 
        
        string newVal = ls_itos(val);
        ls_strNullTerminate(&newVal);
        
        BOOL ret = SetWindowTextA(text->box, newVal.data);
        
        UpdateSavingThrows();
        
        ls_strFree(&s);
        ls_strFree(&newVal);
    }
    
    if(commandID == Init->Roll->id)
    {
        for(u32 i = 0; i < Init->VisibleMobs; i++)
        {
            s32 die = pcg32_bounded(&pcg32_global, 20) + 1;
            char v[32] = {};
            u32 len = Edit_GetText(Init->MobFields[i].Bonus->box, v, 32);
            s32 bonus = ls_atoi(v, len);
            char final[32] = {};
            ls_itoa_t(bonus + die, final, 32);
            Edit_SetText(Init->MobFields[i].Final->box, final);
        }
        
        for(u32 i = 0; i < Init->VisibleAllies; i++)
        {
            s32 die = pcg32_bounded(&pcg32_global, 20) + 1;
            char v[32] = {};
            u32 len = Edit_GetText(Init->MobFields[i].Bonus->box, v, 32);
            s32 bonus = ls_atoi(v, len);
            char final[32] = {};
            ls_itoa_t(bonus + die, final, 32);
            Edit_SetText(Init->AllyFields[i].Final->box, final);
        }
    }
    
    if(commandID == Init->Save->id)
    {
        if(State.StateData == 0x0) { State.StateData = ls_alloc(MBytes(1)); }
        if(State.encounters.data == 0x0) { 
            State.encounters.data = (u8 *)State.StateData + encounterOffset;
        }
        
        buffer encBuff = ls_bufferViewIntoPtr(State.encounters.data, KBytes(64));
        u32 numEncounters = ls_bufferPeekDWord(&encBuff);
        
        if(numEncounters > 64) { Assert(FALSE); /*TODO: Loggging*/ }
        
        ls_bufferChangeDWord(&encBuff, numEncounters + 1);
        ls_bufferAdvanceCursor(&encBuff, sizeof(u32));
        
        //NOTE: This pushes us to the next free Entry;
        u32 offset = numEncounters * sizeOfEncEntry;
        ls_bufferAdvanceCursor(&encBuff, offset);
        
        //Add encounter name
        char encName[32] = {};
        u32 len = Edit_GetText(Init->EncounterName->box, encName, 32);
        ls_bufferAddData(&encBuff, encName, 32);
        
        //Add mobs
        ls_bufferAddDWord(&encBuff, Init->VisibleMobs);
        
        for(u32 i = 0; i < Init->VisibleMobs; i++)
        {
            char mName[32] = {};
            u32 len = Edit_GetText(Init->MobFields[i].Name->box, mName, 32);
            ls_bufferAddData(&encBuff, mName, 32);
            
            char mBonus[8] = {};
            len = Edit_GetText(Init->MobFields[i].Bonus->box, mBonus, 8);
            u32 mobBonus = (u32)ls_atoi(mBonus, len);
            
            ls_bufferAddDWord(&encBuff, mobBonus);
        }
        
        if(Init->VisibleMobs < MOB_NUM) { 
            u32 paddingBytes = (MOB_NUM - Init->VisibleMobs) * sizeOfInitEntry;
            ls_bufferAdvanceCursor(&encBuff, paddingBytes);
        }
        
        //Add allies
        ls_bufferAddDWord(&encBuff, Init->VisibleAllies);
        
        for(u32 i = 0; i < Init->VisibleAllies; i++)
        {
            char aName[32] = {};
            u32 len = Edit_GetText(Init->AllyFields[i].Name->box, aName, 32);
            ls_bufferAddData(&encBuff, aName, 32);
            
            char aBonus[8] = {};
            len = Edit_GetText(Init->AllyFields[i].Bonus->box, aBonus, 8);
            u32 allyBonus = (u32)ls_atoi(aBonus, len);
            
            ls_bufferAddDWord(&encBuff, allyBonus);
        }
        
        if(Init->VisibleAllies < ALLY_NUM) { 
            u32 paddingBytes = (ALLY_NUM - Init->VisibleAllies) * sizeOfInitEntry;
            ls_bufferAdvanceCursor(&encBuff, paddingBytes);
        }
        
        ls_clearBuffer(&encBuff);
        
        //Write to disk the StateFile
        ls_writeFile((char *)"State.bin", (char *)State.StateData, MBytes(1)-1, FALSE);
    }
    
    if(commandID == Init->Set->id)
    {
        tmp_order ord[ORDER_NUM] = {};
        u32 idx = 0;
        
        for(u32 i = 0; i < Init->VisibleMobs; i++)
        {
            char v[32] = {};
            u32 len = Edit_GetText(Init->MobFields[i].Final->box, v, 32);
            ord[idx].init     = ls_atoi(v, len);
            ord[idx].nameBox  = Init->MobFields[i].Name->box;
            ord[idx].fieldId  = Init->MobFields[i].id;
            ord[idx].isParty  = FALSE;
            ord[idx++].isMob  = TRUE;
        }
        
        for(u32 i = 0; i < Init->VisibleAllies; i++)
        {
            char v[32] = {};
            u32 len = Edit_GetText(Init->AllyFields[i].Final->box, v, 32);
            ord[idx].init     = ls_atoi(v, len);
            ord[idx].nameBox  = Init->AllyFields[i].Name->box;
            ord[idx].fieldId  = Init->AllyFields[i].id;
            ord[idx].isParty  = FALSE;
            ord[idx++].isMob  = FALSE;
        }
        
        for(u32 i = 0; i < PARTY_NUM; i++)
        {
            char v[32] = {};
            u32 len = Edit_GetText(Init->PlayerFields[i].Bonus->box, v, 32);
            ord[idx].init      = ls_atoi(v, len);
            ord[idx].nameBox   = Init->PlayerFields[i].Bonus->label;
            ord[idx].fieldId   = Init->PlayerFields[i].id;;
            ord[idx].isParty   = TRUE;
            ord[idx++].isMob   = FALSE;
        }
        
        order_ascending(ord, Init->VisibleOrder);
        
        for(u32 i = 0, j = Init->VisibleOrder - 1; i < Init->VisibleOrder; i++, j--)
        {
            char v[32] = {};
            u32 len = Edit_GetText(ord[j].nameBox, v, 32);
            
            Edit_SetText(Init->Order[i].Field->box, v);
            Init->Order[i].fieldId  = ord[j].fieldId;
            Init->Order[i].isMob    = ord[j].isMob;
            Init->Order[i].isParty  = ord[j].isParty;
            
            if(i == 0) { Edit_SetText(Init->Current->box, v); }
        }
        
        for(u32 i = 0; i < MOB_NUM; i++)  { 
            Edit_SetReadOnly(Init->MobFields[i].Name->box, TRUE);
            Edit_SetReadOnly(Init->MobFields[i].Bonus->box, TRUE);
        }
        
        for(u32 i = 0; i < ALLY_NUM; i++) { 
            Edit_SetReadOnly(Init->AllyFields[i].Name->box, TRUE);
            Edit_SetReadOnly(Init->AllyFields[i].Bonus->box, TRUE);
        }
        
        Init->turnsInRound = Init->VisibleOrder - 1;
        
        State.inBattle = TRUE;
        HideElem(Init->Set->box);
        HideElem(Init->Mobs->box);   HideElem(Init->Mobs->label);
        HideElem(Init->Allies->box); HideElem(Init->Allies->label);
        ShowElem(Init->Next->box);
    }
    
    if(commandID == Init->Next->id)
    {
        //NOTE: Make sure removing order fields doesn't fuck this up!
        Init->currIdx = (Init->currIdx + 1) % Init->VisibleOrder;
        
        char v[32] = {};
        u32 len = Edit_GetText(Init->Order[Init->currIdx].Field->box, v, 32);
        Edit_SetText(Init->Current->box, v);
        
        for(u32 i = 0; i < COUNTER_NUM; i++)
        {
            Counter *C = &Init->Counters[i];
            
            if(C->isActive == TRUE)
            {
                if(C->roundCounter >= Init->turnsInRound)
                { 
                    C->roundCounter = 0;
                    
                    char v[8] = {};
                    u32 len = Edit_GetText(C->Rounds->box, v, 8);
                    s32 val = ls_atoi(v, len);
                    
                    if((val - 1) == 0)
                    {
                        Edit_SetReadOnly(Init->Counters[i].Field->box, FALSE);
                        Edit_SetText(Init->Counters[i].Field->box, "");
                        Edit_SetText(Init->Counters[i].Rounds->box, "0");
                        
                        Init->Counters[i].roundCounter = 0;
                        Init->Counters[i].isActive     = FALSE;
                        return;
                    }
                    
                    char newV[8] = {};
                    ls_itoa_t(val - 1, newV, 8);
                    
                    Edit_SetText(C->Rounds->box, newV);
                    return;
                }
                
                C->roundCounter += 1;
                return;
            }
        }
    }
    
    if(commandID == Init->Reset->id)
    {
        State.inBattle = FALSE;
        
        for(u32 i = 0; i < MOB_NUM; i++)
        { 
            Init->MobFields[i].id = i;
            Edit_SetText(Init->MobFields[i].Name->box, MobName[i]);
            Edit_SetText(Init->MobFields[i].Bonus->box, "");
            Edit_SetText(Init->MobFields[i].Final->box, "0");
            
            Edit_SetReadOnly(Init->MobFields[i].Name->box, FALSE);
            Edit_SetReadOnly(Init->MobFields[i].Bonus->box, FALSE);
        }
        
        for(u32 i = 0; i < ALLY_NUM; i++)
        { 
            Init->AllyFields[i].id = i;
            Edit_SetText(Init->AllyFields[i].Name->box, AllyName[i]);
            Edit_SetText(Init->AllyFields[i].Bonus->box, "");
            Edit_SetText(Init->AllyFields[i].Final->box, "0");
            
            Edit_SetReadOnly(Init->AllyFields[i].Name->box, FALSE);
            Edit_SetReadOnly(Init->AllyFields[i].Bonus->box, FALSE);
        }
        
        for(u32 i = 0; i < ORDER_NUM; i++)
        { 
            Edit_SetText(Init->Order[i].Field->box, "");
            Init->Order[i].fieldId = 0;
            Init->Order[i].isMob   = 0;
            Init->Order[i].isParty = 0;
        }
        
        for(u32 i = 0; i < COUNTER_NUM; i++)
        {
            Edit_SetReadOnly(Init->Counters[i].Field->box, FALSE);
            Edit_SetText(Init->Counters[i].Field->box, "");
            Edit_SetText(Init->Counters[i].Rounds->box, "");
            
            Init->Counters[i].roundCounter = 0;
            Init->Counters[i].isActive     = FALSE;
        }
        
        Init->VisibleMobs   = 0;
        Init->VisibleAllies = 0;
        Init->VisibleOrder  = PARTY_NUM;
        
        Edit_SetText(Init->Current->box, "");
        Init->currIdx       = 0;
        Init->turnsInRound  = 0;
        
        HideInitField(Init->MobFields, MOB_NUM);
        HideInitField(Init->AllyFields, ALLY_NUM);
        HideOrder(Init->Order, ORDER_NUM);
        HideElem(Init->Next->box);
        
        ShowOrder(Init->Order, Init->VisibleOrder);
        ShowElem(Init->Set->box);
        ShowElem(Init->Mobs->box);
        ShowElem(Init->Allies->box);
        
        ComboBox_SetCurSel(Init->Mobs->box, 0);
        ComboBox_SetCurSel(Init->Allies->box, 0);
    }
    
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        if(commandID == Init->Counters[i].Start->id)
        {
            if(State.inBattle == FALSE) { return; }
            
            char rounds[8] = {};
            
            Edit_SetReadOnly(Init->Counters[i].Field->box, TRUE);
            u32 len = Edit_GetText(Init->Counters[i].Rounds->box, rounds, 8);
            
            s32 val = ls_atoi(rounds, len);
            if(val <= 0) { return; }
            
            Init->Counters[i].isActive     = TRUE;
            Init->Counters[i].roundCounter = 0;
            
            return;
        }
    }
    
    for(u32 i = 0; i < MOB_NUM; i++)
    {
        if(commandID == Init->MobFields[i].New.Add->id)
        {
            if(State.inBattle == FALSE) { return; }
            
            HideInitFieldAdd(Init->MobFields, i, MOB_NUM);
        }
    }
    
    for(u32 i = 0; i < ALLY_NUM; i++)
    {
        if(commandID == Init->AllyFields[i].New.Add->id)
        {
            if(State.inBattle == FALSE) { return; }
            
            HideInitFieldAdd(Init->AllyFields, i, ALLY_NUM);
        }
    }
    
    for(u32 i = 0; i < Init->VisibleOrder; i++)
    {
        if(commandID == Init->Order[i].Remove->id)
        {
            if(State.inBattle == FALSE) { return; }
            
            //TODO:Remove the equivalent entry from Mobs/Allies
            if(Init->Order[i].isMob == TRUE)
            {
                u32 mobId = Init->Order[i].fieldId;
                
                InitField *toRemove = getInitById(Init->MobFields, Init->VisibleMobs, mobId);
                if(toRemove == 0x0) { /*TODO: Logging */ Assert(FALSE); }
                
                char name[32] = {};
                Edit_GetText(Init->MobFields[Init->VisibleMobs - 1].Name->box, name, 32);
                Edit_SetText(toRemove->Name->box, name);
                
                char box[8] = {};
                Edit_GetText(Init->MobFields[Init->VisibleMobs - 1].Bonus->box, box, 8);
                Edit_SetText(toRemove->Bonus->box, box);
                
                char final[8] = {};
                Edit_GetText(Init->MobFields[Init->VisibleMobs - 1].Final->box, final, 8);
                Edit_SetText(toRemove->Final->box, final);
                
                toRemove->id = Init->MobFields[Init->VisibleMobs - 1].id;
                
                HideInitField(&Init->MobFields[Init->VisibleMobs - 1], 1);
                HideInitFieldAdd(Init->MobFields, Init->VisibleMobs, MOB_NUM);
                ShowInitFieldAdd(Init->MobFields, Init->VisibleMobs-1, MOB_NUM);
                
                Init->VisibleMobs -= 1;
            }
            else if(Init->Order[i].isParty == TRUE)
            {
                //NOTE: Probably just do nothing?
            }
            else
            {
                u32 allyId = Init->Order[i].fieldId;
                
                InitField *toRemove = getInitById(Init->AllyFields, Init->VisibleAllies, allyId);
                if(toRemove == 0x0) { /*TODO: Logging */ Assert(FALSE); }
                
                char name[32] = {};
                Edit_GetText(Init->AllyFields[Init->VisibleAllies - 1].Name->box, name, 32);
                Edit_SetText(toRemove->Name->box, name);
                
                char box[8] = {};
                Edit_GetText(Init->AllyFields[Init->VisibleAllies - 1].Bonus->box, box, 32);
                Edit_SetText(toRemove->Bonus->box, box);
                
                char final[8] = {};
                Edit_GetText(Init->AllyFields[Init->VisibleAllies - 1].Final->box, final, 32);
                Edit_SetText(toRemove->Final->box, final);
                
                toRemove->id = Init->AllyFields[Init->VisibleAllies - 1].id;
                
                HideInitField(&Init->AllyFields[Init->VisibleAllies - 1], 1);
                HideInitFieldAdd(Init->AllyFields, Init->VisibleAllies, ALLY_NUM);
                ShowInitFieldAdd(Init->AllyFields, Init->VisibleAllies-1, ALLY_NUM);
                
                Init->VisibleAllies -= 1;
            }
            
            //NOTE: Move everything that is below the removed up 1.
            if(i != (Init->VisibleOrder - 1)) 
            {
                for(u32 j = i + 1; j < Init->VisibleOrder; j++)
                {
                    char v[32] = {};
                    u32 len = Edit_GetText(Init->Order[j].Field->box, v, 32);
                    Edit_SetText(Init->Order[j-1].Field->box, v);
                    
                    Init->Order[j-1].fieldId = Init->Order[j].fieldId;
                    Init->Order[j-1].isMob   = Init->Order[j].isMob;
                    Init->Order[j-1].isParty = Init->Order[j].isParty;
                }
            }
            
            Edit_SetText(Init->Order[Init->VisibleOrder - 1].Field->box, "");
            HideOrder(Init->Order + (Init->VisibleOrder - 1), 1);
            
            Init->turnsInRound -= 1;
            Init->VisibleOrder -= 1;
            if(Init->currIdx >= i) { Init->currIdx -= 1; }
            
            //NOTE:TODO: Should I return here?
        }
    }
    
}