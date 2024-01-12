u16 findNeedleAndFillIndexBuffer(utf8 needle, u16 *indexBuffer, buffer *buf, u32 buffSize)
{
    u8 dataBuffer[128] = {};
    utf8 toMatchLower = { dataBuffer, 0, 0, 128 };
    
    buf->cursor = 4;
    
    u16 nibCount = 0;
    while(buf->cursor < buf->size)
    {
        if(nibCount > buffSize)
        { AssertMsg(FALSE, "Index Buffer when searching is not large enough\n"); return nibCount; }
        
        s32 strByteLen = ls_bufferPeekWord(buf);
        u8 *data       = (u8 *)buf->data + buf->cursor + 2;
        
        utf8 toMatch   = ls_utf8Constant(data, strByteLen);
        ls_utf8ToLower(&toMatchLower, toMatch);
        
        if(ls_utf8Contains(toMatchLower, needle))
        {
            indexBuffer[nibCount] = buf->cursor;
            nibCount += 1;
        }
        
        ls_bufferReadSkip(buf, strByteLen + 2);
    }
    
    return nibCount;
}

s32 findGSAndFillIndexBuffer(utf32 gs, s32 *indexBuffer)
{
    s32 count = 0;
    for(s32 i = 0; i < gsSetCount; i++)
    {
        if(ls_utf32Contains(gsSet[i], gs))
        {
            indexBuffer[count] = i;
            count += 1;
        }
    }
    
    return count;
}

b32 CompendiumSearchFunctionMobs(UIContext *c, void *userData)
{
    ls_arenaUse(compTempArena);
    
    CompendiumSearchData *searchData = (CompendiumSearchData *)userData;
    
    //NOTE: Reset the scrollbar when searching
    tableScroll.deltaY = 0;
    
    //NOTE: The limit under which we reset the view and stop searching
    s32 minCharLimit = 2;
    if(searchData->kind == SEARCH_GS) { minCharLimit = 1; }
    
    if(searchData->searchBar->text.len < minCharLimit)
    { 
        tableScroll.minY = -((compendium.codex.pages.count-30) * 19);
        ls_arrayClear(&compendium.viewIndices);
        for(u16 i = 0; i < compendium.viewIndices.cap; i++)
        { ls_arrayAppend(&compendium.viewIndices, i); }
        
        return FALSE;
    }
    
    switch(searchData->kind)
    {
        case SEARCH_NAME:
        {
            //NOTE:Iterate over the name buffer to find all names which exactly contain the substring needle
            u8 needleBuf[256] = {};
            utf8 needle = { needleBuf, 0, 0, 256 };
            ls_utf8FromUTF32_t(&needle, searchData->searchBar->text);
            
            u8 dataBuffer[128] = {};
            utf8 needleLower = { dataBuffer, 0, 0, 128 };
            
            ls_utf8ToLower(&needleLower, needle);
            
            u16 indexBuffer[4096] = {};
            u16 nibCount = findNeedleAndFillIndexBuffer(needleLower, indexBuffer, searchData->searchBuffer, 4096);
            
            if(nibCount > 0)
            {
                ls_arrayClear(&compendium.viewIndices);
                for(u16 i = 0; i < compendium.codex.pages.count; i++)
                {
                    if(nibCount == 0) { break; }
                    
                    u16 indexToMatch = compendium.codex.pages[i].name;
                    for(u32 j = 0; j < nibCount; j++)
                    {
                        if(indexBuffer[j] == indexToMatch)
                        {
                            ls_arrayAppend(&compendium.viewIndices, i);
                            
                            indexBuffer[j] = indexBuffer[nibCount-1];
                            nibCount -= 1;
                            break;
                        }
                    }
                }
            }
        } break;
        
        case SEARCH_TYPE:
        {
            //NOTE:Iterate over the name buffer to find all names which exactly contain the substring needle
            u8 needleBuf[256] = {};
            utf8 needle = { needleBuf, 0, 0, 256 };
            ls_utf8FromUTF32_t(&needle, searchData->searchBar->text);
            
            u8 dataBuffer[128] = {};
            utf8 needleLower = { dataBuffer, 0, 0, 128 };
            
            ls_utf8ToLower(&needleLower, needle);
            
            u16 indexBuffer[4096] = {};
            u16 nibCount = findNeedleAndFillIndexBuffer(needleLower, indexBuffer, searchData->searchBuffer, 4096);
            
            if(nibCount > 0)
            {
                ls_arrayClear(&compendium.viewIndices);
                for(u16 i = 0; i < compendium.codex.pages.count; i++)
                {
                    u16 indexToMatch = compendium.codex.pages[i].type;
                    for(u32 j = 0; j < nibCount; j++)
                    {
                        if(indexBuffer[j] == indexToMatch)
                        {
                            ls_arrayAppend(&compendium.viewIndices, i);
                            break;
                        }
                    }
                }
            }
        } break;
        
        case SEARCH_GS:
        {
            s32 indexBuffer[gsSetCount+1] = {};
            s32 nibCount = findGSAndFillIndexBuffer(searchData->searchBar->text, indexBuffer);
            
            if(nibCount > 0)
            {
                ls_arrayClear(&compendium.viewIndices);
                for(u16 i = 0; i < compendium.codex.pages.count; i++)
                {
                    s32 entryToMatch = CompendiumGetGSIndexFromSet(compendium.codex.pages[i].gs);
                    if(entryToMatch == -1) { continue; }
                    
                    for(s32 j = 0; j < nibCount; j++)
                    {
                        if(indexBuffer[j] == entryToMatch)
                        {
                            ls_arrayAppend(&compendium.viewIndices, i);
                            break;
                        }
                    }
                }
            }
        } break;
    }
    
    //NOTE: Adjust scrollbar height to new monster table count.
    if(compendium.viewIndices.count > 30) { tableScroll.minY = -((compendium.viewIndices.count-30) * 19); }
    else                                  { tableScroll.minY = -1; }
    
    ls_arenaUse(globalArena);
    
    return FALSE;
}

b32 CompendiumSearchFunctionNPCs(UIContext *c, void *userData)
{
    ls_arenaUse(compTempArena);
    
    CompendiumSearchData *searchData = (CompendiumSearchData *)userData;
    
    //NOTE: Reset the scrollbar when searching
    npcTableScroll.deltaY = 0;
    
    //NOTE: The limit under which we reset the view and stop searching
    s32 minCharLimit = 2;
    if(searchData->kind == SEARCH_GS) { minCharLimit = 1; }
    
    if(searchData->searchBar->text.len < minCharLimit)
    { 
        npcTableScroll.minY = -((compendium.codex.npcPages.count-30) * 19);
        ls_arrayClear(&compendium.npcViewIndices);
        for(u16 i = 0; i < compendium.npcViewIndices.cap; i++)
        { ls_arrayAppend(&compendium.npcViewIndices, i); }
        
        return FALSE;
    }
    
    switch(searchData->kind)
    {
        case SEARCH_NAME:
        {
            //NOTE:Iterate over the name buffer to find all names which exactly contain the substring needle
            u8 needleBuf[256] = {};
            utf8 needle = { needleBuf, 0, 0, 256 };
            ls_utf8FromUTF32_t(&needle, searchData->searchBar->text);
            
            u8 dataBuffer[128] = {};
            utf8 needleLower = { dataBuffer, 0, 0, 128 };
            
            ls_utf8ToLower(&needleLower, needle);
            
            u16 indexBuffer[4096] = {};
            u16 nibCount = findNeedleAndFillIndexBuffer(needleLower, indexBuffer, searchData->searchBuffer, 4096);
            
            if(nibCount > 0)
            {
                ls_arrayClear(&compendium.npcViewIndices);
                for(u16 i = 0; i < compendium.codex.npcPages.count; i++)
                {
                    if(nibCount == 0) { break; }
                    
                    u16 indexToMatch = compendium.codex.npcPages[i].name;
                    for(u32 j = 0; j < nibCount; j++)
                    {
                        if(indexBuffer[j] == indexToMatch)
                        {
                            ls_arrayAppend(&compendium.npcViewIndices, i);
                            
                            indexBuffer[j] = indexBuffer[nibCount-1];
                            nibCount -= 1;
                            break;
                        }
                    }
                }
            }
        } break;
        
        case SEARCH_TYPE:
        {
            //NOTE:Iterate over the name buffer to find all names which exactly contain the substring needle
            u8 needleBuf[256] = {};
            utf8 needle = { needleBuf, 0, 0, 256 };
            ls_utf8FromUTF32_t(&needle, searchData->searchBar->text);
            
            u8 dataBuffer[128] = {};
            utf8 needleLower = { dataBuffer, 0, 0, 128 };
            
            ls_utf8ToLower(&needleLower, needle);
            
            u16 indexBuffer[4096] = {};
            u16 nibCount = findNeedleAndFillIndexBuffer(needleLower, indexBuffer, searchData->searchBuffer, 4096);
            
            if(nibCount > 0)
            {
                ls_arrayClear(&compendium.npcViewIndices);
                for(u16 i = 0; i < compendium.codex.npcPages.count; i++)
                {
                    u16 indexToMatch = compendium.codex.npcPages[i].type;
                    for(u32 j = 0; j < nibCount; j++)
                    {
                        if(indexBuffer[j] == indexToMatch)
                        {
                            ls_arrayAppend(&compendium.npcViewIndices, i);
                            break;
                        }
                    }
                }
            }
        } break;
        
        case SEARCH_GS:
        {
            s32 indexBuffer[gsSetCount+1] = {};
            s32 nibCount = findGSAndFillIndexBuffer(searchData->searchBar->text, indexBuffer);
            
            if(nibCount > 0)
            {
                ls_arrayClear(&compendium.npcViewIndices);
                for(u16 i = 0; i < compendium.codex.npcPages.count; i++)
                {
                    s32 entryToMatch = CompendiumGetGSIndexFromSet(compendium.codex.npcPages[i].gs);
                    for(u32 j = 0; j < nibCount; j++)
                    {
                        if(indexBuffer[j] == entryToMatch)
                        {
                            ls_arrayAppend(&compendium.npcViewIndices, i);
                            break;
                        }
                    }
                }
            }
        } break;
    }
    
    //NOTE: Adjust scrollbar height to new table count.
    if(compendium.npcViewIndices.count > 30) { npcTableScroll.minY = -((compendium.npcViewIndices.count-30) * 19); }
    else                                     { npcTableScroll.minY = -1; }
    
    ls_arenaUse(globalArena);
    
    return FALSE;
}
