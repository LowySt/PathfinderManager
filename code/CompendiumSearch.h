#ifndef _COMPENDIUM_SEARCH_H
#define _COMPENDIUM_SEARCH_H

enum SearchKind
{
    SEARCH_NAME,
    SEARCH_GS,
    SEARCH_TYPE,
};

struct CompendiumSearchData
{ 
    UITextBox *searchBar;
    buffer *searchBuffer;
    SearchKind kind;
};

b32 CompendiumSearchFunctionMobs(UIContext *c, void *userData);
b32 CompendiumSearchFunctionNPCs(UIContext *c, void *userData);

#endif //_COMPENDIUM_SEARCH_H
