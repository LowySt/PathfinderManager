struct CachedPageEntry
{
    s32 pageIndex = -1;
    
    b32 acHasArmor = FALSE;
    
    utf32 origin;
    utf32 shortDesc;
    utf32 AC;
    utf32 HP;
    utf32 ST;
    utf32 RD;
    utf32 RI;
    utf32 defensiveCapacity;
    utf32 melee;
    utf32 ranged;
    utf32 specialAttacks;
    utf32 psych;
    utf32 magics;
    utf32 spells;
    utf32 racialMods;
    utf32 spec_qual;
    utf32 specials;
    utf32 org;
    utf32 treasure;
    utf32 desc;
    utf32 source;
    
    utf32 name;
    utf32 gs;
    utf32 pe;
    utf32 alignment;
    utf32 type;
    utf32 subtype;
    utf32 archetype;
    utf32 size;
    utf32 initiative;
    utf32 senses;
    utf32 perception;
    utf32 aura;
    utf32 immunities;
    utf32 resistances;
    utf32 weaknesses;
    utf32 speed;
    utf32 space;
    utf32 reach;
    utf32 STR;
    utf32 DEX;
    utf32 CON;
    utf32 INT;
    utf32 WIS;
    utf32 CHA;
    utf32 BAB;
    utf32 BMC;
    utf32 DMC;
    utf32 talents;
    utf32 skills; //TODO Separate type from value
    utf32 languages;
    utf32 environment;
};

struct PageEntry
{
    /*Image?*/
    /*Menù*/
    
    u32 origin;
    u32 shortDesc;
    u32 AC;
    u32 HP;
    u32 ST;
    u32 RD;
    u32 RI;
    u32 defensiveCapacity;
    u32 melee;
    u32 ranged;
    u32 specialAttacks;
    u32 psych;
    u32 magics;
    u32 spells;
    u32 racialMods;
    u32 spec_qual; //Duplicate of defensiveCapacity + magicalCapacity + special attacks??
    u32 specials[24];
    u32 org;
    u32 treasure;
    u32 desc;
    u32 source;
    
    u16 name;
    u16 gs;
    u16 pe;
    u16 alignment;
    u16 type;
    u16 subtype[8];
    u16 archetype[4];
    u16 size;
    u16 initiative;
    u16 senses[8];
    u16 perception;
    u16 aura;
    u16 immunities[16];
    u16 resistances[16];
    u16 weaknesses[16];
    u16 speed;
    u16 space;
    u16 reach;
    u16 STR;
    u16 DEX;
    u16 CON;
    u16 INT;
    u16 WIS;
    u16 CHA;
    u16 BAB;
    u16 BMC;
    u16 DMC;
    u16 talents[24];
    u16 skills[24]; //TODO Separate type from value
    u16 languages[24];
    u16 environment;
};

struct Codex
{
    buffer names;
    buffer gs;
    buffer types;
    buffer subtypes;
    
    buffer generalStrings;
    buffer numericValues;
    buffer pe;
    buffer alignment;
    buffer archetypes;
    buffer sizes;
    buffer senses;
    buffer auras;
    buffer immunities;
    buffer resistances;
    buffer weaknesses;
    buffer specialAttacks;
    buffer spells;
    buffer talents;
    buffer skills;
    buffer languages;
    buffer environment;
    buffer specials;
    
    //TODO: Change this to a FixedArray
    Array<PageEntry> pages;
};

struct Compendium
{
    Codex codex;
    
    UITextBox searchBar;
    
    b32   isViewingPage;
    s32   pageIndex = -1;
    Array<u16> viewIndices;
};

//MonsterTable monsterTable = {};
Compendium      compendium     = {};
CachedPageEntry cachedPage     = {};
UIScrollableRegion pageScroll  = {};
UIScrollableRegion tableScroll = {};

b32 CompendiumOpenMonsterTable(UIContext *c, void *userData)
{
    compendium.isViewingPage = FALSE;
    compendium.pageIndex     = -1;
    
    return FALSE;
}

b32 CompendiumSearchFunction(UIContext *c, void *userData)
{
    ls_arenaUse(compTempArena);
    
    //NOTE: Reset the scrollbar when searching
    tableScroll.deltaY = 0;
    
    if(compendium.searchBar.text.len < 2)
    { 
        tableScroll.minY = -((compendium.codex.pages.count-30) * 19);
        ls_arrayClear(&compendium.viewIndices);
        for(u16 i = 0; i < compendium.viewIndices.cap; i++)
        { ls_arrayAppend(&compendium.viewIndices, i); }
        
        return FALSE;
    }
    
    //NOTE:Iterate over the name buffer to find all names which exactly contain the substring needle
    utf8 needle = ls_utf8FromUTF32(compendium.searchBar.text);
    
    buffer *names = &compendium.codex.names;
    names->cursor = 4;
    
    u16 namesIndexBuffer[2048] = {};
    u16 nibCount = 0;
    while(names->cursor < names->size)
    {
        s32 strByteLen = ls_bufferPeekWord(names);
        u8 *data       = (u8 *)names->data + names->cursor + 2;
        
        utf8 toMatch   = ls_utf8Constant(data, strByteLen);
        
        if(ls_utf8Contains(toMatch, needle))
        {
            namesIndexBuffer[nibCount] = names->cursor;
            nibCount += 1;
        }
        
        ls_bufferReadSkip(names, strByteLen + 2);
    }
    
    //NOTE: Adjust scrollbar height to new monster table count.
    tableScroll.minY = -((nibCount-30) * 19);
    
    if(nibCount > 0)
    {
        ls_arrayClear(&compendium.viewIndices);
        
        //NOTE: Now that we have a buffer of all matching name indices, let's search through the actual pages
        for(u16 i = 0; i < compendium.codex.pages.count; i++)
        {
            u16 indexToMatch = compendium.codex.pages[i].name;
            for(u32 j = 0; j < nibCount; j++)
            {
                if(namesIndexBuffer[j] == indexToMatch)
                {
                    //NOTE: Remove from the namesIndexBuffer and add to the viewIndices array
                    ls_arrayAppend(&compendium.viewIndices, i);
                    
                    namesIndexBuffer[j] = namesIndexBuffer[nibCount-1];
                    nibCount -= 1;
                    break;
                }
            }
        }
    }
    
    ls_arenaUse(globalArena);
    
    return FALSE;
}

//TODO: What about Deviation being affected by some things?
void CalculateAndCacheAC(utf32 AC, CachedPageEntry *cachedPage)
{
    cachedPage->acHasArmor = FALSE;
    
    s32 acExprBeginIdx = ls_utf32LeftFind(AC, (u32)'(');
    s32 acExprEndIdx   = ls_utf32LeftFind(AC, (u32)')');
    
    if((acExprBeginIdx == -1) || (acExprEndIdx == -1))
    { 
        cachedPage->acHasArmor = TRUE;
        ls_utf32Set(&cachedPage->AC, AC);
        return;
    }
    
    s32 acExprBegin = acExprBeginIdx + 1;
    s32 acExprLen   = acExprEndIdx - acExprBegin;
    utf32 acExpr    = { AC.data + acExprBegin, (u32)acExprLen, (u32)acExprLen };
    
    s32 armorBonusIdx  = ls_utf32LeftFind(acExpr, ls_utf32Constant(U"Armatura"));
    s32 shieldBonusIdx = ls_utf32LeftFind(acExpr, ls_utf32Constant(U"Scudo"));
    
    if((armorBonusIdx != -1) || (shieldBonusIdx != -1))
    {
        cachedPage->acHasArmor = TRUE;
        ls_utf32Set(&cachedPage->AC, AC);
        return;
    }
    
    //TODO: I don't really like these hardcoded offsets, would prefer to have a LeftFindNumeric() function
    s32 firstValEnd     = ls_utf32LeftFind(AC, (u32)',');
    s32 secondValBegin  = firstValEnd + 11;
    s32 secondValEnd    = ls_utf32LeftFind(AC, secondValBegin, (u32)',');
    s32 secondValLen    = secondValEnd - secondValBegin;
    
    if((firstValEnd == -1) || (secondValEnd == -1))
    {
        cachedPage->acHasArmor = TRUE;
        ls_utf32Set(&cachedPage->AC, AC);
        return;
    }
    
    s32 totAC   = ls_utf32ToInt({AC.data, (u32)firstValEnd, (u32)firstValEnd});
    s32 touchAC = ls_utf32ToInt({AC.data + secondValBegin, (u32)secondValLen, (u32)secondValLen});
    
    s32 dexBonusNew = ls_utf32ToInt(cachedPage->DEX) - 10;
    s32 dexBonusOld = s32(dexBonusNew / 2);
    
    totAC   = (totAC   - dexBonusOld) + dexBonusNew;
    touchAC = (touchAC - dexBonusOld) + dexBonusNew;
    
    u32 buff[32] = {};
    utf32 tmpString = { buff, 32, 32 };
    
    ls_utf32Clear(&cachedPage->AC);
    ls_utf32FromInt_t(&cachedPage->AC, totAC);
    ls_utf32Append(&cachedPage->AC, ls_utf32Constant(U", contatto "));
    ls_utf32FromInt_t(&tmpString, touchAC);
    ls_utf32Append(&cachedPage->AC, tmpString);
    ls_utf32Append(&cachedPage->AC, {AC.data + secondValEnd, AC.len - secondValEnd, AC.len - secondValEnd});
}

void CalculateAndCacheST(utf32 ST, CachedPageEntry *cachedPage)
{
    s32 conBonusNew = ls_utf32ToInt(cachedPage->CON) - 10;
    s32 dexBonusNew = ls_utf32ToInt(cachedPage->DEX) - 10;
    s32 wisBonusNew = ls_utf32ToInt(cachedPage->WIS) - 10;
    s32 conBonusOld = s32(conBonusNew / 2);
    s32 dexBonusOld = s32(dexBonusNew / 2);
    s32 wisBonusOld = s32(wisBonusNew / 2);
    
    //NOTE: Fix for Constructs
    if(ls_utf32AreEqual(cachedPage->CON, ls_utf32Constant(U"-")))
    { conBonusNew = 0; conBonusOld = 0; }
    
    //TODO: Make these not hardcoded like that
    s32 conSaveBegin = 7;
    s32 conSaveEnd   = ls_utf32LeftFind(ST, (u32)',');
    s32 dexSaveBegin = conSaveEnd + 11;
    s32 dexSaveEnd   = ls_utf32LeftFind(ST, dexSaveBegin, (u32)',');
    s32 wisSaveBegin = dexSaveEnd + 10;
    s32 wisSaveEnd   = ls_utf32LeftFind(ST, wisSaveBegin, (u32)';')-1; //NOTE: This might fuck
    
    if((conSaveEnd == -1) || (dexSaveEnd == -1)) { ls_utf32Set(&cachedPage->ST, ST); return; }
    
    if(wisSaveEnd < 0) { wisSaveEnd = ST.len-1; }
    
    s32 conSave = ls_utf32ToInt({ST.data + conSaveBegin, (u32)conSaveEnd - conSaveBegin, (u32)conSaveEnd - conSaveBegin});
    s32 dexSave = ls_utf32ToInt({ST.data + dexSaveBegin, (u32)dexSaveEnd - dexSaveBegin, (u32)dexSaveEnd - dexSaveBegin});
    s32 wisSave = ls_utf32ToInt({ST.data + wisSaveBegin, (u32)wisSaveEnd - wisSaveBegin+1, (u32)wisSaveEnd - wisSaveBegin+1});
    
    conSave = (conSave - conBonusOld) + conBonusNew;
    dexSave = (dexSave - dexBonusOld) + dexBonusNew;
    wisSave = (wisSave - wisBonusOld) + wisBonusNew;
    
    u32 buff[32] = {};
    utf32 tmpString = { buff, 32, 32 };
    
    ls_utf32Clear(&cachedPage->ST);
    ls_utf32Append(&cachedPage->ST, ls_utf32Constant(U"Tempra "));
    
    if(conSave >= 0) ls_utf32AppendChar(&cachedPage->ST, (u32)'+');
    
    ls_utf32FromInt_t(&tmpString, conSave);
    ls_utf32Append(&cachedPage->ST, tmpString);
    ls_utf32Clear(&tmpString);
    
    ls_utf32Append(&cachedPage->ST, ls_utf32Constant(U", Riflessi "));
    
    if(dexSave >= 0) ls_utf32AppendChar(&cachedPage->ST, (u32)'+');
    
    ls_utf32FromInt_t(&tmpString, dexSave);
    ls_utf32Append(&cachedPage->ST, tmpString);
    ls_utf32Clear(&tmpString);
    
    ls_utf32Append(&cachedPage->ST, ls_utf32Constant(U", Volont\U000000E0 "));
    
    if(wisSave >= 0) ls_utf32AppendChar(&cachedPage->ST, (u32)'+');
    
    ls_utf32FromInt_t(&tmpString, wisSave);
    ls_utf32Append(&cachedPage->ST, tmpString);
    ls_utf32Clear(&tmpString);
    
    ls_utf32AppendBuffer(&cachedPage->ST, ST.data + wisSaveEnd+1, ST.len - (wisSaveEnd+1));
}

void CalculateAndCacheHP(utf32 hp, CachedPageEntry *cachedPage)
{
    s32 hpEndIndex  = ls_utf32LeftFind(hp, (u32)' ');
    AssertMsg(hpEndIndex >= 0, "HP index can't be found\n");
    
    s32 hpExprBegin = ls_utf32LeftFind(hp, (u32)'(') + 1;
    s32 hpExprLen   = ls_utf32LeftFind(hp, (u32)')') - hpExprBegin;
    
    utf32 hpExpr    = {hp.data + hpExprBegin, (u32)hpExprLen, (u32)hpExprLen};
    
    s32 dToken      = ls_utf32LeftFind(hpExpr, (u32)'d');
    s32 plusToken   = ls_utf32LeftFind(hpExpr, (u32)'+');
    s32 minusToken  = ls_utf32LeftFind(hpExpr, (u32)'-');
    s32 flatLen     = hpExprLen - plusToken - 1;
    
    //NOTE: If the parser fails, we just set the finalHP to -1. The program will not crash
    //      And the error should be clearly visible.
    s32 finalHP = -1;
    if(dToken != -1)
    {
        if(plusToken != -1)
        {
            s32 mulLen   = plusToken - dToken - 1;
            
            s32 dieValue = ls_utf32ToInt({hpExpr.data, (u32)dToken, (u32)dToken});
            s32 dieMul   = ls_utf32ToInt({hpExpr.data + dToken+1, (u32)mulLen, (u32)mulLen});
            s32 flatVal  = ls_utf32ToInt({hpExpr.data + plusToken+1, (u32)flatLen, (u32)flatLen});
            
            //NOTE: The flatVal is multiplied by 2 because the CON bonus is doubled.
            finalHP  = dieValue*dieMul + (flatVal * 2);
        }
        else if(minusToken != -1)
        {
            s32 mulLen   = minusToken - dToken - 1;
            
            s32 dieValue = ls_utf32ToInt({hpExpr.data, (u32)dToken, (u32)dToken});
            s32 dieMul   = ls_utf32ToInt({hpExpr.data + dToken+1, (u32)mulLen, (u32)mulLen});
            s32 flatVal  = ls_utf32ToInt({hpExpr.data + minusToken+1, (u32)flatLen, (u32)flatLen});
            
            //NOTE: The flatVal is multiplied by 2 because the CON bonus is doubled.
            finalHP  = dieValue*dieMul - (flatVal * 2);
        }
        else
        {
            s32 mulLen   = hpExprLen - dToken - 1;
            
            s32 dieValue = ls_utf32ToInt({hpExpr.data, (u32)dToken, (u32)dToken});
            s32 dieMul   = ls_utf32ToInt({hpExpr.data + dToken+1, (u32)mulLen, (u32)mulLen});
            
            finalHP  = dieValue*dieMul;
        }
    }
    
    s32 restLen       = hp.len - hpEndIndex;
    ls_utf32FromInt_t(&cachedPage->HP, finalHP);
    ls_utf32AppendBuffer(&cachedPage->HP, hp.data + hpEndIndex, restLen);
    
    return;
}

void CalculateAndCacheBMC(utf32 BMC, CachedPageEntry *cachedPage)
{
    if(ls_utf32AreEqual(BMC, ls_utf32Constant(U"-"))) { ls_utf32Set(&cachedPage->BMC, BMC); return; }
    
    s32 endIdx = ls_utf32LeftFind(BMC, (u32)'(');
    if(endIdx == -1) endIdx = BMC.len;
    
    //TODO: Fuck Golarion
    b32 smallSize = ls_utf32AreEqual(cachedPage->size, ls_utf32Constant(U"Minuscola"));
    smallSize |= ls_utf32AreEqual(cachedPage->size, ls_utf32Constant(U"Minuta"));
    smallSize |= ls_utf32AreEqual(cachedPage->size, ls_utf32Constant(U"Piccolissima"));
    smallSize |= ls_utf32AreEqual(cachedPage->size, ls_utf32Constant(U"Minuscolo"));
    smallSize |= ls_utf32AreEqual(cachedPage->size, ls_utf32Constant(U"Minuto"));
    smallSize |= ls_utf32AreEqual(cachedPage->size, ls_utf32Constant(U"Piccolissimo"));
    
    s32 statBonusNew = (smallSize ? ls_utf32ToInt(cachedPage->DEX) : ls_utf32ToInt(cachedPage->STR)) - 10;
    s32 statBonusOld = s32(statBonusNew / 2);
    
    s32 bmcVal = ls_utf32ToInt({BMC.data, (u32)endIdx, (u32)endIdx});
    
    bmcVal = (bmcVal - statBonusOld) + statBonusNew;
    
    u32 buff[32] = {};
    utf32 tmpString = { buff, 32, 32 };
    
    ls_utf32Clear(&cachedPage->BMC);
    
    if(bmcVal >= 0) ls_utf32AppendChar(&cachedPage->BMC, (u32)'+');
    
    ls_utf32FromInt_t(&tmpString, bmcVal);
    ls_utf32Append(&cachedPage->BMC, tmpString);
    ls_utf32AppendChar(&cachedPage->BMC, (u32)' ');
    ls_utf32AppendBuffer(&cachedPage->BMC, BMC.data + endIdx, BMC.len - endIdx);
}

void CalculateAndCacheDMC(utf32 DMC, CachedPageEntry *cachedPage)
{
    if(ls_utf32AreEqual(DMC, ls_utf32Constant(U"-"))) { ls_utf32Set(&cachedPage->DMC, DMC); return; }
    
    s32 endIdx = ls_utf32LeftFind(DMC, (u32)'(');
    if(endIdx == -1) endIdx = DMC.len;
    
    s32 dexBonusNew = ls_utf32ToInt(cachedPage->DEX) - 10;
    s32 strBonusNew = ls_utf32ToInt(cachedPage->STR) - 10;
    s32 dexBonusOld = s32(dexBonusNew / 2);
    s32 strBonusOld = s32(strBonusNew / 2);
    
    s32 dmcVal = ls_utf32ToInt({DMC.data, (u32)endIdx, (u32)endIdx});
    
    dmcVal = (dmcVal - dexBonusOld - strBonusOld) + dexBonusNew + strBonusNew;
    
    u32 buff[32] = {};
    utf32 tmpString = { buff, 32, 32 };
    
    ls_utf32Clear(&cachedPage->DMC);
    
    if(dmcVal >= 0) ls_utf32AppendChar(&cachedPage->DMC, (u32)'+');
    
    ls_utf32FromInt_t(&tmpString, dmcVal);
    ls_utf32Append(&cachedPage->DMC, tmpString);
    ls_utf32AppendChar(&cachedPage->DMC, (u32)' ');
    ls_utf32AppendBuffer(&cachedPage->DMC, DMC.data + endIdx, DMC.len - endIdx);
}

void CalculateAndCacheInitiative(utf32 Init, CachedPageEntry *cachedPage)
{
    //TODO: Fix cases like +12 (+99 Maremma Maiala)
    //      Those cases trip the M detection, so we need a first pass to make sure the M isn't between
    //      A couple of parentheses.
    
    if(Init.len == 0) { ls_utf32Set(&cachedPage->initiative, Init); return; }
    
    s32 dexBonusNew = ls_utf32ToInt(cachedPage->DEX) - 10;
    s32 dexBonusOld = s32(dexBonusNew / 2);
    
    s32 multiToken  = ls_utf32LeftFind(Init, (u32)'/');
    s32 mithicToken = ls_utf32LeftFind(Init, (u32)'M');
    
    s32 commaIdx = ls_utf32LeftFind(Init, (u32)',');
    s32 semiIdx  = ls_utf32LeftFind(Init, (u32)';');
    s32 spaceIdx = ls_utf32LeftFind(Init, (u32)' ');
    
    s32 endIdx = commaIdx;
    if(commaIdx == -1) 
    { 
        endIdx = semiIdx;
        
        if(semiIdx == -1)
        {
            endIdx = spaceIdx;
            
            if(spaceIdx == -1)
            { 
                endIdx = Init.len;
            }
        }
    }
    
    u32 buff[32] = {};
    utf32 tmpString = { buff, 32, 32 };
    ls_utf32Clear(&cachedPage->initiative);
    
    s32 initOne = -999;
    s32 initTwo = -999;
    if(multiToken != -1)
    {
        if(mithicToken != -1)
        {
            if(mithicToken < multiToken) // +16M/-4, ...
            {
                s32 twoLen  = endIdx - multiToken - 1;
                
                initOne = ls_utf32ToInt({Init.data, (u32)mithicToken, (u32)mithicToken});
                initTwo = ls_utf32ToInt({Init.data + multiToken + 1, (u32)twoLen, (u32)twoLen});
                initOne = (initOne - dexBonusOld) + dexBonusNew;
                initTwo = (initTwo - dexBonusOld) + dexBonusNew;
                
                if(initOne >= 0) ls_utf32AppendChar(&cachedPage->initiative, (u32)'+');
                ls_utf32FromInt_t(&tmpString, initOne);
                ls_utf32Append(&cachedPage->initiative, tmpString);
                ls_utf32Clear(&tmpString);
                ls_utf32AppendChar(&cachedPage->initiative, (u32)'M');
                ls_utf32AppendChar(&cachedPage->initiative, (u32)'/');
                if(initTwo >= 0) ls_utf32AppendChar(&cachedPage->initiative, (u32)'+');
                ls_utf32FromInt_t(&tmpString, initTwo);
                ls_utf32Append(&cachedPage->initiative, tmpString);
                ls_utf32AppendBuffer(&cachedPage->initiative, Init.data + endIdx, Init.len - endIdx);
                return;
            }
            else // +23/+3M ...
            {
                s32 twoLen  = mithicToken - multiToken - 1;
                
                initOne = ls_utf32ToInt({Init.data, (u32)multiToken, (u32)multiToken});
                initTwo = ls_utf32ToInt({Init.data + multiToken + 1, (u32)twoLen, (u32)twoLen});
                initOne = (initOne - dexBonusOld) + dexBonusNew;
                initTwo = (initTwo - dexBonusOld) + dexBonusNew;
                
                if(initOne >= 0) ls_utf32AppendChar(&cachedPage->initiative, (u32)'+');
                ls_utf32FromInt_t(&tmpString, initOne);
                ls_utf32Append(&cachedPage->initiative, tmpString);
                ls_utf32Clear(&tmpString);
                ls_utf32AppendChar(&cachedPage->initiative, (u32)'/');
                if(initTwo >= 0) ls_utf32AppendChar(&cachedPage->initiative, (u32)'+');
                ls_utf32FromInt_t(&tmpString, initTwo);
                ls_utf32Append(&cachedPage->initiative, tmpString);
                ls_utf32AppendChar(&cachedPage->initiative, (u32)'M');
                ls_utf32AppendBuffer(&cachedPage->initiative, Init.data + endIdx, Init.len - endIdx);
                
                return;
            }
        }
        else // +12/+2,  ...
        {
            s32 twoLen  = endIdx - multiToken - 1;
            
            initOne = ls_utf32ToInt({Init.data, (u32)multiToken, (u32)multiToken});
            initTwo = ls_utf32ToInt({Init.data + multiToken + 1, (u32)twoLen, (u32)twoLen});
            initOne = (initOne - dexBonusOld) + dexBonusNew;
            initTwo = (initTwo - dexBonusOld) + dexBonusNew;
            
            if(initOne >= 0) ls_utf32AppendChar(&cachedPage->initiative, (u32)'+');
            ls_utf32FromInt_t(&tmpString, initOne);
            ls_utf32Append(&cachedPage->initiative, tmpString);
            ls_utf32Clear(&tmpString);
            ls_utf32AppendChar(&cachedPage->initiative, (u32)'/');
            if(initTwo >= 0) ls_utf32AppendChar(&cachedPage->initiative, (u32)'+');
            ls_utf32FromInt_t(&tmpString, initTwo);
            ls_utf32Append(&cachedPage->initiative, tmpString);
            ls_utf32AppendBuffer(&cachedPage->initiative, Init.data + endIdx, Init.len - endIdx);
            
            return;
        }
    }
    else
    {
        if(mithicToken != -1) // +23M ...
        {
            initOne = ls_utf32ToInt({Init.data, (u32)mithicToken, (u32)mithicToken});
            initOne = (initOne - dexBonusOld) + dexBonusNew;
            
            if(initOne >= 0) ls_utf32AppendChar(&cachedPage->initiative, (u32)'+');
            ls_utf32FromInt_t(&tmpString, initOne);
            ls_utf32Append(&cachedPage->initiative, tmpString);
            ls_utf32AppendChar(&cachedPage->initiative, (u32)'M');
            ls_utf32AppendBuffer(&cachedPage->initiative, Init.data + endIdx, Init.len - endIdx);
            
            return;
        }
        else // +12 ...
        {
            initOne = ls_utf32ToInt({Init.data, (u32)endIdx, (u32)endIdx});
            initOne = (initOne - dexBonusOld) + dexBonusNew;
            
            if(initOne >= 0) ls_utf32AppendChar(&cachedPage->initiative, (u32)'+');
            ls_utf32FromInt_t(&tmpString, initOne);
            ls_utf32Append(&cachedPage->initiative, tmpString);
            ls_utf32AppendBuffer(&cachedPage->initiative, Init.data + endIdx, Init.len - endIdx);
            
            return;
        }
    }
    
    LogMsg(FALSE, "Missed case of initiative.\n");
    ls_utf32Set(&cachedPage->initiative, Init);
    return;
}

b32 AddMobOnClick(UIContext *, void *);
b32 AddAllyOnClick(UIContext *, void *);
b32 CompendiumAddPageToInitMob(UIContext *c, void *userData)
{
    if(!compendium.isViewingPage)  return FALSE;
    if(compendium.pageIndex == -1) return FALSE;
    
    InitField *f = State.Init->MobFields + State.Init->Mobs.selectedIndex;
    
    ls_uiTextBoxSet(c, &f->maxLife, cachedPage.HP);
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_NAME], cachedPage.name);
    f->compendiumIdx = compendium.pageIndex;
    
    AddMobOnClick(NULL, NULL);
    
    return TRUE;
}

b32 CompendiumAddPageToInitAlly(UIContext *c, void *userData)
{
    if(!compendium.isViewingPage)  return FALSE;
    if(compendium.pageIndex == -1) return FALSE;
    
    InitField *f = State.Init->AllyFields + State.Init->Allies.selectedIndex;
    
    ls_uiTextBoxSet(c, &f->maxLife, cachedPage.HP);
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_NAME], cachedPage.name);
    f->compendiumIdx = compendium.pageIndex;
    
    AddAllyOnClick(NULL, NULL);
    
    return TRUE;
}

void initCachedPage(CachedPageEntry *cachedPage)
{
    const u32 maxSubtypes    = 8;
    const u32 maxArchetypes  = 4;
    const u32 maxSenses      = 8;
    const u32 maxImmunities  = 16;
    const u32 maxResistances = 16;
    const u32 maxWeaknesses  = 16;
    const u32 maxTalents     = 24;
    const u32 maxSkills      = 24;
    const u32 maxLanguages   = 24;
    const u32 maxSpecials    = 24;
    
    cachedPage->origin            = ls_utf32Alloc(72);
    cachedPage->shortDesc         = ls_utf32Alloc(512);
    cachedPage->AC                = ls_utf32Alloc(192);
    cachedPage->HP                = ls_utf32Alloc(128);
    cachedPage->ST                = ls_utf32Alloc(128);
    cachedPage->RD                = ls_utf32Alloc(128);
    cachedPage->RI                = ls_utf32Alloc(128);
    cachedPage->defensiveCapacity = ls_utf32Alloc(128);
    cachedPage->melee             = ls_utf32Alloc(320);
    cachedPage->ranged            = ls_utf32Alloc(256);
    cachedPage->specialAttacks    = ls_utf32Alloc(448);
    cachedPage->psych             = ls_utf32Alloc(2048);
    cachedPage->magics            = ls_utf32Alloc(2048);
    cachedPage->spells            = ls_utf32Alloc(2048);
    cachedPage->racialMods        = ls_utf32Alloc(128);
    cachedPage->spec_qual         = ls_utf32Alloc(320);
    
    cachedPage->specials          = ls_utf32Alloc(maxTalents * 2048);
    
    cachedPage->org               = ls_utf32Alloc(512);
    cachedPage->treasure          = ls_utf32Alloc(320);
    cachedPage->desc              = ls_utf32Alloc(8192);
    cachedPage->source            = ls_utf32Alloc(256);
    
    cachedPage->name              = ls_utf32Alloc(48);
    cachedPage->gs                = ls_utf32Alloc(16);
    cachedPage->pe                = ls_utf32Alloc(16);
    cachedPage->alignment         = ls_utf32Alloc(32);
    cachedPage->type              = ls_utf32Alloc(32);
    cachedPage->subtype           = ls_utf32Alloc(maxSubtypes * 16);
    cachedPage->archetype         = ls_utf32Alloc(maxArchetypes * 16);
    cachedPage->size              = ls_utf32Alloc(32);
    cachedPage->initiative        = ls_utf32Alloc(32);
    cachedPage->senses            = ls_utf32Alloc(maxSenses * 32);
    cachedPage->perception        = ls_utf32Alloc(128);
    cachedPage->aura              = ls_utf32Alloc(128);
    
    cachedPage->immunities        = ls_utf32Alloc(maxImmunities * 32);
    cachedPage->resistances       = ls_utf32Alloc(maxResistances * 32);;
    cachedPage->weaknesses        = ls_utf32Alloc(maxWeaknesses * 32);;
    
    cachedPage->speed             = ls_utf32Alloc(96);
    cachedPage->space             = ls_utf32Alloc(32);
    cachedPage->reach             = ls_utf32Alloc(64);
    
    //TODO: Pre-merge all these, doens't make sense not to.
    cachedPage->STR               = ls_utf32Alloc(32);
    cachedPage->DEX               = ls_utf32Alloc(32);
    cachedPage->CON               = ls_utf32Alloc(32);
    cachedPage->INT               = ls_utf32Alloc(32);
    cachedPage->WIS               = ls_utf32Alloc(32);
    cachedPage->CHA               = ls_utf32Alloc(32);
    cachedPage->BAB               = ls_utf32Alloc(32);
    cachedPage->BMC               = ls_utf32Alloc(64);
    cachedPage->DMC               = ls_utf32Alloc(96);
    
    cachedPage->talents           = ls_utf32Alloc(maxTalents * 32);
    cachedPage->skills            = ls_utf32Alloc(maxTalents * 32);
    cachedPage->languages         = ls_utf32Alloc(maxTalents * 64);
    cachedPage->environment       = ls_utf32Alloc(96);
}

void LoadCompendium(string path)
{
    ls_arenaUse(compendiumArena);
    
    //NOTE: First Initialize the cached page to avoid constant alloc/free when setting it
    initCachedPage(&cachedPage);
    
    //NOTE: Now load the Compendium from file
    buffer CompendiumBuff = ls_bufferInitFromFile(path);
    
    //NOTE: No valid file was found.
    if(CompendiumBuff.data == 0)
    {
        compendium.codex.names          = ls_bufferInit(128);
        compendium.codex.gs             = ls_bufferInit(128);
        compendium.codex.types          = ls_bufferInit(128);
        compendium.codex.subtypes       = ls_bufferInit(128);
        
        compendium.codex.numericValues  = ls_bufferInit(128);
        compendium.codex.pe             = ls_bufferInit(128);
        compendium.codex.alignment      = ls_bufferInit(128);
        compendium.codex.archetypes     = ls_bufferInit(128);
        compendium.codex.senses         = ls_bufferInit(128);
        compendium.codex.auras          = ls_bufferInit(128);
        compendium.codex.immunities     = ls_bufferInit(128);
        compendium.codex.resistances    = ls_bufferInit(128);
        compendium.codex.weaknesses     = ls_bufferInit(128);
        compendium.codex.specialAttacks = ls_bufferInit(128);
        compendium.codex.spells         = ls_bufferInit(128);
        compendium.codex.talents        = ls_bufferInit(128);
        compendium.codex.skills         = ls_bufferInit(128);
        compendium.codex.languages      = ls_bufferInit(128);
        compendium.codex.environment    = ls_bufferInit(128);
        compendium.codex.specials       = ls_bufferInit(128);
    }
    else
    {
        const u32 reserve = 32;
        
        //NOTE: We point to the beginning of the block, which contains a 4 bytes size
        //      And also copy the size into the buffer struct itself. This is necessary
        //      Because all the indices in the Page Entries are relative to the complete block, size included
        auto viewIntoBuffer = [reserve](buffer *src, buffer *dst) {
            u8 *blockBegin       = (u8 *)src->data + src->cursor;
            u32 blockSize        = ls_bufferReadDWord(src);
            *dst                 = ls_bufferViewIntoPtr((void *)blockBegin, blockSize);
            ls_bufferReadSkip(src, blockSize);
        };
        
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.generalStrings);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.numericValues);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.names);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.gs);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.pe);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.alignment);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.types);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.subtypes);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.archetypes);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.sizes);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.senses);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.auras);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.immunities);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.resistances);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.weaknesses);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.specialAttacks);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.spells);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.talents);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.skills);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.languages);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.environment);
        viewIntoBuffer(&CompendiumBuff, &compendium.codex.specials);
        
        //TODO: Change this to a FixedArray 
        u32 entryCount = ls_bufferReadDWord(&CompendiumBuff);
        u8 *pagesSrc = (u8 *)CompendiumBuff.data + CompendiumBuff.cursor;
        ls_arrayFromPointer(&compendium.codex.pages, (void *)pagesSrc, entryCount);
    }
    
    const u32 currentViewIndicesCount = 2496;
    compendium.viewIndices = ls_arrayAlloc<u16>(currentViewIndicesCount);
    for(u16 i = 0; i < currentViewIndicesCount; i++)
    { ls_arrayAppend(&compendium.viewIndices, i); }
    
    ls_arenaUse(globalArena);
    
    return;
}

void AppendEntryFromBuffer(buffer *buf, utf32 *base, const char32_t *sep, u32 index)
{
    if(index == 0) { AssertMsg(FALSE, "We shouldn't be here\n"); return; } //NOTE: Index zero means no entry
    
    buf->cursor = index;
    
    u32 byteLen   = (u32)ls_bufferPeekWord(buf);
    u8 *utf8_data = (u8 *)buf->data + buf->cursor + 2;
    utf8 toAppend = ls_utf8Constant(utf8_data, byteLen);
    u32 sepLen    = 0;
    
    if(sep) { sepLen = ls_utf32Len(sep); }
    
    if(base->len + toAppend.len + sepLen > base->size)
    { ls_printf("Fuck Size: %d, Len: %d, ByteLen: %d, Index: %d\n", base->size, toAppend.len, byteLen, index);
        AssertMsg(FALSE, ""); }
    
    if(sep)
    {
        ls_memcpy((void *)sep, base->data + base->len, sepLen*4);
        base->len += sepLen;
    }
    
    for(u32 i = 0; i < toAppend.len; i++)
    {
        u32 c = ls_utf32CharFromUtf8(toAppend, i);
        base->data[base->len + i] = c;
    }
    base->len += toAppend.len;
    
    ls_bufferSeekBegin(buf);
}

void AppendEntryFromBuffer(buffer *buf, utf32 *base, const char32_t *sep, u16 index)
{
    AppendEntryFromBuffer(buf, base, sep, (u32)index);
}

void GetEntryFromBuffer_t(buffer *buf, utf32 *toSet, u32 index)
{
    if(index == 0) { toSet->len = 0; return; } //NOTE: Index zero means no entry
    
    buf->cursor = index;
    
    s32 byteLen   = ls_bufferPeekWord(buf);
    u8 *utf8_data = (u8 *)buf->data + buf->cursor + 2;
    
    u32 len = ls_utf8Len(utf8_data, byteLen);
    
    LogMsgF(toSet->size >= len, "Fuck Size: %d, Len: %d, ByteLen: %d, Index: %d\n", toSet->size, len, byteLen, index);
    
    ls_utf32FromUTF8_t(toSet, utf8_data, len);
    
    ls_bufferSeekBegin(buf);
}

void GetEntryFromBuffer_t(buffer *buf, utf32 *toSet, u16 index)
{
    GetEntryFromBuffer_t(buf, toSet, (u32)index);
}

utf8 GetEntryFromBuffer_8(buffer *buf, u32 index)
{
    if(index == 0) { return {}; } //NOTE: Index zero means no entry
    
    buf->cursor = index;
    
    u32 byteLen   = (u32)ls_bufferPeekWord(buf);
    u8 *utf8_data = (u8 *)buf->data + buf->cursor + 2;
    
    utf8 result = ls_utf8Constant(utf8_data, (s32)byteLen);
    
    ls_bufferSeekBegin(buf);
    
    return result;
}

utf8 GetEntryFromBuffer_8(buffer *buf, u16 index)
{
    return GetEntryFromBuffer_8(buf, (u32)index);
}

void SetMonsterTable(UIContext *c)
{
    Codex *codex = &compendium.codex;
    
    s32 monsterTableMinY = -((codex->pages.count-30) * 19);
    tableScroll = { 0, 10, c->windowWidth - 4, c->windowHeight - 36, 0, 0, c->windowWidth - 32, monsterTableMinY };
    
    compendium.searchBar.text = ls_utf32Alloc(64);
    compendium.searchBar.postInput = CompendiumSearchFunction;
    compendium.searchBar.isSingleLine = TRUE;
    
    ls_uiSelectFontByFontSize(c, FS_SMALL);
    
    return;
}

void GetEntryAndConvertAC(buffer *buf, utf32 *toSet, u32 index)
{
    if(index == 0) { toSet->len = 0; return; } //NOTE: Index zero means no entry
    
    buf->cursor = index;
    
    s32 byteLen   = ls_bufferPeekWord(buf);
    u8 *utf8_data = (u8 *)buf->data + buf->cursor + 2;
    
    u32 len = ls_utf8Len(utf8_data, byteLen);
    
    LogMsgF(toSet->size >= len, "Fuck Size: %d, Len: %d, ByteLen: %d, Index: %d\n", toSet->size, len, byteLen, index);
    
    //NOTE: Parse the AC line, and convert every element to the Prana Ruleset
    
    utf8 s = ls_utf8Constant((u8 *)utf8_data, byteLen);
    
    AssertMsg(FALSE, "Not Implemented Yet\n");
    
    //ls_utf32FromUTF8_t(toSet, utf8_data, len);
    
    ls_bufferSeekBegin(buf);
}

void CachePage(PageEntry page, s32 viewIndex, CachedPageEntry *cachedPage)
{
    u32 tempUTF32Buffer[256] = {};
    utf32 tempString = { tempUTF32Buffer, 0, 256 };
    
    Codex *c = &compendium.codex;
    
    cachedPage->pageIndex = viewIndex;
    
    //TODO: What's missing (apart from possible bug fixes)
    //      TxC + Dmg (There's a lot of exceptions with weapons and it's shit. This is for laater)
    //      Maybe DC for Channeling Energy on clerics with Carisma?
    //      Maybe AC Deviation bonus for specific Race/Class combos?
    //      Skill Checks On Any Ability
    
    //NOTE: Everything tries to be ordered like the struct, to be organized
    //      But I need to have these stats earlier because other paramaters depend on them
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->STR, page.STR);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->DEX, page.DEX);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->CON, page.CON);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->INT, page.INT);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->WIS, page.WIS);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->CHA, page.CHA);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->BAB, page.BAB);
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->origin, page.origin);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->shortDesc, page.shortDesc);
    GetEntryFromBuffer_t(&c->sizes, &cachedPage->size, page.size);
    
    GetEntryFromBuffer_t(&c->generalStrings, &tempString, page.AC);
    CalculateAndCacheAC(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->generalStrings, &tempString, page.HP);
    CalculateAndCacheHP(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->generalStrings, &tempString, page.ST);
    CalculateAndCacheST(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->numericValues, &tempString, page.BMC);
    CalculateAndCacheBMC(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->numericValues, &tempString, page.DMC);
    CalculateAndCacheDMC(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->numericValues, &tempString, page.initiative);
    CalculateAndCacheInitiative(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->RD, page.RD);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->RI, page.RI);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->defensiveCapacity, page.defensiveCapacity);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->melee, page.melee);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->ranged, page.ranged);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->specialAttacks, page.specialAttacks);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->psych, page.psych);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->magics, page.magics);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->spells, page.spells);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->racialMods, page.racialMods);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->spec_qual, page.spec_qual);
    
    ls_utf32Clear(&cachedPage->specials);
    if(page.specials[0])
    {
        GetEntryFromBuffer_t(&c->specials, &cachedPage->specials, page.specials[0]);
        u32 i = 1;
        while(page.specials[i] && i < 24)
        {
            AppendEntryFromBuffer(&c->specials, &cachedPage->specials, U"\n\n", page.specials[i]);
            i += 1;
        }
    }
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->org, page.org);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->treasure, page.treasure);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->desc, page.desc);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->source, page.source);
    
    GetEntryFromBuffer_t(&c->names, &cachedPage->name, page.name);
    GetEntryFromBuffer_t(&c->gs, &cachedPage->gs, page.gs);
    GetEntryFromBuffer_t(&c->pe, &cachedPage->pe, page.pe);
    GetEntryFromBuffer_t(&c->alignment, &cachedPage->alignment, page.alignment);
    GetEntryFromBuffer_t(&c->types, &cachedPage->type, page.type);
    
    ls_utf32Clear(&cachedPage->subtype);
    if(page.subtype[0])
    {
        ls_utf32Append(&cachedPage->subtype, ls_utf32Constant(U" ("));
        AppendEntryFromBuffer(&c->subtypes, &cachedPage->subtype, NULL, page.subtype[0]);
        u32 i = 1;
        while(page.subtype[i] && i < 8)
        {
            AppendEntryFromBuffer(&c->subtypes, &cachedPage->subtype, U", ", page.subtype[i]);
            i += 1;
        }
        ls_utf32Append(&cachedPage->subtype, ls_utf32Constant(U") "));
    }
    
    ls_utf32Clear(&cachedPage->archetype);
    if(page.archetype[0])
    {
        ls_utf32Append(&cachedPage->archetype, ls_utf32Constant(U"["));
        AppendEntryFromBuffer(&c->archetypes, &cachedPage->archetype, NULL, page.archetype[0]);
        u32 i = 1;
        while(page.archetype[i] && i < 4)
        {
            AppendEntryFromBuffer(&c->archetypes, &cachedPage->archetype, U", ", page.archetype[i]);
            i += 1;
        }
        ls_utf32Append(&cachedPage->archetype, ls_utf32Constant(U"] "));
    }
    
    ls_utf32Clear(&cachedPage->senses);
    if(page.senses[0])
    {
        GetEntryFromBuffer_t(&c->senses, &cachedPage->senses, page.senses[0]);
        u32 i = 1;
        while(page.senses[i] && i < 8)
        {
            AppendEntryFromBuffer(&c->senses, &cachedPage->senses, U", ", page.senses[i]);
            i += 1;
        }
    }
    
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->perception, page.perception);
    GetEntryFromBuffer_t(&c->auras, &cachedPage->aura, page.aura);
    
    ls_utf32Clear(&cachedPage->immunities);
    if(page.immunities[0])
    {
        GetEntryFromBuffer_t(&c->immunities, &cachedPage->immunities, page.immunities[0]);
        u32 i = 1;
        while(page.immunities[i] && i < 16)
        {
            AppendEntryFromBuffer(&c->immunities, &cachedPage->immunities, U", ", page.immunities[i]);
            i += 1;
        }
    }
    
    ls_utf32Clear(&cachedPage->resistances);
    if(page.resistances[0])
    {
        GetEntryFromBuffer_t(&c->resistances, &cachedPage->resistances, page.resistances[0]);
        u32 i = 1;
        while(page.resistances[i] && i < 16)
        {
            AppendEntryFromBuffer(&c->resistances, &cachedPage->resistances, U", ", page.resistances[i]);
            i += 1;
        }
    }
    
    ls_utf32Clear(&cachedPage->weaknesses);
    if(page.weaknesses[0])
    {
        GetEntryFromBuffer_t(&c->weaknesses, &cachedPage->weaknesses, page.weaknesses[0]);
        u32 i = 1;
        while(page.weaknesses[i] && i < 16)
        {
            AppendEntryFromBuffer(&c->weaknesses, &cachedPage->weaknesses, U", ", page.weaknesses[i]);
            i += 1;
        }
    }
    
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->speed, page.speed);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->space, page.space);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->reach, page.reach);
    
    ls_utf32Clear(&cachedPage->talents);
    if(page.talents[0])
    {
        GetEntryFromBuffer_t(&c->talents, &cachedPage->talents, page.talents[0]);
        u32 i = 1;
        while(page.talents[i] && i < 24)
        {
            AppendEntryFromBuffer(&c->talents, &cachedPage->talents, U", ", page.talents[i]);
            i += 1;
        }
    }
    
    ls_utf32Clear(&cachedPage->skills);
    if(page.skills[0])
    {
        GetEntryFromBuffer_t(&c->skills, &cachedPage->skills, page.skills[0]);
        u32 i = 1;
        while(page.skills[i] && i < 24)
        {
            AppendEntryFromBuffer(&c->skills, &cachedPage->skills, U", ", page.skills[i]);
            i += 1;
        }
    }
    
    ls_utf32Clear(&cachedPage->languages);
    if(page.languages[0])
    {
        GetEntryFromBuffer_t(&c->languages, &cachedPage->languages, page.languages[0]);
        u32 i = 1;
        while(page.languages[i] && i < 24)
        {
            AppendEntryFromBuffer(&c->languages, &cachedPage->languages, U", ", page.languages[i]);
            i += 1;
        }
    }
    
    GetEntryFromBuffer_t(&c->environment, &cachedPage->environment, page.environment);
}

//TODO: Make Certain Fields modifiable directly??
s32 DrawPage(UIContext *c, CachedPageEntry *page, s32 baseX, s32 baseY, s32 maxW, s32 minY)
{
    //NOTE: Used to adjust the next line position when changing font size, because
    //      a change in pixel height will make the next line too close / too far from the ideal position.
    s32 prevPixelHeight = 0;
    s32 currPixelHeight = 0;
    
    Color pureWhite = RGBg(0xFF);
    c->textColor    = RGBg(0xAA);
    
    UIRect baseR  = { c->scroll.x + 10, baseY, maxW, minY };
    UIRect alignR = { baseX, baseY, maxW, minY };
    UIRect offset = {};
    
    s32 hSepWidth = maxW - baseX;
    
    auto renderAndAlignWS = [&](const char32_t *s) {
        offset   = ls_uiLabelLayout(c, s, alignR);
        alignR.x = offset.x;
        alignR.y = offset.y;
        baseR.y  = offset.y;
    };
    
    auto renderAndAlignW = [&](utf32 s) {
        offset   = ls_uiLabelLayout(c, s, alignR);
        alignR.x = offset.x;
        alignR.y = offset.y;
        baseR.y  = offset.y;
    };
    
    auto renderAndAlignS = [&](const char32_t *s) {
        alignR   = baseR;
        offset   = ls_uiLabelLayout(c, s, alignR, pureWhite);
        alignR.x = offset.x;
        alignR.y = offset.y;
    };
    
    auto renderAndAlign = [&](utf32 s) {
        offset   = ls_uiLabelLayout(c, s, alignR);
        baseR.y -= offset.h;
    };
    
    //---------------//
    //    HEADER     //
    //---------------//
    prevPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    ls_uiLabelLayout(c, page->name, baseR, pureWhite);
    {
        ls_uiLabelLayout(c, U"GS", { baseR.x + 495, baseR.y, maxW, minY }, pureWhite);
        ls_uiLabelLayout(c, page->gs, { baseR.x + 525, baseR.y, maxW, minY }, pureWhite);
        ls_uiLabelLayout(c, U"PE", { baseR.x + 620, baseR.y, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->pe, { baseR.x + 655, baseR.y, maxW, minY }, pureWhite);
        ls_uiHSeparator(c, baseR.x, baseR.y-4, hSepWidth, 1, RGB(0, 0, 0));
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseR.y += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        baseR.y -= offset.h;
        
        if(page->origin.len)
        {
            offset = ls_uiLabelLayout(c, page->origin, baseR, pureWhite);
            baseR.y -= offset.h;
        }
        
        if(page->shortDesc.len)
        {
            offset = ls_uiLabelLayout(c, page->shortDesc, baseR);
            baseR.y -= (offset.h + 4);
        }
        
        renderAndAlignS(U"Allineamento: ");
        renderAndAlign(page->alignment);
        
        renderAndAlignS(U"Categoria: ");
        renderAndAlignW(page->type);
        
        if(page->subtype.len)   renderAndAlignW(page->subtype);
        else                    renderAndAlignWS(U" ");
        if(page->archetype.len) renderAndAlignW(page->archetype);
        else                    renderAndAlignWS(U" ");
        if(page->size.len)      renderAndAlignW(page->size);
        
        baseR.y -= offset.h;
        
        renderAndAlignS(U"Iniziativa: ");
        renderAndAlign(page->initiative);
        
        alignR = baseR;
        renderAndAlignS(U"Sensi: ");
        if(page->senses.len) renderAndAlignW(page->senses);
        
        //TODO: This breaks when Perception starts too close to the limit line,
        //      To be fixed the layout system has to use a 6 point bound, rather than just 4 points
        //      So that we have a minimumX and a baseX, to know where to position the newline. @NewLayout
        
        //TODO: This is because the hypergol parser will miss Perception, if the golarion input is not perfect.
        if(page->perception.len)
        {
            offset = ls_uiLabelLayout(c, U"Percezione ", alignR); alignR.x = offset.x;
            renderAndAlign(page->perception);
        }
        else { baseR.y -= offset.h; }
        
        if(page->aura.len)
        {
            renderAndAlignS(U"Aura: ");
            renderAndAlign(page->aura);
        }
    }
    
    
    //---------------//
    //    DEFENSE    //
    //---------------//
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseR.y -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseR.y -= 4;
    baseX  = 164;
    {
        offset = ls_uiLabelLayout(c, U"Difesa", baseR, pureWhite);
        ls_uiHSeparator(c, baseR.x, baseR.y-4, hSepWidth, 1, RGB(0, 0, 0));
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseR.y += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        baseR.y -= offset.h;
        
        if(page->acHasArmor == TRUE)
        {
            c->textColor = RGB(0xCC, 0x22, 0x22);
            renderAndAlignS(U"CA: ");
            renderAndAlign(page->AC);
            c->textColor = RGBg(0xAA);
        }
        else {
            renderAndAlignS(U"CA: ");
            renderAndAlign(page->AC);
        }
        
        renderAndAlignS(U"PF: ");
        renderAndAlign(page->HP);
        
        renderAndAlignS(U"Tiri Salvezza: ");
        renderAndAlign(page->ST);
        
        if(page->RD.len)
        {
            renderAndAlignS(U"RD: ");
            renderAndAlign(page->RD);
        }
        
        if(page->RI.len)
        {
            renderAndAlignS(U"RI: ");
            renderAndAlign(page->RI);
        }
        
        if(page->immunities.len)
        {
            renderAndAlignS(U"Immunit\U000000E0: ");
            renderAndAlign(page->immunities);
        }
        
        if(page->resistances.len)
        {
            renderAndAlignS(U"Resistenze: ");
            renderAndAlign(page->resistances);
        }
        
        if(page->defensiveCapacity.len)
        {
            renderAndAlignS(U"Capacit\U000000E0 Difensive: ");
            renderAndAlign(page->defensiveCapacity);
        }
        
        if(page->weaknesses.len)
        {
            renderAndAlignS(U"Debolezze: ");
            renderAndAlign(page->weaknesses);
        }
    }
    
    //---------------//
    //    ATTACK     //
    //---------------//
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseR.y -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseR.y -= 4;
    baseX  = 200;
    {
        offset = ls_uiLabelLayout(c, U"Attacco", baseR, pureWhite);
        ls_uiHSeparator(c, baseR.x, baseR.y-4, hSepWidth, 1, RGB(0, 0, 0));
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseR.y += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        baseR.y -= offset.h;
        
        if(page->speed.len)
        {
            renderAndAlignS(U"Velocit\U000000E0: ");
            renderAndAlign(page->speed);
        }
        
        if(page->melee.len)
        {
            renderAndAlignS(U"Mischia: ");
            renderAndAlign(page->melee);
        }
        
        if(page->ranged.len)
        {
            renderAndAlignS(U"Distanza: ");
            renderAndAlign(page->ranged);
        }
        
        if(page->specialAttacks.len)
        {
            renderAndAlignS(U"Attacchi Speciali: ");
            renderAndAlign(page->specialAttacks);
        }
        
        if(page->space.len)
        {
            renderAndAlignS(U"Spazio: ");
            renderAndAlign(page->space);
        }
        
        if(page->reach.len)
        {
            renderAndAlignS(U"Portata: ");
            renderAndAlign(page->reach);
        }
        
        if(page->psych.len)
        {
            renderAndAlignS(U"Magia Psichica: ");
            renderAndAlign(page->psych);
        }
        
        if(page->magics.len)
        {
            renderAndAlignS(U"Capacit\U000000E0 Magiche: ");
            renderAndAlign(page->magics);
        }
        
        if(page->spells.len)
        {
            renderAndAlignS(U"Incantesimi Conosciuti: ");
            renderAndAlign(page->spells);
        }
    }
    
    //---------------//
    //     STATS     //
    //---------------//
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseR.y -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseR.y -= 4;
    baseX  = 148;
    {
        offset = ls_uiLabelLayout(c, U"Statistiche", baseR, pureWhite);
        ls_uiHSeparator(c, baseR.x, baseR.y-4, hSepWidth, 1, RGB(0, 0, 0));
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseR.y += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        baseR.y -= offset.h;
        
        renderAndAlignS(U"Caratteristiche: ");
        renderAndAlignWS(U"FOR ");   renderAndAlignW(page->STR);
        renderAndAlignWS(U", DEX "); renderAndAlignW(page->DEX);
        renderAndAlignWS(U", COS "); renderAndAlignW(page->CON);
        renderAndAlignWS(U", INT "); renderAndAlignW(page->INT);
        renderAndAlignWS(U", SAG "); renderAndAlignW(page->WIS);
        renderAndAlignWS(U", CAR "); renderAndAlign(page->CHA);
        
        renderAndAlignS(U"BAB: ");
        renderAndAlign(page->BAB);
        
        renderAndAlignS(U"BMC: ");
        renderAndAlign(page->BMC);
        
        renderAndAlignS(U"DMC: ");
        renderAndAlign(page->DMC);
        
        if(page->talents.len)
        {
            renderAndAlignS(U"Talenti: ");
            renderAndAlign(page->talents);
        }
        
        if(page->skills.len)
        {
            renderAndAlignS(U"Abilit\U000000E0: ");
            renderAndAlign(page->skills);
        }
        
        if(page->languages.len)
        {
            renderAndAlignS(U"Linguaggi: ");
            renderAndAlign(page->languages);
        }
        
        if(page->racialMods.len)
        {
            renderAndAlignS(U"Modificatori Razziali: ");
            renderAndAlign(page->racialMods);
        }
        
        if(page->spec_qual.len)
        {
            renderAndAlignS(U"Qualit\U000000E0 Speciali: ");
            renderAndAlign(page->spec_qual);
        }
        
    }
    
    //---------------//
    //   SPECIALS    //
    //---------------//
    
    if(page->specials.len)
    {
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
        baseR.y -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
        baseR.y -= 4;
        {
            offset = ls_uiLabelLayout(c, U"Capacit\U000000E0 Speciali:", baseR, pureWhite);
            ls_uiHSeparator(c, baseR.x, baseR.y-4, hSepWidth, 1, RGB(0, 0, 0));
            
            currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
            baseR.y += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
            baseR.y -= offset.maxY;
            baseR.y -= 8; //NOTE: Random spacing because it feels cramped
            
            offset = ls_uiLabelLayout(c, page->specials, baseR);
            baseR.y -= (offset.h + 8);
        }
    }
    //---------------//
    //      ORG      //
    //---------------//
    
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseR.y -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseR.y -= 4;
    baseX  = 148;
    {
        offset = ls_uiLabelLayout(c, U"Ecologia", baseR, pureWhite);
        ls_uiHSeparator(c, baseR.x, baseR.y-4, hSepWidth, 1, RGB(0, 0, 0));
        baseR.y -= offset.h;
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseR.y += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        
        renderAndAlignS(U"Ambiente: ");
        renderAndAlign(page->environment);
        
        renderAndAlignS(U"Organizzazione: ");
        renderAndAlign(page->org);
        
        renderAndAlignS(U"Tesoro: ");
        renderAndAlign(page->treasure);
    }
    
    //---------------//
    //     FINAL     //
    //---------------//
    currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
    baseR.y -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
    baseR.y -= 4;
    {
        offset = ls_uiLabelLayout(c, U"Descrizione", baseR, pureWhite);
        ls_uiHSeparator(c, baseR.x, baseR.y-4, hSepWidth, 1, RGB(0, 0, 0));
        baseR.y -= offset.maxY;
        
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
        baseR.y += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
        
        offset = ls_uiLabelLayout(c, page->desc, baseR);
        baseR.y -= offset.maxY;
        
        baseR.y += (currPixelHeight-1);
        ls_uiHSeparator(c, baseR.x, baseR.y-4, hSepWidth, 1, RGB(0, 0, 0));
        
        
        //TODO: Make these links openable.
        //      To do it we need to call this function
        //      https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shellexecutea
        //
        //      Leaving the "verb" (second param) null and passing the http link should open the page in the
        //      default browser.
        //
        //      If that doesn't work, windows in both CommandPrompt and Powershell has a "start" command
        //      that opens whatever path you give it with the appropriate command. So an http link should get
        //      opened in the default browser.
        
        baseR.y -= (currPixelHeight+8);
        ls_uiLabelLayout(c, U"Fonte: ", baseR, pureWhite);
        offset = ls_uiLabelLayout(c, page->source, { baseR.x + 58, baseR.y, maxW, minY });
        baseR.y -= offset.maxY;
    }
    
    c->textColor = RGBg(0xCC);
    
    return baseR.y;
}

void DrawMonsterTable(UIContext *c)
{
    Input *UserInput = &c->UserInput;
    Codex *codex     = &compendium.codex;
    
    Color bkgColor = RGBg(0x40);
    
    s32 baseX = 20;
    s32 baseY = 630;
    
    ls_uiTextBox(c, &compendium.searchBar, baseX, baseY + 40, 200, 20);
    
    ls_uiStartScrollableRegion(c, &tableScroll);
    
    s32 startI = -(tableScroll.deltaY / 19);
    s32 endI   = startI+36 < compendium.viewIndices.count ? startI+36 : compendium.viewIndices.count;
    endI       = endI < codex->pages.count ? endI : codex->pages.count;
    
    //TODO: BUG. Wrong index when Selecting, Searching, Selecting.
    for(s32 i = startI; i < endI; i++)
    {
        PageEntry entry = codex->pages[compendium.viewIndices[i]];
        Color hoverColor = bkgColor;
        
        if(LeftClickIn(baseX-4, baseY-4, 300, 18)) //TODONOTE: I don't like it...
        {
            compendium.isViewingPage = TRUE; 
            compendium.pageIndex     = i;
        }
        
        if(MouseInRect(baseX-4, baseY-4, 300, 18)) { hoverColor = RGBg(0x66); }
        
        //TODO: Matriarca delle Scimmie Cappuccine Corallo is too long...
        ls_uiRect(c, baseX-4, baseY+tableScroll.deltaY-4, 300, 20, hoverColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->names, entry.name), baseX, baseY+tableScroll.deltaY, 1);
        baseX += 299;
        
        ls_uiRect(c, baseX-4, baseY+tableScroll.deltaY-4, 80, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->gs, entry.gs), baseX, baseY+tableScroll.deltaY, 1);
        baseX += 79;
        
        ls_uiRect(c, baseX-4, baseY+tableScroll.deltaY-4, 180, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->types, entry.type), baseX, baseY+tableScroll.deltaY, 1);
        baseX += 179;
        
        //TODO: Show all subtypes. Right now we only grab one.
        //      We can do this by overloading the function GetEntryFromBuffer and passing the array itself.
        ls_uiRect(c, baseX-4, baseY+tableScroll.deltaY-4, 188, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->subtypes, entry.subtype[0]), baseX, baseY+tableScroll.deltaY, 1);
        baseX += 187;
        
        baseY -= 19;
        baseX = 20;
    }
    
    ls_uiEndScrollableRegion(c);
    
    return;
}

void DrawCompendium(UIContext *c)
{
    //NOTE: This arena is cleared every frame,
    //      Because here we store utf32 strings (currently only for the Search Function)
    ls_arenaClear(compTempArena);
    
    Codex *codex = &compendium.codex;
    Input *UserInput = &c->UserInput;
    
    if(compendium.isViewingPage)
    {
        AssertMsg(compendium.pageIndex != -1, "Page Index was not set\n");
        
        if(KeyHeld(keyMap::Shift) && KeyPressOrRepeat(keyMap::DArrow) && compendium.pageIndex < (codex->pages.count-1))
        { compendium.pageIndex += 1; }
        
        if(KeyHeld(keyMap::Shift) && KeyPressOrRepeat(keyMap::UArrow) && compendium.pageIndex > 0)
        { compendium.pageIndex -= 1; }
        
        if(cachedPage.pageIndex != compendium.pageIndex)
        { 
            PageEntry pEntry = compendium.codex.pages[compendium.viewIndices[compendium.pageIndex]];
            CachePage(pEntry, compendium.pageIndex, &cachedPage);
            
            //NOTE: Reset the page scroll for the new page
            pageScroll = { 0, 10, c->windowWidth - 4, c->windowHeight - 36, 0, 0, c->windowWidth - 32, 0};
        }
        
        //NOTE: The first frame is impossible to scroll, because the minY value will be not initialized yet
        //      It's should be fine though. We run at 30FPS on the Compendium, so it should never be felt/seen.
        //      The minY is set by the DrawPage call itself
        ls_uiStartScrollableRegion(c, &pageScroll);
        pageScroll.minY = DrawPage(c, &cachedPage, 0, 670, c->windowWidth-42, 0);
        ls_uiEndScrollableRegion(c);
    }
    else
    {
        DrawMonsterTable(c);
    }
    
    return;
}