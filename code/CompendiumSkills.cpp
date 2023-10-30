//NOTE: Returns the new index into the entries array, since this function could process more than one
//      indexed entry at a time (parentheses elements count as multiple entries, even though are a single skill)
s32 BuildSkillFromPacked_t(u32 *entries, s32 index, utf32 *tmp)
{
    u32 entry = entries[index];
    
    b32 thisParen = (entry & PAREN_BIT_U32) != 0;
    AssertMsg(thisParen == FALSE, "Found entry with parenthesis bit."
              "This should be automatically handled by build SkillFromPacked."
              "Either the skill array is malformed, or BuildSkillFromPacked hasn't consumed all parentheses entries.");
    
    //NOTE: If we could have parentheses
    if(index < 23)
    {
        b32 nextParen = (entries[index+1] & PAREN_BIT_U32) != 0;
        
        const char32_t *name = SkillTypeToName[(entry & SKILL_TYPE_MASK)];
        s32 value = (s32)((s8)(entry >> SKILL_BITS));
        
        //NOTE: Build the main entry
        ls_utf32Append(tmp, ls_utf32Constant(name));
        ls_utf32AppendChar(tmp, ' ');
        if(value >= 0) { ls_utf32AppendChar(tmp, '+');}
        ls_utf32AppendInt(tmp, value);
        
        if(nextParen == TRUE)
        { 
            ls_utf32Append(tmp, ls_utf32Constant(U" ("));
            
            //TODO: Add Sentinel Value for value. (We have the bits now!)
            while(nextParen == TRUE)
            {
                AssertMsgF(index < 24, "Iterated too many times while searching for paren skills");
                
                index += 1;
                u32 nextEntry = entries[index];
                nextParen = (entries[index+1] & PAREN_BIT_U32) != 0;
                
                const char32_t *nextName = SkillTypeToName[(nextEntry & SKILL_TYPE_MASK)];
                s32 nextValue = (s32)((s8)(nextEntry >> SKILL_BITS));
                
                if(nextValue == 0)
                {
                    ls_utf32Append(tmp, ls_utf32Constant(nextName));
                    if(index < 23 && nextParen) ls_utf32Append(tmp, ls_utf32Constant(U", "));
                }
                else
                {
                    if(nextValue >= 0) { ls_utf32AppendChar(tmp, '+');}
                    ls_utf32AppendInt(tmp, nextValue);
                    ls_utf32AppendChar(tmp, ' ');
                    ls_utf32Append(tmp, ls_utf32Constant(name));
                    if(index < 23 && nextParen) ls_utf32Append(tmp, ls_utf32Constant(U", "));
                }
                
            }
            
            ls_utf32AppendChar(tmp, ')');
        }
        
        return index;
    }
    else //NOTE: No parenthesis possible.
    {
        const char32_t *name = SkillTypeToName[(entry & SKILL_TYPE_MASK)];
        s32 value = (s32)((s8)(entry >> SKILL_BITS));
        
        ls_utf32Append(tmp, ls_utf32Constant(name));
        ls_utf32AppendChar(tmp, ' ');
        if(value >= 0) { ls_utf32AppendChar(tmp, '+');}
        ls_utf32AppendInt(tmp, value);
        
        return index;
    }
}