struct TableEntry
{
    u16 name;
    u16 source;
};

struct MonsterTable
{
    buffer names;
    buffer sources;
    
    Array<TableEntry> entries;
    Array<u16>        displayIndices;
};

struct Codex
{
    UIButton  newEntry;
    UITextBox newEntryName;
    UITextBox newEntrySource;
    UIButton  confirmNewEntry;
};

struct Compendium
{
    Codex codex;
    
    b32 isSettingNewEntry;
};

MonsterTable monsterTable = {};
Compendium   compendium   = {};

u16 AddEntryToBufferIfMissing(buffer *buf, unistring element)
{
    ls_bufferSeekBegin(buf);
    
    u64 cursor = 0;
    while(cursor < buf->size)
    {
        unistring tmp = {};
        
        u8 len   = ls_bufferPeekData8(buf, (void **)&tmp.data);
        if(len == 0) { break; }
        
        tmp.len  = len/sizeof(u32);
        tmp.size = len/sizeof(u32);
        
        if(ls_unistrAreEqual(tmp, element)) { return cursor; }
        
        cursor += (len + 1);
        ls_bufferReadSkip(buf, len+1);
    }
    
    //NOTE: Was not found
    ls_bufferAddData8(buf, (void **)element.data, element.len*sizeof(u32));
    
    return cursor;
}

unistring GetEntryFromBuffer(buffer *buf, u16 index)
{
    unistring result = {};
    
    buf->cursor = index;
    u32 byteLen = ls_bufferPeekData8(buf, (void **)&result.data);
    result.len  = byteLen / sizeof(u32);
    
    ls_bufferSeekBegin(buf);
    
    return result;
}

void LoadCompendium(string path)
{
    ls_arenaUse(globalArena);
    
    //TODO: actually load the compendium
    monsterTable.names          = ls_bufferInit(128);
    monsterTable.sources        = ls_bufferInit(128);
    //monsterTable.entries        = (TableEntry *)ls_alloc(sizeof(TableEntry)*256);
    //monsterTable.displayIndices = (u16 *)ls_alloc(sizeof(u16)*256);
    
    
    return;
}

b32 AddEntryToCompendium(UIContext *c, void *userData)
{
    Codex *codex = &compendium.codex;
    
    compendium.isSettingNewEntry = FALSE;
    
    u16 nameIndex   = AddEntryToBufferIfMissing(&monsterTable.names,   codex->newEntryName.text);
    u16 sourceIndex = AddEntryToBufferIfMissing(&monsterTable.sources, codex->newEntrySource.text);
    
    TableEntry newEntry = {nameIndex, sourceIndex};
    u16 newEntryIdx = (u16)monsterTable.entries.push(newEntry);
    monsterTable.displayIndices.push(newEntryIdx);
    
    ls_uiTextBoxClear(c, &codex->newEntryName);
    ls_uiTextBoxClear(c, &codex->newEntrySource);
    
    //TODONOTE: Should this be undoable??
    return FALSE;
}

b32 SetupNewEntry(UIContext *c, void *userData)
{
    compendium.isSettingNewEntry = TRUE;
    
    //TODONOTE: Should this be undoable??
    return FALSE;
}

void SetMonsterTable(UIContext *c)
{
    Codex *codex = &compendium.codex;
    
    codex->newEntry        = ls_uiButtonInit(UIBUTTON_TEXT, U"+", SetupNewEntry, NULL, NULL);
    codex->confirmNewEntry = ls_uiButtonInit(UIBUTTON_TEXT, U"Add", AddEntryToCompendium, NULL, NULL);
    ls_uiTextBoxSet(c, &codex->newEntryName, U"");
    ls_uiTextBoxSet(c, &codex->newEntrySource, U"");
    
    return;
}

void DrawMonsterTable(UIContext *c)
{
    s32 baseX = 20;
    s32 baseY = 540;
    
    for(u32 i = 0; i < monsterTable.displayIndices.count; i++)
    {
        u16 index        = monsterTable.displayIndices[i];
        TableEntry entry = monsterTable.entries[index];
        
        Color bkgColor = RGBg(0x40);
        
        ls_uiRect(c, baseX-4, baseY-4, 80, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer(&monsterTable.names, entry.name), baseX, baseY, 1);
        
        ls_uiRect(c, baseX-4+79, baseY-4, 80, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer(&monsterTable.sources, entry.source), baseX+80, baseY, 1);
        
        baseY -= 19;
    }
    
    return;
}

void DrawCompendium(UIContext *c)
{
    Codex *codex = &compendium.codex;
    
    if(!compendium.isSettingNewEntry)
    {
        ls_uiButton(c, &codex->newEntry, 20, 560, 20, 20);
        
        DrawMonsterTable(c);
    }
    else
    {
        ls_uiTextBox(c, &codex->newEntryName,   20, 540, 100, 20);
        ls_uiTextBox(c, &codex->newEntrySource, 140, 540, 100, 20);
        ls_uiButton(c, &codex->confirmNewEntry, 260, 540, 40, 20);
    }
    
    return;
}