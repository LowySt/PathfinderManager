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


#include "Init.h"
#include "Class.h"
#include "PlayerChar.h"
#include "StateGlobals.h"

#include "Class.cpp"
#include "Skills.cpp"
#include "Feats.cpp"
#include "PlayerChar.cpp"
#include "Init.cpp"
#include "OnButton.cpp"

#include "win32_widgets.cpp"

/// WindowProc Subclass for Edit Controls (Only done on EditBox)
LRESULT subEditProc(HWND h, UINT msg, WPARAM w, LPARAM l)
{
    InitPage *Init = State.Init;
    
    switch (msg)
    {
#if 0
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(h, &ps);
            
            RECT itemRect = {};
            BOOL res = GetWindowRect(h, &itemRect); //Screen Space
            u32  itemWidth  = itemRect.right - itemRect.left;
            u32  itemHeight = itemRect.bottom - itemRect.top;
            
            RECT cRect = {};
            res = GetClientRect(h, &cRect);
            
            //TODO:@msdn InflateRect()
            cRect = {cRect.left+10, cRect.top+10, cRect.right-10, cRect.bottom+10};
            
            
            //NOTETODO: Testing highlighting of *Selected* Init Fields.
            InitField *field = getInitByHWND(h);
            if(field != 0x0)
            {
                if(field->isSelected == TRUE)
                {
                    //TODO:@msdn InflateRect()
                    RECT Rect = {cRect.left-30, cRect.top-30, cRect.right+30, cRect.bottom+30};
                    FillRect(hDC, &Rect, testColor);
                }
            }
            
            //NOTE: Why does this work on the Labels, but it works differently on the
            // read only edit boxes, for which I am using the CTLCOLOR message?
#if 1
            FillRect(hDC, &cRect, controlBkgBrush);
            
            SetBkColor(hDC, controlBkgRGB);
            SetTextColor(hDC, RGB(255, 255, 255));
            char text[32] = {};
            int len = SendMessageA(h, WM_GETTEXT, 32, (LPARAM)text);
            TextOutA(hDC, cRect.left, cRect.top, text, len);
            
            EndPaint(h, &ps);
#endif
            //NOTE: To Finish drawing everything else.
            //return DefWindowProcA(h, msg, w, l);
            return CallWindowProcA(mainWinProc, h, msg, w, l);
            
        } break;
#endif
        
        
        case WM_RBUTTONUP:
        {
            InitField *field = getInitByHWND(h);
            if(field != 0x0)
            {
                field->isSelected = TRUE;
            }
        } break;
        
        case WM_CHAR:
        {
            if(w == VK_RETURN) { return 0; } //NOTE: This stupid shit stops the Beeping when pressing enter
            
            //NOTE: Still needs to call the default proc to actually have characters display
            return CallWindowProcA(mainWinProc, h, msg, w, l);
        } break;
        
        case WM_SETFOCUS:
        {
            //NOTE: We shouldn't touch the edit box if it's set to read only!
            DWORD dwStyle = (DWORD)GetWindowLongA(h, GWL_STYLE);
            if((dwStyle & ES_READONLY) != 0) { break; }
            
            b32 isValid = FALSE;
            for( u32 i = 0; i < Init->VisibleMobs; i++) {
                if(h == Init->MobFields[i].Name->box)  { isValid = TRUE; break; }
                if(h == Init->MobFields[i].Bonus->box) { isValid = TRUE; break; }
                if(h == Init->MobFields[i].Final->box) { isValid = TRUE; break; }
            }
            
            if(!isValid) { 
                for( u32 i = 0; i < Init->VisibleAllies; i++) {
                    if(h == Init->AllyFields[i].Name->box)  { isValid = TRUE; break; }
                    if(h == Init->AllyFields[i].Bonus->box) { isValid = TRUE; break; }
                    if(h == Init->AllyFields[i].Final->box) { isValid = TRUE; break; }
                }
            }
            
            if(!isValid) {
                for( u32 i = 0; i < PARTY_NUM; i++) {
                    if(h == Init->PlayerFields[i].Bonus->box) { isValid = TRUE; break; }
                }
            }
            
            if(isValid) { Edit_SetText(h, ""); }
            
            return CallWindowProcA(mainWinProc, h, msg, w, l);
        } break;
        
        case WM_KEYDOWN:
        {
            switch(w)
            {
                case VK_RETURN:
                {
                    for(u32 i = 0; i < Init->VisibleMobs; i++) {
                        if(h == Init->MobFields[i].Name->box) { 
                            
                            if(i < Init->VisibleMobs - 1) { SetFocus(Init->MobFields[i+1].Name->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                        
                        if(h == Init->MobFields[i].Bonus->box) { 
                            
                            if(i < Init->VisibleMobs - 1) { SetFocus(Init->MobFields[i+1].Bonus->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                        
                        if(h == Init->MobFields[i].Final->box) { 
                            
                            if(i < Init->VisibleMobs - 1) { SetFocus(Init->MobFields[i+1].Final->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                    }
                    
                    for(u32 i = 0; i < PARTY_NUM; i++) {
                        if(h == Init->PlayerFields[i].Bonus->box) {
                            if(i < PARTY_NUM - 1) { SetFocus(Init->PlayerFields[i+1].Bonus->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                    }
                    
                    for(u32 i = 0; i < Init->VisibleAllies; i++) {
                        if(h == Init->AllyFields[i].Name->box) { 
                            if(i < Init->VisibleAllies - 1) { SetFocus(Init->AllyFields[i+1].Name->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                        
                        if(h == Init->AllyFields[i].Bonus->box) { 
                            if(i < Init->VisibleAllies - 1) { SetFocus(Init->AllyFields[i+1].Bonus->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                        
                        if(h == Init->AllyFields[i].Final->box) { 
                            if(i < Init->VisibleAllies - 1) { SetFocus(Init->AllyFields[i+1].Final->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                    }
                    
                    for(u32 i = 0; i < Init->VisibleOrder; i++) {
                        
                        if(h == Init->Order[i].Pos->box) {
                            int len = Edit_GetTextLength(h);
                            char buff[8] = {};
                            Edit_GetText(h, buff, 8);
                            
                            s32 newPosition = ls_atoi(buff, len);
                            
                            if((newPosition >= Init->VisibleOrder) ||
                               (newPosition == i) || (newPosition < 0)) { 
                                return CallWindowProcA(mainWinProc, h, msg, w, l); 
                            }
                            
                            OrderField *oldOrder = &Init->Order[i];
                            
                            u32 tmpFieldID = oldOrder->fieldId;
                            b32 tmpIsMob   = oldOrder->isMob;
                            b32 tmpIsParty = oldOrder->isParty;
                            
                            char tmpName[32] = {};
                            Edit_GetText(oldOrder->Field->box, tmpName, 32);
                            
                            if(newPosition > i)
                            {
                                for(u32 j = i; j <= newPosition; j++)
                                {
                                    char name[32] = {};
                                    Edit_GetText(Init->Order[j+1].Field->box, name, 32);
                                    
                                    Init->Order[j].fieldId = Init->Order[j+1].fieldId;
                                    Init->Order[j].isMob   = Init->Order[j+1].isMob;
                                    Init->Order[j].isParty = Init->Order[j+1].isParty;
                                    
                                    Edit_SetText(Init->Order[j].Field->box, name);
                                }
                            }
                            
                            if(newPosition < i)
                            {
                                //NOTE: This was changed from j >= newPosition, 
                                // because of j-1 going out of bounds when newPosition is 0.
                                for(u32 j = i; j > newPosition; j--)
                                {
                                    char name[32] = {};
                                    Edit_GetText(Init->Order[j-1].Field->box, name, 32);
                                    
                                    Init->Order[j].fieldId = Init->Order[j-1].fieldId;
                                    Init->Order[j].isMob   = Init->Order[j-1].isMob;
                                    Init->Order[j].isParty = Init->Order[j-1].isParty;
                                    
                                    
                                    Edit_SetText(Init->Order[j].Field->box, name);
                                }
                            }
                            
                            Init->Order[newPosition].fieldId = tmpFieldID;
                            Init->Order[newPosition].isMob   = tmpIsMob;
                            Init->Order[newPosition].isParty = tmpIsParty;
                            
                            Edit_SetText(Init->Order[newPosition].Field->box, tmpName);
                            
                            //NOTE: Reset the position to actual ordinal
                            char ith[8] = {};
                            ls_itoa_t(i, ith, 8);
                            Edit_SetText(h, ith);
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                    }
                    
                    return CallWindowProcA(mainWinProc, h, msg, w, l); 
                } break;
            }
        } break;
        
        default: { 
            return CallWindowProcA(mainWinProc, h, msg, w, l); 
        }
    }
    return 0;
}




#define AddAllComboBoxItems(wnd, s, n) { for(size_t i = 0; i < n; i++) \
{ComboBox_AddString(wnd, s[i]); } }

#define AddAllListBoxItems(wnd, s, n)  { for(size_t i = 0; i < n; i++) \
{ListBox_AddString(wnd, s[i]); } }

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

inline void SwitchInitAddToSelect(InitField *f, u32 idx, s32 max)
{
    if(idx < max) { 
        ShowWindow(f[idx].New.Add->box, SW_HIDE);
        
        ShowWindow(f[idx].New.Name->box, SW_SHOW);
        ShowWindow(f[idx].New.Bonus->box, SW_SHOW);
        ShowWindow(f[idx].New.Ok->box, SW_SHOW);
    }
}

inline void HideInitFieldAdd(InitField *f, u32 idx, s32 max)
{
    if(idx < max) { ShowWindow(f[idx].New.Add->box, SW_HIDE); }
}

inline void HideInitField(InitField *f, s32 n)
{
    for(u32 i = 0; i < n; i++)
    {
        if(f[i].Name != 0) //NOTE: Assuming this is not a party member
        { 
            ShowWindow(f[i].Name->box, SW_HIDE);
            ShowWindow(f[i].Final->box, SW_HIDE);
            ShowWindow(f[i].Bonus->box, SW_HIDE);
            
            ShowWindow(f[i].New.Name->box, SW_HIDE);
            ShowWindow(f[i].New.Bonus->box, SW_HIDE);
            ShowWindow(f[i].New.Add->box, SW_HIDE);
            ShowWindow(f[i].New.Ok->box, SW_HIDE);
        }
        else {
            ShowWindow(f[i].Bonus->box, SW_HIDE);
            ShowWindow(f[i].Bonus->label, SW_HIDE);
        }
    }
}

inline void HideOrder(OrderField *o, s32 n)
{
    for(u32 i = 0; i < n; i++)
    {
        ShowWindow(o[i].Field->box, SW_HIDE);
        ShowWindow(o[i].Pos->box, SW_HIDE);
        ShowWindow(o[i].Remove->box, SW_HIDE);
    }
}

inline void HideElem(HWND box) { ShowWindow(box, SW_HIDE); }
inline void ShowElem(HWND box) { ShowWindow(box, SW_SHOW); }

inline void HideInitElem(InitPage *p) {
    HideInitField(p->MobFields, MOB_NUM);
    HideInitField(p->AllyFields, ALLY_NUM);
    HideOrder(p->Order, ORDER_NUM);
}

inline void ShowInitFieldAdd(InitField *f, u32 idx, s32 max)
{
    if(idx < max) { ShowWindow(f[idx].New.Add->box, SW_SHOW); }
}

inline void ShowInitField(InitField *f, s32 n, s32 max)
{
    u32 i = 0; 
    for(; i < n; i++)
    {
        if(f[i].Name  != 0) //NOTE: Assuming this is not a party member
        { 
            ShowWindow(f[i].Name->box, SW_SHOW);
            ShowWindow(f[i].Final->box, SW_SHOW);
            ShowWindow(f[i].Bonus->box, SW_SHOW);
        }
        else {
            ShowWindow(f[i].Bonus->box, SW_SHOW);
            ShowWindow(f[i].Bonus->label, SW_SHOW);
        }
    }
    
    if(i < max) { ShowWindow(f[i].New.Add->box, SW_SHOW); }
}

inline void ShowOrder(OrderField *o, s32 n)
{
    for(u32 i = 0; i < n; i++)
    {
        ShowWindow(o[i].Field->box, SW_SHOW);
        ShowWindow(o[i].Pos->box, SW_SHOW);
        ShowWindow(o[i].Remove->box, SW_SHOW);
    }
}

inline void ShowInitElem(InitPage *p) {
    ShowInitField(p->MobFields, MOB_NUM, MOB_NUM);
    ShowInitField(p->AllyFields, ALLY_NUM, ALLY_NUM);
    ShowOrder(p->Order, ORDER_NUM);
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
            //TODO:NOTE: Page says to return 0, but DO i HAVE to?
            
        } break;
        
        case WM_NCLBUTTONUP:
        {
            State.isDragging = FALSE;
        } break;
        
        case WM_NCMOUSEMOVE:
        {
            if(State.isDragging) {
                POINTS currMouse = *((POINTS *)&l);
                
                SHORT newX = State.prevMousePos.x - currMouse.x;
                SHORT newY = State.prevMousePos.y - currMouse.y;
                
                SHORT newWinX = State.currWindowPos.x - newX;
                SHORT newWinY = State.currWindowPos.y - newY;
                
                State.currWindowPos = { newWinX, newWinY };
                State.prevMousePos  = currMouse;
                //State.isDragging = FALSE;
                
                SetWindowPos(h, 0, newWinX, newWinY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            }
            //TODO:NOTE: Page says to return 0, but DO i HAVE to?
        };
        
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
                
                item->itemWidth  = 50;
                item->itemHeight = 20;
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
                //TODO:@msdn InflateRect()
                RECT Rect = {item->rcItem.left, item->rcItem.top, item->rcItem.left+1400, item->rcItem.top+22};
                FillRect(item->hDC, &Rect, appBkgBrush);
                
                SetBkColor(item->hDC, appBkgRGB);
                SetTextColor(item->hDC, whiteRGB);
                TextOutA(item->hDC, item->rcItem.left, item->rcItem.top+5, (LPCSTR)item->itemData, 4);
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
        
        case WM_NOTIFY:
        {
            struct NMHDR {
                HWND hwndFrom;
                UINT idFrom;
                UINT code;
            };
            
            NMHDR *info = (NMHDR*)l;
            
            switch(info->code)
            {
                case TCN_SELCHANGING:
                {
                    int breakHere = 2;
                    
                } break;
                
                case TCN_SELCHANGE:
                {
                    int breakThere = 3;
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
    
    HMENU SubMenu = CreateMenu();
    Result = AppendMenuA(SubMenu, MF_STRING, 0, "Save");
    Result = AppendMenuA(SubMenu, MF_STRING, 1, "Load");
    
    MENUITEMINFOA FileMenuInfo = {};
    FileMenuInfo.cbSize = sizeof(MENUITEMINFOA);
    
    FileMenuInfo.fMask = MIIM_SUBMENU | MIIM_DATA | MIIM_FTYPE;
    FileMenuInfo.fType = MFT_OWNERDRAW;
    
    FileMenuInfo.dwItemData = (ULONG_PTR)"File";
    FileMenuInfo.hSubMenu = SubMenu;
    FileMenuInfo.cch = 5;
    Result = InsertMenuItemA(MenuBar, 0, TRUE, &FileMenuInfo);
    
    MENUINFO menuInfo = {};
    menuInfo.cbSize  = sizeof(MENUINFO);
    menuInfo.fMask   = MIM_APPLYTOSUBMENUS | MIM_BACKGROUND;
    menuInfo.hbrBack = appBkgBrush;  //RGB(0x38, 0x38, 0x38);
    
    // SetMenuInfo(MenuBar, &menuInfo); Works Shittily
    
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

ComboBox *FillEncounters(HWND h, HWND *wA, const char *label, s32 x, s32 y, u32 width, u32 height, u64 id)
{
    buffer buff = ls_bufferViewIntoPtr(State.encounters.data, KBytes(64));
    
    u32 numOfEncounters = ls_bufferReadDWord(&buff);
    State.encounters.numEncounters = numOfEncounters;
    
    char **encounterNames = (char **)ls_alloc(sizeof(char *) * numOfEncounters);
    for(u32 i = 0; i < numOfEncounters; i++) { encounterNames[i] = (char *)ls_alloc(sizeof(char) * 32); }
    
    u32 cursorStart = buff.cursor;
    for(u32 i = 0; i < numOfEncounters; i++)
    {
        Encounter *CurrEnc = &State.encounters.Enc[i];
        ls_bufferReadData(&buff, encounterNames[i], 32);
        
        u32 numOfMobs = ls_bufferReadDWord(&buff);
        CurrEnc->numMobs = numOfMobs;
        
        for(u32 j = 0; j < numOfMobs; j++)
        {
            char nameBuff[32] = {};
            ls_bufferReadData(&buff, nameBuff, 32);
            
            u32  mobBonus = ls_bufferReadDWord(&buff);
            
            ls_memcpy(nameBuff, CurrEnc->mobNames[j], 32);
            CurrEnc->mobBonus[j] = mobBonus;
        }
        
        if(numOfMobs < MOB_NUM) { 
            u32 paddingBytes = (MOB_NUM - numOfMobs) * sizeOfInitEntry;
            ls_bufferAdvanceCursor(&buff, paddingBytes);
        }
        
        u32 numOfAllies = ls_bufferReadDWord(&buff);
        CurrEnc->numAllies = numOfAllies;
        
        for(u32 j = 0; j < numOfAllies; j++)
        {
            char nameBuff[32] = {};
            ls_bufferReadData(&buff, nameBuff, 32);
            
            u32  allyBonus = ls_bufferReadDWord(&buff);
            
            ls_memcpy(nameBuff, CurrEnc->allyNames[j], 32);
            CurrEnc->allyBonus[j] = allyBonus;
        }
        
        if(numOfAllies < ALLY_NUM) { 
            u32 paddingBytes = (ALLY_NUM - numOfAllies) * sizeOfInitEntry;
            ls_bufferAdvanceCursor(&buff, paddingBytes);
        }
        u32 cursorEnd = buff.cursor;
        Assert(cursorEnd - cursorStart == sizeOfEncEntry);
        cursorStart = cursorEnd;
    }
    
    u32 stupidComboBoxNumberOfItemsWhichDeterminesHeight = numOfEncounters;
    if(numOfEncounters == 1)
    { stupidComboBoxNumberOfItemsWhichDeterminesHeight = 2; }
    
    ComboBox *Result = AddUnsortedComboBox(h, wA, label, LABEL_UP, x, y, width, height, id,
                                           stupidComboBoxNumberOfItemsWhichDeterminesHeight);
    AddAllComboBoxItems(Result->box, encounterNames, numOfEncounters);
    
    return Result;
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

OrderField AddOrderField(HWND win, HWND **winA, s32 x, s32 y, u32 idx, u64 *id)
{
    HWND *wA = *winA;
    
    OrderField Result = {};
    
    Result.Field  = AddStaticUnlabeledTextBox(win, wA, x + 50, y, 100, 20, (*id)++); wA += 1;
    Result.Pos    = AddNumberBox(win, wA, 0, LABEL_NULL, x + 25, y, 20, 20, (*id)++, idx); wA += 1;
    Result.Remove = AddButton(win, wA, "X", x, y, 20, 20, (*id)++, FALSE); wA += 1;
    
    *winA = wA;
    
    return Result;
}

InitField AddInitField(HWND h, HWND **winA, const char *label, s32 x, s32 y, 
                       u64 *id, u64 initId, b32 isParty = FALSE)
{
    InitField Result = {};
    HWND *wA = *winA;
    
    if(isParty == FALSE) 
    {
        Result.Name    = AddTextBox(h, wA, 0, LABEL_NULL, x, y, 100, 20, (*id)++, label);   wA += 1;
        Result.Bonus   = AddNumberBox(h, wA, 0, LABEL_NULL, x + 110, y, 30, 20, (*id)++);   wA += 1;
        //Result.Final   = AddValueBox(h, wA, 0, LABEL_NULL, 0, x + 150, y, 30, 20, (*id)++); wA += 1;
        Result.Final   = AddNumberBox(h, wA, 0, LABEL_NULL, x + 150, y, 30, 20, (*id)++); wA += 1;
        
        Result.New.Name  = AddTextBox(h, wA, 0, LABEL_NULL, x, y, 100, 20, (*id)++, ""); wA += 1;
        Result.New.Bonus = AddNumberBox(h, wA, 0, LABEL_NULL, x + 110, y, 30, 20, (*id)++); wA += 1;
        Result.New.Add   = AddButton(h, wA, "+", x + 50, y, 11, 18, (*id)++, FALSE);        wA += 1;
        Result.New.Ok    = AddButton(h, wA, "v", x + 150, y, 11, 18, (*id)++, FALSE);       wA += 1;
    }
    else {
        Result.Bonus  = AddNumberBox(h, wA, label, LABEL_LEFT, x, y, 30, 20, (*id)++);  wA += 2;
    }
    
    Result.id = initId;
    
    *winA = wA;
    return Result;
}

Counter AddCounter(HWND h, HWND **winA, const char *label, s32 x, s32 y, u64 *id)
{
    HWND *wA = *winA;
    
    Counter Result = {};
    
    Result.Field  = AddTextBox(h, wA, label, LABEL_UP, x, y, 100, 20, (*id)++);      wA += 2;
    Result.Rounds = AddNumberBox(h, wA, 0, LABEL_NULL, x + 105, y, 30, 20, (*id)++); wA += 1;
    Result.Start  = AddButton(h, wA, "Start", x + 140, y, 35, 20, (*id)++);          wA += 1;
    
    *winA = wA;
    return Result;
}

void DrawInitTab(HWND WinH, u64 *ElementId)
{
    InitPage *Page = State.Init;
    HWND *wA = Page->WindowsArray + Page->numWindows;
    
    Page->Mobs = AddUnsortedComboBox(WinH, wA, "Enemies", LABEL_UP,
                                     276, 102, 100, 20, (*ElementId)++, ArraySize(Enemies));;
    wA += 2;
    
    AddAllComboBoxItems(Page->Mobs->box, Enemies, ArraySize(Enemies));
    
    Page->Allies = AddUnsortedComboBox(WinH, wA, "Allies", LABEL_UP,
                                       520, 282, 100, 20, (*ElementId)++, ArraySize(Allies));;
    wA += 2;
    AddAllComboBoxItems(Page->Allies->box, Allies, ArraySize(Allies));
    
    Page->numWindows += 4;
    
    // Party Fields
    s32 yPos = 142;
    for(u32 i = 0; i < PARTY_NUM; i++)
    {
        Page->PlayerFields[i] = AddInitField(WinH, &wA, PartyName[i], 560, yPos, ElementId, i, TRUE);
        yPos += 20;
        Page->numWindows += 2;
    }
    
    // Ally Fields
    yPos += 70;
    for(u32 i = 0; i < ALLY_NUM; i++)
    {
        Page->AllyFields[i] = AddInitField(WinH, &wA, AllyName[i], 480, yPos, ElementId, i, FALSE);
        yPos += 20;
        Page->numWindows += 3;
    }
    
    // Mob Fields
    yPos = 142;
    for(u32 i = 0; i < MOB_NUM; i++)
    {
        Page->MobFields[i] = AddInitField(WinH, &wA, MobName[i], 246, yPos, ElementId, i, FALSE);
        yPos += 20;
        Page->numWindows += 3;
    }
    
    //ORDER
    yPos = 142;
    for(u32 i = 0; i < ORDER_NUM; i += 2)
    {
        Page->Order[i]   = AddOrderField(WinH, &wA, 720, yPos, i, ElementId);
        if((i+1) < ORDER_NUM) { 
            Page->Order[i+1] = AddOrderField(WinH, &wA, 880, yPos, i+1, ElementId); 
            Page->numWindows += 3;
        }
        Page->numWindows += 3;
        yPos += 20;
    }
    
    
    Page->Current = AddStaticUnlabeledTextBox(WinH, wA, 820, 112, 100, 20, (*ElementId)++); wA += 1;
    
    Page->Roll  = AddButton(WinH, wA, "Roll",  406, 102, 45, 20, (*ElementId)++); wA += 1;
    Page->Set   = AddButton(WinH, wA, "Set",   630, 102, 45, 20, (*ElementId)++); wA += 1;
    Page->Next  = AddButton(WinH, wA, "Next",  850, 82, 45, 20, (*ElementId)++); wA += 1;
    Page->Reset = AddButton(WinH, wA, "Reset", 520, 102, 45, 20, (*ElementId)++); wA += 1;
    Page->Save  = AddButton(WinH, wA, "Save",  620, 42, 45, 20, (*ElementId)++); wA += 1;
    
    Page->numWindows += 6;
    
    yPos = 142;
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Page->Counters[i] = AddCounter(WinH, &wA, CounterNames[i], 20, yPos, ElementId);
        yPos += 40;
        Page->numWindows += 4;
    }
    
    
    Page->EncounterName = AddTextBox(WinH, wA, 0, LABEL_NULL, 594, 62, 100, 20, (*ElementId)++); wA += 1;
    Page->numWindows += 1;
    
    //EncounterSelection
    if(State.encounters.isInitialized == TRUE)
    {
        Page->EncounterSel = FillEncounters(WinH, wA, "Encounters", 470, 60, 100, 20, (*ElementId)++);
        wA += 2;
        
        Page->numWindows += 2;
    }
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
    
    HMENU MenuBar = CreateMenu();
    MENUINFO Info = {};
    Info.cbSize = sizeof(MENUINFO);
    Info.fMask = MIM_APPLYTOSUBMENUS | MIM_STYLE;
    Info.dwStyle = MNS_NOTIFYBYPOS;
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
                    HidePage(State.Init);
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
