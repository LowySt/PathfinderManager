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

//NOTE: Used by bitmap
#define LS_MATH_IMPLEMENTATION
#include "lsMath.h"
#undef LS_MATH_IMPLEMENTATION

#define LS_BITMAP_IMPLEMENTATION
#include "lsBitmap.h"
#undef LS_BITMAP_IMPLEMENTATION


#define HAS_TABS 0

#include "lsGraphics.h"

#include "pcg.c"


#include "win32_widgets.h"
#include "Init.h"
#include "Class.h"
#include "PlayerChar.h"
#include "StateGlobals.h"
#include "buttonImage.h"

#include "diceRoller.cpp"

#include "Class.cpp"
#include "Skills.cpp"
#include "Feats.cpp"
#include "PlayerChar.cpp"
#include "Init.cpp"
#include "PCTab.cpp"
#include "FeatsTab.cpp"
#include "OnButton.cpp"
#include "SaveState.cpp"

#include "win32_widgets.cpp"
#include "subEdit.cpp"
#include "ui.cpp"

#define HidePage(page) for(u32 i = 0; i < page->numWindows; i++) { \
ShowWindow(page->WindowsArray[i], SW_HIDE); }

#define ShowPage(page) for(u32 i = 0; i < page->numWindows; i++) { \
ShowWindow(page->WindowsArray[i], SW_SHOW); }

HBITMAP closeButton;
HDC     closeButtonDC;
void    *closeButtBackbuff;

//NOTE:TEST
HDC WindowDC;
HDC BackBufferDC;
u8  *BackBuffer;
//NOTE:TEST

LRESULT WindowProc(HWND h, UINT msg, WPARAM w, LPARAM l)
{
    LRESULT Result = 0;
    
    switch (msg)
    {
        case WM_ERASEBKGND: return TRUE; break;
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps = {};
            RECT r;
            
            //NOTE: I have to call BeginPaint() - EndPaint() Anyway.
            //If I don't, the message loop is gonna get stuck in PAINT calls.
            BeginPaint(h, &ps);
            
            //NOTE: Draw Background
            GetClientRect(h, &r);
            
            BITMAPINFO BitmapInfo = {};
            BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            BitmapInfo.bmiHeader.biWidth = State.windowWidth;
            BitmapInfo.bmiHeader.biHeight = State.windowHeight; //Should it be negative?
            BitmapInfo.bmiHeader.biPlanes = 1;
            BitmapInfo.bmiHeader.biBitCount = 32;
            BitmapInfo.bmiHeader.biCompression = BI_RGB;
            
            StretchDIBits(BackBufferDC, 0, 0, State.windowWidth, State.windowHeight,
                          0, 0, State.windowWidth, State.windowHeight,
                          BackBuffer, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
            
            Result = BitBlt(WindowDC, r.left, r.top, r.right, r.bottom,
                            BackBufferDC, 0, 0, SRCCOPY);
            
            if(Result == 0) {
                DWORD Err = GetLastError();
                int breakHere = 0;
            }
            
            
            
            EndPaint(h, &ps);
        } break;
        
        case WM_NCLBUTTONDOWN:
        {
            State.isDragging = TRUE;
            State.prevMousePos = *((POINTS *)&l);
            
            POINTS cursorPos = State.prevMousePos;
            
            RECT screenRect = {};
            GetMenuItemRect(h, MenuBar, 1, &screenRect);
            
            screenRect.right  = screenRect.right + 1234;
            screenRect.left   = screenRect.left + 1218;
            screenRect.top    = screenRect.top + 1;
            screenRect.bottom = screenRect.bottom + 1;
            
            //NOTE:TODO:NOTE:TODO:
            // HACK HACK HACK HACK HACK HACK
            if((cursorPos.x > screenRect.left) && (cursorPos.x < screenRect.right) &&
               (cursorPos.y < screenRect.bottom) && (cursorPos.y > screenRect.top))
            { 
                PostMessageA(h, WM_DESTROY, 0, 0);
            }
            
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
                
                State.currWindowPos = { newWinX, newWinY };
                State.prevMousePos  = POINTS { (SHORT)currMouse.x, (SHORT)currMouse.y};
                //State.isDragging = FALSE;
                
                SetWindowPos(h, 0, newWinX, newWinY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            }
            //TODO:NOTE: Page says to return 0, but DO i HAVE to?
        } break;
        
        case WM_DESTROY:
        {
            SaveState();
            ExitProcess(0);
        } break;
        
        case WM_MEASUREITEM:
        {
            MEASUREITEMSTRUCT *item = (MEASUREITEMSTRUCT *)l;
            
            if(item->CtlType == ODT_MENU) {
                
                if(item->itemID == MENU_FILE_ITEM_ID)
                {
                    u32 len = ls_len((char *)item->itemData);
                    SIZE size = {};
                    BOOL success = GetTextExtentPoint32A(GetDC(h), (LPCSTR)item->itemData, len, &size);
                    
                    //NOTE: Used to center Text inside the rect.
                    u32 marginX = 0;
                    u32 marginY = 0;
                    
                    item->itemWidth  = size.cx + (2*marginX);
                    item->itemHeight = size.cy + (2*marginY);
                }
                else if(item->itemID == MENU_CLOSE_APP_ID)
                {
                    item->itemWidth = 16;
                    item->itemHeight = 16;
                }
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
                if(item->itemID == MENU_FILE_ITEM_ID)
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
                else if(item->itemID == MENU_CLOSE_APP_ID)
                {
                    //NOTE:TODO:NOTE:TODO:
                    // HACK HACK HACK HACK HACK HACK
                    RECT hackRect  = item->rcItem;
                    hackRect.right = hackRect.right + 1234;
                    hackRect.left  = hackRect.left + 1218;
                    hackRect.top   = hackRect.top + 1;
                    hackRect.bottom = hackRect.bottom + 1;
                    
                    if(!BitBlt(item->hDC, hackRect.left, hackRect.top, 16, 16, closeButtonDC, 0, 0, SRCCOPY))
                    {
                        DWORD Error = GetLastError();
                        ls_printf("When Blitting got error: %d", Error);
                    }
                }
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
                u32 textLen = SendMessageA(item->hwndItem, CB_GETLBTEXTLEN , item->itemID, 0);
                SendMessageA(item->hwndItem, CB_GETLBTEXT, item->itemID, (LPARAM)text);
                
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
            HMENU menuHandle = (HMENU)l;
            
            if(menuHandle == SubMenu)
            {
                switch(itemIdx)
                {
                    case FILE_MENU_SAVE_IDX:
                    {
#if HAS_TABS
                        //First update Ability Scores then serialize.
                        saveAS();
                        SerializePC(&pc);
#endif
                        SaveState();
                    } break;
                    
                    case FILE_MENU_LOAD_IDX:
                    {
#if HAS_TABS
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
#endif
                        
                    } break;
                }
            }
            else if(menuHandle == MenuBar) { }
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
                    //NOTE: Handles storing data for save on the fields in the PC Tab
                    //TODO: Could probably be handled better if it was moved in the subEditProc
                    //      CAREFUL! If more is added a found flag has to be returned.
#if HAS_TABS
                    PCTabOnKillFocus(commandID, handle);
#endif
                } break;
                
                case CBN_SELENDOK:
                {
                    b32 found = FALSE;
                    //NOTE:TODO: This is easier to read. But it forces me to add a check
                    //           for the early break. Bad or Acceptable??
#if HAS_TABS
                    found = PCTabOnComboSelect(commandID, handle);
                    if(found) break;
#endif
                    found = InitTabOnComboSelect(commandID, handle);
                    if(found) break;
                    
                } break;
                
                case LBN_SELCHANGE:
                {
                    //TODO: THIS DOESN'T MAKE ANY SENSE!!!
                    // THERE'S NO DIFFERENTIATION BETWEEN THE FEATS AND THE CHOSEN ONES!
#if HAS_TABS
                    if(commandID == State.Feats->Feats->id)
                    {
                        u32 index = ListBox_GetCurSel(handle);
                        if(index > ArraySize(FeatsDesc))
                        {
                            ls_printf("Not implemented description of Feat n°%d\n", index);
                            Assert(FALSE);
                        }
                        char *Desc = (char *)FeatsDesc[index];
                        
                        Edit_SetText(State.Feats->FeatsDesc->box, Desc);
                    }
#endif
                } break;
                
                case LBN_DBLCLK:
                {
                    //NOTE: Handles selecting feats and chosen feats from the ListBoxes
#if HAS_TABS
                    FeatsTabOnDoubleClick(commandID, handle);
#endif
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
    u32 style = LS_VISIBLE | LS_THIN_BORDER | LS_POPUP; //| LS_OVERLAPPEDWINDOW;
    BOOL Result;
    
    SubMenu = CreateMenu();
    Result = AppendMenuA(SubMenu, MF_STRING, 0, "Save");
    Result = AppendMenuA(SubMenu, MF_STRING, 1, "Load");
    
    MENUITEMINFOA FileMenuInfo = {};
    FileMenuInfo.cbSize     = sizeof(MENUITEMINFOA);
    FileMenuInfo.fMask      = MIIM_SUBMENU | MIIM_DATA | MIIM_FTYPE | MIIM_ID;
    FileMenuInfo.fType      = MFT_OWNERDRAW;
    FileMenuInfo.wID        = MENU_FILE_ITEM_ID;
    FileMenuInfo.dwItemData = (ULONG_PTR)"File";
    FileMenuInfo.hSubMenu   = SubMenu;
    FileMenuInfo.cch        = 5;
    Result = InsertMenuItemA(MenuBar, MENU_FILE_IDX, TRUE, &FileMenuInfo);
    
    BITMAPINFO BitmapInfo = {};
    BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitmapInfo.bmiHeader.biWidth = 16;
    BitmapInfo.bmiHeader.biHeight = -16; //Should it be negative?
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 24;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
    
    closeButton = CreateDIBSection(NULL, &BitmapInfo, DIB_RGB_COLORS, 
                                   &closeButtBackbuff, NULL, NULL);
    closeButtonDC = CreateCompatibleDC(NULL);
    SelectObject(closeButtonDC, closeButton);
    ls_memcpy(pixelButtonData, closeButtBackbuff, 16*16*3);
    
    MENUITEMINFOA CloseBitmap = {};
    CloseBitmap.cbSize     = sizeof(MENUITEMINFOA);
    CloseBitmap.fMask      = MIIM_ID | MIIM_FTYPE;
    CloseBitmap.fType      = MFT_OWNERDRAW;
    CloseBitmap.wID        = MENU_CLOSE_APP_ID;
    Result = InsertMenuItemA(MenuBar, MENU_CLOSE_APP_IDX, TRUE, &CloseBitmap);
    
    MENUINFO menuInfo = {};
    menuInfo.cbSize  = sizeof(MENUINFO);
    menuInfo.fMask   = MIM_APPLYTOSUBMENUS | MIM_BACKGROUND;
    menuInfo.hbrBack = appBkgBrush;  //RGB(0x38, 0x38, 0x38);
    
    //NOTE:TODO: Hardcoded!!
    State.windowWidth = 1280;
    State.windowHeight = 840;
    
    HWND WindowHandle;
    if ((WindowHandle = CreateWindowExA(0, "WndClass",
                                        "PCMan", style,
                                        300, 50, //CW_USEDEFAULT, CW_USEDEFAULT,
                                        State.windowWidth, State.windowHeight,
                                        0, MenuBar, MainInstance, 0)) == nullptr)
    {
        DWORD Error = GetLastError();
        ls_printf("When Retrieving a WindowHandle in Win32_SetupScreen got error: %d", Error);
    }
    
    BITMAPINFO BackBufferInfo = {};
    BackBufferInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BackBufferInfo.bmiHeader.biWidth = State.windowWidth;
    BackBufferInfo.bmiHeader.biHeight = State.windowHeight;
    BackBufferInfo.bmiHeader.biPlanes = 1;
    BackBufferInfo.bmiHeader.biBitCount = 32;
    BackBufferInfo.bmiHeader.biCompression = BI_RGB;
    
    WindowDC = GetDC(WindowHandle);
    BackBufferDC = CreateCompatibleDC(WindowDC);
    HBITMAP DibSection = CreateDIBSection(BackBufferDC, &BackBufferInfo,
                                          DIB_RGB_COLORS, (void **)&BackBuffer, NULL, NULL);
    SelectObject(BackBufferDC, DibSection);
    
    State.currWindowPos = { 300, 50 }; //NOTE:TODO: Hardcoded!!
    
    return WindowHandle;
}

void Windows_LoadFont(UIFont *uiFont, u32 pointSize)
{
    char *fontFace = "Calibri";
    ls_memcpy(fontFace, uiFont->face, ls_len(fontFace));
    
    uiFont->pointSize = pointSize;
    
    HFONT f = CreateFontA(pointSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                          DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                          DEFAULT_PITCH | FF_DONTCARE, fontFace);
    
    HGDIOBJ oldObj = SelectObject(BackBufferDC, f);
    
    MAT2 mat = {{1, 1}, {}, {}, {1, 1}};
    
    for(u32 idx = 33; idx <= 126; idx++)
    {
        UIGlyph *currGlyph = &uiFont->glyph[idx];
        GLYPHMETRICS gm = {};
        
        wchar_t charString[2] = {};
        charString[0] = (wchar_t)idx;
        
        WORD index = 0;
        DWORD len = GetGlyphIndicesW(BackBufferDC, charString, 1, &index, GGI_MARK_NONEXISTING_GLYPHS);
        
        currGlyph->idxInFont = index;
        currGlyph->size = GetGlyphOutlineW(BackBufferDC, currGlyph->idxInFont, GGO_GRAY2_BITMAP|GGO_GLYPH_INDEX, &gm, 0, NULL, &mat);
        
        currGlyph->data = (u8 *)ls_alloc(currGlyph->size);
        
        GetGlyphOutlineW(BackBufferDC, currGlyph->idxInFont, GGO_GRAY2_BITMAP|GGO_GLYPH_INDEX, &gm, currGlyph->size, currGlyph->data, &mat);
        
        currGlyph->width  = gm.gmBlackBoxX;
        currGlyph->height = gm.gmBlackBoxY;
        currGlyph->xOrig  = gm.gmptGlyphOrigin.x;
        currGlyph->yOrig  = gm.gmptGlyphOrigin.y;
        currGlyph->xAdv   = gm.gmCellIncX;
        currGlyph->yAdv   = gm.gmCellIncY;
    }
    
    DeleteObject(f);
    
    return;
}

void windows_Render()
{
    InvalidateRect(MainWindow, NULL, TRUE);
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
    
    UIFont font = {};
    Windows_LoadFont(&font, 32);
    
    UIContext *uiContext  = (UIContext *)ls_alloc(sizeof(UIContext));
    uiContext->drawBuffer = BackBuffer;
    uiContext->width      = State.windowWidth;
    uiContext->height     = State.windowHeight;
    uiContext->font       = font; //TODO: Copy
    uiContext->callbackRender = &windows_Render;
    
    
    State.isInitialized = FALSE;
#if HAS_TABS
    State.PC    = (PCPage *)ls_alloc(sizeof(PCPage));
    State.Feats = (FeatsPage *)ls_alloc(sizeof(FeatsPage));
#endif
    
    State.Init  = (InitPage *)ls_alloc(sizeof(InitPage));
    
    u64 ElementId = 0;
    
#if HAS_TABS
    for(size_t i = 0; i < ArraySize(State.Feats->ChosenFeatsIndices); i++)
    { State.Feats->ChosenFeatsIndices[i] = u32(-1); }
#endif
    
    
    //
    // Create Tabs
    //
    
#if HAS_TABS
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
#endif
    
    DrawInitTab(MainWindow, &ElementId);
    
    State.isInitialized = TRUE;
    
#if HAS_TABS
    HidePage(State.Feats);
    HidePage(State.PC);
#endif
    
    ShowPage(State.Init);
    
    //Initialization of Init Page
    HideInitElem(State.Init);
    ShowOrder(State.Init->Order, PARTY_NUM);
    HideElem(State.Init->Next->box);
    
    HidePage(State.Init);
    
#if HAS_TABS
    TabCtrl_SetCurSel(TabControl, 2);
    
    u32 oldPageIdx = 2;
    u32 newPageIdx = 0;
#endif
    
    b32 success = LoadState();
    if(success == FALSE)
    {
        //NOTE:TODO: How to fail?
        // Nothing should have been changed yet, so the program shouldn't be in a bad state...
    }
    
    SYSTEMTIME endT, beginT;
    GetSystemTime(&beginT);
    
    b32 Running = TRUE;
    while(Running)
    {
        // Process Input
        MSG Msg;
        
#if HAS_TABS
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
        
        oldPageIdx = newPageIdx;
#endif
        
        while (PeekMessageA(&Msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
        }
        
        //NOTE:TEST
        ls_uiBackground(uiContext, (u32)appBkgRGB);
        
        ls_uiButton(uiContext, 100, 700, 80, 20);
        
        ls_uiGS2String(uiContext, 200, 700, ls_strConst("hello!"), RGBg(200));
        
        ls_uiRender(uiContext);
        //NOTE:TEST
        
        State.hasMouseClicked = FALSE;
        
        GetSystemTime(&endT);
        
        State.timePassed += (endT.wSecond - beginT.wSecond);
        if(State.timePassed >= 30)
        {
            State.timePassed = 0;
            SaveState();
        }
        beginT = endT;
    }
    
    return 0;
}
