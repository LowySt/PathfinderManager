/* date = May 22nd 2020 11:45 am */

#ifndef _STATE_GLOBALS_H
#define _STATE_GLOBALS_H

const u32 global_saveVersion = 5;

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

static Arena globalArena;
static Arena fileArena;
static Arena stateArena;
static Arena saveArena;
static Arena renderArena;
static Arena compendiumArena;
static Arena compTempArena;

#endif //_STATE_GLOBALS_H
