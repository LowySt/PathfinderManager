
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
