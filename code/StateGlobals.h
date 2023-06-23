/* date = May 22nd 2020 11:45 am */

#ifndef _STATE_GLOBALS_H
#define _STATE_GLOBALS_H

const u32 global_saveVersion = 8;

//NOTE This is here rather than in themePicker.cpp because fuck c++ and ordered compilation.
struct ThemePicker
{
    UIColorPicker wheel;
    b32           isShown;
    
    s8 selected = -1;
};

struct ProgramState
{
    InitPage  *Init;
    
    //State Management
    b32       inBattle;
    EncList   encounters;
    
    //Custom Theme
    ThemePicker themePicker;
    Color       backgroundColor;
    Color       borderColor;
    Color       menuBarColor;
    Color       highliteColor;
    Color       pressedColor;
    Color       widgetColor;
    Color       textColor;
    Color       invWidgetColor;
    Color       invTextColor;
    
    
    b32 isInitialized;
    
    u64 timePassed;
    u64 dt = 0;
};

HINSTANCE MainInstance = 0;
HWND MainWindow        = 0;
HWND CompendiumWindow  = 0;

u8 *BackBuffer;
u8 *CompendiumBackBuffer;
PlayerChar pc = {};
ProgramState State = {};

const u32 MAX_UNDO_STATES  = 32;
ProgramState UndoStates[MAX_UNDO_STATES] = {};
static u32 matchingUndoIdx = 0;
static u32 distanceFromOld = 0;
static u32 distanceFromNow = 0;

static b32 undoRequest = FALSE;
static b32 redoRequest = FALSE;

static b32 suppressingUndoRecord = FALSE;

#endif //_STATE_GLOBALS_H
