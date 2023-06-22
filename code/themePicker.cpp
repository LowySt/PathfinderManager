b32 themeApplyToBackground(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->backgroundColor    = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToBorder(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->borderColor        = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToHighlite(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->highliteColor      = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToPressed(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->pressedColor       = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToWidget(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->widgetColor        = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToText(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->textColor          = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToInvertedText(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->invTextColor       = picker->pickedColor;
    return TRUE;
}

b32 themeApplyToInvertedWidget(UIContext *c, void *userData)
{
    UIColorPicker *picker = &((ThemePicker *)userData)->wheel;
    c->invWidgetColor     = picker->pickedColor;
    return TRUE;
}

b32 DrawThemePicker(UIContext *c)
{
    Input *UserInput = &c->UserInput;
    b32 usedInput = FALSE;
    
    ThemePicker *theme = &State.themePicker;
    
    if(theme->isShown == TRUE)
    {
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
        { theme->selected = 0; theme->wheel.apply.onClick = themeApplyToBackground; }
        
        if(theme->selected == 0) 
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, theme->wheel.pickedColor, RGBg(0xFF), 2); }
        else
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, c->backgroundColor, c->backgroundColor, 2); }
        
        //NOTE: Normal Text
        rect = ls_uiLabelRect(c, ls_utf32Constant(U"Example"), previewX + 2*xMargin + (s32)(0.2f*previewW), 
                              topY - yMargin - (s32)(0.7f*rectH));
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h))
        { theme->selected = 1; theme->wheel.apply.onClick = themeApplyToText; }
        
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
        { theme->selected = 2; theme->wheel.apply.onClick = themeApplyToInvertedText; }
        
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
        { theme->selected = 3; theme->wheel.apply.onClick = themeApplyToBorder; }
        
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
        { theme->selected = 4; theme->wheel.apply.onClick = themeApplyToHighlite; }
        
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
        { theme->selected = 5; theme->wheel.apply.onClick = themeApplyToPressed; }
        
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
        { theme->selected = 6; theme->wheel.apply.onClick = themeApplyToInvertedWidget; }
        
        if(theme->selected == 6) 
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, theme->wheel.pickedColor, RGBg(0xFF), 2); }
        else
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, c->invWidgetColor, c->invWidgetColor, 2); }
        
        //NOTE: Widget Color
        rect = { previewX + 2*xMargin + rect.w, topY - 3*yMargin - 3*rectH, (s32)(0.2f*previewW), rectH };
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h))
        { theme->selected = 7; theme->wheel.apply.onClick = themeApplyToWidget; }
        
        if(theme->selected == 7) 
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, theme->wheel.pickedColor, RGBg(0xFF), 2); }
        else
        { ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, c->widgetColor, c->borderColor, 2); }
        
        
        ls_uiRect(c, previewX, previewY, previewW, previewH, 2);
        
    }
    
    return usedInput;
}
