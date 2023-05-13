struct CachedPageEntry
{
    s32 pageIndex    = -1;
    
    b32 acHasArmor = FALSE;
    
    utf32 origin;
    utf32 shortDesc;
    utf32 AC;
    utf32 HP; utf32 totHP;
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
    utf32 tactics_before;
    utf32 tactics_during;
    utf32 tactics_stats;
    utf32 skills; //TODO Separate type from value
    utf32 racialMods;
    utf32 spec_qual;
    utf32 given_equip;
    utf32 properties;
    utf32 boons;
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
    utf32 languages;
    utf32 environment;
};

struct NPCPageEntry
{
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
    u32 tactics[3];
    u32 skills[24]; //TODO Separate type from value
    u32 racialMods;
    u32 spec_qual;
    u32 given_equip;
    u32 properties;
    u32 boons;
    u32 specials[24];
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
    u16 languages[24];
};

struct PageEntry
{
    u32 origin;            //4
    u32 shortDesc;         //8
    u32 AC;                //12
    u32 HP;                //16
    u32 ST;                //20
    u32 RD;                //24
    u32 RI;                //28
    u32 defensiveCapacity; //32
    u32 melee;             //36
    u32 ranged;            //40
    u32 specialAttacks;    //44
    u32 psych;             //48
    u32 magics;            //52
    u32 spells;            //56
    u32 skills[24];        //152    //TODO Separate type from value
    u32 racialMods;        //156
    u32 spec_qual;         //160
    u32 specials[24];      //256
    u32 org;               //260
    u32 treasure;          //264
    u32 desc;              //268
    u32 source;            //272
    
    u16 name;              //274
    u16 gs;                //276
    u16 pe;                //278
    u16 alignment;         //280
    u16 type;              //282
    u16 subtype[8];        //298
    u16 archetype[4];      //306
    u16 size;              //308
    u16 initiative;        //310
    u16 senses[8];         //326
    u16 perception;        //328
    u16 aura;              //330
    u16 immunities[16];    //362
    u16 resistances[16];   //394
    u16 weaknesses[16];    //426
    u16 speed;             //428
    u16 space;             //430
    u16 reach;             //432
    u16 STR;               //434
    u16 DEX;               //436
    u16 CON;               //438
    u16 INT;               //440
    u16 WIS;               //442
    u16 CHA;               //444
    u16 BAB;               //446
    u16 BMC;               //448
    u16 DMC;               //450
    u16 talents[24];       //498
    u16 languages[24];     //546
    u16 environment;       //548
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
    Array<PageEntry>    pages;
    Array<NPCPageEntry> npcPages;
};

const s32 NPC_PAGE_INDEX_OFFSET = 5000;

struct Compendium
{
    Codex codex;
    
    UITextBox  searchBarMobs;
    UITextBox  searchBarNPCs;
    
    b32        isViewingPage;
    s32        pageIndex = -1;
    Array<u16> viewIndices;
    
    //TODO: Hate this, make isViewing* more consistent!
    b32        isViewingNPCTable;
    Array<u16> npcViewIndices;
};

Compendium      compendium        = {};
CachedPageEntry cachedPage        = {};
UIScrollableRegion pageScroll     = {};
UIScrollableRegion tableScroll    = {};
UIScrollableRegion npcTableScroll = {};

//NOTE: Used in Init.cpp
CachedPageEntry mainCachedPage = {};

//NOTE: Kinda hacky but okay.
s32 internal_newToOldMap[] = { -5, -5, -4, -4, -3, -3, -2, -2, -1, -1, 0, 0,
    1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10,
    11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17 ,17
};
s32 *newToOldMap = internal_newToOldMap + 10;

void CachePage(PageEntry, s32, CachedPageEntry *, Status *);
void CachePage(NPCPageEntry, s32, CachedPageEntry *, Status *);
void GetPageEntryAndCache(s32 compendiumIdx, CachedPageEntry *page)
{
    if(compendiumIdx < NPC_PAGE_INDEX_OFFSET)
    { 
        PageEntry pEntry = compendium.codex.pages[compendiumIdx];
        CachePage(pEntry, compendiumIdx, page, NULL);
    }
    else
    { 
        NPCPageEntry pEntry = compendium.codex.npcPages[compendiumIdx - NPC_PAGE_INDEX_OFFSET];
        CachePage(pEntry, compendiumIdx, page, NULL);
    }
}

b32 CompendiumOpenMonsterTable(UIContext *c, void *userData)
{
    compendium.isViewingNPCTable = FALSE;
    compendium.isViewingPage     = FALSE;
    compendium.pageIndex         = -1;
    
    return FALSE;
}

b32 CompendiumOpenNPCTable(UIContext *c, void *userData)
{
    compendium.isViewingNPCTable = TRUE;
    compendium.isViewingPage     = FALSE;
    compendium.pageIndex         = -1;
    
    return FALSE;
}

u16 findNeedleAndFillIndexBuffer(utf8 needle, u16 *namesIndexBuffer, u32 buffSize)
{
    u8 dataBuffer[128] = {};
    utf8 toMatchLower = { dataBuffer, 0, 0, 128 };
    
    buffer *names = &compendium.codex.names;
    names->cursor = 4;
    
    u16 nibCount = 0;
    while(names->cursor < names->size)
    {
        if(nibCount > buffSize)
        { AssertMsg(FALSE, "Index Buffer when searching is not large enough\n"); return nibCount; }
        
        s32 strByteLen = ls_bufferPeekWord(names);
        u8 *data       = (u8 *)names->data + names->cursor + 2;
        
        utf8 toMatch   = ls_utf8Constant(data, strByteLen);
        ls_utf8ToLower(&toMatchLower, toMatch);
        
        if(ls_utf8Contains(toMatchLower, needle))
        {
            namesIndexBuffer[nibCount] = names->cursor;
            nibCount += 1;
        }
        
        ls_bufferReadSkip(names, strByteLen + 2);
    }
    
    return nibCount;
}

b32 CompendiumSearchFunctionMobs(UIContext *c, void *userData)
{
    ls_arenaUse(compTempArena);
    
    //NOTE: Reset the scrollbar when searching
    tableScroll.deltaY = 0;
    
    if(compendium.searchBarMobs.text.len < 2)
    { 
        tableScroll.minY = -((compendium.codex.pages.count-30) * 19);
        ls_arrayClear(&compendium.viewIndices);
        for(u16 i = 0; i < compendium.viewIndices.cap; i++)
        { ls_arrayAppend(&compendium.viewIndices, i); }
        
        return FALSE;
    }
    
    //NOTE:Iterate over the name buffer to find all names which exactly contain the substring needle
    utf8 needle = ls_utf8FromUTF32(compendium.searchBarMobs.text);
    
    u8 dataBuffer[128] = {};
    utf8 needleLower = { dataBuffer, 0, 0, 128 };
    
    ls_utf8ToLower(&needleLower, needle);
    
    u16 namesIndexBuffer[4096] = {};
    u16 nibCount = findNeedleAndFillIndexBuffer(needleLower, namesIndexBuffer, 4096);
    
    //TODO: Put the needle on the stack
    ls_utf8Free(&needle);
    
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

b32 CompendiumSearchFunctionNPCs(UIContext *c, void *userData)
{
    ls_arenaUse(compTempArena);
    
    //NOTE: Reset the scrollbar when searching
    npcTableScroll.deltaY = 0;
    
    if(compendium.searchBarNPCs.text.len < 2)
    { 
        npcTableScroll.minY = -((compendium.codex.npcPages.count-30) * 19);
        ls_arrayClear(&compendium.npcViewIndices);
        for(u16 i = 0; i < compendium.npcViewIndices.cap; i++)
        { ls_arrayAppend(&compendium.npcViewIndices, i); }
        
        return FALSE;
    }
    
    //NOTE:Iterate over the name buffer to find all names which exactly contain the substring needle
    utf8 needle = ls_utf8FromUTF32(compendium.searchBarNPCs.text);
    
    u8 dataBuffer[128] = {};
    utf8 needleLower = { dataBuffer, 0, 0, 128 };
    
    ls_utf8ToLower(&needleLower, needle);
    
    u16 namesIndexBuffer[4096] = {};
    u16 nibCount = findNeedleAndFillIndexBuffer(needleLower, namesIndexBuffer, 4096);
    
    //TODO: Put the needle on the stack
    ls_utf8Free(&needle);
    
    //NOTE: Adjust scrollbar height to new monster table count.
    npcTableScroll.minY = -((nibCount-30) * 19);
    
    if(nibCount > 0)
    {
        ls_arrayClear(&compendium.npcViewIndices);
        
        //NOTE: Now that we have a buffer of all matching name indices, let's search through the actual pages
        for(u16 i = 0; i < compendium.codex.npcPages.count; i++)
        {
            u16 indexToMatch = compendium.codex.npcPages[i].name;
            for(u32 j = 0; j < nibCount; j++)
            {
                if(namesIndexBuffer[j] == indexToMatch)
                {
                    //NOTE: Remove from the namesIndexBuffer and add to the viewIndices array
                    ls_arrayAppend(&compendium.npcViewIndices, i);
                    
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
void CalculateAndCacheAC(utf32 AC, CachedPageEntry *cachedPage, b32 isNPC)
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
    utf32 acExpr    = { AC.data + acExprBegin, acExprLen, acExprLen };
    
    //TODO: Golarion has "armor" and "shield" with small letters... fuck them
    s32 armorBonusIdx  = ls_utf32LeftFind(acExpr, ls_utf32Constant(U"Armatura"));
    if(armorBonusIdx == -1) armorBonusIdx = ls_utf32LeftFind(acExpr, ls_utf32Constant(U"armatura"));
    
    s32 shieldBonusIdx = ls_utf32LeftFind(acExpr, ls_utf32Constant(U"Scudo"));
    if(shieldBonusIdx == -1) { shieldBonusIdx = ls_utf32LeftFind(acExpr, ls_utf32Constant(U"scudo")); }
    
    s32 dexBonusNew = ls_utf32ToInt(cachedPage->DEX) - 10;
    s32 dexBonusOld = newToOldMap[dexBonusNew];
    
    s32 strBonusNew = ls_utf32ToInt(cachedPage->STR) - 10;
    s32 strBonusOld = newToOldMap[strBonusNew];
    
    s32 conBonusNew = ls_utf32ToInt(cachedPage->CON) - 10;
    s32 conBonusOld = newToOldMap[conBonusNew];
    
    s32 newMaxDex      = 999;
    s32 newArmorBonus  = -1;
    s32 newShieldBonus = -1;
    s32 oldArmorBonus  = -1;
    s32 oldShieldBonus = -1;
    
    //TODO: Clear Modifiers to Armor and Shields (Like "Chiodato")
    if(armorBonusIdx != -1)
    {
        Armor *found = NULL;
        s32    index = -1;
        if(isNPC == TRUE)
        {
            for(u32 i = 0; i < armorTableCount; i++)
            {
                Armor *armor = armorTable + i;
                if(ls_utf32LeftFind(cachedPage->properties, armor->name) != -1)
                { found = armor; index = i; break; }
                
                if(ls_utf32LeftFind(cachedPage->given_equip, armor->name) != -1)
                { found = armor; index = i; break; }
            }
        }
        else
        {
            for(u32 i = 0; i < armorTableCount; i++)
            {
                Armor *armor = armorTable + i;
                if(ls_utf32LeftFind(cachedPage->treasure, armor->name) != -1)
                { found = armor; index = i; break; }
            }
        }
        
        if(found)
        {
            if(found->dexMax != -1)
            {
                s32 candidateMaxDex = found->dexMax + s32((conBonusNew + strBonusNew) / 2);
                newMaxDex           = ls_min(newMaxDex, candidateMaxDex);
            }
            oldArmorBonus = found->armorBonus;
            newArmorBonus = armorTablePrana[index].armorBonus;
        }
        else
        {
            cachedPage->acHasArmor = TRUE;
            ls_utf32Set(&cachedPage->AC, AC);
            return;
        }
    }
    
    if(shieldBonusIdx != -1)
    {
        //NOTE: Look for the shield in the treasure line.
        Armor *found = NULL;
        s32    index = -1;
        
        if(isNPC == TRUE)
        {
            for(u32 i = 0; i < shieldTableCount; i++)
            {
                Armor *shield = shieldTable + i;
                if(ls_utf32LeftFind(cachedPage->properties, shield->name) != -1)
                { found = shield; index = i; break; }
                
                if(ls_utf32LeftFind(cachedPage->given_equip, shield->name) != -1)
                { found = shield; index = i; break; }
            }
        }
        else
        {
            for(u32 i = 0; i < shieldTableCount; i++)
            {
                Armor *shield = shieldTable + i;
                if(ls_utf32LeftFind(cachedPage->treasure, shield->name) != -1)
                { found = shield; index = i; break; }
            }
        }
        
        if(found)
        {
            if(found->dexMax != -1)
            {
                s32 candidateMaxDex = found->dexMax + s32((conBonusNew + strBonusNew) / 2);
                newMaxDex           = ls_min(newMaxDex, candidateMaxDex);
            }
            oldShieldBonus = found->armorBonus;
            newShieldBonus = shieldTablePrana[index].armorBonus;
        }
        else
        {
            cachedPage->acHasArmor = TRUE;
            ls_utf32Set(&cachedPage->AC, AC);
            return;
        }
    }
    
    //TODO: I don't really like these hardcoded offsets, would prefer to have a LeftFindNumeric() function
    s32 firstValEnd     = ls_utf32LeftFind(AC, (u32)',');
    s32 secondValBegin  = firstValEnd + 11;
    s32 secondValEnd    = ls_utf32LeftFind(AC, secondValBegin, (u32)',');
    s32 secondValLen    = secondValEnd - secondValBegin;
    s32 thirdValBegin   = secondValEnd + 14;
    s32 semiToken       = ls_utf32LeftFind(AC, thirdValBegin, (u32)';');
    s32 parenToken      = ls_utf32LeftFind(AC, thirdValBegin, (u32)'(');
    s32 spaceToken      = ls_utf32LeftFind(AC, thirdValBegin, (u32)' ');
    
    //TODO: I don't really like these hardcoded offsets, would prefer to have a LeftFindNumeric() function
    s32 thirdValEnd = AC.len - 1;
    if(semiToken != -1)
    {
        if(parenToken != -1)
        {
            if(semiToken < parenToken) { thirdValEnd = semiToken; }
            else                       { thirdValEnd = parenToken - 1; }
        }
        else if(spaceToken != -1)
        {
            if(semiToken < spaceToken) { thirdValEnd = semiToken; }
            else                       { thirdValEnd = spaceToken; }
        }
        else
        {
            thirdValEnd = semiToken - 1;
        }
    }
    else
    {
        if(parenToken != -1)
        {
            if(spaceToken != -1)
            {
                if(parenToken < spaceToken) { thirdValEnd = parenToken; }
                else                        { thirdValEnd = spaceToken; }
            }
            else
            {
                thirdValEnd = parenToken;
            }
        }
        else
        {
            if(spaceToken != -1) { thirdValEnd = spaceToken; }
        }
    }
    s32 thirdValLen = thirdValEnd - thirdValBegin;
    
    if((firstValEnd == -1) || (secondValEnd == -1) || (thirdValEnd == -1)
       || (secondValBegin >= AC.len) || (thirdValBegin >= AC.len))
    {
        cachedPage->acHasArmor = TRUE;
        ls_utf32Set(&cachedPage->AC, AC);
        return;
    }
    
    s32 totAC   = ls_utf32ToInt({AC.data, firstValEnd, firstValEnd});
    s32 touchAC = ls_utf32ToInt({AC.data + secondValBegin, secondValLen, secondValLen});
    s32 flatAC  = ls_utf32ToInt({AC.data + thirdValBegin, thirdValLen, thirdValLen});
    
    //NOTE: Adjust the dex bonus
    s32 dexBonusToAC = ls_min(dexBonusNew, newMaxDex);
    totAC   = (totAC   - dexBonusOld) + dexBonusToAC;
    touchAC = (touchAC - dexBonusOld) + dexBonusToAC;
    flatAC  = (flatAC  - dexBonusOld) + dexBonusToAC;
    
    //NOTE: Adjust the armor bonus
    if(armorBonusIdx)
    {
        totAC  = (totAC - oldArmorBonus) + newArmorBonus;
        flatAC = (flatAC - oldArmorBonus) + newArmorBonus;
    }
    
    if(shieldBonusIdx)
    {
        totAC  = (totAC - oldShieldBonus) + newShieldBonus;
        flatAC = (flatAC - oldShieldBonus) + newShieldBonus;
    }
    
    u32 buff[32] = {};
    utf32 tmpString = { buff, 32, 32 };
    
    ls_utf32Clear(&cachedPage->AC);
    ls_utf32FromInt_t(&cachedPage->AC, totAC);
    ls_utf32Append(&cachedPage->AC, ls_utf32Constant(U", contatto "));
    ls_utf32FromInt_t(&tmpString, touchAC);
    ls_utf32Append(&cachedPage->AC, tmpString);
    ls_utf32Append(&cachedPage->AC, ls_utf32Constant(U", impreparato "));
    ls_utf32FromInt_t(&tmpString, flatAC);
    ls_utf32Append(&cachedPage->AC, tmpString);
    ls_utf32Append(&cachedPage->AC, {AC.data + thirdValEnd, AC.len - thirdValEnd, AC.len - thirdValEnd});
}

void CalculateAndCacheST(utf32 ST, CachedPageEntry *cachedPage)
{
    s32 conBonusNew = ls_utf32ToInt(cachedPage->CON) - 10;
    s32 dexBonusNew = ls_utf32ToInt(cachedPage->DEX) - 10;
    s32 wisBonusNew = ls_utf32ToInt(cachedPage->WIS) - 10;
    s32 conBonusOld = newToOldMap[conBonusNew];
    s32 dexBonusOld = newToOldMap[dexBonusNew];
    s32 wisBonusOld = newToOldMap[wisBonusNew];
    
    //NOTE: Fix for Constructs
    if(ls_utf32AreEqual(cachedPage->CON, ls_utf32Constant(U"-")))
    { conBonusNew = 0; conBonusOld = 0; }
    
    //TODO: Make these not hardcoded like that
    s32 conSaveBegin = 7;
    s32 conSaveEnd   = ls_utf32LeftFind(ST, (u32)',');
    s32 dexSaveBegin = conSaveEnd + 11;
    s32 dexSaveEnd   = ls_utf32LeftFind(ST, dexSaveBegin, (u32)',');
    s32 wisSaveBegin = dexSaveEnd + 10;
    s32 semiToken    = ls_utf32LeftFind(ST, wisSaveBegin, (u32)';');
    s32 parenToken   = ls_utf32LeftFind(ST, wisSaveBegin, (u32)'(');
    
    s32 wisSaveEnd = semiToken - 1;
    if(semiToken != -1 && parenToken != -1 && semiToken > parenToken)
    { wisSaveEnd = parenToken - 1; }
    else if(semiToken == -1)
    {
        wisSaveEnd = parenToken - 1;
        if(parenToken == -1)
        {
            wisSaveEnd = ST.len - 1;
        }
    }
    
    if((conSaveEnd == -1) || (dexSaveEnd == -1)) { ls_utf32Set(&cachedPage->ST, ST); return; }
    
    s32 conSave = ls_utf32ToInt({ST.data + conSaveBegin, conSaveEnd - conSaveBegin, conSaveEnd - conSaveBegin});
    s32 dexSave = ls_utf32ToInt({ST.data + dexSaveBegin, dexSaveEnd - dexSaveBegin, dexSaveEnd - dexSaveBegin});
    s32 wisSave = ls_utf32ToInt({ST.data + wisSaveBegin, wisSaveEnd - wisSaveBegin+1, wisSaveEnd - wisSaveBegin+1});
    
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
    if(hpEndIndex == -1) hpEndIndex = ls_utf32LeftFind(hp, (u32)'(');
    AssertMsg(hpEndIndex >= 0, "HP index can't be found\n");
    
    s32 hpExprBegin = ls_utf32LeftFind(hp, (u32)'(') + 1;
    s32 hpExprLen   = ls_utf32LeftFind(hp, (u32)')') - hpExprBegin;
    
    utf32 hpExpr    = {hp.data + hpExprBegin, hpExprLen, hpExprLen};
    
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
            
            s32 dieValue = ls_utf32ToInt({hpExpr.data, dToken, dToken});
            s32 dieMul   = ls_utf32ToInt({hpExpr.data + dToken+1, mulLen, mulLen});
            s32 flatVal  = ls_utf32ToInt({hpExpr.data + plusToken+1, flatLen, flatLen});
            
            //NOTE: The flatVal is multiplied by 2 because the CON bonus is doubled.
            finalHP  = dieValue*dieMul + (flatVal * 2);
        }
        else if(minusToken != -1)
        {
            s32 mulLen   = minusToken - dToken - 1;
            
            s32 dieValue = ls_utf32ToInt({hpExpr.data, dToken, dToken});
            s32 dieMul   = ls_utf32ToInt({hpExpr.data + dToken+1, mulLen, mulLen});
            s32 flatVal  = ls_utf32ToInt({hpExpr.data + minusToken+1, flatLen, flatLen});
            
            //NOTE: The flatVal is multiplied by 2 because the CON bonus is doubled.
            finalHP  = dieValue*dieMul - (flatVal * 2);
        }
        else
        {
            s32 mulLen   = hpExprLen - dToken - 1;
            
            s32 dieValue = ls_utf32ToInt({hpExpr.data, dToken, dToken});
            s32 dieMul   = ls_utf32ToInt({hpExpr.data + dToken+1, mulLen, mulLen});
            
            finalHP  = dieValue*dieMul;
        }
    }
    
    s32 restLen       = hp.len - hpEndIndex;
    ls_utf32FromInt_t(&cachedPage->HP, finalHP);
    
    //NOTE: We copy the current cachedPage->HP into totHP to basically create a view into 
    //      the cachedPage->HP string, and only keep the total value!
    cachedPage->totHP = cachedPage->HP;
    
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
    s32 statBonusOld = newToOldMap[statBonusNew];
    
    s32 bmcVal = ls_utf32ToInt({BMC.data, endIdx, endIdx});
    
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
    s32 dexBonusOld = newToOldMap[dexBonusNew];
    s32 strBonusOld = newToOldMap[strBonusNew];
    
    s32 dmcVal = ls_utf32ToInt({DMC.data, endIdx, endIdx});
    
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
    s32 dexBonusOld = newToOldMap[dexBonusNew];
    
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
                
                initOne = ls_utf32ToInt({Init.data, mithicToken, mithicToken});
                initTwo = ls_utf32ToInt({Init.data + multiToken + 1, twoLen, twoLen});
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
                
                initOne = ls_utf32ToInt({Init.data, multiToken, multiToken});
                initTwo = ls_utf32ToInt({Init.data + multiToken + 1, twoLen, twoLen});
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
            
            initOne = ls_utf32ToInt({Init.data, multiToken, multiToken});
            initTwo = ls_utf32ToInt({Init.data + multiToken + 1, twoLen, twoLen});
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
            initOne = ls_utf32ToInt({Init.data, mithicToken, mithicToken});
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
            initOne = ls_utf32ToInt({Init.data, endIdx, endIdx});
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

void CalculateAndCacheMelee(utf32 Melee, CachedPageEntry *cachedPage)
{
    auto rightFindBonus = [](utf32 Melee, s32 offset, s32 min) -> s32 {
        s32 maybePlus  = ls_utf32RightFind(Melee, offset, '+');
        s32 maybeMinus = ls_utf32RightFind(Melee, offset, '-');
        
        if(maybePlus != -1 && maybePlus > min) { return maybePlus; }
        else if(maybeMinus != -1 && maybeMinus > min) { return maybeMinus; }
        else { return -1; }
    };
    
    if(Melee.len < 2) { ls_utf32Set(&cachedPage->melee, Melee); return; }
    
    s32 sciameIdx  = ls_utf32LeftFind(Melee, ls_utf32Constant(U"sciame"));
    s32 sciame2Idx = ls_utf32LeftFind(Melee, ls_utf32Constant(U"Sciame"));
    s32 truppaIdx  = ls_utf32LeftFind(Melee, ls_utf32Constant(U"truppa"));
    
    //TODO: Maybe signal with red? Or directly fix damage???
    if(sciameIdx != -1) { ls_utf32Set(&cachedPage->melee, Melee); return; }
    if(sciame2Idx != -1) { ls_utf32Set(&cachedPage->melee, Melee); return; }
    if(truppaIdx != -1) { ls_utf32Set(&cachedPage->melee, Melee); return; }
    
    s32 strBonusNew = ls_utf32ToInt(cachedPage->STR) - 10;;
    s32 strBonusOld = newToOldMap[strBonusNew];
    
    s32 dexBonusNew = ls_utf32ToInt(cachedPage->DEX) - 10;
    s32 dexBonusOld = newToOldMap[dexBonusNew];
    
    s32 bab = ls_utf32ToInt(cachedPage->BAB);
    
    
    //TODO: COMBATTERE CON 2 ARMI DA IL DOPPIO DEGLI ATTACCHI!!
    b32 hasTwoW = ls_utf32LeftFind(cachedPage->talents, ls_utf32Constant(U"Combattere con Due Armi")) != -1;
    b32 hasImprTwoW = ls_utf32LeftFind(cachedPage->talents, 
                                       ls_utf32Constant(U"Combattere con Due Armi Migliorato")) != -1;
    
    //AssertMsg(hasTwoW == FALSE && hasImprTwoW == FALSE, "Not handled yet!");
    
    
    b32 hasWeaponFinesse = ls_utf32LeftFind(cachedPage->talents, ls_utf32Constant(U"Arma Accurata")) != -1;
    
    s32 parenOpenIdx = ls_utf32LeftFind(Melee, '(');
    AssertMsg(parenOpenIdx != -1, "NOOOOOOO!\n");
    
    //NOTE: Prepare the string
    ls_utf32Clear(&cachedPage->melee);
    s32 stringIndex = 0;
    
    while(parenOpenIdx != -1)
    {
        s32 slashIdx = ls_utf32LeftFind(Melee, stringIndex, '/');
        s32 bonuses[9] = {};
        s32 attacksCount = 1;
        
        if(slashIdx == -1 || slashIdx > parenOpenIdx)
        { bonuses[0] = rightFindBonus(Melee, parenOpenIdx, stringIndex); }
        else 
        { bonuses[0] = rightFindBonus(Melee, slashIdx, stringIndex); }
        
        //TODO: What to do about this?
        if(bonuses[0] == -1) { ls_utf32Set(&cachedPage->melee, Melee); return; }
        AssertMsg(bonuses[0] != -1, "Missing value in 1 Attack!");
        
        if(slashIdx != -1 && slashIdx < parenOpenIdx)
        {
            //NOTE: Now count the attacks
            s32 currSlash = slashIdx;
            s32 lastSlash = slashIdx;
            while(currSlash = ls_utf32LeftFind(Melee, currSlash+1, '/'), currSlash != -1 && currSlash < parenOpenIdx)
            {
                if(attacksCount >= 8) { break; }
                AssertMsg(attacksCount < 8, "Too many attacks, BUG!\n");
                
                bonuses[attacksCount] = rightFindBonus(Melee, currSlash, stringIndex);
                attacksCount += 1;
                
                lastSlash = currSlash;
            }
            
            bonuses[attacksCount] = lastSlash+1;
            attacksCount += 1;
            
            //NOTE: We got all the attacks. We are done?
        }
        
        //NOTE: Skip the sign we are on and look for first.
        bonuses[attacksCount] = ls_utf32LeftFindNotNumber(Melee, bonuses[attacksCount-1]+1);
        
        u32 tmpBuff[32]  = {};
        utf32 tmpString = { tmpBuff, 0, 32 };
        
        u32 bonusBuffer[256]  = {};
        utf32 newBonusString = { bonusBuffer, 0, 256 };
        
        for(s32 i = 0; i < attacksCount; i++)
        {
            s32 len = bonuses[i+1] - bonuses[i];
            s32 oldBonus = ls_utf32ToInt({Melee.data + bonuses[i], len, len});
            s32 newBonus = oldBonus - strBonusOld + strBonusNew;
            if(hasWeaponFinesse) { newBonus = oldBonus - dexBonusOld + dexBonusNew; }
            
            ls_utf32FromInt_t(&tmpString, newBonus);
            
            if(newBonus > 0) { ls_utf32AppendChar(&newBonusString, '+'); }
            ls_utf32Append(&newBonusString, tmpString);
            
            if(i < attacksCount-1) { ls_utf32AppendChar(&newBonusString, '/'); }
        }
        
        ls_utf32Append(&cachedPage->melee, {Melee.data + stringIndex, bonuses[0] - stringIndex, bonuses[0] - stringIndex});
        ls_utf32Append(&cachedPage->melee, newBonusString);
        
        s32 parenCloseIdx = ls_utf32LeftFind(Melee, parenOpenIdx, ')');
        
        //NOTE: Assume this is a formatting error and clamp it to the end
        if(parenCloseIdx == -1) { parenCloseIdx = Melee.len - 1; }
        //AssertMsg(parenCloseIdx != -1, "WHAAAT!?");
        
        ls_utf32Append(&cachedPage->melee, {Melee.data + bonuses[attacksCount],
                           parenCloseIdx - bonuses[attacksCount] + 1, parenCloseIdx - bonuses[attacksCount] + 1});
        
        stringIndex = parenCloseIdx+1;
        
        parenOpenIdx = ls_utf32LeftFind(Melee, parenCloseIdx, '(');
    }
    
    //NOTE: It's mostly formatting errors because of fucking golarion!
    LogMsgF(Melee.len - stringIndex == 0, "Stuff left at the end of the original string: %d\n", Melee.len - stringIndex);
}

void ShortenAndCacheName(utf32 orig_name, utf32 *out)
{
    if(orig_name.len < 17) { ls_utf32Set(out, orig_name); return; }
    
    //TODO: Can we avoid this?
    utf32 name = ls_utf32Copy(orig_name);
    
    //NOTE: First remove the articles which are redundant to understanding the name
    utf32 articles[6] = {{(u32 *)U" di ", 4, 4}, {(u32 *)U" del ", 5, 5}, {(u32 *)U" della ", 7, 7}, 
        {(u32 *)U" delle ", 7, 7}, {(u32 *)U" dell'", 6, 6}, {(u32 *)U" d'", 3, 3}};
    s32 removeLen[6] = { 2, 3, 5, 5, 5, 2 };
    
    for(u32 i = 0; i < 5; i++)
    {
        s32 index = ls_utf32LeftFind(name, articles[i]);
        while(index >= 0)
        {
            ls_utf32RmSubstr(&name, index, index + removeLen[i]);
            index = ls_utf32LeftFind(name, articles[i]);
        }
    }
    
    if(name.len < 17) { ls_utf32Set(out, name); ls_utf32Free(&name); return; }
    
    //NOTE: If that was not enough, we shorten each word of the name by removing all vowels.
    u32 vowels[5] = { (u32)'a', (u32)'e', (u32)'i', (u32)'o', (u32)'u' };
    for(u32 i = 0; i < 5; i++)
    {
        s32 index = ls_utf32LeftFind(name, vowels[i]);
        while(index > 0)
        {
            ls_utf32RmIdx(&name, index);
            index = ls_utf32LeftFind(name, vowels[i]);
        }
    }
    
    //NOTE: If the name is still too long, shorten it to length 17
    if(name.len > 16) { name.len = 16; }
    
    ls_utf32Set(out, name);
    ls_utf32Free(&name); 
}

void AddIncrementalToName(UIContext *c, utf32 name, UITextBox *box)
{
    s32 visibleMobs   = State.Init->Mobs.selectedIndex;
    s32 visibleAllies = State.Init->Allies.selectedIndex;
    s32 visibleOrder = visibleMobs + visibleAllies + PARTY_NUM - State.Init->orderAdjust;
    
    //NOTE: Determine the number of the new mob/ally
    s32 cleanLen       = name.len;
    s32 maxIncremental = 0;
    for(u32 i = 0; i < visibleOrder; i++)
    {
        Order *o = State.Init->OrderFields + i;
        
        if(ls_utf32AreEqualUpTo(o->field.text, name, cleanLen))
        {
            s32 lenDiff = (o->field.text.len - name.len) - 1;
            utf32 justTheNumber = { o->field.text.data + cleanLen + 1, lenDiff, lenDiff}; //Skip the space
            s32 incremental = ls_utf32ToInt(justTheNumber);
            if(incremental > maxIncremental) maxIncremental = incremental;
        }
    }
    
    //TODO:Yet more temporary copy @ConvertNation
    u32 buff[32] = {};
    utf32 tmp = { buff, 0, 32 };
    
    u32 buff2[32] = {};
    utf32 tmp2 = { buff2, 0, 32 };
    
    ls_utf32FromInt_t(&tmp2, maxIncremental+1);
    ls_utf32Append(&tmp, name);
    ls_utf32AppendChar(&tmp, (u32)' ');
    ls_utf32Append(&tmp, tmp2);
    
    ls_uiTextBoxSet(c, box, tmp);
}

b32 AddMobOnClick(UIContext *, void *);
b32 AddAllyOnClick(UIContext *, void *);
b32 CompendiumAddPageToInitMob(UIContext *c, void *userData)
{
    s32 visibleMobs = State.Init->Mobs.selectedIndex;
    InitField *f = State.Init->MobFields + visibleMobs;
    
    if(!compendium.isViewingPage)  return FALSE;
    if(compendium.pageIndex == -1) return FALSE;
    if(visibleMobs == MOB_NUM)     return FALSE;
    
    ls_uiTextBoxSet(c, &f->maxLife, cachedPage.totHP);
    
    u32 buff[32] = {};
    utf32 tmp = { buff, 0, 32 };
    ShortenAndCacheName(cachedPage.name, &tmp);
    AddIncrementalToName(c, tmp, &f->editFields[IF_IDX_NAME]);
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_BONUS], cachedPage.initiative);
    f->compendiumIdx = compendium.pageIndex;
    
    AddMobOnClick(NULL, NULL);
    
    return TRUE;
}

b32 CompendiumAddPageToInitAlly(UIContext *c, void *userData)
{
    s32 visibleAllies = State.Init->Allies.selectedIndex;
    InitField *f = State.Init->AllyFields + visibleAllies;
    
    if(!compendium.isViewingPage)  return FALSE;
    if(compendium.pageIndex == -1) return FALSE;
    if(visibleAllies == ALLY_NUM)  return FALSE;
    
    ls_uiTextBoxSet(c, &f->maxLife, cachedPage.totHP);
    
    u32 buff[32] = {};
    utf32 tmp = { buff, 0, 32 };
    ShortenAndCacheName(cachedPage.name, &tmp);
    AddIncrementalToName(c, tmp, &f->editFields[IF_IDX_NAME]);
    
    ls_uiTextBoxSet(c, &f->editFields[IF_IDX_BONUS], cachedPage.initiative);
    f->compendiumIdx = compendium.pageIndex;
    
    AddAllyOnClick(NULL, NULL);
    
    return TRUE;
}

b32 onStatusChange(UIContext *c, void *data)
{
    Order *ord = (Order *)data;
    
    //NOTE: We re-cache the page to apply the changes.
    GetPageEntryAndCache(ord->compendiumIdx, &mainCachedPage);
    
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
    
    cachedPage->origin            = ls_utf32Alloc(128);
    cachedPage->shortDesc         = ls_utf32Alloc(512);
    cachedPage->AC                = ls_utf32Alloc(192);
    cachedPage->HP                = ls_utf32Alloc(128);
    cachedPage->ST                = ls_utf32Alloc(128);
    cachedPage->RD                = ls_utf32Alloc(128);
    cachedPage->RI                = ls_utf32Alloc(128);
    cachedPage->defensiveCapacity = ls_utf32Alloc(256);
    cachedPage->melee             = ls_utf32Alloc(1280);
    cachedPage->ranged            = ls_utf32Alloc(1280);
    cachedPage->specialAttacks    = ls_utf32Alloc(1536);
    cachedPage->psych             = ls_utf32Alloc(2048);
    cachedPage->magics            = ls_utf32Alloc(2048);
    cachedPage->spells            = ls_utf32Alloc(2048);
    
    cachedPage->tactics_before    = ls_utf32Alloc(448);
    cachedPage->tactics_during    = ls_utf32Alloc(832);
    cachedPage->tactics_stats     = ls_utf32Alloc(736);
    
    cachedPage->racialMods        = ls_utf32Alloc(256);
    cachedPage->spec_qual         = ls_utf32Alloc(512);
    cachedPage->given_equip       = ls_utf32Alloc(512);
    cachedPage->properties        = ls_utf32Alloc(640);
    cachedPage->boons             = ls_utf32Alloc(512);
    
    cachedPage->specials          = ls_utf32Alloc(maxTalents * 2048);
    
    cachedPage->org               = ls_utf32Alloc(512);
    cachedPage->treasure          = ls_utf32Alloc(320);
    cachedPage->desc              = ls_utf32Alloc(10240);
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
    cachedPage->reach             = ls_utf32Alloc(768);
    
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
    
    cachedPage->talents           = ls_utf32Alloc(maxTalents   * 32);
    cachedPage->skills            = ls_utf32Alloc(maxSkills    * 32);
    cachedPage->languages         = ls_utf32Alloc(maxLanguages * 64);
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
        
        //TODO: Change these to a FixedArray 
        u32 entryCount = ls_bufferReadDWord(&CompendiumBuff);
        u8 *pagesSrc = (u8 *)CompendiumBuff.data + CompendiumBuff.cursor;
        ls_arrayFromPointer(&compendium.codex.pages, (void *)pagesSrc, entryCount);
        ls_bufferReadSkip(&CompendiumBuff, entryCount * sizeof(PageEntry));
        
        u32 npcEntryCount = ls_bufferReadDWord(&CompendiumBuff);
        u8 *npcPagesSrc = (u8 *)CompendiumBuff.data + CompendiumBuff.cursor;
        ls_arrayFromPointer(&compendium.codex.npcPages, (void *)npcPagesSrc, npcEntryCount);
    }
    
    const u32 currentViewIndicesCount = 3200;
    compendium.viewIndices = ls_arrayAlloc<u16>(currentViewIndicesCount);
    for(u16 i = 0; i < currentViewIndicesCount; i++)
    { ls_arrayAppend(&compendium.viewIndices, i); }
    
    const u32 currentNPCViewIndicesCount = 1200;
    compendium.npcViewIndices = ls_arrayAlloc<u16>(currentNPCViewIndicesCount);
    for(u16 i = 0; i < currentNPCViewIndicesCount; i++)
    { ls_arrayAppend(&compendium.npcViewIndices, i); }
    
    ls_arenaUse(globalArena);
    
    return;
}

void AppendEntryFromBuffer(buffer *buf, utf32 *base, const char32_t *sep, u32 index, const char *name = "")
{
    if(index == 0) { AssertMsg(FALSE, "We shouldn't be here\n"); return; } //NOTE: Index zero means no entry
    
    buf->cursor = index;
    
    u32 byteLen   = (u32)ls_bufferPeekWord(buf);
    u8 *utf8_data = (u8 *)buf->data + buf->cursor + 2;
    utf8 toAppend = ls_utf8Constant(utf8_data, byteLen);
    u32 sepLen    = 0;
    
    if(sep) { sepLen = ls_utf32Len(sep); }
    
    if(base->len + toAppend.len + sepLen > base->size)
    { ls_printf("%cs: Fuck Size: %d, Len: %d, ByteLen: %d, Index: %d\n", name, base->size, toAppend.len, byteLen, index);
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

void AppendEntryFromBuffer(buffer *buf, utf32 *base, const char32_t *sep, u16 index, const char *name = "")
{ AppendEntryFromBuffer(buf, base, sep, (u32)index, name); }

void GetEntryFromBuffer_t(buffer *buf, utf32 *toSet, u32 index, const char *name = "")
{
    if(index == 0) { toSet->len = 0; return; } //NOTE: Index zero means no entry
    
    buf->cursor = index;
    
    s32 byteLen   = ls_bufferPeekWord(buf);
    u8 *utf8_data = (u8 *)buf->data + buf->cursor + 2;
    
    u32 len = ls_utf8Len(utf8_data, byteLen);
    
    LogMsgF(toSet->size >= len, "%cs: Fuck Size: %d, Len: %d, ByteLen: %d, Index: %d\n", name, toSet->size, len, byteLen, index);
    
    ls_utf32FromUTF8_t(toSet, utf8_data, len);
    
    ls_bufferSeekBegin(buf);
}

void GetEntryFromBuffer_t(buffer *buf, utf32 *toSet, u16 index, const char *name = "")
{ GetEntryFromBuffer_t(buf, toSet, (u32)index, name); }

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
    tableScroll = { 0, 10, c->width-4, c->height-36, 0, 0, c->width-32, monsterTableMinY };
    
    compendium.searchBarMobs.text = ls_utf32Alloc(64);
    compendium.searchBarMobs.postInput = CompendiumSearchFunctionMobs;
    compendium.searchBarMobs.isSingleLine = TRUE;
    
    ls_uiSelectFontByFontSize(c, FS_SMALL);
    
    return;
}

void SetNPCTable(UIContext *c)
{
    Codex *codex = &compendium.codex;
    
    s32 npcTableMinY = -((codex->npcPages.count-30) * 19);
    npcTableScroll = { 0, 10, c->width-4, c->height-36, 0, 0, c->width-32, npcTableMinY };
    
    compendium.searchBarNPCs.text = ls_utf32Alloc(64);
    compendium.searchBarNPCs.postInput = CompendiumSearchFunctionNPCs;
    compendium.searchBarNPCs.isSingleLine = TRUE;
    
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

void CachePage(PageEntry page, s32 viewIndex, CachedPageEntry *cachedPage, Status *statuses = NULL)
{
    u32 tempUTF32Buffer[512] = {};
    utf32 tempString = { tempUTF32Buffer, 0, 512 };
    
    Codex *c = &compendium.codex;
    
    cachedPage->pageIndex    = viewIndex;
    
    //TODO: What's missing (apart from possible bug fixes)
    //      TxC + Dmg (There's a lot of exceptions with weapons and it's shit. This is for laater)
    //      Maybe DC for Channeling Energy on clerics with Carisma?
    //      Maybe AC Deviation bonus for specific Race/Class combos?
    //      Skill Checks On Any Ability
    
    //NOTE: Everything tries to be ordered like the struct, to be organized
    //      But I need to have these stats earlier because other paramaters depend on them
    GetEntryFromBuffer_t(&c->names, &cachedPage->name, page.name);
    GetEntryFromBuffer_t(&c->gs, &cachedPage->gs, page.gs);
    GetEntryFromBuffer_t(&c->pe, &cachedPage->pe, page.pe);
    
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->STR, page.STR);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->DEX, page.DEX);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->CON, page.CON);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->INT, page.INT);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->WIS, page.WIS);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->CHA, page.CHA);
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->BAB, page.BAB);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->treasure, page.treasure);
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->origin, page.origin);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->shortDesc, page.shortDesc);
    GetEntryFromBuffer_t(&c->sizes, &cachedPage->size, page.size);
    
    //NOTE: Early talents are for attack modifiers!
    //TODO: I should create an enum and save the existance of certain talents
    //      into an array, to be more efficiently used later!!
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
    
    
    GetEntryFromBuffer_t(&c->generalStrings, &tempString, page.AC);
    CalculateAndCacheAC(tempString, cachedPage, FALSE);
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
    
    GetEntryFromBuffer_t(&c->generalStrings, &tempString, page.melee);
    CalculateAndCacheMelee(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->ranged, page.ranged);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->specialAttacks, page.specialAttacks);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->psych, page.psych);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->magics, page.magics);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->spells, page.spells);
    
    ls_utf32Clear(&cachedPage->tactics_before);
    ls_utf32Clear(&cachedPage->tactics_during);
    ls_utf32Clear(&cachedPage->tactics_stats);
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->racialMods, page.racialMods);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->spec_qual, page.spec_qual);
    
    ls_utf32Clear(&cachedPage->given_equip);
    ls_utf32Clear(&cachedPage->properties);
    ls_utf32Clear(&cachedPage->boons);
    
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
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->desc, page.desc);
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->source, page.source);
    
    
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

void CachePage(NPCPageEntry page, s32 viewIndex, CachedPageEntry *cachedPage, Status *statuses = NULL)
{
    u32 tempUTF32Buffer[512] = {};
    utf32 tempString = { tempUTF32Buffer, 0, 512 };
    
    Codex *c = &compendium.codex;
    
    cachedPage->pageIndex = viewIndex;
    
    //TODO: What's missing (apart from possible bug fixes)
    //      TxC + Dmg (There's a lot of exceptions with weapons and it's shit. This is for laater)
    //      Maybe DC for Channeling Energy on clerics with Carisma?
    //      Maybe AC Deviation bonus for specific Race/Class combos?
    //      Skill Checks On Any Ability
    
    //NOTE: Everything tries to be ordered like the struct, to be organized
    //      But I need to have these stats earlier because other paramaters depend on them
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->STR, page.STR, "str");
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->DEX, page.DEX, "dex");
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->CON, page.CON, "con");
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->INT, page.INT, "int");
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->WIS, page.WIS, "wis");
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->CHA, page.CHA, "cha");
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->BAB, page.BAB, "bab");
    
    ls_utf32Clear(&cachedPage->treasure);
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->origin, page.origin, "origin");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->shortDesc, page.shortDesc, "short_desc");
    GetEntryFromBuffer_t(&c->sizes, &cachedPage->size, page.size, "size");
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->given_equip, page.given_equip, "given_equip");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->properties, page.properties, "properties");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->boons, page.boons, "boons");
    
    GetEntryFromBuffer_t(&c->generalStrings, &tempString, page.AC, "AC");
    CalculateAndCacheAC(tempString, cachedPage, TRUE);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->generalStrings, &tempString, page.HP, "HP");
    CalculateAndCacheHP(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->generalStrings, &tempString, page.ST, "ST");
    CalculateAndCacheST(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->numericValues, &tempString, page.BMC, "BMC");
    CalculateAndCacheBMC(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->numericValues, &tempString, page.DMC, "DMC");
    CalculateAndCacheDMC(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->numericValues, &tempString, page.initiative, "initiative");
    CalculateAndCacheInitiative(tempString, cachedPage);
    ls_utf32Clear(&tempString);
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->RD, page.RD, "RD");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->RI, page.RI, "RI");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->defensiveCapacity, page.defensiveCapacity, "Def.Cap");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->melee, page.melee, "Melee");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->ranged, page.ranged, "Ranged");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->specialAttacks, page.specialAttacks, "Spec.Atk");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->psych, page.psych, "Psych");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->magics, page.magics, "Magics");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->spells, page.spells, "Spells");
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->tactics_before, page.tactics[0], "tactics-before");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->tactics_during, page.tactics[1], "tactics-during");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->tactics_stats,  page.tactics[2], "tactics-stats");
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->racialMods, page.racialMods, "racial mods");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->spec_qual, page.spec_qual, "spec_qual");
    
    ls_utf32Clear(&cachedPage->specials);
    if(page.specials[0])
    {
        GetEntryFromBuffer_t(&c->specials, &cachedPage->specials, page.specials[0], "specials");
        u32 i = 1;
        while(page.specials[i] && i < 24)
        {
            AppendEntryFromBuffer(&c->specials, &cachedPage->specials, U"\n\n", page.specials[i], "specials");
            i += 1;
        }
    }
    
    ls_utf32Clear(&cachedPage->org);
    
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->desc, page.desc, "desc");
    GetEntryFromBuffer_t(&c->generalStrings, &cachedPage->source, page.source, "source");
    
    GetEntryFromBuffer_t(&c->names, &cachedPage->name, page.name, "name");
    GetEntryFromBuffer_t(&c->gs, &cachedPage->gs, page.gs, "gs");
    GetEntryFromBuffer_t(&c->pe, &cachedPage->pe, page.pe, "pe");
    GetEntryFromBuffer_t(&c->alignment, &cachedPage->alignment, page.alignment, "align");
    GetEntryFromBuffer_t(&c->types, &cachedPage->type, page.type, "type");
    
    ls_utf32Clear(&cachedPage->subtype);
    if(page.subtype[0])
    {
        ls_utf32Append(&cachedPage->subtype, ls_utf32Constant(U" ("));
        AppendEntryFromBuffer(&c->subtypes, &cachedPage->subtype, NULL, page.subtype[0], "subtypes");
        u32 i = 1;
        while(page.subtype[i] && i < 8)
        {
            AppendEntryFromBuffer(&c->subtypes, &cachedPage->subtype, U", ", page.subtype[i], "subtypes");
            i += 1;
        }
        ls_utf32Append(&cachedPage->subtype, ls_utf32Constant(U") "));
    }
    
    ls_utf32Clear(&cachedPage->archetype);
    if(page.archetype[0])
    {
        ls_utf32Append(&cachedPage->archetype, ls_utf32Constant(U"["));
        AppendEntryFromBuffer(&c->archetypes, &cachedPage->archetype, NULL, page.archetype[0], "archetypes");
        u32 i = 1;
        while(page.archetype[i] && i < 4)
        {
            AppendEntryFromBuffer(&c->archetypes, &cachedPage->archetype, U", ", page.archetype[i], "archetypes");
            i += 1;
        }
        ls_utf32Append(&cachedPage->archetype, ls_utf32Constant(U"] "));
    }
    
    ls_utf32Clear(&cachedPage->senses);
    if(page.senses[0])
    {
        GetEntryFromBuffer_t(&c->senses, &cachedPage->senses, page.senses[0], "senses");
        u32 i = 1;
        while(page.senses[i] && i < 8)
        {
            AppendEntryFromBuffer(&c->senses, &cachedPage->senses, U", ", page.senses[i], "senses");
            i += 1;
        }
    }
    
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->perception, page.perception, "perception");
    GetEntryFromBuffer_t(&c->auras, &cachedPage->aura, page.aura, "aura");
    
    ls_utf32Clear(&cachedPage->immunities);
    if(page.immunities[0])
    {
        GetEntryFromBuffer_t(&c->immunities, &cachedPage->immunities, page.immunities[0], "immunities");
        u32 i = 1;
        while(page.immunities[i] && i < 16)
        {
            AppendEntryFromBuffer(&c->immunities, &cachedPage->immunities, U", ", page.immunities[i], "immunities");
            i += 1;
        }
    }
    
    ls_utf32Clear(&cachedPage->resistances);
    if(page.resistances[0])
    {
        GetEntryFromBuffer_t(&c->resistances, &cachedPage->resistances, page.resistances[0], "resistances");
        u32 i = 1;
        while(page.resistances[i] && i < 16)
        {
            AppendEntryFromBuffer(&c->resistances, &cachedPage->resistances, U", ", page.resistances[i], "resistances");
            i += 1;
        }
    }
    
    ls_utf32Clear(&cachedPage->weaknesses);
    if(page.weaknesses[0])
    {
        GetEntryFromBuffer_t(&c->weaknesses, &cachedPage->weaknesses, page.weaknesses[0], "weaknesses");
        u32 i = 1;
        while(page.weaknesses[i] && i < 16)
        {
            AppendEntryFromBuffer(&c->weaknesses, &cachedPage->weaknesses, U", ", page.weaknesses[i], "weaknesses");
            i += 1;
        }
    }
    
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->speed, page.speed, "speed");
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->space, page.space, "space");
    GetEntryFromBuffer_t(&c->numericValues, &cachedPage->reach, page.reach, "reach");
    
    ls_utf32Clear(&cachedPage->talents);
    if(page.talents[0])
    {
        GetEntryFromBuffer_t(&c->talents, &cachedPage->talents, page.talents[0], "talents");
        u32 i = 1;
        while(page.talents[i] && i < 24)
        {
            AppendEntryFromBuffer(&c->talents, &cachedPage->talents, U", ", page.talents[i], "talents");
            i += 1;
        }
    }
    
    ls_utf32Clear(&cachedPage->skills);
    if(page.skills[0])
    {
        GetEntryFromBuffer_t(&c->skills, &cachedPage->skills, page.skills[0], "skills");
        u32 i = 1;
        while(page.skills[i] && i < 24)
        {
            AppendEntryFromBuffer(&c->skills, &cachedPage->skills, U", ", page.skills[i], "skills");
            i += 1;
        }
    }
    
    ls_utf32Clear(&cachedPage->languages);
    if(page.languages[0])
    {
        GetEntryFromBuffer_t(&c->languages, &cachedPage->languages, page.languages[0], "languages");
        u32 i = 1;
        while(page.languages[i] && i < 24)
        {
            AppendEntryFromBuffer(&c->languages, &cachedPage->languages, U", ", page.languages[i], "languages");
            i += 1;
        }
    }
    
    ls_utf32Clear(&cachedPage->environment);
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
        ls_uiLabelLayout(c, U"GS", UIRect { baseR.x + 455, baseR.y, maxW, minY }, pureWhite);
        ls_uiLabelLayout(c, page->gs, UIRect { baseR.x + 485, baseR.y, maxW, minY }, pureWhite);
        ls_uiLabelLayout(c, U"PE", UIRect { baseR.x + 580, baseR.y, maxW, minY }, pureWhite);
        offset = ls_uiLabelLayout(c, page->pe, UIRect { baseR.x + 615, baseR.y, maxW, minY }, pureWhite);
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
    //    TACTICS    //
    //---------------//
    
    if(page->tactics_before.len || page->tactics_during.len || page->tactics_stats.len)
    {
        currPixelHeight = ls_uiSelectFontByFontSize(c, FS_MEDIUM);
        baseR.y -= currPixelHeight - prevPixelHeight; prevPixelHeight = currPixelHeight;
        baseR.y -= 4;
        baseX    = 148;
        {
            offset = ls_uiLabelLayout(c, U"Tattiche", baseR, pureWhite);
            ls_uiHSeparator(c, baseR.x, baseR.y-4, hSepWidth, 1, RGB(0, 0, 0));
            
            currPixelHeight = ls_uiSelectFontByFontSize(c, FS_SMALL);
            baseR.y += prevPixelHeight - currPixelHeight; prevPixelHeight = currPixelHeight;
            baseR.y -= offset.h;
            
            if(page->tactics_before.len)
            {
                renderAndAlignS(U"Prima del Combattimento: ");
                renderAndAlign(page->tactics_before);
            }
            
            if(page->tactics_during.len)
            {
                renderAndAlignS(U"Durante il Combattimento: ");
                renderAndAlign(page->tactics_during);
            }
            
            if(page->tactics_stats.len)
            {
                renderAndAlignS(U"Statistiche Base: ");
                renderAndAlign(page->tactics_stats);
            }
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
        
        if(page->given_equip.len)
        {
            renderAndAlignS(U"Dotazioni da Combattimento: ");
            renderAndAlign(page->given_equip);
        }
        
        if(page->properties.len)
        {
            renderAndAlignS(U"Propriet\U000000E0: ");
            renderAndAlign(page->properties);
        }
        
        if(page->boons.len)
        {
            renderAndAlignS(U"Beneficio: ");
            renderAndAlign(page->boons);
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
    
    if(page->environment.len || page->org.len || page->treasure.len)
    {
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
        offset = ls_uiLabelLayout(c, page->source, UIRect { baseR.x + 58, baseR.y, maxW, minY });
        baseR.y -= offset.maxY;
    }
    
    c->textColor = RGBg(0xCC);
    
    return baseR.y;
}

void DrawNPCTable(UIContext *c)
{
    Input *UserInput = &c->UserInput;
    Codex *codex     = &compendium.codex;
    
    Color bkgColor = RGBg(0x40);
    
    s32 baseX = 20;
    s32 baseY = 630;
    
    ls_uiTextBox(c, &compendium.searchBarNPCs, baseX, baseY + 40, 200, 20);
    
    ls_uiStartScrollableRegion(c, &npcTableScroll);
    
    s32 startI = -(npcTableScroll.deltaY / 19);
    s32 endI   = startI+36 < compendium.npcViewIndices.count ? startI+36 : compendium.npcViewIndices.count;
    endI       = endI < codex->npcPages.count ? endI : codex->npcPages.count;
    
    for(s32 i = startI; i < endI; i++)
    {
        NPCPageEntry entry = codex->npcPages[compendium.npcViewIndices[i]];
        Color hoverColor = bkgColor;
        
        if(LeftClickIn(baseX-4, baseY-4, 300, 18)) //TODONOTE: I don't like it...
        {
            compendium.isViewingPage = TRUE;
            compendium.pageIndex  = compendium.npcViewIndices[i] + NPC_PAGE_INDEX_OFFSET;
        }
        
        if(MouseInRect(baseX-4, baseY-4, 300, 18)) { hoverColor = RGBg(0x66); }
        
        //NOTE: Matriarca delle Scimmie Cappuccine Corallo is too long...
        //      Adding a scissor cuts it... but I don't like that solution a lot...
        //      What if I want automatic '...' at the end of a label??
        ls_uiRect(c, baseX-4, baseY+npcTableScroll.deltaY-4, 300, 20, hoverColor, c->borderColor);
        c->scissor = UIRect { baseX-4, 0, 298, s32(c->height) };
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->names, entry.name), baseX, baseY+npcTableScroll.deltaY, 1);
        c->scissor = UIRect { 0, 0, s32(c->width), s32(c->height) };
        baseX += 299;
        
        ls_uiRect(c, baseX-4, baseY+npcTableScroll.deltaY-4, 80, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->gs, entry.gs), baseX, baseY+npcTableScroll.deltaY, 1);
        baseX += 79;
        
        ls_uiRect(c, baseX-4, baseY+npcTableScroll.deltaY-4, 180, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->types, entry.type), baseX, baseY+npcTableScroll.deltaY, 1);
        baseX += 179;
        
        //TODO: Show all subtypes. Right now we only grab one.
        //      We can do this by overloading the function GetEntryFromBuffer and passing the array itself.
        ls_uiRect(c, baseX-4, baseY+npcTableScroll.deltaY-4, 188, 20, bkgColor, c->borderColor);
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->subtypes, entry.subtype[0]), baseX, baseY+npcTableScroll.deltaY, 1);
        baseX += 187;
        
        baseY -= 19;
        baseX = 20;
    }
    
    ls_uiEndScrollableRegion(c);
    
}

void DrawMonsterTable(UIContext *c)
{
    Input *UserInput = &c->UserInput;
    Codex *codex     = &compendium.codex;
    
    Color bkgColor = RGBg(0x40);
    
    s32 baseX = 20;
    s32 baseY = 630;
    
    ls_uiTextBox(c, &compendium.searchBarMobs, baseX, baseY + 40, 200, 20);
    
    ls_uiStartScrollableRegion(c, &tableScroll);
    
    s32 startI = -(tableScroll.deltaY / 19);
    s32 endI   = startI+36 < compendium.viewIndices.count ? startI+36 : compendium.viewIndices.count;
    endI       = endI < codex->pages.count ? endI : codex->pages.count;
    
    for(s32 i = startI; i < endI; i++)
    {
        PageEntry entry = codex->pages[compendium.viewIndices[i]];
        Color hoverColor = bkgColor;
        
        if(LeftClickIn(baseX-4, baseY-4, 300, 18)) //TODONOTE: I don't like it...
        {
            compendium.isViewingPage = TRUE; 
            compendium.pageIndex     = compendium.viewIndices[i];
        }
        
        if(MouseInRect(baseX-4, baseY-4, 300, 18)) { hoverColor = RGBg(0x66); }
        
        //NOTE: Matriarca delle Scimmie Cappuccine Corallo is too long...
        //      Adding a scissor cuts it... but I don't like that solution a lot...
        //      What if I want automatic '...' at the end of a label??
        ls_uiRect(c, baseX-4, baseY+tableScroll.deltaY-4, 300, 20, hoverColor, c->borderColor);
        c->scissor = UIRect { baseX-4, 0, 298, s32(c->height) };
        ls_uiLabel(c, GetEntryFromBuffer_8(&codex->names, entry.name), baseX, baseY+tableScroll.deltaY, 1);
        c->scissor = UIRect { 0, 0, s32(c->width), s32(c->height) };
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

#if _DEBUG
void testAllCompendiumForAsserts();
#endif

void DrawCompendium(UIContext *c)
{
    //NOTE: This arena is cleared every frame,
    //      Because here we store utf32 strings (currently only for the Search Function)
    ls_arenaClear(compTempArena);
    
    Codex *codex = &compendium.codex;
    Input *UserInput = &c->UserInput;
    
#if _DEBUG
    if(KeyPress(keyMap::F1))
    {
        testAllCompendiumForAsserts();
    }
#endif
    
    
    if(compendium.pageIndex == -1)
    {
        if(compendium.isViewingNPCTable) DrawNPCTable(c); 
        else                             DrawMonsterTable(c);
    }
    else
    {
        //NOTE: It's a Mob's Page Index
        if(compendium.pageIndex < NPC_PAGE_INDEX_OFFSET)
        {
            if(KeyHeld(keyMap::Shift) && KeyPressOrRepeat(keyMap::DArrow) && compendium.pageIndex < (codex->pages.count-1))
            { compendium.pageIndex += 1; }
            
            if(KeyHeld(keyMap::Shift) && KeyPressOrRepeat(keyMap::UArrow) && compendium.pageIndex > 0)
            { compendium.pageIndex -= 1; }
            
            if(cachedPage.pageIndex != compendium.pageIndex)
            { 
                PageEntry pEntry = compendium.codex.pages[compendium.pageIndex];
                CachePage(pEntry, compendium.pageIndex, &cachedPage);
                
                //NOTE: Reset the page scroll for the new page (Fuck GCC)
                pageScroll = { 0, 10, c->width-4, c->height-36, 0, 0, c->width-32, 0 };
            }
            
            //NOTE: The first frame is impossible to scroll, because the minY value will be not initialized yet
            //      It's should be fine though. We run at 30FPS on the Compendium, so it should never be felt/seen.
            //      The minY is set by the DrawPage call itself
            ls_uiStartScrollableRegion(c, &pageScroll);
            pageScroll.minY = DrawPage(c, &cachedPage, 0, 670, c->width-42, 0);
            ls_uiEndScrollableRegion(c);
        }
        else
        {
            s32 npcPageIndex = compendium.pageIndex - NPC_PAGE_INDEX_OFFSET;
            
            if(KeyHeld(keyMap::Shift) && KeyPressOrRepeat(keyMap::DArrow) && npcPageIndex < (codex->npcPages.count-1))
            { compendium.pageIndex += 1; npcPageIndex += 1; }
            
            if(KeyHeld(keyMap::Shift) && KeyPressOrRepeat(keyMap::UArrow) && npcPageIndex > 0)
            { compendium.pageIndex -= 1; npcPageIndex -= 1; }
            
            if(cachedPage.pageIndex != compendium.pageIndex)
            { 
                NPCPageEntry pEntry = compendium.codex.npcPages[npcPageIndex];
                CachePage(pEntry, compendium.pageIndex, &cachedPage);
                
                //NOTE: Reset the page scroll for the new page (Fuck GCC)
                pageScroll = { 0, 10, c->width-4, c->height-36, 0, 0, c->width-32, 0 };
            }
            
            //NOTE: The first frame is impossible to scroll, because the minY value will be not initialized yet
            //      It's should be fine though. We run at 30FPS on the Compendium, so it should never be felt/seen.
            //      The minY is set by the DrawPage call itself
            ls_uiStartScrollableRegion(c, &pageScroll);
            pageScroll.minY = DrawPage(c, &cachedPage, 0, 670, c->width-42, 0);
            ls_uiEndScrollableRegion(c);
        }
    }
    
    return;
}