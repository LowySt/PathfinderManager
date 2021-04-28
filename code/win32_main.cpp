#define LS_WINDOWS_IMPLEMENTATION
#include "lsWindows.h"
#undef LS_WINDOWS_IMPLEMENTATION

#define LS_CRT_IMPLEMENTATION
#include "lsCRT.h"
#undef LS_CRT_IMPLEMENTATION

#define LS_STRING_IMPLEMENTATION
#include "lsString.h"
#undef LS_STRING_IMPLEMENTATION

#define LS_BUFFER_IMPLEMENTATION
#include "lsBuffer.h"
#undef LS_BUFFER_IMPLEMENTATION

#include "lsGraphics.h"

#include "pcg.c"


#include "win32_widgets.h"
#include "Init.h"
#include "Class.h"
#include "PlayerChar.h"
#include "StateGlobals.h"

#include "Class.cpp"
#include "Skills.cpp"
#include "Feats.cpp"
#include "PlayerChar.cpp"
#include "Init.cpp"
#include "PCTab.cpp"
#include "FeatsTab.cpp"
#include "OnButton.cpp"

#include "win32_widgets.cpp"
#include "subEdit.cpp"

#define HidePage(page) for(u32 i = 0; i < page->numWindows; i++) { \
ShowWindow(page->WindowsArray[i], SW_HIDE); }

#define ShowPage(page) for(u32 i = 0; i < page->numWindows; i++) { \
ShowWindow(page->WindowsArray[i], SW_SHOW); }

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

string getText(HWND hwnd)
{
    u32 textLen = GetWindowTextLengthA(hwnd);
    char buffer[32] = {};
    GetWindowTextA(hwnd, buffer, textLen + 1);
    
    string Result = ls_strInit(buffer);
    
    return Result;
}

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

LRESULT WindowProc(HWND h, UINT msg, WPARAM w, LPARAM l)
{
    LRESULT Result = 0;
    
    InitPage  *Init = State.Init;
    FeatsPage *Feats = State.Feats;
    PCPage    *PC   = State.PC;
    
    switch (msg)
    {
        case WM_NCLBUTTONDOWN:
        {
            State.isDragging = TRUE;
            State.prevMousePos = *((POINTS *)&l);
            
            SetCapture(h);
            //TODO:NOTE: Page says to return 0, but DO i HAVE to?
            
        } break;
        
        case WM_LBUTTONUP:
        {
            State.isDragging = FALSE;
            BOOL success = ReleaseCapture();
        } break;
        
        case WM_MOUSEMOVE:
        {
            if(State.isDragging) {
                POINTS currMouseClient = *((POINTS *)&l);
                POINT currMouse = {currMouseClient.x, currMouseClient.y};
                BOOL success = ClientToScreen(h, &currMouse);
                
                SHORT newX = State.prevMousePos.x - currMouse.x;
                SHORT newY = State.prevMousePos.y - currMouse.y;
                
                SHORT newWinX = State.currWindowPos.x - newX;
                SHORT newWinY = State.currWindowPos.y - newY;
                
#if 0
                ls_printf("CurrMousePos: (%d, %d)\n"
                          "PrevMousePos: (%d, %d)\n"
                          "DragState:\n\t newXY: (%d, %d)\n\t newWinXY: (%d, %d)\n\t currWinPos: (%d, %d)\n",
                          currMouse.x, currMouse.y,
                          State.prevMousePos.x, State.prevMousePos.y, newX, newY, newWinX, newWinY,
                          State.currWindowPos.x, State.currWindowPos.y);
#endif
                
                State.currWindowPos = { newWinX, newWinY };
                State.prevMousePos  = POINTS { (SHORT)currMouse.x, (SHORT)currMouse.y};
                //State.isDragging = FALSE;
                
                SetWindowPos(h, 0, newWinX, newWinY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            }
            //TODO:NOTE: Page says to return 0, but DO i HAVE to?
        } break;
        
        case WM_KEYDOWN:
        {
            switch(w)
            {
                case VK_ESCAPE:
                {
                    ExitProcess(0);
                } break;
            }
        } break;
        
        case WM_DESTROY:
        {
            ExitProcess(0);
        } break;
        
        case WM_MEASUREITEM:
        {
            MEASUREITEMSTRUCT *item = (MEASUREITEMSTRUCT *)l;
            
            if(item->CtlType == ODT_MENU) {
                
                u32 len = ls_len((char *)item->itemData);
                SIZE size = {};
                BOOL success = GetTextExtentPoint32A(GetDC(h), (LPCSTR)item->itemData, len, &size);
                
                //NOTE: Used to center Text inside the rect.
                u32 marginX = 0;
                u32 marginY = 0;
                
                item->itemWidth  = size.cx + (2*marginX);
                item->itemHeight = size.cy + (2*marginY);
            }
            
        } break;
        
        case WM_DRAWITEM:
        {
            //NOTE: Only owner drawn right now are static (labels)
            DRAWITEMSTRUCT *item = (DRAWITEMSTRUCT *)l;
            
            if(item->CtlType == ODT_STATIC)
            {
                //NOTE: Why does this work on the Labels, but it works differently on the
                // read only edit boxes, for which I am using the CTLCOLOR message?
                FillRect(item->hDC, &item->rcItem, appBkgBrush);
                
                SetBkColor(item->hDC, appBkgRGB);
                SetTextColor(item->hDC, whiteRGB);
                char text[32] = {};
                int len = SendMessageA(item->hwndItem, WM_GETTEXT, 32, (LPARAM)text);
                TextOutA(item->hDC, item->rcItem.left, item->rcItem.top, text, len);
            }
            
            if(item->CtlType == ODT_MENU)
            {
                POINT cursorPos = {};
                GetCursorPos(&cursorPos);
                
                RECT screenRect = {};
                GetMenuItemRect(h, MenuBar, 0, &screenRect);
                
                HBRUSH brush      = appBkgBrush;
                COLORREF brushRGB = appBkgRGB;
                if((cursorPos.x > screenRect.left) && (cursorPos.x < screenRect.right) &&
                   (cursorPos.y < screenRect.bottom) && (cursorPos.y > screenRect.top))
                { brush = menuBkgBrush; brushRGB = menuBkgRGB; }
                
                FillRect(item->hDC, &item->rcItem, brush);
                
                SetBkColor(item->hDC, brushRGB);
                SetTextColor(item->hDC, whiteRGB);
                
                u32 marginX = 8, marginY = 2;
                TextOutA(item->hDC, item->rcItem.left + marginX, item->rcItem.top + marginY, 
                         (LPCSTR)item->itemData, 4);
            }
            
            if(item->CtlType == ODT_COMBOBOX)
            {
                FillRect(item->hDC, &item->rcItem, controlBkgBrush);
                
                SetBkColor(item->hDC, controlBkgRGB);
                SetTextColor(item->hDC, whiteRGB);
                
                u32 count = SendMessageA(item->hwndItem, CB_GETCOUNT, 0, 0);
                
                //for(u32 i = 0; i < count; i++)
                //{
                char text[32] = {};
                SendMessageA(item->hwndItem, CB_GETLBTEXT, item->itemID, (LPARAM)text);
                u32 textLen = SendMessageA(item->hwndItem, CB_GETLBTEXTLEN , item->itemID, 0);
                
                TextOutA(item->hDC, item->rcItem.left, item->rcItem.top, (LPCSTR)text, textLen);
                //}
            }
            
            if(item->CtlType == ODT_LISTBOX)
            {
                u32 count = SendMessageA(item->hwndItem, LB_GETCOUNT, 0, 0);
                u32 textLen = SendMessageA(item->hwndItem, LB_GETTEXTLEN , item->itemID, 0);
                
                char *text = (char *)ls_alloc(sizeof(char)*(textLen + 1));
                SendMessageA(item->hwndItem, LB_GETTEXT, item->itemID, (LPARAM)text);
                
                TextOutA(item->hDC, item->rcItem.left, item->rcItem.top, (LPCSTR)text, textLen);
                ls_free(text);
            }
            
            if(item->CtlType == ODT_BUTTON)
            {
                int textLen = GetWindowTextLengthA(item->hwndItem) + 1; //Add the null
                char *text = (char *)ls_alloc(sizeof(char)*textLen);
                
                GetWindowTextA(item->hwndItem, text, textLen);
                
                TextOutA(item->hDC, item->rcItem.left, item->rcItem.top, (LPCSTR)text, textLen);
                ls_free(text);
            }
            
        } break;
        
        case WM_CTLCOLORBTN:
        {
            HDC buttonHDC   = (HDC)w;
            
            SetBkColor(buttonHDC, RGB(0x56, 0x56, 0x56));
            SetTextColor(buttonHDC, RGB(255, 255, 255));
            
            return (LRESULT)controlBkgBrush;
        } break;
        
        case WM_CTLCOLORLISTBOX:
        {
            HDC listboxHDC = (HDC)w;
            
            SetBkColor(listboxHDC, RGB(0x56, 0x56, 0x56));
            SetTextColor(listboxHDC, RGB(255, 255, 255));
            
            return (LRESULT)controlBkgBrush;
        } break;
        
        case WM_CTLCOLOREDIT:
        {
            HDC editHDC = (HDC)w;
            
            SetBkColor(editHDC, RGB(0x56, 0x56, 0x56));
            SetTextColor(editHDC, RGB(255, 255, 255));
            
            return (LRESULT)controlBkgBrush;
        } break;
        
        case WM_CTLCOLORSTATIC:
        {
            HDC editHDC = (HDC)w;
            
            SetBkColor(editHDC, RGB(0x56, 0x56, 0x56));
            SetTextColor(editHDC, RGB(255, 255, 255));
            
            return (LRESULT)controlBkgBrush;
        };
        
        case WM_MENUCOMMAND:
        {
            u32 itemIdx = w;
            
            switch(itemIdx)
            {
                case FILE_MENU_SAVE_IDX:
                {
                    //First update Ability Scores then serialize.
                    saveAS();
                    SerializePC(&pc);
                } break;
                
                case FILE_MENU_LOAD_IDX:
                {
                    LoadPC(&pc);
                    
                    //TODO: Stop allocating and deallocating mem for stupid C Strings
                    char *cStr = ls_strToCStr(pc.Name);
                    Edit_SetText(State.PC->Name->box, cStr);
                    ls_free(cStr);
                    
                    cStr = ls_strToCStr(pc.Player);
                    Edit_SetText(State.PC->Player->box, cStr);
                    ls_free(cStr);
                    
                    ComboBox_SelectString(State.PC->Race->box, -1, Races[pc.Race]);
                    ComboBox_SelectString(State.PC->Class->box, -1, Classes[pc.Class]);
                    
                    State.PC->wasClassChosen = TRUE;
                    
                    loadAS();
                    
                    char **TraitsList;
                    u32 arrSize = 0;
                    
                    TraitsList = (char **)RaceTraits[pc.Race];
                    arrSize = RaceTraitsArraySize[pc.Race];
                    
                    ListBox_ResetContent(State.PC->RacialTraits->box);
                    AddAllListBoxItems(State.PC->RacialTraits->box, TraitsList, arrSize);
                    
                    //TODO: Check why lvl is not being update at loading
                    cStr = ls_itoa(pc.lvl);
                    Edit_SetText(State.PC->currLevel->box, cStr);
                    ls_free(cStr);
                    
                    Edit_SetText(State.PC->BaseAttackBonus->box,
                                 ClassBABString[pc.Class][0][pc.lvl]);
                    
                    cStr = ls_itoa(pc.xp);
                    Edit_SetText(State.PC->currXP->box, cStr);
                    ls_free(cStr);
                    
                    State.PC->xpIdx = pc.xpCurve;
                    ComboBox_SelectString(State.PC->XPCurve->box, -1,
                                          XPCurvesString[pc.xpCurve]);
                    
                    char **xpCurve = (char **)XPCurvesArr[State.PC->xpIdx];
                    Edit_SetText(State.PC->nextLevelXP->box, xpCurve[pc.lvl]);
                    
                    UpdateSavingThrows();
                    
                } break;
            }
            
        } break;
        
        case WM_COMMAND:
        {
            u32 commandID = LOWORD(w);
            u32 notificationCode = HIWORD(w);
            HWND handle = (HWND)l;
            
            //TODO: NOTE: ASBonus doesn't check Out Of Bounds
            switch(notificationCode)
            {
                case EN_KILLFOCUS:
                {
                    if(commandID == State.PC->Name->id)
                    {
                        string newName = getText(handle);
                        
                        if(pc.Name.data)
                        { ls_strFree(&pc.Name); }
                        
                        pc.Name = newName;
                    }
                    
                    else if(commandID == State.PC->Player->id)
                    {
                        string newPlayer = getText(handle);
                        
                        if(pc.Player.data)
                        { ls_strFree(&pc.Player); }
                        
                        pc.Player = newPlayer;
                    }
                    
                    else if(commandID == State.PC->Scores->Box[ABILITY_STR]->id)
                    {
                        string s = getText(handle);
                        u32 v = ls_stoi(s);
                        pc.AbilityScores[ABILITY_STR] = v;
                        
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_STR]->box, getASBonusStr(v));
                        UpdateSavingThrows();
                        
                        ls_strFree(&s);
                    }
                    
                    else if(commandID == State.PC->Scores->Box[ABILITY_DEX]->id)
                    {
                        string s = getText(handle);
                        u32 v = ls_stoi(s);
                        pc.AbilityScores[ABILITY_DEX] = v;
                        
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_DEX]->box, getASBonusStr(v));
                        UpdateSavingThrows();
                        
                        ls_strFree(&s);
                    }
                    
                    else if(commandID == State.PC->Scores->Box[ABILITY_CON]->id)
                    {
                        string s = getText(handle);
                        u32 v = ls_stoi(s);
                        pc.AbilityScores[ABILITY_CON] = v;
                        
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_CON]->box, getASBonusStr(v));
                        UpdateSavingThrows();
                        
                        ls_strFree(&s);
                    }
                    
                    else if(commandID == State.PC->Scores->Box[ABILITY_INT]->id)
                    {
                        string s = getText(handle);
                        u32 v = ls_stoi(s);
                        pc.AbilityScores[ABILITY_INT] = v;
                        
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_INT]->box, getASBonusStr(v));
                        UpdateSavingThrows();
                        
                        ls_strFree(&s);
                    }
                    
                    else if(commandID == State.PC->Scores->Box[ABILITY_WIS]->id)
                    {
                        string s = getText(handle);
                        u32 v = ls_stoi(s);
                        pc.AbilityScores[ABILITY_WIS] = v;
                        
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_WIS]->box, getASBonusStr(v));
                        UpdateSavingThrows();
                        
                        ls_strFree(&s);
                    }
                    
                    else if(commandID == State.PC->Scores->Box[ABILITY_CHA]->id)
                    {
                        string s = getText(handle);
                        u32 v = ls_stoi(s);
                        pc.AbilityScores[ABILITY_CHA] = v;
                        
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_CHA]->box, getASBonusStr(v));
                        UpdateSavingThrows();
                        
                        ls_strFree(&s);
                    }
                    
                    else if(commandID == State.PC->currLevel->id)
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
                    else if(commandID == State.PC->currXP->id)
                    {
                        string s = getText(handle);
                        u32 currXP = ls_stoi(s);
                        
                        pc.xp = currXP;
                        
                        ls_strFree(&s);
                    }
                } break;
                
                case CBN_SELENDOK:
                {
                    if(commandID == State.PC->Race->id)
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
                    }
                    else if(commandID == State.PC->Class->id)
                    {
                        string s = getText(handle);
                        
                        GameClass r = getClassFromString(s);
                        pc.Class = r;
                        
                        UpdateSavingThrows();
                        
                        ls_strFree(&s);
                    }
                    else if(commandID == State.PC->XPCurve->id)
                    {
                        State.PC->xpIdx = (XPCurveIdx)ComboBox_GetCurSel(handle);
                        pc.xpCurve = State.PC->xpIdx;
                        char **xpCurve = (char **)XPCurvesArr[State.PC->xpIdx];
                        
                        string s = getText(State.PC->currLevel->box);
                        u32 currLevel = ls_stoi(s);
                        
                        Edit_SetText(State.PC->nextLevelXP->box, xpCurve[currLevel]);
                        
                        ls_strFree(&s);
                    }
                    else if(commandID == State.Init->Mobs->id)
                    {
                        s32 idx = ComboBox_GetCurSel(handle);
                        Assert(idx != CB_ERR);
                        State.Init->VisibleMobs  = idx;
                        State.Init->VisibleOrder = PARTY_NUM + State.Init->VisibleAllies + idx;
                        
                        HideInitField(State.Init->MobFields, MOB_NUM);
                        ShowInitField(State.Init->MobFields, idx, MOB_NUM);
                        
                        HideOrder(State.Init->Order, ORDER_NUM);
                        ShowOrder(State.Init->Order, State.Init->VisibleOrder);
                    }
                    else if(commandID == Init->Allies->id)
                    {
                        s32 idx = ComboBox_GetCurSel(handle);
                        Assert(idx != CB_ERR);
                        Init->VisibleAllies = idx;
                        Init->VisibleOrder = PARTY_NUM + Init->VisibleMobs + idx;
                        
                        HideInitField(Init->AllyFields, ALLY_NUM);
                        ShowInitField(Init->AllyFields, idx, ALLY_NUM);
                        
                        HideOrder(Init->Order, ORDER_NUM);
                        ShowOrder(Init->Order, State.Init->VisibleOrder);
                    }
                    else if(commandID == Init->EncounterSel->id)
                    {
                        s32 idx = ComboBox_GetCurSel(handle);
                        Assert(idx != CB_ERR);
                        
                        Encounter *Curr = &State.encounters.Enc[idx];
                        Init->VisibleMobs   = Curr->numMobs;
                        Init->VisibleAllies = Curr->numAllies;
                        Init->VisibleOrder  = PARTY_NUM + Curr->numMobs + Curr->numAllies;
                        
                        ComboBox_SetCurSel(Init->Mobs->box, Curr->numMobs);
                        ComboBox_SetCurSel(Init->Allies->box, Curr->numAllies);
                        
                        HideInitField(Init->MobFields, MOB_NUM);
                        ShowInitField(Init->MobFields, Curr->numMobs, MOB_NUM);
                        
                        HideInitField(Init->AllyFields, ALLY_NUM);
                        ShowInitField(Init->AllyFields, Curr->numAllies, ALLY_NUM);
                        
                        HideOrder(Init->Order, ORDER_NUM);
                        ShowOrder(Init->Order, Init->VisibleOrder);
                        
                        for(u32 i = 0; i < Init->VisibleMobs; i++)
                        {
                            Edit_SetText(Init->MobFields[i].Name->box, Curr->mobNames[i]);
                            
                            char bonus[8] = {};
                            ls_itoa_t(Curr->mobBonus[i], bonus, 8);
                            
                            Edit_SetText(Init->MobFields[i].Bonus->box, bonus);
                        }
                        
                        for(u32 i = 0; i < Init->VisibleAllies; i++)
                        {
                            Edit_SetText(Init->AllyFields[i].Name->box, Curr->allyNames[i]);
                            
                            char bonus[8] = {};
                            ls_itoa_t(Curr->allyBonus[i], bonus, 8);
                            
                            Edit_SetText(Init->AllyFields[i].Bonus->box, bonus);
                        }
                    }
                    
                } break;
                
                case LBN_SELCHANGE:
                {
                    u32 index = ListBox_GetCurSel(handle);
                    if(index > ArraySize(FeatsDesc))
                    {
                        ls_printf("Not implemented description of Feat n°%d\n", index);
                        Assert(FALSE);
                    }
                    char *Desc = (char *)FeatsDesc[index];
                    
                    Edit_SetText(State.Feats->FeatsDesc->box, Desc);
                    
                } break;
                
                case LBN_DBLCLK:
                {
                    if(commandID == State.Feats->Feats->id)
                    {
                        u32 index = ListBox_GetCurSel(handle);
                        if(index > ArraySize(FeatsDesc))
                        {
                            ls_printf("Not implemented description of Feat n°%d\n", index);
                            Assert(FALSE);
                        }
                        
                        if(ArrayContains(State.Feats->ChosenFeatsIndices, index) == TRUE)
                        { break; }
                        
                        ListBox_AddString(State.Feats->ChosenFeats->box, FeatNames[index]);
                        State.Feats->ChosenFeatsIndices[State.Feats->usedFeats] = index;
                        State.Feats->usedFeats += 1;
                    }
                    else if(commandID == State.Feats->ChosenFeats->id)
                    {
                        u32 index = ListBox_GetCurSel(handle);
                        
                        ListBox_DeleteString(handle, index);
                        
                        ArrayRemove(State.Feats->ChosenFeatsIndices, index);
                        State.Feats->usedFeats -= 1;
                    }
                    
                } break;
                
                case BN_CLICKED: { OnButton(commandID, notificationCode, handle); } break;
            }
            
        } break;
        
        default:
        {
            return DefWindowProcA(h, msg, w, l);
        }
    }
    
    return Result;
}

void RegisterWindow()
{
    
    u32 prop = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    
    WNDCLASSA WindowClass = { 0 };
    WindowClass.style = prop;
    WindowClass.lpfnWndProc = WindowProc;
    WindowClass.hInstance = MainInstance;
    WindowClass.hbrBackground = appBkgBrush; //RGB(0x38, 0x38, 0x38);
    WindowClass.lpszClassName = "WndClass";
    
    if (!RegisterClassA(&WindowClass))
    {
        DWORD Error = GetLastError();
        ls_printf("When Registering WindowClass in Win32_SetupScreen got error: %d", Error);
    }
}

HWND CreateWindow(HMENU MenuBar)
{
    u32 style = LS_VISIBLE | LS_THIN_BORDER | LS_POPUP; // | LS_OVERLAPPEDWINDOW;
    BOOL Result;
    
    SubMenu = CreateMenu();
    Result = AppendMenuA(SubMenu, MF_STRING, 0, "Save");
    Result = AppendMenuA(SubMenu, MF_STRING, 1, "Load");
    
    MENUITEMINFOA FileMenuInfo = {};
    FileMenuInfo.cbSize = sizeof(MENUITEMINFOA);
    
    FileMenuInfo.fMask = MIIM_SUBMENU | MIIM_DATA | MIIM_FTYPE | MIIM_ID;
    FileMenuInfo.fType = MFT_OWNERDRAW;
    FileMenuInfo.wID   = MENU_FILE_ITEM_ID;
    
    FileMenuInfo.dwItemData = (ULONG_PTR)"File";
    FileMenuInfo.hSubMenu = SubMenu;
    FileMenuInfo.cch = 5;
    Result = InsertMenuItemA(MenuBar, 0, TRUE, &FileMenuInfo);
    
    MENUINFO menuInfo = {};
    menuInfo.cbSize  = sizeof(MENUINFO);
    menuInfo.fMask   = MIM_APPLYTOSUBMENUS | MIM_BACKGROUND;
    menuInfo.hbrBack = appBkgBrush;  //RGB(0x38, 0x38, 0x38);
    
    //NOTE:TODO: Hardcoded!!
    HWND WindowHandle;
    if ((WindowHandle = CreateWindowExA(0, "WndClass",
                                        "Name", style,
                                        300, 50, //CW_USEDEFAULT, CW_USEDEFAULT,
                                        1280, 840,//1350, 900,
                                        0, MenuBar, MainInstance, 0)) == nullptr)
    {
        DWORD Error = GetLastError();
        ls_printf("When Retrieving a WindowHandle in Win32_SetupScreen got error: %d", Error);
    }
    
    State.currWindowPos = { 300, 50 }; //NOTE:TODO: Hardcoded!!
    
    return WindowHandle;
}

int WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR cmdLine, int nCmdShow)
{
    MainInstance = hInst;
    
#ifdef __GNUG__
    u64 rand_init_state = 2349879125314230;
    u64 rand_init_seq = 9827346259348;
#else
    u64 rand_init_state = 0;
    u64 rand_init_seq = 0;
    _rdseed64_step(&rand_init_state);
    _rdseed64_step(&rand_init_seq);
#endif
    
    pcg32_seed(&pcg32_global, rand_init_state, rand_init_seq);
    
    RegisterWindow();
    
    MenuBar = CreateMenu();
    MENUINFO Info = {};
    Info.cbSize = sizeof(MENUINFO);
    Info.fMask = MIM_APPLYTOSUBMENUS | MIM_STYLE | MIM_BACKGROUND;
    Info.dwStyle = MNS_NOTIFYBYPOS;
    Info.hbrBack = appBkgBrush;
    SetMenuInfo(MenuBar, &Info);
    
    MainWindow = CreateWindow(MenuBar);
    
    State.isInitialized = FALSE;
    State.PC    = (PCPage *)ls_alloc(sizeof(PCPage));
    State.Feats = (FeatsPage *)ls_alloc(sizeof(FeatsPage));
    State.Init  = (InitPage *)ls_alloc(sizeof(InitPage));
    
    u64 ElementId = 0;
    
    for(size_t i = 0; i < ArraySize(State.Feats->ChosenFeatsIndices); i++)
    { State.Feats->ChosenFeatsIndices[i] = u32(-1); }
    
    //
    // Initialize StateData
    //
    
    char fullPathBuff[128] = {};
    u32 len = ls_getFullPathName((char *)"State.bin", fullPathBuff, 128);
    
    if(ls_fileExists(fullPathBuff) == TRUE)
    {
        ls_readFile((char *)"State.bin", (char **)&State.StateData, 0);
        State.encounters.data = (u8 *)State.StateData + encounterOffset;
        State.encounters.isInitialized = TRUE;
    }
    //
    // Create Tabs
    //
    
    HWND TabControl = CreateWindowExA(0, WC_TABCONTROL, "",
                                      WS_CHILD | WS_VISIBLE | WS_BORDER,
                                      0, 0, 300, 20,
                                      MainWindow, (HMENU)99, MainInstance, 0);
    State.PC->TabControl = TabControl;
    TCITEMA TabItem = {};
    TabItem.mask = TCIF_TEXT;
    TabItem.pszText = (char *)"PC";
    TabItem.cchTextMax = 3;
    TabItem.iImage = -1;
    Tab_InsertItem(TabControl, 0, &TabItem);
    
    TCITEMA TabItem2 = {};
    TabItem2.mask = TCIF_TEXT;
    TabItem2.pszText = (char *)"Feats";
    TabItem2.cchTextMax = 6;
    TabItem2.iImage = -1;
    Tab_InsertItem(TabControl, 1, &TabItem2);
    
    TCITEMA TabItem3 = {};
    TabItem3.mask = TCIF_TEXT;
    TabItem3.pszText = (char *)"Init";
    TabItem3.cchTextMax = 4;
    TabItem3.iImage = -1;
    Tab_InsertItem(TabControl, 2, &TabItem3);
    
    DrawPCTab(MainWindow, &ElementId);
    DrawFeatsTab(MainWindow, &ElementId);
    
    DrawInitTab(MainWindow, &ElementId);
    
    State.isInitialized = TRUE;
    
    HidePage(State.Feats);
    HidePage(State.PC);
    ShowPage(State.Init);
    
    //Initialization of Init Page
    HideInitElem(State.Init);
    ShowOrder(State.Init->Order, PARTY_NUM);
    HideElem(State.Init->Next->box);          
    
    TabCtrl_SetCurSel(TabControl, 2);
    
    b32 Running = TRUE;
    u32 oldPageIdx = 2;
    u32 newPageIdx = 0;
    
    while(Running)
    {
        // Process Input
        MSG Msg;
        
        
        newPageIdx = TabCtrl_GetCurSel(TabControl);
        
        if(newPageIdx != oldPageIdx)
        {
            switch(newPageIdx)
            {
                case 0:
                {
                    HidePage(State.Feats);
                    HidePage(State.Init);
                    ShowPage(State.PC);
                } break;
                
                case 1:
                {
                    HidePage(State.PC);
                    //HidePage(State.Init);
                    
                    for(u32 i = 0; i < State.Init->numWindows; i++) 
                    {
                        ShowWindow(State.Init->WindowsArray[i], SW_HIDE);
                    }
                    
                    ShowPage(State.Feats);
                } break;
                
                case 2:
                {
                    HidePage(State.PC);
                    HidePage(State.Feats);
                    ShowPage(State.Init);
                    HideInitElem(State.Init);
                    
                } break;
            }
        }
        
        while (PeekMessageA(&Msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
        }
        
        oldPageIdx = newPageIdx;
    }
    
    return 0;
}
