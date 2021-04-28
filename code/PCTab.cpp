
void saveAS()
{
    string as = getText(State.PC->Scores->Box[ABILITY_STR]->box);
    pc.AbilityScores[ABILITY_STR] = (u8)ls_stoi(as);
    ls_strFree(&as);
    
    as = getText(State.PC->Scores->Box[ABILITY_DEX]->box);
    pc.AbilityScores[ABILITY_DEX] = (u8)ls_stoi(as);
    ls_strFree(&as);
    
    as = getText(State.PC->Scores->Box[ABILITY_CON]->box);
    pc.AbilityScores[ABILITY_CON] = (u8)ls_stoi(as);
    ls_strFree(&as);
    
    as = getText(State.PC->Scores->Box[ABILITY_INT]->box);
    pc.AbilityScores[ABILITY_INT] = (u8)ls_stoi(as);
    ls_strFree(&as);
    
    as = getText(State.PC->Scores->Box[ABILITY_WIS]->box);
    pc.AbilityScores[ABILITY_WIS] = (u8)ls_stoi(as);
    ls_strFree(&as);
    
    as = getText(State.PC->Scores->Box[ABILITY_CHA]->box);
    pc.AbilityScores[ABILITY_CHA] = (u8)ls_stoi(as);
    ls_strFree(&as);
}

void loadAS()
{
    u8 v = pc.AbilityScores[ABILITY_STR];
    char *a = ls_itoa(v);
    Edit_SetText(State.PC->Scores->Box[ABILITY_STR]->box, a);
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_STR]->box, getASBonusStr(v));
    ls_free(a);
    
    v = pc.AbilityScores[ABILITY_DEX];
    a = ls_itoa(v);
    Edit_SetText(State.PC->Scores->Box[ABILITY_DEX]->box, a);
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_DEX]->box, getASBonusStr(v));
    ls_free(a);
    
    v = pc.AbilityScores[ABILITY_CON];
    a = ls_itoa(v);
    Edit_SetText(State.PC->Scores->Box[ABILITY_CON]->box, a);
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_CON]->box, getASBonusStr(v));
    ls_free(a);
    
    v = pc.AbilityScores[ABILITY_INT];
    a = ls_itoa(v);
    Edit_SetText(State.PC->Scores->Box[ABILITY_INT]->box, a);
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_INT]->box, getASBonusStr(v));
    ls_free(a);
    
    v = pc.AbilityScores[ABILITY_WIS];
    a = ls_itoa(v);
    Edit_SetText(State.PC->Scores->Box[ABILITY_WIS]->box, a);
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_WIS]->box, getASBonusStr(v));
    ls_free(a);
    
    v = pc.AbilityScores[ABILITY_CHA];
    a = ls_itoa(v);
    Edit_SetText(State.PC->Scores->Box[ABILITY_CHA]->box, a);
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_CHA]->box, getASBonusStr(v));
    ls_free(a);
}

void UpdateSavingThrows()
{
    s32 Fortitude = ClassSavingThrows[pc.Class][0][pc.lvl] + getASBonusVal(pc.AbilityScores[ABILITY_CON]);
    s32 Reflex = ClassSavingThrows[pc.Class][1][pc.lvl] + getASBonusVal(pc.AbilityScores[ABILITY_DEX]);
    s32 Will = ClassSavingThrows[pc.Class][2][pc.lvl] + getASBonusVal(pc.AbilityScores[ABILITY_WIS]);
    
    char *ST = ls_itoa(Fortitude);
    Edit_SetText(State.PC->SavingThrows[0]->box, ST);
    ls_free(ST);
    
    ST = ls_itoa(Reflex);
    Edit_SetText(State.PC->SavingThrows[1]->box, ST);
    ls_free(ST);
    
    ST = ls_itoa(Will);
    Edit_SetText(State.PC->SavingThrows[2]->box, ST);
    ls_free(ST);
}

b32 PCTabOnComboSelect(u32 commandID, HWND handle)
{
    PCPage *PC   = State.PC;
    
    if(commandID == PC->Race->id)
    {
        string s = getText(handle);
        
        GameRace r = getRaceFromString(s);
        pc.Race = r;
        
        if(State.PC->wasClassChosen)
        { ListBox_ResetContent(State.PC->RacialTraits->box); }
        
        if(!State.PC->wasClassChosen) { State.PC->wasClassChosen = TRUE; }
        
        char **TraitsList;
        u32 arrSize = 0;
        
        TraitsList = (char **)RaceTraits[r];
        arrSize = RaceTraitsArraySize[r];
        
        AddAllListBoxItems(State.PC->RacialTraits->box, TraitsList, arrSize);
        
        ls_strFree(&s);
        
        return TRUE;
    }
    else if(commandID == PC->Class->id)
    {
        string s = getText(handle);
        
        GameClass r = getClassFromString(s);
        pc.Class = r;
        
        UpdateSavingThrows();
        
        ls_strFree(&s);
        
        return TRUE;
    }
    else if(commandID == PC->XPCurve->id)
    {
        State.PC->xpIdx = (XPCurveIdx)ComboBox_GetCurSel(handle);
        pc.xpCurve = State.PC->xpIdx;
        char **xpCurve = (char **)XPCurvesArr[State.PC->xpIdx];
        
        string s = getText(State.PC->currLevel->box);
        u32 currLevel = ls_stoi(s);
        
        Edit_SetText(State.PC->nextLevelXP->box, xpCurve[currLevel]);
        
        ls_strFree(&s);
        
        return TRUE;
    }
    
    return FALSE;
}

void PCTabOnKillFocus(u32 commandID, HWND handle)
{
    PCPage    *PC   = State.PC;
    
    if(commandID == PC->Name->id)
    {
        string newName = getText(handle);
        
        if(pc.Name.data)
        { ls_strFree(&pc.Name); }
        
        pc.Name = newName;
    }
    
    else if(commandID == PC->Player->id)
    {
        string newPlayer = getText(handle);
        
        if(pc.Player.data)
        { ls_strFree(&pc.Player); }
        
        pc.Player = newPlayer;
    }
    
    else if(commandID == PC->Scores->Box[ABILITY_STR]->id)
    {
        string s = getText(handle);
        u32 v = ls_stoi(s);
        pc.AbilityScores[ABILITY_STR] = v;
        
        Edit_SetText(State.PC->Scores->Bonus[ABILITY_STR]->box, getASBonusStr(v));
        UpdateSavingThrows();
        
        ls_strFree(&s);
    }
    
    else if(commandID == PC->Scores->Box[ABILITY_DEX]->id)
    {
        string s = getText(handle);
        u32 v = ls_stoi(s);
        pc.AbilityScores[ABILITY_DEX] = v;
        
        Edit_SetText(State.PC->Scores->Bonus[ABILITY_DEX]->box, getASBonusStr(v));
        UpdateSavingThrows();
        
        ls_strFree(&s);
    }
    
    else if(commandID == PC->Scores->Box[ABILITY_CON]->id)
    {
        string s = getText(handle);
        u32 v = ls_stoi(s);
        pc.AbilityScores[ABILITY_CON] = v;
        
        Edit_SetText(State.PC->Scores->Bonus[ABILITY_CON]->box, getASBonusStr(v));
        UpdateSavingThrows();
        
        ls_strFree(&s);
    }
    
    else if(commandID == PC->Scores->Box[ABILITY_INT]->id)
    {
        string s = getText(handle);
        u32 v = ls_stoi(s);
        pc.AbilityScores[ABILITY_INT] = v;
        
        Edit_SetText(State.PC->Scores->Bonus[ABILITY_INT]->box, getASBonusStr(v));
        UpdateSavingThrows();
        
        ls_strFree(&s);
    }
    
    else if(commandID == PC->Scores->Box[ABILITY_WIS]->id)
    {
        string s = getText(handle);
        u32 v = ls_stoi(s);
        pc.AbilityScores[ABILITY_WIS] = v;
        
        Edit_SetText(State.PC->Scores->Bonus[ABILITY_WIS]->box, getASBonusStr(v));
        UpdateSavingThrows();
        
        ls_strFree(&s);
    }
    
    else if(commandID == PC->Scores->Box[ABILITY_CHA]->id)
    {
        string s = getText(handle);
        u32 v = ls_stoi(s);
        pc.AbilityScores[ABILITY_CHA] = v;
        
        Edit_SetText(State.PC->Scores->Bonus[ABILITY_CHA]->box, getASBonusStr(v));
        UpdateSavingThrows();
        
        ls_strFree(&s);
    }
    
    else if(commandID == PC->currLevel->id)
    {
        string s = getText(handle);
        u32 currLevel = ls_stoi(s);
        if(currLevel > 20)
        { currLevel = 20; Edit_SetText(handle, "20"); }
        pc.lvl = (u8)currLevel;
        
        char **xpCurve = (char **)XPCurvesArr[State.PC->xpIdx];
        Edit_SetText(State.PC->nextLevelXP->box, xpCurve[currLevel]);
        
        UpdateSavingThrows();
        
        Edit_SetText(State.PC->BaseAttackBonus->box,
                     ClassBABString[pc.Class][0][pc.lvl]);
        
        ls_strFree(&s);
    }
    else if(commandID == PC->currXP->id)
    {
        string s = getText(handle);
        u32 currXP = ls_stoi(s);
        
        pc.xp = currXP;
        
        ls_strFree(&s);
    }
}


void AddAbilityScoreBoxes(HWND WindowHandle, u32 baseX, u32 baseY, u64 *ElementId)
{
    PCPage *Page = State.PC;
    HWND *wA = Page->WindowsArray +  Page->numWindows;
    
    AbilityScores *Scores  = Page->Scores;
    
    s32 yPos = baseY;
    for(u32 i = 0; i < 6; i++)
    {
        Scores->Box[i] = AddNumberBox(WindowHandle, wA, (char *)AS_string[i], LABEL_UP,
                                      364, yPos, 60, 20, (*ElementId)++);
        wA += 2;
        Scores->Bonus[i] = AddValueBox(WindowHandle, wA, (char *)"", LABEL_NULL, 0,
                                       434, yPos, 40, 20, (*ElementId)++);
        wA += 1;
        Scores->Plus1[i]  = AddButton(WindowHandle, wA, (char *)"+1",
                                      484, yPos, 30, 20, (*ElementId)++);
        wA += 1;
        Scores->Plus10[i] = AddButton(WindowHandle, wA, (char *)"+10",
                                      524, yPos, 40, 20, (*ElementId)++);
        wA += 1;
        Scores->Plus1[i]->LinkedText = Scores->Box[i];
        Scores->Plus1[i]->value = 1;
        Scores->Plus10[i]->LinkedText = Scores->Box[i];
        Scores->Plus10[i]->value = 10;
        
        yPos += 44;
        Page->numWindows += 5;
    }
}

void DrawPCTab(HWND WindowHandle, u64 *ElementId)
{
    PCPage *Page = State.PC;
    HWND *wA = Page->WindowsArray + Page->numWindows;
    
    //
    // Create Basic Fields
    //
    
    Page->Name = AddTextBox(WindowHandle, wA, "Name", LABEL_LEFT, 
                            128, 42, 100, 20, (*ElementId)++);
    wA += 2;
    Page->Player = AddTextBox(WindowHandle, wA, "Player", LABEL_LEFT,
                              128, 82, 100, 20, (*ElementId)++);
    wA += 2;
    Page->Race = AddComboBox(WindowHandle, wA, "Race", LABEL_LEFT,
                             128, 122, 100, 20, (*ElementId)++, ArraySize(Races));
    wA += 2;
    AddAllComboBoxItems(Page->Race->box, Races, ArraySize(Races));
    
    
    Page->Class = AddComboBox(WindowHandle, wA, "Class", LABEL_LEFT,
                              128, 162, 100, 20, (*ElementId)++, ArraySize(Classes));
    wA += 2;
    AddAllComboBoxItems(Page->Class->box, Classes, ArraySize(Classes));
    
    Page->numWindows += 8;
    
    //
    // Create Ability Scores Fields
    //
    
    Page->Scores = (AbilityScores *)ls_alloc(sizeof(AbilityScores));
    AddAbilityScoreBoxes(WindowHandle, 364, 40, ElementId);
    
    wA = Page->WindowsArray + Page->numWindows;
    
    //
    // Skill Choice
    //
    
    Page->RacialTraits = AddListBox(WindowHandle, wA, "Racial Traits", LABEL_UP,
                                    28, 302, 1220, 384, (*ElementId)++);
    wA += 2;
    
    Page->currLevel = AddNumberBox(WindowHandle, wA, "LvL", LABEL_UP,
                                   664, 42, 100, 20, (*ElementId)++);
    Edit_SetText(Page->currLevel->box, "1");
    
    //
    // XP Info
    //
    
    wA += 2;
    
    Page->currXP = AddNumberBox(WindowHandle, wA, "XP", LABEL_UP,
                                784, 42, 100, 20, (*ElementId)++);
    
    wA += 2;
    
    u32 nextXPValue = 0;
    Page->nextLevelXP = AddValueBox(WindowHandle, wA, "To Next LVL", LABEL_UP,
                                    nextXPValue, 924, 42, 100, 20, (*ElementId)++);
    wA += 2;
    
    Page->XPCurve = AddComboBox(WindowHandle, wA, "XP Curve", LABEL_UP,
                                1044, 42, 100, 20, (*ElementId)++, ArraySize(XPCurvesString));
    wA += 2;
    AddAllComboBoxItems(Page->XPCurve->box, XPCurvesString, ArraySize(XPCurvesString));
    
    //
    // Class & Level Info
    //
    
    Page->BaseAttackBonus = AddValueBox(WindowHandle, wA, "BAB", LABEL_LEFT,
                                        0, 764, 102, 20, 20, (*ElementId)++);
    wA += 2;
    Page->SavingThrows[0] = AddValueBox(WindowHandle, wA, "FORTITUDE", LABEL_LEFT,
                                        0, 764, 142, 20, 20, (*ElementId)++);
    wA += 2;
    Page->SavingThrows[1] = AddValueBox(WindowHandle, wA, "REFLEX", LABEL_LEFT,
                                        0, 764, 182, 20, 20, (*ElementId)++);
    wA += 2;
    Page->SavingThrows[2] = AddValueBox(WindowHandle, wA, "WILL", LABEL_LEFT,
                                        0, 764, 222, 20, 20, (*ElementId)++);
    wA += 2;
    
    Page->numWindows += 18;
}
