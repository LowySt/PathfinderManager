#define RGBA(r,g,b,a) (u32)((a<<24)|(r<<16)|(g<<8)|b)
#define RGB(r,g,b) (u32)((0xFF<<24)|(r<<16)|(g<<8)|b)
#define RGBg(v) (u32)((0xFF<<24)|(v<<16)|(v<<8)|v)

typedef u32 Color;

struct UIGlyph
{
    u8 *data;
    u32 size;
    
    u32 codepoint;
    u32 idxInFont;
    
    u32 width;
    u32 height;
    
    u32 xOrig;
    u32 yOrig;
    
    u32 xAdv;
    u32 yAdv;
};

struct UIFont
{
    UIGlyph glyph[256];
    u32 pixelHeight;
};

struct UITextBox
{
    string text;
};

struct UIContext
{
    u8 *drawBuffer;
    u32 width;
    u32 height;
    
    UIFont font[4]; //TODO: Hardcoded
    char   face[128];
    
    UIFont *currFont;
    
    void (*callbackRender)();
};

Color ls_uiDarkenRGB(Color c, u32 factor)
{
    u8 *c8 = (u8 *)&c;
    c8[0] -= factor;
    c8[1] -= factor;
    c8[2] -= factor;
    
    return c;
}

Color ls_uiLightenRGB(Color c, u32 factor)
{
    u8 *c8 = (u8 *)&c;
    c8[0] += factor;
    c8[1] += factor;
    c8[2] += factor;
    
    return c;
}

Color ls_uiAlphaBlend(Color source, Color dest, u8 alpha)
{
    u8 *c8 = (u8 *)&source;
    u8 bS = c8[0];
    u8 gS = c8[1];
    u8 rS = c8[2];
    u8 aS = alpha;
    
    c8 = (u8 *)&dest;
    u8 bD = c8[0];
    u8 gD = c8[1];
    u8 rD = c8[2];
    u8 aD = c8[3];
    
    f32 factor = (f32)(255 - aS) / 255.0f;
    f32 aMulti = (f32)aS / 255.0f;
    
    Color Result = 0;
    c8 = (u8 *)&Result;
    
    c8[0] = bS*aMulti + bD*factor;
    c8[1] = gS*aMulti + gD*factor;
    c8[2] = rS*aMulti + rD*factor;
    c8[3] = aS + aD*factor;
    
    return Result;
}

Color ls_uiRGBAtoARGB(Color c)
{
    u8 *c8 = (u8 *)&c;
    
    u8 A = c8[0];
    
    c8[0] = c8[1];
    c8[1] = c8[2];
    c8[2] = c8[3];
    c8[3] = A;
    
    return c;
}

void ls_uiFillGSColorTable(Color c, Color baseColor, u8 darkenFactor, Color *table, u32 tableSize)
{
    table[0] = baseColor;
    
    u8 alphaCurr = (u32)ls_ceil((( 1.0f / (f32) tableSize ) * 255.0f));
    Color currColor = ls_uiAlphaBlend(c, baseColor, alphaCurr);
    table[1] = currColor;
    
    for(u32 i = 2; i < tableSize-1; i++)
    {
        table[i] = currColor;
        
        alphaCurr = (u32)ls_ceil((((f32)i / (f32) tableSize) * 255.0f));
        currColor = ls_uiAlphaBlend(c, baseColor, alphaCurr);
    }
    
    table[tableSize-1] = c;
    
    return;
}

void ls_uiFillRect(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h, Color c)
{
    s32 diffWidth = (w % 4);
    s32 simdWidth = w - diffWidth;
    
    s32 diffHeight = (h % 4);
    s32 simdHeight = h - diffHeight;
    
    //NOTE: Do the first Sub-Rectangle divisible by 4.
    __m128i color = _mm_set1_epi32 ((int)c);
    for(s32 y = yPos; y < yPos+simdHeight; y++)
    {
        for(s32 x = xPos; x < xPos+simdWidth; x+=4)
        {
            if(x < 0 || x >= cxt->width)  continue;
            if(y < 0 || y >= cxt->height) continue;
            
            u32 idx = ((y*cxt->width) + x)*sizeof(s32);
            __m128i *At = (__m128i *)(cxt->drawBuffer + idx);
            _mm_storeu_si128(At, color);
        }
    }
    
    //NOTE: Complete the 2 remaining Sub-Rectangles at the right and top. (if there are).
    u32 *At = (u32 *)cxt->drawBuffer;
    
    if(diffWidth) 
    {
        for(s32 y = yPos; y < yPos+h; y++)
        {
            for(s32 x = xPos+simdWidth; x < xPos+w; x++)
            {
                if(x < 0 || x >= cxt->width)  continue;
                if(y < 0 || y >= cxt->height) continue;
                
                At[y*cxt->width + x] = c;
            }
        }
    }
    
    if(diffHeight)
    {
        for(s32 y = yPos+simdHeight; y < yPos+h; y++)
        {
            for(s32 x = xPos; x < xPos+w; x++)
            {
                if(x < 0 || x >= cxt->width)  continue;
                if(y < 0 || y >= cxt->height) continue;
                
                At[y*cxt->width + x] = c;
            }
        }
    }
}

void ls_uiBackground(UIContext *cxt, Color c)
{
    AssertMsg((cxt->height % 4) == 0, "Window Height not divisible by 4 (SIMD)\n");
    AssertMsg((cxt->width % 4) == 0, "Window Width not divisible by 4 (SIMD)\n");
    
    __m128i color = _mm_set1_epi32 ((int)c);
    
    u32 numIterations = (cxt->height*cxt->width) / 4;
    for(u32 i = 0; i < numIterations; i++)
    {
        u32 idx = i*sizeof(s32)*4;
        
        __m128i *At = (__m128i *)(cxt->drawBuffer + idx);
        _mm_storeu_si128(At, color);
    }
}

void ls_uiBitmap(UIContext *cxt, s32 xPos, s32 yPos, u32 *data, s32 w, s32 h)
{
    u32 *At = (u32 *)cxt->drawBuffer;
    
    for(s32 y = yPos, eY = 0; y < yPos+h; y++, eY++)
    {
        for(s32 x = xPos, eX = 0; x < xPos+w; x++, eX++)
        {
            if(x < 0 || x >= cxt->width)  continue;
            if(y < 0 || y >= cxt->height) continue;
            
            At[y*cxt->width + x] = data[eY*w + eX];
        }
    }
}

void ls_uiGlyph(UIContext *cxt, s32 xPos, s32 yPos, UIGlyph *glyph, Color textColor, Color bkgColor)
{
    u32 *At = (u32 *)cxt->drawBuffer;
    
    const u32 colorTableSize = 256;
    u32 colorTable[colorTableSize] = {};
    ls_uiFillGSColorTable(textColor, bkgColor, 0x01, colorTable, colorTableSize);
    
    for(s32 y = yPos, eY = glyph->height-1; eY >= 0; y++, eY--)
    {
        for(s32 x = xPos, eX = 0; eX < glyph->width; x++, eX++)
        {
            if(x < 0 || x >= cxt->width)  continue;
            if(y < 0 || y >= cxt->height) continue;
            
            u32 Color = colorTable[glyph->data[eY*glyph->width + eX]];
            At[y*cxt->width + x] = Color;
        }
    }
}

void ls_uiGlyphString(UIContext *cxt, s32 xPos, s32 yPos, string text, Color textColor, Color bkgColor = appBkgRGB)
{
    s32 currXPos = xPos;
    for(u32 i = 0; i < text.len; i++)
    {
        u32 indexInGlyphArray = text.data[i];
        AssertMsg(indexInGlyphArray <= 256, "GlyphIndex OutOfBounds\n"); //TODO: HARDCODED //TODO: Only handling ASCII
        
        UIGlyph *currGlyph = &cxt->currFont->glyph[indexInGlyphArray];
        ls_uiGlyph(cxt, currXPos, yPos, currGlyph, textColor, bkgColor);
        currXPos += currGlyph->xAdv;
    }
}

void ls_uiSelectFontByPixelHeight(UIContext *cxt, u32 pixelHeight)
{
    //TODO: Hardcoded
    b32 found = FALSE;
    for(u32 i = 0; i < 4; i++)
    { if(cxt->font[i].pixelHeight == pixelHeight) { found = TRUE; cxt->currFont = &cxt->font[i]; } }
    
    AssertMsg(found, "Asked pixelHeight not available\n");
}

void ls_uiButton(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h, b32 isPressed)
{
    Color bkgColor = isPressed ? RGBg(0x65) : RGBg(0x45);
    
    //TODO: Draw the border without wasting CPU time.
    ls_uiFillRect(cxt, xPos, yPos, w, h, RGBg(0x22)); //NOTE:Border
    ls_uiFillRect(cxt, xPos+1, yPos+1, w-2, h-2, bkgColor);
    
    ls_uiSelectFontByPixelHeight(cxt, 16);
    ls_uiGlyphString(cxt, xPos+(w/4), yPos+4, ls_strConst("Button"), RGBg(0xCC), bkgColor);
}

void ls_uiTextBox(UIContext *cxt, UITextBox *box, s32 xPos, s32 yPos, s32 w, s32 h, b32 isSelected)
{
    //NOTE: Draw the box
    ls_uiFillRect(cxt, xPos, yPos, w, h, RGBg(0x22)); //NOTE:Border
    ls_uiFillRect(cxt, xPos+1, yPos+1, w-2, h-2, RGBg(0x45));
    
    //TODO: Draw the caret
    
    //NOTE: Draw characters.
    
    if(HasPrintableKey()) { ls_strAppendChar(&box->text, GetPrintableKey()); }
    
    if(box->text.len > 0)
    {
        ls_uiSelectFontByPixelHeight(cxt, 16);
        ls_uiGlyphString(cxt, xPos+10, yPos+4, box->text, RGBg(0xCC), RGBg(0x45));
    }
}

void ls_uiRender(UIContext *c)
{
    c->callbackRender();
    return;
}
