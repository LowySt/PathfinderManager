
/// WindowProc Subclass for Edit Controls (Only done on EditBox)
LRESULT subEditProc(HWND h, UINT msg, WPARAM w, LPARAM l)
{
    InitPage *Init = State.Init;
    
    switch (msg)
    {
        case WM_RBUTTONUP:
        {
            InitField *field = getInitByHWND(h);
            if(field != 0x0)
            {
                field->isSelected = TRUE;
            }
        } break;
        
        case WM_CHAR:
        {
            if(w == VK_RETURN) { return 0; } //NOTE: This stupid shit stops the Beeping when pressing enter
            
            //NOTE: Still needs to call the default proc to actually have characters display
            return CallWindowProcA(mainWinProc, h, msg, w, l);
        } break;
        
        case WM_SETFOCUS:
        {
            //NOTE: We shouldn't touch the edit box if it's set to read only!
            DWORD dwStyle = (DWORD)GetWindowLongA(h, GWL_STYLE);
            if((dwStyle & ES_READONLY) != 0) { break; }
            
            b32 isValid = FALSE;
            for( u32 i = 0; i < Init->VisibleMobs; i++) {
                if(h == Init->MobFields[i].Name->box)  { isValid = TRUE; break; }
                if(h == Init->MobFields[i].Bonus->box) { isValid = TRUE; break; }
                if(h == Init->MobFields[i].AC->box)    { isValid = TRUE; break; }
                if(h == Init->MobFields[i].Final->box) { isValid = TRUE; break; }
            }
            
            if(!isValid) { 
                for( u32 i = 0; i < Init->VisibleAllies; i++) {
                    if(h == Init->AllyFields[i].Name->box)  { isValid = TRUE; break; }
                    if(h == Init->AllyFields[i].Bonus->box) { isValid = TRUE; break; }
                    if(h == Init->AllyFields[i].AC->box)    { isValid = TRUE; break; }
                    if(h == Init->AllyFields[i].Final->box) { isValid = TRUE; break; }
                }
            }
            
            if(!isValid) {
                for( u32 i = 0; i < PARTY_NUM; i++) {
                    if(h == Init->PlayerFields[i].Bonus->box) { isValid = TRUE; break; }
                }
            }
            
            if(isValid) { Edit_SetText(h, ""); }
            
            return CallWindowProcA(mainWinProc, h, msg, w, l);
        } break;
        
        case WM_KEYDOWN:
        {
            //TODO: Make this more efficient by using int GetDlgCtrlID(HWND hWnd);
            //      To get the position in the array using the control IDs. This way i don't need to for loop
            switch(w)
            {
                case VK_DELETE: { return CallWindowProcA(mainWinProc, h, msg, w, l); } break;
                case VK_LEFT:   { return CallWindowProcA(mainWinProc, h, msg, w, l); } break;
                case VK_RIGHT:  { return CallWindowProcA(mainWinProc, h, msg, w, l); } break;
                
                case VK_DOWN:
                {
                    for(u32 i = 0; i < Init->VisibleMobs; i++) 
                    {
                        if(h == Init->MobFields[i].Name->box) 
                        {
                            if(i < Init->VisibleMobs - 1) 
                            { 
                                char name[32] = {};
                                s32 len = Edit_GetTextLength(h);
                                Edit_GetText(h, name, 32);
                                
                                char *BeginOfNumber = name;
                                u32 alphaLen = 0;
                                while(!ls_isANumber(*BeginOfNumber)) 
                                {
                                    if(BeginOfNumber > name + len) 
                                    {
                                        SetFocus(MainWindow);
                                        return CallWindowProcA(mainWinProc, h, msg, w, l);
                                    }
                                    
                                    BeginOfNumber += 1; 
                                    alphaLen += 1;
                                }
                                
                                s64 newNumber = ls_atoi(BeginOfNumber, len - alphaLen) + 1;
                                ls_itoa_t(newNumber, BeginOfNumber, 32 - alphaLen);
                                
                                char bonus[8] = {};
                                char AC[8] = {};
                                
                                Edit_GetText(Init->MobFields[i].Bonus->box, bonus, 8);
                                Edit_GetText(Init->MobFields[i].AC->box, AC, 8);
                                
                                if(!((bonus[0] == '0') || (bonus[0] == 0)))
                                { Edit_SetText(Init->MobFields[i+1].Bonus->box, bonus); }
                                
                                if(!((AC[0] == '0') || (AC[0] == 0)))
                                { Edit_SetText(Init->MobFields[i+1].AC->box, AC); }
                                
                                //NOTE: Just make sure the buffer is zero terminated.
                                name[31] = 0;
                                SetFocus(Init->MobFields[i+1].Name->box);
                                Edit_SetText(Init->MobFields[i+1].Name->box, name);
                            }
                            else 
                            { SetFocus(MainWindow); }
                        }
                    }
                    
                    for(u32 i = 0; i < Init->VisibleAllies; i++) {
                        
                        if(h == Init->AllyFields[i].Name->box) 
                        {
                            if(i < Init->VisibleAllies - 1) 
                            { 
                                char name[32] = {};
                                s32 len = Edit_GetTextLength(h);
                                Edit_GetText(h, name, 32);
                                
                                char *BeginOfNumber = name;
                                u32 alphaLen = 0;
                                while(!ls_isANumber(*BeginOfNumber)) 
                                { 
                                    if(BeginOfNumber > name + len) 
                                    { 
                                        SetFocus(MainWindow);
                                        return CallWindowProcA(mainWinProc, h, msg, w, l);
                                    }
                                    
                                    BeginOfNumber += 1; 
                                    alphaLen += 1;
                                }
                                
                                s64 newNumber = ls_atoi(BeginOfNumber, len - alphaLen) + 1;
                                ls_itoa_t(newNumber, BeginOfNumber, 32 - alphaLen);
                                
                                char bonus[8] = {};
                                char AC[8] = {};
                                
                                Edit_GetText(Init->AllyFields[i].Bonus->box, bonus, 8);
                                Edit_GetText(Init->AllyFields[i].AC->box, AC, 8);
                                
                                if(!((bonus[0] == '0') || (bonus[0] == 0)))
                                { Edit_SetText(Init->AllyFields[i+1].Bonus->box, bonus); }
                                
                                if(!((AC[0] == '0') || (AC[0] == 0)))
                                { Edit_SetText(Init->AllyFields[i+1].AC->box, AC); }
                                
                                //NOTE: Just make sure the buffer is zero terminated.
                                name[31] = 0;
                                SetFocus(Init->AllyFields[i+1].Name->box);
                                Edit_SetText(Init->AllyFields[i+1].Name->box, name);
                            }
                            else 
                            { 
                                SetFocus(MainWindow);
                                return CallWindowProcA(mainWinProc, h, msg, w, l);
                            }
                        }
                    }
                    
                    return CallWindowProcA(mainWinProc, h, msg, w, l);
                } break;
                
                case VK_RETURN:
                {
                    for(u32 i = 0; i < Init->VisibleMobs; i++) {
                        if(h == Init->MobFields[i].Name->box) { 
                            
                            if(i < Init->VisibleMobs - 1) { SetFocus(Init->MobFields[i+1].Name->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                        
                        if(h == Init->MobFields[i].Bonus->box) { 
                            
                            if(i < Init->VisibleMobs - 1) { SetFocus(Init->MobFields[i+1].Bonus->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                        
                        if(h == Init->MobFields[i].AC->box) { 
                            
                            if(i < Init->VisibleMobs - 1) { SetFocus(Init->MobFields[i+1].AC->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                        
                        if(h == Init->MobFields[i].Final->box) { 
                            
                            if(i < Init->VisibleMobs - 1) { SetFocus(Init->MobFields[i+1].Final->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                    }
                    
                    for(u32 i = 0; i < PARTY_NUM; i++) {
                        if(h == Init->PlayerFields[i].Bonus->box) {
                            if(i < PARTY_NUM - 1) { SetFocus(Init->PlayerFields[i+1].Bonus->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                    }
                    
                    for(u32 i = 0; i < Init->VisibleAllies; i++) {
                        if(h == Init->AllyFields[i].Name->box) { 
                            if(i < Init->VisibleAllies - 1) { SetFocus(Init->AllyFields[i+1].Name->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                        
                        if(h == Init->AllyFields[i].Bonus->box) { 
                            if(i < Init->VisibleAllies - 1) { SetFocus(Init->AllyFields[i+1].Bonus->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                        
                        if(h == Init->AllyFields[i].AC->box) { 
                            if(i < Init->VisibleAllies - 1) { SetFocus(Init->AllyFields[i+1].AC->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                        
                        if(h == Init->AllyFields[i].Final->box) { 
                            if(i < Init->VisibleAllies - 1) { SetFocus(Init->AllyFields[i+1].Final->box); }
                            else { SetFocus(MainWindow); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                    }
                    
                    for(u32 i = 0; i < THROWER_NUM; i++)
                    {
                        if(h == Init->Throwers[i].Name->box)   
                        { 
                            SetFocus(Init->Throwers[i].ToHit->box); 
                            return CallWindowProcA(mainWinProc, h, msg, w, l);
                        }
                        
                        if(h == Init->Throwers[i].ToHit->box)
                        { 
                            SetFocus(Init->Throwers[i].Damage->box); 
                            return CallWindowProcA(mainWinProc, h, msg, w, l);
                        }
                        if(h == Init->Throwers[i].Damage->box)
                        { 
                            SetFocus(MainWindow);
                            return CallWindowProcA(mainWinProc, h, msg, w, l);
                        }
                    }
                    
                    for(u32 i = 0; i < COUNTER_NUM; i++)
                    {
                        if(h == Init->Counters[i].Field->box)
                        {
                            SetFocus(Init->Counters[i].Rounds->box);
                            return CallWindowProcA(mainWinProc, h, msg, w, l);
                        }
                        
                        if(h == Init->Counters[i].Rounds->box)
                        {
                            SetFocus(MainWindow);
                            return CallWindowProcA(mainWinProc, h, msg, w, l);
                        }
                    }
                    
                    for(u32 i = 0; i < Init->VisibleOrder; i++) {
                        
                        if(h == Init->Order[i].Pos->box) {
                            int len = Edit_GetTextLength(h);
                            char buff[8] = {};
                            Edit_GetText(h, buff, 8);
                            
                            s32 newPosition = ls_atoi(buff, len);
                            
                            if((newPosition >= Init->VisibleOrder) ||
                               (newPosition == i) || (newPosition < 0)) { 
                                return CallWindowProcA(mainWinProc, h, msg, w, l); 
                            }
                            
                            OrderField *oldOrder = &Init->Order[i];
                            
                            u32 tmpFieldID = oldOrder->fieldId;
                            b32 tmpIsMob   = oldOrder->isMob;
                            b32 tmpIsParty = oldOrder->isParty;
                            
                            char tmpName[32] = {};
                            Edit_GetText(oldOrder->Field->box, tmpName, 32);
                            
                            if(newPosition > i)
                            {
                                for(u32 j = i; j <= newPosition; j++)
                                {
                                    char name[32] = {};
                                    Edit_GetText(Init->Order[j+1].Field->box, name, 32);
                                    
                                    Init->Order[j].fieldId = Init->Order[j+1].fieldId;
                                    Init->Order[j].isMob   = Init->Order[j+1].isMob;
                                    Init->Order[j].isParty = Init->Order[j+1].isParty;
                                    
                                    Edit_SetText(Init->Order[j].Field->box, name);
                                }
                            }
                            
                            if(newPosition < i)
                            {
                                //NOTE: This was changed from j >= newPosition, 
                                // because of j-1 going out of bounds when newPosition is 0.
                                for(u32 j = i; j > newPosition; j--)
                                {
                                    char name[32] = {};
                                    Edit_GetText(Init->Order[j-1].Field->box, name, 32);
                                    
                                    Init->Order[j].fieldId = Init->Order[j-1].fieldId;
                                    Init->Order[j].isMob   = Init->Order[j-1].isMob;
                                    Init->Order[j].isParty = Init->Order[j-1].isParty;
                                    
                                    
                                    Edit_SetText(Init->Order[j].Field->box, name);
                                }
                            }
                            
                            Init->Order[newPosition].fieldId = tmpFieldID;
                            Init->Order[newPosition].isMob   = tmpIsMob;
                            Init->Order[newPosition].isParty = tmpIsParty;
                            
                            Edit_SetText(Init->Order[newPosition].Field->box, tmpName);
                            
                            //NOTE: Reset the position to actual ordinal
                            char ith[8] = {};
                            ls_itoa_t(i, ith, 8);
                            Edit_SetText(h, ith);
                            
                            //NOTE: Update "Current" TextBox to reflect change if there was any
                            //NOTE:TODO: Do I actually like this? Do we want this?
                            if(Init->currIdx == newPosition)
                            { Edit_SetText(Init->Current->box, tmpName); }
                            
                            return CallWindowProcA(mainWinProc, h, msg, w, l); 
                        }
                    }
                    
                    return CallWindowProcA(mainWinProc, h, msg, w, l);
                } break;
            }
        } break;
        
        default: { 
            return CallWindowProcA(mainWinProc, h, msg, w, l); 
        }
    }
    return 0;
}
