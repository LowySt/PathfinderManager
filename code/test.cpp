#if 0
//NOTE: This is used to generate all archetypes combinations once. Run it in main and copy the output into a file.
void testGenerateAllArchetypesCombinations()
{
    s32 lineCounter = 0;
    //NOTETODO: This is technically non generalized, because I need a MAX_CONCURRENT_ARCHETYPES long
    //          combination. In this version, MAX_CONCURRENT_ARCHETYPES == 4, so having 4 nested loops works
    //          (and it's better compile-time, since recursive functions at compile time are annoying)
    for(s32 i = 0; i < MAX_ARCHETYPES; i++)
    {
        for(s32 j = 0; j < MAX_ARCHETYPES; j++)
        {
            for(s32 k = 0; k < MAX_ARCHETYPES; k++)
            {
                for(s32 w = 0; w < MAX_ARCHETYPES; w++)
                {
                    ls_logs("{s32}, {s32}, {s32}, {s32}, ", i, j, k, w);
                    lineCounter += 1;
                    
                    if((lineCounter % 6) == 0)
                    {
                        ls_log("");
                    }
                }
            }
        }
    }
}


#endif

//TODO: To be able to do an actual rendering test, the function needs to become like a co-procedure
//      And yield after the drawing of a single page, to allow the actual rendering to happen.
void testAllCompendiumForAsserts(UIContext *c, b32 logNames = FALSE)
{
    ls_arenaUse(compendiumArena);
    
    CachedPageEntry dummy = {};
    initCachedPage(&dummy);
    
    StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> dummyArch = {};
    
    char buff[64] = {};
    s32 len = 0;
    
    s32 failedACCount = 0;
    for(s32 i = 0; i < compendium.codex.pages.count; i++)
    {
        //NOTE: Test Caching the vanilla page
        PageEntry pEntry = compendium.codex.pages[i];
        CachePage(pEntry, i, &dummy, {}, NULL);
        
        if(dummy.acError == TRUE) { failedACCount += 1; }
        
        len = ls_utf32ToAscii_t(dummy.name, buff, 64);
        
        if(logNames) { ls_log("{string}", string({buff, len, len})); }
        
#if 0
        //NOTE: Test Caching the page with all possible combinations of archetypes applied
        s32 logCount = 0;
        const s32 sizeOfCombinations = sizeof(testCombinations) / sizeof(testCombinations[0]);
        for(s32 j = 0; j < sizeOfCombinations; j += 4)
        {
            dummyArch.count = 0;
            
            dummyArch[0] = testCombinations[j];
            dummyArch.count = 1;
            
            s32 j1 = testCombinations[j+1];
            if(allArchetypeDiffs[j1].isCompatible(&dummy)) { dummyArch[1] = j1; dummyArch.count += 1; }
            
            s32 j2 = testCombinations[j+2];
            if(allArchetypeDiffs[j2].isCompatible(&dummy)) { dummyArch[2] = j2; dummyArch.count += 1; }
            
            s32 j3 = testCombinations[j+3];
            if(allArchetypeDiffs[j3].isCompatible(&dummy)) { dummyArch[3] = j3; dummyArch.count += 1; }
            
            CachePage(pEntry, i, &dummy, dummyArch, NULL);
            
            logCount += 1;
            
            if((logCount % 1200) == 0)
            { ls_log("Done {s32} Archetype Combinations of {string}", logCount, string({buff, len, len})); }
        }
#endif
        
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
        CachePage(pEntry, i, &dummy, {}, NULL);
        
        if(dummy.acError == TRUE) { failedACCount += 1; }
        
        len = ls_utf32ToAscii_t(dummy.name, buff, 64);
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
        
        len = ls_utf32ToAscii_t(dummyTalent.name, buff, 64);
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