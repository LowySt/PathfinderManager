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
    utf32 specials[24];
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
    u32 specials[24];
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
Compendium      compendium   = {};
CachedPageEntry cachedPage   = {};
ScrollableRegion pageScroll  = {};
ScrollableRegion tableScroll = {};

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
    
    cachedPage.perception = ls_utf32Alloc(128);
    cachedPage.aura       = ls_utf32Alloc(128);
    
    for(u32 i = 0; i < 16; i++) { cachedPage.immunities[i]  = ls_utf32Alloc(64); }
    for(u32 i = 0; i < 16; i++) { cachedPage.resistances[i] = ls_utf32Alloc(64); }
    for(u32 i = 0; i < 16; i++) { cachedPage.weaknesses[i]  = ls_utf32Alloc(64); }
    
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
    for(u32 i = 0; i < 24; i++) { cachedPage.languages[i] = ls_utf32Alloc(96); }
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
    if(index == 0) { toSet->len = 0; return; } //NOTE: Index zero means no entry
    
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
    
    s32 monsterTableMinY = -((codex->pages.count-30) * 19);
    tableScroll = { 0, 10, c->windowWidth - 4, c->windowHeight - 36, 0, 0, c->windowWidth - 32, monsterTableMinY };
    
    ls_uiSelectFontByFontSize(c, FS_SMALL);
    
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
    
    for(u32 i = 0; i < 24; i++) { GetEntryFromBuffer_t(&c->specials, cachedPage.specials + i, page.specials[i]); }
    
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
    
    GetEntryFromBuffer_t(&c->environment, &cachedPage.environment, page.environment);
    
    pageScroll = { 0, 10, ui->windowWidth - 4, ui->windowHeight - 36, 0, 0, ui->windowWidth - 32, 0};
}

void DrawPage(UIContext *c, CachedPageEntry *page)
{
    AssertMsg(FALSE, "I hate this, try pre-merging the strings");
    
    //NOTE: The first frame is impossible to scroll, because the minY value will be not initialized yet
    //      It's should be fine though. We run at 30FPS on the Compendium, so it should never be felt/seen.
    ls_uiStartScrollableRegion(c, &pageScroll);
    
    //NOTE: Used to adjust the next line position when changing font size, because
    //      a change in pixel height will make the next line too close / too far from the ideal position.
    s32 prevPixelHeight = 0;
    s32 currPixelHeight = 0;
    
    s32 maxW = c->windowWidth - 36;
    s32 minY = 0;
    
    s32 baseY = 670;
    
    Color pureWhite = RGBg(0xFF);
    c->textColor    = RGBg(0xAA);
    
    s32 baseX = 148;
    UIRect alignR = { baseX, baseY, maxW-110, minY };
    UIRect offset = {};
    
    auto renderAndAlign = [&](utf32 s) {
        offset = ls_uiLabelLayout(c, s, alignR);
        alignR.x += offset.w;
        if(alignR.x > alignR.w) { alignR.x = baseX; alignR.y -= offset.h; baseY -= offset.h; }
    };
    
    auto renderAndAlignS = [&](const char32_t *s, Color clr = 0) {
        if(clr == 0) { offset = ls_uiLabelLayout(c, s, alignR); }
        else         { offset = ls_uiLabelLayout(c, s, alignR, clr); }
        alignR.x += offset.w;
        if(alignR.x > alignR.w) { alignR.x = baseX; alignR.y -= offset.h; baseY -= offset.h; }
    };
    
    auto renderAndAlignArr = [&](utf32 *s, u32 size, const char32_t *b, const char32_t *m, const char32_t *e) {
        if(b) { renderAndAlignS(b); }
        for(u32 i = 0; i < size; i++)
        {
            renderAndAlign(s[i]);
            if(s[i+1].len)
            {
                if(m) { renderAndAlignS(m); }
                continue;
            }
            break;
        }
        if(e) { renderAndAlignS(e); }
    };
    
    //---------------//
    //    HEADER     //
    //---------------//
    prevPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    ls_uiLabelLayout(c, page->name, { 10, baseY, maxW, minY }, pureWhite);
    {
        ls_uiLabelLayout(c, U"GS", { 510, baseY, maxW, minY }, pureWhite);
        ls_uiLabelLayout(c, page->gs, { 540, baseY, maxW, minY }, pureWhite);
        ls_uiLabelLayout(c, U"PE", { 635, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->pe, { 670, baseY, maxW, minY }, pureWhite);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        baseY -= offset.h;
        
        if(page->origin.len)
        {
            offset = ls_uiLabelLayout(c, page->origin, { 10, baseY, maxW, minY }, pureWhite);
            baseY -= offset.h;
        }
        
        offset = ls_uiLabelLayout(c, page->shortDesc, { 10, baseY, maxW, minY });
        baseY -= offset.h;
        
        ls_uiLabelLayout(c, U"Allineamento: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->alignment, { baseX, baseY, maxW, minY });
        baseY -= offset.h;
        
        alignR = { baseX, baseY, maxW-110, minY };
        ls_uiLabelLayout(c, U"Categoria: ", { 10, baseY, maxW, minY }, pureWhite);
        renderAndAlign(page->type);
        
        if(page->subtype[0].len)   renderAndAlignArr(page->subtype, 8, U" (", U", ", U")");
        if(page->archetype[0].len) renderAndAlignArr(page->archetype, 8, U" [", U", ", U"]");
        
        if(page->size.len)
        {
            renderAndAlignS(U" ");
            renderAndAlign(page->size);
        }
        baseY -= offset.h;
        
        
        ls_uiLabelLayout(c, U"Iniziativa: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->initiative, { baseX, baseY, maxW, minY });
        baseY -= offset.maxY;
        
        alignR = { baseX, baseY, maxW-110, minY };
        if(page->senses[0].len)
        {
            //TODO: Lich has a strange trailing comma after the last Sense. More Parser Problems?
            offset = ls_uiLabelLayout(c, U"Sensi: ", { 10, baseY, maxW, minY }, pureWhite);
            renderAndAlignArr(page->senses, 8, NULL, U", ", NULL);
            
            //TODO: Shouldn't Perception be outside the senses existance check?
            //      https://golarion.altervista.org/wiki/Cetus
            AssertMsg(FALSE, "Percezione Tellurica fotte il Parser che cerca Percezione in hyperGol");
            renderAndAlignS(U"; Percezione ");
            renderAndAlign(page->perception);
            
            baseY -= offset.h;
        }
        
        if(page->aura.len)
        {
            ls_uiLabelLayout(c, U"Aura: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->aura, { baseX, baseY, maxW, minY });
            baseY -= offset.h;
        }
    }
    
    
    //---------------//
    //    DEFENSE    //
    //---------------//
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    baseX  = 164;
    {
        offset = ls_uiLabelLayout(c, U"Difesa", { 10, baseY, maxW, minY }, pureWhite);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        baseY -= offset.h;
        
        ls_uiLabelLayout(c, U"CA: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->AC, { baseX, baseY, maxW, minY });
        baseY -= offset.h;
        
        ls_uiLabelLayout(c, U"PF: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->HP, { baseX, baseY, maxW, minY });
        baseY -= offset.h;
        
        ls_uiLabelLayout(c, U"Tiri Salvezza: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->ST, { baseX, baseY, maxW, minY });
        baseY -= offset.h;
        
        if(page->RD.len)
        {
            ls_uiLabelLayout(c, U"RD: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->RD, { baseX, baseY, maxW, minY });
            baseY -= offset.h;
        }
        
        if(page->RI.len)
        {
            ls_uiLabelLayout(c, U"RI: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->RI, { baseX, baseY, maxW, minY });
            baseY -= offset.h;
        }
        
        alignR = { baseX, baseY, maxW-152, minY };
        if(page->immunities[0].len)
        {
            ls_uiLabelLayout(c, U"Immunit\U000000E0: ", { 10, baseY, maxW, minY }, pureWhite);
            renderAndAlignArr(page->immunities, 16, NULL, U", ", NULL);
            baseY -= offset.h;
        }
        
        alignR = { baseX, baseY, maxW-152, minY };
        if(page->resistances[0].len)
        {
            ls_uiLabelLayout(c, U"Resistenze: ", { 10, baseY, maxW, minY }, pureWhite);
            renderAndAlignArr(page->resistances, 16, NULL, U", ", NULL);
            baseY -= offset.h;
        }
        
        if(page->defensiveCapacity.len)
        {
            ls_uiLabelLayout(c, U"Capacit\U000000E0 Difensive: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->defensiveCapacity, { baseX, baseY, maxW, minY });
            baseY -= offset.h;
        }
        
        alignR = { baseX, baseY, maxW-152, minY };
        if(page->weaknesses[0].len)
        {
            ls_uiLabelLayout(c, U"Debolezze: ", { 10, baseY, maxW, minY }, pureWhite);
            renderAndAlignArr(page->weaknesses, 16, NULL, U", ", NULL);
            baseY -= offset.h;
        }
    }
    
    //---------------//
    //    ATTACK     //
    //---------------//
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    baseX  = 200;
    {
        offset = ls_uiLabelLayout(c, U"Attacco", { 10, baseY, maxW, minY }, pureWhite);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        baseY -= offset.h;
        
        alignR = { baseX, baseY, maxW-192, minY };
        ls_uiLabelLayout(c, U"Velocit\U000000E0: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->speed, alignR);
        baseY -= offset.h;
        
        alignR = { baseX, baseY, maxW-192, minY };
        ls_uiLabelLayout(c, U"Mischia: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->melee, alignR);
        baseY -= offset.h;
        
        if(page->ranged.len)
        {
            alignR = { baseX, baseY, maxW-192, minY };
            ls_uiLabelLayout(c, U"Distanza: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->ranged, alignR);
            baseY -= offset.maxY;
        }
        
        if(page->specialAttacks.len)
        {
            alignR = { baseX, baseY, maxW-192, minY };
            ls_uiLabelLayout(c, U"Attacchi Speciali: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->specialAttacks, alignR);
            baseY -= offset.maxY;
        }
        
        if(page->space.len)
        {
            alignR = { baseX, baseY, maxW-192, minY };
            ls_uiLabelLayout(c, U"Spazio: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->space, alignR);
            baseY -= offset.maxY;
        }
        
        if(page->reach.len)
        {
            alignR = { baseX, baseY, maxW-192, minY };
            ls_uiLabelLayout(c, U"Portata: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->reach, alignR);
            baseY -= offset.maxY;
        }
        
        if(page->psych.len)
        {
            alignR = { baseX, baseY, maxW-192, minY };
            ls_uiLabelLayout(c, U"Magia Psichica: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->psych, alignR);
            baseY -= offset.maxY;
        }
        
        if(page->magics.len)
        {
            alignR = { baseX, baseY, maxW-192, minY };
            ls_uiLabelLayout(c, U"Capacit\U000000E0 Magiche: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->magics, alignR);
            baseY -= offset.maxY;
        }
        
        if(page->spells.len)
        {
            alignR = { baseX, baseY, maxW-192, minY };
            ls_uiLabelLayout(c, U"Incantesimi Conosciuti: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->spells, alignR);
            baseY -= offset.maxY;
        }
    }
    
    //---------------//
    //     STATS     //
    //---------------//
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    baseX  = 148;
    {
        offset = ls_uiLabelLayout(c, U"Statistiche", { 10, baseY, maxW, minY }, pureWhite);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        baseY -= offset.h;
        
        alignR = { baseX, baseY, maxW-40, minY };
        ls_uiLabelLayout(c, U"Caratteristiche: ", { 10, baseY, maxW, minY }, pureWhite);
        
        renderAndAlignS(U"FOR ");
        renderAndAlign(page->STR);
        renderAndAlignS(U", DEX ");
        renderAndAlign(page->DEX);
        renderAndAlignS(U", COS ");
        renderAndAlign(page->CON);
        renderAndAlignS(U", INT ");
        renderAndAlign(page->INT);
        renderAndAlignS(U", SAG ");
        renderAndAlign(page->WIS);
        renderAndAlignS(U", CAR ");
        renderAndAlign(page->CHA);
        baseY -= offset.h;
        
        ls_uiLabelLayout(c, U"BAB: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->BAB, { baseX, baseY, maxW-40, minY });
        baseY -= offset.h;
        
        ls_uiLabelLayout(c, U"BMC: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->BMC, { baseX, baseY, maxW-40, minY });
        baseY -= offset.h;
        
        ls_uiLabelLayout(c, U"DMC: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->DMC, { baseX, baseY, maxW-40, minY });
        baseY -= offset.h;
        
        alignR = { baseX, baseY, maxW-128, minY };
        if(page->talents[0].len)
        {
            ls_uiLabelLayout(c, U"Talenti: ", { 10, baseY, maxW, minY }, pureWhite);
            renderAndAlignArr(page->talents, 24, NULL, U", ", NULL);
            baseY -= offset.h;
        }
        
        alignR = { baseX, baseY, maxW-128, minY };
        if(page->skills[0].len)
        {
            ls_uiLabelLayout(c, U"Abilit\U000000E0: ", { 10, baseY, maxW, minY }, pureWhite);
            renderAndAlignArr(page->skills, 24, NULL, U", ", NULL);
            baseY -= offset.h;
        }
        
        alignR = { baseX, baseY, maxW-128, minY };
        if(page->languages[0].len)
        {
            ls_uiLabelLayout(c, U"Linguaggi: ", { 10, baseY, maxW, minY }, pureWhite);
            renderAndAlignArr(page->languages, 24, NULL, U", ", NULL);
            baseY -= offset.h;
        }
        
        if(page->racialMods.len)
        {
            ls_uiLabelLayout(c, U"Modificatori Razziali: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->racialMods, { baseX, baseY, maxW, minY });
            baseY -= offset.h;
        }
        
        if(page->spec_qual.len)
        {
            ls_uiLabelLayout(c, U"Qualit\U000000E0 Speciali: ", { 10, baseY, maxW, minY }, pureWhite);
            offset = ls_uiLabelLayout(c, page->spec_qual, { baseX, baseY, maxW, minY });
            baseY -= offset.h;
        }
        
    }
    
    //---------------//
    //   SPECIALS    //
    //---------------//
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    {
        offset = ls_uiLabelLayout(c, U"Capacit\U000000E0 Speciali:", { 10, baseY, maxW, minY }, pureWhite);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        baseY -= offset.maxY;
        
        baseY -= 8; //NOTE: Random spacing because it feels cramped
        
        for(u32 i = 0; i < 24; i++)
        {
            if(page->specials[i].len == 0) { continue; }
            offset = ls_uiLabelLayout(c, page->specials[i], { 10, baseY, maxW, minY });
            baseY -= (offset.maxY + 8);
        }
    }
    
    //---------------//
    //      ORG      //
    //---------------//
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    baseX  = 148;
    {
        offset = ls_uiLabelLayout(c, U"Ecologia", { 10, baseY, maxW, minY }, pureWhite);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        baseY -= offset.maxY;
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        
        ls_uiLabelLayout(c, U"Ambiente: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->environment, { baseX, baseY, maxW, minY });
        baseY -= offset.maxY;
        
        ls_uiLabelLayout(c, U"Organizzazione: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->org, { baseX, baseY, maxW, minY });
        baseY -= offset.maxY;
        
        ls_uiLabelLayout(c, U"Tesoro: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->treasure, { baseX, baseY, maxW, minY });
        baseY -= offset.maxY;
    }
    
    //---------------//
    //     FINAL     //
    //---------------//
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseY -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseY -= 4;
    {
        offset = ls_uiLabelLayout(c, U"Descrizione", { 10, baseY, maxW, minY }, pureWhite);
        ls_uiHSeparator(c, baseY-4, 10, 1, RGB(0, 0, 0));
        baseY -= offset.maxY;
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        
        offset = ls_uiLabelLayout(c, page->desc, { 10, baseY, maxW, minY });
        baseY -= offset.maxY;
        
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
        ls_uiLabelLayout(c, U"Fonte: ", { 10, baseY, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->source, { 72, baseY, maxW, minY });
        baseY -= offset.maxY;
    }
    
    c->textColor = RGBg(0xCC);
    
    pageScroll.minY = baseY;
    
    ls_uiEndScrollableRegion(c);
}

void DrawMonsterTable(UIContext *c)
{
    Input *UserInput = &c->UserInput;
    Codex *codex     = &compendium.codex;
    
    Color bkgColor = RGBg(0x40);
    
    s32 baseX = 20;
    s32 baseY = 630;
    
    ls_uiStartScrollableRegion(c, &tableScroll);
    
    s32 startI = -(tableScroll.deltaY / 19);
    s32 endI   = startI+36 < codex->pages.count ? startI+36 : codex->pages.count;
    
    for(s32 i = startI; i < endI; i++)
    {
        PageEntry entry = codex->pages[i];
        Color hoverColor = bkgColor;
        
        if(LeftClickIn(baseX-4, baseY-4, 300, 18)) //TODONOTE: I don't like it...
        {
            compendium.isViewingPage = TRUE; 
            compendium.pageIndex     = i;
        }
        
        if(MouseInRect(baseX-4, baseY-4, 300, 18)) { hoverColor = RGBg(0x66); }
        
        ls_uiRect(c, baseX-4, baseY+tableScroll.deltaY-4, 300, 20, hoverColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->names, entry.name), baseX, baseY+tableScroll.deltaY, 1);
        baseX += 299;
        
        ls_uiRect(c, baseX-4, baseY+tableScroll.deltaY-4, 80, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->gs, entry.gs), baseX, baseY+tableScroll.deltaY, 1);
        baseX += 79;
        
        ls_uiRect(c, baseX-4, baseY+tableScroll.deltaY-4, 180, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->types, entry.type), baseX, baseY+tableScroll.deltaY, 1);
        baseX += 179;
        
        //TODO: Show all subtypes. Right now we only grab one.
        //      We can do this by overloading the function GetEntryFromBuffer and passing the array itself.
        ls_uiRect(c, baseX-4, baseY+tableScroll.deltaY-4, 188, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->subtypes, entry.subtype[0]), baseX, baseY+tableScroll.deltaY, 1);
        baseX += 187;
        
        baseY -= 19;
        baseX = 20;
    }
    
    ls_uiEndScrollableRegion(c);
    
    return;
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