#define RGBA(r,g,b,a)  (u32)((a<<24)|(r<<16)|(g<<8)|b)
#define RGB(r,g,b)     (u32)((0xFF<<24)|(r<<16)|(g<<8)|b)
#define RGBg(v)        (u32)((0xFF<<24)|(v<<16)|(v<<8)|v)
#define SetAlpha(v, a) (u32)((v & 0x00FFFFFF) | (((u32)a)<<24))
#define GetAlpha(v)    ( u8)(((v) & 0xFF000000) >> 24)

typedef u32 Color;

struct UIGlyph
{
    u8 *data;
    u32 size;
    
    u32 width;
    u32 height;
    
    s32 x0, x1;
    s32 y0, y1;
    
    s32 xAdv;
    s32 yAdv;
};

enum UIFontSize
{
    FS_SMALL = 0,
    FS_MEDIUM = 1,
    FS_LARGE = 2,
    FS_EXTRALARGE = 3
};

//TODO: Add max descent of font to adjust text vertical position in text boxes.
struct UIFont
{
    UIGlyph *glyph;
    s32 **kernAdvanceTable; //NOTE: Needs to keep this in the heap cause it stack-overflows.
    
    u32 pixelHeight;
    u32 maxCodepoint;
};

struct UIScissor
{
    struct UIRect { s32 x, y, w, h; };
    stack rects;
    
    UIRect *currRect;
};

struct UILPane
{
    u32 dtOpen;
    b32 isOpening;
    b32 isOpen;
};

struct UIContext;

enum UIButtonStyle { UIBUTTON_TEXT, UIBUTTON_TEXT_NOBORDER, UIBUTTON_BMP };

typedef void(*ButtonProc)(UIContext *cxt, void *data);
struct UIButton
{
    UIButtonStyle style;
    
    unistring name;
    u8 *bmpData;
    s32 bmpW, bmpH;
    
    b32 isHot;
    b32 isHeld;
    
    ButtonProc onClick;
    ButtonProc onHold;
    
    void *data; //TODO: Separate onClick / onHold user data
};


typedef void(*TextBoxProc)(UIContext *cxt, void *data);
struct UITextBox
{
    unistring text;
    u32 maxLen;
    
    b32 isReadonly;
    
    u32 dtCaret;
    b32 isCaretOn;
    s32 caretIndex;
    
    s32 viewBeginIdx;
    s32 viewEndIdx;
    
    s32 selectBeginIdx;
    s32 selectEndIdx;
    b32 isSelecting;
    
    TextBoxProc preInput;
    TextBoxProc postInput;
    
    void *data; //TODO: Separate preInput / postInput user data
};


typedef void(*ListBoxProc)(UIContext *cxt, void *data);
struct UIListBox
{
    Array<unistring> list;
    s32 selectedIndex;
    
    u32 dtOpen;
    b32 isOpening;
    b32 isOpen;
    
    ListBoxProc onSelect;
    void *data;
};


enum SliderStyle { SL_LINE, SL_BOX };

struct UISlider
{
    b32 isHot;
    b32 isHeld;
    
    unistring text;
    
    s32 currValue;
    s32 maxValue;
    s32 minValue;
    
    f64 currPos;
    
    SliderStyle style;
    Color lColor;
    Color rColor;
};


//TODO: Very Shitty implementation of Menus
struct UIMenu
{
    UIButton  closeWindow;
    UIButton  minimize;
    
    b32 isOpen;
    s32 openIdx;
    Array<UIButton> items;
    
    UIMenu *sub;            //NOTE: There is 1 sub for every item
    u32     maxSub; 
};

struct UIContext
{
    u8 *drawBuffer;
    u32 width;
    u32 height;
    
    UIFont *fonts;
    u32 numFonts;
    
    UIFont *currFont;
    
    Color backgroundColor;
    Color borderColor;
    
    Color highliteColor;
    Color pressedColor;
    
    Color widgetColor;
    Color textColor;
    Color invWidgetColor;
    Color invTextColor;
    
    UIScissor scissor;
    
    u64 *currentFocus;
    u64 *lastFocus;
    b32 focusWasSetThisFrame;
    
    b32 nextFrameFocusChange;
    u64 *nextFrameFocus;
    
    u64 *mouseCapture;
    
    void (*callbackRender)();
    u32 dt;
};

void ls_uiFocusChange(UIContext *cxt, u64 *focus)
{
    cxt->nextFrameFocusChange = TRUE;
    cxt->nextFrameFocus = focus;
}


//TODO:NOTE: Scissors are busted. A smaller scissor doesn't check if it is inside it's own parent!
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

Color ls_uiAlphaBlend(Color source, Color dest)
{
    u8 Alpha = (u8)((source >> 24) & 0x000000FF);
    return ls_uiAlphaBlend(source, dest, Alpha);
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
    
    //TODO: Make AlphaBlending happen with SSE!!!
    
    //NOTE: Do the first Sub-Rectangle divisible by 4.
    //__m128i color = _mm_set1_epi32((int)c);
    
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
            
            __m128i val = _mm_loadu_si128(At);
            
            u32 a1 = _mm_cvtsi128_si32(_mm_shuffle_epi32(val, 0b00000000));
            u32 a2 = _mm_cvtsi128_si32(_mm_shuffle_epi32(val, 0b01010101));
            u32 a3 = _mm_cvtsi128_si32(_mm_shuffle_epi32(val, 0b10101010));
            u32 a4 = _mm_cvtsi128_si32(_mm_shuffle_epi32(val, 0b11111111));
            
            Color c1 = ls_uiAlphaBlend(c, a1);
            Color c2 = ls_uiAlphaBlend(c, a2);
            Color c3 = ls_uiAlphaBlend(c, a3);
            Color c4 = ls_uiAlphaBlend(c, a4);
            
            __m128i color = _mm_setr_epi32(c1, c2, c3, c4);
            
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
                
                Color base = At[y*cxt->width + x];
                Color blendedColor = ls_uiAlphaBlend(c, base);
                At[y*cxt->width + x] = blendedColor;
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
                
                Color base = At[y*cxt->width + x];
                Color blendedColor = ls_uiAlphaBlend(c, base);
                At[y*cxt->width + x] = blendedColor;
            }
        }
    }
}

inline
void ls_uiBorder(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h)
{
    Color C = cxt->borderColor;
    
    ls_uiFillRect(cxt, xPos,     yPos,     w, 1, C);
    ls_uiFillRect(cxt, xPos,     yPos+h-1, w, 1, C);
    ls_uiFillRect(cxt, xPos,     yPos,     1, h, C);
    ls_uiFillRect(cxt, xPos+w-1, yPos,     1, h, C);
}

inline
void ls_uiBorder(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h, Color borderColor)
{
    Color C = borderColor;
    
    ls_uiFillRect(cxt, xPos,     yPos,     w, 1, C);
    ls_uiFillRect(cxt, xPos,     yPos+h-1, w, 1, C);
    ls_uiFillRect(cxt, xPos,     yPos,     1, h, C);
    ls_uiFillRect(cxt, xPos+w-1, yPos,     1, h, C);
}

inline
void ls_uiBorderedRect(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h)
{
    ls_uiBorder(cxt, xPos, yPos, w, h);
    ls_uiFillRect(cxt, xPos+1, yPos+1, w-2, h-2, cxt->widgetColor);
}

inline
void ls_uiBorderedRect(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h, Color widgetColor)
{
    ls_uiBorder(cxt, xPos, yPos, w, h);
    ls_uiFillRect(cxt, xPos+1, yPos+1, w-2, h-2, widgetColor);
}

inline
void ls_uiBorderedRect(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h, Color widgetColor, Color borderColor)
{
    ls_uiBorder(cxt, xPos, yPos, w, h, borderColor);
    ls_uiFillRect(cxt, xPos+1, yPos+1, w-2, h-2, widgetColor);
}

inline
void ls_uiRect(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h)
{
    ls_uiFillRect(cxt, xPos, yPos, w, h, cxt->widgetColor);
}

inline
void ls_uiRect(UIContext *cxt, s32 xPos, s32 yPos, s32 w, s32 h, Color widgetColor)
{
    ls_uiFillRect(cxt, xPos, yPos, w, h, widgetColor);
}

void ls_uiCircle(UIContext *cxt, s32 xPos, s32 yPos, s32 selRadius)
{
    s32 radSq = selRadius*selRadius;
    
    auto getBest = [radSq](s32 x, s32 y, s32 *bX, s32 *bY)
    {
        s32 bestX = 0, bestY = 0, bestSum = 0;
        
        s32 rx = x+1;
        s32 ry = y;
        s32 sum = (rx*rx) + (ry*ry);
        if(sum <= radSq) { bestX = rx; bestY = ry; bestSum = sum; }
        
        rx = x;
        ry = y-1;
        sum = (rx*rx) + (ry*ry);
        if((sum <= radSq) && sum > bestSum) { bestX = rx; bestY = ry; bestSum = sum; }
        
        rx = x+1;
        ry = y-1;
        sum = (rx*rx) + (ry*ry);
        if((sum <= radSq) && sum > bestSum) { bestX = rx; bestY = ry; bestSum = sum; }
        
        *bX = bestX;
        *bY = bestY;
    };
    
    f64 sin = 0, cos = 0;
    
    //NOTE: Everything is done relative to (0,0) until when drawing happens
    //NOTE: We start at the top middle pixel of the Circle
    s32 startX = 0;
    s32 startY = selRadius;
    
    //NOTE: And end at the first octant swept going clockwise.
    ls_sincos(PI/4.0, &sin, &cos);
    s32 endX = (cos*selRadius);
    s32 endY = (sin*selRadius);
    
    s32 currX = startX;
    s32 currY = startY;
    
    UIScissor::UIRect *scRect = cxt->scissor.currRect;
    u32 *At = (u32 *)cxt->drawBuffer;
    
    Color bCol   = cxt->borderColor;
    
    //TODO: Should be SIMDable
    b32 Running = TRUE;
    while(Running)
    {
        if((currX == endX) || (currY == endY)) { Running = FALSE; }
        
        s32 drawX1 = xPos + currX; s32 drawY1 = yPos + currY;
        s32 drawX2 = xPos - currX; s32 drawY2 = yPos + currY;
        s32 drawX3 = xPos + currY; s32 drawY3 = yPos + currX;
        s32 drawX4 = xPos - currY; s32 drawY4 = yPos + currX;
        s32 drawX5 = xPos + currX; s32 drawY5 = yPos - currY;
        s32 drawX6 = xPos - currX; s32 drawY6 = yPos - currY;
        s32 drawX7 = xPos + currY; s32 drawY7 = yPos - currX;
        s32 drawX8 = xPos - currY; s32 drawY8 = yPos - currX;
        
        
        s32 nextX = 0, nextY = 0;
        
        getBest(currX, currY, &nextX, &nextY);
        
        currX = nextX;
        currY = nextY;
        
#define CIRCLE_DRAW_BORD(xP, yP, r, c) \
if((xP) >= r->x && (xP) < r->x+r->w && (yP) >= r->y && (yP) < r->y+r->h) \
{ At[(yP)*cxt->width + (xP)] = c; }
        
        CIRCLE_DRAW_BORD(drawX1, drawY1,   scRect, bCol);
        //CIRCLE_DRAW_BORD(drawX1, drawY1-1, scRect, bCol);
        CIRCLE_DRAW_BORD(drawX2, drawY2,   scRect, bCol);
        //CIRCLE_DRAW_BORD(drawX2, drawY2-1, scRect, bCol);
        
        //ls_uiFillRect(cxt, drawX2+1, drawY2-1, (drawX1-drawX2)-1, 1, cxt->widgetColor);
        ls_uiFillRect(cxt, drawX2+1, drawY2, (drawX1-drawX2)-1, 1, cxt->widgetColor);
        
        
        CIRCLE_DRAW_BORD(drawX3,   drawY3, scRect, bCol);
        //CIRCLE_DRAW_BORD(drawX3-1, drawY3, scRect, bCol);
        CIRCLE_DRAW_BORD(drawX4,   drawY4, scRect, bCol);
        //CIRCLE_DRAW_BORD(drawX4+1, drawY4, scRect, bCol);
        
        //ls_uiFillRect(cxt, drawX4+2, drawY4, (drawX3-drawX4)-3, 1, cxt->widgetColor);
        ls_uiFillRect(cxt, drawX4+1, drawY4, (drawX3-drawX4)-1, 1, cxt->widgetColor);
        
        CIRCLE_DRAW_BORD(drawX5, drawY5,   scRect, bCol);
        //CIRCLE_DRAW_BORD(drawX5, drawY5+1, scRect, bCol);
        CIRCLE_DRAW_BORD(drawX6, drawY6,   scRect, bCol);
        //CIRCLE_DRAW_BORD(drawX6, drawY6+1, scRect, bCol);
        
        //ls_uiFillRect(cxt, drawX6+1, drawY6+1, (drawX5-drawX6)-1, 1, cxt->widgetColor);
        ls_uiFillRect(cxt, drawX6+1, drawY6, (drawX5-drawX6)-1, 1, cxt->widgetColor);
        
        CIRCLE_DRAW_BORD(drawX7,   drawY7, scRect, bCol);
        //CIRCLE_DRAW_BORD(drawX7-1, drawY7, scRect, bCol);
        CIRCLE_DRAW_BORD(drawX8,   drawY8, scRect, bCol);
        //CIRCLE_DRAW_BORD(drawX8+1, drawY8, scRect, bCol);
        
        //ls_uiFillRect(cxt, drawX8+2, drawY8, (drawX7-drawX8)-3, 1, cxt->widgetColor);
        ls_uiFillRect(cxt, drawX8+1, drawY8, (drawX7-drawX8)-1, 1, cxt->widgetColor);
    }
#undef CIRCLE_DRAW_BORD
}


void ls_uiCircleFloat(UIContext *cxt, s32 xPos, s32 yPos, s32 selRadius)
{
    f64 radSq = selRadius*selRadius;
    f64 sin = 0, cos = 0;
    
    //NOTE: Everything is done relative to (0,0) until when drawing happens
    //NOTE: We start at the top middle pixel of the Circle
    f64 startX = 0.0;
    f64 startY = selRadius;
    
    //NOTE: And end at the first octant swept going clockwise.
    
    f64 currX = startX;
    f64 currY = startY;
    
    UIScissor::UIRect *scRect = cxt->scissor.currRect;
    u32 *At = (u32 *)cxt->drawBuffer;
    
    for(f64 Angle = 0.0; Angle < TAU; Angle += TAU/360.0)
    {
        ls_sincos(Angle, &sin, &cos);
        
        startX = cos*selRadius;
        startY = sin*selRadius;
        
        s32 x1  = ls_floor(startX);
        f64 floatXA1 = 1.0 - ls_fabs(startX - x1);
        
        s32 x2 = ls_ceil(startX);
        f64 floatXA2 = 1.0 - ls_fabs(startX - x2);
        
        s32 y1 = ls_floor(startY);
        f64 floatYA1 = 1.0 - ls_fabs(startY - y1);
        
        s32 y2 = ls_ceil(startY);
        f64 floatYA2 = 1.0 - ls_fabs(startY - y2);
        
        s32 alpha1 = (s32)((floatXA1*floatYA1)*255);
        s32 alpha2 = (s32)((floatXA2*floatYA2)*255);
        
        s32 alpha3 = (s32)((floatXA1*floatYA2)*255);
        s32 alpha4 = (s32)((floatXA2*floatYA1)*255);
        
        Color c1 = ls_uiAlphaBlend(cxt->borderColor, cxt->backgroundColor, alpha1);
        Color c2 = ls_uiAlphaBlend(cxt->borderColor, cxt->backgroundColor, alpha2);
        
        Color c3 = ls_uiAlphaBlend(cxt->borderColor, cxt->backgroundColor, alpha3);
        Color c4 = ls_uiAlphaBlend(cxt->borderColor, cxt->backgroundColor, alpha4);
        
#define CIRCLE_DRAW_BORD(xP, yP, r, c) \
if((xP) >= r->x && (xP) < r->x+r->w && (yP) >= r->y && (yP) < r->y+r->h) \
{ At[(yP)*cxt->width + (xP)] = c; }
        
        CIRCLE_DRAW_BORD(xPos+x1, yPos+y1, scRect, c1);
        CIRCLE_DRAW_BORD(xPos+x2, yPos+y2, scRect, c2);
        
        CIRCLE_DRAW_BORD(xPos+x1, yPos+y2, scRect, c3);
        CIRCLE_DRAW_BORD(xPos+x2, yPos+y1, scRect, c4);
        
#undef CIRCLE_DRAW_BORD
    }
    
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
    UIScissor::UIRect *scRect = cxt->scissor.currRect;
    
    u32 *At = (u32 *)cxt->drawBuffer;
    
    for(s32 y = yPos, eY = 0; y < yPos+h; y++, eY++)
    {
        for(s32 x = xPos, eX = 0; x < xPos+w; x++, eX++)
        {
            if(x < 0 || x >= cxt->width)  continue;
            if(y < 0 || y >= cxt->height) continue;
            
            if(x < scRect->x || x >= scRect->x+scRect->w) continue;
            if(y < scRect->y || y >= scRect->y+scRect->h) continue;
            
            At[y*cxt->width + x] = data[eY*w + eX];
        }
    }
}

void ls_uiGlyph(UIContext *cxt, s32 xPos, s32 yPos, UIGlyph *glyph, Color textColor)
{
    UIScissor::UIRect *scRect = cxt->scissor.currRect;
    
    u32 *At = (u32 *)cxt->drawBuffer;
    
    for(s32 y = yPos-glyph->y1, eY = glyph->height-1; eY >= 0; y++, eY--)
    {
        for(s32 x = xPos+glyph->x0, eX = 0; eX < glyph->width; x++, eX++)
        {
            if(x < 0 || x >= cxt->width)  continue;
            if(y < 0 || y >= cxt->height) continue;
            
            if(x < scRect->x || x >= scRect->x+scRect->w) continue;
            if(y < scRect->y || y >= scRect->y+scRect->h) continue;
            
            Color base = At[y*cxt->width + x];
            
            u8 sourceA = GetAlpha(textColor);
            u8 dstA = glyph->data[eY*glyph->width + eX];
            
            f64 sA = (f64)sourceA / 255.0;
            f64 dA = (f64)dstA / 255.0;
            
            u8 Alpha = (sA * dA) * 255;
            
            Color blendedColor = ls_uiAlphaBlend(textColor, base, Alpha);
            At[y*cxt->width + x] = blendedColor;
        }
    }
}

s32 ls_uiGetKernAdvance(UIContext *cxt, s32 codepoint1, s32 codepoint2)
{
    UIFont *font = cxt->currFont;
    s32 kernAdvance = font->kernAdvanceTable[codepoint1][codepoint2];
    
    return kernAdvance;
}

//TODO: Use font max descent to determine yOffsets globally
void ls_uiGlyphString(UIContext *cxt, s32 xPos, s32 yPos, unistring text, Color textColor)
{
    s32 currXPos = xPos;
    s32 currYPos = yPos;
    for(u32 i = 0; i < text.len; i++)
    {
        u32 indexInGlyphArray = text.data[i];
        AssertMsg(indexInGlyphArray <= cxt->currFont->maxCodepoint, "GlyphIndex OutOfBounds\n");
        
        UIGlyph *currGlyph = &cxt->currFont->glyph[indexInGlyphArray];
        ls_uiGlyph(cxt, currXPos, yPos, currGlyph, textColor);
        
        s32 kernAdvance = 0;
        if(i < text.len-1) { kernAdvance = ls_uiGetKernAdvance(cxt, text.data[i], text.data[i+1]); }
        
        currXPos += (currGlyph->xAdv + kernAdvance);
    }
}

s32 ls_uiGlyphStringLen(UIContext *cxt, unistring text)
{
    s32 totalLen = 0;
    for(u32 i = 0; i < text.len; i++)
    {
        u32 indexInGlyphArray = text.data[i];
        AssertMsg(indexInGlyphArray <= cxt->currFont->maxCodepoint, "GlyphIndex OutOfBounds\n");
        
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
    { if(cxt->fonts[i].pixelHeight == pixelHeight) { found = TRUE; cxt->currFont = &cxt->fonts[i]; } }
    
    AssertMsg(found, "Asked pixelHeight not available\n");
}

inline
s32 ls_uiSelectFontByFontSize(UIContext *cxt, UIFontSize fontSize)
{ cxt->currFont = &cxt->fonts[fontSize]; return cxt->currFont->pixelHeight; }

//TODO:Button autosizing width
void ls_uiButton(UIContext *cxt, UIButton *button, s32 xPos, s32 yPos, s32 w, s32 h)
{
    Color bkgColor = cxt->widgetColor;
    if(button->style == UIBUTTON_TEXT_NOBORDER) { bkgColor = cxt->backgroundColor; } //TODO:Hack
    
    if(MouseInRect(xPos, yPos, w, h))
    { 
        button->isHot = TRUE;
        bkgColor = cxt->highliteColor;
        
        if(button->onClick && LeftClick)
        {
            button->onClick(cxt, button->data);
        }
        if(LeftHold)
        {
            button->isHeld = TRUE;
            bkgColor = cxt->pressedColor;
            if(button->onHold) { button->onHold(cxt, button->data); }
        }
    }
    
    ls_uiSelectFontByFontSize(cxt, FS_SMALL);
    
    if(button->style == UIBUTTON_TEXT)
    {
        ls_uiBorderedRect(cxt, xPos, yPos, w, h, bkgColor);
        
        ls_uiPushScissor(cxt, xPos+2, yPos+2, w-4, h-4);
        
        s32 strWidth = ls_uiGlyphStringLen(cxt, button->name);
        s32 xOff      = (w - strWidth) / 2; //TODO: What happens when the string is too long?
        s32 strHeight = cxt->currFont->pixelHeight;
        s32 yOff      = strHeight*0.25; //TODO: @FontDescent
        
        ls_uiGlyphString(cxt, xPos+xOff, yPos+yOff, button->name, cxt->textColor);
        
        ls_uiPopScissor(cxt);
    }
    else if(button->style == UIBUTTON_TEXT_NOBORDER)
    {
        ls_uiRect(cxt, xPos, yPos, w, h, bkgColor);
        
        ls_uiPushScissor(cxt, xPos, yPos, w, h);
        
        s32 strWidth = ls_uiGlyphStringLen(cxt, button->name);
        s32 xOff      = (w - strWidth) / 2; //TODO: What happens when the string is too long?
        s32 strHeight = cxt->currFont->pixelHeight;
        s32 yOff      = strHeight*0.25; //TODO: @FontDescent
        
        ls_uiGlyphString(cxt, xPos+xOff, yPos+yOff, button->name, cxt->textColor);
        
        ls_uiPopScissor(cxt);
    }
    else if(button->style == UIBUTTON_BMP)
    {
        ls_uiPushScissor(cxt, xPos, yPos, w, h);
        
        ls_uiBitmap(cxt, xPos, yPos, (u32 *)button->bmpData, button->bmpW, button->bmpH);
        
        ls_uiPopScissor(cxt);
    }
    else { AssertMsg(FALSE, "Unhandled button style"); }
}

void ls_uiLabel(UIContext *cxt, unistring label, s32 xPos, s32 yPos)
{
    s32 strPixelHeight = ls_uiSelectFontByFontSize(cxt, FS_SMALL);
    
    s32 strWidth = ls_uiGlyphStringLen(cxt, label);
    
    ls_uiPushScissor(cxt, xPos, yPos-4, strWidth, strPixelHeight+8);
    
    ls_uiGlyphString(cxt, xPos, yPos, label, cxt->textColor);
    
    ls_uiPopScissor(cxt);
}

void ls_uiLabel(UIContext *cxt, const char32_t *label, s32 xPos, s32 yPos)
{
    unistring lab = ls_unistrConstant(label);
    ls_uiLabel(cxt, lab, xPos, yPos);
}

void ls_uiTextBoxClear(UIContext *cxt, UITextBox *box)
{
    ls_unistrClear(&box->text);
    
    box->caretIndex     = 0;
    box->isReadonly     = FALSE;
    box->selectBeginIdx = 0;
    box->selectEndIdx   = 0;
    box->isSelecting    = FALSE;
    box->viewBeginIdx   = 0;
    box->viewEndIdx     = 0;
}

void ls_uiTextBoxSet(UIContext *cxt, UITextBox *box, unistring s)
{
    ls_unistrSet(&box->text, s);
    box->viewEndIdx = box->text.len;
}

//TODO: Text Alignment
//TODO: A artificially made UITextBox string doesn't know how much to show.
//      So, viewEndIdx has to be set manually, which could be bad news.
void ls_uiTextBox(UIContext *cxt, UITextBox *box, s32 xPos, s32 yPos, s32 w, s32 h)
{
    if(LeftClick && MouseInRect(xPos, yPos, w, h) && (box->isReadonly == FALSE)) {
        cxt->currentFocus = (u64 *)box;
        cxt->focusWasSetThisFrame = TRUE;
        box->isCaretOn = TRUE; 
    }
    
    s32 strPixelHeight = ls_uiSelectFontByFontSize(cxt, FS_SMALL);
    
    ls_uiBorderedRect(cxt, xPos, yPos, w, h);
    
    Color caretColor = cxt->textColor;
    
    const s32 horzOff   = 4;
    s32 scissorWidth    = w - horzOff;
    s32 viewAddWidth    = scissorWidth - horzOff;
    ls_uiPushScissor(cxt, xPos+4, yPos, scissorWidth, h);
    
    if(cxt->currentFocus == (u64 *)box)
    {
        if(box->preInput)
        { box->preInput(cxt, box->data); }
        
        //NOTE: Draw characters. (box->maxLen == 0 means there's no max len)
        if(HasPrintableKey() && (box->text.len < box->maxLen || box->maxLen == 0)) 
        {
            if(box->caretIndex == box->text.len) { ls_unistrAppendChar(&box->text, GetPrintableKey()); }
            else { ls_unistrInsertChar(&box->text, GetPrintableKey(), box->caretIndex); }
            
            box->caretIndex += 1;
            box->viewEndIdx += 1;
            
            if(ls_uiGlyphStringLen(cxt, box->text) > viewAddWidth)
            { box->viewBeginIdx += 1; }
            
        }
        if(KeyPress(keyMap::Backspace) && box->text.len > 0 && box->caretIndex > 0) 
        {
            if(box->caretIndex == box->text.len) { ls_unistrTrimRight(&box->text, 1); }
            else { ls_unistrRmIdx(&box->text, box->caretIndex-1); }
            
            box->caretIndex -= 1;
            box->viewEndIdx -= 1;
            if(box->viewBeginIdx != 0) { box->viewBeginIdx -= 1; }
        }
        if(KeyPress(keyMap::Delete) && box->text.len > 0 && box->caretIndex < box->text.len)
        {
            if(box->caretIndex == box->text.len-1) { ls_unistrTrimRight(&box->text, 1); }
            else { ls_unistrRmIdx(&box->text, box->caretIndex); }
            
            if(box->text.len < box->viewEndIdx) { box->viewEndIdx -= 1; }
        }
        
        if(KeyPress(keyMap::LArrow) && box->caretIndex > 0)
        { 
            if(KeyHeld(keyMap::Shift))
            {
                if(!box->isSelecting) 
                { 
                    box->selectEndIdx   = box->caretIndex;
                    box->selectBeginIdx = box->caretIndex - 1;
                    box->isSelecting    = TRUE;
                }
                else
                {
                    if(box->caretIndex == box->selectBeginIdx)
                    { box->selectBeginIdx -= 1; }
                    else if(box->caretIndex == box->selectEndIdx)
                    { box->selectEndIdx   -= 1;}
                    else
                    { AssertMsg(FALSE, "LArrow -> Caret is not aligned with select anymore\n"); }
                    
                    if(box->selectBeginIdx == box->selectEndIdx) 
                    { box->isSelecting = FALSE; }
                }
            }
            else 
            { if(box->isSelecting) { box->isSelecting = FALSE; box->selectEndIdx = 0; box->selectBeginIdx = 0; } }
            
            box->isCaretOn = TRUE; box->dtCaret = 0; 
            box->caretIndex -= 1;
            if(box->caretIndex < box->viewBeginIdx) { box->viewBeginIdx -= 1; box->viewEndIdx -= 1; }
            
        }
        if(KeyPress(keyMap::RArrow) && box->caretIndex < box->text.len)
        { 
            if(KeyHeld(keyMap::Shift))
            {
                if(!box->isSelecting) 
                { 
                    box->selectEndIdx   = box->caretIndex + 1;
                    box->selectBeginIdx = box->caretIndex;
                    box->isSelecting    = TRUE;
                }
                else
                {
                    if(box->caretIndex == box->selectBeginIdx)
                    { box->selectBeginIdx += 1; }
                    else if(box->caretIndex == box->selectEndIdx)
                    { box->selectEndIdx   += 1;}
                    else
                    { AssertMsg(FALSE, "RArrow -> Caret is not aligned with select anymore\n"); }
                    
                    if(box->selectBeginIdx == box->selectEndIdx) 
                    { box->isSelecting = FALSE; }
                }
            }
            else 
            { if(box->isSelecting) { box->isSelecting = FALSE; box->selectEndIdx = 0; box->selectBeginIdx = 0; } }
            
            box->isCaretOn = TRUE; box->dtCaret = 0; 
            box->caretIndex += 1; 
            if(box->caretIndex > box->viewEndIdx) { box->viewBeginIdx += 1; box->viewEndIdx += 1; }
            
        }
        if(KeyPress(keyMap::Home)) 
        { 
            if(KeyHeld(keyMap::Shift))
            {
                if(!box->isSelecting) 
                { 
                    box->selectEndIdx   = box->caretIndex;
                    box->selectBeginIdx = 0;
                    box->isSelecting    = TRUE;
                }
                else
                {
                    if(box->caretIndex == box->selectBeginIdx)
                    { box->selectBeginIdx = 0; }
                    else if(box->caretIndex == box->selectEndIdx)
                    { box->selectEndIdx   = box->selectBeginIdx; box->selectBeginIdx = 0; }
                    else
                    { AssertMsg(FALSE, "Home -> Caret is not aligned with select anymore\n"); }
                    
                    if(box->selectBeginIdx == box->selectEndIdx) 
                    { box->isSelecting = FALSE; }
                }
            }
            else 
            { if(box->isSelecting) { box->isSelecting = FALSE; box->selectEndIdx = 0; box->selectBeginIdx = 0; } }
            
            box->isCaretOn = TRUE; box->dtCaret = 0; 
            box->caretIndex = 0;
            
            s32 vLen = box->viewEndIdx - box->viewBeginIdx;
            box->viewBeginIdx = 0;
            box->viewEndIdx = vLen;
        }
        if(KeyPress(keyMap::End)) 
        {
            if(KeyHeld(keyMap::Shift))
            {
                if(!box->isSelecting) 
                { 
                    box->selectEndIdx   = box->text.len;
                    box->selectBeginIdx = box->caretIndex;
                    box->isSelecting    = TRUE;
                }
                else
                {
                    if(box->caretIndex == box->selectBeginIdx)
                    { box->selectBeginIdx = box->selectEndIdx; box->selectEndIdx = box->text.len; }
                    else if(box->caretIndex == box->selectEndIdx)
                    { box->selectEndIdx   = box->text.len; }
                    else
                    { AssertMsg(FALSE, "End -> Caret is not aligned with select anymore\n"); }
                    
                    if(box->selectBeginIdx == box->selectEndIdx) 
                    { box->isSelecting = FALSE; }
                }
            }
            else 
            { if(box->isSelecting) { box->isSelecting = FALSE; box->selectEndIdx = 0; box->selectBeginIdx = 0; } }
            
            box->isCaretOn = TRUE; box->dtCaret = 0; 
            box->caretIndex = box->text.len;
            
            s32 vLen = box->viewEndIdx - box->viewBeginIdx;
            box->viewEndIdx = box->caretIndex;
            box->viewBeginIdx = box->viewEndIdx - vLen;
        }
        
        if(KeyHeld(keyMap::Control) && KeyPress(keyMap::V))
        {
            u32 buff[128] = {};
            u32 copiedLen = GetClipboard(buff, 128);
            
            u32 realCopyLen = copiedLen;
            
            //NOTE: maxLen == 0 means there's no max len.
            if(box->maxLen != 0) 
            {
                if(box->text.len + copiedLen > box->maxLen) {
                    if(box->text.len < box->maxLen) { realCopyLen = box->maxLen - box->text.len; }
                    else { realCopyLen = 0; }
                }
            }
            
            if(box->caretIndex == box->text.len) { ls_unistrAppendBuffer(&box->text, buff, realCopyLen); }
            else { ls_unistrInsertBuffer(&box->text, buff, realCopyLen, box->caretIndex); }
            
            box->caretIndex += copiedLen;
        }
        
        if(KeyHeld(keyMap::Control) && KeyPress(keyMap::C))
        { 
            SetClipboard(box->text.data, box->text.len); 
        }
        
        //NOTETODO: Duplicated Values
        s32 vertOff = ((h - strPixelHeight) / 2) + 5;
        u32 viewLen = box->viewEndIdx - box->viewBeginIdx;
        u32 actualViewLen = viewLen <= box->text.len ? viewLen : box->text.len;
        unistring viewString = {box->text.data + box->viewBeginIdx, actualViewLen, actualViewLen};
        
        //NOTE: Draw the Caret
        box->dtCaret += cxt->dt;
        if(box->dtCaret >= 400) { box->dtCaret = 0; box->isCaretOn = !box->isCaretOn; }
        
        if(box->isCaretOn)
        {
            UIGlyph *caretGlyph = &cxt->currFont->glyph['|'];
            
            u32 caretIndexInView = box->caretIndex - box->viewBeginIdx;
            unistring tmp = {viewString.data, caretIndexInView, caretIndexInView};
            
            u32 stringLen = ls_uiGlyphStringLen(cxt, tmp);
            
            const s32 randffset = 4; //TODO: Maybe try to remove this?
            ls_uiGlyph(cxt, xPos + horzOff + stringLen - randffset, yPos+vertOff, caretGlyph, caretColor);
        }
        
        
        if(box->postInput) 
        { box->postInput(cxt, box->data); }
    }
    
    //NOTETODO: Duplicated Values
    s32 vertOff = ((h - strPixelHeight) / 2) + 5; //TODO: @FontDescent
    u32 viewLen = box->viewEndIdx - box->viewBeginIdx;
    u32 actualViewLen = viewLen <= box->text.len ? viewLen : box->text.len;
    unistring viewString = {box->text.data + box->viewBeginIdx, actualViewLen, actualViewLen};
    
    ls_uiGlyphString(cxt, xPos + horzOff, yPos + vertOff, viewString, cxt->textColor);
    
    if(box->isSelecting)
    {
        //TODO: Draw this more efficiently by drawing text in 3 different non-overlapping calls??
        s32 viewSelBegin = box->selectBeginIdx;
        if(viewSelBegin <= box->viewBeginIdx) { viewSelBegin = box->viewBeginIdx; }
        
        s32 viewSelEnd = box->selectEndIdx;
        if(box->viewEndIdx != 0 && viewSelEnd >= box->viewEndIdx) { viewSelEnd = box->viewEndIdx; }
        
        u32 selLen = viewSelEnd -  viewSelBegin;
        unistring selString = {box->text.data + viewSelBegin, selLen, selLen};
        s32 selStringWidth  = ls_uiGlyphStringLen(cxt, selString);
        
        u32 diffLen = 0;
        if(box->selectBeginIdx > box->viewBeginIdx) { diffLen = box->selectBeginIdx - box->viewBeginIdx; }
        
        unistring diffString = { box->text.data + box->viewBeginIdx, diffLen, diffLen };
        s32 diffStringWidth = ls_uiGlyphStringLen(cxt, diffString);
        
        ls_uiFillRect(cxt, xPos + horzOff + diffStringWidth, yPos+1, selStringWidth, h-2, cxt->invWidgetColor);
        ls_uiGlyphString(cxt, xPos + horzOff + diffStringWidth, yPos + vertOff, selString, cxt->invTextColor);
        
        if(box->caretIndex == box->selectBeginIdx)
        { caretColor = cxt->invTextColor; }
    }
    
    
    ls_uiPopScissor(cxt);
}


enum UIArrowSide { UIA_LEFT, UIA_RIGHT, UIA_UP, UIA_DOWN };
void ls_uiDrawArrow(UIContext *cxt, s32 x, s32 yPos, s32 w, s32 h, UIArrowSide s)
{
    UIScissor::UIRect *scRect = cxt->scissor.currRect;
    u32 *At = (u32 *)cxt->drawBuffer;
    
    s32 xPos = x-1;
    
    Color bkgColor = cxt->widgetColor;
    if(MouseInRect(xPos, yPos, w, h)) { bkgColor = cxt->highliteColor; }
    
    ls_uiBorderedRect(cxt, xPos, yPos, w, h, bkgColor);
    
    if(s == UIA_DOWN)
    {
        s32 arrowWidth = 8;
        s32 hBearing = (w - arrowWidth)/2;
        s32 xBase = xPos+hBearing;
        s32 xEnd  = xBase + arrowWidth;
        
        s32 arrowHeight = 4;
        s32 vBearing = (h - arrowHeight)/2;
        s32 yStart = (yPos + h - vBearing) - 1;
        s32 yEnd = yStart - arrowHeight;
        
        for(s32 y = yStart; y >= yEnd; y--)
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
    else if(s == UIA_RIGHT)
    {
        s32 arrowWidth  = 4;
        s32 hBearing    = (w - arrowWidth)/2;
        s32 xBase       = xPos + hBearing;
        s32 xEnd        = xBase+1;
        
        s32 arrowHeight = 8;
        s32 vBearing    = (h - arrowHeight)/2;
        s32 yStart1     = (yPos + h - vBearing) - 1;
        s32 yEnd1       = yStart1 - (arrowHeight/2);
        s32 yStart2     = yEnd1;
        s32 yEnd2       = yStart2 - arrowHeight;
        
        for(s32 y = yStart1; y >= yEnd1; y--)
        {
            for(s32 x = xBase; x < xEnd; x++)
            {
                if(x < 0 || x >= cxt->width)  continue;
                if(y < 0 || y >= cxt->height) continue;
                
                if(x < scRect->x || x >= scRect->x+scRect->w) continue;
                if(y < scRect->y || y >= scRect->y+scRect->h) continue;
                
                At[y*cxt->width + x] = RGBg(0x00);
            }
            
            xEnd  += 1;
        }
        
        xEnd -= 1;
        for(s32 y = yStart2; y >= yEnd2; y--)
        {
            for(s32 x = xBase; x < xEnd; x++)
            {
                if(x < 0 || x >= cxt->width)  continue;
                if(y < 0 || y >= cxt->height) continue;
                
                if(x < scRect->x || x >= scRect->x+scRect->w) continue;
                if(y < scRect->y || y >= scRect->y+scRect->h) continue;
                
                At[y*cxt->width + x] = RGBg(0x00);
            }
            
            xEnd  -= 1;
        }
    }
    else if(s == UIA_LEFT)
    {
        s32 arrowWidth  = 4;
        s32 hBearing    = (w - arrowWidth)/2;
        s32 xBase       = xPos + w - hBearing - 1;
        s32 xEnd        = xBase+1;
        
        s32 arrowHeight = 8;
        s32 vBearing    = (h - arrowHeight)/2;
        s32 yStart1     = (yPos + h - vBearing) - 1;
        s32 yEnd1       = yStart1 - (arrowHeight/2);
        s32 yStart2     = yEnd1;
        s32 yEnd2       = yStart2 - arrowHeight;
        
        for(s32 y = yStart1; y >= yEnd1; y--)
        {
            for(s32 x = xBase; x < xEnd; x++)
            {
                if(x < 0 || x >= cxt->width)  continue;
                if(y < 0 || y >= cxt->height) continue;
                
                if(x < scRect->x || x >= scRect->x+scRect->w) continue;
                if(y < scRect->y || y >= scRect->y+scRect->h) continue;
                
                At[y*cxt->width + x] = RGBg(0x00);
            }
            
            xBase -= 1;
        }
        
        xBase += 1;
        for(s32 y = yStart2; y >= yEnd2; y--)
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
        }
    }
}


void ls_uiLPane(UIContext *cxt, UILPane *pane, s32 xPos, s32 yPos, s32 w, s32 h)
{
    if(pane->isOpen)
    {
        s32 xArrowPos  = xPos + w - 1;
        s32 arrowWidth = 16;
        
        s32 arrowHeight = 14;
        s32 yArrowPos   = yPos + h - arrowHeight;
        
        ls_uiDrawArrow(cxt, xArrowPos, yArrowPos, arrowWidth, arrowHeight, UIA_LEFT);
        
        if(LeftClickIn(xArrowPos, yArrowPos, arrowWidth, arrowHeight))
        { pane->isOpen = FALSE; }
        
        ls_uiBorderedRect(cxt, xPos-1, yPos, w, h, cxt->widgetColor);
    }
    else
    {
        s32 width = 0;
        if(pane->isOpening)
        {
            pane->dtOpen += cxt->dt;
            
            s32 maxDT = 120;
            f64 dtFract = (f64)pane->dtOpen / maxDT;
            width = w*dtFract;
            
            if(pane->dtOpen >= maxDT) { 
                pane->isOpen = TRUE; pane->isOpening = FALSE; pane->dtOpen = 0;
                ls_uiBorderedRect(cxt, xPos-1, yPos, w, h, cxt->widgetColor);
            }
            
            if(!pane->isOpen) { ls_uiBorderedRect(cxt, xPos-1, yPos, width, h, cxt->widgetColor); }
        }
        
        s32 xArrowPos  = xPos + width;
        s32 arrowWidth = 16;
        
        s32 arrowHeight = 14;
        s32 yArrowPos   = yPos + h - arrowHeight;
        
        ls_uiDrawArrow(cxt, xArrowPos, yArrowPos, arrowWidth, arrowHeight, UIA_RIGHT);
        
        if(LeftClickIn(xArrowPos, yArrowPos, arrowWidth, arrowHeight))
        { pane->isOpening = TRUE; }
    }
}


//NOTETODO: ListBox manages the data of the entry itself, even when a unistring is already passed.
//          This feels strange, and probably error-prone.

//TODO I really think ListBox shouldn't manage memory. Fix This.
inline u32 ls_uiListBoxAddEntry(UIContext *cxt, UIListBox *list, char *s)
{ 
    unistring text = ls_unistrFromAscii(s);
    return list->list.push(text);
}

inline u32 ls_uiListBoxAddEntry(UIContext *cxt, UIListBox *list, unistring s)
{
    return list->list.push(s); 
}

inline void ls_uiListBoxRemoveEntry(UIContext *cxt, UIListBox *list, u32 index)
{ 
    //NOTETODO: Is this good????
    if(list->selectedIndex == index) { list->selectedIndex = 0; }
    
    unistring val = list->list[index];
    ls_unistrFree(&val);
    list->list.remove(index);
}

void ls_uiListBox(UIContext *cxt, UIListBox *list, s32 xPos, s32 yPos, s32 w, s32 h)
{
    s32 strHeight = ls_uiSelectFontByFontSize(cxt, FS_SMALL);
    s32 vertOff = ((h - strHeight) / 2) + 4; //TODO: @FontDescent
    
    ls_uiBorderedRect(cxt, xPos, yPos, w, h);
    
    const s32 arrowBoxWidth = 24;
    ls_uiPushScissor(cxt, xPos, yPos, w+arrowBoxWidth, h);
    
    ls_uiDrawArrow(cxt, xPos + w, yPos, arrowBoxWidth, h, UIA_DOWN);
    
    if(LeftClickIn(xPos+w, yPos, arrowBoxWidth, h))
    {
        if(list->isOpen) { list->isOpen = FALSE; }
        else { list->isOpening = TRUE; }
    }
    
    if(list->list.count)
    {
        unistring selected = list->list[list->selectedIndex];
        ls_uiGlyphString(cxt, xPos+10, yPos + vertOff, selected, cxt->textColor);
    }
    
    ls_uiPopScissor(cxt);
    
    s32 maxHeight = (list->list.count)*h;
    //TODO: Should I try adding another Scissor? Has to be added inside the branches.
    if(list->isOpening)
    {
        list->dtOpen += cxt->dt;
        
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
    
    s32 toBeChanged = 9999;
    if(list->isOpen)
    {
        Color bkgColor = cxt->widgetColor;
        
        for(u32 i = 0; i < list->list.count; i++)
        {
            s32 currY = yPos - (h*(i+1));
            unistring currStr = list->list[i];
            
            if(MouseInRect(xPos+1, currY+1, w-2, h-1)) 
            { 
                bkgColor = cxt->highliteColor;
                if(LeftHold)
                {
                    bkgColor = cxt->pressedColor;
                }
                
                if(LeftUp)
                {
                    toBeChanged = i;
                }
            }
            
            ls_uiRect(cxt, xPos+1, currY, w-2, h, bkgColor);
            ls_uiGlyphString(cxt, xPos+10, yPos + vertOff - (h*(i+1)), currStr, cxt->textColor);
            
            bkgColor = cxt->widgetColor;
        }
        
        ls_uiBorder(cxt, xPos, yPos-maxHeight, w, maxHeight+1);
    }
    
    //NOTE: We defer the selectedIndex change to the end of the frame 
    //      to avoid the for loop blinking wrongly for a frame
    if(toBeChanged != 9999) { 
        list->selectedIndex = toBeChanged; list->isOpen = FALSE;
        if(list->onSelect) { list->onSelect(cxt, list->data); }
    }
}

UISlider ls_uiSliderInit(char32_t *name, s32 maxVal, s32 minVal, f64 currPos, SliderStyle s, Color l, Color r)
{
    UISlider Result = {};
    
    if(name) { Result.text = ls_unistrFromUTF32((const char32_t *)name); }
    else     { Result.text = ls_unistrAlloc(16); }
    
    Result.maxValue = maxVal;
    Result.minValue = minVal;
    Result.currPos  = currPos;
    Result.style    = s;
    Result.lColor   = l;
    Result.rColor   = r;
    
    return Result;
}

void ls_uiSliderChangeValueBy(UIContext *cxt, UISlider *f, s32 valueDiff)
{
    s32 newValue = ls_mathClamp(f->currValue + valueDiff, f->maxValue, f->minValue);
    
    s32 range = (f->maxValue - f->minValue);
    f->currPos = (f32)(newValue - f->minValue) / (f32)range;
    
    return;
}

//TODO: The things are rendered in a logical order, but that makes the function's flow very annoying
//      going in and out of if blocks to check hot/held and style.
void ls_uiSlider(UIContext *cxt, UISlider *slider, s32 xPos, s32 yPos, s32 w, s32 h)
{
    if(LeftUp) { slider->isHeld = FALSE; }
    
    //NOTE: Box Slider Branchless Opacity Check
    u8 opacity = 0xEE - (0xB0*slider->isHeld);
    
    if(slider->style == SL_LINE) 
    {
        ls_uiBorderedRect(cxt, xPos, yPos, w, h);
        
        s32 selRadius = 50;
        
        ls_uiPushScissor(cxt, xPos-selRadius, yPos-selRadius, w+selRadius, h+(selRadius*2));
        
        ls_uiCircleFloat(cxt, xPos+400, yPos+(h/2), selRadius);
        
        ls_uiPopScissor(cxt);
    }
    else if(slider->style == SL_BOX)
    {
        s32 slideWidth = 3;
        
        ls_uiBorder(cxt, xPos, yPos, w, h);
        
        ls_uiPushScissor(cxt, xPos-4, yPos-3, w+8, h+6);
        
        slider->currValue = ((slider->maxValue - slider->minValue) * slider->currPos) + slider->minValue;
        s32 slidePos = w*slider->currPos;
        
        ls_uiFillRect(cxt, xPos+1, yPos+1, slidePos, h-2, slider->lColor);
        ls_uiFillRect(cxt, xPos+slidePos, yPos+1, w-slidePos-1, h-2, slider->rColor);
        
        unistring val = ls_unistrFromInt(slider->currValue);
        
        ls_uiPushScissor(cxt, xPos+1, yPos+1, w-2, h-2);
        
        ls_uiSelectFontByFontSize(cxt, FS_SMALL);
        u32 textLen = ls_uiGlyphStringLen(cxt, val);
        
        
        s32 strXPos = xPos + slidePos - textLen - 2;
        s32 strHeight = cxt->currFont->pixelHeight;
        Color textBkgC = slider->lColor;
        if(strXPos < xPos+1) { strXPos = xPos + slidePos + slideWidth + 2; textBkgC = slider->rColor; }
        
        Color valueColor = RGBA(0x22, 0x22, 0x22, (0x00 + (slider->isHeld*0xFF)));
        ls_uiGlyphString(cxt, strXPos, yPos + h - strHeight, val, valueColor);
        
        ls_unistrFree(&val);
        
        ls_uiPopScissor(cxt);
        
        if(MouseInRect(xPos + slidePos-5, yPos, 10, h) && !(cxt->mouseCapture != 0 && cxt->mouseCapture != (u64 *)slider))
        {
            slider->isHot = TRUE; 
            if(LeftHold) { slider->isHeld = TRUE; cxt->mouseCapture = (u64 *)slider; }
            
            
            s32 actualX = (xPos + slidePos) - 1;
            s32 actualY = yPos - 2;
            
            s32 actualWidth  = slideWidth+2;
            s32 actualHeight = 4 + h;
            
            ls_uiFillRect(cxt, actualX, actualY, actualWidth, actualHeight, cxt->borderColor);
        }
        else
        {
            ls_uiFillRect(cxt, xPos+slidePos, yPos, slideWidth, h, cxt->borderColor);
        }
        
        ls_uiPopScissor(cxt);
    }
    
    //NOTE: Draw the displayed text, and hide through Alpha the slider info.
    ls_uiPushScissor(cxt, xPos+1, yPos, w-2, h);
    
    Color rectColor = cxt->widgetColor;
    rectColor = SetAlpha(rectColor, opacity);
    ls_uiBorderedRect(cxt, xPos, yPos, w, h, rectColor);
    
    ls_uiSelectFontByFontSize(cxt, FS_SMALL);
    
    s32 strWidth  = ls_uiGlyphStringLen(cxt, slider->text);
    s32 xOff      = (w - strWidth) / 2;
    s32 strHeight = cxt->currFont->pixelHeight;
    s32 yOff      = (h - strHeight) + 3; //TODO: @FontDescent
    
    Color textColor = cxt->textColor;
    textColor = SetAlpha(textColor, opacity);
    
    ls_uiGlyphString(cxt, xPos+xOff, yPos + yOff, slider->text, textColor);
    
    ls_uiPopScissor(cxt);
    
    
    if(slider->isHeld) { 
        s32 deltaX = (UserInput.Mouse.prevPos.x - UserInput.Mouse.currPos.x);//*cxt->dt;
        
        f64 fractionMove = (f64)deltaX / (f64)w;
        
        slider->currPos -= fractionMove;
        
        slider->currPos = ls_mathClamp(slider->currPos, 1.0, 0.0);
    }
    
    slider->isHot = FALSE;
}

struct __UImenuDataPass { UIMenu *menu; s32 idx; };

void ls_uiMenuDefaultOnClick(UIContext *cxt, void *data)
{
    __UImenuDataPass *pass = (__UImenuDataPass *)data;
    
    UIMenu *menu  = pass->menu;
    
    menu->isOpen  = TRUE;
    menu->openIdx = pass->idx;
    
    return;
}

void ls_uiMenuAddItem(UIContext *cxt, UIMenu *menu, UIButton b)
{
    if(b.onClick == ls_uiMenuDefaultOnClick)
    {
        //TODO: I hate having to allocate data for this. I also need a way to deallocate it!
        //      @Leak @Memory
        __UImenuDataPass *pass = (__UImenuDataPass *)ls_alloc(sizeof(__UImenuDataPass));
        pass->menu = menu;
        pass->idx = menu->items.count;
        b.data = pass;
    }
    
    menu->items.push(b);
}

//TODO This is completely unnecessary
void ls_uiMenuAddSub(UIContext *cxt, UIMenu *menu, UIMenu sub, s32 idx)
{ menu->sub[idx] = sub; }

void ls_uiMenu(UIContext *cxt, UIMenu *menu, s32 x, s32 y, s32 w, s32 h)
{
    if(LeftClickIn(x, y, w, h)) {
        cxt->currentFocus = (u64 *)menu;
        cxt->focusWasSetThisFrame = TRUE;
    }
    
    if(cxt->currentFocus != (u64 *)menu) { menu->isOpen = FALSE; }
    
    ls_uiPushScissor(cxt, x, y, w, h);
    
    ls_uiBorderedRect(cxt, x, y, w, h, cxt->backgroundColor, RGBg(110));
    
    ls_uiButton(cxt, &menu->closeWindow, x+w-20, y+2, 16, 16);
    
    s32 xOff = 100;
    for(u32 i = 0; i < menu->items.count; i++)
    {
        UIButton *currItem = &menu->items[i];
        ls_uiButton(cxt, currItem, x+xOff, y+2, 100, 20);
        xOff += 100;
    }
    
    ls_uiPopScissor(cxt);
    
    if(menu->isOpen == TRUE)
    {
        UIButton *openItem = &menu->items[menu->openIdx];
        UIMenu *openSub = menu->sub + menu->openIdx;
        
        AssertMsg(openItem, "The item doesn't exist\n");
        AssertMsg(openSub, "The sub-menu doesn't exist\n");
        
        s32 yPos   = y-20;
        s32 height = openSub->items.count*20;
        
        ls_uiBorderedRect(cxt, 99, y-height-2, 102, height+3, cxt->backgroundColor);
        
        //TODO: Store the xPos of every menu item.
        for(u32 i = 0; i < openSub->items.count; i++)
        {
            UIButton *currItem = &openSub->items[i];
            ls_uiButton(cxt, currItem, 100, yPos, 100, 20);
            yPos -= 21;
        }
    }
}

void ls_uiRender(UIContext *c)
{
    c->callbackRender();
    return;
}
