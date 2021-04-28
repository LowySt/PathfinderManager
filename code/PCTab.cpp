
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
