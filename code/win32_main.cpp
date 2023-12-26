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

#define LS_LOG_IMPLEMENTATION
#include "lsLog.h"
#undef LS_LOG_IMPLEMENTATION

#include "lsArray.h"

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

#include "lsInput.h"

//TODO: The UI System is NOT actually properly using arenas
//      See renderArena being picked but it's always empty
//      Because the render commands aren't actually being placed there...
static Arena globalArena;
static Arena fileArena;
static Arena stateArena;
static Arena saveArena;
static Arena renderArena;
static Arena frameArena;

static Arena compendiumArena;
static Arena compTempArena;


#define LS_UI_IMPLEMENTATION
#include "lsUI.h"
#undef LS_UI_IMPLEMENTATION

#include "Init.h"
#include "Class.h"
#include "PlayerChar.h"
#include "StateGlobals.h"
#include "buttonImage.h"
#include "status_icons.h"
#include "Equip.h"

#include "CompendiumSkills.h"
#include "CompendiumTalents.h"
#include "CompendiumArchetypes.h"

#include "themePicker.cpp"
#include "diceRoller.cpp"

#include "Compendium.cpp"
#include "CompendiumSkills.cpp"
#include "CompendiumTalents.cpp"
#include "CompendiumArchetypes.cpp"

#include "Class.cpp"
#include "Feats.cpp"
#include "PlayerChar.cpp"
#include "Init.cpp"
#include "PCTab.cpp"
#include "FeatsTab.cpp"
#include "SaveState.cpp"

#include "AssetLoader.cpp"

#if _DEBUG
#include "test.cpp"
#endif

b32 ProgramExitOnButton(UIContext *c, void *data) { SendMessageA(MainWindow, WM_DESTROY, 0, 0); return FALSE; }
b32 CompendiumExitOnButton(UIContext *c, void *data){ ShowWindow(CompendiumWindow, SW_HIDE); return FALSE; }

//TODO: Compendium does not answer keyboard input when MainWindow is minimized. Why???
//      Do I have a fucked up keyboard state???
b32 ProgramMinimizeOnButton(UIContext *c, void *data) { 
    //NOTE: We have to send an LButtonUp, else our Input handling will be confused
    //      and not register the un-pressing of the left button.
    SendMessageA(c->Window, WM_LBUTTONUP, 0, 0);
    ShowWindow(c->Window, SW_MINIMIZE);
    return FALSE;
}

b32 ProgramOpenCompendium(UIContext *c, void *data)
{
    if(CompendiumWindow)
    {
        ShowWindow(CompendiumWindow, SW_SHOW);
        c->renderFunc(c);
        return FALSE;
    }
    
    LogMsg(TRUE, "No Compendium Window yet!\n");
    return FALSE;
}

b32 ProgramOpenPlayersSettings(UIContext *c, void *data)
{
    if(State.inBattle) { return FALSE; }
    
    State.arePlayerSettingsOpen = !State.arePlayerSettingsOpen;
    return FALSE;
}

b32 ProgramOpenInfoSettings(UIContext *c, void *data)
{
    State.areInfoSettingsOpen = !State.areInfoSettingsOpen;
    return FALSE;
}

void CopyState(UIContext *cxt, ProgramState *FromState, ProgramState *ToState)
{
    //NOTE: Copy Init Page
    InitPage *curr = FromState->Init;
    InitPage *dest = ToState->Init;
    
    dest->Mobs.selectedIndex   = curr->Mobs.selectedIndex;
    dest->Allies.selectedIndex = curr->Allies.selectedIndex;
    
    for(u32 i = 0; i < party_count; i++)
    { ls_uiTextBoxSet(cxt, dest->PlayerInit + i, curr->PlayerInit[i].text); }
    
    for(u32 i = 0; i < ally_count; i++)
    {
        InitField *From = curr->AllyFields + i;
        InitField *To   = dest->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_uiTextBoxSet(cxt, To->editFields + j, From->editFields[j].text); }
        
        ls_uiTextBoxSet(cxt, &To->maxLife, From->maxLife.text);
        To->ID       = From->ID;
    }
    
    for(u32 i = 0; i < mob_count; i++)
    {
        InitField *From = curr->MobFields + i;
        InitField *To   = dest->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_uiTextBoxSet(cxt, To->editFields + j, From->editFields[j].text); }
        
        ls_uiTextBoxSet(cxt, &To->maxLife, From->maxLife.text);
        To->ID = From->ID;
    }
    
    for(u32 i = 0; i < order_count; i++)
    {
        Order *From = curr->OrderFields + i;
        Order *To   = dest->OrderFields + i;
        
        ls_utf32Set(&To->field.text, From->field.text);
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
        DiceThrowBox *From = curr->Throwers + i;
        DiceThrowBox *To   = dest->Throwers + i;
        
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
    dest->isAdding = curr->isAdding;
    
    //NOTE: Restore state of Add New Buttons
    ls_utf32Set(&dest->addNewMob.name, curr->addNewMob.name);
    dest->addNewMob.onClick = curr->addNewMob.onClick;
    
    ls_utf32Set(&dest->addNewAlly.name, curr->addNewAlly.name);
    dest->addNewAlly.onClick = curr->addNewAlly.onClick;
}

int WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR cmdLine, int nCmdShow)
{
    MainInstance = hInst;
    
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
    
    globalArena     = ls_arenaCreate(MBytes(12));
    fileArena       = ls_arenaCreate(MBytes(4));
    stateArena      = ls_arenaCreate(MBytes(8));
    saveArena       = ls_arenaCreate(MBytes(4));
    renderArena     = ls_arenaCreate(KBytes(8));
    
    frameArena      = ls_arenaCreate(KBytes(8));
    
    //TODO: Make this much smaller. It can be reduced to at least 6 MBytes, probably smaller
    compendiumArena = ls_arenaCreate(MBytes(16));
    compTempArena   = ls_arenaCreate(KBytes(16));
    
    ls_arenaUse(globalArena);
    //------------
    
    ls_logDefaultTypesRegister();
    
#if _DEBUG
    ls_vlogRegister("Order", ls_vlogFormatOrder);
    ls_vlogRegister("UIRect", ls_vlogFormatUIRect);
#endif
    
    //TODO: Hardcoded Compendium Window
    const int compendiumWidth  = 800;
    const int compendiumHeight = 720;
    UIContext *compendiumContext = ls_uiInitDefaultContext(CompendiumBackBuffer, compendiumWidth, compendiumHeight);
    CompendiumWindow = ls_uiCreateWindow(MainInstance, compendiumContext, "Compendium");
    
    //TODO Hardcoded MainWindow
    const int windowWidth = 1280;
    const int windowHeight = 860;
    UIContext *uiContext = ls_uiInitDefaultContext(BackBuffer, windowWidth, windowHeight);
    MainWindow = ls_uiCreateWindow(MainInstance, uiContext, "PCMan");
    
    ls_uiAddOnDestroyCallback(uiContext, SaveState);
    
    loadAssetFile(uiContext, ls_strConstant("assetFile"));
    ls_uiSelectFontByFontSize(uiContext, FS_SMALL);
    
    //NOTETODOHACK:
    compendiumContext->fonts    = uiContext->fonts;
    compendiumContext->numFonts = uiContext->numFonts;
    compendiumContext->currFont = uiContext->currFont;
    
    LoadCompendium(compendiumContext, ls_strConstant("Compendium"));
    
    UIMenu WindowMenu      = {};
    WindowMenu.closeWindow = ls_uiMenuButton(ProgramExitOnButton, closeBtnDataPremulti, closeBtnWidth, closeBtnHeight);
    WindowMenu.minimize    = ls_uiMenuButton(ProgramMinimizeOnButton, minBtnDataPremulti, minBtnWidth, minBtnHeight);
    WindowMenu.itemWidth   = 100;
    
    ls_uiMenuAddSub(uiContext, &WindowMenu, U"Style");
    ls_uiSubMenuAddItem(uiContext, &WindowMenu, 0, U"Default", selectStyleDefault, NULL);
    ls_uiSubMenuAddItem(uiContext, &WindowMenu, 0, U"Prana", selectStylePrana, NULL);
    
    ls_uiMenuAddSub(uiContext, &WindowMenu, U"Theme");
    ls_uiSubMenuAddItem(uiContext, &WindowMenu, 1, U"Default", selectThemeDefault, NULL);
    ls_uiSubMenuAddItem(uiContext, &WindowMenu, 1, U"Dark Night", selectThemeDarkNight, NULL);
    ls_uiSubMenuAddItem(uiContext, &WindowMenu, 1, U"Light", selectThemeLight, NULL);
    ls_uiSubMenuAddItem(uiContext, &WindowMenu, 1, U"User", selectThemeUser, NULL);
    ls_uiSubMenuAddItem(uiContext, &WindowMenu, 1, U"Customize", openThemeColorPicker, NULL);
    
    ls_uiMenuAddSub(uiContext, &WindowMenu, U"Settings");
    State.playerSettingsMenuItem = ls_uiSubMenuAddItem(uiContext, &WindowMenu, 2, 
                                                       U"Players", ProgramOpenPlayersSettings, NULL);
    ls_uiSubMenuAddItem(uiContext, &WindowMenu, 2, U"Info", ProgramOpenInfoSettings, NULL);
    
    ls_uiMenuAddItem(uiContext, &WindowMenu, U"Compendium", ProgramOpenCompendium, NULL);
    
    
    UIMenu CompendiumMenu = {};
    CompendiumMenu.closeWindow  = ls_uiMenuButton(CompendiumExitOnButton, closeBtnData, closeBtnWidth, closeBtnHeight);
    CompendiumMenu.itemWidth    = 120;
    
    ls_uiMenuAddItem(uiContext, &CompendiumMenu, U"Monster Table", CompendiumOpenMonsterTable, NULL);
    ls_uiMenuAddItem(uiContext, &CompendiumMenu, U"NPC Table", CompendiumOpenNPCTable, NULL);
    
    SYSTEMTIME endT, beginT;
    GetSystemTime(&beginT);
    
    ls_arenaUse(stateArena);
    
    //NOTE: Initialize State and Undo States
    State.themePicker.wheel                = ls_uiColorPickerInit(uiContext, &State.themePicker);
    State.themePicker.wheel.pickedColor    = uiContext->backgroundColor;
    
    State.Init = (InitPage *)ls_alloc(sizeof(InitPage));
    SetInitTab(uiContext, &State);
    
    //NOTE: Initialize the mainCachedPage to display the mob page inside init.
    initCachedPage(&mainCachedPage);
    InitCachedTalentEntry(&mainCachedTalent);
    mainCachedPage.talentPage = &mainCachedTalent;
    
    SetMonsterTable(compendiumContext);
    SetNPCTable(compendiumContext);
    
    //NOTE: Single block allocation for all Init Pages.
    InitPage *UndoInitPages = (InitPage *)ls_alloc(sizeof(InitPage)*MAX_UNDO_STATES);
    
    //TODO: I had to nerf frame-by-frame initialization because LoadState needs to load the entire
    //      undo chain, even if it was unused. And all at once. Want to try and fix it with multithreading.
    for(u32 i = 0; i < MAX_UNDO_STATES; i++)
    {
        UndoStates[i].Init = UndoInitPages + i;
        SetInitTab(uiContext, UndoStates + i);
    }
    
    //NOTE: Set the Party Settings TextBoxes and Buttons
    for(s32 i = 0; i < MAX_PARTY_NUM; i++)
    { ls_uiTextBoxInit(uiContext, State.PartyName + i, 16); }
    ls_uiTextBoxSet(uiContext, &State.PartyName[0], ls_utf32Constant(U"Efrea"));
    ls_uiTextBoxSet(uiContext, &State.PartyName[1], ls_utf32Constant(U"Nick"));
    ls_uiTextBoxSet(uiContext, &State.PartyName[2], ls_utf32Constant(U"Clovis"));
    for(s32 i = party_count; i < MAX_PARTY_NUM; i++)
    { ls_uiTextBoxSet(uiContext, State.PartyName + i, ls_utf32Constant(U"XXXXX")); }
    
    ls_uiButtonInit(uiContext, &State.addPartyMember, UIBUTTON_CLASSIC, U"+", OnClickAddPlayerToState);
    ls_uiButtonInit(uiContext, &State.removePartyMember, UIBUTTON_CLASSIC, U"-", OnClickRemovePlayerFromState);
    
    State.isInitialized = TRUE;
    
    ls_arenaUse(globalArena);
    
    //NOTE: The state HAS to be loaded after the InitTab 
    //      has ben Initialized to allow data to be properly set.
    b32 result = LoadState(uiContext);
    
    if(State.inBattle) { State.playerSettingsMenuItem->isVisible = FALSE; }
    
    RegionTimer frameTime     = {};
    
    b32 Running               = TRUE;
    utf32 frameTimeString     = ls_utf32Alloc(8);
    b32 showDebug             = FALSE;
    b32 userInputConsumed     = FALSE;
    b32 externalInputReceived = FALSE;
    
    while(Running)
    {
        u32 frameLockMain       = 16;
        u32 frameLockCompendium = 16;
        
        //NOTE: The child frame doesn't need a message pump, because windows
        //      Pumps messages to all windows that were created by this thread.
        //      BUT This means that we must begin the child frame before the main frame
        //      to properly clear input.
        //      TODO: Maybe we want to move the message pump to a separate function
        //      to make the order of function calls more obvious and less error prone!
        ls_uiFrameBeginChild(compendiumContext);
        ls_uiFrameBegin(uiContext);
        
        //NOTE: If any user input was consumed in the previous frame, than we advance the UndoStates.
        //      The first frame is always registered, so the first Undo State is always valid.
        if(userInputConsumed == TRUE && !suppressingUndoRecord)
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
        
        ls_uiSelectFontByFontSize(uiContext, FS_SMALL);
        //NOTE: Render The Window Menu
        userInputConsumed = ls_uiMenu(uiContext, &WindowMenu, -1, 
                                      uiContext->height-20, uiContext->width+1, 21);
        
        //NOTE: Custom Theme Color Picker
        userInputConsumed |= DrawThemePicker(uiContext);
        
        //NOTE: Player Settings
        if(State.arePlayerSettingsOpen)
        {
            userInputConsumed |= DrawPlayerSettings(uiContext);
            
            Input *UserInput = &uiContext->UserInput;
            if(KeyPress(keyMap::Escape)) { State.arePlayerSettingsOpen = FALSE; }
        }
        else if(State.areInfoSettingsOpen)
        {
            userInputConsumed |= DrawInfoSettings(uiContext);
            
            Input *UserInput = &uiContext->UserInput;
            if(KeyPress(keyMap::Escape)) { State.areInfoSettingsOpen = FALSE; }
        }
        else
        {
            //NOTE: The actual Init Tab
            userInputConsumed |= DrawInitTab(uiContext);
        }
        
        if(!uiContext->hasReceivedInput && !uiContext->isDragging && !externalInputReceived)
        {
            externalInputReceived = FALSE;
            
            for(u32 i = 0; i < RENDER_GROUP_COUNT; i++)
            {
                for(s32 zLayer = 0; zLayer < UI_Z_LAYERS; zLayer++)
                {
                    ls_stackClear(&uiContext->renderGroups[i].RenderCommands[zLayer]);
                }
            }
            
            frameLockMain = 32;
        }
        else
        {
            externalInputReceived = FALSE;
            
            //NOTETODO: annoying non-global user input
            Input *UserInput = &uiContext->UserInput;
            
            //TODO: Undo-Redo for StatusConditions
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
                
                //NOTETODO: All globals that affect the state of the program have to be
                //          Set to a valid state, otherwise it will not work.
                globalSelectedIndex = -1;
                suppressingUndoRecord = FALSE;
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
                
                //NOTETODO: All globals that affect the state of the program have to be
                //          Set to a valid state, otherwise it will not work.
                globalSelectedIndex = -1;
                suppressingUndoRecord = FALSE;
            }
            
            //NOTE: If user clicked somewhere, but nothing set the focus, then we should reset the focus
            if(LeftClick && !uiContext->focusWasSetThisFrame)
            { 
                uiContext->currentFocus = 0;
            }
            
            //NOTE: We clear the globalSelectedIndex so that we can exit out of detail mob
            if(KeyPress(keyMap::Escape) && !State.Init->isAdding && (mainCachedPage.talentIndex == -1))
            { globalSelectedIndex = -1; }
            
            //NOTE: If we are showing a talent, we exit out of the talent
            if(KeyPress(keyMap::Escape) && !State.Init->isAdding && (mainCachedPage.talentIndex != -1))
            { mainCachedPage.talentIndex = -1; }
            
            //NOTE: We close the theme selector
            if(KeyPress(keyMap::Escape) && State.themePicker.isShown) { State.themePicker.isShown = FALSE; }
            
            
            //NOTE: Right-Alt Drag, only when nothing is in focus
            if(KeyHeld(keyMap::RAlt) && LeftClick && uiContext->currentFocus == 0)
            { 
                uiContext->isDragging = TRUE;
                POINT currMouse = {};
                GetCursorPos(&currMouse);
                uiContext->prevMousePosX = currMouse.x;
                uiContext->prevMousePosY = currMouse.y;
            }
            
            //NOTE: Handle Dragging
            if(uiContext->isDragging && LeftHold)
            { 
                MouseInput *Mouse = &uiContext->UserInput.Mouse;
                
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
            }
            
            if(uiContext->isDragging && LeftUp) { uiContext->isDragging = FALSE; }
            
            
            if(LeftUp || RightUp || MiddleUp)
            { uiContext->mouseCapture = 0; }
            
            // ----------------
            // Render Everything
            ls_arenaUse(renderArena);
            
            ls_uiRender(uiContext);
            
            ls_arenaUse(globalArena);
            ls_arenaClear(renderArena);
            //
            // ----------------
        }
        
        //NOTETODO: just annoying non global user input bullshit.
        if(uiContext->UserInput.Keyboard.currentState.keyMap::F12 == 1 && 
           uiContext->UserInput.Keyboard.prevState.keyMap::F12 == 0) { showDebug = !showDebug; }
        
        if(showDebug)
        {
            //ls_uiGlyph(uiContext, 0, 3, UIRect {0, 0, (s32)uiContext->width, (s32)uiContext->height}, uiContext->scissor, &uiContext->currFont->glyph['M'], RGBg(0xFF));
            
            ls_uiFillRect(uiContext, 1248, 760, 20, 20, UIRect {0, (s32)uiContext->width, 0, (s32)uiContext->height},
                          uiContext->scissor, uiContext->backgroundColor);
            ls_utf32FromInt_t(&frameTimeString, uiContext->dt);
            ls_uiGlyphString(uiContext, uiContext->currFont, 1248, 760,
                             UIRect {(s32)uiContext->width/2, 0, (s32)uiContext->width, (s32)uiContext->height},
                             uiContext->scissor, frameTimeString, RGBg(0xEE));
            
            ls_uiFillRect(uiContext, windowWidth/2, 0, 2, windowHeight, UIRect {0,0,windowWidth,windowHeight},
                          uiContext->scissor, RGB(0xFF, 0xFF, 0));
            ls_uiFillRect(uiContext, 0, windowHeight/2, windowWidth, 2, UIRect {0,0,windowWidth,windowHeight},
                          uiContext->scissor, RGB(0xFF, 0xFF, 0));
            
            ls_uiFillRect(uiContext, 260, 0, 2, windowHeight, UIRect {0,0,windowWidth,windowHeight},
                          uiContext->scissor, RGB(0xFF, 0, 0xFF));
            ls_uiFillRect(uiContext, 1040, 0, 2, windowHeight, UIRect {0,0,windowWidth,windowHeight},
                          uiContext->scissor, RGB(0xFF, 0, 0xFF));
            ls_uiFillRect(uiContext, 0, 218, windowWidth, 2, UIRect {0,0,windowWidth,windowHeight},
                          uiContext->scissor, RGB(0xFF, 0, 0xFF));
            ls_uiFillRect(uiContext, 0, 696, windowWidth, 2, UIRect {0,0,windowWidth,windowHeight},
                          uiContext->scissor, RGB(0xFF, 0, 0xFF));
        }
        
        //-------------------------------
        //NOTE: Begin Compendium Frame
        
        //TODO: Skip drawing the compendium if the window is closed
        //      But also update the window on first entry. For some reason, putting the entire
        //      Block between the Child Frame inside an if doesn't paint the window on first open.
        b32 compendiumInput = ls_uiMenu(compendiumContext, &CompendiumMenu, -1,
                                        compendiumContext->height-20, compendiumContext->width+1, 21);
        
        compendiumInput |= DrawCompendium(compendiumContext);
        
        if(compendiumInput) { externalInputReceived = TRUE; userInputConsumed |= compendiumInput; }
        
        if(!compendiumContext->hasReceivedInput && !compendiumContext->isDragging)
        {
            for(u32 i = 0; i < RENDER_GROUP_COUNT; i++)
            {
                for(s32 zLayer = 0; zLayer < UI_Z_LAYERS; zLayer++)
                {
                    ls_stackClear(&compendiumContext->renderGroups[i].RenderCommands[zLayer]);
                }
            }
            
            frameLockCompendium = 32;
        }
        else
        {
            //NOTETODO: annoying non-global user input
            Input *UserInput = &compendiumContext->UserInput;
            
            //NOTE: If user clicked somewhere, but nothing set the focus, then we should reset the focus
            if(LeftClick && !compendiumContext->focusWasSetThisFrame) { compendiumContext->currentFocus = 0; }
            
            //NOTE: Exit out of archetype selection.
            if(KeyPress(keyMap::Escape) && (compendium.arch.isChoosingArchetype == TRUE))
            { compendium.arch.isChoosingArchetype = FALSE; }
            
            //NOTE: We clear the globalSelectedIndex so that we can exit out of detail mob
            if(KeyPress(keyMap::Escape) && (cachedPage.talentIndex != -1))
            { cachedPage.talentIndex = -1; }
            
            //NOTE: Right-Alt Drag, only when nothing is in focus
            if(KeyHeld(keyMap::RAlt) && LeftClick && compendiumContext->currentFocus == 0)
            { 
                compendiumContext->isDragging = TRUE;
                POINT currMouse = {};
                GetCursorPos(&currMouse);
                compendiumContext->prevMousePosX = currMouse.x;
                compendiumContext->prevMousePosY = currMouse.y;
            }
            
            if(compendiumContext->isDragging && LeftHold)
            { 
                MouseInput *Mouse = &compendiumContext->UserInput.Mouse;
                
                POINT currMouse = {};
                GetCursorPos(&currMouse);
                
                POINT prevMouse = { compendiumContext->prevMousePosX, compendiumContext->prevMousePosY };
                
                SHORT newX = prevMouse.x - currMouse.x;
                SHORT newY = prevMouse.y - currMouse.y;
                
                SHORT newWinX = compendiumContext->windowPosX - newX;
                SHORT newWinY = compendiumContext->windowPosY - newY;
                
                compendiumContext->windowPosX = newWinX;
                compendiumContext->windowPosY = newWinY;
                
                compendiumContext->prevMousePosX  = currMouse.x;
                compendiumContext->prevMousePosY  = currMouse.y;
                
                SetWindowPos(CompendiumWindow, 0, newWinX, newWinY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            }
            
            if(compendiumContext->isDragging && LeftUp) { compendiumContext->isDragging = FALSE; }
            
            
            if(LeftUp || RightUp || MiddleUp)
            { compendiumContext->mouseCapture = 0; }
            
            // ----------------
            // Render Everything
            ls_arenaUse(renderArena);
            
            ls_uiRender(compendiumContext);
            
            ls_arenaUse(globalArena);
            ls_arenaClear(renderArena);
            //
            // ----------------
            
            if(compendiumContext->UserInput.Keyboard.currentState.keyMap::F12 == 1 && 
               compendiumContext->UserInput.Keyboard.prevState.keyMap::F12 == 0) { showDebug = !showDebug; }
            
            if(showDebug)
            {
                ls_uiFillRect(compendiumContext, 762, 620, 20, 20, UIRect {0, (s32)compendiumContext->width, 0, (s32)compendiumContext->height},
                              compendiumContext->scissor, compendiumContext->backgroundColor);
                ls_utf32FromInt_t(&frameTimeString, compendiumContext->dt);
                ls_uiGlyphString(compendiumContext, compendiumContext->currFont, 762, 620,
                                 UIRect {(s32)compendiumContext->width/2, 0, (s32)compendiumContext->width, (s32)compendiumContext->height},
                                 compendiumContext->scissor, frameTimeString, RGBg(0xEE));
                
                ls_uiFillRect(compendiumContext, compendiumWidth/2, 0, 2, compendiumHeight, UIRect {0,0,compendiumWidth,compendiumHeight},
                              compendiumContext->scissor, RGB(0xFF, 0xFF, 0));
                
                ls_uiFillRect(compendiumContext, 0, compendiumHeight/2, compendiumWidth, 2, UIRect {0,0,compendiumWidth,compendiumHeight},
                              compendiumContext->scissor, RGB(0xFF, 0xFF, 0));
                
                ls_uiFillRect(compendiumContext, compendiumWidth/4, 0, 2, compendiumHeight, UIRect {0,0,compendiumWidth,compendiumHeight},
                              compendiumContext->scissor, RGB(0xFF, 0, 0xFF));
                ls_uiFillRect(compendiumContext, 3*compendiumWidth/4, 0, 2, compendiumHeight, UIRect {0,0,compendiumWidth,compendiumHeight},
                              compendiumContext->scissor, RGB(0xFF, 0, 0xFF));
                ls_uiFillRect(compendiumContext, 0, compendiumHeight/3, compendiumWidth, 2, UIRect {0,0,compendiumWidth,compendiumHeight},
                              compendiumContext->scissor, RGB(0xFF, 0, 0xFF));
                ls_uiFillRect(compendiumContext, 0, 2*compendiumHeight/3, compendiumWidth, 2, UIRect {0,0,compendiumWidth,compendiumHeight},
                              compendiumContext->scissor, RGB(0xFF, 0, 0xFF));
            }
            
        }
        
        //NOTE: End Compendium Frame
        //-------------------------------
        
        ls_arenaClear(frameArena);
        
        GetSystemTime(&endT);
        
        State.timePassed += (endT.wSecond - beginT.wSecond);
        if(State.timePassed >= 30)
        {
            State.timePassed = 0;
            SaveState(NULL);
        }
        
        beginT = endT;
        
        const s32 frameLock = ls_min((s32)frameLockCompendium, (s32)frameLockMain);
        ls_uiFrameEndChild(compendiumContext, frameLock);
        ls_uiFrameEnd(uiContext, frameLock);
    }
    
    return 0;
}
