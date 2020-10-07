
WNDPROC mainWinProc;
LRESULT subEditProc(HWND h, UINT msg, WPARAM w, LPARAM l);


HWND AddLabelBox(HWND win, LabelAlign A, const char *label, s32 x, s32 y, u32 width, u32 height)
{
    s32 correctX = x;
    s32 correctY = y;
    s32 correctWidth = width;
    s32 correctHeight = height;
    
    switch(A)
    {
        case LABEL_NULL:
        {
            return NULL;
        } break;
        
        case LABEL_UP:
        {
            correctHeight = 20;
            correctY = y - correctHeight;
        } break;
        
        case LABEL_DOWN:
        {
            correctHeight = 20;
            correctY = y + correctHeight;
        } break;
        
        case LABEL_LEFT:
        {
            correctWidth = 80;
            correctHeight = 20;
            correctX = x - correctWidth;
        } break;
        
        case LABEL_RIGHT:
        {
            correctWidth = 80;
            correctHeight = 20;
            correctX = x + correctWidth;
        } break;
        
        default:
        {
            Assert(FALSE);
        }
    };
    
    //WindowClass labelClass = {};
    
    //TODO: Make label id dynamically allocated
    HWND Result = CreateWindowExA(0, WC_STATIC, 0,
                                  WS_CHILD | WS_VISIBLE |
                                  SS_CENTER | SS_OWNERDRAW,
                                  correctX, correctY, correctWidth, correctHeight,
                                  win, (HMENU)1, MainInstance, 0);
    
    SetWindowTextA(Result, label);
    
    return Result;
}


HWND AddEditBox(HWND win, s32 x, s32 y, u32 width, u32 height, u64 id, const char *defName = (const char *)"")
{
    HWND Result = CreateWindowExA(0, WC_EDIT, defName,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER |
                                  ES_LEFT | ES_AUTOHSCROLL,
                                  x, y, width, height,
                                  win, (HMENU)id, MainInstance, 0);
    mainWinProc = (WNDPROC)SetWindowLongPtrA(Result, GWLP_WNDPROC, (LONG_PTR)subEditProc);
    
    return Result;
}

HWND AddStaticEditBox(HWND win, s32 x, s32 y, u32 width, u32 height, u64 id,
                      b32 isMultiline = FALSE)
{
    u32 flags = ES_LEFT | ES_READONLY;
    if(isMultiline == TRUE) { flags |= WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL;}
    else { flags |= ES_AUTOHSCROLL; }
    
    HWND Result = CreateWindowExA(0, WC_EDIT, "",
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | flags,
                                  x, y, width, height,
                                  win, (HMENU)id, MainInstance, 0);
    
    return Result;
}

HWND AddEditNumberBox(HWND win, s32 x, s32 y, u32 width, u32 height, u64 id, s32 defaultNumber = 0)
{
    char num[32] = {};
    ls_itoa_t(defaultNumber, num, 32);
    
    HWND Result = CreateWindowExA(0, WC_EDIT, num,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER |
                                  ES_LEFT | ES_AUTOHSCROLL | ES_NUMBER,
                                  x, y, width, height,
                                  win, (HMENU)id, MainInstance, 0);
    
    mainWinProc = (WNDPROC)SetWindowLongPtrA(Result, GWLP_WNDPROC, (LONG_PTR)subEditProc);
    
    return Result;
}

HWND AddStaticNumberBox(HWND win, s32 value, s32 x, s32 y, u32 width, u32 height, u64 id)
{
    char *v = ls_itoa(value);
    HWND Result = CreateWindowExA(0, WC_EDIT, v,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER |
                                  ES_LEFT | ES_AUTOHSCROLL | ES_NUMBER | ES_READONLY,
                                  x, y, width, height,
                                  win, (HMENU)id, MainInstance, 0);
    ls_free(v);
    
    return Result;
}

Button *AddButton(HWND win, HWND *pageArr, const char *label, s32 x, s32 y,
                  u32 width, u32 height, u64 id, b32 hasBackground = TRUE)
{
    Button *Result = (Button *)ls_alloc(sizeof(Button));
    
    Result->box = CreateWindowExA(0, WC_BUTTON, label,
                                  WS_CHILD | WS_VISIBLE | WS_BORDER |
                                  BS_CENTER | BS_TEXT | BS_OWNERDRAW,
                                  x, y, width, height,
                                  win, (HMENU)id, MainInstance, 0);
    Result->id = id;
    pageArr[0] = Result->box;
    
    if(hasBackground == TRUE) { Result->hasBackground = TRUE; }
    else { Result->hasBackground = FALSE; }
    
    ElementMap[id].ptr      = (void *)Result;
    ElementMap[id].isButton = TRUE;
    
    return Result;
}

ComboBox *AddComboBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                      s32 x, s32 y, u32 width, u32 height, u64 id, u32 numItems)
{
    ComboBox *Result = (ComboBox *)ls_alloc(sizeof(ComboBox));
    
    HWND Label = AddLabelBox(win, A, label, x, y, width, height);
    Result->label = Label;
    
    HWND Box = CreateWindowExA(0, WC_COMBOBOX, 0,
                               WS_CHILD | WS_VISIBLE | WS_BORDER |
                               CBS_DROPDOWNLIST | CBS_SORT | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED,
                               x, y, width, height*(numItems + 1),
                               win, (HMENU)id, MainInstance, 0);
    Result->box = Box;
    Result->id = id;
    
    pageArr[0] = Box;
    pageArr[1] = Label;
    
    ElementMap[id].ptr        = (void *)Result;
    ElementMap[id].isComboBox = TRUE;
    
    return Result;
}

ComboBox *AddUnsortedComboBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                              s32 x, s32 y, u32 width, u32 height, u64 id, u32 numItems)
{
    ComboBox *Result = (ComboBox *)ls_alloc(sizeof(ComboBox));
    
    HWND Label = AddLabelBox(win, A, label, x, y, width, height);
    Result->label = Label;
    
    HWND Box = CreateWindowExA(0, WC_COMBOBOX, 0,
                               WS_CHILD | WS_VISIBLE | WS_BORDER |
                               CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED,
                               x, y, width, height*(numItems + 1),
                               win, (HMENU)id, MainInstance, 0);
    Result->box = Box;
    Result->id = id;
    
    pageArr[0] = Box;
    pageArr[1] = Label;
    
    ElementMap[id].ptr        = (void *)Result;
    ElementMap[id].isComboBox = TRUE;
    
    return Result;
}

TextBox *AddTextBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                    s32 x, s32 y, u32 width, u32 height, u64 id, const char *defName = (const char *)"")
{
    TextBox *Result = (TextBox *)ls_alloc(sizeof(TextBox));
    
    if(A != LABEL_NULL)
    {
        HWND Label = AddLabelBox(win, A, label, x, y, width, height);
        Result->label = Label;
        pageArr[1] = Label;
    }
    
    Result->box = AddEditBox(win, x, y, width, height, id, defName);
    Result->id = id;
    
    pageArr[0] = Result->box;
    
    ElementMap[id].ptr        = (void *)Result;
    ElementMap[id].isTextBox  = TRUE;
    
    return Result;
}

TextBox *AddStaticTextBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                          s32 x, s32 y, u32 width, u32 height, u64 id, b32 isMultiline = FALSE)
{
    TextBox *Result = (TextBox *)ls_alloc(sizeof(TextBox));
    
    HWND Label = AddLabelBox(win, A, label, x, y, width, height);;
    Result->label = Label;
    
    Result->box = AddStaticEditBox(win, x, y, width, height, id, isMultiline);
    Result->id = id;
    
    pageArr[0] = Result->box;
    pageArr[1] = Label;
    
    ElementMap[id].ptr        = (void *)Result;
    ElementMap[id].isTextBox  = TRUE;
    
    return Result;
}

TextBox *AddStaticUnlabeledTextBox(HWND win, HWND *pageArr, s32 x, s32 y, u32 width, u32 height,
                                   u64 id, b32 isMultiline = FALSE)
{
    TextBox *Result = (TextBox *)ls_alloc(sizeof(TextBox));
    
    Result->box = AddStaticEditBox(win, x, y, width, height, id, isMultiline);
    Result->id = id;
    
    pageArr[0] = Result->box;
    
    ElementMap[id].ptr        = (void *)Result;
    ElementMap[id].isTextBox  = TRUE;
    
    return Result;
}

TextBox *AddNumberBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                      s32 x, s32 y, u32 width, u32 height, u64 id, s32 defaultNumber = 0)
{
    TextBox *Result = (TextBox *)ls_alloc(sizeof(TextBox));
    HWND Label;
    if(A != LABEL_NULL)
    {
        Label = AddLabelBox(win, A, label, x, y, width, height);
        Result->label = Label;
    }
    
    Result->box = AddEditNumberBox(win, x, y, width, height, id, defaultNumber);
    Result->id = id;
    
    pageArr[0] = Result->box;
    
    if(A != LABEL_NULL)
    { pageArr[1] = Label; }
    
    ElementMap[id].ptr        = (void *)Result;
    ElementMap[id].isTextBox  = TRUE;
    
    return Result;
}

TextBox *AddValueBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                     s32 value, s32 x, s32 y, u32 width, u32 height, u64 id)
{
    TextBox *Result = (TextBox *)ls_alloc(sizeof(TextBox));
    HWND Label;
    if(A != LABEL_NULL)
    {
        Label = AddLabelBox(win, A, label, x, y, width, height);
        Result->label = Label;
    }
    
    Result->box = AddStaticNumberBox(win, value, x, y, width, height, id);
    Result->id = id;
    
    pageArr[0] = Result->box;
    
    if(A != LABEL_NULL)
    { pageArr[1] = Label; }
    
    ElementMap[id].ptr        = (void *)Result;
    ElementMap[id].isTextBox  = TRUE;
    
    return Result;
}

ListBox *AddListBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                    s32 x, s32 y, u32 width, u32 height, u64 id, b32 isSorted = TRUE)
{
    ListBox *Result = (ListBox *)ls_alloc(sizeof(ListBox));
    HWND Label = AddLabelBox(win, A, label, x, y, width, height);
    
    Result->label = Label;
    
    u32 flags;
    if(isSorted) { flags = LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_MULTIPLESEL | LBS_NOTIFY | LBS_SORT; }
    else { flags = LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_MULTIPLESEL | LBS_NOTIFY; }
    
    Result->box = CreateWindowExA(0, WC_LISTBOX, "",
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | flags,
                                  x, y, width, height,
                                  win, (HMENU)id, MainInstance, 0);
    
    Result->id = id;
    
    pageArr[0] = Result->box;
    pageArr[1] = Label;
    
    ElementMap[id].ptr        = (void *)Result;
    ElementMap[id].isListBox  = TRUE;
    
    return Result;
}

ListBox *AddSingleSelListBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                             s32 x, s32 y, u32 width, u32 height, u64 id, b32 isSorted = TRUE)
{
    ListBox *Result = (ListBox *)ls_alloc(sizeof(ListBox));
    HWND Label = AddLabelBox(win, A, label, x, y, width, height);
    
    Result->label = Label;
    
    u32 flags;
    if(isSorted) { flags = LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_NOTIFY | LBS_SORT; }
    else { flags = LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_NOTIFY; }
    
    Result->box = CreateWindowExA(0, WC_LISTBOX, "",
                                  WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | flags,
                                  x, y, width, height,
                                  win, (HMENU)id, MainInstance, 0);
    Result->id = id;
    
    pageArr[0] = Result->box;
    pageArr[1] = Label;
    
    ElementMap[id].ptr        = (void *)Result;
    ElementMap[id].isListBox  = TRUE;
    
    return Result;
}
