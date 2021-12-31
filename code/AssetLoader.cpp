void loadAssetFile(UIContext *cxt, string assetFilePath)
{
    AssertMsg(cxt, "Context ptr was NULL");
    
    ls_arenaUse(fileArena);
    buffer buff = ls_bufferInitFromFile(assetFilePath);
    ls_arenaUse(globalArena);
    
    cxt->numFonts = ls_bufferReadDWord(&buff);
    cxt->fonts = (UIFont *)ls_alloc(sizeof(UIFont)*cxt->numFonts);
    
    for(u32 i = 0; i < cxt->numFonts; i++)
    {
        UIFont *font = cxt->fonts + i;
        
        font->pixelHeight  = ls_bufferReadDWord(&buff);
        font->maxCodepoint = ls_bufferReadDWord(&buff);
        
        font->glyph = (UIGlyph *)ls_alloc(sizeof(UIGlyph) * (font->maxCodepoint + 1));
        
        //NOTE: ASCII characters
        for(u32 codepoint = 32; codepoint <= 126; codepoint++)
        {
            UIGlyph *currGlyph = &font->glyph[codepoint];
            
            currGlyph->size = ls_bufferPeekDWord(&buff);
            currGlyph->data = (u8 *)ls_alloc(sizeof(u8)*currGlyph->size);
            ls_bufferReadData(&buff, currGlyph->data); //TODO: Bad design
            
            currGlyph->width = ls_bufferReadDWord(&buff);
            currGlyph->height = ls_bufferReadDWord(&buff);
            currGlyph->x0 = ls_bufferReadDWord(&buff);
            currGlyph->x1 = ls_bufferReadDWord(&buff);
            currGlyph->y0 = ls_bufferReadDWord(&buff);
            currGlyph->y1 = ls_bufferReadDWord(&buff);
            currGlyph->xAdv = ls_bufferReadDWord(&buff);
            currGlyph->yAdv = ls_bufferReadDWord(&buff);
        }
        
        //NOTE: Plane 0 Latin Glyphs
        for(u32 codepoint = 0x00A1; codepoint <= 0x024F; codepoint++)
        {
            UIGlyph *currGlyph = &font->glyph[codepoint];
            
            currGlyph->size = ls_bufferPeekDWord(&buff);
            currGlyph->data = (u8 *)ls_alloc(sizeof(u8)*currGlyph->size);
            ls_bufferReadData(&buff, currGlyph->data); //TODO: Bad design
            
            currGlyph->width = ls_bufferReadDWord(&buff);
            currGlyph->height = ls_bufferReadDWord(&buff);
            currGlyph->x0 = ls_bufferReadDWord(&buff);
            currGlyph->x1 = ls_bufferReadDWord(&buff);
            currGlyph->y0 = ls_bufferReadDWord(&buff);
            currGlyph->y1 = ls_bufferReadDWord(&buff);
            currGlyph->xAdv = ls_bufferReadDWord(&buff);
            currGlyph->yAdv = ls_bufferReadDWord(&buff);
        }
        
        font->kernAdvanceTable = (s32 **)ls_alloc(sizeof(s32 *) * (font->maxCodepoint + 1));
        for(u32 i = 0; i < font->maxCodepoint+1; i++) 
        { font->kernAdvanceTable[i] = (s32 *)ls_alloc(sizeof(s32) * (font->maxCodepoint + 1)); }
        
        //NOTE: KernTable will be laid out as rows, so [0][1], [0][2], [0][3]... etc.
        for(u32 cp1 = 32; cp1 <= 126; cp1++)
        {
            for(u32 cp2 = 32; cp2 <= 126; cp2++)
            {
                font->kernAdvanceTable[cp1][cp2] = ls_bufferReadDWord(&buff);
            }
        }
        
        for(u32 cp1 = 0x00A1; cp1 <= 0x024F; cp1++)
        {
            for(u32 cp2 = 0x00A1; cp2 <= 0x024F; cp2++)
            {
                font->kernAdvanceTable[cp1][cp2] = ls_bufferReadDWord(&buff);
            }
        }
    }
    
    ls_arenaDestroy(fileArena);
}