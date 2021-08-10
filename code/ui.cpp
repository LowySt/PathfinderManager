#define RGBA(r,g,b,a) (u32)((a<<24)|(r<<16)|(g<<8)|b)
#define RGB(r,g,b) (u32)((0xFF<<24)|(r<<16)|(g<<8)|b)
#define RGBg(v) (u32)((0xFF<<24)|(v<<16)|(v<<8)|v)

struct UIGlyph
{
    u8 *data;
    u32 size;
    
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

u32 ls_uiDarkenRGB(u32 Color, u32 factor)
{
    u8 *c8 = (u8 *)&Color;
    c8[0] -= factor;
    c8[1] -= factor;
    c8[2] -= factor;
    
    return Color;
}

void ls_uiBackground(UIContext *c, u32 color)
{
    for(u32 y = 0; y < c->height; y++)
    {
        for(u32 x = 0; x < c->width; x++)
        {
            ((u32 *)c->drawBuffer)[y*c->width + x] = color;
        }
    }
}

void ls_uiButton(UIContext *c, s32 xPos, s32 yPos, s32 w, s32 h)
{
    u32 *At = (u32 *)c->drawBuffer;
    
    for(s32 y = yPos; y < yPos+h; y++)
    {
        for(s32 x = xPos; x < xPos+w; x++)
        {
            if(x < 0 || x >= c->width)  continue;
            if(y < 0 || y >= c->height) continue;
            
            At[y*c->width + x] = RGBg(0x22);
        }
    }
    
}

void ls_uiBitmap(UIContext *c, s32 xPos, s32 yPos, u32 *data, s32 w, s32 h)
{
    u32 *At = (u32 *)c->drawBuffer;
    
    for(s32 y = yPos, eY = 0; y < yPos+h; y++, eY++)
    {
        for(s32 x = xPos, eX = 0; x < xPos+w; x++, eX++)
        {
            if(x < 0 || x >= c->width)  continue;
            if(y < 0 || y >= c->height) continue;
            
            u32 Color = data[eY*w + eX];
            At[y*c->width + x] = Color;
        }
    }
}

void ls_uiGS2Glyph(UIContext *c, s32 xPos, s32 yPos, UIGlyph *glyph, u32 textColor)
{
    u32 *At = (u32 *)c->drawBuffer;
    
    u32 textColor1 = textColor;
    u32 textColor2 = ls_uiDarkenRGB(textColor1, 0x04);
    u32 textColor3 = ls_uiDarkenRGB(textColor2, 0x04);
    u32 textColor4 = ls_uiDarkenRGB(textColor3, 0x04);
    
    u32 colorTable[5] = {(u32)appBkgRGB, textColor1, textColor2, textColor3, textColor4 };
    
    u32 realWidth = glyph->size / glyph->height;
    
    for(s32 y = yPos, eY = glyph->height-1; eY >= 0; y++, eY--)
    {
        for(s32 x = xPos, eX = 0; eX < realWidth; x++, eX++)
        {
            if(x < 0 || x >= c->width)  continue;
            if(y < 0 || y >= c->height) continue;
            
            u32 Color = colorTable[glyph->data[eY*realWidth + eX]];
            At[y*c->width + x] = Color;
        }
    }
}

void ls_uiGS2String(UIContext *c, s32 xPos, s32 yPos, string text, u32 textColor)
{
    s32 currXPos = xPos;
    for(u32 i = 0; i < text.len; i++)
    {
        UIGlyph *currGlyph = &c->font.glyph[text.data[i]];
        ls_uiGS2Glyph(c, currXPos, yPos, currGlyph, textColor);
        currXPos += currGlyph->xAdv;
    }
}

void ls_uiRender(UIContext *c)
{
    c->callbackRender();
    return;
}
