#define RGBA(r,g,b,a) (u32)((a<<24)|(r<<16)|(g<<8)|b)
#define RGB(r,g,b) (u32)((0xFF<<24)|(r<<16)|(g<<8)|b)
#define RGBg(v) (u32)((0xFF<<24)|(v<<16)|(v<<8)|v)

struct UIContext
{
    u8 *drawBuffer;
    u32 width;
    u32 height;
    
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

void ls_uiGrayscale2(UIContext *c, s32 xPos, s32 yPos, u8 *data, s32 w, s32 h, u32 textColor)
{
    u32 *At = (u32 *)c->drawBuffer;
    
    u32 textColor1 = textColor;
    u32 textColor2 = ls_uiDarkenRGB(textColor1, 0x04);
    u32 textColor3 = ls_uiDarkenRGB(textColor2, 0x04);
    u32 textColor4 = ls_uiDarkenRGB(textColor3, 0x04);
    
    for(s32 y = yPos, eY = h-1; y < yPos+h; y++, eY--)
    {
        for(s32 x = xPos, eX = 0; x < xPos+w; x++, eX++)
        {
            if(x < 0 || x >= c->width)  continue;
            if(y < 0 || y >= c->height) continue;
            
            u32 Color = 0;
            switch(data[eY*w + eX])
            {
                case 0: Color = (u32)appBkgRGB; break;
                case 1: Color = textColor1; break;
                case 2: Color = textColor2; break;
                case 3: Color = textColor3; break;
                case 4: Color = textColor4; break;
            }
            
            At[y*c->width + x] = Color;
        }
    }
}

void ls_uiRender(UIContext *c)
{
    c->callbackRender();
    return;
}

//const u32 aLetterBufferSize = 4*128*128;
//u8 aLetterBuffer[aLetterBufferSize] = {};
u8 *aLetterBuffer = NULL;
DWORD aLetterSize = 0;
u32 aLetterWidth = 0;
u32 aLetterHeight = 0;