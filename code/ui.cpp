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
    
    s32 x0, x1;
    s32 y0, y1;
    
    s32 xAdv;
    s32 yAdv;
};

struct UIFont
{
    UIGlyph glyph[256];
    s32 **kernAdvanceTable; //NOTE: Needs to keep this in the heap cause it stack-overflows.
    
    u32 pixelHeight;
};

struct UIScissor
{
    struct UIRect { s32 x, y, w, h; };
    stack rects;
    
    UIRect *currRect;
};

struct UIContext;
typedef void(*ButtonProc)(UIContext *cxt);
struct UIButton
{
    string name;
    b32 isHot;
    b32 isHeld;
    
    ButtonProc onClick;
    ButtonProc onHold;
};

struct UITextBox
{
    string text;
    b32 isSelected;
    
    u32 dtCaret;
    b32 isCaretOn;
    s32 caretIndex;
};

struct UIListBox
{
    Array<string> list;
    s32 selectedIndex;
    
    u32 dtOpen;
    b32 isOpening;
    b32 isOpen;
};

struct UIContext
{
    u8 *drawBuffer;
    u32 width;
    u32 height;
    
    UIFont font[4]; //TODO: Hardcoded
    char   face[128];
    
    UIFont *currFont;
    
    Color backgroundColor;
    Color widgetColor;
    Color highliteColor;
    Color borderColor;
    Color textColor;
    
    UIScissor scissor;
    
    void (*callbackRender)();
    u32 dt;
};

void ls_uiPushScissor(UIContext *cxt, s32 x, s32 y, s32 w, s32 h)
{
    UIScissor *scissor = &cxt->scissor;
    
    if(!scissor->rects.data) { scissor->rects = ls_stackInit(sizeof(UIScissor::UIRect), 8); }
    UIScissor::UIRect rect = { x, y, w, h };
    
    ls_stackPush(&scissor->rects, (void *)&rect);
    scissor->currRect = (UIScissor::UIRect *)ls_stackTop(&scissor->rects);
}

void ls_uiPopScissor(UIContext *cxt)
{
    UIScissor *scissor = &cxt->scissor;
    AssertMsg(scissor->rects.data, "Scissor stack is null\n");
    AssertMsg(scissor->rects.count > 0, "Scissor stack is already empty\n");
    
    ls_stackPop(&scissor->rects);
    if(scissor->rects.count > 0) //TODO: Top doesn't care if the count == 0
    {
        scissor->currRect = (UIScissor::UIRect *)ls_stackTop(&scissor->rects);
    }
    else
    {
        scissor->currRect = NULL;
    }
    
}

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
    UIScissor::UIRect *scRect = cxt->scissor.currRect;
    
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
            
            if(x < scRect->x || x >= scRect->x+scRect->w) continue;
            if(y < scRect->y || y >= scRect->y+scRect->h) continue;
            
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
                
                if(x < scRect->x || x >= scRect->x+scRect->w) continue;
                if(y < scRect->y || y >= scRect->y+scRect->h) continue;
                
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
                
                if(x < scRect->x || x >= scRect->x+scRect->w) continue;
                if(y < scRect->y || y >= scRect->y+scRect->h) continue;
                
                At[y*cxt->width + x] = c;
            }
        }
    }
}

inline
void ls_uiBorderedRect(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h)
{
    Color C = cxt->borderColor;
    
    ls_uiFillRect(cxt, xPos,     yPos,     w, 1, C);
    ls_uiFillRect(cxt, xPos,     yPos+h-1, w, 1, C);
    ls_uiFillRect(cxt, xPos,     yPos,     1, h, C);
    ls_uiFillRect(cxt, xPos+w-1, yPos,     1, h, C);
    
    ls_uiFillRect(cxt, xPos+1, yPos+1, w-2, h-2, cxt->widgetColor);
}

inline
void ls_uiBorderedRect(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h, Color widgetColor)
{
    Color C = cxt->borderColor;
    
    ls_uiFillRect(cxt, xPos,     yPos,     w, 1, C);
    ls_uiFillRect(cxt, xPos,     yPos+h-1, w, 1, C);
    ls_uiFillRect(cxt, xPos,     yPos,     1, h, C);
    ls_uiFillRect(cxt, xPos+w-1, yPos,     1, h, C);
    
    ls_uiFillRect(cxt, xPos+1, yPos+1, w-2, h-2, widgetColor);
}

inline
void ls_uiBorderedRect(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h, Color widgetColor, Color borderColor)
{
    ls_uiFillRect(cxt, xPos,     yPos,     w, 1, borderColor);
    ls_uiFillRect(cxt, xPos,     yPos+h-1, w, 1, borderColor);
    ls_uiFillRect(cxt, xPos,     yPos,     1, h, borderColor);
    ls_uiFillRect(cxt, xPos+w-1, yPos,     1, h, borderColor);
    
    ls_uiFillRect(cxt, xPos+1, yPos+1, w-2, h-2, widgetColor);
}

void ls_uiBackground(UIContext *cxt)
{
    AssertMsg((cxt->height % 4) == 0, "Window Height not divisible by 4 (SIMD)\n");
    AssertMsg((cxt->width % 4) == 0, "Window Width not divisible by 4 (SIMD)\n");
    
    __m128i color = _mm_set1_epi32 ((int)cxt->backgroundColor);
    
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
    UIScissor::UIRect *scRect = cxt->scissor.currRect;
    
    u32 *At = (u32 *)cxt->drawBuffer;
    
    const u32 colorTableSize = 256;
    u32 colorTable[colorTableSize] = {};
    ls_uiFillGSColorTable(textColor, bkgColor, 0x01, colorTable, colorTableSize);
    
    for(s32 y = yPos-glyph->y1, eY = glyph->height-1; eY >= 0; y++, eY--)
        //for(s32 y = yPos, eY = 0; eY < glyph->height; y--, eY++)
    {
        for(s32 x = xPos+glyph->x0, eX = 0; eX < glyph->width; x++, eX++)
        {
            if(x < 0 || x >= cxt->width)  continue;
            if(y < 0 || y >= cxt->height) continue;
            
            if(x < scRect->x || x >= scRect->x+scRect->w) continue;
            if(y < scRect->y || y >= scRect->y+scRect->h) continue;
            
            u32 Color = colorTable[glyph->data[eY*glyph->width + eX]];
            At[y*cxt->width + x] = Color;
        }
    }
}

s32 ls_uiGetKernAdvance(UIContext *cxt, s32 codepoint1, s32 codepoint2)
{
    UIFont *font = cxt->currFont;
    s32 kernAdvance = font->kernAdvanceTable[codepoint1][codepoint2];
    
    return kernAdvance;
}

void ls_uiGlyphString(UIContext *cxt, s32 xPos, s32 yPos, string text, Color textColor, Color bkgColor = appBkgRGB)
{
    s32 currXPos = xPos;
    s32 currYPos = yPos;
    for(u32 i = 0; i < text.len; i++)
    {
        u32 indexInGlyphArray = text.data[i];
        AssertMsg(indexInGlyphArray <= 256, "GlyphIndex OutOfBounds\n"); //TODO: HARDCODED //TODO: Only handling ASCII
        
        UIGlyph *currGlyph = &cxt->currFont->glyph[indexInGlyphArray];
        ls_uiGlyph(cxt, currXPos, yPos, currGlyph, textColor, bkgColor);
        
        s32 kernAdvance = 0;
        if(i < text.len-1) { kernAdvance = ls_uiGetKernAdvance(cxt, text.data[i], text.data[i+1]); }
        
        currXPos += (currGlyph->xAdv + kernAdvance);
    }
}

s32 ls_uiGlyphStringLen(UIContext *cxt, string text)
{
    s32 totalLen = 0;
    for(u32 i = 0; i < text.len; i++)
    {
        u32 indexInGlyphArray = text.data[i];
        AssertMsg(indexInGlyphArray <= 256, "GlyphIndex OutOfBounds\n"); //TODO: HARDCODED //TODO: Only handling ASCII
        
        UIGlyph *currGlyph = &cxt->currFont->glyph[indexInGlyphArray];
        
        s32 kernAdvance = 0;
        if(i < text.len-1) { kernAdvance = ls_uiGetKernAdvance(cxt, text.data[i], text.data[i+1]); }
        
        totalLen += (currGlyph->xAdv + kernAdvance);
    }
    
    return totalLen;
}

void ls_uiSelectFontByPixelHeight(UIContext *cxt, u32 pixelHeight)
{
    //TODO: Hardcoded
    b32 found = FALSE;
    for(u32 i = 0; i < 4; i++)
    { if(cxt->font[i].pixelHeight == pixelHeight) { found = TRUE; cxt->currFont = &cxt->font[i]; } }
    
    AssertMsg(found, "Asked pixelHeight not available\n");
}

void ls_uiButton(UIContext *cxt, UIButton button, s32 xPos, s32 yPos, s32 w, s32 h)
{
    if(MouseInRect(xPos, yPos, w, h))
    { 
        button.isHot = TRUE; //TODO: Not used.
        if(LeftClick)
        {
            button.onClick(cxt);
        }
        if(LeftHold) 
        {
            button.isHeld = TRUE; 
            button.onHold(cxt); 
        }
    }
    
    Color bkgColor = button.isHeld ? cxt->highliteColor : cxt->widgetColor;
    
    ls_uiBorderedRect(cxt, xPos, yPos, w, h, bkgColor);
    
    ls_uiSelectFontByPixelHeight(cxt, 16);
    ls_uiGlyphString(cxt, xPos+(w/4), yPos+4, button.name, cxt->textColor, bkgColor);
}

void ls_uiTextBox(UIContext *cxt, UITextBox *box, s32 xPos, s32 yPos, s32 w, s32 h)
{
    ls_uiSelectFontByPixelHeight(cxt, 32);
    
    ls_uiBorderedRect(cxt, xPos, yPos, w, h);
    
    ls_uiPushScissor(cxt, xPos+4, yPos, w-8, h);
    
    if(box->isSelected)
    {
        //NOTE: Draw characters.
        if(HasPrintableKey()) 
        {
            if(box->caretIndex == box->text.len) { ls_strAppendChar(&box->text, GetPrintableKey()); }
            else { ls_strInsertChar(&box->text, GetPrintableKey(), box->caretIndex); }
            
            box->caretIndex += 1; 
        }
        if(KeyPress(keyMap::Backspace) && box->text.len > 0 && box->caretIndex > 0) 
        {
            if(box->caretIndex == box->text.len) { ls_strTrimRight(&box->text, 1); }
            else { ls_strRmIdx(&box->text, box->caretIndex-1); }
            
            box->caretIndex -= 1;
        }
        if(KeyPress(keyMap::Delete) && box->text.len > 0 && box->caretIndex < box->text.len)
        {
            if(box->caretIndex == box->text.len-1) { ls_strTrimRight(&box->text, 1); }
            else { ls_strRmIdx(&box->text, box->caretIndex); }
        }
        if(KeyPress(keyMap::LArrow) && box->caretIndex > 0) { box->caretIndex -= 1; }
        if(KeyPress(keyMap::RArrow) && box->caretIndex < box->text.len) { box->caretIndex += 1; }
        if(KeyPress(keyMap::Home)) { box->caretIndex = 0; }
        if(KeyPress(keyMap::End)) { box->caretIndex = box->text.len; }
        
        if(KeyHeld(keyMap::Control) && KeyPress(keyMap::V))
        {
            //TODO: This could be inserting strange multiple-byte character... need to look into it.
            u8 buff[128] = {};
            u32 copiedLen = GetClipboard(buff, 128);
            
            if(box->caretIndex == box->text.len) { ls_strAppendCStr(&box->text, (char *)buff); }
            else { ls_strInsertCStr(&box->text, (char *)buff, box->caretIndex); }
            
            box->caretIndex += copiedLen;
        }
        
        if(KeyHeld(keyMap::Control) && KeyPress(keyMap::C))
        {
            SetClipboard(box->text.data, box->text.len);
        }
        
        //TODO: The positioning is hardcoded. Bad Pasta.
        if(box->text.len > 0)
        { 
            ls_uiGlyphString(cxt, xPos+10, yPos+8, box->text, cxt->textColor, cxt->widgetColor);
        }
        
        //NOTE: Draw the Caret
        box->dtCaret += cxt->dt;
        if(box->dtCaret >= 600) { box->dtCaret = 0; box->isCaretOn = !box->isCaretOn; }
        
        if(box->isCaretOn)
        {
            UIGlyph *caretGlyph = &cxt->currFont->glyph['|'];
            
            string tmp = {box->text.data, (u32)box->caretIndex, (u32)box->caretIndex};
            
            u32 stringLen = ls_uiGlyphStringLen(cxt, tmp);
            ls_uiGlyph(cxt, xPos+12+stringLen, yPos+10, caretGlyph, cxt->textColor, cxt->widgetColor);
        }
        
    }
    
    ls_uiPopScissor(cxt);
}

void ls_uiDrawArrow(UIContext *cxt, s32 xPos, s32 yPos)
{
    UIScissor::UIRect *scRect = cxt->scissor.currRect;
    u32 *At = (u32 *)cxt->drawBuffer;
    
    //TODO: Hardcoded positions.
    s32 xBase = xPos;
    s32 xEnd  = xBase + 10;
    
    for(s32 y = yPos+18; y >= yPos; y--)
    {
        for(s32 x = xBase; x < xEnd; x++)
        {
            if(x < 0 || x >= cxt->width)  continue;
            if(y < 0 || y >= cxt->height) continue;
            
            if(x < scRect->x || x >= scRect->x+scRect->w) continue;
            if(y < scRect->y || y >= scRect->y+scRect->h) continue;
            
            At[y*cxt->width + x] = RGBg(0x00);
        }
        
        xBase += 1;
        xEnd  -= 1;
    }
}

inline void ls_uiListBoxAddEntry(UIContext *cxt, UIListBox *list, string s)
{ list->list.push(s); }

inline void ls_uiListBoxRemoveEntry(UIContext *cxt, UIListBox *list, u32 index)
{ list->list.remove(index); }

void ls_uiListBox(UIContext *cxt, UIListBox *list, s32 xPos, s32 yPos, s32 w, s32 h)
{
    ls_uiSelectFontByPixelHeight(cxt, 16);
    
    ls_uiBorderedRect(cxt, xPos, yPos, w, h);
    
    ls_uiPushScissor(cxt, xPos+4, yPos, w-8, h);
    
    ls_uiDrawArrow(cxt, xPos + w - 20, yPos);
    
    if(LeftClick && MouseInRect(xPos+w-20, yPos+7, 30, 30))
    {
        if(list->isOpen) { list->isOpen = FALSE; }
        else { list->isOpening = TRUE; }
    }
    
    if(list->list.count)
    {
        string selected = list->list[list->selectedIndex];
        ls_uiGlyphString(cxt, xPos+10, yPos+12, selected, cxt->textColor, cxt->widgetColor);
    }
    
    ls_uiPopScissor(cxt);
    
    
    //TODO: Should I try adding another Scissor? Has to be added inside the branches.
    if(list->isOpening)
    {
        list->dtOpen += cxt->dt;
        s32 maxHeight = (list->list.count-1)*h;
        
        s32 height = 0;
        if(list->dtOpen > 17)  { height = maxHeight*0.10f; }
        if(list->dtOpen > 34)  { height = maxHeight*0.35f; }
        if(list->dtOpen > 52)  { height = maxHeight*0.70f; }
        if(list->dtOpen > 70) { list->isOpen = TRUE; list->isOpening = FALSE; list->dtOpen = 0; }
        
        if(!list->isOpen)
        {
            ls_uiFillRect(cxt, xPos+1, yPos-height, w-2, height, cxt->widgetColor);
        }
    }
    
    if(list->isOpen)
    {
        s32 maxHeight = (list->list.count-1)*h;
        ls_uiFillRect(cxt, xPos+1, yPos-maxHeight, w-2, maxHeight, cxt->widgetColor);
        
        for(u32 i = 0; i < list->list.count; i++) 
        {
            string currStr = list->list[i];
            ls_uiGlyphString(cxt, xPos+10, yPos+12-(h*i), currStr, cxt->textColor, cxt->widgetColor);
        }
    }
}

void ls_uiRender(UIContext *c)
{
    c->callbackRender();
    return;
}
