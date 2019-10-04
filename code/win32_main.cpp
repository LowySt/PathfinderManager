#define LS_CRT_IMPLEMENTATION
#include "tools\lsCRT.h"

#define PLATFORM_IMPLEMENTATION
#include "Platform\lsWindows.h"

#include "tools\lsGraphics.h"

#include "Class.cpp"
#include "Skills.cpp"
#include "Feats.cpp"
#include "PlayerChar.cpp"

#define AddAllComboBoxItems(wnd, s, n) { for(size_t i = 0; i < n; i++) \
    {ComboBox_AddString(wnd, s[i]); } }

#define AddAllListBoxItems(wnd, s, n)  { for(size_t i = 0; i < n; i++) \
    {ListBox_AddString(wnd, s[i]); } }

#define HidePage(page) for(u32 i = 0; i < page->numWindows; i++) { \
    ShowWindow(page->WindowsArray[i], SW_HIDE); }

#define ShowPage(page) for(u32 i = 0; i < page->numWindows; i++) { \
    ShowWindow(page->WindowsArray[i], SW_SHOW); }

#define FILE_MENU_SAVE_IDX 0
#define FILE_MENU_LOAD_IDX 1

enum LabelAlign
{
    LABEL_NULL,
    
    LABEL_UP,
    LABEL_DOWN,
    LABEL_LEFT,
    LABEL_RIGHT,
    
    LABEL_COUNT
};

struct TextBox
{
    HWND box;
    HWND label;
    
    u64 id;
};

struct ComboBox
{
    HWND box;
    HWND label;
    
    u64 id;
};

struct ListBox
{
    HWND box;
    HWND label;
    
    u64 id;
};

struct Button
{
    HWND box;
    
    union
    {
        TextBox *LinkedText;
    };
    u32 value;
    
    u64 id;
};

struct AbilityScores
{
    TextBox *Box[6];
    TextBox *Bonus[6];
    Button *Plus1[6];
    Button *Plus10[6];
};

struct PCPage
{
    HWND TabControl; //DEBUG
    
    TextBox *Name;
    TextBox *Player;
    ComboBox *Race;
    
    ComboBox *Class;
    b32 wasClassChosen;
    
    ComboBox *GenMethod;
    AbilityScores *Scores;
    
    ListBox *RacialTraits;
    
    TextBox *currLevel;
    TextBox *currXP;
    TextBox *nextLevelXP;
    ComboBox *XPCurve;
    XPCurveIdx xpIdx;
    
    TextBox *SavingThrows[3];
    TextBox *BaseAttackBonus;
    
    HWND WindowsArray[64];
    u32 numWindows;
};

struct FeatsPage
{
    ListBox *Feats;
    TextBox *FeatsDesc;
    ListBox *ChosenFeats;
    
    HWND WindowsArray[64];
    u32 numWindows;
};

struct ProgramState
{
    PCPage *PC;
    FeatsPage *Feats;
};


HINSTANCE MainInstance;
PlayerChar pc = {};
ProgramState State = {};

//TODO: Can I manage to do things using this?
void *ElementMap[512] = {};

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
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_STR]->box, ASBonusTable[v]);
    ls_free(a);
    
    v = pc.AbilityScores[ABILITY_DEX];
    a = ls_itoa(v);
    Edit_SetText(State.PC->Scores->Box[ABILITY_DEX]->box, a);
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_DEX]->box, ASBonusTable[v]);
    ls_free(a);
    
    v = pc.AbilityScores[ABILITY_CON];
    a = ls_itoa(v);
    Edit_SetText(State.PC->Scores->Box[ABILITY_CON]->box, a);
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_CON]->box, ASBonusTable[v]);
    ls_free(a);
    
    v = pc.AbilityScores[ABILITY_INT];
    a = ls_itoa(v);
    Edit_SetText(State.PC->Scores->Box[ABILITY_INT]->box, a);
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_INT]->box, ASBonusTable[v]);
    ls_free(a);
    
    v = pc.AbilityScores[ABILITY_WIS];
    a = ls_itoa(v);
    Edit_SetText(State.PC->Scores->Box[ABILITY_WIS]->box, a);
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_WIS]->box, ASBonusTable[v]);
    ls_free(a);
    
    v = pc.AbilityScores[ABILITY_CHA];
    a = ls_itoa(v);
    Edit_SetText(State.PC->Scores->Box[ABILITY_CHA]->box, a);
    Edit_SetText(State.PC->Scores->Bonus[ABILITY_CHA]->box, ASBonusTable[v]);
    ls_free(a);
}

void UpdateSavingThrows()
{
    s32 Fortitude = 0;
}

LRESULT WindowProc(HWND h, UINT msg, WPARAM w, LPARAM l)
{
    LRESULT Result = 0;
    
    switch (msg)
    {
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
                    char *cStr = ls_toCStr(pc.Name);
                    Edit_SetText(State.PC->Name->box, cStr);
                    ls_free(cStr);
                    
                    cStr = ls_toCStr(pc.Player);
                    Edit_SetText(State.PC->Player->box, cStr);
                    ls_free(cStr);
                    
                    ComboBox_SelectString(State.PC->Race->box, -1, Races[pc.Race]);
                    ComboBox_SelectString(State.PC->Class->box, -1, Classes[pc.Class]);
                    
                    State.PC->wasClassChosen = TRUE;
                    
                    ComboBox_SelectString(State.PC->GenMethod->box, -1, 
                                          AbilityGenMethods[pc.GenMethod]);
                    
                    loadAS();
                    
                    char **TraitsList;
                    u32 arrSize = 0;
                    
                    TraitsList = RaceTraits[pc.Race];
                    arrSize = RaceTraitsArraySize[pc.Race];
                    
                    ListBox_ResetContent(State.PC->RacialTraits->box);
                    AddAllListBoxItems(State.PC->RacialTraits->box, TraitsList, arrSize);
                    
                    cStr = ls_itoa(pc.lvl);
                    Edit_SetText(State.PC->currLevel->box, cStr);
                    ls_free(cStr);
                    
                    cStr = ls_itoa(pc.xp);
                    Edit_SetText(State.PC->currXP->box, cStr);
                    ls_free(cStr);
                    
                    State.PC->xpIdx = pc.xpCurve;
                    ComboBox_SelectString(State.PC->XPCurve->box, -1,
                                          XPCurvesString[pc.xpCurve]);
                    
                    char **xpCurve = XPCurvesArr[State.PC->xpIdx];
                    Edit_SetText(State.PC->nextLevelXP->box, xpCurve[pc.lvl]);
                    
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
                    else if(commandID == State.PC->currLevel->id)
                    {
                        string s = getText(handle);
                        u32 currLevel = ls_stoi(s);
                        if(currLevel > 20) 
                        { currLevel = 20; Edit_SetText(handle, "20"); }
                        pc.lvl = (u8)currLevel;
                        
                        char **xpCurve = XPCurvesArr[State.PC->xpIdx];
                        Edit_SetText(State.PC->nextLevelXP->box, xpCurve[currLevel]);
                        
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
                        
                        TraitsList = RaceTraits[r];
                        arrSize = RaceTraitsArraySize[r];
                        
                        AddAllListBoxItems(State.PC->RacialTraits->box, TraitsList, arrSize);
                        
                        ls_strFree(&s);
                    }
                    else if(commandID == State.PC->Class->id)
                    {
                        string s = getText(handle);
                        
                        GameClass r = getClassFromString(s);
                        pc.Class = r;
                        
                        ls_strFree(&s);
                    }
                    else if(commandID == State.PC->GenMethod->id)
                    {
                        string s = getText(handle);
                        
                        GenMethod r = getGenMethodFromString(s);
                        pc.GenMethod = r;
                        
                        ls_strFree(&s);
                    }
                    else if(commandID == State.PC->XPCurve->id)
                    {
                        State.PC->xpIdx = (XPCurveIdx)ComboBox_GetCurSel(handle);
                        pc.xpCurve = State.PC->xpIdx;
                        char **xpCurve = XPCurvesArr[State.PC->xpIdx];
                        
                        string s = getText(State.PC->currLevel->box);
                        u32 currLevel = ls_stoi(s);
                        
                        Edit_SetText(State.PC->nextLevelXP->box, xpCurve[currLevel]);
                        
                        
                        
                        ls_strFree(&s);
                    }
                    
                } break;
                
                case LBN_DBLCLK:
                {
                    u32 index = ListBox_GetCurSel(handle);
                    if(index > ArraySize(FeatsDesc)) 
                    { 
                        ls_printf("Not implemented description of Feat n°%d\n", index);
                        Assert(FALSE);
                    }
                    char *Desc = FeatsDesc[index];
                    
                    Edit_SetText(State.Feats->FeatsDesc->box, Desc);
                    
                } break;
                
                //TODO: I'd like to add functionality for removing Ability Scores.
                case BN_CLICKED:
                { 
                    Button *clicked = (Button *)ElementMap[commandID];
                    TextBox *text = clicked->LinkedText;
                    
                    string s = getText(text->box);
                    u32 val = ls_stoi(s);
                    val += clicked->value;
                    
                    if((commandID == State.PC->Scores->Plus1[ABILITY_STR]->id) ||
                       (commandID == State.PC->Scores->Plus10[ABILITY_STR]->id))
                    {
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_STR]->box, ASBonusTable[val]);
                    }
                    else if((commandID == State.PC->Scores->Plus1[ABILITY_DEX]->id) ||
                            (commandID == State.PC->Scores->Plus10[ABILITY_DEX]->id))
                    {
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_DEX]->box, ASBonusTable[val]);
                    }
                    else if((commandID == State.PC->Scores->Plus1[ABILITY_CON]->id) ||
                            (commandID == State.PC->Scores->Plus10[ABILITY_CON]->id))
                    {
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_CON]->box, ASBonusTable[val]);
                    }
                    else if((commandID == State.PC->Scores->Plus1[ABILITY_INT]->id) ||
                            (commandID == State.PC->Scores->Plus10[ABILITY_INT]->id))
                    {
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_INT]->box, ASBonusTable[val]);
                    }
                    else if((commandID == State.PC->Scores->Plus1[ABILITY_WIS]->id) ||
                            (commandID == State.PC->Scores->Plus10[ABILITY_WIS]->id))
                    {
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_WIS]->box, ASBonusTable[val]);
                    }
                    else if((commandID == State.PC->Scores->Plus1[ABILITY_CHA]->id) ||
                            (commandID == State.PC->Scores->Plus10[ABILITY_CHA]->id))
                    {
                        Edit_SetText(State.PC->Scores->Bonus[ABILITY_CHA]->box, ASBonusTable[val]);
                    }
                    
                    string newVal = ls_itos(val);
                    ls_nullTerminate(&newVal);
                    
                    BOOL ret = SetWindowTextA(text->box, newVal.data);
                    
                    ls_strFree(&s);
                    ls_strFree(&newVal);
                } break;
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
    WindowClass.hbrBackground = (HBRUSH)COLOR_GRAYTEXT;
    WindowClass.lpszClassName = "WndClass";
    
    if (!RegisterClassA(&WindowClass))
    {
        DWORD Error = GetLastError();
        ls_printf("When Registering WindowClass in Win32_SetupScreen got error: %d", Error);
    }
}

HWND CreateWindow(HMENU MenuBar)
{
    u32 style = LS_OVERLAPPEDWINDOW | LS_VISIBLE;
    BOOL Result;
    
    HMENU SubMenu = CreateMenu();
    Result = AppendMenuA(SubMenu, MF_STRING, 0, "Save");
    Result = AppendMenuA(SubMenu, MF_STRING, 1, "Load");
    
    MENUITEMINFOA FileMenuInfo = {};
    FileMenuInfo.cbSize = sizeof(MENUITEMINFOA);
    FileMenuInfo.fMask = MIIM_STRING | MIIM_SUBMENU;
    FileMenuInfo.dwTypeData = "File";
    FileMenuInfo.hSubMenu = SubMenu;
    FileMenuInfo.cch = 5;
    Result = InsertMenuItemA(MenuBar, 0, TRUE, &FileMenuInfo);
    
    HWND WindowHandle;
    if ((WindowHandle = CreateWindowExA(0, "WndClass", 
                                        "Name", style, 
                                        CW_USEDEFAULT, CW_USEDEFAULT, 
                                        1350, 900, 
                                        0, MenuBar, MainInstance, 0)) == nullptr)
    {
        DWORD Error = GetLastError();
        ls_printf("When Retrieving a WindowHandle in Win32_SetupScreen got error: %d", Error);
    }
    
    return WindowHandle;
}


HWND AddLabelBox(HWND win, LabelAlign A, char *label, s32 x, s32 y, u32 width, u32 height)
{
    s32 correctX = x;
    s32 correctY = y;
    s32 correctWidth = width;
    s32 correctHeight = height;
    
    switch(A)
    {
        case LABEL_NULL:
        {
            return NULL;
        } break;
        
        case LABEL_UP:
        {
            correctHeight = 20;
            correctY = y - correctHeight;
        } break;
        
        case LABEL_DOWN:
        {
            correctHeight = 20;
            correctY = y + correctHeight;
        } break;
        
        case LABEL_LEFT:
        {
            correctWidth = 100;
            correctHeight = 20;
            correctX = x - correctWidth;
        } break;
        
        case LABEL_RIGHT:
        {
            correctWidth = 100;
            correctHeight = 20;
            correctX = x + correctWidth;
        } break;
        
        default:
        {
            Assert(FALSE);
        }
    };
    
    //TODO: Make label id dynamically allocated
    HWND Result = CreateWindowExA(0, WC_STATIC, 0,
                                  WS_CHILD | WS_VISIBLE | 
                                  SS_CENTER, 
                                  correctX, correctY, correctWidth, correctHeight, 
                                  win, (HMENU)1, MainInstance, 0);
    
    SetWindowTextA(Result, label);
    
    return Result;
}


HWND AddEditBox(HWND win, s32 x, s32 y, u32 width, u32 height, u64 id)
{
    HWND Result = CreateWindowExA(0, WC_EDIT, "",
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | 
                                  ES_LEFT | ES_AUTOHSCROLL, 
                                  x, y, width, height, 
                                  win, (HMENU)id, MainInstance, 0);
    
    return Result;
}

HWND AddStaticEditBox(HWND win, s32 x, s32 y, u32 width, u32 height, u64 id,
                      b32 isMultiline = FALSE)
{
    u32 flags = ES_LEFT | ES_READONLY;
    if(isMultiline == TRUE) { flags |= ES_MULTILINE | ES_AUTOVSCROLL;}
    else { flags |= ES_AUTOHSCROLL; }
    
    HWND Result = CreateWindowExA(0, WC_EDIT, "",
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | flags, 
                                  x, y, width, height, 
                                  win, (HMENU)id, MainInstance, 0);
    
    return Result;
}

HWND AddEditNumberBox(HWND win, s32 x, s32 y, u32 width, u32 height, u64 id)
{
    HWND Result = CreateWindowExA(0, WC_EDIT, "0",
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | 
                                  ES_LEFT | ES_AUTOHSCROLL | ES_NUMBER,
                                  x, y, width, height, 
                                  win, (HMENU)id, MainInstance, 0);
    
    return Result;
}

HWND AddStaticNumberBox(HWND win, s32 value, s32 x, s32 y, u32 width, u32 height, u64 id)
{
    char *v = ls_itoa(value);
    HWND Result = CreateWindowExA(0, WC_EDIT, v,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | 
                                  ES_LEFT | ES_AUTOHSCROLL | ES_NUMBER | ES_READONLY,
                                  x, y, width, height, 
                                  win, (HMENU)id, MainInstance, 0);
    ls_free(v);
    
    return Result;
}

Button *AddButton(HWND win, HWND *pageArr, const char *label, s32 x, s32 y, 
                  u32 width, u32 height, u64 id)
{
    Button *Result = (Button *)ls_alloc(sizeof(Button));
    
    Result->box = CreateWindowExA(0, WC_BUTTON, label,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER |
                                  BS_CENTER | BS_TEXT,
                                  x, y, width, height, 
                                  win, (HMENU)id, MainInstance, 0);
    Result->id = id;
    pageArr[0] = Result->box;
    
    ElementMap[id] = (void *)Result;
    
    return Result;
}

ComboBox *AddComboBox(HWND win, HWND *pageArr, char *label, LabelAlign A,
                      s32 x, s32 y, u32 width, u32 height, u64 id, u32 numItems)
{
    ComboBox *Result = (ComboBox *)ls_alloc(sizeof(ComboBox));
    
    HWND Label = AddLabelBox(win, A, label, x, y, width, height);
    Result->label = Label;
    
    HWND Box = CreateWindowExA(0, WC_COMBOBOX, 0,
                               WS_CHILD | WS_VISIBLE | WS_BORDER |
                               CBS_DROPDOWNLIST | CBS_SORT,
                               x, y, width, height*(numItems + 1), 
                               win, (HMENU)id, MainInstance, 0);
    Result->box = Box;
    Result->id = id;
    
    pageArr[0] = Box;
    pageArr[1] = Label;
    
    ElementMap[id] = (void *)Result;
    
    return Result;
}

TextBox *AddTextBox(HWND win, HWND *pageArr, char *label, LabelAlign A,
                    s32 x, s32 y, u32 width, u32 height, u64 id)
{
    TextBox *Result = (TextBox *)ls_alloc(sizeof(TextBox));
    
    HWND Label = AddLabelBox(win, A, label, x, y, width, height);
    Result->label = Label;
    
    Result->box = AddEditBox(win, x, y, width, height, id);
    Result->id = id;
    
    pageArr[0] = Result->box;
    pageArr[1] = Label;
    
    ElementMap[id] = (void *)Result;
    
    return Result;
}

TextBox *AddStaticTextBox(HWND win, HWND *pageArr, char *label, LabelAlign A,
                          s32 x, s32 y, u32 width, u32 height, u64 id, b32 isMultiline = FALSE)
{
    TextBox *Result = (TextBox *)ls_alloc(sizeof(TextBox));
    
    HWND Label = AddLabelBox(win, A, label, x, y, width, height);;
    Result->label = Label;
    
    Result->box = AddStaticEditBox(win, x, y, width, height, id, isMultiline);
    Result->id = id;
    
    pageArr[0] = Result->box;
    pageArr[1] = Label;
    
    ElementMap[id] = (void *)Result;
    
    return Result;
}

TextBox *AddNumberBox(HWND win, HWND *pageArr, char *label, LabelAlign A,
                      s32 x, s32 y, u32 width, u32 height, u64 id)
{
    TextBox *Result = (TextBox *)ls_alloc(sizeof(TextBox));
    HWND Label;
    if(A != LABEL_NULL)
    {
        Label = AddLabelBox(win, A, label, x, y, width, height);
        Result->label = Label;
    }
    
    Result->box = AddEditNumberBox(win, x, y, width, height, id);
    Result->id = id;
    
    pageArr[0] = Result->box;
    
    if(A != LABEL_NULL)
    { pageArr[1] = Label; }
    
    ElementMap[id] = (void *)Result;
    
    return Result;
}

TextBox *AddValueBox(HWND win, HWND *pageArr, char *label, LabelAlign A,
                     s32 value, s32 x, s32 y, u32 width, u32 height, u64 id)
{
    TextBox *Result = (TextBox *)ls_alloc(sizeof(TextBox));
    HWND Label;
    if(A != LABEL_NULL)
    {
        Label = AddLabelBox(win, A, label, x, y, width, height);
        Result->label = Label;
    }
    
    Result->box = AddStaticNumberBox(win, value, x, y, width, height, id);
    Result->id = id;
    
    pageArr[0] = Result->box;
    
    if(A != LABEL_NULL)
    { pageArr[1] = Label; }
    
    ElementMap[id] = (void *)Result;
    
    return Result;
}

ListBox *AddListBox(HWND win, HWND *pageArr, char *label, LabelAlign A,
                    s32 x, s32 y, u32 width, u32 height, u64 id, b32 isSorted = TRUE)
{
    ListBox *Result = (ListBox *)ls_alloc(sizeof(ListBox));
    HWND Label = AddLabelBox(win, A, label, x, y, width, height);
    
    Result->label = Label;
    
    u32 flags;
    if(isSorted) { flags = LBS_HASSTRINGS | LBS_MULTIPLESEL | LBS_NOTIFY | LBS_SORT; }
    else { flags = LBS_HASSTRINGS | LBS_MULTIPLESEL | LBS_NOTIFY | LBS_SORT; }
    
    Result->box = CreateWindowExA(0, WC_LISTBOX, "",
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | flags, 
                                  x, y, width, height, 
                                  win, (HMENU)id, MainInstance, 0);
    Result->id = id;
    
    pageArr[0] = Result->box;
    pageArr[1] = Label;
    
    ElementMap[id] = (void *)Result;
    
    return Result;
}

ListBox *AddSingleSelListBox(HWND win, HWND *pageArr, char *label, LabelAlign A,
                             s32 x, s32 y, u32 width, u32 height, u64 id, b32 isSorted = TRUE)
{
    ListBox *Result = (ListBox *)ls_alloc(sizeof(ListBox));
    HWND Label = AddLabelBox(win, A, label, x, y, width, height);
    
    Result->label = Label;
    
    u32 flags;
    if(isSorted) { flags = LBS_HASSTRINGS | LBS_NOTIFY | LBS_SORT; }
    else { flags = LBS_HASSTRINGS | LBS_MULTIPLESEL | LBS_NOTIFY | LBS_SORT; }
    
    Result->box = CreateWindowExA(0, WC_LISTBOX, "",
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | flags, 
                                  x, y, width, height, 
                                  win, (HMENU)id, MainInstance, 0);
    Result->id = id;
    
    pageArr[0] = Result->box;
    pageArr[1] = Label;
    
    ElementMap[id] = (void *)Result;
    
    return Result;
}

void AddAbilityScoreBoxes(HWND WindowHandle, u32 baseX, u32 baseY, u64 *ElementId)
{
    PCPage *Page = State.PC;
    HWND *wA = Page->WindowsArray +  Page->numWindows;
    
    AbilityScores *Scores  = Page->Scores;
    
    Scores->Box[ABILITY_STR] = AddNumberBox(WindowHandle, wA, "STR", LABEL_UP, 
                                            364, baseY, 100, 20, (*ElementId)++);
    wA += 2;
    Scores->Bonus[ABILITY_STR] = AddValueBox(WindowHandle, wA, "", LABEL_NULL, 0,
                                             474, baseY, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_STR]  = AddButton(WindowHandle, wA, "+1", 
                                            504, baseY, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus10[ABILITY_STR] = AddButton(WindowHandle, wA, "+10", 
                                            534, baseY, 40, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_STR]->LinkedText = Scores->Box[ABILITY_STR];
    Scores->Plus1[ABILITY_STR]->value = 1;
    Scores->Plus10[ABILITY_STR]->LinkedText = Scores->Box[ABILITY_STR];
    Scores->Plus10[ABILITY_STR]->value = 10;
    
    
    Scores->Box[ABILITY_DEX] = AddNumberBox(WindowHandle, wA, "DEX", LABEL_UP, 
                                            364, baseY+44, 100, 20, (*ElementId)++);
    wA += 2;
    Scores->Bonus[ABILITY_DEX] = AddValueBox(WindowHandle, wA, "", LABEL_NULL, 0,
                                             474, baseY+44, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_DEX]  = AddButton(WindowHandle, wA, "+1", 
                                            504, baseY+44, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus10[ABILITY_DEX] = AddButton(WindowHandle, wA, "+10", 
                                            534, baseY+44, 40, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_DEX]->LinkedText = Scores->Box[ABILITY_DEX];
    Scores->Plus1[ABILITY_DEX]->value = 1;
    Scores->Plus10[ABILITY_DEX]->LinkedText = Scores->Box[ABILITY_DEX];
    Scores->Plus10[ABILITY_DEX]->value = 10;
    
    
    Scores->Box[ABILITY_CON] = AddNumberBox(WindowHandle, wA, "CON", LABEL_UP, 
                                            364, baseY+88, 100, 20, (*ElementId)++);
    wA += 2;
    Scores->Bonus[ABILITY_CON] = AddValueBox(WindowHandle, wA, "", LABEL_NULL, 0,
                                             474, baseY+88, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_CON]  = AddButton(WindowHandle, wA, "+1", 
                                            504, baseY+88, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus10[ABILITY_CON] = AddButton(WindowHandle, wA, "+10", 
                                            534, baseY+88, 40, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_CON]->LinkedText = Scores->Box[ABILITY_CON];
    Scores->Plus1[ABILITY_CON]->value = 1;
    Scores->Plus10[ABILITY_CON]->LinkedText = Scores->Box[ABILITY_CON];
    Scores->Plus10[ABILITY_CON]->value = 10;
    
    
    Scores->Box[ABILITY_INT] = AddNumberBox(WindowHandle, wA, "INT", LABEL_UP, 
                                            364, baseY+132, 100, 20, (*ElementId)++);
    wA += 2;
    Scores->Bonus[ABILITY_INT] = AddValueBox(WindowHandle, wA, "", LABEL_NULL, 0,
                                             474, baseY+132, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_INT]  = AddButton(WindowHandle, wA, "+1", 
                                            504, baseY+132, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus10[ABILITY_INT] = AddButton(WindowHandle, wA, "+10", 
                                            534, baseY+132, 40, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_INT]->LinkedText = Scores->Box[ABILITY_INT];
    Scores->Plus1[ABILITY_INT]->value = 1;
    Scores->Plus10[ABILITY_INT]->LinkedText = Scores->Box[ABILITY_INT];
    Scores->Plus10[ABILITY_INT]->value = 10;
    
    
    Scores->Box[ABILITY_WIS] = AddNumberBox(WindowHandle, wA, "WIS", LABEL_UP, 
                                            364, baseY+176, 100, 20, (*ElementId)++);
    wA += 2;
    Scores->Bonus[ABILITY_WIS] = AddValueBox(WindowHandle, wA, "", LABEL_NULL, 0,
                                             474, baseY+176, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_WIS]  = AddButton(WindowHandle, wA, "+1", 
                                            504, baseY+176, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus10[ABILITY_WIS] = AddButton(WindowHandle, wA, "+10", 
                                            534, baseY+176, 40, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_WIS]->LinkedText = Scores->Box[ABILITY_WIS];
    Scores->Plus1[ABILITY_WIS]->value = 1;
    Scores->Plus10[ABILITY_WIS]->LinkedText = Scores->Box[ABILITY_WIS];
    Scores->Plus10[ABILITY_WIS]->value = 10;
    
    
    Scores->Box[ABILITY_CHA] = AddNumberBox(WindowHandle, wA, "CHA", LABEL_UP, 
                                            364, 252, 100, 20, (*ElementId)++);
    wA += 2;
    Scores->Bonus[ABILITY_CHA] = AddValueBox(WindowHandle, wA, "", LABEL_NULL, 0,
                                             474, 252, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_CHA]  = AddButton(WindowHandle, wA, "+1", 
                                            504, 252, 20, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus10[ABILITY_CHA] = AddButton(WindowHandle, wA, "+10", 
                                            534, 252, 40, 20, (*ElementId)++);
    wA += 1;
    Scores->Plus1[ABILITY_CHA]->LinkedText = Scores->Box[ABILITY_CHA];
    Scores->Plus1[ABILITY_CHA]->value = 1;
    Scores->Plus10[ABILITY_CHA]->LinkedText = Scores->Box[ABILITY_CHA];
    Scores->Plus10[ABILITY_CHA]->value = 10;
    
    Page->numWindows += 30;
    
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
    
    //
    // Create Ability Scores Fields
    //
    Page->GenMethod = AddComboBox(WindowHandle, wA, "Gen Method", LABEL_UP, 
                                  256, 40, 100, 20, 
                                  (*ElementId)++, ArraySize(AbilityGenMethods));
    wA += 2;
    AddAllComboBoxItems(Page->GenMethod->box, AbilityGenMethods, ArraySize(AbilityGenMethods));
    
    Page->numWindows += 10;
    
    Page->Scores = (AbilityScores *)ls_alloc(sizeof(AbilityScores));
    AddAbilityScoreBoxes(WindowHandle, 364, 40, ElementId);
    
    wA = Page->WindowsArray + Page->numWindows;
    
    //
    // Skill Choice
    //
    
    Page->RacialTraits = AddListBox(WindowHandle, wA, "Racial Traits", LABEL_UP,
                                    28, 302, 1280, 384, (*ElementId)++);
    wA += 2;
    
    Page->currLevel = AddNumberBox(WindowHandle, wA, "LvL", LABEL_UP, 
                                   664, 42, 100, 20, (*ElementId)++);
    Edit_SetText(Page->currLevel, "0");
    
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
                                      24, 48, 640, 506, (*ElementId)++);
    AddAllListBoxItems(Page->Feats->box, FeatNames, ArraySize(FeatNames));
    wA += 2;
    
    
    
    Page->FeatsDesc = AddStaticTextBox(WindowHandle, wA, "Feat Description", LABEL_UP,
                                       24, 574, 640, 256, (*ElementId)++, TRUE);
    wA += 2;
    
    Page->ChosenFeats = AddListBox(WindowHandle, wA, "Your Feats", LABEL_UP,
                                   684, 48, 640, 786, (*ElementId)++, FALSE);
    wA += 2;
    
    Page->numWindows += 6;
}

int WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR cmdLine, int nCmdShow)
{
    MainInstance = hInst;
    
    RegisterWindow();
    
    HMENU MenuBar = CreateMenu();
    MENUINFO Info = {};
    Info.cbSize = sizeof(MENUINFO);
    Info.fMask = MIM_APPLYTOSUBMENUS | MIM_STYLE;
    Info.dwStyle = MNS_NOTIFYBYPOS;
    SetMenuInfo(MenuBar, &Info);
    
    HWND WindowHandle = CreateWindow(MenuBar);
    State.PC = (PCPage *)ls_alloc(sizeof(PCPage));
    State.Feats = (FeatsPage *)ls_alloc(sizeof(FeatsPage));
    
    u64 ElementId = 0;
    
    //
    // Create Tabs
    //
    
    HWND TabControl = CreateWindowExA(0, WC_TABCONTROL, "",
                                      WS_CHILD | WS_VISIBLE | WS_BORDER, 
                                      0, 0, 300, 20, 
                                      WindowHandle, (HMENU)99, MainInstance, 0);
    State.PC->TabControl = TabControl;
    TCITEMA TabItem = {};
    TabItem.mask = TCIF_TEXT;
    TabItem.pszText = "PC";
    TabItem.cchTextMax = 2;
    TabItem.iImage = -1;
    Tab_InsertItem(TabControl, 0, &TabItem);
    
    TCITEMA TabItem2 = {};
    TabItem2.mask = TCIF_TEXT;
    TabItem2.pszText = "Feats";
    TabItem2.cchTextMax = 9;
    TabItem2.iImage = -1;
    Tab_InsertItem(TabControl, 1, &TabItem2);
    
    DrawPCTab(WindowHandle, &ElementId);
    DrawFeatsTab(WindowHandle, &ElementId);
    
    HidePage(State.Feats);
    ShowPage(State.PC);
    
    b32 Running = TRUE;
    u32 oldPageIdx = 0;
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
                    ShowPage(State.PC); 
                } break;
                
                case 1:
                {
                    HidePage(State.PC); 
                    ShowPage(State.Feats); 
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
