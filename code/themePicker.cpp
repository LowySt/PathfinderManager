#if _DEBUG
void dumpCurrentTheme(UIContext *c)
{
    ls_log("backgroundColor = RGB({u8}, {u8}, {u8})",
           c->backgroundColor.r, c->backgroundColor.g, c->backgroundColor.b);
    
    ls_log("menuBarColor = RGB({u8}, {u8}, {u8})",
           c->menuBarColor.r, c->menuBarColor.g, c->menuBarColor.b);
    
    ls_log("highliteColor = RGB({u8}, {u8}, {u8})",
           c->highliteColor.r, c->highliteColor.g, c->highliteColor.b);
    
    ls_log("pressedColor = RGB({u8}, {u8}, {u8})",
           c->pressedColor.r, c->pressedColor.g, c->pressedColor.b);
    
    ls_log("widgetColor = RGB({u8}, {u8}, {u8})",
           c->widgetColor.r, c->widgetColor.g, c->widgetColor.b);
    
    ls_log("borderColor = RGB({u8}, {u8}, {u8})",
           c->borderColor.r, c->borderColor.g, c->borderColor.b);
    
    ls_log("textColor = RGB({u8}, {u8}, {u8})",
           c->textColor.r, c->textColor.g, c->textColor.b);
    
    ls_log("invWidgetColor = RGB({u8}, {u8}, {u8})",
           c->invWidgetColor.r, c->invWidgetColor.g, c->invWidgetColor.b);
    
    ls_log("invTextColor = RGB({u8}, {u8}, {u8})",
           c->invTextColor.r, c->invTextColor.g, c->invTextColor.b);
}
#endif

enum ProgramTheme
{
    THEME_DEFAULT,
    THEME_DARKNIGHT,
    THEME_LIGHT,
    THEME_GREEN,
    THEME_USER,
    
    THEME_COUNT
};

static ProgramTheme currentTheme = THEME_DEFAULT;


b32 themeApplyToBackground(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->backgroundColor    = picker->pickedColor;
    State.backgroundColor = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToMenu(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->menuBarColor       = picker->pickedColor;
    State.menuBarColor    = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToBorder(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->borderColor        = picker->pickedColor;
    State.borderColor     = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToHighlite(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->highliteColor      = picker->pickedColor;
    State.highliteColor   = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToPressed(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->pressedColor       = picker->pickedColor;
    State.pressedColor    = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToWidget(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->widgetColor        = picker->pickedColor;
    State.widgetColor     = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToText(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->textColor          = picker->pickedColor;
    State.textColor       = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToInvertedText(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->invTextColor       = picker->pickedColor;
    State.invTextColor    = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToInvertedWidget(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->invWidgetColor     = picker->pickedColor;
    State.invWidgetColor  = picker->pickedColor;
    return TRUE;
}

b32 DrawThemePicker(UIContext *c)
{
    Input *UserInput = &c->UserInput;
    b32 usedInput = FALSE;
    
    ThemePicker *theme = &State.themePicker;
    
    if(theme->isShown == TRUE)
    {
        
#if _DEBUG
        if(KeyPress(keyMap::F8)) { dumpCurrentTheme(c); }
#endif
        
        s32 pickerX = c->width/2;
        s32 pickerY = c->height/3;
        s32 pickerW = 300;
        s32 pickerH = 320;
        s32 pickerRadius = 100;
        
        usedInput |= ls_uiColorPicker(c, &theme->wheel, pickerX, pickerY, 300, 320, 2);
        
        s32 previewW = pickerW;
        s32 previewX = pickerX - previewW;
        s32 previewY = pickerY;
        s32 previewH = pickerH;
        
        s32 topY    = previewY + previewH;
        s32 xMargin = 0.1f*previewW;
        s32 yMargin = 0.1f*previewH;
        
        UIRect rect = {};
        s32 rectH = 0.2f*previewH;
        
        //NOTE: Background example rect
        rect = { previewX + xMargin, topY - yMargin - rectH, (s32)(0.2f*previewW), rectH };
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h))
        { theme->selected = 0; theme->wheel.apply.callback1 = themeApplyToBackground; }
        
        if(theme->selected == 0) 
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, theme->wheel.pickedColor, RGBg(0xFF), 2); }
        else
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, c->backgroundColor, c->backgroundColor, 2); }
        
        //NOTE: Normal Text
        rect = ls_uiLabelRect(c, ls_utf32Constant(U"Example"), previewX + 2*xMargin + (s32)(0.2f*previewW), 
                              topY - yMargin - (s32)(0.7f*rectH));
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h))
        { theme->selected = 1; theme->wheel.apply.callback1 = themeApplyToText; }
        
        if(theme->selected == 1)
        {
            ls_uiLabelInRect(c, ls_utf32Constant(U"Example"), rect.x, rect.y,
                             c->widgetColor, RGBg(0xFF), theme->wheel.pickedColor, 2);
        }
        else
        { ls_uiLabelInRect(c, ls_utf32Constant(U"Example"), rect.x, rect.y,
                           c->widgetColor, c->widgetColor, c->textColor, 2);
        }
        
        
        //NOTE: Inverted Text
        rect = ls_uiLabelRect(c, ls_utf32Constant(U"Inverted"), previewX + 2*xMargin + (s32)(0.45f*previewW), 
                              topY - yMargin - (s32)(0.7f*rectH));
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h))
        { theme->selected = 2; theme->wheel.apply.callback1 = themeApplyToInvertedText; }
        
        if(theme->selected == 2)
        {
            ls_uiLabelInRect(c, ls_utf32Constant(U"Inverted"), rect.x, rect.y,
                             c->invWidgetColor, RGBg(0x00), theme->wheel.pickedColor, 2);
        }
        else
        {
            ls_uiLabelInRect(c, ls_utf32Constant(U"Inverted"), rect.x, rect.y,
                             c->invWidgetColor, c->invWidgetColor, c->invTextColor, 2);
        }
        
        //NOTE: Border Color
        rect = { previewX + xMargin, topY - 2*yMargin - 2*rectH, (s32)(0.2f*previewW), rectH };
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h))
        { theme->selected = 3; theme->wheel.apply.callback1 = themeApplyToBorder; }
        
        UIRect smaller = { rect.x+(s32)(0.1f*rect.w), rect.y+(s32)(0.1f*rect.h), (s32)(0.8f*rect.w), (s32)(0.8f*rect.h) };
        if(theme->selected == 3)
        { 
            ls_uiRect(c, smaller.x, smaller.y, smaller.w, smaller.h, c->widgetColor, c->widgetColor, 2);
            ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, theme->wheel.pickedColor, RGBg(0xFF), 2);
        }
        else
        { 
            ls_uiRect(c, smaller.x, smaller.y, smaller.w, smaller.h, c->widgetColor, c->widgetColor, 2);
            ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, c->borderColor, c->borderColor, 2);
        }
        
        
        //NOTE: Button for Highlite
        static UIButton buttHi = ls_uiButtonInit(c, UIBUTTON_CLASSIC, ls_utf32Constant(U"Hilight"), NULL, NULL, NULL);
        
        rect = { previewX + 2*xMargin + (s32)(0.2f*previewW), topY - 2*yMargin - rectH-buttHi.h, buttHi.w, buttHi.h };
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h))
        { theme->selected = 4; theme->wheel.apply.callback1 = themeApplyToHighlite; }
        
        if(theme->selected == 4)
        {
            Color tmpHighlite = c->highliteColor;
            c->highliteColor  = theme->wheel.pickedColor;
            Color tmpBorder   = c->borderColor;
            c->borderColor    = RGBg(0xFF);
            ls_uiButton(c, &buttHi, rect.x, rect.y, 2);
            c->highliteColor  = tmpHighlite;
            c->borderColor    = tmpBorder;
            
        }
        else
        {
            ls_uiButton(c, &buttHi, rect.x, rect.y, 2);
        }
        
        //NOTE: Button for Pressed
        static UIButton buttPr = ls_uiButtonInit(c, UIBUTTON_CLASSIC, ls_utf32Constant(U"Pressed"), NULL, NULL, NULL);
        rect = { rect.x + (s32)(0.5f*xMargin) + buttHi.w, topY - 2*yMargin - rectH - buttPr.h, buttPr.w, buttPr.h };
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h))
        { theme->selected = 5; theme->wheel.apply.callback1 = themeApplyToPressed; }
        
        if(theme->selected == 5)
        {
            Color tmpPressed  = c->pressedColor;
            c->pressedColor   = theme->wheel.pickedColor;
            Color tmpBorder   = c->borderColor;
            c->borderColor    = RGBg(0xFF);
            ls_uiButton(c, &buttPr, rect.x, rect.y, 2);
            c->pressedColor   = tmpPressed;
            c->borderColor    = tmpBorder;
            
        }
        else
        {
            ls_uiButton(c, &buttPr, rect.x, rect.y, 2);
        }
        
        //NOTE: Inverted Widget Color
        rect = { previewX + xMargin, topY - 3*yMargin - 3*rectH, (s32)(0.2f*previewW), rectH };
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h))
        { theme->selected = 6; theme->wheel.apply.callback1 = themeApplyToInvertedWidget; }
        
        if(theme->selected == 6) 
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, theme->wheel.pickedColor, RGBg(0xFF), 2); }
        else
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, c->invWidgetColor, c->invWidgetColor, 2); }
        
        //NOTE: Widget Color
        rect = { previewX + 2*xMargin + rect.w, topY - 3*yMargin - 3*rectH, (s32)(0.2f*previewW), rectH };
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h))
        { theme->selected = 7; theme->wheel.apply.callback1 = themeApplyToWidget; }
        
        if(theme->selected == 7) 
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, theme->wheel.pickedColor, RGBg(0xFF), 2); }
        else
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, c->widgetColor, c->borderColor, 2); }
        
        
        //NOTE: Menu Bar Color
        rect = { previewX + 3*xMargin + 2*rect.w, topY - 3*yMargin - 3*rectH, (s32)(0.2f*previewW), rectH };
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h))
        { theme->selected = 8; theme->wheel.apply.callback1 = themeApplyToMenu; }
        
        if(theme->selected == 8) 
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, theme->wheel.pickedColor, RGBg(0xFF), 2); }
        else
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, c->menuBarColor, c->menuBarColor, 2); }
        
        ls_uiRect(c, previewX, previewY, previewW, previewH, 2);
        
    }
    
    return usedInput;
}

b32 selectThemeDefault(UIContext *c, void *data)
{
    currentTheme = THEME_DEFAULT;
    
    c->backgroundColor = RGBg(0x38);
    c->borderColor     = RGBg(0x22);
    c->menuBarColor    = RGBg(0x20);
    c->highliteColor   = RGBg(0x65);
    c->pressedColor    = RGBg(0x75);
    c->widgetColor     = RGBg(0x45);
    c->textColor       = RGBg(0xCC);
    c->invWidgetColor  = RGBg(0xBA);
    c->invTextColor    = RGBg(0x33);
    
    return FALSE;
}

b32 selectThemeDarkNight(UIContext *c, void *data)
{
    currentTheme = THEME_DARKNIGHT;
    
    c->backgroundColor = RGB(0x0C, 0x08, 0x0B);
    c->borderColor     = RGB(0x60, 0x7B, 0x7D);
    c->menuBarColor    = RGBg(0x20);
    c->highliteColor   = RGB(0x14, 0x15, 0x13);
    c->pressedColor    = RGB(0x18, 0x20, 0x19);
    c->widgetColor     = RGB(0x1C, 0x18, 0x1B);
    c->textColor       = RGBg(0xCC);
    c->invWidgetColor  = RGB(0xE3, 0xE7, 0xE4);
    c->invTextColor    = RGB(0x7D, 0x71, 0x7D);
    
    return FALSE;
}

b32 selectThemeLight(UIContext *c, void *data)
{
    currentTheme = THEME_LIGHT;
    
    c->backgroundColor = RGBg(0xEE);
    c->borderColor     = RGBg(0x00);
    c->menuBarColor    = RGBg(0x20);
    c->highliteColor   = RGB(0xAA, 0x99, 0xAA);
    c->pressedColor    = RGB(0xBB, 0x99, 0xAA);
    c->widgetColor     = RGBg(0xBB);
    c->textColor       = RGBg(0x33);
    c->invWidgetColor  = RGB(7, 5, 43);
    c->invTextColor    = RGBg(0xFF);
    
    return FALSE;
}

b32 selectThemeGreen(UIContext *c, void *data)
{
    currentTheme = THEME_GREEN;
    
    c->backgroundColor = RGB(13, 63, 30);
    c->menuBarColor    = RGB(32, 32, 32);
    c->highliteColor   = RGB(123, 192, 84);
    c->pressedColor    = RGB(143, 223, 97);
    c->widgetColor     = RGB(23, 115, 55);
    c->borderColor     = RGB(3, 15, 7);
    c->textColor       = RGB(253, 255, 252);
    c->invWidgetColor  = RGB(186, 186, 186);
    c->invTextColor    = RGB(51, 51, 51);
    
    return FALSE;
}

b32 selectThemeUser(UIContext *c, void *data)
{
    currentTheme = THEME_USER;
    
    c->backgroundColor = State.backgroundColor;
    c->menuBarColor    = State.menuBarColor;
    c->highliteColor   = State.highliteColor;
    c->pressedColor    = State.pressedColor;
    c->widgetColor     = State.widgetColor;
    c->borderColor     = State.borderColor;
    c->textColor       = State.textColor;
    c->invWidgetColor  = State.invWidgetColor;
    c->invTextColor    = State.invTextColor;
    
    return FALSE;
}

typedef  b32(*SelectThemeProc)(UIContext *, void *);
SelectThemeProc selectThemeProcs[THEME_COUNT] = { selectThemeDefault, selectThemeDarkNight, selectThemeLight,
    selectThemeGreen, selectThemeUser };

b32 openThemeColorPicker(UIContext *c, void *data)
{
    State.themePicker.isShown = !State.themePicker.isShown;
    return TRUE;
}
