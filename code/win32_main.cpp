#define LS_EXTERNAL_CONSOLE
#define LS_WINDOWS_IMPLEMENTATION
#include "lsWindows.h"
#undef LS_WINDOWS_IMPLEMENTATION

#define LS_OPENGL_IMPLEMENTATION
#include "OpenGL/lsOpenGL.h"
#undef LS_OPENGL_IMPLEMENTATION

#define LS_ARENA_IMPLEMENTATION
#include "lsArena.h"
#undef LS_ARENA_IMPLEMENTATION

#define LS_CRT_IMPLEMENTATION
#include "lsCRT.h"
#undef LS_CRT_IMPLEMENTATION

#define LS_STRING_IMPLEMENTATION
#include "lsString.h"
#undef LS_STRING_IMPLEMENTATION

#include "lsArray.h"

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

//#define LS_UI_OPENGL_BACKEND
#define LS_UI_SOFTWARE_BACKEND
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

#include "CompendiumSearch.h"
#include "CompendiumGS.h"
#include "CompendiumSkills.h"
#include "CompendiumTalents.h"
#include "CompendiumAlignment.h"
#include "CompendiumArchetypes.h"
#include "CompendiumHP.h"
#include "CompendiumResistance.h"
#include "CompendiumImmunities.h"

#include "themePicker.cpp"
#include "diceRoller.cpp"

#include "Compendium.cpp"
#include "CompendiumSearch.cpp"
#include "CompendiumGS.cpp"
#include "CompendiumSkills.cpp"
#include "CompendiumTalents.cpp"
#include "CompendiumArchetypes.cpp"
#include "CompendiumAlignment.cpp"
#include "CompendiumHP.cpp"
#include "CompendiumResistance.cpp"
#include "CompendiumImmunities.cpp"

#include "Class.cpp"
#include "Feats.cpp"
#include "PlayerChar.cpp"
#include "Init.cpp"
#include "PCTab.cpp"
#include "FeatsTab.cpp"
#include "SaveState.cpp"

//#include "AssetLoader.cpp"

#if _DEBUG
#include "test.cpp"
#endif

b32 ProgramExitOnButton(UIContext *c, void *data) { SendMessageA(MainWin.Window, WM_DESTROY, 0, 0); return FALSE; }

//TODO: Compendium does not answer keyboard input when MainWindow is minimized. Why???
//      Do I have a fucked up keyboard state???
//NOTE: We have to send an LButtonUp, else our Input handling will be confused
//      and not register the un-pressing of the left button.
b32 ProgramMinimizeOnButton(UIContext *c, void *data) { SendMessageA(MainWin.Window, WM_LBUTTONUP, 0, 0); ShowWindow(MainWin.Window, SW_MINIMIZE); return FALSE; }

//TODO: If we Maximize and Restore without other operations in between, the menu bar will then require an extra click to start dragging again!?
b32 ProgramToggleMaxOnButton(UIContext *c, void *data) { static bool isMax = false; ShowWindow(MainWin.Window, isMax ? SW_NORMAL : SW_MAXIMIZE); isMax = !isMax; return FALSE; }

b32 CompendiumExitOnButton(UIContext *c, void *data) { ShowWindow(CompendiumWin.Window, SW_HIDE); return FALSE; }
b32 CompendiumMinimizeOnButton(UIContext *c, void *data) { SendMessageA(CompendiumWin.Window, WM_LBUTTONUP, 0, 0); ShowWindow(CompendiumWin.Window, SW_MINIMIZE); return FALSE; }
b32 CompendiumToggleMaxOnButton(UIContext *c, void *data) { static bool isMax = false; ShowWindow(CompendiumWin.Window, isMax ? SW_NORMAL : SW_MAXIMIZE); isMax = !isMax; return FALSE; }
b32 ProgramOpenCompendium(UIContext *c, void *data)
{
    if(CompendiumWin.Window)
    {
        ShowWindow(CompendiumWin.Window, SW_SHOW);
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

void CopyState(UIContext *c, ProgramState *FromState, ProgramState *ToState)
{
    //NOTE: Copy Init Page
    InitPage *curr = FromState->Init;
    InitPage *dest = ToState->Init;
    
    dest->Mobs.selectedIndex   = curr->Mobs.selectedIndex;
    dest->Allies.selectedIndex = curr->Allies.selectedIndex;
    
    for(u32 i = 0; i < party_count; i++)
    { ls_uiTextBoxSet(c, dest->PlayerInit + i, curr->PlayerInit[i].text); }
    
    for(u32 i = 0; i < ally_count; i++)
    {
        InitField *From = curr->AllyFields + i;
        InitField *To   = dest->AllyFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_uiTextBoxSet(c, To->editFields + j, From->editFields[j].text); }
        
        ls_uiTextBoxSet(c, &To->maxLifeDisplay, From->maxLifeDisplay.text);
        ls_uiTextBoxSet(c, &To->nonLethalDisplay, From->nonLethalDisplay.text);
        To->maxLife       = From->maxLife;
        To->nonLethal     = From->nonLethal;
        
        To->compendiumIdx = From->compendiumIdx;
        To->ID            = From->ID;
    }
    
    for(u32 i = 0; i < mob_count; i++)
    {
        InitField *From = curr->MobFields + i;
        InitField *To   = dest->MobFields + i;
        
        for(u32 j = 0; j < IF_IDX_COUNT; j++)
        { ls_uiTextBoxSet(c, To->editFields + j, From->editFields[j].text); }
        
        ls_uiTextBoxSet(c, &To->maxLifeDisplay, From->maxLifeDisplay.text);
        ls_uiTextBoxSet(c, &To->nonLethalDisplay, From->nonLethalDisplay.text);
        To->maxLife       = From->maxLife;
        To->nonLethal     = From->nonLethal;
        
        To->compendiumIdx = From->compendiumIdx;
        To->ID            = From->ID;
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
        
        //Status Conditions, we store the check isActive status
        for(s32 statusIdx = 0; statusIdx < STATUS_COUNT; statusIdx++)
        { To->status[statusIdx].check.isActive = From->status[statusIdx].check.isActive; }
        
        To->field.lColor    = From->field.lColor;
        To->field.rColor    = From->field.rColor;
        
        To->compendiumIdx   = From->compendiumIdx;
        To->ID              = From->ID;
    }
    
    dest->turnsInRound = curr->turnsInRound;
    dest->orderAdjust  = curr->orderAdjust;
    
    ls_uiTextBoxSet(c, &dest->RoundCounter, curr->RoundCounter.text);
    dest->roundCount = curr->roundCount;
    
    ls_uiTextBoxSet(c, &dest->Current, curr->Current.text);
    dest->currIdx = curr->currIdx;
    
    for(u32 i = 0; i < COUNTER_NUM; i++)
    {
        Counter *From = curr->Counters + i;
        Counter *To   = dest->Counters + i;
        
        ls_uiTextBoxSet(c, &To->name, From->name.text);
        ls_uiTextBoxSet(c, &To->rounds, From->rounds.text);
        
        To->roundsLeft      = From->roundsLeft;
        To->startIdxInOrder = From->startIdxInOrder;
        To->turnCounter     = From->turnCounter;
        To->isActive        = From->isActive;
    }
    
    for(u32 i = 0; i < THROWER_NUM; i++)
    {
        DiceThrowBox *From = curr->Throwers + i;
        DiceThrowBox *To   = dest->Throwers + i;
        
        ls_uiTextBoxSet(c, &To->name,   From->name.text);
        ls_uiTextBoxSet(c, &To->toHit,  From->toHit.text);
        ls_uiTextBoxSet(c, &To->hitRes, From->hitRes.text);
        ls_uiTextBoxSet(c, &To->damage, From->damage.text);
        ls_uiTextBoxSet(c, &To->dmgRes, From->dmgRes.text);
    }
    
    ls_uiTextBoxSet(c, &dest->GeneralThrower.name,   curr->GeneralThrower.name.text);
    ls_uiTextBoxSet(c, &dest->GeneralThrower.toHit,  curr->GeneralThrower.toHit.text);
    ls_uiTextBoxSet(c, &dest->GeneralThrower.hitRes, curr->GeneralThrower.hitRes.text);
    ls_uiTextBoxSet(c, &dest->GeneralThrower.damage, curr->GeneralThrower.damage.text);
    ls_uiTextBoxSet(c, &dest->GeneralThrower.dmgRes, curr->GeneralThrower.dmgRes.text);
    
    //NOTE: Since the state of the encounters is separate from the state of the InitPage
    //      We don't want the Undo System to keep a full copy of the Encounters and this needs to be
    //      reflected in the EncounterSel. Thus, we try to go back to the previously selected index,
    //      but if a series of operations (like additions and removals of encounters) made the selectedIndex
    //      invalid, we just revert to the *always* valid 0 index (No Encounter)
    dest->EncounterSel.selectedIndex = curr->EncounterSel.selectedIndex;
    if(dest->EncounterSel.selectedIndex >= dest->EncounterSel.list.count)
    { dest->EncounterSel.selectedIndex = 0; }
    
    //NOTE: Copy General Info
    ToState->inBattle = FromState->inBattle;
    dest->isAdding = curr->isAdding;
    
    //NOTE: Restore state of Add New Buttons
    ls_utf32Set(&dest->addNewMob.name, curr->addNewMob.name);
    dest->addNewMob.callback1 = curr->addNewMob.callback1;
    
    ls_utf32Set(&dest->addNewAlly.name, curr->addNewAlly.name);
    dest->addNewAlly.callback1 = curr->addNewAlly.callback1;
    
    //NOTE: Misc Menu info
    ToState->playerSettingsMenuItem->isVisible = !ToState->inBattle;
}

UIMenu SetupMainWindowMenu(UIContext *c)
{
    UIMenu WindowMenu      = {};
    WindowMenu.closeWindow = ls_uiMenuButton(c, ProgramExitOnButton, closeBtnDataPremulti, closeBtnWidth, closeBtnHeight);
    WindowMenu.minimize    = ls_uiMenuButton(c, ProgramMinimizeOnButton, minBtnDataPremulti, minBtnWidth, minBtnHeight);
    WindowMenu.maximize    = ls_uiMenuButton(c, ProgramToggleMaxOnButton, maxBtnDataPremulti, maxBtnWidth, maxBtnHeight);
    WindowMenu.itemWidth   = 100;
    
    ls_uiMenuAddSub(c, &WindowMenu, U"Style");
    ls_uiSubMenuAddItem(c, &WindowMenu, 0, U"Default", selectStyleDefault, NULL);
    ls_uiSubMenuAddItem(c, &WindowMenu, 0, U"Prana", selectStylePrana, NULL);
    
    ls_uiMenuAddSub(c, &WindowMenu, U"Theme");
    ls_uiSubMenuAddItem(c, &WindowMenu, 1, U"Default", selectThemeDefault, NULL);
    ls_uiSubMenuAddItem(c, &WindowMenu, 1, U"Dark Night", selectThemeDarkNight, NULL);
    ls_uiSubMenuAddItem(c, &WindowMenu, 1, U"Light", selectThemeLight, NULL);
    ls_uiSubMenuAddItem(c, &WindowMenu, 1, U"Green", selectThemeGreen, NULL);
    ls_uiSubMenuAddItem(c, &WindowMenu, 1, U"User", selectThemeUser, NULL);
    ls_uiSubMenuAddItem(c, &WindowMenu, 1, U"Customize", openThemeColorPicker, NULL);
    
    ls_uiMenuAddSub(c, &WindowMenu, U"Settings");
    State.playerSettingsMenuItem = ls_uiSubMenuAddItem(c, &WindowMenu, 2, 
                                                       U"Players", ProgramOpenPlayersSettings, NULL);
    ls_uiSubMenuAddItem(c, &WindowMenu, 2, U"Info", ProgramOpenInfoSettings, NULL);
    
    ls_uiMenuAddItem(c, &WindowMenu, U"Compendium", ProgramOpenCompendium, NULL);
    return WindowMenu;
}

UIMenu SetupCompendiumWindowMenu(UIContext *c)
{
    UIMenu CompendiumMenu = {};
    CompendiumMenu.closeWindow  = ls_uiMenuButton(c, CompendiumExitOnButton, closeBtnDataPremulti, closeBtnWidth, closeBtnHeight);
    CompendiumMenu.minimize     = ls_uiMenuButton(c, CompendiumMinimizeOnButton, minBtnDataPremulti, minBtnWidth, minBtnHeight);
    CompendiumMenu.maximize     = ls_uiMenuButton(c, CompendiumToggleMaxOnButton, maxBtnDataPremulti, maxBtnWidth, maxBtnHeight);
    CompendiumMenu.itemWidth    = 120;
    
    ls_uiMenuAddItem(c, &CompendiumMenu, U"Monster Table", CompendiumOpenMonsterTable, NULL);
    ls_uiMenuAddItem(c, &CompendiumMenu, U"NPC Table", CompendiumOpenNPCTable, NULL);
    return CompendiumMenu;
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
    
    globalArena     = ls_arenaCreate(MBytes(12), (char *)"global");
    fileArena       = ls_arenaCreate(MBytes(4), (char *)"file");
    stateArena      = ls_arenaCreate(MBytes(12), (char *)"state");
    saveArena       = ls_arenaCreate(MBytes(4), (char *)"save");
    renderArena     = ls_arenaCreate(KBytes(8), (char *)"render");
    
    frameArena      = ls_arenaCreate(KBytes(8), (char *)"frame");
    
    //TODO: Make this much smaller. It can be reduced to at least 6 MBytes, probably smaller
    compendiumArena = ls_arenaCreate(MBytes(16), (char *)"compendium");
    compTempArena   = ls_arenaCreate(KBytes(16), (char *)"compendium temporary");
    
    ls_arenaUse(globalArena);
    //------------
    
    ls_logDefaultTypesRegister();
    
#if _DEBUG
    ls_vlogRegister("Order", ls_vlogFormatOrder);
    ls_vlogRegister("UIRect", ls_vlogFormatUIRect);
#endif
    
    //TODO: Here we are passing the globalArena and the frameArena into the context.
    const int windowWidth = 1280;
    const int windowHeight = 860;
    UIContext *uiContext = ls_uiInitDefaultContext(globalArena, frameArena, stateArena);
    MainWin = ls_uiCreateWindow(uiContext, BackBuffer, windowWidth, windowHeight, "PCMan", true);
    ls_uiAddOnDestroyCallback(uiContext, SaveState);
    ls_uiLoadPackedFontAtlas(uiContext, (char *)"PackedFontAtlas.bmp");

    //TODO: Probably want to make something a little bit more sophisticated... or we just pass the actual
    // UIWindow to the init functions? @CopyPasted
    uiContext->currWindow = &MainWin;

    UIMenu WindowMenu = SetupMainWindowMenu(uiContext);
    
    //TODO: Here we are passing the globalArena and the frameArena into the context.
    const int compendiumWidth  = 800;
    const int compendiumHeight = 720;
    CompendiumWin = ls_uiCreateWindow(uiContext, CompendiumBackBuffer, compendiumWidth, compendiumHeight, "Compendium", false);
    
    //TODO: Probably want to make something a little bit more sophisticated... or we just pass the actual
    // UIWindow to the init functions? @CopyPasted
    uiContext->currWindow = &CompendiumWin;
    
    LoadCompendium(uiContext, ls_strConstant("Compendium"));

    UIMenu CompendiumMenu = SetupCompendiumWindowMenu(uiContext);
    
    //TODO: Abstract away this Windows-Specific thing...
    SYSTEMTIME endT, beginT;
    GetSystemTime(&beginT);
    
    ls_arenaUse(stateArena);
    
    //TODO: Probably want to make something a little bit more sophisticated... or we just pass the actual
    // UIWindow to the init functions? @CopyPasted
    uiContext->currWindow = &MainWin;
    
    //NOTE: Initialize State and Undo States
    State.themePicker.wheel                = ls_uiColorPickerInit(uiContext, &State.themePicker);
    State.themePicker.wheel.pickedColor    = uiContext->backgroundColor;
    
    State.backgroundColor = RGBg(0x38);
    State.borderColor     = RGBg(0x22);
    State.menuBarColor    = RGBg(0x20);
    State.highliteColor   = RGBg(0x65);
    State.pressedColor    = RGBg(0x75);
    State.widgetColor     = RGBg(0x45);
    State.textColor       = RGBg(0xCC);
    State.invWidgetColor  = RGBg(0xBA);
    State.invTextColor    = RGBg(0x33);
    
    State.Init = (InitPage *)ls_alloc(sizeof(InitPage));
    SetInitTab(uiContext, &State);
    
    //NOTE: Initialize the mainCachedPage to display the mob page inside init.
    initCachedPage(&mainCachedPage);
    InitCachedTalentEntry(&mainCachedTalent);
    mainCachedPage.talentPage = &mainCachedTalent;
    
    SetMonsterTable(uiContext);
    SetNPCTable(uiContext);
    
    //NOTE: Single block allocation for all Init Pages.
    InitPage *UndoInitPages = (InitPage *)ls_alloc(sizeof(InitPage)*MAX_UNDO_STATES);
    
    //TODO: I had to nerf frame-by-frame initialization because LoadState needs to load the entire
    //      undo chain, even if it was unused. And all at once. Want to try and fix it with multithreading.
    for(u32 i = 0; i < MAX_UNDO_STATES; i++)
    {
        UndoStates[i].Init = UndoInitPages + i;
        UndoStates[i].playerSettingsMenuItem = State.playerSettingsMenuItem;
        SetInitTab(uiContext, UndoStates + i);
    }
    
    //NOTE: Set the Party Settings TextBoxes and Buttons
    for(s32 i = 0; i < MAX_PARTY_NUM; i++)
    { ls_uiTextBoxInit(uiContext, State.PartyName + i, 16); }
    ls_uiTextBoxSet(uiContext, &State.PartyName[0], ls_utf32Constant(U"Adventurer 1"));
    ls_uiTextBoxSet(uiContext, &State.PartyName[1], ls_utf32Constant(U"Adventurer 2"));
    ls_uiTextBoxSet(uiContext, &State.PartyName[2], ls_utf32Constant(U"Adventurer 3"));
    for(s32 i = party_count; i < MAX_PARTY_NUM; i++)
    { ls_uiTextBoxSet(uiContext, State.PartyName + i, ls_utf32Constant(U"XXXXX")); }
    
    State.addPartyMember    = ls_uiButtonInit(uiContext, UIBUTTON_CLASSIC, U"+", OnClickAddPlayerToState);
    State.removePartyMember = ls_uiButtonInit(uiContext, UIBUTTON_CLASSIC, U"-", OnClickRemovePlayerFromState);
    
    State.isInitialized = TRUE;
    
    ls_arenaUse(globalArena);
    
    //NOTE: The state HAS to be loaded after the InitTab 
    //      has ben Initialized to allow data to be properly set.
    //TODO: The only reason the memory management currently works with lsUI is because LoadState uses
    // the globalArena, which is shared with the lsUI widgetArena.
    // The way LoadState allocates memory for textboxes is actually wrong, because it's not asking the
    // uicontext (which holds the arena for the widget's storage) for the memory, it's poking into the
    // widget's internal containers and allocating there directly. It's a hack!
    if(LoadState(uiContext) == FALSE)
    {
        //NOTE: No Save file has been found.
        //      As of right now this is irrelevant, as the program can always start in a valid state.
    }
    
    
    if(State.inBattle) { State.playerSettingsMenuItem->isVisible = FALSE; }
    
    RegionTimer frameTime     = {};
    
    b32 Running               = TRUE;
    utf32 frameTimeString     = ls_utf32Alloc(8);
    b32 showDebug             = FALSE;
    b32 userInputConsumed     = FALSE;
    b32 externalInputReceived = FALSE;
    
    while(Running)
    {
        //NOTE: Start the Main Thread frame
        u32 frameLockMain       = 16;
        ls_uiStartFrameTimer(uiContext);
        
        ls_uiFrameBegin(uiContext, &MainWin);
        //NOTETODO: annoying non-global user input
        Input *UserInput = &MainWin.UserInput;

        //NOTE: If any user input was consumed in the previous frame, than we advance the UndoStates.
        //      The first frame is always registered, so the first Undo State is always valid.
        //TODO: Maybe I want to put this at the end of the frame, rather than the beggining?
        //      Just for better organization...
        if(userInputConsumed == TRUE && !suppressingUndoRecord)
        {
            matchingUndoIdx = (matchingUndoIdx + 1) % MAX_UNDO_STATES;
            CopyState(uiContext, &State, UndoStates + matchingUndoIdx);
            
            if(distanceFromOld < (MAX_UNDO_STATES-1)) { distanceFromOld += 1; }
            
            //NOTE: If an operation is performed, that is the new NOW, the new Present, 
            //      and no other REDOs can be performed
            distanceFromNow = 0;
        }
        
        
        ls_uiSelectFontByPixelHeight(uiContext, 16);

        userInputConsumed = ls_uiMenu(uiContext, &WindowMenu, -1, MainWin.height-20, MainWin.width+1, 21);
        userInputConsumed |= DrawThemePicker(uiContext);
        
        //NOTE: Player Settings
        if(State.arePlayerSettingsOpen)
        {
            userInputConsumed |= DrawPlayerSettings(uiContext);
            if(KeyPress(keyMap::Escape)) { State.arePlayerSettingsOpen = FALSE; }
        }
        else if(State.areInfoSettingsOpen)
        {
            userInputConsumed |= DrawInfoSettings(uiContext);
            if(KeyPress(keyMap::Escape)) { State.areInfoSettingsOpen = FALSE; }
        }
        else
        {
            //NOTE: The actual Init Tab
            userInputConsumed |= DrawInitTab(uiContext);
        }
        
        //NOTE: externalInputReceived is used to update the main window when other windows (like the Compendium)
        //      require it.
        //
        //      isAddingFailedSet is used by the Init Page to update a fader animation, which needs to keep going.
        if(!MainWin.hasReceivedInput && !MainWin.isDragging && !externalInputReceived && !isAddingFailedSet)
        {
            externalInputReceived = FALSE;
            
            for(u32 i = 0; i < LS_UI_RENDER_GROUP_COUNT; i++)
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

            //TODO: If a page was open in initiative, keep it open after the undo/redo.
            //      Although that only makes sense in specific cases.
            //
            //      Either we make the globalSelectedIndex a part of the State.
            //         Which means possibly changing the opened page for every undo/redo.
            //
            //      Or it's still a global. Which means we only keep it around if the gobalSelectedIndex
            //      was the same in the previous/next state. Which would mean recording it anyway.
            //      But only showing it if it matches? Not sure how useful it is...
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
                globalSelectedIndex      = -1;
                ls_staticArrayClear(&globalSelectedArchetypes);
                mainCachedPage.pageIndex = -1; //Force a re-cache of the page
                suppressingUndoRecord    = FALSE;
                isAddingFailedSetTimer   = 0;
                isAddingFailedSet        = FALSE;
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
                globalSelectedIndex      = -1;
                ls_staticArrayClear(&globalSelectedArchetypes);
                mainCachedPage.pageIndex = -1; //Force a recache of the page
                suppressingUndoRecord    = FALSE;
                isAddingFailedSetTimer   = 0;
                isAddingFailedSet        = FALSE;
            }
            
            //NOTE: We clear the globalSelectedIndex so that we can exit out of detail mob
            if(KeyPress(keyMap::Escape) && !State.Init->isAdding && (mainCachedPage.talentIndex == -1))
            { globalSelectedIndex = -1; ls_staticArrayClear(&globalSelectedArchetypes); }
            
            //NOTE: If we are showing a talent, we exit out of the talent
            if(KeyPress(keyMap::Escape) && !State.Init->isAdding && (mainCachedPage.talentIndex != -1))
            { mainCachedPage.talentIndex = -1; }
            
            //NOTE: We close the theme selector
            if(KeyPress(keyMap::Escape) && State.themePicker.isShown) { State.themePicker.isShown = FALSE; }

            //NOTE: We can toggle drawn debug info (render threads regions (which can also be used as alignment rulers)
            // and display frame time
            if(KeyPress(keyMap::K)) { showDebug = !showDebug; }
            if(showDebug) { ls_uiDebugDrawInfo(uiContext); }
    
            // ----------------
            // Render Everything
            ls_arenaUse(renderArena);
            
            ls_uiRender(uiContext);
            
            ls_arenaUse(globalArena);
            ls_arenaClear(renderArena);
            //
            // ----------------
        }

        //NOTE: End MainWindow Frame
        ls_uiFrameEnd(uiContext);
        
        //-------------------------------
        //NOTE: Begin Compendium Frame
        ls_uiFrameBegin(uiContext, &CompendiumWin);
        //NOTETODO: annoying non-global user input
        UserInput = &CompendiumWin.UserInput;

        
        //TODO: Skip drawing the compendium if the window is closed
        //      But also update the window on first entry. For some reason, putting the entire
        //      Block between the Child Frame inside an if doesn't paint the window on first open.
        b32 compendiumInput = ls_uiMenu(uiContext, &CompendiumMenu, -1, CompendiumWin.height-20, CompendiumWin.width+1, 21);
        
        compendiumInput |= DrawCompendium(uiContext);
        
        if(compendiumInput) { externalInputReceived = TRUE; userInputConsumed |= compendiumInput; }
        
        if(!CompendiumWin.hasReceivedInput && !CompendiumWin.isDragging)
        {
            for(u32 i = 0; i < LS_UI_RENDER_GROUP_COUNT; i++)
            {
                for(s32 zLayer = 0; zLayer < UI_Z_LAYERS; zLayer++)
                {
                    ls_stackClear(&uiContext->renderGroups[i].RenderCommands[zLayer]);
                }
            }
        }
        else
        {
            //NOTE: If user clicked somewhere, but nothing set the focus, then we should reset the focus
            if(LeftClick && !CompendiumWin.focusWasSetThisFrame) { CompendiumWin.currentFocus = 0; }
            
            //NOTE: Exit out of archetype selection.
            if(KeyPress(keyMap::Escape) && (compendium.arch.isChoosingArchetype == TRUE))
            { compendium.arch.isChoosingArchetype = FALSE; }
            
            //NOTE: We clear the talentIndex so that we can exit out of a talent Page
            if(KeyPress(keyMap::Escape) && (cachedPage.talentIndex != -1))
            { cachedPage.talentIndex = -1; }

            //NOTE: We can toggle drawn debug info (render threads regions (which can also be used as alignment rulers)
            // and display frame time
            if(KeyPress(keyMap::F12)) { showDebug = !showDebug; }
            if(showDebug) { ls_uiDebugDrawInfo(uiContext); }
            
            // ----------------
            // Render Everything
            ls_arenaUse(renderArena);
            
            ls_uiRender(uiContext);
            
            ls_arenaUse(globalArena);
            ls_arenaClear(renderArena);
            //
            // ----------------
        }
        
        //NOTE: End Compendium Frame
        //-------------------------------
        ls_uiFrameEnd(uiContext);
        
        GetSystemTime(&endT);
        State.timePassed += (endT.wSecond - beginT.wSecond);
        if(State.timePassed >= 30)
        {
            State.timePassed = 0;
            SaveState(NULL);
        }
        beginT = endT;
        
        //NOTE: End the Main Thread frame
        ls_uiEndFrameTimer(uiContext, frameLockMain);
    }
    
    return 0;
}
