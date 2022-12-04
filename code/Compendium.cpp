struct CachedPageEntry
{
    utf32 origin;
    utf32 shortDesc;
    utf32 AC;
    utf32 HP;
    utf32 ST;
    utf32 RD;
    utf32 RI;
    utf32 defensiveCapacity;
    utf32 melee;
    utf32 ranged;
    utf32 specialAttacks;
    utf32 psych;
    utf32 magics;
    utf32 spells;
    utf32 racialMods;
    utf32 spec_qual; //Duplicate of defensiveCapacity + magicalCapacity + special attacks??
    utf32 org;
    utf32 treasure;
    utf32 desc;
    utf32 source;
    
    utf32 name;
    utf32 gs;
    utf32 pe;
    utf32 alignment;
    utf32 type;
    utf32 subtype[8];
    utf32 archetype[4];
    utf32 size;
    utf32 initiative;
    utf32 senses[8];
    utf32 perception;
    utf32 aura;
    utf32 immunities[16];
    utf32 resistances[16];
    utf32 weaknesses[16];
    utf32 speed;
    utf32 space;
    utf32 reach;
    utf32 STR;
    utf32 DEX;
    utf32 CON;
    utf32 INT;
    utf32 WIS;
    utf32 CHA;
    utf32 BAB;
    utf32 BMC;
    utf32 DMC;
    utf32 talents[24];
    utf32 skills[24]; //TODO Separate type from value
    utf32 languages[24];
    utf32 specials[24];
    utf32 enviroment;
};

struct PageEntry
{
    /*Image?*/
    /*Menù*/
    
    u32 origin;
    u32 shortDesc;
    u32 AC;
    u32 HP;
    u32 ST;
    u32 RD;
    u32 RI;
    u32 defensiveCapacity;
    u32 melee;
    u32 ranged;
    u32 specialAttacks;
    u32 psych;
    u32 magics;
    u32 spells;
    u32 racialMods;
    u32 spec_qual; //Duplicate of defensiveCapacity + magicalCapacity + special attacks??
    u32 org;
    u32 treasure;
    u32 desc;
    u32 source;
    
    u16 name;
    u16 gs;
    u16 pe;
    u16 alignment;
    u16 type;
    u16 subtype[8];
    u16 archetype[4];
    u16 size;
    u16 initiative;
    u16 senses[8];
    u16 perception;
    u16 aura;
    u16 immunities[16];
    u16 resistances[16];
    u16 weaknesses[16];
    u16 speed;
    u16 space;
    u16 reach;
    u16 STR;
    u16 DEX;
    u16 CON;
    u16 INT;
    u16 WIS;
    u16 CHA;
    u16 BAB;
    u16 BMC;
    u16 DMC;
    u16 talents[24];
    u16 skills[24]; //TODO Separate type from value
    u16 languages[24];
    u16 specials[24];
    u16 enviroment;
};

struct TableEntry
{
    u16 name;
    u16 gs;
    u16 environment;
    u16 type;
    u16 subtype;
    u16 source;
    
    u32 page;
};

struct MonsterTable
{
    //NOTE: These buffers are used by the table, 
    //      but most informations will be used globally as well.
    buffer names;
    buffer gs;
    //buffer terrains;
    //buffer climates;
    buffer types;
    buffer subtypes;
    //buffer sources;
    
    Array<TableEntry> entries;
    
    //TODO: We'll add this when adding sorting / searching
    //Array<u16>        displayIndices;
};

struct Codex
{
    buffer names;
    buffer gs;
    buffer types;
    buffer subtypes;
    
    buffer generalStrings;
    buffer numericValues;
    buffer pe;
    buffer alignment;
    buffer archetypes;
    buffer sizes;
    buffer senses;
    buffer auras;
    buffer immunities;
    buffer resistances;
    buffer weaknesses;
    buffer specialAttacks;
    buffer spells;
    buffer talents;
    buffer skills;
    buffer languages;
    buffer environment;
    buffer specials;
    
    //TODO: Change this to a FixedArray
    Array<PageEntry> pages;
    
    UIButton  newEntry;
    
    UITextBox newEntryName;
    UITextBox newEntryGS;
    UITextBox newEntryTerrain;
    UITextBox newEntryClimate;
    UITextBox newEntryType;
    UITextBox newEntrySubtype;
    UITextBox newEntrySource;
    
    UIButton  confirmNewEntry;
    
    UIButton  saveCompendium;
};

struct Compendium
{
    Codex codex;
    
    b32 isSettingNewEntry;
    
    b32         isViewingPage;
    u32         pageIndex;
};

//MonsterTable monsterTable = {};
Compendium      compendium = {};
CachedPageEntry cachedPage = {};

#if 0 //TODO Re-implement correct
u16 AddEntryToBufferIfMissing(buffer *buf, unistring element)
{
    //TODO: This HAS to change the data size. It MUST be 16 bit otherwise strings just won't fit.
    buf->cursor = 4;
    
    //NOTE: The first 32bit integer of the reference tables are the size of the entire buffer 
    //      (to simplify serialization)
    u64 cursor = 4;
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
    
    ls_bufferSeekBegin(buf);
    u32 currentSize = ls_bufferPeekDWord(buf);
    u32 newSize = currentSize + (element.len*sizeof(u32)) + 1;
    ls_bufferChangeDWord(buf, newSize);
    
    return cursor;
}
#endif

utf32 GetEntryFromBuffer(buffer *buf, u32 index)
{
    buf->cursor = index;
    
    u32 byteLen   = (u32)ls_bufferPeekWord(buf);
    u8 *utf8_data = (u8 *)buf->data + buf->cursor + 2;
    
    utf32 result = ls_utf32FromUTF8(utf8_data, byteLen);
    
    ls_bufferSeekBegin(buf);
    
    return result;
}

utf32 GetEntryFromBuffer(buffer *buf, u16 index)
{
    return GetEntryFromBuffer(buf, (u32)index);
}


void LoadCompendium(string path)
{
    ls_arenaUse(compendiumArena);
    
    buffer CompendiumBuff = ls_bufferInitFromFile(path);
    
    //NOTE: No valid file was found.
    if(CompendiumBuff.data == 0)
    {
        compendium.codex.names          = ls_bufferInit(128);
        compendium.codex.gs             = ls_bufferInit(128);
        compendium.codex.types          = ls_bufferInit(128);
        compendium.codex.subtypes       = ls_bufferInit(128);
        
        compendium.codex.numericValues  = ls_bufferInit(128);
        compendium.codex.pe             = ls_bufferInit(128);
        compendium.codex.alignment      = ls_bufferInit(128);
        compendium.codex.archetypes     = ls_bufferInit(128);
        compendium.codex.senses         = ls_bufferInit(128);
        compendium.codex.auras          = ls_bufferInit(128);
        compendium.codex.immunities     = ls_bufferInit(128);
        compendium.codex.resistances    = ls_bufferInit(128);
        compendium.codex.weaknesses     = ls_bufferInit(128);
        compendium.codex.specialAttacks = ls_bufferInit(128);
        compendium.codex.spells         = ls_bufferInit(128);
        compendium.codex.talents        = ls_bufferInit(128);
        compendium.codex.skills         = ls_bufferInit(128);
        compendium.codex.languages      = ls_bufferInit(128);
        compendium.codex.environment    = ls_bufferInit(128);
        compendium.codex.specials       = ls_bufferInit(128);
    }
    else
    {
        const u32 reserve = 32;
        
        //NOTE: We point to the beginning of the block, which contains a 4 bytes size
        //      And also copy the size into the buffer struct itself. This is necessary
        //      Because all the indices in the Page Entries are relative to the complete block, size included
        auto viewIntoBuffer = [reserve](buffer *src, buffer *dst) {
            u8 *blockBegin       = (u8 *)src->data + src->cursor;
            u32 blockSize        = ls_bufferReadDWord(src);
            *dst                 = ls_bufferViewIntoPtr((void *)blockBegin, blockSize);
            ls_bufferReadSkip(src, blockSize);
        };
        
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.generalStrings);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.numericValues);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.names);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.gs);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.pe);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.alignment);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.types);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.subtypes);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.archetypes);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.sizes);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.senses);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.auras);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.immunities);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.resistances);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.weaknesses);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.specialAttacks);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.spells);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.talents);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.skills);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.languages);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.environment);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.specials);
        
        //TODO: Change this to a FixedArray 
        u32 entryCount = ls_bufferReadDWord(&CompendiumBuff);
        u8 *pagesSrc = (u8 *)CompendiumBuff.data + CompendiumBuff.cursor;
        ls_arrayFromPointer(&compendium.codex.pages, (void *)pagesSrc, entryCount);
    }
    
    ls_arenaUse(globalArena);
    
    return;
}

void SaveCompendium(string path)
{
    AssertMsg(FALSE, "This is completely wrong and has to be re-implemented");
#if 0
    char cPath[128] = {};
    ls_strToCStr_t(path, cPath, 128);
    
    ls_bufferSeekBegin(&monsterTable.names);
    ls_bufferSeekBegin(&monsterTable.gs);
    ls_bufferSeekBegin(&monsterTable.terrains);
    ls_bufferSeekBegin(&monsterTable.climates);
    ls_bufferSeekBegin(&monsterTable.types);
    ls_bufferSeekBegin(&monsterTable.subtypes);
    ls_bufferSeekBegin(&monsterTable.sources);
    
    u32 namesSize    = ls_bufferPeekDWord(&monsterTable.names);
    u32 gsSize       = ls_bufferPeekDWord(&monsterTable.gs);
    u32 terrainsSize = ls_bufferPeekDWord(&monsterTable.terrains);
    u32 climatesSize = ls_bufferPeekDWord(&monsterTable.climates);
    u32 typesSize    = ls_bufferPeekDWord(&monsterTable.types);
    u32 subtypesSize = ls_bufferPeekDWord(&monsterTable.subtypes);
    u32 sourcesSize  = ls_bufferPeekDWord(&monsterTable.sources);
    
    ls_writeFile(cPath, monsterTable.names.data,    namesSize + 4,    FALSE);
    ls_writeFile(cPath, monsterTable.gs.data,       gsSize + 4,       TRUE);
    ls_writeFile(cPath, monsterTable.terrains.data, terrainsSize + 4, TRUE);
    ls_writeFile(cPath, monsterTable.climates.data, climatesSize + 4, TRUE);
    ls_writeFile(cPath, monsterTable.types.data,    typesSize + 4,    TRUE);
    ls_writeFile(cPath, monsterTable.subtypes.data, subtypesSize + 4, TRUE);
    ls_writeFile(cPath, monsterTable.sources.data,  sourcesSize + 4,  TRUE);
    
    //NOTETODO We are not writing explicitly the size of this last array.
    //         For now it's fine because being the last it can be implied from the total size of the file.
    ls_writeFile(cPath, monsterTable.entries.data, monsterTable.entries.count*sizeof(TableEntry), TRUE);
#endif
    return;
}

b32 AddEntryToCompendium(UIContext *c, void *userData)
{
    AssertMsg(FALSE, "This is completely wrong and has to be re-implemented");
#if 0
    Codex *codex = &compendium.codex;
    
    compendium.isSettingNewEntry = FALSE;
    
    u16 nameIndex    = AddEntryToBufferIfMissing(&monsterTable.names,    codex->newEntryName.text);
    u16 gsIndex      = AddEntryToBufferIfMissing(&monsterTable.gs,       codex->newEntryGS.text);
    u16 terrainIndex = AddEntryToBufferIfMissing(&monsterTable.terrains, codex->newEntryTerrain.text);
    u16 climateIndex = AddEntryToBufferIfMissing(&monsterTable.climates, codex->newEntryClimate.text);
    u16 typeIndex    = AddEntryToBufferIfMissing(&monsterTable.types,    codex->newEntryType.text);
    u16 subtypeIndex = AddEntryToBufferIfMissing(&monsterTable.subtypes, codex->newEntrySubtype.text);
    u16 sourceIndex  = AddEntryToBufferIfMissing(&monsterTable.sources,  codex->newEntrySource.text);
    
    TableEntry newEntry = {nameIndex, gsIndex, terrainIndex, climateIndex, typeIndex, subtypeIndex, sourceIndex, 0};
    u16 newEntryIdx = (u16)ls_arrayAppend(&monsterTable.entries, newEntry);
    ls_arrayAppend(&monsterTable.displayIndices, newEntryIdx);
    
    ls_uiTextBoxClear(c, &codex->newEntryName);
    ls_uiTextBoxClear(c, &codex->newEntryGS);
    ls_uiTextBoxClear(c, &codex->newEntryTerrain);
    ls_uiTextBoxClear(c, &codex->newEntryClimate);
    ls_uiTextBoxClear(c, &codex->newEntryType);
    ls_uiTextBoxClear(c, &codex->newEntrySubtype);
    ls_uiTextBoxClear(c, &codex->newEntrySource);
#endif
    
    //TODONOTE: Should this be undoable??
    return FALSE;
}

b32 SetupNewEntry(UIContext *c, void *userData)
{
    compendium.isSettingNewEntry = TRUE;
    
    //TODONOTE: Should this be undoable??
    return FALSE;
}

b32 SaveCompendiumOnClick(UIContext *c, void *userData)
{
    SaveCompendium(ls_strConstant("Compendium"));
    return FALSE;
}

void SetMonsterTable(UIContext *c)
{
    Codex *codex = &compendium.codex;
    
    codex->newEntry        = ls_uiButtonInit(UIBUTTON_TEXT, U"+", SetupNewEntry, NULL, NULL);
    codex->confirmNewEntry = ls_uiButtonInit(UIBUTTON_TEXT, U"Add", AddEntryToCompendium, NULL, NULL);
    codex->saveCompendium  = ls_uiButtonInit(UIBUTTON_TEXT, U"Save", SaveCompendiumOnClick, NULL, NULL);
    
    ls_uiTextBoxSet(c, &codex->newEntryName, U"");
    codex->newEntryName.isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &codex->newEntryGS, U"");
    codex->newEntryGS.isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &codex->newEntryTerrain, U"");
    codex->newEntryTerrain.isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &codex->newEntryClimate, U"");
    codex->newEntryClimate.isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &codex->newEntryType, U"");
    codex->newEntryType.isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &codex->newEntrySubtype, U"");
    codex->newEntrySubtype.isSingleLine = TRUE;
    
    ls_uiTextBoxSet(c, &codex->newEntrySource, U"");
    codex->newEntrySource.isSingleLine = TRUE;
    
    ls_uiSelectFontByFontSize(c, FS_SMALL);
    
    return;
}

void DrawMonsterTable(UIContext *c)
{
    Input *UserInput = &c->UserInput;
    Codex *codex     = &compendium.codex;
    
    s32 baseX = 20;
    s32 baseY = 540;
    
    for(u32 i = 0; i < codex->pages.count; i++)
    {
        PageEntry entry = codex->pages[i];
        
        Color bkgColor = RGBg(0x40);
        
        //NOTETODO: MAYBE I don't like a lot how this works.
        if(LeftClickIn(baseX, baseY, 80, 20)) 
        { 
            compendium.isViewingPage = TRUE; 
            compendium.pageIndex     = i;
        }
        
        ls_uiRect(c, baseX-4, baseY-4, 120, 20, bkgColor, c->borderColor);
        
        ls_uiLabel(c, GetEntryFromBuffer(&codex->names, entry.name), baseX, baseY, 1);
        baseX += 119;
        
        ls_uiRect(c, baseX-4, baseY-4, 120, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer(&codex->gs, entry.gs), baseX, baseY, 1);
        baseX += 119;
        
        ls_uiRect(c, baseX-4, baseY-4, 120, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer(&codex->types, entry.type), baseX, baseY, 1);
        baseX += 119;
        
        //TODO: Show all subtypes. Right now we only grab one.
        //      We can do this by overloading the function GetEntryFromBuffer and passing the array itself.
        ls_uiRect(c, baseX-4, baseY-4, 120, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer(&codex->subtypes, entry.subtype[0]), baseX, baseY, 1);
        baseX += 119;
        
        baseY -= 19;
        baseX = 20;
    }
    
    return;
}

void DrawPage(UIContext *c, PageEntry *page)
{
    AssertMsg(FALSE, "Not implemented yet");
#if 0
    if(page)
    {
    }
    else
    {
        s32 baseY = 670;
        s32 valueBaseX = 148;
        
        //NOTE: Draw an empty page template
        
        ls_uiSelectFontByFontSize(c, FS_LARGE);
        ls_uiLabel(c, GetEntryFromBuffer(&monsterTable.names, entry->name), 10, baseY);
        ls_uiLabel(c, U"GS", 440, baseY);
        ls_uiLabel(c, GetEntryFromBuffer(&monsterTable.gs, entry->gs), 480, baseY);
        ls_uiLabel(c, U"PE", 545, baseY);
        ls_uiLabel(c, GetEntryFromBuffer(&compendium.codex.pe, page->pe), 580, baseY);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        
        ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseY -= 30;
        
        ls_uiLabel(c, U"Descrizione breve...", 10, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"Allineamento: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"Categoria: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"Iniziativa: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"Sensi: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 30;
        
        ls_uiSelectFontByFontSize(c, FS_LARGE);
        ls_uiLabel(c, U"Difesa", 10, baseY);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        
        ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseY -= 30;
        
        ls_uiLabel(c, U"CA: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"PF: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"Tiri Salvezza: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"RD: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"Immunit\U000000E0: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 30;
        
        ls_uiSelectFontByFontSize(c, FS_LARGE);
        ls_uiLabel(c, U"Attacco", 10, baseY);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        
        ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseY -= 30;
        
        ls_uiLabel(c, U"Velocit\U000000E0: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"Mischia: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 30;
        
        ls_uiSelectFontByFontSize(c, FS_LARGE);
        ls_uiLabel(c, U"Statistiche", 10, baseY);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        
        ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseY -= 30;
        
        ls_uiLabel(c, U"Caratteristiche: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"BAB: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"BMC: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"DMC: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"Talenti: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 20;
        
        ls_uiLabel(c, U"Qualit\U000000E0 Speciali: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
        baseY -= 30;
        
        ls_uiSelectFontByFontSize(c, FS_MEDIUM);
        ls_uiLabel(c, U"Barcollante (Str)", 10, baseY);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        baseY -= 40;
        
        ls_uiSelectFontByFontSize(c, FS_MEDIUM);
        ls_uiLabel(c, U"Tratti dei Non Morti (Str)", 10, baseY);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        baseY -= 40;
        
        ls_uiSelectFontByFontSize(c, FS_LARGE);
        ls_uiLabel(c, U"Descrizione", 10, baseY);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        baseY -= 30;
        
        ls_uiSelectFontByFontSize(c, FS_SMALL);
        ls_uiLabel(c, U"---", 10, baseY);
        baseY -= 30;
        
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(30, 30, 30));
        ls_uiLabel(c, U"Fonte: ", 10, baseY);
        ls_uiLabel(c, U"---", valueBaseX, baseY);
    }
#endif
}

void DrawCompendium(UIContext *c)
{
    Codex *codex = &compendium.codex;
    
    if(compendium.isSettingNewEntry)
    {
        ls_uiTextBox(c, &codex->newEntryName,    20,  660, 80, 20);
        ls_uiTextBox(c, &codex->newEntryGS,      100, 660, 80, 20);
        ls_uiTextBox(c, &codex->newEntryTerrain, 180, 660, 80, 20);
        ls_uiTextBox(c, &codex->newEntryClimate, 260, 660, 80, 20);
        ls_uiTextBox(c, &codex->newEntryType,    340, 660, 80, 20);
        ls_uiTextBox(c, &codex->newEntrySubtype, 420, 660, 80, 20);
        ls_uiTextBox(c, &codex->newEntrySource,  500, 660, 80, 20);
        
        ls_uiButton(c, &codex->confirmNewEntry,  580, 660, 40, 20);
    }
    else if(compendium.isViewingPage)
    {
        PageEntry *page = 0;
        
        if(compendium.pageIndex != 0) page = compendium.codex.pages + compendium.pageIndex;
        
        //DrawPage(c, compendium.tableEntry, page);
    }
    else
    {
        ls_uiButton(c, &codex->newEntry, 20, 670, 20, 20);
        ls_uiButton(c, &codex->saveCompendium, 120, 670, 50, 20);
        
        DrawMonsterTable(c);
    }
    
    return;
}