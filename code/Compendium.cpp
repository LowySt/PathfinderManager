struct CachedPageEntry
{
    s32 pageIndex = -1;
    
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
    utf32 environment;
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
    u16 environment;
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
};

struct Compendium
{
    Codex codex;
    
    b32   isViewingPage;
    s32   pageIndex = -1;
};

//MonsterTable monsterTable = {};
Compendium      compendium = {};
CachedPageEntry cachedPage = {};
ScrollableRegion scroll = {};

b32 CompendiumOpenMonsterTable(UIContext *c, void *userData)
{
    compendium.isViewingPage = FALSE;
    compendium.pageIndex     = -1;
    
    return FALSE;
}

void LoadCompendium(string path)
{
    ls_arenaUse(compendiumArena);
    
    //----------------------
    //NOTE: First Initialize the cached page to avoid constant alloc/free when setting it
    cachedPage.origin            = ls_utf32Alloc(48);
    cachedPage.shortDesc         = ls_utf32Alloc(256);
    cachedPage.AC                = ls_utf32Alloc(128);
    cachedPage.HP                = ls_utf32Alloc(128);
    cachedPage.ST                = ls_utf32Alloc(128);
    cachedPage.RD                = ls_utf32Alloc(128);
    cachedPage.RI                = ls_utf32Alloc(128);
    cachedPage.defensiveCapacity = ls_utf32Alloc(128);
    cachedPage.melee             = ls_utf32Alloc(256);
    cachedPage.ranged            = ls_utf32Alloc(256);
    cachedPage.specialAttacks    = ls_utf32Alloc(256);
    cachedPage.psych             = ls_utf32Alloc(2048);
    cachedPage.magics            = ls_utf32Alloc(2048);
    cachedPage.spells            = ls_utf32Alloc(2048);
    cachedPage.racialMods        = ls_utf32Alloc(128);
    cachedPage.spec_qual         = ls_utf32Alloc(256);
    cachedPage.org               = ls_utf32Alloc(448);
    cachedPage.treasure          = ls_utf32Alloc(256);
    cachedPage.desc              = ls_utf32Alloc(4096);
    cachedPage.source            = ls_utf32Alloc(256);
    
    cachedPage.name              = ls_utf32Alloc(48);
    cachedPage.gs                = ls_utf32Alloc(16);
    cachedPage.pe                = ls_utf32Alloc(16);
    cachedPage.alignment         = ls_utf32Alloc(32);
    cachedPage.type              = ls_utf32Alloc(32);
    
    for(u32 i = 0; i < 8; i++) { cachedPage.subtype[i]   = ls_utf32Alloc(32); }
    for(u32 i = 0; i < 4; i++) { cachedPage.archetype[i] = ls_utf32Alloc(32); }
    
    cachedPage.size       = ls_utf32Alloc(32);
    cachedPage.initiative = ls_utf32Alloc(32);
    
    for(u32 i = 0; i < 8; i++) { cachedPage.senses[i] = ls_utf32Alloc(32); }
    
    cachedPage.perception = ls_utf32Alloc(64);
    cachedPage.aura       = ls_utf32Alloc(128);
    
    for(u32 i = 0; i < 16; i++) { cachedPage.immunities[i]  = ls_utf32Alloc(48); }
    for(u32 i = 0; i < 16; i++) { cachedPage.resistances[i] = ls_utf32Alloc(48); }
    for(u32 i = 0; i < 16; i++) { cachedPage.weaknesses[i]  = ls_utf32Alloc(48); }
    
    cachedPage.speed = ls_utf32Alloc(64);
    cachedPage.space = ls_utf32Alloc(32);
    cachedPage.reach = ls_utf32Alloc(64);
    cachedPage.STR   = ls_utf32Alloc(16);
    cachedPage.DEX   = ls_utf32Alloc(16);
    cachedPage.CON   = ls_utf32Alloc(16);
    cachedPage.INT   = ls_utf32Alloc(16);
    cachedPage.WIS   = ls_utf32Alloc(16);
    cachedPage.CHA   = ls_utf32Alloc(16);
    cachedPage.BAB   = ls_utf32Alloc(16);
    cachedPage.BMC   = ls_utf32Alloc(64);
    cachedPage.DMC   = ls_utf32Alloc(64);
    
    for(u32 i = 0; i < 24; i++) { cachedPage.talents[i]   = ls_utf32Alloc(64); }
    for(u32 i = 0; i < 24; i++) { cachedPage.skills[i]    = ls_utf32Alloc(64); }
    for(u32 i = 0; i < 24; i++) { cachedPage.languages[i] = ls_utf32Alloc(64); }
    for(u32 i = 0; i < 24; i++) { cachedPage.specials[i]  = ls_utf32Alloc(4096); }
    
    cachedPage.environment = ls_utf32Alloc(64);
    
    //----------------------
    //NOTE: Now load the Compendium from file
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

void GetEntryFromBuffer_t(buffer *buf, utf32 *toSet, u32 index)
{
    if(index == 0) { return; } //NOTE: Index zero means no entry
    
    buf->cursor = index;
    
    u32 byteLen   = (u32)ls_bufferPeekWord(buf);
    u8 *utf8_data = (u8 *)buf->data + buf->cursor + 2;
    
    u32 len = ls_utf8Len(utf8_data, byteLen);
    
    if(toSet->size < len)
    { ls_printf("Fuck Size: %d, Len: %d, ByteLen: %d, Index: %d\n", toSet->size, len, byteLen, index); }
    
    ls_utf32FromUTF8_t(toSet, utf8_data, len);
    
    ls_bufferSeekBegin(buf);
}

void GetEntryFromBuffer_t(buffer *buf, utf32 *toSet, u16 index)
{
    GetEntryFromBuffer_t(buf, toSet, (u32)index);
}

//NOTE: We intentionally don't free the utf32 data because at the beginning
//      Of each DrawCompendium() we clear the compTempArena
utf32 GetEntryFromBuffer(buffer *buf, u32 index)
{
    ls_arenaUse(compTempArena);
    
    if(index == 0) { return {}; } //NOTE: Index zero means no entry
    
    buf->cursor = index;
    
    u32 byteLen   = (u32)ls_bufferPeekWord(buf);
    u8 *utf8_data = (u8 *)buf->data + buf->cursor + 2;
    
    utf32 result = ls_utf32FromUTF8(utf8_data, byteLen);
    
    ls_bufferSeekBegin(buf);
    
    ls_arenaUse(globalArena);
    
    return result;
}

utf32 GetEntryFromBuffer(buffer *buf, u16 index)
{
    return GetEntryFromBuffer(buf, (u32)index);
}

utf8 GetEntryFromBuffer_8(buffer *buf, u32 index)
{
    if(index == 0) { return {}; } //NOTE: Index zero means no entry
    
    buf->cursor = index;
    
    u32 byteLen   = (u32)ls_bufferPeekWord(buf);
    u8 *utf8_data = (u8 *)buf->data + buf->cursor + 2;
    
    utf8 result = ls_utf8Constant(utf8_data, (s32)byteLen);
    
    ls_bufferSeekBegin(buf);
    
    return result;
}

utf8 GetEntryFromBuffer_8(buffer *buf, u16 index)
{
    return GetEntryFromBuffer_8(buf, (u32)index);
}

void SetMonsterTable(UIContext *c)
{
    Codex *codex = &compendium.codex;
    
    ls_uiSelectFontByFontSize(c, FS_SMALL);
    
    return;
}

void DrawMonsterTable(UIContext *c)
{
    Input *UserInput = &c->UserInput;
    Codex *codex     = &compendium.codex;
    
    s32 baseX = 20;
    s32 baseY = 540;
    
    s32 entryRenderCount = (codex->pages.count > 32) ? 32 : codex->pages.count;
    
    for(u32 i = 0; i < entryRenderCount; i++)
    {
        PageEntry entry = codex->pages[i];
        
        Color bkgColor = RGBg(0x40);
        
        //NOTETODO: MAYBE I don't like a lot how this works.
        if(LeftClickIn(baseX, baseY, 300, 20)) 
        { 
            compendium.isViewingPage = TRUE; 
            compendium.pageIndex     = i;
        }
        
        ls_uiRect(c, baseX-4, baseY-4, 300, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->names, entry.name), baseX, baseY, 1);
        baseX += 299;
        
        ls_uiRect(c, baseX-4, baseY-4, 80, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->gs, entry.gs), baseX, baseY, 1);
        baseX += 79;
        
        ls_uiRect(c, baseX-4, baseY-4, 180, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->types, entry.type), baseX, baseY, 1);
        baseX += 179;
        
        //TODO: Show all subtypes. Right now we only grab one.
        //      We can do this by overloading the function GetEntryFromBuffer and passing the array itself.
        ls_uiRect(c, baseX-4, baseY-4, 120, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->subtypes, entry.subtype[0]), baseX, baseY, 1);
        baseX += 119;
        
        baseY -= 19;
        baseX = 20;
    }
    
    return;
}

void CachePage(UIContext *ui, PageEntry page)
{
    Codex *c = &compendium.codex;
    
    cachedPage.pageIndex = compendium.pageIndex;
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.origin, page.origin);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.shortDesc, page.shortDesc);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.AC, page.AC);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.HP, page.HP);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.ST, page.ST);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.RD, page.RD);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.RI, page.RI);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.defensiveCapacity, page.defensiveCapacity);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.melee, page.melee);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.ranged, page.ranged);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.specialAttacks, page.specialAttacks);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.psych, page.psych);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.magics, page.magics);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.spells, page.spells);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.racialMods, page.racialMods);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.spec_qual, page.spec_qual);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.org, page.org);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.treasure, page.treasure);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.desc, page.desc);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage.source, page.source);
    
    GetEntryFromBuffer_t(&c->names, &cachedPage.name, page.name);
    GetEntryFromBuffer_t(&c->gs, &cachedPage.gs, page.gs);
    GetEntryFromBuffer_t(&c->pe, &cachedPage.pe, page.pe);
    GetEntryFromBuffer_t(&c->alignment, &cachedPage.alignment, page.alignment);
    GetEntryFromBuffer_t(&c->types, &cachedPage.type, page.type);
    
    for(u32 i = 0; i < 8; i++) { GetEntryFromBuffer_t(&c->subtypes, cachedPage.subtype + i, page.subtype[i]); }
    for(u32 i = 0; i < 4; i++) { GetEntryFromBuffer_t(&c->archetypes, cachedPage.archetype + i, page.archetype[i]); }
    
    GetEntryFromBuffer_t(&c->sizes, &cachedPage.size, page.size);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.initiative, page.initiative);
    
    for(u32 i = 0; i < 8; i++) { GetEntryFromBuffer_t(&c->senses, cachedPage.senses + i, page.senses[i]); }
    
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.perception, page.perception);
    GetEntryFromBuffer_t(&c->auras, &cachedPage.aura, page.aura);
    
    for(u32 i = 0; i < 16; i++) {GetEntryFromBuffer_t(&c->immunities, cachedPage.immunities + i, page.immunities[i]);}
    for(u32 i = 0; i < 16; i++) {GetEntryFromBuffer_t(&c->resistances, cachedPage.resistances+i, page.resistances[i]);}
    for(u32 i = 0; i < 16; i++) {GetEntryFromBuffer_t(&c->weaknesses, cachedPage.weaknesses + i, page.weaknesses[i]);}
    
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.speed, page.speed);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.space, page.space);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.reach, page.reach);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.STR, page.STR);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.DEX, page.DEX);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.CON, page.CON);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.INT, page.INT);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.WIS, page.WIS);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.CHA, page.CHA);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.BAB, page.BAB);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.BMC, page.BMC);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage.DMC, page.DMC);
    
    for(u32 i = 0; i < 24; i++) { GetEntryFromBuffer_t(&c->talents, cachedPage.talents + i, page.talents[i]); }
    for(u32 i = 0; i < 24; i++) { GetEntryFromBuffer_t(&c->skills, cachedPage.skills + i, page.skills[i]); }
    for(u32 i = 0; i < 24; i++) { GetEntryFromBuffer_t(&c->languages, cachedPage.languages + i, page.languages[i]); }
    for(u32 i = 0; i < 24; i++) { GetEntryFromBuffer_t(&c->specials, cachedPage.specials + i, page.specials[i]); }
    
    GetEntryFromBuffer_t(&c->environment, &cachedPage.environment, page.environment);
    
    scroll = { 0, 10, ui->windowWidth - 4, ui->windowHeight - 36, 0, 0, ui->windowWidth - 32, 0};
}

void DrawPage(UIContext *c, CachedPageEntry *page)
{
    //NOTE: The first frame is impossible to scroll, because the minY value will be not initialized yet
    //      It's should be fine though. We run at 30FPS on the Compendium, so it should never be felt/seen.
    ls_uiStartScrollableRegion(c, &scroll);
    
    //NOTE: Used to adjust the next line position when changing font size, because
    //      a change in pixel height will make the next line too close / too far from the ideal position.
    s32 prevPixelHeight = 0;
    s32 currPixelHeight = 0;
    
    s32 maxX = c->windowWidth - 32;
    s32 minY = 0;
    
    s32 origY = 670;
    s32 baseY = 670;
    s32 valueBaseX = 148;
    s32 yOff = 0;
    
    Color pureWhite = RGBg(0xFF);
    c->textColor = RGBg(0xAA);
    
    prevPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    ls_uiLabelLayout(c, page->name, { 10, baseY, maxX, minY }, pureWhite);
    ls_uiLabelLayout(c, U"GS", { 510, baseY, maxX, minY }, pureWhite);
    ls_uiLabelLayout(c, page->gs, { 540, baseY, maxX, minY }, pureWhite);
    ls_uiLabelLayout(c, U"PE", { 635, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->pe, { 670, baseY, maxX, minY }, pureWhite);
    ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
    baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= yOff;
    
    yOff = ls_uiLabelLayout(c, page->shortDesc, { 10, baseY, maxX, minY });
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"Allineamento: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->alignment, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"Categoria: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->type, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"Iniziativa: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->initiative, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    //TODO: All of them
    if(page->senses[0].len)
    {
        ls_uiLabelLayout(c, U"Sensi: ", { 10, baseY, maxX, minY }, pureWhite);
        yOff = ls_uiLabelLayout(c, page->senses[0], { valueBaseX, baseY, maxX, minY });
        baseY -= yOff;
    }
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    
    yOff = ls_uiLabelLayout(c, U"Difesa", { 10, baseY, maxX, minY }, pureWhite);
    ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
    baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"CA: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->AC, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"PF: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->HP, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"Tiri Salvezza: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->ST, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    if(page->RD.len)
    {
        ls_uiLabelLayout(c, U"RD: ", { 10, baseY, maxX, minY }, pureWhite);
        yOff = ls_uiLabelLayout(c, page->RD, { valueBaseX, baseY, maxX, minY });
        baseY -= yOff;
    }
    
    if(page->RI.len)
    {
        ls_uiLabelLayout(c, U"RI: ", { 10, baseY, maxX, minY }, pureWhite);
        yOff = ls_uiLabelLayout(c, page->RI, { valueBaseX, baseY, maxX, minY });
        baseY -= yOff;
    }
    
    //TODO: X offset
    if(page->immunities[0].len)
    {
        ls_uiLabelLayout(c, U"Immunit\U000000E0: ", { 10, baseY, maxX, minY }, pureWhite);
        for(u32 i = 0; i < 16; i++)
        {
            if(page->immunities[i].len == 0) { break; }
            yOff = ls_uiLabelLayout(c, page->immunities[i], { valueBaseX, baseY, maxX, minY });
        }
        baseY -= yOff;
    }
    
    //TODO: X offset
    if(page->resistances[0].len)
    {
        ls_uiLabelLayout(c, U"Resistenze: ", { 10, baseY, maxX, minY }, pureWhite);
        for(u32 i = 0; i < 16; i++)
        {
            if(page->resistances[i].len == 0) { break; }
            yOff = ls_uiLabelLayout(c, page->resistances[i], { valueBaseX, baseY, maxX, minY });
        }
        baseY -= yOff;
    }
    
    //TODO: X offset
    if(page->weaknesses[0].len)
    {
        ls_uiLabelLayout(c, U"Debolezze: ", { 10, baseY, maxX, minY }, pureWhite);
        for(u32 i = 0; i < 16; i++)
        {
            if(page->weaknesses[i].len == 0) { break; }
            yOff = ls_uiLabelLayout(c, page->weaknesses[0], { valueBaseX, baseY, maxX, minY });
        }
        
        baseY -= yOff;
    }
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    
    yOff = ls_uiLabelLayout(c, U"Attacco", { 10, baseY, maxX, minY }, pureWhite);
    ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
    baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= yOff;
    
    //TODO: Temporary
    valueBaseX = 200;
    ls_uiLabelLayout(c, U"Velocit\U000000E0: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->speed, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"Mischia: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->melee, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    if(page->ranged.len)
    {
        ls_uiLabelLayout(c, U"Distanza: ", { 10, baseY, maxX, minY }, pureWhite);
        yOff = ls_uiLabelLayout(c, page->ranged, { valueBaseX, baseY, maxX, minY });
        baseY -= yOff;
    }
    
    if(page->specialAttacks.len)
    {
        ls_uiLabelLayout(c, U"Attacchi Speciali: ", { 10, baseY, maxX, minY }, pureWhite);
        yOff = ls_uiLabelLayout(c, page->specialAttacks, { valueBaseX, baseY, maxX, minY });
        baseY -= yOff;
    }
    
    if(page->space.len)
    {
        ls_uiLabelLayout(c, U"Spazio: ", { 10, baseY, maxX, minY }, pureWhite);
        yOff = ls_uiLabelLayout(c, page->space, { valueBaseX, baseY, maxX, minY });
        baseY -= yOff;
    }
    
    if(page->reach.len)
    {
        ls_uiLabelLayout(c, U"Portata: ", { 10, baseY, maxX, minY }, pureWhite);
        yOff = ls_uiLabelLayout(c, page->reach, { valueBaseX, baseY, maxX, minY });
        baseY -= yOff;
    }
    
    if(page->psych.len)
    {
        ls_uiLabelLayout(c, U"Magia Psichica: ", { 10, baseY, maxX, minY }, pureWhite);
        yOff = ls_uiLabelLayout(c, page->psych, { valueBaseX, baseY, maxX, minY });
        baseY -= yOff;
    }
    
    if(page->magics.len)
    {
        ls_uiLabelLayout(c, U"Capacit\U000000E0 Magiche: ", { 10, baseY, maxX, minY }, pureWhite);
        yOff = ls_uiLabelLayout(c, page->magics, { valueBaseX, baseY, maxX, minY });
        baseY -= yOff;
    }
    
    if(page->spells.len)
    {
        ls_uiLabelLayout(c, U"Incantesimi Conosciuti: ", { 10, baseY, maxX, minY }, pureWhite);
        yOff = ls_uiLabelLayout(c, page->spells, { valueBaseX, baseY, maxX, minY });
        baseY -= yOff;
    }
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    
    yOff = ls_uiLabelLayout(c, U"Statistiche", { 10, baseY, maxX, minY }, pureWhite);
    ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
    baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= yOff;
    
    //TODO: Temporary
    valueBaseX = 148;
    
    //TODO: All of them
    ls_uiLabelLayout(c, U"Caratteristiche: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->STR, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"BAB: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->BAB, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"BMC: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->BMC, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"DMC: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->DMC, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    //TODO: All of them
    ls_uiLabelLayout(c, U"Talenti: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->talents[0], { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    if(page->spec_qual.len) {
        ls_uiLabelLayout(c, U"Qualit\U000000E0 Speciali: ", { 10, baseY, maxX, minY }, pureWhite);
        yOff = ls_uiLabelLayout(c, page->spec_qual, { valueBaseX, baseY, maxX, minY });
        baseY -= yOff;
    }
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    
    yOff = ls_uiLabelLayout(c, U"Capacit\U000000E0 Speciali:", { 10, baseY, maxX, minY }, pureWhite);
    ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
    baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= yOff;
    
    baseY -= 8; //NOTE: Random spacing because it feels cramped
    
    for(u32 i = 0; i < 24; i++)
    {
        if(page->specials[i].len == 0) { continue; }
        yOff = ls_uiLabelLayout(c, page->specials[i], { 10, baseY, maxX, minY });
        baseY -= (yOff + 8);
    }
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    
    yOff = ls_uiLabelLayout(c, U"Ecologia", { 10, baseY, maxX, minY }, pureWhite);
    ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
    baseY -= yOff;
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
    baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
    
    ls_uiLabelLayout(c, U"Ambiente: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->environment, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"Organizzazione: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->org, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    ls_uiLabelLayout(c, U"Tesoro: ", { 10, baseY, maxX, minY }, pureWhite);
    yOff = ls_uiLabelLayout(c, page->treasure, { valueBaseX, baseY, maxX, minY });
    baseY -= yOff;
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    
    yOff = ls_uiLabelLayout(c, U"Descrizione", { 10, baseY, maxX, minY }, pureWhite);
    ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
    baseY -= yOff;
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
    baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
    
    yOff = ls_uiLabelLayout(c, page->desc, { 10, baseY, maxX, minY });
    baseY -= yOff;
    
    baseY += (currPixelHeight-1);
    ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
    
    
    //TODO: Make these links openable.
    //      To do it we need to call this function
    //      https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shellexecutea
    //
    //      Leaving the "verb" (second param) null and passing the http link should open the page in the
    //      default browser.
    //
    //      If that doesn't work, windows in both CommandPrompt and Powershell has a "start" command
    //      that opens whatever path you give it with the appropriate command. So an http link should get
    //      opened in the default browser.
    
    baseY -= (currPixelHeight+8);
    ls_uiLabelLayout(c, U"Fonte: ", { 10, baseY, maxX, minY }, pureWhite);
    baseY -= ls_uiLabelLayout(c, page->source, { 72, baseY, maxX, minY });
    
    c->textColor = RGBg(0xCC);
    
    scroll.minY = baseY;
    
    ls_uiEndScrollableRegion(c);
}

void DrawCompendium(UIContext *c)
{
    //NOTE: This arena is cleared every frame,
    //      Because here we store utf32 strings (currently only for the MonsterTable
    //TODO: Maybe we can find a way to cache the MonsterTable and not allocate every frame?
    ls_arenaClear(compTempArena);
    
    Codex *codex = &compendium.codex;
    
    if(compendium.isViewingPage)
    {
        AssertMsg(compendium.pageIndex != -1, "Page Index was not set\n");
        
        if(cachedPage.pageIndex != compendium.pageIndex)
        { CachePage(c, compendium.codex.pages[compendium.pageIndex]); }
        
        DrawPage(c, &cachedPage);
    }
    else
    {
        DrawMonsterTable(c);
    }
    
    return;
}