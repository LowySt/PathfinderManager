void CompendiumIncreaseGS(utf32 oldGS, s32 gsDiff, s32 rmDiff, utf32 *newGS, utf32 *newPE)
{
    AssertMsg(((gsDiff >= 0) && (gsDiff < rmIncreaseStride)), "Invalid GS Increase\n");
    AssertMsg(((rmDiff >= 0) && (rmDiff < rmSetCount)), "Invalid RM Increase\n");
    
    s32 index = ls_utf32FirstEqual(oldGS, (utf32 *)gsSet, gsSetCount);
    
    AssertMsg(((index != -1) || (index < gsSetCount)), "Invalid GS when applying Advanced archetype\n");
    if((index == -1) || (index >= gsSetCount)) { return; }
    
    //TODO: Bound checking gs increase going over GS 31
    if(gsDiff) { index += gsDiff; }
    
    //TODO: Bound checking rm increase going over RM 10
    if(rmDiff) { index += rmIncreaseStride; }
    
    AssertMsgF(index < gsSetCount, "While increasing GS went over the maximum valid index: %d\n", index);
    
    ls_utf32Set(newGS, gsSet[index]);
    ls_utf32Set(newPE, peSet[index%peSetCount]);
}

//-------------------------//
//   STUBS FOR ARCHETYPE   //
//-------------------------//

void archetypeGSStub(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{ *gsDiff = 0; *rmDiff = 0; }

void archetypeASStub(s32 as[AS_COUNT])
{ return; }

void archetypeACStub(s32 ac[AC_TYPES_COUNT])
{ return; }

void archetypeSensesStub(utf32 *old)
{ return; }

//-------------------------//
//   ADVANCED ARCHETYPE    //
//-------------------------//

void advancedCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{
    *gsDiff = 1;
    *rmDiff = 0;
}

void advancedCreatureAS(s32 as[AS_COUNT])
{
    as[AS_STR] += 4;
    as[AS_DEX] += 4;
    as[AS_CON] += 4;
    if(as[AS_INT] > 2) { as[AS_INT] += 4; }
    as[AS_WIS] += 4;
    as[AS_CHA] += 4;
}

void advancedCreatureAC(s32 ac[AC_TYPES_COUNT])
{
    ac[AC_NATURAL] += 2;
}

ArchetypeDiff AdvancedCreature = {
    U"Avanzata"_W, advancedCreatureGS, advancedCreatureAS, advancedCreatureAC, archetypeSensesStub
};

//-------------------------//
//   CELESTIAL ARCHETYPE   //
//-------------------------//

void celestialCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{
    if(hitDice >= 5) { *gsDiff = 1; *rmDiff = 0; return; }
    
    *gsDiff = 0;
    *rmDiff = 0;
}

//TODO: Static utf32 strings AAAAAH
void celestialCreatureSenses(utf32 *old)
{
    AssertMsg(old, "Null utf32 pointer\n");
    
    if(ls_utf32LeftFind(*old, U"Scurovisione 18 m"_W) == -1)
    {
        AssertMsg(old->len + 19 < old->size, "Insufficient space in senses string.\n");
        ls_utf32Prepend(old, U"Scurovisione 18 m, "_W);
    }
}

ArchetypeDiff CelestialCreature = {
    U"Celestiale"_W, celestialCreatureGS, archetypeASStub, archetypeACStub, celestialCreatureSenses
};

ArchetypeDiff allArchetypeDiffs[MAX_ARCHETYPES] = {
    AdvancedCreature, CelestialCreature
};


b32 CompendiumOpenArchetypeWindow(UIContext *c, void *user)
{
    compendium.arch.isChoosingArchetype = !compendium.arch.isChoosingArchetype;
    return FALSE;
}

b32 CompendiumSelectArchetype(UIContext *c, void *user)
{
    s32 index = (s32)((s64)user);
    ls_staticArrayAppendUnique(&compendium.appliedArchetypes, allArchetypeDiffs[index]);
    GetPageEntryAndCache(compendium.pageIndex, -1, &cachedPage, NULL);
    compendium.arch.isChoosingArchetype = FALSE;
    return FALSE;
}

void CompendiumApplyAllArchetypeNames(utf32 *name)
{
    AssertMsg(name, "Null utf32 pointer\n");
    
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        
        AssertMsg(name->len + curr->nameStr.len+1 < name->size, "Can't fit archetype name\n");
        if(name->len + curr->nameStr.len+1 > name->size) { return; }
        
        ls_utf32Append(name, U" ");
        ls_utf32Append(name, curr->nameStr);
    }
}

void CompendiumApplyAllArchetypeGS(utf32 oldGS, s32 hitDice, utf32 *newGS, utf32 *newPE)
{
    s32 totalGSDiff = 0;
    s32 totalRMDiff = 0;
    
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        
        s32 gsDiff = 0;
        s32 rmDiff = 0;
        curr->gs(hitDice, &gsDiff, &rmDiff);
        
        totalGSDiff += gsDiff;
        totalRMDiff += rmDiff;
    }
    
    CompendiumIncreaseGS(oldGS, totalGSDiff, totalRMDiff, newGS, newPE);
}

void CompendiumApplyAllArchetypeAS(s32 as[AS_COUNT])
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->abilityScores(as);
    }
}

//TODO: Static UTF32 Strings REALLY Needed
void CompendiumAddAllArchetypesToList(utf32 *archetypeList)
{
    u32 tempBuff[128] = {};
    utf32 tempString = { tempBuff, 0, 128 };
    
    if(archetypeList->len)
    {
        ls_utf32Copy_t(*archetypeList, &tempString);
        ls_utf32Clear(archetypeList);
        
        for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
        {
            ArchetypeDiff *curr = compendium.appliedArchetypes + i;
            ls_utf32InsertSubstr(&tempString, U", ", tempString.len-2);
            ls_utf32InsertSubstr(&tempString, curr->nameStr, tempString.len-2);
        }
        
        ls_utf32Copy_t(tempString, archetypeList);
    }
    else
    {
        ls_utf32Append(archetypeList, ls_utf32Constant(U"["));
        for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
        {
            ArchetypeDiff *curr = compendium.appliedArchetypes + i;
            ls_utf32Append(archetypeList, curr->nameStr);
            if(i < compendium.appliedArchetypes.count - 1) { ls_utf32Append(archetypeList, U", "); }
        }
        
        ls_utf32Append(archetypeList, ls_utf32Constant(U"] "));
    }
}

void CompendiumApplyAllArchetypeSenses(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->senses(old);
    }
}