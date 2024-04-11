void CompendiumIncreaseGS(u16 gsEntry, s32 gsDiff, s32 rmDiff, utf32 *newGS, utf32 *newPE)
{
    AssertMsg(newGS, "Null gs string pointer\n");
    AssertMsg(newPE, "Null pe string pointer\n");
    AssertMsg(gsDiff < rmIncreaseStride, "Invalid GS Increase\n");
    AssertMsg(rmDiff < rmSetCount, "Invalid RM Increase\n");
    
    u16 gsBase = gsEntry & 0x003F;
    if(gsDiff)
    {
        if(gsDiff < 0 && (gsBase < (gsDiff*-1))) { gsBase = 0; }
        else { gsBase += gsDiff; }
    }
    
    s32 indexInGSSet = gsBase;
    
    u16 rmBase = 999;
    if((gsEntry & GS_RM_VALUE_BIT) != 0)
    {
        rmBase = (gsEntry & 0x0FC0) >> GS_RM_BIT_OFFSET;
        if(rmDiff) { rmBase += rmDiff; }
        
        indexInGSSet += (rmBase * rmIncreaseStride);
    }
    
    AssertMsgF((indexInGSSet >= 0) && (indexInGSSet < gsSetCount),
               "Invalid Index (%d) for GS (%d) RM (%d) when Applying Archetype\n", indexInGSSet, gsBase, rmBase);
    if((indexInGSSet < 0) || (indexInGSSet >= gsSetCount)) { return; }
    
    ls_utf32Append(newGS, gsSet[indexInGSSet]);
    ls_utf32Append(newPE, peSet[gsBase]);
}

void CompendiumSetGS(s32 gsIndex, s32 rm, utf32 *newGS, utf32 *newPE)
{
    AssertMsg(newGS, "Null gs string pointer\n");
    AssertMsg(newPE, "Null pe string pointer\n");
    
    s32 indexInGSSet = gsIndex + (rm * rmIncreaseStride);
    
    AssertMsgF((indexInGSSet >= 0) && (indexInGSSet < gsSetCount),
               "Invalid Index (%d) for GS (%d) RM (%d) when Applying Archetype\n", indexInGSSet, gsIndex, rm);
    if((indexInGSSet < 0) || (indexInGSSet >= gsSetCount)) { return; }
    
    ls_utf32Append(newGS, gsSet[indexInGSSet]);
    ls_utf32Append(newPE, peSet[gsIndex]);
}

utf32 CompendiumGetGSEntryFromSet(u16 gsEntry)
{
    if(gsEntry == GS_SENTINEL_VALUE) { return U""_W; }
    
    u16 gsBase = gsEntry & 0x003F;
    s32 indexInGSSet = gsBase;
    
    u16 rmBase = 999;
    if((gsEntry & GS_RM_VALUE_BIT) != 0)
    {
        rmBase = (gsEntry & 0x0FC0) >> GS_RM_BIT_OFFSET;
        indexInGSSet += (rmBase * rmIncreaseStride);
    }
    
    AssertMsgF((indexInGSSet >= 0) && (indexInGSSet < gsSetCount),
               "Invalid Index (%d) for GS (%d) RM (%d) when Applying Archetype\n", indexInGSSet, gsBase, rmBase);
    if((indexInGSSet < 0) || (indexInGSSet >= gsSetCount)) { return U"NOT_FOUND"_W; }
    
    return gsSet[indexInGSSet];
}

s32 CompendiumGetGSIndexFromSet(u16 gsEntry)
{
    if(gsEntry == GS_SENTINEL_VALUE) { return -1; }
    
    u16 gsBase = gsEntry & 0x003F;
    s32 indexInGSSet = gsBase;
    
    u16 rmBase = 999;
    if((gsEntry & GS_RM_VALUE_BIT) != 0)
    {
        rmBase = (gsEntry & 0x0FC0) >> GS_RM_BIT_OFFSET;
        indexInGSSet += (rmBase * rmIncreaseStride);
    }
    
    AssertMsgF((indexInGSSet >= 0) && (indexInGSSet < gsSetCount),
               "Invalid Index (%d) for GS (%d) RM (%d) when Applying Archetype\n", indexInGSSet, gsBase, rmBase);
    if((indexInGSSet < 0) || (indexInGSSet >= gsSetCount)) { return -1; }
    
    return indexInGSSet;
}
