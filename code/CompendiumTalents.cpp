void CompendiumOpenTalentPage(s32 talentEntry)
{
    s32 indexInTalentArray = talentEntry & TALENT_MODULE_IDX_MASK;
    
    compendium.isViewingTalentPage = TRUE;
    compendium.isViewingNPCTable   = FALSE;
    compendium.isViewingMobTable   = FALSE;
    compendium.isViewingPage       = FALSE;
    compendium.pageIndex           = -1;
    compendium.talentIndex         = indexInTalentArray;
    
    return;
}

void InitCachedTalentEntry(CachedTalentEntry *cachedEntry)
{
    cachedEntry->name   = ls_utf32Alloc(64);
    cachedEntry->desc   = ls_utf32Alloc(1024);
    cachedEntry->pre    = ls_utf32Alloc(4096);
    cachedEntry->gain   = ls_utf32Alloc(8192);
    cachedEntry->norm   = ls_utf32Alloc(4096);
    cachedEntry->spec   = ls_utf32Alloc(4096);
    
    //TODO: Fattura_Rituale Actually contains some extra stuff after the source
    //      which makes it think the source is much longer than it should be!
    //      256 should me WAAAY more than enough... but it isnt.
    cachedEntry->source = ls_utf32Alloc(10240);
}

void CacheTalentEntry(CachedTalentEntry *cachedEntry, s32 talentIndex)
{
    Arena prevArena = ls_arenaUse(compTempArena);
    
    TalentEntry *entry = compendium.codex.talentPages + talentIndex;
    cachedEntry->entryIndex = talentIndex;
    
    GetEntryFromBuffer_t(&compendium.codex.talentsModule, &cachedEntry->name, entry->name, "talent_name");
    GetEntryFromBuffer_t(&compendium.codex.talentsModule, &cachedEntry->desc, entry->desc, "talent_desc");
    GetEntryFromBuffer_t(&compendium.codex.talentsModule, &cachedEntry->pre, entry->pre, "talent_pre");
    GetEntryFromBuffer_t(&compendium.codex.talentsModule, &cachedEntry->gain, entry->gain, "talent_gain");
    GetEntryFromBuffer_t(&compendium.codex.talentsModule, &cachedEntry->norm, entry->norm, "talent_norm");
    GetEntryFromBuffer_t(&compendium.codex.talentsModule, &cachedEntry->spec, entry->spec, "talent_spec");
    GetEntryFromBuffer_t(&compendium.codex.talentsModule, &cachedEntry->source, entry->source, "talent_source");
    
    ls_arenaClear(compTempArena);
    ls_arenaUse(prevArena);
}

void BuildTalentFromPacked_t(Codex *c, u32 entry, utf32 *tmp)
{
    u32 tempUTF32Buffer[512] = {};
    utf32 tempString = { tempUTF32Buffer, 0, 512 };
    
    b32 isMithic   = (entry & TALENT_MITHIC_BIT_U32) != 0;
    b32 isBonus    = (entry & TALENT_BONUS_BIT_U32)  != 0;
    b32 isInterned = (entry & TALENT_INTERN_BIT_U32) != 0;
    
    if(isInterned)
    {
        u16 internedIdx = (u16)((entry & TALENT_INTERN_MASK) >> TALENT_INTERN_IDX_SHIFT);
        GetEntryFromBuffer_t(&c->talents, &tempString, internedIdx, "talents");
    }
    else
    {
        u32 moduleIdx       = entry & TALENT_MODULE_IDX_MASK;
        u32 nameIdxInModule = c->talentPages[moduleIdx].name;
        GetEntryFromBuffer_t(&c->talentsModule, &tempString, nameIdxInModule, "talentsMod");
    }
    
    if(isMithic && isBonus)
    {
        s32 bracketIdx = ls_utf32LeftFind(tempString, '(');
        s32 BMpos = bracketIdx == -1 ? tempString.len : bracketIdx-1;
        
        ls_utf32InsertCStr(&tempString, "[B, M]", BMpos);
    }
    else if(isBonus)
    {
        s32 bracketIdx = ls_utf32LeftFind(tempString, '(');
        s32 Bpos = bracketIdx == -1 ? tempString.len : bracketIdx-1;
        
        ls_utf32InsertCStr(&tempString, "[B]", Bpos);
    }
    else if(isMithic)
    {
        s32 bracketIdx = ls_utf32LeftFind(tempString, '(');
        s32 Mpos = bracketIdx == -1 ? tempString.len : bracketIdx-1;
        
        ls_utf32InsertCStr(&tempString, "[M]", Mpos);
    }
    
    
    //NOTE: Remove the talent tags from the display name.
    const s32 needleArrSize = 7;
    const char32_t *needleArr[needleArrSize] = 
    { 
        U" (Combattimento)", U" (Combattimento, Squadra)", U" (Combattimento, Critico)", 
        U" (Combattimento, Occhiata)", U" (Creazione Oggetto)", U" (Creazione Oggetti)", U" (Metamagia)"
    };
    
    s32 toRemoveIdx = -1;
    for(s32 nextIdx = 0; nextIdx < needleArrSize; nextIdx++)
    {
        toRemoveIdx = ls_utf32LeftFind(tempString, ls_utf32Constant(needleArr[nextIdx]));
        if(toRemoveIdx != -1)
        {
            tempString.len = toRemoveIdx;
            break;
        }
        
    }
    
    ls_utf32Set(tmp, tempString);
    
    return;
}

TalentDisplayResult CheckTalentTooltipAndClick(UIContext *c, CachedPageEntry *page, UILayoutRect oldLayout,
                                               utf32 talent, s32 talentEntry)
{
    b32 isPresent = (talentEntry & TALENT_MODULE_IDX_MASK) != TALENT_NOT_FOUND;
    
    s32 startX = oldLayout.startX;
    s32 startY = oldLayout.startY;
    
    UIRect delta           = ls_uiGlyphStringLayout(c, c->currFont, oldLayout, talent);
    UILayoutRect newLayout = oldLayout;
    newLayout.maxY         = delta.h;
    newLayout.startX       = delta.x;
    newLayout.startY       = delta.y;
    
    Input *UserInput = &c->UserInput;
    
    //NOTE: We went down a line, we need to split the region in 2 rects
    //TODO: We are assuming that it's divided in only 2 rects, and every rect is 1 font height tall
    if(newLayout.startY < startY)
    {
        s32 rectHeight = (newLayout.maxY / 2)-1;
        
        //NOTE: Rect 1
        UIRect rect1 = { startX, startY-3-c->scroll->deltaY, newLayout.maxX-startX, rectHeight };
        
        //NOTE: We stop talent interaction when the archetype window is active
        if(MouseInRect(rect1.x, rect1.y, rect1.w, rect1.h) && !compendium.arch.isChoosingArchetype
           && (rect1.y > c->scroll->y) && (rect1.y < (c->scroll->y + c->scroll->h)))
        {
            if(isPresent)
            {
                if(LeftClick) { 
                    page->talentIndex = talentEntry & TALENT_MODULE_IDX_MASK;
                    ls_uiResetScrollableRegion(c);
                }
                return TSR_AVAILABLE;
            }
            else { return TSR_MISSING; }
        }
        
        //NOTE: Rect 2
        UIRect rect2 = { newLayout.minX, newLayout.startY-3-c->scroll->deltaY, 
            newLayout.startX-newLayout.minX, rectHeight };
        
        if(MouseInRect(rect2.x, rect2.y, rect2.w, rect2.h) && !compendium.arch.isChoosingArchetype
           && (rect2.y > c->scroll->y) && (rect2.y < (c->scroll->y + c->scroll->h)))
        {
            if(isPresent)
            {
                if(LeftClick) { 
                    page->talentIndex = talentEntry & TALENT_MODULE_IDX_MASK;
                    ls_uiResetScrollableRegion(c);
                }
                return TSR_AVAILABLE;
            }
            else { return TSR_MISSING; }
        }
    }
    else
    {
        s32 adjY = startY-3-c->scroll->deltaY;
        if(MouseInRect(startX, adjY, newLayout.startX - startX, newLayout.maxY-1)
           && !compendium.arch.isChoosingArchetype
           && (adjY > c->scroll->y) && (adjY < (c->scroll->y + c->scroll->h)))
        {
            if(isPresent)
            {
                if(LeftClick) {
                    page->talentIndex = talentEntry & TALENT_MODULE_IDX_MASK;
                    ls_uiResetScrollableRegion(c);
                }
                return TSR_AVAILABLE;
            }
            else { return TSR_MISSING; }
        }
    }
    
    return TSR_NORMAL;
}

s32 DrawTalentPage(UIContext *c, CachedTalentEntry *entry, s32 baseX, s32 baseY, s32 width, s32 minY)
{
    Color originalTextColor = c->textColor;
    Color pureWhite         = RGBg(0xFF);
    c->textColor            = RGBg(0xAA);
    
    //NOTE: Currently layout.maxY shouldn't even be used...
    UILayoutRect baseR  = { baseX + 10, baseY, baseX + 10 + width, 0xFEFE, baseX + 10, baseY };
    UILayoutRect alignR = { baseX, baseY, baseX + width, minY, baseX, baseY };
    UILayoutRect offset = {};
    
    auto renderAndAlignWS = [&](const char32_t *s) {
        alignR = ls_uiLabelLayout(c, s, alignR);
    };
    
    auto renderAndAlignW = [&](utf32 s) {
        alignR  = ls_uiLabelLayout(c, s, alignR);
    };
    
    auto renderAndAlignS = [&](const char32_t *s) {
        alignR = baseR;
        alignR = ls_uiLabelLayout(c, s, alignR, pureWhite);
    };
    
    auto renderAndAlign = [&](utf32 s) {
        offset   = ls_uiLabelLayout(c, s, alignR);
        baseR.startY -= offset.maxY;
    };
    
    s32 prevPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    offset = ls_uiLabelLayout(c, entry->name, baseR, pureWhite);
    ls_uiHSeparator(c, baseR.startX, baseR.startY-4, width, 1, RGB(0, 0, 0));
    
    s32 currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
    baseR.startY += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
    baseR.startY -= (offset.maxY + 8);
    alignR = baseR;
    
    if(entry->desc.len)
    {
        renderAndAlign(entry->desc);
    }
    
    if(entry->pre.len)
    {
        renderAndAlignS(U"Prerequisiti: ");
        renderAndAlign(entry->pre);
    }
    
    if(entry->gain.len)
    {
        renderAndAlignS(U"Beneficio: ");
        renderAndAlign(entry->gain);
    }
    
    if(entry->norm.len)
    {
        renderAndAlignS(U"Normale: ");
        renderAndAlign(entry->norm);
    }
    
    if(entry->spec.len)
    {
        renderAndAlignS(U"Speciale: ");
        renderAndAlign(entry->spec);
    }
    
    if(entry->source.len)
    {
        renderAndAlignS(U"Fonte: ");
        renderAndAlign(entry->source);
    }
    
    c->textColor = originalTextColor;
    
    return baseR.startY;
}