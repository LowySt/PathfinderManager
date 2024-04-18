#if 0
//NOTE: Stubbed Archetype
ArchetypeDiff stub = {
    U"Name"_W, archetypeIsCompatibleStub, archetypeGSStub, archetypeASStub, archetypeACStub, archetypeSensesStub,
    archetypeRDStub, archetypeResistanceStub, archetypeRIStub, archetypeSpecAtkStub,
    archetypeSizeStub, archetypeMeleeStub, archetypeAlignStub, archetypeTypeStub,
    archetypeSubTypeStub, archetypeDVStub, archetypeSTStub, archetypeDefCapStub, archetypeSpeedStub,
    archetypeImmunitiesStub, archetypeBABStub, archetypeSkillsStub, archetypeTalentsStub, 
    archetypeEnvStub, archetypeOrgStub, archetypeTreasureStub, archetypeSpecQualStub, archetypeSpecCapStub,
    archetypeLangStub, archetypeAuraStub, archetypeWeakStub,
};
#endif

//-------------------------//
//   STUBS FOR ARCHETYPE   //
//-------------------------//

b32 archetypeIsCompatibleStub(CachedPageEntry *page)
{ return TRUE; }

b32 archetypeGSStub(s32 hitDice, s32 *gsDiff, s32 *rmDiff)
{ *gsDiff = 0; *rmDiff = 0; return FALSE; }

void archetypeASStub(s32 as[AS_COUNT])
{ return; }

b32 archetypeACStub(utf32 *size, s32 ac[AC_TYPES_COUNT])
{ return FALSE; }

void archetypeSensesStub(utf32 *old)
{ return; }

void archetypeRDStub(s32 hitDice, utf32 *old)
{ return; }

void archetypeResistanceStub(s32 hitDice, u64 orig, utf32 *old)
{ return; }

void archetypeRIStub(utf32 gs, utf32 *ri)
{ return; }

b32 archetypeSpecAtkStub(utf32 *spec)
{ return FALSE; }

void archetypeSizeStub(utf32 *size)
{ return; }

void archetypeMeleeStub(CachedPageEntry *page, utf32 *melee)
{ return; }

b32 archetypeAlignStub(utf32 *align)
{ return FALSE; }

void archetypeTypeStub(utf32 *type)
{ return; }

void archetypeSubTypeStub(utf32 *subtype)
{ return; }

void archetypeDVStub(CachedPageEntry *, utf32 *oldType, u64 *DV)
{ return; }

b32 archetypeSTStub(s32 totalDV, s32 st[ST_COUNT])
{ return FALSE; }

void archetypeDefCapStub(utf32 *old)
{ return; }

void archetypeSpeedStub(utf32 *old)
{ return; }

void archetypeImmunitiesStub(utf32 *old)
{ return; }

void archetypeBABStub(utf32 *old, s32 dv)
{ return; }

u32 archetypeSkillsStub(u32 skillEntry)
{ return skillEntry; }

void archetypeTalentsStub(CachedPageEntry *)
{ return; }

void archetypeEnvStub(utf32 *old)
{ return; }

void archetypeOrgStub(utf32 *old)
{ return; }

void archetypeTreasureStub(utf32 *old)
{ return; }

void archetypeSpecQualStub(utf32 *old)
{ return; }

void archetypeSpecCapStub(utf32 *old)
{ return; }

void archetypeLangStub(utf32 *old)
{ return; }

void archetypeAuraStub(utf32 *old)
{ return; }

void archetypeWeakStub(utf32 *old)
{ return; }

//NOTE: All archetypes code is in other files, but included directly here.
#include "CompendiumArchetypesSimple.cpp"
#include "CompendiumArchetypesAcquired.cpp"
#include "CompendiumArchetypesInherited.cpp"

//-------------------------//
//  ARCHETYPE APPLICATION  //
//-------------------------//

//TODO: Right now there's no enforcement on this array being all initialized, which is bad.
//      Solution would be to make a static compile time check for StaticArray that enforces size?
//      Or make a SizedArray that enforces initialization
ArchetypeDiff allArchetypeDiffs[] = {
    AdvancedCreature, CelestialCreature, FiendishCreature, EntropicCreature, ResoluteCreature, 
    GiantCreature, FungoidCreature, ShadowCreature, FleshwarpedCreature, DegenerateCreature,
    
    SkeletonCreature, BloodySkeletonCreature, BurningSkeletonCreature,
    
    ZombieCreature, FastZombieCreature,
    
    VampireCreature,
    
    FungalCreature, ExoskeletonCreature
};

static_assert((sizeof(allArchetypeDiffs) / sizeof(ArchetypeDiff)) == MAX_ARCHETYPES,
              "All Archetype Diffs is not exhaustive\n");

b32 CompendiumOpenArchetypeWindow(UIContext *c, void *user)
{
    compendium.arch.isChoosingArchetype = !compendium.arch.isChoosingArchetype;
    return FALSE;
}

b32 CompendiumSelectArchetype(UIContext *c, void *user)
{
    s32 index = (s32)((s64)user);
    
    ArchetypeDiff selectedDiff = allArchetypeDiffs[index];
    
    //NOTE: This AppendUnique makes sure the same Archetype can't be selected more than once.
    //      And that up to 4 archetypes are selectable.
    if(!ls_staticArrayIsFull(compendium.appliedArchetypes) && selectedDiff.isCompatible(&cachedPage))
    { ls_staticArrayAppendUnique(&compendium.appliedArchetypes, selectedDiff); }
    
    GetPageEntryAndCache(compendium.pageIndex, -1, &cachedPage, NULL);
    compendium.arch.isChoosingArchetype = FALSE;
    return FALSE;
}



//----------------------------//
// ARCHETYPE DIFF APPLICATION //
//----------------------------//

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
        b32 setsGS = curr->gs(hitDice, &gsDiff, &rmDiff);
        
        if(setsGS) { CompendiumSetGS(gsDiff, rmDiff, newGS, newPE); return; }
        
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

b32 CompendiumApplyAllArchetypeAC(utf32 *size, s32 ac[AC_TYPES_COUNT])
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        b32 replaces = curr->armorClass(size, ac);
        if(replaces == TRUE) { return TRUE; }
    }
    
    return FALSE;
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

void CompendiumApplyAllArchetypeResistances(s32 hitDice, u64 orig, utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->resistances(hitDice, orig, old);
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
        b32 hasSet = curr->specialAtk(spec);
        if(hasSet) { return; }
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

//NOTETODO: This currently applies after all melee calculations have been performed.
//          So all values must be already computed. Maybe we want an intermediate step
//          to know if there are any natural attacks already? And the kind of those natural attacks...
void CompendiumApplyAllArchetypeMelee(CachedPageEntry *page, utf32 *melee)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->melee(page, melee);
    }
}

void CompendiumApplyAllArchetypeAlign(utf32 *align)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        b32 setsAlign = curr->align(align);
        if(setsAlign) { return; }
    }
}

void CompendiumApplyAllArchetypeTypes(utf32 *type)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->type(type);
    }
}

void CompendiumApplyAllArchetypeSubTypes(utf32 *subtype)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->subtype(subtype);
    }
}

void CompendiumApplyAllArchetypeDV(CachedPageEntry *page, utf32 *oldType, u64 *DV)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->DV(page, oldType, DV);
    }
}

b32 CompendiumApplyAllArchetypeST(s32 DV, s32 st[ST_COUNT])
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        b32 hasReplaced = curr->st(DV, st);
        if(hasReplaced) { return TRUE; }
    }
    
    return FALSE;
}

void CompendiumApplyAllArchetypeDefCap(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->defCap(old);
    }
}

void CompendiumApplyAllArchetypeSpeed(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->speed(old);
    }
}

void CompendiumApplyAllArchetypeImmunities(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->immunities(old);
    }
}

void CompendiumApplyAllArchetypeBAB(utf32 *old, s32 dv)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->bab(old, dv);
    }
}

u32 CompendiumApplyAllArchetypeSkills(u32 skillEntry)
{
    u32 currEntry = skillEntry;
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        currEntry = curr->skills(currEntry);
    }
    return currEntry;
}

void CompendiumApplyAllArchetypeTalents(CachedPageEntry *page)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->talents(page);
    }
}

void CompendiumApplyAllArchetypeEnv(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->env(old);
    }
}

void CompendiumApplyAllArchetypeOrg(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->org(old);
    }
}

void CompendiumApplyAllArchetypeTreasure(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->treasure(old);
    }
}

void CompendiumApplyAllArchetypeSpecQual(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->specialQual(old);
    }
}

void CompendiumApplyAllArchetypeSpecCap(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->specialCap(old);
    }
}

void CompendiumApplyAllArchetypeLang(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->lang(old);
    }
}

void CompendiumApplyAllArchetypeAura(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->aura(old);
    }
}

void CompendiumApplyAllArchetypeWeak(utf32 *old)
{
    for(s32 i = 0; i < compendium.appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = compendium.appliedArchetypes + i;
        curr->weak(old);
    }
}