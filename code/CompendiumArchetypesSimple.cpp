//-------------------------//
//   ADVANCED ARCHETYPE    //
//-------------------------//

b32 advancedCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{
    *gsDiff = 1;
    *rmDiff = 0;
    return FALSE;
}

void advancedCreatureAS(s32 as[AS_COUNT])
{
    if(as[AS_STR] != AS_NO_VALUE) { as[AS_STR] += 4; }
    if(as[AS_DEX] != AS_NO_VALUE) { as[AS_DEX] += 4; }
    if(as[AS_CON] != AS_NO_VALUE) { as[AS_CON] += 4; }
    if(as[AS_INT] != AS_NO_VALUE) { if(as[AS_INT] > 2) { as[AS_INT] += 4; } }
    if(as[AS_WIS] != AS_NO_VALUE) { as[AS_WIS] += 4; }
    if(as[AS_CHA] != AS_NO_VALUE) { as[AS_CHA] += 4; }
}

b32 advancedCreatureAC(utf32 *size, s32 acDiff[AC_TYPES_COUNT])
{
    acDiff[AC_NATURAL] += 2;
    return FALSE;
}

ArchetypeDiff AdvancedCreature = {
    U"Avanzata"_W, archetypeIsCompatibleStub, 
    advancedCreatureGS, advancedCreatureAS, advancedCreatureAC, archetypeSensesStub,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, archetypeSpecAtkStub,
    archetypeSizeStub, archetypeMeleeStub, archetypeAlignStub, archetypeTypeStub, archetypeSubTypeStub,
    archetypeDVStub, archetypeSTStub, archetypeDefCapStub, archetypeSpeedStub, 
    archetypeImmunitiesStub, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, archetypeSpecQualStub, archetypeSpecCapStub,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};

//-------------------------//
//   CELESTIAL ARCHETYPE   //
//-------------------------//

b32 celestialCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{
    if(hitDice >= 5) { *gsDiff = 1; *rmDiff = 0; return FALSE; }
    
    *gsDiff = 0;
    *rmDiff = 0;
    return FALSE; 
}

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

void celestialCreatureResistance(s32 hitDice, u64 orig, utf32 *oldRes)
{
    AssertMsg(oldRes, "Null utf32 pointer\n");
    
    if(hitDice < 5)
    {
        AppendResistanceStringIfNotPresent(orig, RES_ACID_TYPE, 5, U"Acido 5"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_ELEC_TYPE, 5, U"Elettricit\U000000E0 5"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 5, U"Freddo 5"_W, oldRes);
    }
    else if(hitDice < 11)
    {
        AppendResistanceStringIfNotPresent(orig, RES_ACID_TYPE, 10, U"Acido 10"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_ELEC_TYPE, 10, U"Elettricit\U000000E0 10"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 10, U"Freddo 10"_W, oldRes);
    }
    else
    {
        AppendResistanceStringIfNotPresent(orig, RES_ACID_TYPE, 15, U"Acido 15"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_ELEC_TYPE, 15, U"Elettricit\U000000E0 15"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 15, U"Freddo 15"_W, oldRes);
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

b32 celestialCreatureSpecAtk(utf32 *spec)
{
    AssertMsg(spec, "Null utf32 pointer\n");
    
    const utf32 toAdd = U"Punire il Male 1/giorno, "_W;
    AssertMsg(spec->len + toAdd.len < spec->size, "Insufficient space in special attack string.\n");
    if(spec->len + toAdd.len >= spec->size) { return FALSE; }
    
    ls_utf32Prepend(spec, toAdd);
    return FALSE;
}

ArchetypeDiff CelestialCreature = {
    U"Celestiale"_W, archetypeIsCompatibleStub, 
    celestialCreatureGS, archetypeASStub, archetypeACStub, celestialCreatureSenses,
    celestialCreatureRD, celestialCreatureResistance, celestialCreatureRI, celestialCreatureSpecAtk,
    archetypeSizeStub, archetypeMeleeStub, archetypeAlignStub, archetypeTypeStub, archetypeSubTypeStub,
    archetypeDVStub, archetypeSTStub, archetypeDefCapStub, archetypeSpeedStub, 
    archetypeImmunitiesStub, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, archetypeSpecQualStub, archetypeSpecCapStub,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
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

void fiendishCreatureResistance(s32 hitDice, u64 orig, utf32 *oldRes)
{
    AssertMsg(oldRes, "Null utf32 pointer\n");
    
    if(hitDice < 5)
    {
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 5, U"Freddo 5"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_FIRE_TYPE, 5, U"Fuoco 5"_W, oldRes);
    }
    else if(hitDice < 11)
    {
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 10, U"Freddo 10"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_FIRE_TYPE, 10, U"Fuoco 10"_W, oldRes);
    }
    else
    {
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 15, U"Freddo 15"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_FIRE_TYPE, 15, U"Fuoco 15"_W, oldRes);
    }
}

b32 fiendishCreatureSpecAtk(utf32 *spec)
{
    AssertMsg(spec, "Null utf32 pointer\n");
    
    const utf32 toAdd = U"Punire il Bene 1/giorno, "_W;
    AssertMsg(spec->len + toAdd.len < spec->size, "Insufficient space in special attack string.\n");
    if(spec->len + toAdd.len >= spec->size) { return FALSE; }
    
    ls_utf32Prepend(spec, toAdd);
    return FALSE;
}

ArchetypeDiff FiendishCreature = {
    U"Immonda"_W, archetypeIsCompatibleStub, 
    celestialCreatureGS, archetypeASStub, archetypeACStub, celestialCreatureSenses,
    fiendishCreatureRD, fiendishCreatureResistance, celestialCreatureRI, fiendishCreatureSpecAtk,
    archetypeSizeStub, archetypeMeleeStub, archetypeAlignStub, archetypeTypeStub, archetypeSubTypeStub,
    archetypeDVStub, archetypeSTStub, archetypeDefCapStub, archetypeSpeedStub, 
    archetypeImmunitiesStub, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, archetypeSpecQualStub, archetypeSpecCapStub,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};

//-------------------------//
//     GIANT ARCHETYPE     //
//-------------------------//

b32 giantIsCompatible(CachedPageEntry *page)
{
    AssertNonNull(page);
    
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Colossale"))) { return FALSE; }
    
    return TRUE;
}

b32 giantCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{ *gsDiff = 1; *rmDiff = 0; return FALSE; }

void giantCreatureAS(s32 as[AS_COUNT])
{
    if(as[AS_STR] != AS_NO_VALUE) { as[AS_STR] += 4; }
    if(as[AS_DEX] != AS_NO_VALUE) { as[AS_DEX] -= 2; }
    if(as[AS_CON] != AS_NO_VALUE) { as[AS_CON] += 4; }
}

b32 giantCreatureAC(utf32 *size, s32 acDiff[AC_TYPES_COUNT])
{ acDiff[AC_NATURAL] += 3; return FALSE; }

void giantCreatureSize(utf32 *size)
{
    AssertMsg(size, "Null utf32 pointer\n");
    
    s32 index = ls_utf32FirstEqual(*size, (utf32 *)sizeSet, SIZE_COUNT*2);
    if((index == -1) || (index >= (SIZE_COUNT*2)-1)) { return; }
    
    ls_utf32Clear(size);
    ls_utf32Set(size, sizeSet[index+1]);
}

void giantCreatureMelee(CachedPageEntry *page, utf32 *melee)
{
    AssertMsg(melee, "Null utf32 pointer\n");
    
    const utf32 Message = U" [N.B. Avanza dado per Arch.Gigante]"_W;
    AssertMsg(melee->len + Message.len < melee->size, "Melee archetype can't fit\n");
    if(melee->len + Message.len >= melee->size) { return; }
    
    ls_utf32Append(melee, Message);
}


ArchetypeDiff GiantCreature = {
    U"Gigante"_W, giantIsCompatible, giantCreatureGS, giantCreatureAS, giantCreatureAC, archetypeSensesStub,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, archetypeSpecAtkStub,
    giantCreatureSize, giantCreatureMelee, archetypeAlignStub, archetypeTypeStub, archetypeSubTypeStub,
    archetypeDVStub, archetypeSTStub, archetypeDefCapStub, archetypeSpeedStub,
    archetypeImmunitiesStub, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, archetypeSpecQualStub, archetypeSpecCapStub,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};


//-------------------------//
//   ENTROPIC ARCHETYPE    //
//-------------------------//

b32 entropicCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{
    if(hitDice >= 5) { *gsDiff = 1; *rmDiff = 0; return FALSE; }
    
    *gsDiff = 0;
    *rmDiff = 0;
    return FALSE;
}

void entropicCreatureSenses(utf32 *old)
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
void entropicCreatureRD(s32 hitDice, utf32 *oldRD)
{
    AssertMsg(oldRD, "Null utf32 pointer\n");
    
    if(hitDice < 5) { return; }
    else if(hitDice < 11)
    {
        const utf32 toAdd = U"5/legale"_W;
        if(ls_utf32LeftFind(*oldRD, toAdd) == -1)
        {
            AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
            if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
            ls_utf32Prepend(oldRD, U"5/legale, "_W);
        }
    }
    else
    {
        const utf32 toAdd = U"10/legale"_W;
        if(ls_utf32LeftFind(*oldRD, toAdd) == -1)
        {
            AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
            if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
            ls_utf32Prepend(oldRD, U"10/legale, "_W);
        }
    }
}

void entropicCreatureResistance(s32 hitDice, u64 orig, utf32 *oldRes)
{
    AssertMsg(oldRes, "Null utf32 pointer\n");
    
    if(hitDice < 5)
    {
        AppendResistanceStringIfNotPresent(orig, RES_ACID_TYPE, 5, U"Acido 5"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_FIRE_TYPE, 5, U"Fuoco 5"_W, oldRes);
    }
    else if(hitDice < 11)
    {
        AppendResistanceStringIfNotPresent(orig, RES_ACID_TYPE, 10, U"Acido 10"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_FIRE_TYPE, 10, U"Fuoco 10"_W, oldRes);
    }
    else
    {
        AppendResistanceStringIfNotPresent(orig, RES_ACID_TYPE, 15, U"Acido 15"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_FIRE_TYPE, 15, U"Fuoco 15"_W, oldRes);
    }
}

void entropicCreatureRI(utf32 gs, utf32 *ri)
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

b32 entropicCreatureSpecAtk(utf32 *spec)
{
    AssertMsg(spec, "Null utf32 pointer\n");
    
    const utf32 toAdd = U"Punire la Legge 1/giorno, "_W;
    AssertMsg(spec->len + toAdd.len < spec->size, "Insufficient space in special attack string.\n");
    if(spec->len + toAdd.len >= spec->size) { return FALSE; }
    
    ls_utf32Prepend(spec, toAdd);
    return FALSE;
}


ArchetypeDiff EntropicCreature = {
    U"Entropico"_W, archetypeIsCompatibleStub, 
    entropicCreatureGS, archetypeASStub, archetypeACStub, entropicCreatureSenses,
    entropicCreatureRD, entropicCreatureResistance, entropicCreatureRI, entropicCreatureSpecAtk,
    archetypeSizeStub, archetypeMeleeStub, archetypeAlignStub, archetypeTypeStub,
    archetypeSubTypeStub, archetypeDVStub, archetypeSTStub, archetypeDefCapStub, archetypeSpeedStub,
    archetypeImmunitiesStub, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, archetypeSpecQualStub, archetypeSpecCapStub,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};

//-------------------------//
//   RESOLUTE ARCHETYPE    //
//-------------------------//

b32 resoluteCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{
    if(hitDice >= 5) { *gsDiff = 1; *rmDiff = 0; return FALSE; }
    
    *gsDiff = 0;
    *rmDiff = 0;
    return FALSE;
}

void resoluteCreatureSenses(utf32 *old)
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
void resoluteCreatureRD(s32 hitDice, utf32 *oldRD)
{
    AssertMsg(oldRD, "Null utf32 pointer\n");
    
    if(hitDice < 5) { return; }
    else if(hitDice < 11)
    {
        const utf32 toAdd = U"5/caotico"_W;
        if(ls_utf32LeftFind(*oldRD, toAdd) == -1)
        {
            AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
            if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
            ls_utf32Prepend(oldRD, U"5/caotico, "_W);
        }
    }
    else
    {
        const utf32 toAdd = U"10/caotico"_W;
        if(ls_utf32LeftFind(*oldRD, toAdd) == -1)
        {
            AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
            if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
            ls_utf32Prepend(oldRD, U"10/caotico, "_W);
        }
    }
}

void resoluteCreatureResistance(s32 hitDice, u64 orig, utf32 *oldRes)
{
    AssertMsg(oldRes, "Null utf32 pointer\n");
    
    if(hitDice < 5)
    {
        AppendResistanceStringIfNotPresent(orig, RES_ACID_TYPE, 5, U"Acido 5"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 5, U"Freddo 5"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_FIRE_TYPE, 5, U"Fuoco 5"_W, oldRes);
    }
    else if(hitDice < 11)
    {
        AppendResistanceStringIfNotPresent(orig, RES_ACID_TYPE, 10, U"Acido 10"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 10, U"Freddo 10"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_FIRE_TYPE, 10, U"Fuoco 10"_W, oldRes);
    }
    else
    {
        AppendResistanceStringIfNotPresent(orig, RES_ACID_TYPE, 15, U"Acido 15"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 15, U"Freddo 15"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_FIRE_TYPE, 15, U"Fuoco 15"_W, oldRes);
    }
}

void resoluteCreatureRI(utf32 gs, utf32 *ri)
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

b32 resoluteCreatureSpecAtk(utf32 *spec)
{
    AssertMsg(spec, "Null utf32 pointer\n");
    
    const utf32 toAdd = U"Punire il Caos 1/giorno, "_W;
    AssertMsg(spec->len + toAdd.len < spec->size, "Insufficient space in special attack string.\n");
    if(spec->len + toAdd.len >= spec->size) { return FALSE; }
    
    ls_utf32Prepend(spec, toAdd);
    return FALSE;
}


ArchetypeDiff ResoluteCreature = {
    U"Risoluto"_W, archetypeIsCompatibleStub, 
    resoluteCreatureGS, archetypeASStub, archetypeACStub, resoluteCreatureSenses,
    resoluteCreatureRD, resoluteCreatureResistance, resoluteCreatureRI, resoluteCreatureSpecAtk,
    archetypeSizeStub, archetypeMeleeStub, archetypeAlignStub, archetypeTypeStub,
    archetypeSubTypeStub, archetypeDVStub, archetypeSTStub, archetypeDefCapStub, archetypeSpeedStub,
    archetypeImmunitiesStub, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, archetypeSpecQualStub, archetypeSpecCapStub,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};


//-------------------------//
//   FUNGOID ARCHETYPE    //
//-------------------------//

b32 fungoidIsCompatible(CachedPageEntry *page)
{
    AssertNonNull(page);
    
    if(ls_utf32AreEqual(page->type, U"Non Morto"_W)) { return FALSE; }
    if(ls_utf32AreEqual(page->type, U"Vegetale"_W))  { return FALSE; }
    
    return TRUE;
}

b32 fungoidCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{ *gsDiff = 1; *rmDiff = 0; return FALSE; }

b32 fungoidCreatureAlign(utf32 *align)
{
    ls_utf32Set(align, U"CM"_W);
    return TRUE;
}

void fungoidCreatureType(utf32 *type)
{ 
    ls_utf32Clear(type);
    ls_utf32Append(type, U"Vegetale");
    return;
}

void fungoidCreatureImmunities(utf32 *old)
{ 
    AssertMsg(old, "Null Old Immunities string\n");
    
    const utf32 toAdd2 = U"Tratti dei Vegetali"_W;
    AssertMsg(old->len + toAdd2.len+2 < old->size, "Insufficient space in immunities string.\n");
    if(old->len + toAdd2.len+2 >= old->size) { return; }
    ls_utf32Append(old, toAdd2);
    
    return;
}

void fungoidCreatureLanguage(utf32 *old)
{
    AssertMsg(old, "Null Old Language string\n");
    
    const utf32 toAdd = U"; Telepatia 9m (creature fungine)"_W;
    AssertMsg(old->len + toAdd.len+2 < old->size, "Insufficient space in immunities string.\n");
    if(old->len + toAdd.len+2 >= old->size) { return; }
    ls_utf32Append(old, toAdd);
    
    return;
}

ArchetypeDiff FungoidCreature = {
    U"Fungoide"_W, fungoidIsCompatible, 
    fungoidCreatureGS, archetypeASStub, archetypeACStub, archetypeSensesStub,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, archetypeSpecAtkStub,
    archetypeSizeStub, archetypeMeleeStub, fungoidCreatureAlign, fungoidCreatureType,
    archetypeSubTypeStub, archetypeDVStub, archetypeSTStub, archetypeDefCapStub, archetypeSpeedStub,
    fungoidCreatureImmunities, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, archetypeSpecQualStub, archetypeSpecCapStub,
    fungoidCreatureLanguage, archetypeAuraStub, archetypeWeakStub,
};


//-------------------------//
//    SHADOW ARCHETYPE     //
//-------------------------//

b32 shadowCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{ *gsDiff = 1; *rmDiff = 0; return FALSE; }

//TODO: Static utf32 strings AAAAAH
void shadowCreatureSenses(utf32 *old)
{
    AssertMsg(old, "Null utf32 pointer\n");
    
    const utf32 toAdd = U"Scurovisione 18 m"_W;
    if(ls_utf32LeftFind(*old, toAdd) == -1)
    {
        AssertMsg(old->len + toAdd.len+2 < old->size, "Insufficient space in senses string.\n");
        if(old->len + toAdd.len+2 >= old->size) { return; }
        ls_utf32Prepend(old, U"Scurovisione 18 m, "_W);
    }
    
    const utf32 toAdd2 = U"Visione Crepuscolare 18 m"_W;
    if(ls_utf32LeftFind(*old, toAdd2) == -1)
    {
        AssertMsg(old->len + toAdd2.len+2 < old->size, "Insufficient space in senses string.\n");
        if(old->len + toAdd2.len+2 >= old->size) { return; }
        ls_utf32Prepend(old, U"Visione Crepuscolare 18 m, "_W);
    }
}

//TODO: This actually sucks. The way it's setup now it would need parsing to properly find old rd,
//      and replace them with better ones if necessary. Need to update hyperGol
void shadowCreatureRD(s32 hitDice, utf32 *oldRD)
{
    AssertMsg(oldRD, "Null utf32 pointer\n");
    
    if(hitDice < 5) { return; }
    else if(hitDice < 11)
    {
        const utf32 toAdd = U"5/magia"_W;
        if(ls_utf32LeftFind(*oldRD, toAdd) == -1)
        {
            AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
            if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
            ls_utf32Prepend(oldRD, U"5/magia, "_W);
        }
    }
    else
    {
        const utf32 toAdd = U"10/magia"_W;
        if(ls_utf32LeftFind(*oldRD, toAdd) == -1)
        {
            AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
            if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
            ls_utf32Prepend(oldRD, U"10/magia, "_W);
        }
    }
}

void shadowCreatureResistance(s32 hitDice, u64 orig, utf32 *oldRes)
{
    AssertMsg(oldRes, "Null utf32 pointer\n");
    
    if(hitDice < 5)
    {
        AppendResistanceStringIfNotPresent(orig, RES_ELEC_TYPE, 5, U"Elettricit\U000000E0 5"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 5, U"Freddo 5"_W, oldRes);
    }
    else if(hitDice < 11)
    {
        AppendResistanceStringIfNotPresent(orig, RES_ELEC_TYPE, 10, U"Elettricit\U000000E0 10"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 10, U"Freddo 10"_W, oldRes);
    }
    else
    {
        AppendResistanceStringIfNotPresent(orig, RES_ELEC_TYPE, 15, U"Elettricit\U000000E0 15"_W, oldRes);
        AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 15, U"Freddo 15"_W, oldRes);
    }
}

void shadowCreatureRI(utf32 gs, utf32 *ri)
{
    AssertMsg(ri, "Null utf32 pointer\n");
    
    s32 index = ls_utf32FirstEqual(gs, (utf32 *)gsSet, gsSetCount);
    
    AssertMsg(((index != -1) || (index < gsSetCount)), "Invalid GS when applying Advanced archetype\n");
    if((index == -1) || (index >= gsSetCount)) { return; }
    
    s64 oldRI = ls_utf32ToInt(*ri);
    
    s32 gsVal = (index - 4);
    if(gsVal <= 0) {
        const utf32 toAdd = U"6"_W;
        if(oldRI < 6) {
            ls_utf32Clear(ri);
            ls_utf32Append(ri, toAdd);
        }
        else { return; }
    }
    else
    {
        gsVal %= 36;
        if(oldRI < (gsVal + 6))
        {
            ls_utf32Clear(ri);
            ls_utf32AppendInt(ri, gsVal+6);
        }
        else { return; }
    }
}

void shadowCreatureSpecQual(utf32 *old)
{
    AssertMsg(old, "Null spec qual pointer string\n");
    
    utf32 toAdd = U"Fondersi con le Ombre"_W;
    if(old->len + toAdd.len+2 >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec qual\n"); return; }
    
    if(old->len == 0)
    { ls_utf32Prepend(old, toAdd); }
    else
    { ls_utf32Prepend(old, U"Fondersi con le Ombre, "_W); }
    
    return;
}

void shadowCreatureSpecCap(utf32 *old)
{
    AssertMsg(old, "Null spec cap pointer string\n");
    
    utf32 toAdd = U"\n\nFondersi con le Ombre (Sop)\nIn qualsiasi condizione di illuminazione diversa"
        "dalla luce intensa, una Creatura d'Ombra si fonde con le ombre, "
        "guadagnando Occultamento (20% di probabilit\U000000E0 di mancare). "
        "Una Creatura d'Ombra può interrompere e riattivare questa sua capacit\U000000E0"
        "come azione gratuita."_W;
    
    if(old->len + toAdd.len >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec cap\n"); return; }
    
    ls_utf32Append(old, toAdd);
    return;
}


ArchetypeDiff ShadowCreature = {
    U"d'Ombra"_W, archetypeIsCompatibleStub, 
    shadowCreatureGS, archetypeASStub, archetypeACStub, shadowCreatureSenses,
    shadowCreatureRD, shadowCreatureResistance, shadowCreatureRI, archetypeSpecAtkStub,
    archetypeSizeStub, archetypeMeleeStub, archetypeAlignStub, archetypeTypeStub,
    archetypeSubTypeStub, archetypeDVStub, archetypeSTStub, archetypeDefCapStub, archetypeSpeedStub,
    archetypeImmunitiesStub, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, shadowCreatureSpecQual, shadowCreatureSpecCap,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};


//-------------------------//
//  FLESHWARPED ARCHETYPE  //
//-------------------------//

b32 fleshwarpedIsCompatible(CachedPageEntry *page)
{
    AssertNonNull(page);
    
    if(ls_utf32AreEqual(page->type, U"Non Morto"_W)) { return FALSE; }
    if(ls_utf32Contains(page->subtype, U"Incorporeo"_W)) { return FALSE; }
    
    return TRUE;
}

b32 fleshwarpedCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{ *gsDiff = 1; *rmDiff = 0; return FALSE; }

void fleshwarpedCreatureAS(s32 as[AS_COUNT])
{
    if(as[AS_STR] != AS_NO_VALUE) { as[AS_STR] += 4; }
    if(as[AS_CON] != AS_NO_VALUE) { as[AS_CON] += 4; }
    if(as[AS_INT] != AS_NO_VALUE) { if(as[AS_INT] < 4) { as[AS_INT] = 1; } else { as[AS_INT] -= 4; } }
    if(as[AS_CHA] != AS_NO_VALUE) { if(as[AS_CHA] < 4) { as[AS_CHA] = 1; } else { as[AS_CHA] -= 4; } }
}

b32 fleshwarpedCreatureAC(utf32 *size, s32 acDiff[AC_TYPES_COUNT])
{
    acDiff[AC_NATURAL] += 2;
    return FALSE;
}

void fleswarpedCreatureSpeed(utf32 *old)
{
    //TODO: It technically lets you choose a new movement speed among 4 different kinds
    //      To be done properly should add optional prompt to ask the user which it would want.
    //      Look at @ArchOptionalPrompt
    return;
}


ArchetypeDiff FleshwarpedCreature = {
    U"Carnideformato"_W, fleshwarpedIsCompatible, 
    fleshwarpedCreatureGS, fleshwarpedCreatureAS, fleshwarpedCreatureAC, archetypeSensesStub,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, archetypeSpecAtkStub,
    archetypeSizeStub, archetypeMeleeStub, archetypeAlignStub, archetypeTypeStub, archetypeSubTypeStub,
    archetypeDVStub, archetypeSTStub, archetypeDefCapStub, fleswarpedCreatureSpeed, 
    archetypeImmunitiesStub, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, archetypeSpecQualStub, archetypeSpecCapStub,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};



//-------------------------//
//   DEGENERATE ARCHETYPE  //
//-------------------------//

b32 degenerateCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{ *gsDiff = -1; *rmDiff = 0; return FALSE; }

void degenerateCreatureAS(s32 as[AS_COUNT])
{
    if(as[AS_STR] != AS_NO_VALUE) { if(as[AS_STR] < 4) { as[AS_STR] = 1; } else { as[AS_STR] -= 4; } }
    if(as[AS_DEX] != AS_NO_VALUE) { if(as[AS_DEX] < 4) { as[AS_DEX] = 1; } else { as[AS_DEX] -= 4; } }
    if(as[AS_CON] != AS_NO_VALUE) { if(as[AS_CON] < 4) { as[AS_CON] = 1; } else { as[AS_CON] -= 4; } }
    if(as[AS_INT] != AS_NO_VALUE) { if(as[AS_INT] < 4) { as[AS_INT] = 1; } else { as[AS_INT] -= 4; } }
    if(as[AS_WIS] != AS_NO_VALUE) { if(as[AS_WIS] < 4) { as[AS_WIS] = 1; } else { as[AS_WIS] -= 4; } }
    if(as[AS_CHA] != AS_NO_VALUE) { if(as[AS_CHA] < 4) { as[AS_CHA] = 1; } else { as[AS_CHA] -= 4; } }
}

ArchetypeDiff DegenerateCreature = {
    U"Degenerato"_W, archetypeIsCompatibleStub, 
    degenerateCreatureGS, degenerateCreatureAS, archetypeACStub, archetypeSensesStub,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, archetypeSpecAtkStub,
    archetypeSizeStub, archetypeMeleeStub, archetypeAlignStub, archetypeTypeStub, archetypeSubTypeStub,
    archetypeDVStub, archetypeSTStub, archetypeDefCapStub, archetypeSpeedStub, 
    archetypeImmunitiesStub, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, archetypeSpecQualStub, archetypeSpecCapStub,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};
