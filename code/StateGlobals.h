/* date = May 22nd 2020 11:45 am */

#ifndef _STATE_GLOBALS_H
#define _STATE_GLOBALS_H

const u32 global_saveVersion = 9;

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
    
    //Program Settings
    UITextBox PartyName[MAX_PARTY_NUM];
    UIButton  addPartyMember;
    UIButton  removePartyMember;
    
    UIMenuItem *playerSettingsMenuItem;
    b32 arePlayerSettingsOpen;
    b32 areInfoSettingsOpen;
    
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

b32 OnClickAddPlayerToState(UIContext *c, void *data)
{
    party_count += 1;
    order_count += 1;
    
    AssertMsg(party_count <= MAX_PARTY_NUM, "Party Count exceeded Max\n");
    AssertMsg(order_count <= MAX_ORDER_NUM, "Order Num exceeded Max\n");
    
    return FALSE;
}

b32 OnClickRemovePlayerFromState(UIContext *c, void *data)
{
    ls_uiTextBoxSet(c, &State.PartyName[party_count - 1], U"XXXXX");
    
    party_count -= 1;
    order_count -= 1;
    
    AssertMsg(party_count > 0, "Party Count exceeded Max\n");
    AssertMsg(order_count > 0, "Order Num exceeded Max\n");
    
    return FALSE;
}

b32 DrawPlayerSettings(UIContext *c)
{
    b32 usedInput = FALSE;
    
    s32 x = 0.37f*c->width;
    s32 y = 0.25f*c->height;
    
    s32 w = 0.26f*c->width;
    s32 h = 0.5f*c->height;
    
    ls_uiRect(c, x, y, w, h, ls_uiLightenRGB(c->widgetColor, 0.25f), c->borderColor);
    
    x = 0.39f*c->width;
    y = 0.70f*c->height;
    
    ls_uiSelectFontByPixelHeight(c, 24);
    
    s32 boxHeight = c->currFont->pixelHeight*1.1f;
    
    for(s32 i = 0; i < party_count; i++)
    {
        ls_uiTextBox(c, &State.PartyName[i], x, y, 0.18f*c->width, boxHeight, 1);
        y -= boxHeight*1.2f;
    }
    
    if(party_count < MAX_PARTY_NUM)
    {
        s32 addX = x + 0.09f*c->width;
        usedInput |= ls_uiButton(c, &State.addPartyMember, addX, y, 1);
    }
    
    if(party_count > 0)
    {
        s32 remX = x + 0.20f*c->width;
        s32 remY = y + boxHeight*1.3f;
        usedInput |= ls_uiButton(c, &State.removePartyMember, remX, remY, 1);
    }
    
    ls_uiSelectFontByPixelHeight(c, 18);
    
    return usedInput;
}

b32 DrawInfoSettings(UIContext *c)
{
    s32 x = 0.15f*c->width;
    s32 y = 0.20f*c->height;
    
    s32 w = 0.70f*c->width;
    s32 h = 0.60f*c->height;
    
    ls_uiRect(c, x, y, w, h, ls_uiLightenRGB(c->widgetColor, 0.25f), c->borderColor);
    
    ls_uiSelectFontByPixelHeight(c, 24);
    
    x = 0.16f*c->width;
    y = 0.77f*c->height;
    s32 boxHeight = c->currFont->pixelHeight*1.1f;
    
    ls_uiLabel(c, U"Created by Lorenzo Stramondo 2018 \u00A9", x, y, 1); 
    y -= 2*boxHeight;
    
    ls_uiLabel(c, U"Shortcuts:", x, y, 1);
    y -= boxHeight;
    
    ls_uiSelectFontByPixelHeight(c, 18);
    
    boxHeight = c->currFont->pixelHeight*1.1f;
    
    s32 shortcutStartY = y;
    
    ls_uiLabel(c, U"Escape", x, y, 1); y -= boxHeight;
    ls_uiLabel(c, U"   Exits out of many interfaces", x, y, 1); y -= 2*boxHeight;
    
    ls_uiLabel(c, U"Ctrl + Z", x, y, 1); y -= boxHeight;
    ls_uiLabel(c, U"   Undo", x, y, 1);  y -= 2*boxHeight;
    
    ls_uiLabel(c, U"Ctrl + Y", x, y, 1); y -= boxHeight;
    ls_uiLabel(c, U"   Redo", x, y, 1);  y -= 2*boxHeight;
    
    ls_uiLabel(c, U"Right Alt + Left Mouse Drag", x, y, 1); y -= boxHeight;
    ls_uiLabel(c, U"   Drag Window", x, y, 1);              y -= 2*boxHeight;
    
    ls_uiLabel(c, U"Enter in Textboxes", x, y, 1);                                 y -= boxHeight;
    ls_uiLabel(c, U"   Either lose focus or switch to next logical box", x, y, 1); y -= 2*boxHeight;
    
    ls_uiLabel(c, U"Down Arrow in Init Field Textboxes", x, y, 1); y -= boxHeight;
    ls_uiLabel(c, U"   Copy current init field down", x, y, 1);    y -= 2*boxHeight;
    
    x = 0.53f*c->width;
    y = shortcutStartY;
    
    ls_uiLabel(c, U"Shift + Down Arrow in Compendium Entry", x, y, 1); y -= boxHeight;
    ls_uiLabel(c, U"   Move to next entry", x, y, 1);    y -= 2*boxHeight;
    
    ls_uiLabel(c, U"Shift + Up Arrow in Compendium Entry", x, y, 1); y -= boxHeight;
    ls_uiLabel(c, U"   Move to previous entry", x, y, 1);    y -= 2*boxHeight;
    
    return FALSE;
}

#endif //_STATE_GLOBALS_H
