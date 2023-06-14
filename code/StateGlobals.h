/* date = May 22nd 2020 11:45 am */

#ifndef _STATE_GLOBALS_H
#define _STATE_GLOBALS_H

const u32 global_saveVersion = 7;

enum ProgramTheme
{
    THEME_DEFAULT,
    THEME_DARKNIGHT,
    THEME_LIGHT,
    
    THEME_COUNT
};

static ProgramTheme currentTheme = THEME_DEFAULT;

struct ProgramState
{
    InitPage  *Init;
    
    //State Management
    b32       inBattle;
    EncList   encounters;
    
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

b32 selectThemeDefault(UIContext *c, void *data)
{
    currentTheme = THEME_DEFAULT;
    
    c->backgroundColor = RGBg(0x38);
    c->highliteColor   = RGBg(0x65);
    c->pressedColor    = RGBg(0x75);
    c->widgetColor     = RGBg(0x45);
    c->borderColor     = RGBg(0x22);
    c->textColor       = RGBg(0xCC);
    c->invWidgetColor  = RGBg(0xBA);
    c->invTextColor    = RGBg(0x33);
    
    return FALSE;
}

b32 selectThemeDarkNight(UIContext *c, void *data)
{
    currentTheme = THEME_DARKNIGHT;
    
    c->backgroundColor = RGB(0x0C, 0x08, 0x0B);
    c->highliteColor   = RGB(0x14, 0x15, 0x13);
    c->pressedColor    = RGB(0x18, 0x20, 0x19);
    c->widgetColor     = RGB(0x1C, 0x18, 0x1B);
    c->borderColor     = RGB(0x60, 0x7B, 0x7D);
    c->textColor       = RGBg(0xCC);
    c->invWidgetColor  = RGB(0xE3, 0xE7, 0xE4);
    c->invTextColor    = RGB(0x7D, 0x71, 0x7D);
    
    return FALSE;
}

b32 selectThemeLight(UIContext *c, void *data)
{
    currentTheme = THEME_LIGHT;
    
    c->backgroundColor = RGBg(0x99);
    c->highliteColor   = RGB(0xAA, 0x99, 0xAA);
    c->pressedColor    = RGB(0xBB, 0x99, 0xAA);
    c->widgetColor     = RGBg(0x88);
    c->borderColor     = RGBg(0x00);
    c->textColor       = RGBg(0x33);
    c->invWidgetColor  = RGB(7, 5, 43);
    c->invTextColor    = RGBg(0xFF);
    
    return FALSE;
}

typedef  b32(*SelectThemeProc)(UIContext *, void *);
SelectThemeProc selectThemeProcs[THEME_COUNT] = { selectThemeDefault, selectThemeDarkNight, selectThemeLight };

#endif //_STATE_GLOBALS_H
