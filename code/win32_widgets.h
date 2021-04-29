/* date = April 28th 2021 0:01 pm */

#ifndef WIN32_WIDGETS_H
#define WIN32_WIDGETS_H


#define AddAllComboBoxItems(wnd, s, n) { for(size_t i = 0; i < n; i++) \
{ComboBox_AddString(wnd, s[i]); } }

#define AddAllListBoxItems(wnd, s, n)  { for(size_t i = 0; i < n; i++) \
{ListBox_AddString(wnd, s[i]); } }


enum LabelAlign : u8;
struct Button;
struct ComboBox;
struct TextBox;
struct ListBox;
struct OrderField;
struct InitField;
struct Counter;

string getText(HWND hwnd);

HWND AddLabelBox(HWND win, LabelAlign A, const char *label, s32 x, s32 y, u32 width, u32 height);
HWND AddEditBox(HWND win, s32 x, s32 y, u32 width, u32 height, u64 id, const char *defName);
HWND AddStaticEditBox(HWND win, s32 x, s32 y, u32 width, u32 height, u64 id, b32 isMultiline);
HWND AddEditNumberBox(HWND win, s32 x, s32 y, u32 width, u32 height, u64 id, s32 defaultNumber);
HWND AddStaticNumberBox(HWND win, s32 value, s32 x, s32 y, u32 width, u32 height, u64 id);

Button *AddButton(HWND win, HWND *pageArr, const char *label, s32 x, s32 y, 
                  u32 w, u32 h, u64 id, b32 hasBckgnd = TRUE);

ComboBox *AddComboBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                      s32 x, s32 y, u32 width, u32 height, u64 id, u32 numItems);

ComboBox *AddUnsortedComboBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                              s32 x, s32 y, u32 width, u32 height, u64 id, u32 numItems);

TextBox *AddTextBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                    s32 x, s32 y, u32 width, u32 height, u64 id, const char *defName = (const char *)"");

TextBox *AddStaticTextBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                          s32 x, s32 y, u32 width, u32 height, u64 id, b32 isMultiline);

TextBox *AddStaticUnlabeledTextBox(HWND win, HWND *pageArr, s32 x, s32 y, u32 width, u32 height,
                                   u64 id, b32 isMultiline = FALSE);

TextBox *AddNumberBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                      s32 x, s32 y, u32 width, u32 height, u64 id, s32 defaultNumber = 0);

TextBox *AddValueBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                     s32 value, s32 x, s32 y, u32 width, u32 height, u64 id);

ListBox *AddListBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                    s32 x, s32 y, u32 width, u32 height, u64 id, b32 isSorted = TRUE);

ListBox *AddSingleSelListBox(HWND win, HWND *pageArr, const char *label, LabelAlign A,
                             s32 x, s32 y, u32 width, u32 height, u64 id, b32 isSorted = TRUE);

OrderField AddOrderField(HWND win, HWND **winA, s32 x, s32 y, u32 idx, u64 *id);

InitField AddInitField(HWND h, HWND **winA, const char *label, s32 x, s32 y, 
                       u64 *id, u64 initId, b32 isParty);

Counter AddCounter(HWND h, HWND **winA, const char *label, s32 x, s32 y, u64 *id);
#endif //WIN32_WIDGETS_H
