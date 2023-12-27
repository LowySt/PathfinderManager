//TODO: To be able to do an actual rendering test, the function needs to become like a co-procedure
//      And yield after the drawing of a single page, to allow the actual rendering to happen.
void testAllCompendiumForAsserts(UIContext *c, b32 logNames = FALSE)
{
    ls_arenaUse(compendiumArena);
    
    CachedPageEntry dummy = {};
    initCachedPage(&dummy);
    
    char buff[64] = {};
    s32 len = 0;
    
    s32 failedACCount = 0;
    for(s32 i = 0; i < compendium.codex.pages.count; i++)
    {
        PageEntry pEntry = compendium.codex.pages[i];
        CachePage(pEntry, i, &dummy, NULL, NULL);
        
        if(dummy.acError == TRUE) { failedACCount += 1; }
        
        len = ls_utf32ToAscii_t(&dummy.name, buff, 64);
        
        if(logNames) { ls_log("{string}", string({buff, len, len})); }
        
#if 0
        pageScroll = { 0, 10, c->width-4, c->height-36, 0, 0, c->width-32, 0 };
        ls_uiStartScrollableRegion(c, &pageScroll);
        pageScroll.minY = DrawPage(c, &dummy, 0, 670, c->width-42, 0);
        if(pageScroll.minY > -19) { pageScroll.minY = -1; }
        ls_uiEndScrollableRegion(c);
#endif
    }
    
    ls_log("Count of Failed AC Fixes: {s32}", failedACCount);
    ls_log("Done With Mobs, beginning NPCs");
    
    failedACCount = 0;
    for(s32 i = 0; i < compendium.codex.npcPages.count; i++)
    {
        NPCPageEntry pEntry = compendium.codex.npcPages[i];
        CachePage(pEntry, i, &dummy, NULL, NULL);
        
        if(dummy.acError == TRUE) { failedACCount += 1; }
        
        len = ls_utf32ToAscii_t(&dummy.name, buff, 64);
        if(logNames) { ls_log("{string}", string({buff, len, len})); }
        
#if 0
        pageScroll = { 0, 10, c->width-4, c->height-36, 0, 0, c->width-32, 0 };
        ls_uiStartScrollableRegion(c, &pageScroll);
        pageScroll.minY = DrawPage(c, &dummy, 0, 670, c->width-42, 0);
        if(pageScroll.minY > -19) { pageScroll.minY = -1; }
        ls_uiEndScrollableRegion(c);
#endif
    }
    
    ls_log("Count of Failed AC Fixes: {s32}", failedACCount);
    ls_log("Done With NPCs!");
    
    CachedTalentEntry dummyTalent = {};
    InitCachedTalentEntry(&dummyTalent);
    
    dummy.talentPage = &dummyTalent;
    
    for(s32 i = 0; i < compendium.codex.talentPages.count; i++)
    {
        TalentEntry tEntry = compendium.codex.talentPages[i];
        CacheTalentEntry(&dummyTalent, i);
        dummy.talentIndex = i;
        
        len = ls_utf32ToAscii_t(&dummyTalent.name, buff, 64);
        if(logNames) { ls_log("{string}", string({buff, len, len})); }
        
#if 0
        pageScroll = { 0, 10, c->width-4, c->height-36, 0, 0, c->width-32, 0 };
        ls_uiStartScrollableRegion(c, &pageScroll);
        pageScroll.minY = DrawPage(c, &dummy, 0, 670, c->width-42, 0);
        if(pageScroll.minY > -19) { pageScroll.minY = -1; }
        ls_uiEndScrollableRegion(c);
#endif
    }
    
    ls_log("Done With Talents!");
    
    //NOTE: By the way, I'm leaking memory, because I'm never deallocating the dummy cached pages.
    //      But these test won't run in deployed software, so... who cares.
    
    ls_arenaUse(compTempArena);
}