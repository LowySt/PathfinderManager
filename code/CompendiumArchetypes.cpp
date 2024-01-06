#if 0
//NOTE: Stubbed Archetype
ArchetypeDiff stub = {
    U"Name"_W, archetypeGSStub, archetypeASStub, archetypeACStub, archetypeSensesStub,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, archetypeSpecAtkStub,
    archetypeSizeStub, archetypeMeleeStub
};
#endif

void CompendiumIncreaseGS(u16 gsEntry, s32 gsDiff, s32 rmDiff, utf32 *newGS, utf32 *newPE)
{
    AssertMsg(((gsDiff >= 0) && (gsDiff < rmIncreaseStride)), "Invalid GS Increase\n");
    AssertMsg(((rmDiff >= 0) && (rmDiff < rmSetCount)), "Invalid RM Increase\n");
    
    u16 gsBase = gsEntry & 0x003F;
    if(gsDiff) { gsBase += gsDiff; }
    
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

void archetypeRDStub(s32 hitDice, utf32 *old)
{ return; }

void archetypeResistanceStub(s32 hitDice, utf32 *old)
{ return; }

void archetypeRIStub(utf32 gs, utf32 *ri)
{ return; }

void archetypeSpecAtkStub(utf32 *spec)
{ return; }

void archetypeSizeStub(utf32 *size)
{ return; }

void archetypeMeleeStub(utf32 *melee)
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

void advancedCreatureAC(s32 acDiff[AC_TYPES_COUNT])
{
    acDiff[AC_NATURAL] += 2;
}

ArchetypeDiff AdvancedCreature = {
    U"Avanzata"_W, advancedCreatureGS, advancedCreatureAS, advancedCreatureAC, archetypeSensesStub,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, archetypeSpecAtkStub,
    archetypeSizeStub, archetypeMeleeStub
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
    
    const utf32 toAdd = U"Scurovisione 18 m"_W;
    if(ls_utf32LeftFind(*old, toAdd) == -1)
    {
        AssertMsg(old->len + toAdd.len+2 < old->size, "Insufficient space in senses string.\n");
        if(old->len + toAdd.len+2 >= old->size) { return; }
        ls_utf32Prepend(old, U"Scurovisione 18 m, "_W);
    }
}

//TODO: This actually sucks. The way it's setup now it would need parsing to properly find old rd,
//      and replace them with better ones if necessary. Need to update hyperGol
void celestialCreatureRD(s32 hitDice, utf32 *oldRD)
{
    AssertMsg(oldRD, "Null utf32 pointer\n");
    
    if(hitDice < 5) { return; }
    else if(hitDice < 11)
    {
        const utf32 toAdd = U"5/male"_W;
        if(ls_utf32LeftFind(*oldRD, toAdd) == -1)
        {
            AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
            if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
            ls_utf32Prepend(oldRD, U"5/male, "_W);
        }
    }
    else
    {
        const utf32 toAdd = U"10/male"_W;
        if(ls_utf32LeftFind(*oldRD, toAdd) == -1)
        {
            AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
            if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
            ls_utf32Prepend(oldRD, U"10/male, "_W);
        }
    }
}

//TODO: This actually sucks. The way it's setup now it would need parsing to properly find old resistances,
//      and replace them with better ones if necessary. Need to update hyperGol
void celestialCreatureResistance(s32 hitDice, utf32 *oldRes)
{
    AssertMsg(oldRes, "Null utf32 pointer\n");
    
    if(hitDice < 5) {
        const utf32 toAdd = U"Acido 5, Elettricit\U000000E0 5, Freddo 5"_W;
        if(ls_utf32LeftFind(*oldRes, toAdd) == -1)
        {
            AssertMsg(oldRes->len + toAdd.len+2 < oldRes->size, "Insufficient space in resistances string.\n");
            if(oldRes->len + toAdd.len+2 >= oldRes->size) { return; }
            ls_utf32Prepend(oldRes, U"Acido 5, Elettricit\U000000E0 5, Freddo 5, "_W);
        }
        
        return;
    }
    else if(hitDice < 11)
    {
        const utf32 toAdd = U"Acido 10, Elettricit\U000000E0 10, Freddo 10"_W;
        if(ls_utf32LeftFind(*oldRes, toAdd) == -1)
        {
            AssertMsg(oldRes->len + toAdd.len+2 < oldRes->size, "Insufficient space in resistances string.\n");
            if(oldRes->len + toAdd.len+2 >= oldRes->size) { return; }
            ls_utf32Prepend(oldRes, U"Acido 10, Elettricit\U000000E0 10, Freddo 10, "_W);
        }
    }
    else
    {
        const utf32 toAdd = U"Acido 15, Elettricit\U000000E0 15, Freddo 15"_W;
        if(ls_utf32LeftFind(*oldRes, toAdd) == -1)
        {
            AssertMsg(oldRes->len + toAdd.len+2 < oldRes->size, "Insufficient space in resistances string.\n");
            if(oldRes->len + toAdd.len+2 >= oldRes->size) { return; }
            ls_utf32Prepend(oldRes, U"Acido 15, Elettricit\U000000E0 15, Freddo 15, "_W);
        }
    }
}

void celestialCreatureRI(utf32 gs, utf32 *ri)
{
    AssertMsg(ri, "Null utf32 pointer\n");
    
    s32 index = ls_utf32FirstEqual(gs, (utf32 *)gsSet, gsSetCount);
    
    AssertMsg(((index != -1) || (index < gsSetCount)), "Invalid GS when applying Advanced archetype\n");
    if((index == -1) || (index >= gsSetCount)) { return; }
    
    s64 oldRI = ls_utf32ToInt(*ri);
    
    s32 gsVal = (index - 4);
    if(gsVal <= 0) {
        const utf32 toAdd = U"5"_W;
        if(oldRI < 5) {
            ls_utf32Clear(ri);
            ls_utf32Append(ri, toAdd);
        }
        else { return; }
    }
    else
    {
        gsVal %= 36;
        if(oldRI < (gsVal + 5))
        {
            ls_utf32Clear(ri);
            ls_utf32AppendInt(ri, gsVal+5);
        }
        else { return; }
    }
}

void celestialCreatureSpecAtk(utf32 *spec)
{
    AssertMsg(spec, "Null utf32 pointer\n");
    
    const utf32 toAdd = U"Punire il Male 1/giorno, "_W;
    AssertMsg(spec->len + toAdd.len < spec->size, "Insufficient space in special attack string.\n");
    if(spec->len + toAdd.len >= spec->size) { return; }
    
    ls_utf32Prepend(spec, toAdd);
}

ArchetypeDiff CelestialCreature = {
    U"Celestiale"_W, celestialCreatureGS, archetypeASStub, archetypeACStub, celestialCreatureSenses,
    celestialCreatureRD, celestialCreatureResistance, celestialCreatureRI, celestialCreatureSpecAtk,
    archetypeSizeStub, archetypeMeleeStub
};

//-------------------------//
//   FIENDISH ARCHETYPE    //
//-------------------------//

//TODO: This actually sucks. The way it's setup now it would need parsing to properly find old rd,
//      and replace them with better ones if necessary. Need to update hyperGol
void fiendishCreatureRD(s32 hitDice, utf32 *oldRD)
{
    AssertMsg(oldRD, "Null utf32 pointer\n");
    
    if(hitDice < 5) { return; }
    else if(hitDice < 11)
    {
        const utf32 toAdd = U"5/bene"_W;
        if(ls_utf32LeftFind(*oldRD, toAdd) == -1)
        {
            AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
            if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
            ls_utf32Prepend(oldRD, U"5/bene, "_W);
        }
    }
    else
    {
        const utf32 toAdd = U"10/bene"_W;
        if(ls_utf32LeftFind(*oldRD, toAdd) == -1)
        {
            AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
            if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
            ls_utf32Prepend(oldRD, U"10/bene, "_W);
        }
    }
}

//TODO: This actually sucks. The way it's setup now it would need parsing to properly find old resistances,
//      and replace them with better ones if necessary. Need to update hyperGol
void fiendishCreatureResistance(s32 hitDice, utf32 *oldRes)
{
    AssertMsg(oldRes, "Null utf32 pointer\n");
    
    if(hitDice < 5) {
        const utf32 toAdd = U"Freddo 5, Fuoco 5"_W;
        if(ls_utf32LeftFind(*oldRes, toAdd) == -1)
        {
            AssertMsg(oldRes->len + toAdd.len+2 < oldRes->size, "Insufficient space in resistances string.\n");
            if(oldRes->len + toAdd.len+2 >= oldRes->size) { return; }
            ls_utf32Prepend(oldRes, U"Freddo 5, Fuoco 5, "_W);
        }
        
        return;
    }
    else if(hitDice < 11)
    {
        const utf32 toAdd = U"Freddo 10, Fuoco 10"_W;
        if(ls_utf32LeftFind(*oldRes, toAdd) == -1)
        {
            AssertMsg(oldRes->len + toAdd.len+2 < oldRes->size, "Insufficient space in resistances string.\n");
            if(oldRes->len + toAdd.len+2 >= oldRes->size) { return; }
            ls_utf32Prepend(oldRes, U"Freddo 10, Fuoco 10, "_W);
        }
    }
    else
    {
        const utf32 toAdd = U"Freddo 15, Fuoco 15"_W;
        if(ls_utf32LeftFind(*oldRes, toAdd) == -1)
        {
            AssertMsg(oldRes->len + toAdd.len+2 < oldRes->size, "Insufficient space in resistances string.\n");
            if(oldRes->len + toAdd.len+2 >= oldRes->size) { return; }
            ls_utf32Prepend(oldRes, U"Freddo 15, Fuoco 15, "_W);
        }
    }
}

void fiendishCreatureSpecAtk(utf32 *spec)
{
    AssertMsg(spec, "Null utf32 pointer\n");
    
    const utf32 toAdd = U"Punire il Bene 1/giorno, "_W;
    AssertMsg(spec->len + toAdd.len < spec->size, "Insufficient space in special attack string.\n");
    if(spec->len + toAdd.len >= spec->size) { return; }
    
    ls_utf32Prepend(spec, toAdd);
}

ArchetypeDiff FiendishCreature = {
    U"Immonda"_W, celestialCreatureGS, archetypeASStub, archetypeACStub, celestialCreatureSenses,
    fiendishCreatureRD, fiendishCreatureResistance, celestialCreatureRI, fiendishCreatureSpecAtk,
    archetypeSizeStub, archetypeMeleeStub
};

//-------------------------//
//     GIANT ARCHETYPE     //
//-------------------------//

void giantCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{ *gsDiff = 1; *rmDiff = 0; }

void giantCreatureAS(s32 as[AS_COUNT])
{
    as[AS_STR] += 4;
    as[AS_DEX] -= 2;
    as[AS_CON] += 4;
}

void giantCreatureAC(s32 acDiff[AC_TYPES_COUNT])
{ acDiff[AC_NATURAL] += 3; }

void giantCreatureSize(utf32 *size)
{
    AssertMsg(size, "Null utf32 pointer\n");
    
    s32 index = ls_utf32FirstEqual(*size, (utf32 *)sizeSet, SIZE_COUNT*2);
    if((index == -1) || (index >= (SIZE_COUNT*2)-1)) { return; }
    
    ls_utf32Clear(size);
    ls_utf32Set(size, sizeSet[index+1]);
}

void giantCreatureMelee(utf32 *melee)
{
    AssertMsg(melee, "Null utf32 pointer\n");
    
    const utf32 Message = U" [N.B. Avanza dado per Arch.Gigante]"_W;
    AssertMsg(melee->len + Message.len < melee->size, "Melee archetype can't fit\n");
    if(melee->len + Message.len >= melee->size) { return; }
    
    ls_utf32Append(melee, Message);
}


ArchetypeDiff GiantCreature = {
    U"Gigante"_W, giantCreatureGS, giantCreatureAS, giantCreatureAC, archetypeSensesStub,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, archetypeSpecAtkStub,
    giantCreatureSize, giantCreatureMelee
};

//-------------------------//
//   SCHELETON ARCHETYPE   //
//-------------------------//
#if 0
void scheletonCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{
    //1/2	1/6	65
    switch(hitDice)
    {
        case 1: 
    }
    /*
    1	1/3	135
    2-3	1	400
    4-5	2	600
    6‒7	3	800
    8‒9	4	1.200
    10-11	5	1.600
    12-14	6	2.400
    15-17	7	3.200
    18-20	8	4.800
*/
}

ArchetypeDiff ScheletonCreature = {
    U"Scheletro"_W, scheletonCreatureGS, archetypeASStub, archetypeACStub, archetypeSensesStub,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, archetypeSpecAtkStub,
    archetypeSizeStub, archetypeMeleeStub
};
#endif
//-------------------------//
//    ZOMBIE ARCHETYPE     //
//-------------------------//


//-------------------------//
//  ARCHETYPE APPLICATION  //
//-------------------------//

ArchetypeDiff allArchetypeDiffs[MAX_ARCHETYPES] = {
    AdvancedCreature, CelestialCreature, FiendishCreature, GiantCreature//, ScheletonCreature
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

void CompendiumApplyAllArchetypeGS(u16 gsEntry, s32 hitDice, utf32 *newGS, utf32 *newPE)
{
    if(gsEntry == GS_SENTINEL_VALUE) { return; }
    
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
    
    CompendiumIncreaseGS(gsEntry, totalGSDiff, totalRMDiff, newGS, newPE);
}

void CompendiumApplyAllArchetypeAS(s32 as[AS_COUNT])
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->abilityScores(as);
    }
}

void CompendiumApplyAllArchetypeAC(s32 ac[AC_TYPES_COUNT])
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->armorClass(ac);
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

void CompendiumApplyAllArchetypeRD(s32 hitDice, utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->rd(hitDice, old);
    }
}

void CompendiumApplyAllArchetypeResistances(s32 hitDice, utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->resistances(hitDice, old);
    }
}

void CompendiumApplyAllArchetypeRI(utf32 gs, utf32 *ri)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->ri(gs, ri);
    }
}

void CompendiumApplyAllArchetypeSpecAtk(utf32 *spec)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->specialAtk(spec);
    }
}

void CompendiumApplyAllArchetypeSize(utf32 *size)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->size(size);
    }
}

void CompendiumApplyAllArchetypeMelee(utf32 *melee)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->melee(melee);
    }
}