//-------------------------//
//   SKELETON ARCHETYPE   //
//-------------------------//

b32 skeletonCreatureGS(s32 hitDice, s32 *gsIndex, s32 *rm)
{
    //NOTE: Because this creature sets the gs, rather than modify it, we report the index in the gsSet
    switch(hitDice)
    {
        //NOTETODO: The table refers to 1/2 DV, but I haven't seen any creature with half DV ?!?!?
        case 1: { *gsIndex = 3; } break;
        
        case 2:
        case 3: { *gsIndex = 5; } break;
        
        case 4:
        case 5: { *gsIndex = 6; } break;
        
        case 6:
        case 7: { *gsIndex = 7; } break;
        
        case 8:
        case 9: { *gsIndex = 8; } break;
        
        case 10:
        case 11: { *gsIndex = 9; } break;
        
        case 12:
        case 13:
        case 14: { *gsIndex = 10; } break;
        
        case 15:
        case 16:
        case 17: { *gsIndex = 11; } break;
        
        case 18:
        case 19:
        case 20: { *gsIndex = 12; } break;
    }
    
    return TRUE;
}

b32 skeletonCreatureAC(utf32 *size, s32 ac[AC_TYPES_COUNT])
{
    for(s32 i = 0; i < AC_TYPES_COUNT; i++)
    { ac[i] = -99; }
    
    if(ls_utf32AreEqual(*size, U"Colossale"_W))         { ac[AC_NATURAL] = 10; }
    else if(ls_utf32AreEqual(*size, U"Mastodontica"_W)) { ac[AC_NATURAL] = 6; }
    else if(ls_utf32AreEqual(*size, U"Enorme"_W))       { ac[AC_NATURAL] = 3; }
    else if(ls_utf32AreEqual(*size, U"Grande"_W))       { ac[AC_NATURAL] = 2; }
    else if(ls_utf32AreEqual(*size, U"Media"_W))        { ac[AC_NATURAL] = 2; }
    else if(ls_utf32AreEqual(*size, U"Piccola"_W))      { ac[AC_NATURAL] = 1; }
    else                                                { ac[AC_NATURAL] = 0; }
    
    return TRUE;
}

b32 skeletonCreatureAlign(utf32 *oldAlign)
{
    ls_utf32Set(oldAlign, U"NM"_W);
    return TRUE;
}

void skeletonCreatureType(utf32 *oldType)
{
    ls_utf32Clear(oldType);
    ls_utf32Append(oldType, U"Non Morto");
    
    return;
}

void skeletonCreatureSubType(utf32 *oldSubType)
{
    //NOTETODO: It works like shit and I'm not sure on how it works.
#if 0
    //NOTE: List of Invalid Subtypes of Alignment
    ls_utf32RmSubstrInList(oldSubType, (utf32 *)alignmentSubtypes, ALIGNMENT_SUBTYPE_COUNT);
    
    //NOTE: List of Invalid Subtypes of Kind
    ls_utf32RmSubstrInList(oldSubType, (utf32 *)raceSubtypes, RACE_SUBTYPES_COUNT);
#endif
    
    return;
}

void skeletonCreatureDV(CachedPageEntry *page, utf32 *oldType, u64 *DV)
{
    u64 newDV = *DV;
    
    //NOTE: Clear all DV except for the first one (Basically remove all class DV and keep only Race DVs
    //      Also clear Race DVs face (lowest 3 bits), since we will convert it to a D8.
    newDV &= 0xFFFFFFF0000001F8;
    
    //NOTE: Set Racial Die Face to HP_Die_D8
    newDV |= (u64)(HP_Die_Face::HP_Die_D8);
    
    //NOTE: When changing type, the ability score bonuses to flat hp might change. This is taken care of
    //      in CalculateAndCacheHP, to avoid copy-pasting the same code over and over again in every archetype.
    *DV = newDV;
}

b32 skeletonCreatureST(s32 totalDV, s32 st[ST_COUNT])
{
    st[ST_CON] = totalDV / 3;
    st[ST_DEX] = totalDV / 3;
    st[ST_WIS] = (totalDV / 2) + 2;
    
    return TRUE;
}

void skeletonCreatureDefCap(utf32 *old)
{
    ls_utf32Clear(old);
}

void skeletonCreatureSpeed(utf32 *old)
{
    utf32 toFind = U"Volare "_W;
    utf32 toInsert = U"[SOLO MAGICO] "_W;
    if(old->len+toInsert.len >= old->size) { AssertMsg(FALSE, "speed string not large enough\n"); return; }
    
    s32 index = ls_utf32LeftFind(*old, toFind);
    if(index == -1) { return; }
    
    s32 insertIndex = index + toFind.len;
    ls_utf32InsertSubstr(old, toInsert, insertIndex);
    return;
}

void skeletonCreatureRD(s32 hitDice, utf32 *oldRD)
{ 
    AssertMsg(oldRD, "Null Old RD string\n");
    
    ls_utf32Clear(oldRD);
    
    const utf32 toAdd = U"5/contundente"_W;
    AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
    if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
    ls_utf32Prepend(oldRD, toAdd);
    
    return;
}

//TODO: This is stupid. We do build the immunity from a packed value, thus we could
//      avoid the stupid text check for an already present immunity.
//      We probably don't want to modify the packed value itself, 
//      since it might create problems (like having too many immunities...)
void skeletonCreatureImmunities(utf32 *old)
{
    AssertMsg(old, "Null Old Immunities string\n");
    
    ls_utf32Clear(old);
    
    const utf32 toAdd = U"Freddo, "_W;
    AssertMsg(old->len + toAdd.len+2 < old->size, "Insufficient space in immunities string.\n");
    if(old->len + toAdd.len+2 >= old->size) { return; }
    ls_utf32Append(old, toAdd);
    
    const utf32 toAdd2 = U"Tratti dei Non Morti"_W;
    AssertMsg(old->len + toAdd2.len+2 < old->size, "Insufficient space in immunities string.\n");
    if(old->len + toAdd2.len+2 >= old->size) { return; }
    ls_utf32Append(old, toAdd2);
    
    return;
}

void skeletonCreatureMelee(CachedPageEntry *page, utf32 *melee)
{
    //TODO: We should technically check if the melee attack was already present, and only keep the
    //      stronger one. For now, for semplicity, we'll just append a new attack (without specifying the number)
    
    s32 attackBonus = page->BABval + page->modAS[AS_STR] - 10;
    s32 damageBonus = page->modAS[AS_STR] - 10;
    if(melee->len == 0) { damageBonus *= 1.5; } //NOTE: If only attack
    
    if(melee->len > 0) { ls_utf32Append(melee, U", N artigli "_W); }
    else               { ls_utf32Append(melee, U"N artigli "_W); }
    
    if(attackBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, attackBonus);
    
    ls_utf32Append(melee, U" ("_W);
    
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuta")))       { ls_utf32Append(melee, U"1"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuscola")))    { ls_utf32Append(melee, U"1d2"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Piccola")))      { ls_utf32Append(melee, U"1d3"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Media")))        { ls_utf32Append(melee, U"1d4"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Grande")))       { ls_utf32Append(melee, U"1d6"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Enorme")))       { ls_utf32Append(melee, U"1d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Mastodontica"))) { ls_utf32Append(melee, U"2d6"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Colossale")))    { ls_utf32Append(melee, U"2d8"_W); }
    
    if(damageBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, damageBonus);
    
    ls_utf32AppendChar(melee, ')');
    
    return;
}

b32 skeletonCreatureSpecAtk(utf32 *spec)
{
    ls_utf32Clear(spec);
    return TRUE;
}

void skeletonCreatureAS(s32 as[AS_COUNT])
{
    as[AS_DEX] += 2;
    as[AS_CON] = AS_NO_VALUE;
    as[AS_INT] = AS_NO_VALUE;
    as[AS_WIS] = 10;
    as[AS_CHA] = 10;
    return;
}

void skeletonCreatureBAB(utf32 *old, s32 dv)
{
    ls_utf32Clear(old);
    
    s32 bab = (s32)(((f64)dv)*0.75);
    if(bab > 0) { ls_utf32Append(old, U"+"); }
    ls_utf32AppendInt(old, bab);
}

u32 skeletonCreatureSkills(u32 skillEntry)
{ 
    //NOTE: Skeletons loose all skills
    return 0;
}

void skeletonCreatureTalents(CachedPageEntry *page)
{
    for(s32 i = 0; i < 24; i++)
    { ls_utf32Clear(page->talents + i); }
    
    u32 packed = GetTalentPackedFromName(U"Iniziativa Migliorata"_W);
    
    if(packed != TALENT_NOT_FOUND)
    {
        BuildTalentFromPacked_t(&compendium.codex, packed, &page->talents[0]);
        page->talentEntry[0] = packed;
    }
}

void skeletonCreatureSpecQual(utf32 *old)
{
    AssertMsg(old, "Null spec qual string pointer\n");
    
    utf32 toAdd = U"N.B. Solo (Str) che potenziano Mischia e Distanza! "_W;
    if(old->len + toAdd.len >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec qual\n"); return; }
    
    ls_utf32Prepend(old, toAdd);
    return;
}

void skeletonCreatureSpecCap(utf32 *old)
{
    AssertMsg(old, "Null spec cap string pointer\n");
    
    utf32 toAdd = U"N.B. Solo (Str) che potenziano Mischia e Distanza!\n\n"_W;
    if(old->len + toAdd.len >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec cap\n"); return; }
    
    ls_utf32Prepend(old, toAdd);
    return;
}

ArchetypeDiff SkeletonCreature = {
    U"Scheletro"_W, skeletonCreatureGS, skeletonCreatureAS, skeletonCreatureAC, archetypeSensesStub,
    skeletonCreatureRD, archetypeResistanceStub, archetypeRIStub, skeletonCreatureSpecAtk,
    archetypeSizeStub, skeletonCreatureMelee, skeletonCreatureAlign, skeletonCreatureType, skeletonCreatureSubType,
    skeletonCreatureDV, skeletonCreatureST, skeletonCreatureDefCap, skeletonCreatureSpeed, 
    skeletonCreatureImmunities, skeletonCreatureBAB, skeletonCreatureSkills, skeletonCreatureTalents, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, skeletonCreatureSpecQual, skeletonCreatureSpecCap,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};

//-------------------------//
//    ZOMBIE ARCHETYPE     //
//-------------------------//

b32 zombieCreatureGS(s32 hitDice, s32 *gsIndex, s32 *rm)
{
    //NOTE: Because this creature sets the gs, rather than modify it, we report the index in the gsSet
    switch(hitDice)
    {
        //NOTETODO: The table refers to 1/2 DV, but I haven't seen any creature with half DV ?!?!?
        case 1:  { *gsIndex = 2; } break;
        
        case 2:  { *gsIndex = 4; } break;
        
        case 3:
        case 4:  { *gsIndex = 5; } break;
        
        case 5:
        case 6:  { *gsIndex = 6; } break;
        
        case 7:
        case 8:  { *gsIndex = 7; } break;
        
        case 9:
        case 10: { *gsIndex = 8; } break;
        
        case 11:
        case 12: { *gsIndex = 9; } break;
        
        case 13:
        case 14:
        case 15:
        case 16: { *gsIndex = 10; } break;
        
        case 17:
        case 18:
        case 19:
        case 20: { *gsIndex = 11; } break;
        
        case 21:
        case 22:
        case 23:
        case 24: { *gsIndex = 12; } break;
        
        case 25:
        case 26:
        case 27:
        case 28: { *gsIndex = 13; } break;
    }
    
    return TRUE;
}

b32 zombieCreatureAC(utf32 *size, s32 ac[AC_TYPES_COUNT])
{
    for(s32 i = 0; i < AC_TYPES_COUNT; i++)
    { ac[i] = -99; }
    
    if(ls_utf32AreEqual(*size, U"Colossale"_W))         { ac[AC_NATURAL] = 11; }
    else if(ls_utf32AreEqual(*size, U"Mastodontica"_W)) { ac[AC_NATURAL] = 7; }
    else if(ls_utf32AreEqual(*size, U"Enorme"_W))       { ac[AC_NATURAL] = 4; }
    else if(ls_utf32AreEqual(*size, U"Grande"_W))       { ac[AC_NATURAL] = 3; }
    else if(ls_utf32AreEqual(*size, U"Media"_W))        { ac[AC_NATURAL] = 2; }
    else if(ls_utf32AreEqual(*size, U"Piccola"_W))      { ac[AC_NATURAL] = 1; }
    else                                                { ac[AC_NATURAL] = 0; }
    
    return TRUE;
}

b32 zombieCreatureAlign(utf32 *oldAlign)
{
    ls_utf32Set(oldAlign, U"NM"_W);
    return TRUE;
}

void zombieCreatureType(utf32 *oldType)
{
    ls_utf32Clear(oldType);
    ls_utf32Append(oldType, U"Non Morto");
    
    return;
}

void zombieCreatureSubType(utf32 *oldSubType)
{
    //NOTETODO: It works like shit and I'm not sure on how it works.
#if 0
    //NOTE: List of Invalid Subtypes of Alignment
    ls_utf32RmSubstrInList(oldSubType, (utf32 *)alignmentSubtypes, ALIGNMENT_SUBTYPE_COUNT);
    
    //NOTE: List of Invalid Subtypes of Kind
    ls_utf32RmSubstrInList(oldSubType, (utf32 *)raceSubtypes, RACE_SUBTYPES_COUNT);
#endif
    
    return;
}

void zombieCreatureDV(CachedPageEntry *page, utf32 *oldType, u64 *DV)
{
    u64 newDV = *DV;
    u16 oldRaceDV = (newDV >> ((HP_MAX_DICE_COUNT-1)*HP_DIE_BITLEN)) & HP_DIE_MASK;
    u16 oldCount  = (oldRaceDV & HP_DIE_COUNT_MASK) >> HP_DIE_FACE_BITLEN;
    
    if(ls_utf32AreEqual(page->size, U"Colossale"_W))         { oldCount += 11; }
    else if(ls_utf32AreEqual(page->size, U"Mastodontica"_W)) { oldCount += 6; }
    else if(ls_utf32AreEqual(page->size, U"Enorme"_W))       { oldCount += 4; }
    else if(ls_utf32AreEqual(page->size, U"Grande"_W))       { oldCount += 2; }
    else if(ls_utf32AreEqual(page->size, U"Media"_W))        { oldCount += 1; }
    else if(ls_utf32AreEqual(page->size, U"Piccola"_W))      { oldCount += 1; }
    else                                                     { oldCount += 0; }
    
    oldCount &= 0x003F;
    
    //NOTE: Clear all DV except for the first one (Basically remove all class DV and keep only Race DVs
    //      Also clear Race DVs face (lowest 3 bits), since we will convert it to a D8.
    newDV &= 0xFFFFFFF0000001F8;
    
    //NOTE: Set Racial Die Face to HP_Die_D8, and set new DV Count.
    newDV |= (u64)(HP_Die_Face::HP_Die_D8);
    newDV |= (u64)(oldCount << HP_DIE_FACE_BITLEN);
    
    //NOTE: When changing type, the ability score bonuses to flat hp might change. This is taken care of
    //      in CalculateAndCacheHP, to avoid copy-pasting the same code over and over again in every archetype.
    *DV = newDV;
}

b32 zombieCreatureST(s32 totalDV, s32 st[ST_COUNT])
{
    st[ST_CON] = totalDV / 3;
    st[ST_DEX] = totalDV / 3;
    st[ST_WIS] = (totalDV / 2) + 2;
    
    return TRUE;
}

void zombieCreatureDefCap(utf32 *old)
{
    ls_utf32Clear(old);
}

void zombieCreatureSpeed(utf32 *old)
{
    //TODO: Because maneuverability goes down, the bonus to the flight ability changes.
    //      Zombies loose all abilities, so I'll leave it to the GM to calculate it if necessary
    //      But showing it properly would be better.
    const s32 manuvCount = 8;
    const utf32 patterns[manuvCount] = { U"(scarsa)"_W, U"(media)"_W, U"(buona)"_W, U"(perfetta)"_W,
        U"(Scarsa)"_W, U"(Media)"_W, U"(Buona)"_W, U"(Perfetta)"_W,
    };
    utf32 replacement = U"(maldestra)"_W;
    
    for(s32 i = 0; i < manuvCount; i++)
    {
        if(old->len - patterns[i].len + replacement.len >= old->size)
        { AssertMsg(FALSE, "speed string not large enough\n"); return; }
        
        if(ls_utf32Replace(old, patterns[i], replacement) == TRUE) { return; }
    }
    
    return;
}

void zombieCreatureRD(s32 hitDice, utf32 *oldRD)
{ 
    AssertMsg(oldRD, "Null Old RD string\n");
    
    ls_utf32Clear(oldRD);
    
    const utf32 toAdd = U"5/tagliente"_W;
    AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
    if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
    ls_utf32Prepend(oldRD, toAdd);
    
    return;
}

//TODO: This is stupid. We do build the immunity from a packed value, thus we could
//      avoid the stupid text check for an already present immunity.
//      We probably don't want to modify the packed value itself, 
//      since it might create problems (like having too many immunities...)
void zombieCreatureImmunities(utf32 *old)
{
    AssertMsg(old, "Null Old Immunities string\n");
    
    ls_utf32Clear(old);
    
    const utf32 toAdd2 = U"Tratti dei Non Morti"_W;
    AssertMsg(old->len + toAdd2.len+2 < old->size, "Insufficient space in immunities string.\n");
    if(old->len + toAdd2.len+2 >= old->size) { return; }
    ls_utf32Append(old, toAdd2);
    
    return;
}

void zombieCreatureMelee(CachedPageEntry *page, utf32 *melee)
{
    //TODO: We should technically check if the melee attack was already present, and only keep the
    //      stronger one. For now, for semplicity, we'll just append a new attack (without specifying the number)
    
    s32 attackBonus = page->BABval + page->modAS[AS_STR] - 10;
    s32 damageBonus = page->modAS[AS_STR] - 10;
    if(melee->len == 0) { damageBonus *= 1.5; } //NOTE: If only attack
    
    if(melee->len > 0) { ls_utf32Append(melee, U", schianto "_W); }
    else               { ls_utf32Append(melee, U"schianto  "_W); }
    
    if(attackBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, attackBonus);
    
    ls_utf32Append(melee, U" ("_W);
    
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuta")))       { ls_utf32Append(melee, U"1d2"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuscola")))    { ls_utf32Append(melee, U"1d3"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Piccola")))      { ls_utf32Append(melee, U"1d4"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Media")))        { ls_utf32Append(melee, U"1d6"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Grande")))       { ls_utf32Append(melee, U"1d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Enorme")))       { ls_utf32Append(melee, U"2d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Mastodontica"))) { ls_utf32Append(melee, U"2d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Colossale")))    { ls_utf32Append(melee, U"3d6"_W); }
    
    if(damageBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, damageBonus);
    
    ls_utf32AppendChar(melee, ')');
    
    return;
}

b32 zombieCreatureSpecAtk(utf32 *spec)
{
    ls_utf32Clear(spec);
    return TRUE;
}

void zombieCreatureAS(s32 as[AS_COUNT])
{
    as[AS_STR] += 2;
    if(as[AS_DEX] <= 2) { as[AS_DEX] = 1; } else { as[AS_DEX] -= 2; }
    as[AS_CON] = AS_NO_VALUE;
    as[AS_INT] = AS_NO_VALUE;
    as[AS_WIS] = 10;
    as[AS_CHA] = 10;
    return;
}

void zombieCreatureBAB(utf32 *old, s32 dv)
{
    ls_utf32Clear(old);
    
    s32 bab = (s32)(((f64)dv)*0.75);
    if(bab > 0) { ls_utf32Append(old, U"+"); }
    ls_utf32AppendInt(old, bab);
}

u32 zombieCreatureSkills(u32 skillEntry)
{
    //NOTE: Zombies loose all skills
    return 0;
}

void zombieCreatureTalents(CachedPageEntry *page)
{
    for(s32 i = 0; i < 24; i++)
    { ls_utf32Clear(page->talents + i); }
    
    u32 packed = GetTalentPackedFromName(U"Robustezza"_W);
    
    if(packed != TALENT_NOT_FOUND)
    {
        BuildTalentFromPacked_t(&compendium.codex, packed, &page->talents[0]);
        page->talentEntry[0] = packed;
    }
}

void zombieCreatureSpecQual(utf32 *old)
{
    utf32 toAdd = U"N.B. Solo (Str) che potenziano Mischia e Distanza! "_W;
    utf32 toAdd2 = U", Barcollante"_W;
    if(old->len + toAdd.len + toAdd2.len >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec qual\n"); return; }
    
    ls_utf32Prepend(old, toAdd);
    ls_utf32Append(old, toAdd2);
    return;
}

void zombieCreatureSpecCap(utf32 *old)
{
    utf32 toAdd = U"N.B. Solo (Str) che potenziano Mischia e Distanza!\n"_W;
    utf32 toAdd2 = U"\n\nBarcollante (Str)\nGli zombi hanno riflessi scarsi e possono "
        "effettuare soltanto una azione di movimento o un'azione standard ogni round. "
        "Uno zombi pu\U000000F2 muoversi alla propria velocit\U000000E0 ed attaccare soltanto effettuando "
        "un'azione di carica."_W;
    
    if(old->len + toAdd.len + toAdd2.len >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec cap\n"); return; }
    
    ls_utf32Prepend(old, toAdd);
    ls_utf32Append(old, toAdd2);
    return;
}

ArchetypeDiff ZombieCreature = {
    U"Zombi"_W, zombieCreatureGS, zombieCreatureAS, zombieCreatureAC, archetypeSensesStub,
    zombieCreatureRD, archetypeResistanceStub, archetypeRIStub, zombieCreatureSpecAtk,
    archetypeSizeStub, zombieCreatureMelee, zombieCreatureAlign, zombieCreatureType, zombieCreatureSubType,
    zombieCreatureDV, zombieCreatureST, zombieCreatureDefCap, zombieCreatureSpeed, zombieCreatureImmunities,
    zombieCreatureBAB, zombieCreatureSkills, zombieCreatureTalents, archetypeEnvStub, archetypeOrgStub,
    archetypeTreasureStub, zombieCreatureSpecQual, zombieCreatureSpecCap, archetypeLangStub, archetypeAuraStub,
    archetypeWeakStub,
};


//----------------------------//
// BLOODY SKELETON ARCHETYPE  //
//----------------------------//

b32 bloodySkeletonCreatureGS(s32 hitDice, s32 *gsIndex, s32 *rm)
{
    //NOTE: Because this creature sets the gs, rather than modify it, we report the index in the gsSet
    switch(hitDice)
    {
        //NOTETODO: The table refers to 1/2 DV, but I haven't seen any creature with half DV ?!?!?
        case 1: { *gsIndex = 4; } break;
        
        case 2:
        case 3: { *gsIndex = 6; } break;
        
        case 4:
        case 5: { *gsIndex = 7; } break;
        
        case 6:
        case 7: { *gsIndex = 8; } break;
        
        case 8:
        case 9: { *gsIndex = 9; } break;
        
        case 10:
        case 11: { *gsIndex = 10; } break;
        
        case 12:
        case 13:
        case 14: { *gsIndex = 11; } break;
        
        case 15:
        case 16:
        case 17: { *gsIndex = 12; } break;
        
        case 18:
        case 19:
        case 20: { *gsIndex = 13; } break;
    }
    
    return TRUE;
}

b32 bloodySkeletonCreatureAC(utf32 *size, s32 ac[AC_TYPES_COUNT])
{
    for(s32 i = 0; i < AC_TYPES_COUNT; i++)
    { ac[i] = -99; }
    
    if(ls_utf32AreEqual(*size, U"Colossale"_W))         { ac[AC_NATURAL] = 10; }
    else if(ls_utf32AreEqual(*size, U"Mastodontica"_W)) { ac[AC_NATURAL] = 6; }
    else if(ls_utf32AreEqual(*size, U"Enorme"_W))       { ac[AC_NATURAL] = 3; }
    else if(ls_utf32AreEqual(*size, U"Grande"_W))       { ac[AC_NATURAL] = 2; }
    else if(ls_utf32AreEqual(*size, U"Media"_W))        { ac[AC_NATURAL] = 2; }
    else if(ls_utf32AreEqual(*size, U"Piccola"_W))      { ac[AC_NATURAL] = 1; }
    else                                                { ac[AC_NATURAL] = 0; }
    
    return TRUE;
}

b32 bloodySkeletonCreatureAlign(utf32 *oldAlign)
{
    ls_utf32Set(oldAlign, U"NM"_W);
    return TRUE;
}

void bloodySkeletonCreatureType(utf32 *oldType)
{
    ls_utf32Clear(oldType);
    ls_utf32Append(oldType, U"Non Morto");
    
    return;
}

void bloodySkeletonCreatureSubType(utf32 *oldSubType)
{
    //NOTETODO: It works like shit and I'm not sure on how it works.
#if 0
    //NOTE: List of Invalid Subtypes of Alignment
    ls_utf32RmSubstrInList(oldSubType, (utf32 *)alignmentSubtypes, ALIGNMENT_SUBTYPE_COUNT);
    
    //NOTE: List of Invalid Subtypes of Kind
    ls_utf32RmSubstrInList(oldSubType, (utf32 *)raceSubtypes, RACE_SUBTYPES_COUNT);
#endif
    
    return;
}

void bloodySkeletonCreatureDV(CachedPageEntry *page, utf32 *oldType, u64 *DV)
{
    u64 newDV = *DV;
    u16 oldRaceDV = (newDV >> ((HP_MAX_DICE_COUNT-1)*HP_DIE_BITLEN)) & HP_DIE_MASK;
    u16 oldCount  = (oldRaceDV & HP_DIE_COUNT_MASK) >> HP_DIE_FACE_BITLEN;
    u8 fastHealing = (oldCount / 2);
    if(fastHealing == 0) { fastHealing = 1; }
    u8 fastHealingType = HP_Options_Type::HP_Guarigione_Rapida;
    
    //NOTE: Clear all DV except for the first one (Basically remove all class DV and keep only Race DVs
    //      Also clear Race DVs face (lowest 3 bits), since we will convert it to a D8.
    newDV &= 0xFFFFFFF0000001F8;
    
    //NOTE: Set Racial Die Face to HP_Die_D8
    newDV |= (u64)(HP_Die_Face::HP_Die_D8);
    
    //TODO: What happens if the creature already had an option? We are overwriting it like this.
    //      We need to introduce an helper that adds the option (by possibly substituting with a merged version?)
    newDV |= (u64)((fastHealingType & HP_OPTION_TYPE_MASK) << HP_OPTION_TYPE_OFFSET);
    newDV |= (u64)((fastHealing & HP_OPTION_VAL_MASK) << HP_OPTION_VAL_OFFSET);
    
    //NOTE: When changing type, the ability score bonuses to flat hp might change. This is taken care of
    //      in CalculateAndCacheHP, to avoid copy-pasting the same code over and over again in every archetype.
    *DV = newDV;
}

b32 bloodySkeletonCreatureST(s32 totalDV, s32 st[ST_COUNT])
{
    st[ST_CON] = totalDV / 3;
    st[ST_DEX] = totalDV / 3;
    st[ST_WIS] = (totalDV / 2) + 2;
    
    return TRUE;
}

void bloodySkeletonCreatureDefCap(utf32 *old)
{
    ls_utf32Clear(old);
}

void bloodySkeletonCreatureSpeed(utf32 *old)
{
    utf32 toFind = U"Volare "_W;
    utf32 toInsert = U"[SOLO MAGICO] "_W;
    if(old->len+toInsert.len >= old->size) { AssertMsg(FALSE, "speed string not large enough\n"); return; }
    
    s32 index = ls_utf32LeftFind(*old, toFind);
    if(index == -1) { return; }
    
    s32 insertIndex = index + toFind.len;
    ls_utf32InsertSubstr(old, toInsert, insertIndex);
    return;
}

void bloodySkeletonCreatureRD(s32 hitDice, utf32 *oldRD)
{ 
    AssertMsg(oldRD, "Null Old RD string\n");
    
    ls_utf32Clear(oldRD);
    
    const utf32 toAdd = U"5/contundente"_W;
    AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
    if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
    ls_utf32Prepend(oldRD, toAdd);
    
    return;
}

//TODO: This is stupid. We do build the immunity from a packed value, thus we could
//      avoid the stupid text check for an already present immunity.
//      We probably don't want to modify the packed value itself, 
//      since it might create problems (like having too many immunities...)
void bloodySkeletonCreatureImmunities(utf32 *old)
{
    AssertMsg(old, "Null Old Immunities string\n");
    
    ls_utf32Clear(old);
    
    const utf32 toAdd = U"Freddo, "_W;
    AssertMsg(old->len + toAdd.len+2 < old->size, "Insufficient space in immunities string.\n");
    if(old->len + toAdd.len+2 >= old->size) { return; }
    ls_utf32Append(old, toAdd);
    
    const utf32 toAdd2 = U"Tratti dei Non Morti"_W;
    AssertMsg(old->len + toAdd2.len+2 < old->size, "Insufficient space in immunities string.\n");
    if(old->len + toAdd2.len+2 >= old->size) { return; }
    ls_utf32Append(old, toAdd2);
    
    return;
}

void bloodySkeletonCreatureMelee(CachedPageEntry *page, utf32 *melee)
{
    //TODO: We should technically check if the melee attack was already present, and only keep the
    //      stronger one. For now, for semplicity, we'll just append a new attack (without specifying the number)
    
    s32 attackBonus = page->BABval + page->modAS[AS_STR] - 10;
    s32 damageBonus = page->modAS[AS_STR] - 10;
    if(melee->len == 0) { damageBonus *= 1.5; } //NOTE: If only attack
    
    if(melee->len > 0) { ls_utf32Append(melee, U", N artigli "_W); }
    else               { ls_utf32Append(melee, U"N artigli "_W); }
    
    if(attackBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, attackBonus);
    
    ls_utf32Append(melee, U" ("_W);
    
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuta")))       { ls_utf32Append(melee, U"1"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuscola")))    { ls_utf32Append(melee, U"1d2"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Piccola")))      { ls_utf32Append(melee, U"1d3"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Media")))        { ls_utf32Append(melee, U"1d4"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Grande")))       { ls_utf32Append(melee, U"1d6"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Enorme")))       { ls_utf32Append(melee, U"1d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Mastodontica"))) { ls_utf32Append(melee, U"2d6"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Colossale")))    { ls_utf32Append(melee, U"2d8"_W); }
    
    if(damageBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, damageBonus);
    
    ls_utf32AppendChar(melee, ')');
    
    return;
}

b32 bloodySkeletonCreatureSpecAtk(utf32 *spec)
{
    ls_utf32Clear(spec);
    return TRUE;
}

void bloodySkeletonCreatureAS(s32 as[AS_COUNT])
{
    as[AS_DEX] += 2;
    as[AS_CON] = AS_NO_VALUE;
    as[AS_INT] = AS_NO_VALUE;
    as[AS_WIS] = 10;
    as[AS_CHA] = 14;
    return;
}

void bloodySkeletonCreatureBAB(utf32 *old, s32 dv)
{
    ls_utf32Clear(old);
    
    s32 bab = (s32)(((f64)dv)*0.75);
    if(bab > 0) { ls_utf32Append(old, U"+"); }
    ls_utf32AppendInt(old, bab);
}

u32 bloodySkeletonCreatureSkills(u32 skillEntry)
{
    //NOTE: Skeletons loose all skills
    return 0;
}

void bloodySkeletonCreatureTalents(CachedPageEntry *page)
{
    for(s32 i = 0; i < 24; i++)
    { ls_utf32Clear(page->talents + i); }
    
    u32 packed = GetTalentPackedFromName(U"Iniziativa Migliorata"_W);
    
    if(packed != TALENT_NOT_FOUND)
    {
        BuildTalentFromPacked_t(&compendium.codex, packed, &page->talents[0]);
        page->talentEntry[0] = packed;
    }
}

void bloodySkeletonCreatureSpecQual(utf32 *old)
{
    AssertMsg(old, "Null spec qual string pointer\n");
    
    utf32 toAdd = U"N.B. Solo (Str) che potenziano Mischia e Distanza! "_W;
    utf32 toAdd2 = U", Immortale"_W;
    if(old->len + toAdd.len + toAdd2.len >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec qual\n"); return; }
    
    ls_utf32Prepend(old, toAdd);
    ls_utf32Append(old, toAdd2);
    return;
}

void bloodySkeletonCreatureSpecCap(utf32 *old)
{
    AssertMsg(old, "Null spec cap string pointer\n");
    
    utf32 toAdd = U"N.B. Solo (Str) che potenziano Mischia e Distanza!\n"_W;
    utf32 toAdd2 = U"\n\nImmortale (Sop)\nUno scheletro insanguinato viene distrutto quando \U000000E8 "
        "ridotto a 0 punti ferita, ma un'ora dopo torna alla non vita a 1 punto ferita, e la "
        "guarigione rapida gli permette di Guarire. Uno scheletro insanguinato pu\U000000F2 essere distrutto "
        "permanentemente tramite l'energia positiva, se viene ridotto a 0 punti ferita nell'area di "
        "un incantesimo benedizione o consacrare, oppure se sui suoi resti viene versata un'ampolla "
        "di Acqua Santa."_W;
    
    if(old->len + toAdd.len + toAdd2.len >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec cap\n"); return; }
    
    ls_utf32Prepend(old, toAdd);
    ls_utf32Append(old, toAdd2);
    return;
}

ArchetypeDiff BloodySkeletonCreature = {
    U"Scheletro Insanguinato"_W, bloodySkeletonCreatureGS, bloodySkeletonCreatureAS, bloodySkeletonCreatureAC,
    archetypeSensesStub, bloodySkeletonCreatureRD, archetypeResistanceStub, archetypeRIStub,
    bloodySkeletonCreatureSpecAtk, archetypeSizeStub, bloodySkeletonCreatureMelee, bloodySkeletonCreatureAlign,
    bloodySkeletonCreatureType, bloodySkeletonCreatureSubType, bloodySkeletonCreatureDV, bloodySkeletonCreatureST,
    bloodySkeletonCreatureDefCap, bloodySkeletonCreatureSpeed, bloodySkeletonCreatureImmunities,
    bloodySkeletonCreatureBAB, bloodySkeletonCreatureSkills, bloodySkeletonCreatureTalents, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, bloodySkeletonCreatureSpecQual,
    bloodySkeletonCreatureSpecCap, archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};


//----------------------------//
// BURNING SKELETON ARCHETYPE //
//----------------------------//

b32 burningSkeletonCreatureGS(s32 hitDice, s32 *gsIndex, s32 *rm)
{
    //NOTE: Because this creature sets the gs, rather than modify it, we report the index in the gsSet
    switch(hitDice)
    {
        //NOTETODO: The table refers to 1/2 DV, but I haven't seen any creature with half DV ?!?!?
        case 1: { *gsIndex = 4; } break;
        
        case 2:
        case 3: { *gsIndex = 6; } break;
        
        case 4:
        case 5: { *gsIndex = 7; } break;
        
        case 6:
        case 7: { *gsIndex = 8; } break;
        
        case 8:
        case 9: { *gsIndex = 9; } break;
        
        case 10:
        case 11: { *gsIndex = 10; } break;
        
        case 12:
        case 13:
        case 14: { *gsIndex = 11; } break;
        
        case 15:
        case 16:
        case 17: { *gsIndex = 12; } break;
        
        case 18:
        case 19:
        case 20: { *gsIndex = 13; } break;
    }
    
    return TRUE;
}

b32 burningSkeletonCreatureAC(utf32 *size, s32 ac[AC_TYPES_COUNT])
{
    for(s32 i = 0; i < AC_TYPES_COUNT; i++)
    { ac[i] = -99; }
    
    if(ls_utf32AreEqual(*size, U"Colossale"_W))         { ac[AC_NATURAL] = 10; }
    else if(ls_utf32AreEqual(*size, U"Mastodontica"_W)) { ac[AC_NATURAL] = 6; }
    else if(ls_utf32AreEqual(*size, U"Enorme"_W))       { ac[AC_NATURAL] = 3; }
    else if(ls_utf32AreEqual(*size, U"Grande"_W))       { ac[AC_NATURAL] = 2; }
    else if(ls_utf32AreEqual(*size, U"Media"_W))        { ac[AC_NATURAL] = 2; }
    else if(ls_utf32AreEqual(*size, U"Piccola"_W))      { ac[AC_NATURAL] = 1; }
    else                                                { ac[AC_NATURAL] = 0; }
    
    return TRUE;
}

b32 burningSkeletonCreatureAlign(utf32 *oldAlign)
{
    ls_utf32Set(oldAlign, U"NM"_W);
    return TRUE;
}

void burningSkeletonCreatureType(utf32 *oldType)
{
    ls_utf32Clear(oldType);
    ls_utf32Append(oldType, U"Non Morto");
    
    return;
}

void burningSkeletonCreatureSubType(utf32 *oldSubType)
{
    //NOTETODO: It works like shit and I'm not sure on how it works.
#if 0
    //NOTE: List of Invalid Subtypes of Alignment
    ls_utf32RmSubstrInList(oldSubType, (utf32 *)alignmentSubtypes, ALIGNMENT_SUBTYPE_COUNT);
    
    //NOTE: List of Invalid Subtypes of Kind
    ls_utf32RmSubstrInList(oldSubType, (utf32 *)raceSubtypes, RACE_SUBTYPES_COUNT);
#endif
    
    return;
}

void burningSkeletonCreatureDV(CachedPageEntry *page, utf32 *oldType, u64 *DV)
{
    u64 newDV = *DV;
    
    //NOTE: Clear all DV except for the first one (Basically remove all class DV and keep only Race DVs
    //      Also clear Race DVs face (lowest 3 bits), since we will convert it to a D8.
    newDV &= 0xFFFFFFF0000001F8;
    
    //NOTE: Set Racial Die Face to HP_Die_D8
    newDV |= (u64)(HP_Die_Face::HP_Die_D8);
    
    //NOTE: When changing type, the ability score bonuses to flat hp might change. This is taken care of
    //      in CalculateAndCacheHP, to avoid copy-pasting the same code over and over again in every archetype.
    *DV = newDV;
}

b32 burningSkeletonCreatureST(s32 totalDV, s32 st[ST_COUNT])
{
    st[ST_CON] = totalDV / 3;
    st[ST_DEX] = totalDV / 3;
    st[ST_WIS] = (totalDV / 2) + 2;
    
    return TRUE;
}

void burningSkeletonCreatureDefCap(utf32 *old)
{
    ls_utf32Clear(old);
}

void burningSkeletonCreatureSpeed(utf32 *old)
{
    utf32 toFind = U"Volare "_W;
    utf32 toInsert = U"[SOLO MAGICO] "_W;
    if(old->len+toInsert.len >= old->size) { AssertMsg(FALSE, "speed string not large enough\n"); return; }
    
    s32 index = ls_utf32LeftFind(*old, toFind);
    if(index == -1) { return; }
    
    s32 insertIndex = index + toFind.len;
    ls_utf32InsertSubstr(old, toInsert, insertIndex);
    return;
}

void burningSkeletonCreatureRD(s32 hitDice, utf32 *oldRD)
{ 
    AssertMsg(oldRD, "Null Old RD string\n");
    
    ls_utf32Clear(oldRD);
    
    const utf32 toAdd = U"5/contundente"_W;
    AssertMsg(oldRD->len + toAdd.len+2 < oldRD->size, "Insufficient space in rd string.\n");
    if(oldRD->len + toAdd.len+2 >= oldRD->size) { return; }
    ls_utf32Prepend(oldRD, toAdd);
    
    return;
}

//TODO: This is stupid. We do build the immunity from a packed value, thus we could
//      avoid the stupid text check for an already present immunity.
//      We probably don't want to modify the packed value itself, 
//      since it might create problems (like having too many immunities...)
void burningSkeletonCreatureImmunities(utf32 *old)
{
    AssertMsg(old, "Null Old Immunities string\n");
    
    ls_utf32Clear(old);
    
    const utf32 toAdd = U"Fuoco, "_W;
    AssertMsg(old->len + toAdd.len+2 < old->size, "Insufficient space in immunities string.\n");
    if(old->len + toAdd.len+2 >= old->size) { return; }
    ls_utf32Append(old, toAdd);
    
    const utf32 toAdd2 = U"Tratti dei Non Morti"_W;
    AssertMsg(old->len + toAdd2.len+2 < old->size, "Insufficient space in immunities string.\n");
    if(old->len + toAdd2.len+2 >= old->size) { return; }
    ls_utf32Append(old, toAdd2);
    
    return;
}

void burningSkeletonCreatureMelee(CachedPageEntry *page, utf32 *melee)
{
    //TODO: We should technically check if the melee attack was already present, and only keep the
    //      stronger one. For now, for semplicity, we'll just append a new attack (without specifying the number)
    
    s32 attackBonus = page->BABval + page->modAS[AS_STR] - 10;
    s32 damageBonus = page->modAS[AS_STR] - 10;
    if(melee->len == 0) { damageBonus *= 1.5; } //NOTE: If only attack
    
    if(melee->len > 0) { ls_utf32Append(melee, U", N artigli "_W); }
    else               { ls_utf32Append(melee, U"N artigli "_W); }
    
    if(attackBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, attackBonus);
    
    ls_utf32Append(melee, U" ("_W);
    
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuta")))       { ls_utf32Append(melee, U"1"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuscola")))    { ls_utf32Append(melee, U"1d2"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Piccola")))      { ls_utf32Append(melee, U"1d3"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Media")))        { ls_utf32Append(melee, U"1d4"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Grande")))       { ls_utf32Append(melee, U"1d6"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Enorme")))       { ls_utf32Append(melee, U"1d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Mastodontica"))) { ls_utf32Append(melee, U"2d6"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Colossale")))    { ls_utf32Append(melee, U"2d8"_W); }
    
    if(damageBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, damageBonus);
    
    ls_utf32Append(melee, U" + 1d6 Fuoco"_W);
    
    ls_utf32AppendChar(melee, ')');
    
    return;
}

b32 burningSkeletonCreatureSpecAtk(utf32 *spec)
{
    ls_utf32Clear(spec);
    return TRUE;
}

void burningSkeletonCreatureAS(s32 as[AS_COUNT])
{
    as[AS_DEX] += 2;
    as[AS_CON] = AS_NO_VALUE;
    as[AS_INT] = AS_NO_VALUE;
    as[AS_WIS] = 10;
    as[AS_CHA] = 12;
    return;
}

void burningSkeletonCreatureBAB(utf32 *old, s32 dv)
{
    ls_utf32Clear(old);
    
    s32 bab = (s32)(((f64)dv)*0.75);
    if(bab > 0) { ls_utf32Append(old, U"+"); }
    ls_utf32AppendInt(old, bab);
}

u32 burningSkeletonCreatureSkills(u32 skillEntry)
{
    //NOTE: Skeletons loose all skills
    return 0;
}

void burningSkeletonCreatureTalents(CachedPageEntry *page)
{
    for(s32 i = 0; i < 24; i++)
    { ls_utf32Clear(page->talents + i); }
    
    u32 packed = GetTalentPackedFromName(U"Iniziativa Migliorata"_W);
    
    if(packed != TALENT_NOT_FOUND)
    {
        BuildTalentFromPacked_t(&compendium.codex, packed, &page->talents[0]);
        page->talentEntry[0] = packed;
    }
}

void burningSkeletonCreatureSpecQual(utf32 *old)
{
    AssertMsg(old, "Null spec qual string pointer\n");
    
    utf32 toAdd = U"N.B. Solo (Str) che potenziano Mischia e Distanza! "_W;
    utf32 toAdd2 = U", Aura Infuocata, Morte Bruciante"_W;
    if(old->len + toAdd.len + toAdd2.len >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec qual\n"); return; }
    
    ls_utf32Prepend(old, toAdd);
    ls_utf32Append(old, toAdd2);
    return;
}

void burningSkeletonCreatureSpecCap(utf32 *old)
{
    AssertMsg(old, "Null spec cap string pointer\n");
    
    utf32 toAdd = U"N.B. Solo (Str) che potenziano Mischia e Distanza!\n"_W;
    utf32 toAdd2 = U"\n\nMorte bruciante (Sop)\nQuando muore, uno scheletro bruciante esplode "
        "in uno scoppio di fiamme. Tutti coloro che sono adiacenti allo scheletro quando viene "
        "distrutto subiscono 1d6 di danni da fuoco per 2 Dadi Vita posseduti dallo scheletro (minimo 1d6). "
        "Un Tiro Salvezza su Riflessi (CD = 10 + 1/2 dei Dadi Vita dello scheletro + il bonus di Carisma "
        "dello scheletro) dimezza i danni."_W;
    utf32 toAdd3 = U"\n\nAura Infuocata (Str)\nLe creature adiacenti ad uno scheletro bruciante "
        "subiscono 1d6 danni da fuoco all'inizio del loro turno. Chiunque colpisca uno "
        "scheletro bruciante con un colpo senz'armi o un attacco naturale subisce 1d6 danni da fuoco."_W;
    
    if(old->len + toAdd.len + toAdd2.len + toAdd3.len >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec cap\n"); return; }
    
    ls_utf32Prepend(old, toAdd);
    ls_utf32Append(old, toAdd2);
    ls_utf32Append(old, toAdd3);
    return;
}

void burningSkeletonCreatureAura(utf32 *old)
{
    ls_utf32Append(old, U"Aura Infuocata"_W);
    return;
}

ArchetypeDiff BurningSkeletonCreature = {
    U"Scheletro Bruciante"_W, burningSkeletonCreatureGS, burningSkeletonCreatureAS, burningSkeletonCreatureAC,
    archetypeSensesStub, burningSkeletonCreatureRD, archetypeResistanceStub, archetypeRIStub,
    burningSkeletonCreatureSpecAtk, archetypeSizeStub, burningSkeletonCreatureMelee, burningSkeletonCreatureAlign,
    burningSkeletonCreatureType, burningSkeletonCreatureSubType, burningSkeletonCreatureDV, burningSkeletonCreatureST,
    burningSkeletonCreatureDefCap, burningSkeletonCreatureSpeed, burningSkeletonCreatureImmunities,
    burningSkeletonCreatureBAB, burningSkeletonCreatureSkills, burningSkeletonCreatureTalents, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, burningSkeletonCreatureSpecQual,
    burningSkeletonCreatureSpecCap, archetypeLangStub, burningSkeletonCreatureAura, archetypeWeakStub,
};



//-------------------------//
//  FAST ZOMBIE ARCHETYPE  //
//-------------------------//

b32 fastZombieCreatureGS(s32 hitDice, s32 *gsIndex, s32 *rm)
{
    //NOTE: Because this creature sets the gs, rather than modify it, we report the index in the gsSet
    switch(hitDice)
    {
        //NOTETODO: The table refers to 1/2 DV, but I haven't seen any creature with half DV ?!?!?
        case 1:  { *gsIndex = 2; } break;
        
        case 2:  { *gsIndex = 4; } break;
        
        case 3:
        case 4:  { *gsIndex = 5; } break;
        
        case 5:
        case 6:  { *gsIndex = 6; } break;
        
        case 7:
        case 8:  { *gsIndex = 7; } break;
        
        case 9:
        case 10: { *gsIndex = 8; } break;
        
        case 11:
        case 12: { *gsIndex = 9; } break;
        
        case 13:
        case 14:
        case 15:
        case 16: { *gsIndex = 10; } break;
        
        case 17:
        case 18:
        case 19:
        case 20: { *gsIndex = 11; } break;
        
        case 21:
        case 22:
        case 23:
        case 24: { *gsIndex = 12; } break;
        
        case 25:
        case 26:
        case 27:
        case 28: { *gsIndex = 13; } break;
    }
    
    return TRUE;
}

b32 fastZombieCreatureAC(utf32 *size, s32 ac[AC_TYPES_COUNT])
{
    for(s32 i = 0; i < AC_TYPES_COUNT; i++)
    { ac[i] = -99; }
    
    if(ls_utf32AreEqual(*size, U"Colossale"_W))         { ac[AC_NATURAL] = 11; }
    else if(ls_utf32AreEqual(*size, U"Mastodontica"_W)) { ac[AC_NATURAL] = 7; }
    else if(ls_utf32AreEqual(*size, U"Enorme"_W))       { ac[AC_NATURAL] = 4; }
    else if(ls_utf32AreEqual(*size, U"Grande"_W))       { ac[AC_NATURAL] = 3; }
    else if(ls_utf32AreEqual(*size, U"Media"_W))        { ac[AC_NATURAL] = 2; }
    else if(ls_utf32AreEqual(*size, U"Piccola"_W))      { ac[AC_NATURAL] = 1; }
    else                                                { ac[AC_NATURAL] = 0; }
    
    return TRUE;
}

b32 fastZombieCreatureAlign(utf32 *oldAlign)
{
    ls_utf32Set(oldAlign, U"NM"_W);
    return TRUE;
}

void fastZombieCreatureType(utf32 *oldType)
{
    ls_utf32Clear(oldType);
    ls_utf32Append(oldType, U"Non Morto");
    
    return;
}

void fastZombieCreatureSubType(utf32 *oldSubType)
{
    //NOTETODO: It works like shit and I'm not sure on how it works.
#if 0
    //NOTE: List of Invalid Subtypes of Alignment
    ls_utf32RmSubstrInList(oldSubType, (utf32 *)alignmentSubtypes, ALIGNMENT_SUBTYPE_COUNT);
    
    //NOTE: List of Invalid Subtypes of Kind
    ls_utf32RmSubstrInList(oldSubType, (utf32 *)raceSubtypes, RACE_SUBTYPES_COUNT);
#endif
    
    return;
}

void fastZombieCreatureDV(CachedPageEntry *page, utf32 *oldType, u64 *DV)
{
    u64 newDV = *DV;
    u16 oldRaceDV = (newDV >> ((HP_MAX_DICE_COUNT-1)*HP_DIE_BITLEN)) & HP_DIE_MASK;
    u16 oldCount  = (oldRaceDV & HP_DIE_COUNT_MASK) >> HP_DIE_FACE_BITLEN;
    
    if(ls_utf32AreEqual(page->size, U"Colossale"_W))         { oldCount += 11; }
    else if(ls_utf32AreEqual(page->size, U"Mastodontica"_W)) { oldCount += 6; }
    else if(ls_utf32AreEqual(page->size, U"Enorme"_W))       { oldCount += 4; }
    else if(ls_utf32AreEqual(page->size, U"Grande"_W))       { oldCount += 2; }
    else if(ls_utf32AreEqual(page->size, U"Media"_W))        { oldCount += 1; }
    else if(ls_utf32AreEqual(page->size, U"Piccola"_W))      { oldCount += 1; }
    else                                                     { oldCount += 0; }
    
    oldCount &= 0x003F;
    
    //NOTE: Clear all DV except for the first one (Basically remove all class DV and keep only Race DVs
    //      Also clear Race DVs face (lowest 3 bits), since we will convert it to a D8.
    newDV &= 0xFFFFFFF0000001F8;
    
    //NOTE: Set Racial Die Face to HP_Die_D8, and set new DV Count.
    newDV |= (u64)(HP_Die_Face::HP_Die_D8);
    newDV |= (u64)(oldCount << HP_DIE_FACE_BITLEN);
    
    //NOTE: When changing type, the ability score bonuses to flat hp might change. This is taken care of
    //      in CalculateAndCacheHP, to avoid copy-pasting the same code over and over again in every archetype.
    *DV = newDV;
}

b32 fastZombieCreatureST(s32 totalDV, s32 st[ST_COUNT])
{
    st[ST_CON] = totalDV / 3;
    st[ST_DEX] = totalDV / 3;
    st[ST_WIS] = (totalDV / 2) + 2;
    
    return TRUE;
}

void fastZombieCreatureDefCap(utf32 *old)
{
    ls_utf32Clear(old);
}

void fastZombieCreatureSpeed(utf32 *old)
{
    //TODO: Because maneuverability goes down, the bonus to the flight ability changes.
    //      Zombies loose all abilities, so I'll leave it to the GM to calculate it if necessary
    //      But showing it properly would be better.
    const s32 manuvCount = 8;
    const utf32 patterns[manuvCount] = { U"(scarsa)"_W, U"(media)"_W, U"(buona)"_W, U"(perfetta)"_W,
        U"(Scarsa)"_W, U"(Media)"_W, U"(Buona)"_W, U"(Perfetta)"_W,
    };
    utf32 replacement = U"(maldestra)"_W;
    
    for(s32 i = 0; i < manuvCount; i++)
    {
        if(old->len - patterns[i].len + replacement.len >= old->size)
        { AssertMsg(FALSE, "speed string not large enough\n"); return; }
        
        if(ls_utf32Replace(old, patterns[i], replacement) == TRUE) { return; }
    }
    
    return;
}

//TODO: This is stupid. We do build the immunity from a packed value, thus we could
//      avoid the stupid text check for an already present immunity.
//      We probably don't want to modify the packed value itself, 
//      since it might create problems (like having too many immunities...)
void fastZombieCreatureImmunities(utf32 *old)
{
    AssertMsg(old, "Null Old Immunities string\n");
    
    ls_utf32Clear(old);
    
    const utf32 toAdd2 = U"Tratti dei Non Morti"_W;
    AssertMsg(old->len + toAdd2.len+2 < old->size, "Insufficient space in immunities string.\n");
    if(old->len + toAdd2.len+2 >= old->size) { return; }
    ls_utf32Append(old, toAdd2);
    
    return;
}

void fastZombieCreatureMelee(CachedPageEntry *page, utf32 *melee)
{
    //TODO: We should technically check if the melee attack was already present, and only keep the
    //      stronger one. For now, for semplicity, we'll just append a new attack (without specifying the number)
    
    s32 attackBonus = page->BABval + page->modAS[AS_STR] - 10;
    s32 damageBonus = page->modAS[AS_STR] - 10;
    if(melee->len == 0) { damageBonus *= 1.5; } //NOTE: If only attack
    
    if(melee->len > 0) { ls_utf32Append(melee, U", schianto "_W); }
    else               { ls_utf32Append(melee, U"schianto  "_W); }
    
    if(attackBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, attackBonus);
    
    ls_utf32Append(melee, U" ("_W);
    
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuta")))       { ls_utf32Append(melee, U"1d2"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuscola")))    { ls_utf32Append(melee, U"1d3"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Piccola")))      { ls_utf32Append(melee, U"1d4"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Media")))        { ls_utf32Append(melee, U"1d6"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Grande")))       { ls_utf32Append(melee, U"1d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Enorme")))       { ls_utf32Append(melee, U"2d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Mastodontica"))) { ls_utf32Append(melee, U"2d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Colossale")))    { ls_utf32Append(melee, U"3d6"_W); }
    
    if(damageBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, damageBonus);
    
    ls_utf32AppendChar(melee, ')');
    
    return;
}

b32 fastZombieCreatureSpecAtk(utf32 *spec)
{
    AssertMsg(spec, "Null input string pointer\n");
    ls_utf32Clear(spec);
    
    const utf32 toAdd = U"Colpi Rapidi"_W;
    if(spec->len + toAdd.len >= spec->size) { AssertMsg(FALSE, "Not enough space in special atk\n"); return FALSE; }
    
    ls_utf32Append(spec, toAdd);
    return TRUE;
}

void fastZombieCreatureAS(s32 as[AS_COUNT])
{
    as[AS_STR] += 2;
    as[AS_DEX] += 2;
    as[AS_CON] = AS_NO_VALUE;
    as[AS_INT] = AS_NO_VALUE;
    as[AS_WIS] = 10;
    as[AS_CHA] = 10;
    return;
}

void fastZombieCreatureBAB(utf32 *old, s32 dv)
{
    ls_utf32Clear(old);
    
    s32 bab = (s32)(((f64)dv)*0.75);
    if(bab > 0) { ls_utf32Append(old, U"+"); }
    ls_utf32AppendInt(old, bab);
}

u32 fastZombieCreatureSkills(u32 skillEntry)
{
    //NOTE: Zombies loose all skills
    return 0;
}

void fastZombieCreatureTalents(CachedPageEntry *page)
{
    for(s32 i = 0; i < 24; i++)
    { ls_utf32Clear(page->talents + i); }
    
    u32 packed = GetTalentPackedFromName(U"Robustezza"_W);
    
    if(packed != TALENT_NOT_FOUND)
    {
        BuildTalentFromPacked_t(&compendium.codex, packed, &page->talents[0]);
        page->talentEntry[0] = packed;
    }
}

void fastZombieCreatureSpecQual(utf32 *old)
{
    utf32 toAdd = U"N.B. Solo (Str) che potenziano Mischia e Distanza! "_W;
    if(old->len + toAdd.len >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec qual\n"); return; }
    
    ls_utf32Prepend(old, toAdd);
    return;
}

void fastZombieCreatureSpecCap(utf32 *old)
{
    utf32 toAdd = U"N.B. Solo (Str) che potenziano Mischia e Distanza!\n"_W;
    utf32 toAdd2 = U"\n\nColpi rapidi (Str)\nQuando uno zombi rapido effettua un'azione "
        "di attacco completo pu\U000000F2 effettuare un ulteriore attacco di schianto al "
        "suo Bonus di Attacco Base pi\U000000F9 alto."_W;
    
    if(old->len + toAdd.len + toAdd2.len >= old->size)
    { AssertMsg(FALSE, "Not enough space in spec cap\n"); return; }
    
    ls_utf32Prepend(old, toAdd);
    ls_utf32Append(old, toAdd2);
    return;
}

ArchetypeDiff FastZombieCreature = {
    U"Zombi Rapido"_W, fastZombieCreatureGS, fastZombieCreatureAS, fastZombieCreatureAC, archetypeSensesStub,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, fastZombieCreatureSpecAtk,
    archetypeSizeStub, fastZombieCreatureMelee, fastZombieCreatureAlign, fastZombieCreatureType, fastZombieCreatureSubType,
    fastZombieCreatureDV, fastZombieCreatureST, fastZombieCreatureDefCap, fastZombieCreatureSpeed, fastZombieCreatureImmunities,
    fastZombieCreatureBAB, fastZombieCreatureSkills, fastZombieCreatureTalents, archetypeEnvStub, archetypeOrgStub,
    archetypeTreasureStub, fastZombieCreatureSpecQual, fastZombieCreatureSpecCap, archetypeLangStub, archetypeAuraStub,
    archetypeWeakStub,
};



//-------------------------//
//    VAMPIRE ARCHETYPE    //
//-------------------------//

b32 vampireCreatureGS(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{ *gsDiff = 2; *rmDiff = 0; return FALSE; }

void vampireCreatureAS(s32 as[AS_COUNT])
{ 
    as[AS_STR] += 6;
    as[AS_DEX] += 4;
    as[AS_CON]  = AS_NO_VALUE;
    as[AS_INT] += 2;
    as[AS_WIS] += 2;
    as[AS_CHA] += 4;
    return;
}

b32 vampireCreatureAC(utf32 *size, s32 ac[AC_TYPES_COUNT])
{
    ac[AC_NATURAL] += 6;
    return FALSE;
}

void vampireCreatureSenses(utf32 *old)
{ 
    AssertMsg(old, "Null senses string pointer\n");
    
    const utf32 check = U"Scurovisione 18 m"_W;
    const utf32 toAdd = U"Scurovisione 18 m, "_W;
    CompendiumPrependStringIfMissing(old, check, toAdd);
    
    return;
}

void vampireCreatureRD(s32 hitDice, utf32 *old)
{ 
    AssertMsg(old, "Null rd string pointer\n");
    
    const utf32 check = U"RD 10/argento e magia"_W;
    const utf32 toAdd = U"RD 10/argento e magia, "_W;
    CompendiumPrependStringIfMissing(old, check, toAdd);
    
    return;
}

void vampireCreatureResistance(s32 hitDice, u64 orig, utf32 *old)
{ 
    AssertMsg(old, "Null resistance string pointer\n");
    AppendResistanceStringIfNotPresent(orig, RES_ELEC_TYPE, 10, U"Elettricit\U000000E0 10"_W, old);
    AppendResistanceStringIfNotPresent(orig, RES_COLD_TYPE, 10, U"Freddo 10"_W, old);
    AppendResistanceStringIfNotPresent(orig, RES_CHAN_TYPE, 4, U"Resistenza ad Incanalare +4"_W, old);
    return;
}

b32 vampireCreatureSpecAtk(utf32 *spec)
{
    AssertMsg(spec, "Null spec atk string pointer\n");
    
    CompendiumPrependStringIfMissing(spec, U"Creare Progenie"_W, U"Creare Progenie, "_W);
    CompendiumPrependStringIfMissing(spec, U"Dominare"_W, U"Dominare, "_W);
    CompendiumPrependStringIfMissing(spec, U"Figli della Notte"_W, U"Figli della Notte, "_W);
    CompendiumPrependStringIfMissing(spec, U"Risucchio di Energia"_W, U"Risucchio di Energia, "_W);
    CompendiumPrependStringIfMissing(spec, U"Risucchio di Sangue"_W, U"Risucchio di Sangue, "_W);
    
    return FALSE;
}

void vampireCreatureMelee(CachedPageEntry *page, utf32 *melee)
{ 
    //TODO: We should technically check if the melee attack was already present, and only keep the
    //      stronger one. For now, for semplicity, we'll just append a new attack (without specifying the number)
    s32 attackBonus = page->BABval + page->modAS[AS_STR] - 10;
    s32 damageBonus = page->modAS[AS_STR] - 10;
    if(melee->len == 0) { damageBonus *= 1.5; } //NOTE: If only attack
    
    if(melee->len > 0) { ls_utf32Append(melee, U", schianto "_W); }
    else               { ls_utf32Append(melee, U"schianto  "_W); }
    
    if(attackBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, attackBonus);
    
    ls_utf32Append(melee, U" ("_W);
    
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuta")))       { ls_utf32Append(melee, U"1"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Minuscola")))    { ls_utf32Append(melee, U"1d2"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Piccola")))      { ls_utf32Append(melee, U"1d3"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Media")))        { ls_utf32Append(melee, U"1d4"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Grande")))       { ls_utf32Append(melee, U"1d6"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Enorme")))       { ls_utf32Append(melee, U"1d8"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Mastodontica"))) { ls_utf32Append(melee, U"2d6"_W); }
    if(ls_utf32AreEqual(page->size, ls_utf32Constant(U"Colossale")))    { ls_utf32Append(melee, U"2d8"_W); }
    
    if(damageBonus >= 0)      { ls_utf32AppendChar(melee, '+'); }
    ls_utf32AppendInt(melee, damageBonus);
    
    ls_utf32AppendChar(melee, ')');
    
    return;
}

b32 vampireCreatureAlign(utf32 *align)
{
    AssertMsg(align, "Null align string\n");
    AssertMsg(align->len > 0, "Alignment is empty\n");
    
    //TODONOTE: This is potentially dangerous and shit, but I only need to change the
    //          alignment to evil, and the rest can stay the same.
    align->data[1] = (u32)'M';
    return TRUE;
}

void vampireCreatureType(utf32 *type)
{ 
    AssertMsg(type, "Null type string pointer\n");
    
    ls_utf32Clear(type);
    ls_utf32Append(type, U"Non Morto");
    return;
}

void vampireCreatureSubType(utf32 *subtype)
{
    AssertMsg(subtype, "Null subtype string pointer\n");
    
    //TODO: This requires more work. It's shit.
    CompendiumPrependStringIfMissing(subtype, U"Potenziato"_W, U"Potenziato, "_W);
    
    return;
}

void vampireCreatureDV(CachedPageEntry *page, utf32 *oldType, u64 *DV)
{
    u64 newDV = ChangeRacialDVType(*DV, HP_Die_Face::HP_Die_D8);
    newDV = AddHPOptionIfMissing(newDV, HP_Options_Type::HP_Guarigione_Rapida, 5);
    *DV = newDV;
    return;
}

void vampireCreatureDefCap(utf32 *old)
{
    const utf32 check = U"Tratti dei Non Morti"_W;
    const utf32 toAdd = U"Tratti dei Non Morti, "_W;
    CompendiumPrependStringIfMissing(old, check, toAdd);
    return;
}

u32 vampireCreatureSkills(u32 entry)
{
    //NOTE: Vampires gain +8 racial bonus on those 4 skills.
    
    SkillType entryType = SkillType(entry & SKILL_TYPE_MASK);
    switch(entryType)
    {
        case SkillType::Furtivit:
        case SkillType::Intuizione:
        case SkillType::Percezione:
        case SkillType::Raggirare:  { return ChangeBonusToSkill(entry, 8); } break;
    }
    
    return entry;
}

void vampireCreatureTalents(CachedPageEntry *page)
{
    AssertMsg(page, "Null cached page pointer\n");
    
    CompendiumAddTalentIfItFits(page, U"Allerta"_W, TRUE);
    CompendiumAddTalentIfItFits(page, U"Iniziativa Migliorata"_W, TRUE);
    CompendiumAddTalentIfItFits(page, U"Riflessi Fulminei"_W, TRUE);
    CompendiumAddTalentIfItFits(page, U"Riflessi in Combattimento"_W, TRUE);
    CompendiumAddTalentIfItFits(page, U"Robustezza"_W, TRUE);
    CompendiumAddTalentIfItFits(page, U"Schivare"_W, TRUE);
    
    return;
}

void vampireCreatureSpecQual(utf32 *old)
{
    AssertMsg(old, "Null spec qual string pointer\n");
    
    const utf32 racialBonus = U"Bonus Razziale +8 a prove di Furtivit\U000000E0, Intuizione, "
        "Percezione e Raggirare, "_W;
    
    CompendiumPrependStringIfMissing(old, U"Cambiare Forma"_W, U"Cambiare Forma, "_W);
    CompendiumPrependStringIfMissing(old, U"Forma Gassosa"_W, U"Forma Gassosa, "_W);
    CompendiumPrependStringIfMissing(old, U"Movimenti del Ragno"_W, U"Movimenti del Ragno, "_W);
    CompendiumPrependStringIfMissing(old, U"Senz'Ombra"_W, U"Senz'Ombra, "_W);
    CompendiumPrependStringIfMissing(old, racialBonus, racialBonus);
    
    return;
}

void vampireCreatureSpecCap(utf32 *old)
{ 
    AssertMsg(old, "Null spec cap string pointer\n");
    
    const utf32 changeForm = U"Cambiare Forma (Sop)\nUn vampiro pu\U000000F2 usare "
        "cambiare forma per assumere la forma di Pipistrello o Lupo Crudele, come Forma Ferina II\n\n"_W;
    
    const utf32 gasForm = U"Forma Gassosa (Sop)\nCome azione standard, un vampiro pu\U000000F2 "
        "assumere Forma Gassosa a volont\U000000E0 (livello dell'incantatore 5), ma pu\U000000F2 rimanere "
        "gassoso indefinitamente e ha una velocit\U000000E0 di Volare di 6 metri "
        "con manovrabilit\U000000E0 perfetta\n\n"_W;
    
    const utf32 spiderMove = U"Movimenti del Ragno (Str)\nUn vampiro pu\U000000F2 Scalare "
        "superfici verticali come se fosse sotto l'effetto dell'incantesimo Movimenti del Ragno\n\n"_W;
    
    const utf32 shadowless = U"Senz'Ombra (Str)\nUn vampiro non proietta ombra e "
        "non si riflette negli specchi\n\n"_W;
    
    const utf32 createChildren = U"Creare Progenie (Sop)\nUn vampiro pu\U000000F2 creare progenie "
        "da coloro che uccide con il risucchio di sangue o il risucchio di energia, sempre che "
        "la creatura uccisa sia dello stesso tipo della creatura base del vampiro. La vittima risorge "
        "dalla morte come vampiro in 1d4 giorni. Questo vampiro \U000000E8 sotto il comando del suo creatore e "
        "ne rimane schiavo fino alla distruzione del suo padrone. Un vampiro pu\U000000F2 avere come progenie "
        "schiava un numero di individui non superiore al doppio dei suoi Dadi Vita; qualsiasi progenie "
        "creata che ecceda questo limite diventa un non morto con libero arbitrio. Un vampiro pu\U000000F2 liberare "
        "una progenie schiava per asservirne una nuova, ma una volta libero, un vampiro o una progenie "
        "vampirica non pu\U000000F2 essere schiavizzata nuovamente\n\n"_W;
    
    const utf32 dominate = U"Dominare (Sop)\nUn vampiro pu\U000000F2 piegare la volont\U000000E0 di un "
        "umanoide come azione standard. Il bersaglio deve superare un Tiro Salvezza sulla "
        "Volont\U000000E0 o cadere immediatamente sotto la sua influenza, come per l'incantesimo Dominare "
        "Persone (livello dell'incantatore 12). Questa capacit\U000000E0 ha un raggio di 9 metri. "
        "A discrezione del GM, alcuni vampiri sono in grado di influenzare tipi diversi di "
        "creature con questo potere\n\n"_W;
    
    const utf32 childrenOfNight = U"Figli della Notte (Sop)\nUna volta al giorno, un vampiro pu\U000000F2 "
        "richiamare 1d6+1 sciami di ratti, 1d4+1 sciami di pipistrelli o 2d6 lupi come azione standard "
        "(se la creatura base non \U000000E8 terrestre, questo potere pu\U000000F2 evocare altre creature di "
        "potere similare). Queste creature arrivano in 2d6 round e servono il vampiro per massimo 1 ora.\n\n"_W;
    
    const utf32 drain = U"Risucchio di Energia (Sop)\nUna creatura colpita dallo schianto "
        "di un vampiro (o da altri attacchi naturali) ottiene due livelli negativi. "
        "Questa capacit\U000000E0 si attiva una volta per round, indipendentemente dal numero "
        "di attacchi che un vampiro pu\U000000F2 effettuare\n\n"_W;
    
    const utf32 bloodSuck = U"Risucchio di Sangue (Sop)\nUn vampiro pu\U000000F2 succhiare sangue "
        "lottando col suo avversario; se il vampiro stabilisce e mantiene una presa, "
        "risucchia il sangue, infliggendo 1d4 danni alla Costituzione. Il vampiro guarisce "
        "di 5 punti ferita o ne ottiene 5 temporanei per 1 ora (fino ad un massimo di punti "
        "ferita temporanei pari al normale numero di punti ferita totali del vampiro) "
        "ogni round in cui risucchia sangue.\n\n"_W;
    
    const utf32 vampireWeakness = U"Debolezze del Vampiro\nI vampiri non tollerano l'odore forte dell'aglio "
        "e non entreranno volontariamente in un'area che ne \U000000E8 impregnata. Allo stesso modo, rifuggono "
        "gli specchi o i simboli sacri mostrati con fermezza. Queste cose non danneggiano il vampiro, "
        "possono solo tenerlo a bada. Un vampiro deve stare ad almeno 1,5 metri da uno specchio o un "
        "simbolo sacro, e non pu\U000000F2 toccare o sferrare attacchi in mischia contro chi li usa. Tenere a bada "
        "un vampiro richiede un'azione standard. Dopo 1 round, un vampiro pu\U000000F2 superare la sua repulsione "
        "per l'oggetto e pu\U000000F2 tentare di farlo ogni round superando un "
        "Tiro Salvezza su Volont\U000000E0 con CD 25."
        "\nI vampiri non possono entrare nelle case private a meno che non vengano invitati da qualcuno "
        "che ne ha l'autorit\U000000E0.\nRiducendo un vampiro a 0 Punti Ferita o meno lo si rende Inabile, ma non "
        "sempre lo si distrugge (vedi Guarigione Rapida). Tuttavia, certi attacchi \"uccidono\" i vampiri. "
        "Un vampiro esposto alla luce solare diretta diviene Barcollante nel primo round di esposizione "
        "e viene distrutto completamente nel round successivo se non riesce a scappare. Ogni round di "
        "immersione in acqua corrente infligge ad un vampiro danni pari a 1/3 dei suoi punti "
        "ferita massimi; un vampiro ridotto a 0 punti ferita in questa maniera viene distrutto. "
        "Piantare un Paletto di Legno nel cuore di un vampiro Indifeso lo uccide sul colpo "
        "(questa \U000000E8 un'azione di round completo). Comunque, questi pu\U000000F2 tornare in "
        "vita se il paletto viene rimosso, a meno che la sua testa venga mozzata e cosparsa di Acqua Santa.\n\n"_W;
    
    CompendiumPrependStringIfMissing(old, changeForm, changeForm);
    CompendiumPrependStringIfMissing(old, gasForm, gasForm);
    CompendiumPrependStringIfMissing(old, spiderMove, spiderMove);
    CompendiumPrependStringIfMissing(old, shadowless, shadowless);
    
    CompendiumPrependStringIfMissing(old, createChildren, createChildren);
    CompendiumPrependStringIfMissing(old, dominate, dominate);
    CompendiumPrependStringIfMissing(old, childrenOfNight, childrenOfNight);
    CompendiumPrependStringIfMissing(old, drain, drain);
    CompendiumPrependStringIfMissing(old, bloodSuck, bloodSuck);
    
    CompendiumPrependStringIfMissing(old, vampireWeakness, vampireWeakness);
    
    ls_utf32TrimWhitespaceRight(old);
    
    return;
}

void vampireCreatureWeak(utf32 *old)
{
    AssertMsg(old, "Null weakness string pointer\n");
    
    CompendiumPrependStringIfMissing(old, U"Debolezze del Vampiro"_W, U"Debolezze del Vampiro, "_W);
    
    return;
}

ArchetypeDiff VampireCreature = {
    U"Vampiro"_W, vampireCreatureGS, vampireCreatureAS, vampireCreatureAC, vampireCreatureSenses,
    vampireCreatureRD, vampireCreatureResistance, archetypeRIStub, vampireCreatureSpecAtk,
    archetypeSizeStub, vampireCreatureMelee, vampireCreatureAlign, vampireCreatureType,
    vampireCreatureSubType, vampireCreatureDV, archetypeSTStub, vampireCreatureDefCap, archetypeSpeedStub,
    archetypeImmunitiesStub, archetypeBABStub, vampireCreatureSkills, vampireCreatureTalents, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, vampireCreatureSpecQual, vampireCreatureSpecCap,
    archetypeLangStub, archetypeAuraStub, vampireCreatureWeak,
};