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

//NOTETODO: Because of Serialization, changing the order in this specific array is bad news.
//          Between versions the archetypes would get fucked.
//          So, let's try to keep the index position of these fields always the same and only append
//          to the array. If I want to change the order or add things in between, it should be handled
//          when displaying the options in the UI, rather than here.
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
    { ls_staticArrayAppendUnique(&compendium.appliedArchetypes, index); }
    
    GetPageEntryAndCache(compendium.pageIndex, -1, &cachedPage, compendium.appliedArchetypes, NULL);
    compendium.arch.isChoosingArchetype = FALSE;
    return FALSE;
}

//TODO: Make Window parameterised on a passed (x,y,width,height)
b32 DrawArchetypeSelection(UIContext *c)
{
    b32 inputUse = FALSE;
    
    //NOTE: Darken the Compendium Page to indicate inactiveness
    ls_uiRect(c, 0, 0, c->width, 0.935f*c->height, RGBA(0, 0, 0, 0xAA), RGBA(0, 0, 0, 0xAA), 1);
    
    //NOTE: Draw the Archetype Selection Window
    ls_uiRect(c, 0.02f*c->width, 0.33f*c->height, 0.955*c->width, 0.47f*c->height, 2);
    
    //NOTE: Draw Simple Archetypes
    s32 baseX = 0.02f*c->width;
    s32 baseY = 0.74f*c->height;
    
    ls_uiSelectFontByPixelHeight(c, 20);
    ls_uiLabel(c, U"Simple", 0.03f*c->width, 0.77f*c->height, c->textColor, 3);
    ls_uiHSeparator(c, 0.03f*c->width, 0.76f*c->height, 0.94f*c->width, 1, RGBg(0), 3);
    ls_uiSelectFontByPixelHeight(c, 16);
    
    //NOTE: Looping over all Simple Archetypes Indices
    s32 xAdv = 0;
    baseX = 0.03f*c->width;
    baseY = 0.72f*c->height;
    
    s32 maxWidth = 0;
    for(s32 simpleIdx = 0; simpleIdx < 10; simpleIdx++)
    {
        if(compendium.arch.archetypes[simpleIdx].bmp.w > maxWidth)
        { maxWidth = compendium.arch.archetypes[simpleIdx].bmp.w; }
    }
    xAdv = maxWidth + (0.02f*c->width);
    
    for(s32 simpleIdx = 0; simpleIdx < 10; simpleIdx++)
    {
        ArchetypeDiff selectedDiff = allArchetypeDiffs[simpleIdx];
        if(!selectedDiff.isCompatible(&cachedPage))
        {
            compendium.arch.archetypes[simpleIdx].isInactive = TRUE;
            inputUse |= ls_uiButton(c, &compendium.arch.archetypes[simpleIdx], baseX, baseY, 3);
            compendium.arch.archetypes[simpleIdx].isInactive = FALSE;
        }
        else
        {
            inputUse |= ls_uiButton(c, &compendium.arch.archetypes[simpleIdx], baseX, baseY, 3);
        }
        
        baseX += xAdv;
        if((simpleIdx+1) % 5 == 0)
        {
            baseX  = 0.03f*c->width;
            if(simpleIdx < 9) { baseY -= 0.058f*c->height; }
        }
    }
    
    //NOTE: Draw Acquired Archetypes
    baseX  = 0.03f*c->width;
    baseY -= 0.041f*c->height;
    
    ls_uiSelectFontByPixelHeight(c, 20);
    ls_uiLabel(c, U"Acquired", 0.03f*c->width, baseY, c->textColor, 3); baseY -= 0.010f*c->height;
    ls_uiHSeparator(c, 0.03f*c->width, baseY, 0.94f*c->width, 1, RGBg(0), 3);
    ls_uiSelectFontByPixelHeight(c, 16);
    
    //NOTE: Looping over all Acquired Archetypes Indices
    baseY -= 0.042f*c->height;
    
    maxWidth = 0;
    for(s32 acqIdx = 10; acqIdx < 16; acqIdx++)
    {
        if(compendium.arch.archetypes[acqIdx].bmp.w > maxWidth)
        { maxWidth = compendium.arch.archetypes[acqIdx].bmp.w; }
    }
    xAdv = maxWidth + (0.02f*c->width);
    
    for(s32 acqIdx = 10; acqIdx < 16; acqIdx++)
    {
        ArchetypeDiff selectedDiff = allArchetypeDiffs[acqIdx];
        if(!selectedDiff.isCompatible(&cachedPage))
        {
            compendium.arch.archetypes[acqIdx].isInactive = TRUE;
            inputUse |= ls_uiButton(c, &compendium.arch.archetypes[acqIdx], baseX, baseY, 3);
            compendium.arch.archetypes[acqIdx].isInactive = FALSE;
        }
        else
        { inputUse |= ls_uiButton(c, &compendium.arch.archetypes[acqIdx], baseX, baseY, 3); }
        
        baseX += xAdv;
        if((acqIdx-10+1) % 4 == 0)
        {
            baseX  = 0.03f*c->width;
            if(acqIdx < 15) { baseY -= 0.058f*c->height; }
        }
    }
    
    //NOTE: Adding Exoscheletro, which is out of order.
    inputUse |= ls_uiButton(c, &compendium.arch.archetypes[17], baseX, baseY, 3);
    
    //NOTE: Draw Acquired Archetypes
    baseX  = 0.03f*c->width;
    baseY -= 0.041f*c->height;
    
    ls_uiSelectFontByPixelHeight(c, 20);
    ls_uiLabel(c, U"Inherited", 0.03f*c->width, baseY, c->textColor, 3); baseY -= 0.010f*c->height;
    ls_uiHSeparator(c, 0.03f*c->width, baseY, 0.94f*c->width, 1, RGBg(0), 3);
    ls_uiSelectFontByPixelHeight(c, 16);
    
    //NOTE: Looping over all Acquired Archetypes Indices
    baseY -= 0.042f*c->height;
    
    //NOTE: Adding Micotico, the only Inherited, currently.
    inputUse |= ls_uiButton(c, &compendium.arch.archetypes[16], baseX, baseY, 3);
    
    return inputUse;
}

//----------------------------//
// ARCHETYPE DIFF APPLICATION //
//----------------------------//

void CompendiumApplyAllArchetypeNames(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, utf32 *name)
{
    AssertMsg(name, "Null utf32 pointer\n");
    
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        
        AssertMsg(name->len + curr->nameStr.len+1 < name->size, "Can't fit archetype name\n");
        if(name->len + curr->nameStr.len+1 > name->size) { return; }
        
        ls_utf32Append(name, U" ");
        ls_utf32Append(name, curr->nameStr);
    }
}

void CompendiumApplyAllArchetypeGS(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                   u16 gsEntry, s32 hitDice, utf32 *newGS, utf32 *newPE)
{
    if(gsEntry == GS_SENTINEL_VALUE) { return; }
    
    s32 totalGSDiff = 0;
    s32 totalRMDiff = 0;
    
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        
        s32 gsDiff = 0;
        s32 rmDiff = 0;
        b32 setsGS = curr->gs(hitDice, &gsDiff, &rmDiff);
        
        if(setsGS) { CompendiumSetGS(gsDiff, rmDiff, newGS, newPE); return; }
        
        totalGSDiff += gsDiff;
        totalRMDiff += rmDiff;
    }
    
    CompendiumIncreaseGS(gsEntry, totalGSDiff, totalRMDiff, newGS, newPE);
}

void CompendiumApplyAllArchetypeAS(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                   s32 as[AS_COUNT])
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->abilityScores(as);
    }
}

b32 CompendiumApplyAllArchetypeAC(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                  utf32 *size, s32 ac[AC_TYPES_COUNT])
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        b32 replaces = curr->armorClass(size, ac);
        if(replaces == TRUE) { return TRUE; }
    }
    
    return FALSE;
}

//TODO: Static UTF32 Strings REALLY Needed
void CompendiumAddAllArchetypesToList(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                      utf32 *archetypeList)
{
    u32 tempBuff[128] = {};
    utf32 tempString = { tempBuff, 0, 128 };
    
    if(archetypeList->len)
    {
        ls_utf32Copy_t(*archetypeList, &tempString);
        ls_utf32Clear(archetypeList);
        
        for(s32 i = 0; i < appliedArchetypes.count; i++)
        {
            ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
            ls_utf32InsertSubstr(&tempString, U", ", tempString.len-2);
            ls_utf32InsertSubstr(&tempString, curr->nameStr, tempString.len-2);
        }
        
        ls_utf32Copy_t(tempString, archetypeList);
    }
    else
    {
        ls_utf32Append(archetypeList, ls_utf32Constant(U"["));
        for(s32 i = 0; i < appliedArchetypes.count; i++)
        {
            ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
            ls_utf32Append(archetypeList, curr->nameStr);
            if(i < appliedArchetypes.count - 1) { ls_utf32Append(archetypeList, U", "); }
        }
        
        ls_utf32Append(archetypeList, ls_utf32Constant(U"] "));
    }
}

void CompendiumApplyAllArchetypeSenses(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                       utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->senses(old);
    }
}

void CompendiumApplyAllArchetypeRD(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                   s32 hitDice, utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->rd(hitDice, old);
    }
}

void CompendiumApplyAllArchetypeResistances(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                            s32 hitDice, u64 orig, utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->resistances(hitDice, orig, old);
    }
}

void CompendiumApplyAllArchetypeRI(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                   utf32 gs, utf32 *ri)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->ri(gs, ri);
    }
}

void CompendiumApplyAllArchetypeSpecAtk(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                        utf32 *spec)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        b32 hasSet = curr->specialAtk(spec);
        if(hasSet) { return; }
    }
}

void CompendiumApplyAllArchetypeSize(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                     utf32 *size)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->size(size);
    }
}

//NOTETODO: This currently applies after all melee calculations have been performed.
//          So all values must be already computed. Maybe we want an intermediate step
//          to know if there are any natural attacks already? And the kind of those natural attacks...
void CompendiumApplyAllArchetypeMelee(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                      CachedPageEntry *page, utf32 *melee)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->melee(page, melee);
    }
}

void CompendiumApplyAllArchetypeAlign(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                      utf32 *align)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        b32 setsAlign = curr->align(align);
        if(setsAlign) { return; }
    }
}

void CompendiumApplyAllArchetypeTypes(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                      utf32 *type)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->type(type);
    }
}

void CompendiumApplyAllArchetypeSubTypes(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                         utf32 *subtype)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->subtype(subtype);
    }
}

void CompendiumApplyAllArchetypeDV(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                   CachedPageEntry *page, utf32 *oldType, u64 *DV)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->DV(page, oldType, DV);
    }
}

b32 CompendiumApplyAllArchetypeST(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                  s32 DV, s32 st[ST_COUNT])
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        b32 hasReplaced = curr->st(DV, st);
        if(hasReplaced) { return TRUE; }
    }
    
    return FALSE;
}

void CompendiumApplyAllArchetypeDefCap(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                       utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->defCap(old);
    }
}

void CompendiumApplyAllArchetypeSpeed(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                      utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->speed(old);
    }
}

void CompendiumApplyAllArchetypeImmunities(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                           utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->immunities(old);
    }
}

void CompendiumApplyAllArchetypeBAB(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                    utf32 *old, s32 dv)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->bab(old, dv);
    }
}

u32 CompendiumApplyAllArchetypeSkills(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                      u32 skillEntry)
{
    u32 currEntry = skillEntry;
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        currEntry = curr->skills(currEntry);
    }
    return currEntry;
}

void CompendiumApplyAllArchetypeTalents(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                        CachedPageEntry *page)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->talents(page);
    }
}

void CompendiumApplyAllArchetypeEnv(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                    utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->env(old);
    }
}

void CompendiumApplyAllArchetypeOrg(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                    utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->org(old);
    }
}

void CompendiumApplyAllArchetypeTreasure(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                         utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->treasure(old);
    }
}

void CompendiumApplyAllArchetypeSpecQual(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                         utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->specialQual(old);
    }
}

void CompendiumApplyAllArchetypeSpecCap(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                        utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->specialCap(old);
    }
}

void CompendiumApplyAllArchetypeLang(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                     utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->lang(old);
    }
}

void CompendiumApplyAllArchetypeAura(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                     utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->aura(old);
    }
}

void CompendiumApplyAllArchetypeWeak(StaticArray<s32, MAX_CONCURRENT_ARCHETYPES> appliedArchetypes, 
                                     utf32 *old)
{
    for(s32 i = 0; i < appliedArchetypes.count; i++)
    {
        ArchetypeDiff *curr = &allArchetypeDiffs[appliedArchetypes[i]];
        curr->weak(old);
    }
}
