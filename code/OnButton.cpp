string getText(HWND hwnd);
void UpdateSavingThrows();
void HideElem(HWND box);
void ShowElem(HWND box);
void HideOrder(OrderField *o, s32 n);
void ShowOrder(OrderField *o, s32 n);
void HideInitField(InitField *f, s32 n);

//TODO: I'd like to add functionality for removing Ability Scores.
void OnButton(u32 commandID, u32 notificationCode, HWND handle)
{
    InitPage  *Init = State.Init;
    FeatsPage *Feats = State.Feats;
    PCPage    *PCPage   = State.PC;
    
    Button *clicked = (Button *)ElementMap[commandID].ptr;
    
    if(clicked->LinkedText != 0x0) {
        TextBox *text = clicked->LinkedText;
        
        string s = getText(text->box);
        u32 val = ls_stoi(s);
        val += clicked->value;
        
        if((commandID == PCPage->Scores->Plus1[ABILITY_STR]->id) ||
           (commandID == PCPage->Scores->Plus10[ABILITY_STR]->id))
        {
            Edit_SetText(PCPage->Scores->Bonus[ABILITY_STR]->box, getASBonusStr(val));
            
            pc.AbilityScores[ABILITY_STR] = val;
        }
        else if((commandID == PCPage->Scores->Plus1[ABILITY_DEX]->id) ||
                (commandID == PCPage->Scores->Plus10[ABILITY_DEX]->id))
        {
            Edit_SetText(PCPage->Scores->Bonus[ABILITY_DEX]->box, getASBonusStr(val));
            
            pc.AbilityScores[ABILITY_DEX] = val;
        }
        else if((commandID == PCPage->Scores->Plus1[ABILITY_CON]->id) ||
                (commandID == PCPage->Scores->Plus10[ABILITY_CON]->id))
        {
            Edit_SetText(PCPage->Scores->Bonus[ABILITY_CON]->box, getASBonusStr(val));
            
            pc.AbilityScores[ABILITY_CON] = val;
        }
        else if((commandID == PCPage->Scores->Plus1[ABILITY_INT]->id) ||
                (commandID == PCPage->Scores->Plus10[ABILITY_INT]->id))
        {
            Edit_SetText(PCPage->Scores->Bonus[ABILITY_INT]->box, getASBonusStr(val));
            
            pc.AbilityScores[ABILITY_INT] = val;
        }
        else if((commandID == PCPage->Scores->Plus1[ABILITY_WIS]->id) ||
                (commandID == PCPage->Scores->Plus10[ABILITY_WIS]->id))
        {
            Edit_SetText(PCPage->Scores->Bonus[ABILITY_WIS]->box, getASBonusStr(val));
            
            pc.AbilityScores[ABILITY_WIS] = val;
        }
        else if((commandID == PCPage->Scores->Plus1[ABILITY_CHA]->id) ||
                (commandID == PCPage->Scores->Plus10[ABILITY_CHA]->id))
        {
            Edit_SetText(PCPage->Scores->Bonus[ABILITY_CHA]->box, getASBonusStr(val));
            
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
        
        Init->VisibleMobs   = 0;
        Init->VisibleAllies = 0;
        Init->VisibleOrder  = PARTY_NUM;
        
        Edit_SetText(Init->Current->box, "");
        Init->currIdx       = 0;
        
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
    
    for(u32 i = 0; i < Init->VisibleOrder; i++)
    {
        if(commandID == Init->Order[i].Remove->id)
        {
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
            
            Init->VisibleOrder -= 1;
            if(Init->currIdx >= i) { Init->currIdx -= 1; }
        }
    }
    
}