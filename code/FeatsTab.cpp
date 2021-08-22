#if 0
//TODO: Why is this here...
b32 ArrayContains(u32 *a, u32 v)
{
    for(size_t i = 0; i < State.Feats->usedFeats; i++)
    { if(a[i] == v) { return TRUE; } }
    return FALSE;
}

//TODO: Why is this here...
void ArrayRemove(u32 *a, u32 idx)
{
    u32 len = State.Feats->usedFeats;
    ls_memcpy(a + (idx + 1), a + idx, (len - idx)*sizeof(u32));
    a[len] = u32(-1);
}


void FeatsTabOnDoubleClick(u32 commandID, HWND handle)
{
    FeatsPage *Feats = State.Feats;
    
    if(commandID == Feats->Feats->id)
    {
        u32 index = ListBox_GetCurSel(handle);
        if(index > ArraySize(FeatsDesc))
        {
            ls_printf("Not implemented description of Feat n°%d\n", index);
            Assert(FALSE);
        }
        
        if(ArrayContains(State.Feats->ChosenFeatsIndices, index) == TRUE)
        { return; }
        
        ListBox_AddString(State.Feats->ChosenFeats->box, FeatNames[index]);
        State.Feats->ChosenFeatsIndices[State.Feats->usedFeats] = index;
        State.Feats->usedFeats += 1;
    }
    else if(commandID == Feats->ChosenFeats->id)
    {
        u32 index = ListBox_GetCurSel(handle);
        
        ListBox_DeleteString(handle, index);
        
        ArrayRemove(State.Feats->ChosenFeatsIndices, index);
        State.Feats->usedFeats -= 1;
    }
}

void DrawFeatsTab(HWND WindowHandle, u64 *ElementId)
{
    FeatsPage *Page = State.Feats;
    HWND *wA = Page->WindowsArray + Page->numWindows;
    
    Page->Feats = AddSingleSelListBox(WindowHandle, wA, "Feats Table", LABEL_UP,
                                      12, 48, 640, 456, (*ElementId)++);
    AddAllListBoxItems(Page->Feats->box, FeatNames, ArraySize(FeatNames));
    wA += 2;
    
    
    
    Page->FeatsDesc = AddStaticTextBox(WindowHandle, wA, "Feat Description", LABEL_UP,
                                       12, 526, 640, 276, (*ElementId)++, TRUE);
    wA += 2;
    
    Page->ChosenFeats = AddSingleSelListBox(WindowHandle, wA, "Your Feats", LABEL_UP,
                                            672, 48, 596, 766, (*ElementId)++, FALSE);
    wA += 2;
    
    Page->numWindows += 6;
}
#endif