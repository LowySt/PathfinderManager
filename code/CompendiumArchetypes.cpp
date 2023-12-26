b32 CompendiumOpenArchetypeWindow(UIContext *c, void *user)
{
    compendium.arch.isChoosingArchetype = !compendium.arch.isChoosingArchetype;
    return FALSE;
}

b32 CompendiumSelectArchetype(UIContext *c, void *user)
{
    s32 index = (s32)((s64)user);
    
    return FALSE;
}

void advancedCreatureGS(s32 *oldGS)
{
    *oldGS += 1;
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

ArchetypeDiff AdvancedCreature = { advancedCreatureGS, advancedCreatureAS, advancedCreatureAC };
