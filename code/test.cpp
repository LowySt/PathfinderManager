void testAllCompendiumForAsserts(b32 logNames = FALSE)
{
    ls_arenaUse(compendiumArena);
    
    CachedPageEntry dummy = {};
    initCachedPage(&dummy);
    
    b32 hasMessaged1 = FALSE;
    b32 hasMessaged2 = FALSE;
    b32 hasMessaged3 = FALSE;
    b32 hasMessaged4 = FALSE;
    b32 hasMessaged5 = FALSE;
    
    char buff[64] = {};
    s32 len = 0;
    
    s32 failedACCount = 0;
    for(s32 i = 0; i < compendium.codex.pages.count; i++)
    {
        PageEntry pEntry = compendium.codex.pages[i];
        CachePage(pEntry, i, &dummy, NULL);
        
        if(dummy.acError == TRUE) { failedACCount += 1; }
        
        len = ls_utf32ToAscii_t(&dummy.name, buff, 64);
        
        if(logNames) { ls_log("{string}", string({buff, len, len})); }
        
        f32 fract = (f32)i / (f32)compendium.codex.pages.count;
        if((fract > 0.1) && (hasMessaged1 == FALSE))  { ls_log("10% of Mobs Done"); hasMessaged1 = TRUE; }
        if((fract > 0.25) && (hasMessaged2 == FALSE)) { ls_log("25% of Mobs Done"); hasMessaged2 = TRUE; }
        if((fract > 0.33) && (hasMessaged3 == FALSE)) { ls_log("33% of Mobs Done"); hasMessaged3 = TRUE; }
        if((fract > 0.50) && (hasMessaged4 == FALSE)) { ls_log("50% of Mobs Done"); hasMessaged4 = TRUE; }
        if((fract > 0.75) && (hasMessaged5 == FALSE)) { ls_log("75% of Mobs Done"); hasMessaged5 = TRUE; }
    }
    
    ls_log("Count of Failed AC Fixes: {s32}", failedACCount);
    ls_log("Done With Mobs, beginning NPCs");
    
    hasMessaged1 = FALSE;
    hasMessaged2 = FALSE;
    hasMessaged3 = FALSE;
    hasMessaged4 = FALSE;
    hasMessaged5 = FALSE;
    
    failedACCount = 0;
    for(s32 i = 0; i < compendium.codex.npcPages.count; i++)
    {
        NPCPageEntry pEntry = compendium.codex.npcPages[i];
        CachePage(pEntry, i, &dummy, NULL);
        
        if(dummy.acError == TRUE) { failedACCount += 1; }
        
        len = ls_utf32ToAscii_t(&dummy.name, buff, 64);
        if(logNames) { ls_log("{string}", string({buff, len, len})); }
        
        f32 fract = (f32)i / (f32)compendium.codex.npcPages.count;
        if((fract > 0.1) && (hasMessaged1 == FALSE))  { ls_log("10% of NPCs Done"); hasMessaged1 = TRUE; }
        if((fract > 0.25) && (hasMessaged2 == FALSE)) { ls_log("25% of NPCs Done"); hasMessaged2 = TRUE; }
        if((fract > 0.33) && (hasMessaged3 == FALSE)) { ls_log("33% of NPCs Done"); hasMessaged3 = TRUE; }
        if((fract > 0.50) && (hasMessaged4 == FALSE)) { ls_log("50% of NPCs Done"); hasMessaged4 = TRUE; }
        if((fract > 0.75) && (hasMessaged5 == FALSE)) { ls_log("75% of NPCs Done"); hasMessaged5 = TRUE; }
    }
    
    ls_log("Count of Failed AC Fixes: {s32}", failedACCount);
    ls_log("Done With NPCs!");
    
    CachedTalentEntry dummyTalent = {};
    InitCachedTalentEntry(&dummyTalent);
    
    for(s32 i = 0; i < compendium.codex.talentPages.count; i++)
    {
        TalentEntry tEntry = compendium.codex.talentPages[i];
        CacheTalentEntry(&dummyTalent, i);
        
        len = ls_utf32ToAscii_t(&dummyTalent.name, buff, 64);
        if(logNames) { ls_log("{string}", string({buff, len, len})); }
    }
    
    ls_log("Done With Talents!");
    
    //NOTE: By the way, I'm leaking memory, because I'm never deallocating the dummy cached pages.
    //      But these test won't run in deployed software, so... who cares.
    
    ls_arenaUse(compTempArena);
}