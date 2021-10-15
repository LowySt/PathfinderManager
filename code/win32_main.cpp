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

#define LS_SORT_IMPLEMENTATION
#include "lsSort.h"
#undef LS_SORT_IMPLEMENTATION

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


#include "Input.cpp"
#include "ui.cpp"

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

#include "AssetLoader.cpp"

HBITMAP closeButton;
HDC     closeButtonDC;
void    *closeButtBackbuff;

HDC WindowDC;
HDC BackBufferDC;
u8  *BackBuffer;

LRESULT WindowProc(HWND h, UINT msg, WPARAM w, LPARAM l)
{
    LRESULT Result = 0;
    
    MouseInput *Mouse = &UserInput.Mouse;
    
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
            //Repeat is the first 16 bits of the LPARAM. Bits [0-15];
            u16 rep = (u16)l;
            
            switch(w)
            { 
                case VK_F2:      KeySetAndRepeat(keyMap::F2, rep);        break;
                
                case VK_DOWN:    KeySetAndRepeat(keyMap::DArrow, rep);    break;
                case VK_UP:      KeySetAndRepeat(keyMap::UArrow, rep);    break;
                case VK_LEFT:    KeySetAndRepeat(keyMap::LArrow, rep);    break;
                case VK_RIGHT:   KeySetAndRepeat(keyMap::RArrow, rep);    break;
                
                case VK_RETURN:  KeySetAndRepeat(keyMap::Enter, rep);     break;
                case VK_BACK:    KeySetAndRepeat(keyMap::Backspace, rep); break;
                case VK_DELETE:  KeySetAndRepeat(keyMap::Delete, rep);    break;
                case VK_HOME:    KeySetAndRepeat(keyMap::Home, rep);      break;
                case VK_END:     KeySetAndRepeat(keyMap::End, rep);       break;
                case VK_CONTROL: KeySetAndRepeat(keyMap::Control, rep);   break;
                case VK_SHIFT:   KeySetAndRepeat(keyMap::Shift, rep);     break; //TODO: Differentiate L/R Shift
                
                case 'A':        KeySetAndRepeat(keyMap::A, rep);         break;
                case 'C':        KeySetAndRepeat(keyMap::C, rep);         break;
                case 'V':        KeySetAndRepeat(keyMap::V, rep);         break;
                case 'G':        KeySetAndRepeat(keyMap::G, rep);         break;
            }
            
        } break;
        
        case WM_KEYUP:
        {
            switch(w)
            { 
                case VK_F2:      KeyUnset(keyMap::F2);        break;
                
                case VK_DOWN:    KeyUnset(keyMap::DArrow);    break;
                case VK_UP:      KeyUnset(keyMap::UArrow);    break;
                case VK_LEFT:    KeyUnset(keyMap::LArrow);    break;
                case VK_RIGHT:   KeyUnset(keyMap::RArrow);    break;
                
                case VK_RETURN:  KeyUnset(keyMap::Enter);     break;
                case VK_BACK:    KeyUnset(keyMap::Backspace); break;
                case VK_DELETE:  KeyUnset(keyMap::Delete);    break;
                case VK_HOME:    KeyUnset(keyMap::Home);      break;
                case VK_END:     KeyUnset(keyMap::End);       break;
                case VK_CONTROL: KeyUnset(keyMap::Control);   break;
                case VK_SHIFT:   KeyUnset(keyMap::Shift);     break; //TODO: Differentiate L/R Shift
                
                case 'A':        KeyUnset(keyMap::A);         break;
                case 'C':        KeyUnset(keyMap::C);         break;
                case 'V':        KeyUnset(keyMap::V);         break;
                case 'G':        KeyUnset(keyMap::G);         break;
            }
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
        
        case WM_RBUTTONDOWN: { Mouse->isRightPressed = TRUE; } break;
        case WM_RBUTTONUP:   { Mouse->isRightPressed = FALSE; } break;
        
        case WM_MOUSEMOVE:
        {
            POINTS currMouseClient = *((POINTS *)&l);
            Mouse->currPos = { currMouseClient.x, State.windowHeight - currMouseClient.y };
            
            //TODO:NOTE: Page says to return 0, but DO i HAVE to?
        } break;
        
        case WM_DESTROY:
        {
            SaveState();
            ExitProcess(0);
        } break;
        
        default: { return DefWindowProcA(h, msg, w, l); }
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
    WindowClass.lpszClassName = "WndClass";
    WindowClass.hCursor = cursor;
    
    if (!RegisterClassA(&WindowClass))
    {
        DWORD Error = GetLastError();
        ls_printf("When Registering WindowClass in Win32_SetupScreen got error: %d", Error);
    }
}

HWND CreateWindow()
{
    u32 style = LS_THIN_BORDER | LS_POPUP;// | LS_VISIBLE; //| LS_OVERLAPPEDWINDOW;
    BOOL Result;
    
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    
    int windowWidth = 1280;
    int windowHeight = 860;
    const int taskbarHeight = 20;
    
    int spaceX = (screenWidth - windowWidth) / 2;
    int spaceY = ((screenHeight - windowHeight) / 2);// - taskbarHeight;
    if(spaceX < 0) { spaceX = 0; }
    if(spaceY < 0) { spaceY = 0; }
    
    //NOTE:TODO: Hardcoded!!
    State.windowWidth = 1280;
    State.windowHeight = 800;
    
    HWND WindowHandle;
    if ((WindowHandle = CreateWindowExA(0, "WndClass",
                                        "PCMan", style,
                                        spaceX, spaceY, //CW_USEDEFAULT, CW_USEDEFAULT,
                                        State.windowWidth, State.windowHeight,
                                        0, 0, MainInstance, 0)) == nullptr)
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
    
    WindowDC           = GetDC(WindowHandle);
    BackBufferDC       = CreateCompatibleDC(WindowDC);
    HBITMAP DibSection = CreateDIBSection(BackBufferDC, &BackBufferInfo,
                                          DIB_RGB_COLORS, (void **)&BackBuffer, NULL, 0);
    SelectObject(BackBufferDC, DibSection);
    
    State.currWindowPos = { (s16)spaceX, (s16)spaceY }; //NOTE:TODO: Hardcoded!!
    
    return WindowHandle;
}

void windows_Render()
{
    InvalidateRect(MainWindow, NULL, TRUE);
}

void ProgramExitOnButton(UIContext *cxt, void *data) { SendMessageA(MainWindow, WM_DESTROY, 0, 0); }

int WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR cmdLine, int nCmdShow)
{
    MainInstance = hInst;
    
    windows_initRegionTimer(RT_MILLISECOND);
    
#if 0 //RDRAND was introduced in IvyBridge (2012). It may be too new to put in.
    u64 rand_init_state = 0;
    u64 rand_init_seq = 0;
    _rdseed64_step(&rand_init_state);
    _rdseed64_step(&rand_init_seq);
#else
    u64 rand_init_state = windows_GetUnix64Time();
    u64 rand_init_seq = windows_GetWindowsTimeInMicrosec();
#endif
    
    pcg32_seed(&pcg32_global, rand_init_state, rand_init_seq);
    
    RegisterWindow();
    
    MainWindow = CreateWindow();
    
    UserInput.Keyboard.getClipboard = win32_GetClipboard;
    UserInput.Keyboard.setClipboard = win32_SetClipboard;
    
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
    uiContext->invWidgetColor  = RGBg(0xBA);
    uiContext->invTextColor    = RGBg(0x33);
    
    loadAssetFile(uiContext, ls_strConstant((char *)"assetFile"));
    
    UIButton closeButton = {};
    closeButton.style    = UIBUTTON_BMP;
    closeButton.bmpData  = pixelButtonData;
    closeButton.bmpW     = pixelButtonWidth;
    closeButton.bmpH     = pixelButtonHeight;
    closeButton.onClick  = ProgramExitOnButton;
    
    UIButton styleDefaultBtn = {};
    styleDefaultBtn.style   = UIBUTTON_TEXT_NOBORDER;
    styleDefaultBtn.name    = ls_unistrFromUTF32(U"Default");
    styleDefaultBtn.onClick = selectStyleDefault;
    
    UIButton stylePranaBtn  = {};
    stylePranaBtn.style     = UIBUTTON_TEXT_NOBORDER;
    stylePranaBtn.name      = ls_unistrFromUTF32(U"Prana");
    stylePranaBtn.onClick   = selectStylePrana;
    
    UIMenu styleSubMenu     = {};
    ls_uiMenuAddItem(uiContext, &styleSubMenu, styleDefaultBtn);
    ls_uiMenuAddItem(uiContext, &styleSubMenu, stylePranaBtn);
    
    UIButton styleMenuBtn   = {};
    styleMenuBtn.style         = UIBUTTON_TEXT_NOBORDER;
    styleMenuBtn.name          = ls_unistrFromUTF32(U"Style");
    styleMenuBtn.onClick       = ls_uiMenuDefaultOnClick;
    
    UIMenu WindowMenu       = {};
    WindowMenu.closeWindow  = closeButton;
    WindowMenu.sub          = (UIMenu *)ls_alloc(sizeof(UIMenu) * 32);
    WindowMenu.maxSub       = 32;
    
    ls_uiMenuAddItem(uiContext, &WindowMenu, styleMenuBtn);
    ls_uiMenuAddSub(uiContext, &WindowMenu, styleSubMenu, 0);
    
    
    ls_uiPushScissor(uiContext, 0, 0, State.windowWidth, State.windowHeight);
    
    State.isInitialized = TRUE;
    
    SYSTEMTIME endT, beginT;
    GetSystemTime(&beginT);
    
    State.Init = (InitPage *)ls_alloc(sizeof(InitPage));
    SetInitTab(uiContext);
    
    //NOTE: The state HAS to be loaded after the InitTab 
    //      has ben Initialized to allow data to be properly set.
    b32 result = LoadState(uiContext);
    
    RegionTimer frameTime = {};
    b32 Running = TRUE;
    u32 lastFrameTime = 0;
    
    b32 isStartup = TRUE;
    while(Running)
    {
        RegionTimerBegin(frameTime);
        
        UserInput.Keyboard.prevState = UserInput.Keyboard.currentState;
        UserInput.Keyboard.repeatState = {};
        
        UserInput.Keyboard.hasPrintableKey = FALSE;
        UserInput.Keyboard.keyCodepoint    = 0;
        
        UserInput.Mouse.prevPos          = UserInput.Mouse.currPos;
        UserInput.Mouse.wasLeftPressed   = UserInput.Mouse.isLeftPressed;
        UserInput.Mouse.wasRightPressed  = UserInput.Mouse.isRightPressed;
        UserInput.Mouse.wasMiddlePressed = UserInput.Mouse.isMiddlePressed;
        
        uiContext->focusWasSetThisFrame = FALSE;
        uiContext->lastFocus            = uiContext->currentFocus;
        if(uiContext->nextFrameFocusChange == TRUE)
        {
            uiContext->currentFocus = uiContext->nextFrameFocus;
            uiContext->lastFocus    = uiContext->currentFocus;
            uiContext->nextFrameFocusChange = FALSE;
        }
        
        // Process Input
        MSG Msg;
        while (PeekMessageA(&Msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
        }
        
        //NOTE: Window starts hidden, and then is shown after the first frame, 
        //      to avoid flashing because initially the frame buffer is all white.
        if(isStartup) { ShowWindow(MainWindow, SW_SHOW); isStartup = FALSE; }
        
        //NOTE: Render The Frame
        ls_uiBackground(uiContext);
        
        //NOTE: Render The Window Menu
        ls_uiMenu(uiContext, &WindowMenu, -1, State.windowHeight-20, State.windowWidth, 22);
        
        DrawInitTab(uiContext);
        
#if 1//_DEBUG
        char buff[32] = {};
        ls_itoa_t(lastFrameTime, buff, 32);
        ls_uiGlyphString(uiContext, 1240, 760, ls_unistrFromAscii(buff), RGBg(0xEE));
#endif
        
        //NOTE: If user clicked somewhere, but nothing set the focus, then we should reset the focus
        if(LeftClick && !uiContext->focusWasSetThisFrame)
        { uiContext->currentFocus = 0; }
        
        
        //NOTE: Dragging code, only happens when menu is selected.
        //TODO: Dragging while interacting with menu items! /Separate items region from draggable region!
        if(LeftClick && uiContext->currentFocus == (u64 *)&WindowMenu)
        { 
            State.isDragging = TRUE;
            POINT currMouse = {};
            GetCursorPos(&currMouse);
            State.prevMousePos = currMouse;
        }
        if(State.isDragging && LeftHold)
        { 
            MouseInput *Mouse = &UserInput.Mouse;
            
            POINT currMouse = {};
            GetCursorPos(&currMouse);
            
            POINT prevMouse = State.prevMousePos;
            
            SHORT newX = prevMouse.x - currMouse.x;
            SHORT newY = prevMouse.y - currMouse.y;
            
            SHORT newWinX = State.currWindowPos.x - newX;
            SHORT newWinY = State.currWindowPos.y - newY;
            
            State.currWindowPos = { newWinX, newWinY };
            State.prevMousePos  = currMouse; 
            SetWindowPos(MainWindow, 0, newWinX, newWinY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            
            if(LeftUp) { State.isDragging = FALSE; }
        }
        
        
        
        if(LeftUp || RightUp || MiddleUp)
        { uiContext->mouseCapture = 0; }
        
        ls_uiRender(uiContext); 
        
        State.hasMouseClicked = FALSE;
        
        GetSystemTime(&endT);
        
        //TODO:NOTE: DEBUG TEMPORARY
        if(KeyPress(keyMap::F2))
        { 
            SaveState(); 
        }
        
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
