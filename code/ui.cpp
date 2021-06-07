struct UIContext
{
    u8 *drawBuffer;
    u32 width;
    u32 height;
    
    void (*callbackRender)();
};

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
            
            At[y*c->width + x] = (u32)RGB(0x22, 0x22, 0x22);
        }
    }
    
}

void ls_uiRender(UIContext *c)
{
    c->callbackRender();
    return;
}