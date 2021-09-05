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

#define LS_STACK_IMPLEMENTATION
#include "lsStack.h"
#undef LS_STACK_IMPLEMENTATION

//NOTE: Used by bitmap
#define LS_MATH_IMPLEMENTATION
#include "lsMath.h"
#undef LS_MATH_IMPLEMENTATION

#define LS_BITMAP_IMPLEMENTATION
#include "lsBitmap.h"
#undef LS_BITMAP_IMPLEMENTATION

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_ifloor(x)     ls_floor(x)
#define STBTT_iceil(x)      ls_ceil(x)
#define STBTT_malloc(x, u) ((void)(u),ls_alloc(x))
#define STBTT_free(x, u)   ((void)(u),ls_free(x))
#define STBTT_assert(x)    Assert(x)

#include "..\lib\stb_truetype.h"
#undef STB_TRUETYPE_IMPLEMENTATION

#define HAS_TABS 0

#include "lsGraphics.h"

#include "pcg.c"


#include "Init.h"
#include "Class.h"
#include "PlayerChar.h"
#include "StateGlobals.h"
#include "buttonImage.h"

#include "diceRoller.cpp"

#include "Input.cpp"
#include "Class.cpp"
#include "Skills.cpp"
#include "Feats.cpp"
#include "PlayerChar.cpp"
#include "Init.cpp"
#include "PCTab.cpp"
#include "FeatsTab.cpp"
#include "OnButton.cpp"
#include "SaveState.cpp"

#include "ui.cpp"
#include "AssetLoader.cpp"

HBITMAP closeButton;
HDC     closeButtonDC;
void    *closeButtBackbuff;

HDC WindowDC;
HDC BackBufferDC;
u8  *BackBuffer;

//NOTE:TEST
static b32 showText = FALSE;
//NOTE:TEST

LRESULT WindowProc(HWND h, UINT msg, WPARAM w, LPARAM l)
{
    LRESULT Result = 0;
    
    MouseInput    *Mouse    = &UserInput.Mouse;
    
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
            BitmapInfo.bmiHeader.biHeight = State.windowHeight;
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
        
        case WM_CHAR:
        {
            b32 wasPressed = (l >> 30) & 0x1;
            u16 repeat     = (u16)l;
            
            b32 asciiRange  = ((w >= 32) && (w <= 126));
            b32 plane0Latin = ((w >= 0x00A1) && (w <= 0x024F));
            
            if(asciiRange || plane0Latin)
            {
                if(!wasPressed || repeat > 0)
                {
                    UserInput.Keyboard.hasPrintableKey = TRUE;
                    UserInput.Keyboard.keyCodepoint    = w;
                }
            }
        } break;
        
        case WM_KEYDOWN:
        {
            switch(w)
            { 
                case VK_BACK:    KeySet(keyMap::Backspace); break;
                case VK_DELETE:  KeySet(keyMap::Delete);    break;
                case VK_HOME:    KeySet(keyMap::Home);      break;
                case VK_END:     KeySet(keyMap::End);       break;
                case VK_LEFT:    KeySet(keyMap::LArrow);    break;
                case VK_RIGHT:   KeySet(keyMap::RArrow);    break;
                case VK_UP:      KeySet(keyMap::UArrow);    break;
                case VK_DOWN:    KeySet(keyMap::DArrow);    break;
                case VK_CONTROL: KeySet(keyMap::Control);   break;
                case 'C':        KeySet(keyMap::C);         break; 
                case 'G':        KeySet(keyMap::G);         break;
                case 'V':        KeySet(keyMap::V);         break; 
            }
        } break;
        
        case WM_KEYUP:
        {
            switch(w)
            { 
                case VK_BACK:    KeyUnset(keyMap::Backspace); break;
                case VK_DELETE:  KeyUnset(keyMap::Delete);    break;
                case VK_HOME:    KeyUnset(keyMap::Home);      break;
                case VK_END:     KeyUnset(keyMap::End);       break;
                case VK_LEFT:    KeyUnset(keyMap::LArrow);    break;
                case VK_RIGHT:   KeyUnset(keyMap::RArrow);    break;
                case VK_UP:      KeyUnset(keyMap::UArrow);    break;
                case VK_DOWN:    KeyUnset(keyMap::DArrow);    break;
                case VK_CONTROL: KeyUnset(keyMap::Control);   break;
                case 'C':        KeyUnset(keyMap::C);         break;
                case 'G':        KeyUnset(keyMap::G);         break;
                case 'V':        KeyUnset(keyMap::V);         break;
            }
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
        
        case WM_LBUTTONDOWN:
        {
            Mouse->isLeftPressed = TRUE;
        } break;
        
        case WM_LBUTTONUP:
        {
            Mouse->isLeftPressed = FALSE;
            
            State.isDragging = FALSE;
            BOOL success = ReleaseCapture();
        } break;
        
        case WM_MOUSEMOVE:
        {
            POINTS currMouseClient = *((POINTS *)&l);
            Mouse->currPos = { currMouseClient.x, State.windowHeight - currMouseClient.y };
            
            //TODO: Convert this to the input file, rather than the bad stateglobals thing.
            if(State.isDragging) {
                
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
            //TODO:Reinstate SaveState();
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
            
        } break;
        
        case WM_CTLCOLORSTATIC:
        {
            HDC editHDC = (HDC)w;
            
            SetBkColor(editHDC, RGB(0x56, 0x56, 0x56));
            SetTextColor(editHDC, RGB(255, 255, 255));
            
            return (LRESULT)controlBkgBrush;
        };
        
        default:
        {
            return DefWindowProcA(h, msg, w, l);
        }
    }
    
    return Result;
}

u32 win32_convertUTF16To32(u32 *utf32Buff, u32 maxBuff, wchar_t* data, u32 u16Len)
{
    //NOTE: isSurrogate is checking if the code is in range [0xD800 - 0xDFFF], with smartypants unsined math.
    auto isSurrogate     = [](wchar_t code) -> b32 { return ((u16)code - (u16)0xD800) < (u16)2048; };
    auto isHighSurrogate = [](wchar_t code) -> b32 { return (code & 0xFFFFFC00) == 0xD800; };
    auto isLowSurrogate  = [](wchar_t code) -> b32 { return (code & 0xFFFFFC00) == 0xDC00; };
    auto surrogateTo32   = [](wchar_t h, wchar_t l) -> u32 { return ((h << 10) + l - 0x35FDC00); };
    
    u32 idx = 0;
    
    wchar_t *In = data;
    while(In < (data + u16Len))
    {
        wchar_t curr16 = *In; 
        In += 1;
        
        b32 isSurr = (curr16 >= 0xD800) && (curr16 <= 0xDFFF);
        if(!isSurr) { utf32Buff[idx] = (u32)curr16; idx += 1; }
        else
        {
            AssertMsg(In < (data + u16Len), "Input cut off low surrogate??\n");
            
            wchar_t next16 = *In;
            In += 1;
            
            b32 hS = isHighSurrogate(curr16);
            b32 lS = isLowSurrogate(next16);
            
            AssertMsg((hS && lS) == TRUE, "Surrogate missing either High or Low\n");
            
            utf32Buff[idx] = surrogateTo32(curr16, next16);
        }
        
        AssertMsg(idx < maxBuff, "UTF32 buffer provided is too small\n");
    }
    
    return idx;
}

//NOTE:TODO: Stuff copy-pasted from the internet. Those left and right shifts are just masking bits...
u32 win32_convertUTF32To16(wchar_t* utf16Buff, u32 maxBuff, u32 *data, u32 u32Len)
{
    u32 *In = data;
    
    u32 index = 0;
    while(In < (data + u32Len))
    {
        u32 codepoint = *In;
        In += 1;
        
        wchar_t high = 0;
        wchar_t low  = 0;
        
        if(codepoint < 0x10000) 
        { 
            utf16Buff[index] = (wchar_t)codepoint; 
            index += 1; 
            continue; 
        }
        
        u32 temp = codepoint -  0x10000;
        high = (((temp << 12) >> 22) + 0xD800);
        low  = (((temp << 22) >> 22) + 0xDC00);
        
        utf16Buff[index]   = high;
        utf16Buff[index+1] = low;
        
        index += 1;
    }
    
    return index;
}

u32 win32_GetClipboard(void *buff, u32 maxUTF32Len)
{
    if(OpenClipboard(NULL) == 0) { return 0; }
    
    HANDLE Clipboard = GetClipboardData(CF_UNICODETEXT);
    
    wchar_t* data = (wchar_t *)GlobalLock(Clipboard);
    
    wchar_t* At = data;
    u32 strLen = 0;
    while(*At != 0) { strLen += 1; At += 1; }
    
    u32 utf32Buff[256] = {}; //TODO: I am unsure about this.
    
    u32 utf32Len = win32_convertUTF16To32(utf32Buff, 256, data, strLen);
    
    u32 copyLen = utf32Len < maxUTF32Len ? utf32Len : maxUTF32Len;
    
    ls_memcpy(utf32Buff, buff, copyLen*sizeof(u32));
    GlobalUnlock(Clipboard);
    
    CloseClipboard();
    
    return copyLen;
}

u32 win32_SetClipboard(void *data, u32 len)
{
    if(OpenClipboard(NULL) == 0) { return 0; }
    
    HANDLE Clipboard = GetClipboardData(CF_UNICODETEXT);
    EmptyClipboard();
    
    wchar_t charBuff[256] = {};
    u32 buffLen = win32_convertUTF32To16(charBuff, 256, (u32 *)data, len);
    
    HGLOBAL clipMem = GlobalAlloc(GMEM_MOVEABLE, (buffLen+1)*sizeof(u32));
    wchar_t *clipData = (wchar_t *)GlobalLock(clipMem);
    
    ls_memcpy(charBuff, clipData, buffLen*sizeof(wchar_t));
    clipData[buffLen] = 0;
    
    GlobalUnlock(clipMem);
    
    SetClipboardData(CF_UNICODETEXT, clipMem);
    
    CloseClipboard();
    
    return len;
}

void RegisterWindow()
{
    
    u32 prop = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    
    HCURSOR cursor = LoadCursorA(NULL, IDC_ARROW);
    
    WNDCLASSA WindowClass = { 0 };
    WindowClass.style = prop;
    WindowClass.lpfnWndProc = WindowProc;
    WindowClass.hInstance = MainInstance;
    WindowClass.hbrBackground = appBkgBrush; //RGB(0x38, 0x38, 0x38);
    WindowClass.lpszClassName = "WndClass";
    WindowClass.hCursor = cursor;
    
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

void windows_Render()
{
    InvalidateRect(MainWindow, NULL, TRUE);
}

//TODO:RemoveThisShit
void testProc(UIContext *cxt)
{
    showText = !showText;
    return;
}

void testHold(UIContext *cxt)
{
    ls_uiSelectFontByPixelHeight(cxt, 64);
    
    unistring text = ls_unistrFromAscii("I exist only when the button is held!");
    ls_uiGlyphString(cxt, 200, 300, text, RGB(0x3f, 0xa1, 0x37));
}

int WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR cmdLine, int nCmdShow)
{
    MainInstance = hInst;
    
    windows_initRegionTimer(RT_MILLISECOND);
    
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
    
    MainWindow = CreateWindow(MenuBar); //TODO: Stop flashing on window creation.
    
    UserInput.Keyboard.getClipboard = win32_GetClipboard;
    UserInput.Keyboard.setClipboard = win32_SetClipboard;
    
    //TODO: Maybe make this less manual?
    UIContext *uiContext       = (UIContext *)ls_alloc(sizeof(UIContext));
    uiContext->drawBuffer      = BackBuffer;
    uiContext->width           = State.windowWidth;
    uiContext->height          = State.windowHeight;
    uiContext->callbackRender  = &windows_Render;
    uiContext->backgroundColor = RGBg(0x38);
    uiContext->highliteColor   = RGBg(0x65);
    uiContext->pressedColor    = RGBg(0x75);
    uiContext->widgetColor     = RGBg(0x45);
    uiContext->borderColor     = RGBg(0x22);
    uiContext->textColor       = RGBg(0xCC);
    
    
    ls_uiPushScissor(uiContext, 0, 0, State.windowWidth, State.windowHeight);
    
    loadAssetFile(uiContext, ls_strConstant("assetFile"));
    
    State.isInitialized = TRUE;
    
    /*
    //TODO: Reinstate this b32 success = LoadState();
    if(success == FALSE)
    {
        //NOTE:TODO: How to fail?
        // Nothing should have been changed yet, so the program shouldn't be in a bad state...
    }
    */
    
    SYSTEMTIME endT, beginT;
    GetSystemTime(&beginT);
    
    //TODO:Remove with ID system
    UITextBox box = {};
    box.text = ls_unistrAlloc(16);
    
    UIButton button = {};
    button.name = ls_unistrFromAscii("Button");
    button.onClick = testProc;
    button.onHold = testHold;
    
    UIListBox listBox = {};
    ls_uiListBoxAddEntry(uiContext, &listBox, "Test 1");
    ls_uiListBoxAddEntry(uiContext, &listBox, "Test 2");
    ls_uiListBoxAddEntry(uiContext, &listBox, "Test 3");
    ls_uiListBoxAddEntry(uiContext, &listBox, "Test 4");
    ls_uiListBoxAddEntry(uiContext, &listBox, "Test 5");
    ls_uiListBoxAddEntry(uiContext, &listBox, "Test 6");
    
    ls_uiListBoxRemoveEntry(uiContext, &listBox, 3);
    
    UISlider slider  = {};
    slider.text = ls_unistrFromAscii("Il Nome di un Nemico");
    slider.maxValue  = 100;
    slider.minValue  = -50;
    slider.currPos   = 0.5;
    slider.style     = SL_BOX;
    slider.lColor    = ls_uiAlphaBlend(RGBA(0x10, 0xDD, 0x20, 0x99), uiContext->widgetColor);
    slider.rColor    = ls_uiAlphaBlend(RGBA(0xDD, 0x10, 0x20, 0x99), uiContext->widgetColor);
    
    RegionTimer frameTime = {};
    
    unistring buttonTestString = ls_unistrFromAscii("Hello gouda! 'Bitch'");
    
    b32 Running = TRUE;
    u32 lastFrameTime = 0;
    
    while(Running)
    {
        RegionTimerBegin(frameTime);
        
        UserInput.Keyboard.prevState = UserInput.Keyboard.currentState;
        UserInput.Keyboard.hasPrintableKey = FALSE;
        UserInput.Keyboard.keyCodepoint    = 0;
        
        UserInput.Mouse.prevPos          = UserInput.Mouse.currPos;
        UserInput.Mouse.wasLeftPressed   = UserInput.Mouse.isLeftPressed;
        UserInput.Mouse.wasRightPressed  = UserInput.Mouse.isRightPressed;
        UserInput.Mouse.wasMiddlePressed = UserInput.Mouse.isMiddlePressed;
        
        // Process Input
        MSG Msg;
        
        while (PeekMessageA(&Msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
        }
        
        //NOTE:TEST
        ls_uiBackground(uiContext);
        
        //TODO: Make sure UI bounding boxes for input are pixel perfect aligned.
        ls_uiButton(uiContext, button, 100, 700, 80, 20);
        
        
        if(showText)  //NOTETODO: This is just a test, controlled by a global var.
        {
            ls_uiSelectFontByFontSize(uiContext, FS_EXTRALARGE);
            ls_uiGlyphString(uiContext, 200, 700, buttonTestString, RGB(0xbf, 0x41, 0x37));
        }
        
        if(LeftClick && MouseInRect(100, 600, 1000, 36)) { box.isSelected = TRUE; box.isCaretOn = TRUE; }
        ls_uiTextBox(uiContext, &box, 100, 600, 1000, 36);
        
        ls_uiListBox(uiContext, &listBox, 100, 400, 200, 36);
        
        ls_uiSlider(uiContext, &slider, 400, 420, 400, 72);
        
#if 1//_DEBUG
        char buff[32] = {};
        ls_itoa_t(lastFrameTime, buff, 32);
        ls_uiGlyphString(uiContext, 1240, 820, ls_unistrFromAscii(buff), RGBg(0xEE));
#endif
        
        ls_uiRender(uiContext);
        //NOTE:TEST
        
        State.hasMouseClicked = FALSE;
        
        GetSystemTime(&endT);
        
        State.timePassed += (endT.wSecond - beginT.wSecond);
        if(State.timePassed >= 30)
        {
            State.timePassed = 0;
            //TODO: Reinstate this. SaveState();
        }
        
        beginT = endT;
        
        RegionTimerEnd(frameTime);
        uiContext->dt = RegionTimerGet(frameTime);
        lastFrameTime = uiContext->dt;
    }
    
    return 0;
}
