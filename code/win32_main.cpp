#define LS_WINDOWS_IMPLEMENTATION
#include "lsWindows.h"
#undef LS_WINDOWS_IMPLEMENTATION

#define LS_ARENA_IMPLEMENTATION
#include "lsArena.h"
#undef LS_ARENA_IMPLEMENTATION

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


#if 1
#include "lsInput.h"

#define LS_UI_IMPLEMENTATION
#include "lsUI.h"
#undef LS_UI_IMPLEMENTATION

#else

#include "Input.cpp"
#include "ui.cpp"

#endif

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
#include "SaveState.cpp"

#include "AssetLoader.cpp"

b32 ProgramExitOnButton(UIContext *cxt, void *data) { SendMessageA(MainWindow, WM_DESTROY, 0, 0); return FALSE; }

void CopyState(UIContext *cxt, ProgramState *FromState, ProgramState *ToState)
{
    //NOTE: Copy Init Page
    InitPage *curr = FromState->Init;
    InitPage *dest = ToState->Init;
    
    dest->Mobs.selectedIndex   = curr->Mobs.selectedIndex;
    dest->Allies.selectedIndex = curr->Allies.selectedIndex;
    
    for(u32 i = 0; i < PARTY_NUM; i++)
    { ls_uiTextBoxSet(cxt, dest->PlayerInit + i, curr->PlayerInit[i].text); }
    
    for(u32 i = 0; i < ALLY_NUM; i++)
    {
        InitField *From = curr->AllyFields + i;
        InitField *To   = dest->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_uiTextBoxSet(cxt, To->editFields + j, From->editFields[j].text); }
        
        ls_uiTextBoxSet(cxt, &To->maxLife, From->maxLife.text);
        ls_uiTextBoxSet(cxt, &To->addName, From->addName.text);
        ls_uiTextBoxSet(cxt, &To->addInit, From->addInit.text);
        
        To->isAdding = From->isAdding;
        To->ID       = From->ID;
    }
    
    for(u32 i = 0; i < MOB_NUM; i++)
    {
        InitField *From = curr->MobFields + i;
        InitField *To   = dest->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_uiTextBoxSet(cxt, To->editFields + j, From->editFields[j].text); }
        
        ls_uiTextBoxSet(cxt, &To->maxLife, From->maxLife.text);
        ls_uiTextBoxSet(cxt, &To->addName, From->addName.text);
        ls_uiTextBoxSet(cxt, &To->addInit, From->addInit.text);
        
        To->isAdding = From->isAdding;
        To->ID = From->ID;
    }
    
    for(u32 i = 0; i < ORDER_NUM; i++)
    {
        Order *From = curr->OrderFields + i;
        Order *To   = dest->OrderFields + i;
        
        ls_unistrSet(&To->field.text, From->field.text);
        To->field.currValue = From->field.currValue;
        To->field.maxValue  = From->field.maxValue;
        To->field.minValue  = From->field.minValue;
        To->field.currPos   = From->field.currPos;
        To->field.lColor    = From->field.lColor;
        To->field.rColor    = From->field.rColor;
        
        To->ID = From->ID;
    }
    
    dest->turnsInRound = curr->turnsInRound;
    dest->orderAdjust  = curr->orderAdjust;
    
    ls_uiTextBoxSet(cxt, &dest->RoundCounter, curr->RoundCounter.text);
    dest->roundCount = curr->roundCount;
    
    ls_uiTextBoxSet(cxt, &dest->Current, curr->Current.text);
    dest->currIdx = curr->currIdx;
    
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *From = curr->Counters + i;
        Counter *To   = dest->Counters + i;
        
        ls_uiTextBoxSet(cxt, &To->name, From->name.text);
        ls_uiTextBoxSet(cxt, &To->rounds, From->rounds.text);
        
        To->roundsLeft      = From->roundsLeft;
        To->startIdxInOrder = From->startIdxInOrder;
        To->turnCounter     = From->turnCounter;
        To->isActive        = From->isActive;
    }
    
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrow *From = curr->Throwers + i;
        DiceThrow *To   = dest->Throwers + i;
        
        ls_uiTextBoxSet(cxt, &To->name,   From->name.text);
        ls_uiTextBoxSet(cxt, &To->toHit,  From->toHit.text);
        ls_uiTextBoxSet(cxt, &To->hitRes, From->hitRes.text);
        ls_uiTextBoxSet(cxt, &To->damage, From->damage.text);
        ls_uiTextBoxSet(cxt, &To->dmgRes, From->dmgRes.text);
    }
    
    ls_uiTextBoxSet(cxt, &dest->GeneralThrower.name,   curr->GeneralThrower.name.text);
    ls_uiTextBoxSet(cxt, &dest->GeneralThrower.toHit,  curr->GeneralThrower.toHit.text);
    ls_uiTextBoxSet(cxt, &dest->GeneralThrower.hitRes, curr->GeneralThrower.hitRes.text);
    ls_uiTextBoxSet(cxt, &dest->GeneralThrower.damage, curr->GeneralThrower.damage.text);
    ls_uiTextBoxSet(cxt, &dest->GeneralThrower.dmgRes, curr->GeneralThrower.dmgRes.text);
   
    dest->EncounterSel.selectedIndex = curr->EncounterSel.selectedIndex;
    
    
    //NOTE: Copy General Info
    ToState->inBattle = FromState->inBattle;
}

int WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR cmdLine, int nCmdShow)
{
    MainInstance = hInst;
    
    //------------------------------------------------------
    //NOTE: This shit is necessary to request millisecond-precision sleeps.
    //      An equivalent call to timeEndPeriod should happen at the end. It technically
    //      Doesn't in this program, because we use it every frame, so we don't re-call it.
    //      But I'm pointing it out for possible microsoft weirdness...
    TIMECAPS tc = {};
    MMRESULT res = timeGetDevCaps(&tc, sizeof(TIMECAPS));
    res = timeBeginPeriod(tc.wPeriodMin);
    //------------------------------------------------------
    
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
    
    //-------------------------------------
    //NOTE: Switch to global memory arena 
    //      for general allocations
    
    globalArena = ls_arenaCreate(MBytes(8));
    fileArena   = ls_arenaCreate(MBytes(4));
    stateArena  = ls_arenaCreate(MBytes(6));
    saveArena   = ls_arenaCreate(MBytes(4));
    renderArena = ls_arenaCreate(KBytes(8));
    
    ls_arenaUse(globalArena);
    //------------
    
    
    //TODO Hardcoded
    const int windowWidth = 1280;
    const int windowHeight = 860;
    UIContext *uiContext = ls_uiInitDefaultContext(BackBuffer, windowWidth, windowHeight);
    MainWindow = ls_uiCreateWindow(MainInstance, uiContext);
    
    ls_uiAddOnDestroyCallback(uiContext, SaveState);
    
    
    loadAssetFile(uiContext, ls_strConstant((char *)"assetFile"));
    
    UIButton closeButton = {};
    closeButton.style    = UIBUTTON_BMP;
    closeButton.bmpData  = pixelButtonData;
    closeButton.bmpW     = pixelButtonWidth;
    closeButton.bmpH     = pixelButtonHeight;
    closeButton.onClick  = ProgramExitOnButton;
    
    // -----------
    // STYLE MENU
    UIButton styleDefaultBtn = {};
    styleDefaultBtn.style   = UIBUTTON_TEXT_NOBORDER;
    styleDefaultBtn.name    = ls_unistrFromUTF32(U"Default");
    styleDefaultBtn.onClick = selectStyleDefault;
    
    UIButton stylePranaBtn  = {};
    stylePranaBtn.style     = UIBUTTON_TEXT_NOBORDER;
    stylePranaBtn.name      = ls_unistrFromUTF32(U"Prana");
    stylePranaBtn.onClick   = selectStylePrana;
    
    UIMenu styleSubMenu     = {};
    ls_uiMenuAddItem(&styleSubMenu, styleDefaultBtn);
    ls_uiMenuAddItem(&styleSubMenu, stylePranaBtn);
    
    UIButton styleMenuBtn   = {};
    styleMenuBtn.style         = UIBUTTON_TEXT_NOBORDER;
    styleMenuBtn.name          = ls_unistrFromUTF32(U"Style");
    styleMenuBtn.onClick       = ls_uiMenuDefaultOnClick;
    //
    // -----------
    
    // -----------
    // THEME MENU
    UIButton themeDefaultBtn = {};
    themeDefaultBtn.style   = UIBUTTON_TEXT_NOBORDER;
    themeDefaultBtn.name    = ls_unistrFromUTF32(U"Default");
    themeDefaultBtn.onClick = selectThemeDefault;
    
    UIButton themeDarkNightBtn  = {};
    themeDarkNightBtn.style     = UIBUTTON_TEXT_NOBORDER;
    themeDarkNightBtn.name      = ls_unistrFromUTF32(U"Dark Night");
    themeDarkNightBtn.onClick   = selectThemeDarkNight;
    
    UIMenu themeSubMenu     = {};
    ls_uiMenuAddItem(&themeSubMenu, themeDefaultBtn);
    ls_uiMenuAddItem(&themeSubMenu, themeDarkNightBtn);
    
    UIButton themeMenuBtn   = {};
    themeMenuBtn.style         = UIBUTTON_TEXT_NOBORDER;
    themeMenuBtn.name          = ls_unistrFromUTF32(U"Theme");
    themeMenuBtn.onClick       = ls_uiMenuDefaultOnClick;
    //
    // -----------
    
    UIMenu WindowMenu       = {};
    WindowMenu.closeWindow  = closeButton;
    WindowMenu.sub          = (UIMenu *)ls_alloc(sizeof(UIMenu) * 32);
    WindowMenu.maxSub       = 32;
    
    ls_uiMenuAddItem(&WindowMenu, styleMenuBtn);
    ls_uiMenuAddSub(&WindowMenu, styleSubMenu, 0);
    
    ls_uiMenuAddItem(&WindowMenu, themeMenuBtn);
    ls_uiMenuAddSub(&WindowMenu, themeSubMenu, 1);
    
    State.isInitialized = TRUE;
    
    SYSTEMTIME endT, beginT;
    GetSystemTime(&beginT);
    
    
    ls_arenaUse(stateArena);
    
    //NOTE: Initialize State and Undo States
    State.Init = (InitPage *)ls_alloc(sizeof(InitPage));
    SetInitTab(uiContext, &State);
    
    //NOTE: Single block allocation for all Init Pages.
    InitPage *UndoInitPages = (InitPage *)ls_alloc(sizeof(InitPage)*MAX_UNDO_STATES);
    
    
    //TODO: I had to nerf frame-by-frame initialization because LoadState needs to load the entire
    //      undo chain, even if it was unused. And all at once. Want to try and fix it with multithreading.
    for(u32 i = 0; i < MAX_UNDO_STATES; i++)
    {
        UndoStates[i].Init = UndoInitPages + i;
        SetInitTab(uiContext, UndoStates + i);
    }
    
    ls_arenaUse(globalArena);
    
    //NOTE: The state HAS to be loaded after the InitTab 
    //      has ben Initialized to allow data to be properly set.
    b32 result = LoadState(uiContext);
    
    //NOTE: We initialize the first Undo State to a valid setting
    //TODO: I've now added serialization of undo-chains, also all undo states are initialized by default,
    //      So there's no point in trying to Copy the state???
    //CopyState(uiContext, &State, UndoStates);
    
    //TODO: But if I've loaded an undo chain it means it makes sense to load the State with the "current" Undo state
    CopyState(uiContext, UndoStates + matchingUndoIdx, &State);
    
    
    RegionTimer frameTime = {};
    
    b32 Running = TRUE;
    unistring frameTimeString = ls_unistrAlloc(8);
    b32 showDebug = FALSE;
    b32 userInputConsumed = FALSE;
    
#if 0
    UITextBox tb = {};
    UIListBox lb = {};
    UIButton  bt = {};
    UISlider  sl = {};
    
    Color lColor      = ls_uiAlphaBlend(RGBA(0x10, 0xDD, 0x20, 0x99), uiContext->widgetColor);
    Color rColor      = ls_uiAlphaBlend(RGBA(0xF0, 0xFF, 0x3D, 0x99), uiContext->widgetColor);
    sl = ls_uiSliderInit(NULL, 100, -30, 1.0, SL_BOX, lColor, rColor);
    
    bt.style     = UIBUTTON_TEXT;
    bt.name      = ls_unistrFromUTF32(U"Button");
    bt.onClick   = [](UIContext *, void *) -> b32 { return FALSE; };
    bt.onHold    = [](UIContext *, void *) -> b32 { return FALSE; };
    bt.data      = 0x0;
    
    ls_uiListBoxAddEntry(uiContext, &lb, "Item 1___");
    ls_uiListBoxAddEntry(uiContext, &lb, "Item 2___");
    ls_uiTextBoxSet(uiContext, &tb, ls_unistrConstant(U""));
#endif
    while(Running)
    {
        ls_uiFrameBegin(uiContext);
        
        //NOTE: If any user input was consumed in the previous frame, than we advance the UndoStates.
        //      The first frame is always registered, so the first Undo State is always valid.
        if(userInputConsumed == TRUE)
        {
            matchingUndoIdx = (matchingUndoIdx + 1) % MAX_UNDO_STATES;
            CopyState(uiContext, &State, UndoStates + matchingUndoIdx);
            
            if(distanceFromOld < (MAX_UNDO_STATES-1)) { distanceFromOld += 1; }
            
            //NOTE: If an operation is performed, that is the new NOW, the new Present, 
            //      and no other REDOs can be performed
            distanceFromNow = 0;
        }
        
#if 0
        //NOTE: Render The Frame
        ls_uiBackground(uiContext);
#endif
        
#if 1
        //NOTE: Render The Window Menu
        ls_uiMenu(uiContext, &WindowMenu, 0, uiContext->windowHeight-20, uiContext->windowWidth, 20);
        
        
        userInputConsumed = DrawInitTab(uiContext);
#else
        ls_uiTextBox(uiContext, &tb, 400, 400, 500, 30);
        
        ls_uiSelectFontByFontSize(uiContext, FS_SMALL);
        ls_uiLabel(uiContext, U"--test--", 630, 430);
        ls_uiListBox(uiContext, &lb, 580, 500, 100, 20);
        ls_uiButton(uiContext, &bt, 600, 540, 100, 20);
        ls_uiSlider(uiContext, &sl, 460, 580, 300, 20);
        
        //userInputConsumed = FALSE;
#endif
        if(!uiContext->hasReceivedInput && !uiContext->isDragging)
        {
            for(u32 i = 0; i < RENDER_GROUP_COUNT; i++)
            {
                ls_stackClear(&uiContext->renderGroups[i].RenderCommands[0]);
                ls_stackClear(&uiContext->renderGroups[i].RenderCommands[1]);
                ls_stackClear(&uiContext->renderGroups[i].RenderCommands[2]);
            }
            
            Sleep(32);
        }
        else
        {
            if((KeyPress(keyMap::Z) && KeyHeld(keyMap::Control)) || undoRequest)
            {
                undoRequest = FALSE;
                
                //NOTE: We only undo when there's available states to undo into (avoid rotation).
                if(distanceFromOld != 0)
                {
                    u32 undoIdx = matchingUndoIdx - 1;
                    if(matchingUndoIdx == 0) { undoIdx = MAX_UNDO_STATES-1; }
                    
                    CopyState(uiContext, UndoStates + undoIdx, &State);
                    matchingUndoIdx  = undoIdx;
                    distanceFromOld -= 1;
                    distanceFromNow += 1;
                }
            }
            
            if((KeyPress(keyMap::Y) && KeyHeld(keyMap::Control)) || redoRequest)
            {
                redoRequest = FALSE;
                
                //NOTE: We only redo if we have previously perfomed an undo.
                if(distanceFromNow > 0)
                {
                    u32 redoIdx = (matchingUndoIdx + 1) % MAX_UNDO_STATES;
                    CopyState(uiContext, UndoStates + redoIdx, &State);
                    
                    matchingUndoIdx  = redoIdx;
                    distanceFromOld += 1;
                    distanceFromNow -= 1;
                }
            }
            
            //NOTE: If user clicked somewhere, but nothing set the focus, then we should reset the focus
            if(LeftClick && !uiContext->focusWasSetThisFrame)
            { uiContext->currentFocus = 0; }
            
            
            //NOTE: Dragging code, only happens when menu is selected.
            //TODO: Dragging while interacting with menu items! /Separate items region from draggable region!
            if(LeftClick && uiContext->currentFocus == (u64 *)&WindowMenu)
            { 
                uiContext->isDragging = TRUE;
                POINT currMouse = {};
                GetCursorPos(&currMouse);
                uiContext->prevMousePosX = currMouse.x;
                uiContext->prevMousePosY = currMouse.y;
            }
            if(uiContext->isDragging && LeftHold)
            { 
                MouseInput *Mouse = &UserInput.Mouse;
                
                POINT currMouse = {};
                GetCursorPos(&currMouse);
                
                POINT prevMouse = { uiContext->prevMousePosX, uiContext->prevMousePosY };
                
                SHORT newX = prevMouse.x - currMouse.x;
                SHORT newY = prevMouse.y - currMouse.y;
                
                SHORT newWinX = uiContext->windowPosX - newX;
                SHORT newWinY = uiContext->windowPosY - newY;
                
                uiContext->windowPosX = newWinX;
                uiContext->windowPosY = newWinY;
                
                uiContext->prevMousePosX  = currMouse.x;
                uiContext->prevMousePosY  = currMouse.y;
                
                SetWindowPos(MainWindow, 0, newWinX, newWinY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
                
                if(LeftUp) { uiContext->isDragging = FALSE; }
            }
            
            
            if(LeftUp || RightUp || MiddleUp)
            { uiContext->mouseCapture = 0; }
            
            //ls_uiSlider(uiContext, &sldr, 400, 300, 300, 20);
            
            // ----------------
            // Render Everything
            ls_arenaUse(renderArena);
            
            ls_uiRender(uiContext);
            
            ls_arenaUse(globalArena);
            ls_arenaClear(renderArena);
            //
            // ----------------
            
        }
        
        if(KeyPress(keyMap::F12)) { showDebug = !showDebug; }
        
        if(showDebug)
        {
            ls_uiFillRect(uiContext, 1248, 760, 20, 20, 0, uiContext->width, 0, uiContext->height, uiContext->backgroundColor);
            ls_unistrFromInt_t(&frameTimeString, uiContext->dt);
            ls_uiGlyphString(uiContext, 1248, 760, 0, uiContext->width, 0, uiContext->height, frameTimeString, RGBg(0xEE));
            
#if 0
            ls_uiFillRect(uiContext, (uiContext->width/2)-5, 0, 10, 40, 0, uiContext->width, 0, uiContext->height, RGB(255, 255, 255));
            ls_uiFillRect(uiContext, (uiContext->width/2)-5, uiContext->height-40, 10, 40, 0, uiContext->width, 0, uiContext->height, RGB(255, 255, 255));
            ls_uiFillRect(uiContext, 0, (uiContext->height/2)-5, 40, 10, 0, uiContext->width, 0, uiContext->height, RGB(255, 255, 255));
            ls_uiFillRect(uiContext, uiContext->width-40, (uiContext->height/2)-5, 40, 10, 0, uiContext->width, 0, uiContext->height, RGB(255, 255, 255));
#endif
            
            
        }
        
        State.hasMouseClicked = FALSE;
        
        GetSystemTime(&endT);
        
        
        State.timePassed += (endT.wSecond - beginT.wSecond);
        if(State.timePassed >= 30)
        {
            State.timePassed = 0;
            SaveState(NULL);
        }
        
        beginT = endT;
        
        const u32 frameLock = 16;
        ls_uiFrameEnd(uiContext, frameLock);
    }
    
    //NOTE: Technically useless, just reminding myself of this function's existance
    res = timeEndPeriod(tc.wPeriodMin);
    return 0;
}
