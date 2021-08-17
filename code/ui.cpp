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
    char face[64];
    u32 pointSize;
};

struct UIContext
{
    u8 *drawBuffer;
    u32 width;
    u32 height;
    
    UIFont font;
    
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

void ls_uiBackground(UIContext *cxt, Color c)
{
    for(u32 y = 0; y < cxt->height; y++)
    {
        for(u32 x = 0; x < cxt->width; x++)
        {
            ((u32 *)cxt->drawBuffer)[y*cxt->width + x] = c;
        }
    }
}

void ls_uiButton(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h)
{
    u32 *At = (u32 *)cxt->drawBuffer;
    
    for(s32 y = yPos; y < yPos+h; y++)
    {
        for(s32 x = xPos; x < xPos+w; x++)
        {
            if(x < 0 || x >= cxt->width)  continue;
            if(y < 0 || y >= cxt->height) continue;
            
            At[y*cxt->width + x] = RGBg(0x22);
        }
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
            
            Color c = ls_uiRGBAtoARGB(data[eY*w + eX]);
            At[y*cxt->width + x] = c;
        }
    }
}

void ls_uiGS2Glyph(UIContext *cxt, s32 xPos, s32 yPos, UIGlyph *glyph, Color textColor)
{
    u32 *At = (u32 *)cxt->drawBuffer;
    
    const u32 colorTableSize = 5;
    Color colorTable[colorTableSize] = {};
    ls_uiFillGSColorTable(textColor, appBkgRGB, 0x04, colorTable, colorTableSize);
    
    u32 realWidth = glyph->size / glyph->height;
    
    for(s32 y = yPos, eY = glyph->height-1; eY >= 0; y++, eY--)
    {
        for(s32 x = xPos, eX = 0; eX < realWidth; x++, eX++)
        {
            if(x < 0 || x >= cxt->width)  continue;
            if(y < 0 || y >= cxt->height) continue;
            
            Color c = colorTable[glyph->data[eY*realWidth + eX]];
            At[y*cxt->width + x] = c;
        }
    }
}

void ls_uiGS4Glyph(UIContext *cxt, s32 xPos, s32 yPos, UIGlyph *glyph, Color textColor)
{
    u32 *At = (u32 *)cxt->drawBuffer;
    
    const u32 colorTableSize = 17;
    u32 colorTable[colorTableSize] = {};
    ls_uiFillGSColorTable(textColor, appBkgRGB, 0x01, colorTable, colorTableSize);
    
    u32 realWidth = glyph->size / glyph->height;
    
    for(s32 y = yPos, eY = glyph->height-1; eY >= 0; y++, eY--)
    {
        for(s32 x = xPos, eX = 0; eX < realWidth; x++, eX++)
        {
            if(x < 0 || x >= cxt->width)  continue;
            if(y < 0 || y >= cxt->height) continue;
            
            u32 Color = colorTable[glyph->data[eY*realWidth + eX]];
            At[y*cxt->width + x] = Color;
        }
    }
}

void ls_uiGlyph(UIContext *cxt, s32 xPos, s32 yPos, UIGlyph *glyph, Color textColor)
{
    u32 *At = (u32 *)cxt->drawBuffer;
    
    const u32 colorTableSize = 256;
    u32 colorTable[colorTableSize] = {};
    ls_uiFillGSColorTable(textColor, appBkgRGB, 0x01, colorTable, colorTableSize);
    
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

void ls_uiGS2String(UIContext *cxt, s32 xPos, s32 yPos, string text, Color textColor)
{
    s32 currXPos = xPos;
    for(u32 i = 0; i < text.len; i++)
    {
        UIGlyph *currGlyph = &cxt->font.glyph[text.data[i]];
        ls_uiGS2Glyph(cxt, currXPos, yPos, currGlyph, textColor);
        currXPos += currGlyph->xAdv;
    }
}

void ls_uiGS4String(UIContext *cxt, s32 xPos, s32 yPos, string text, Color textColor)
{
    s32 currXPos = xPos;
    for(u32 i = 0; i < text.len; i++)
    {
        UIGlyph *currGlyph = &cxt->font.glyph[text.data[i]];
        ls_uiGS4Glyph(cxt, currXPos, yPos, currGlyph, textColor);
        currXPos += currGlyph->xAdv;
    }
}

void ls_uiGlyphString(UIContext *cxt, s32 xPos, s32 yPos, string text, Color textColor)
{
    s32 currXPos = xPos;
    for(u32 i = 0; i < text.len; i++)
    {
        UIGlyph *currGlyph = &cxt->font.glyph[text.data[i]];
        ls_uiGlyph(cxt, currXPos, yPos, currGlyph, textColor);
        currXPos += currGlyph->xAdv;
    }
}

void ls_uiRender(UIContext *c)
{
    c->callbackRender();
    return;
}
