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
        Color col   = {};
        
        s32 rectH = 0.2f*previewH;
        
        //NOTE: Background example rect
        rect = { previewX + xMargin, topY - yMargin - rectH, (s32)(0.2f*previewW), rectH };
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h)) { theme->selected = 0; }
        
        col = c->backgroundColor;
        if(theme->selected == 0) { col = theme->wheel.pickedColor; }
        ls_uiRect(c, rect.x, rect.y, rect.w, rect.h, col, col, 2);
        
        //NOTE: Example Text TODO
        rect = { previewX + 2*xMargin + (s32)(0.2f*previewW), topY - yMargin - (s32)(0.7f*rectH), 200, 20 };
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h)) { theme->selected = 1; }
        
        col = c->textColor;
        if(theme->selected == 1) { col = theme->wheel.pickedColor; }
        ls_uiLabelInRect(c, ls_utf32Constant(U"Example"), rect.x, rect.y, c->widgetColor, c->widgetColor, col, 2);
        
        rect = { previewX + 2*xMargin + (s32)(0.45f*previewW), topY - yMargin - (s32)(0.7f*rectH), 200, 20 };
        if(LeftClickIn(rect.x, rect.y, rect.w, rect.h)) { theme->selected = 2; }
        
        col = c->invTextColor;
        if(theme->selected == 2) { col = theme->wheel.pickedColor; }
        ls_uiLabelInRect(c, ls_utf32Constant(U"Inverted"), rect.x, rect.y, c->widgetColor, c->widgetColor, col, 2);
        
        ls_uiRect(c, previewX, previewY, previewW, previewH, 2);
    }
    
    return usedInput;
}