//NOTE: This function processes one skill entry at a time, and returns either 0 or 1.
//      The return value represents whether an entry was written or not.
void BuildSkillsFromPacked_t(CachedPageEntry *page, Status *status, u32 *entries)
{
    SkillASCat prevCat = SK_UNDEFINED;
    b32 wasInParen = FALSE;
    b32 firstEntry = TRUE;
    s32 i = 0;
    while(entries[i] && i < 24)
    {
        u32 entry = entries[i];
        
        b32 hasArchetype = compendium.appliedArchetypes.count > 0;
        
        if(hasArchetype) { 
            entry = CompendiumApplyAllArchetypeSkills(entry);
            
            //NOTE: Applying the archetype has removed the current skill, so we skip it
            if(entry == 0) { i += 1; continue; }
        }
        
        u32 tempUTF32Buffer[256] = {};
        utf32 tempString = { tempUTF32Buffer, 0, 256 };
        
        //NOTE: If the skill was interned, we just copy the interned value and return.
        //      TODO: This does not modify the skill value through Statuses and AS changes
        //            But it should only count for a minority of relatively irrelevant skills.
        if((entry & INTERN_BIT_U32) != 0)
        { 
            GetEntryFromBuffer_t(&compendium.codex.skills, &tempString, (entry & (~INTERN_BIT_U32)), "skills");
            if(firstEntry == FALSE) { ls_utf32Append(&page->skills, ls_utf32Constant(U", ")); }
            else                    { firstEntry = FALSE; }
            ls_utf32Append(&page->skills, tempString);
            i += 1;
            continue;
        }
        
        
        b32 isInParen = (entry & PAREN_BIT_U32) != 0;
        
        SkillType skType     = SkillType(entry & SKILL_TYPE_MASK);
        SkillASCat skCat     = SkillTypeToCat[skType];
        const char32_t *name = SkillTypeToName[skType];
        s32 value            = (s32)((s8)(entry >> SKILL_BITS));
        
        if(skCat == SK_UNDEFINED) { skCat = prevCat; }
        
        //NOTE: The skill value may be modified by both changes in the Ability Scores
        //      and status effects. We calculate those changes and apply them to the value
        //      before converting it into a string.
        GET_AS_BONUS_ARRAY(bonusOld, bonusNew, page->modAS, page->origAS);
        
        s32 asBonusNew = 0;
        s32 asBonusOld = 0;
        if(value != SKILL_SENTINEL_VALUE)
        {
            //NOTE: The switch is technically foldable, since AS_* and SK_* match in integer value.
            //      But it's not enforceable or checkable at compile-time. So to avoid future bugs 
            //      by changing the order/quantity of enum value, and for clarity, we'll keep the switch.
            //      Also, the switch protects from invalid SK_* values, by keeping the new/old bonus to 0.
            switch(skCat)
            {
                case SK_STR: { asBonusNew = bonusNew[AS_STR]; asBonusOld = bonusOld[AS_STR]; } break;
                case SK_DEX: { asBonusNew = bonusNew[AS_DEX]; asBonusOld = bonusOld[AS_DEX]; } break;
                case SK_CON: { asBonusNew = bonusNew[AS_CON]; asBonusOld = bonusOld[AS_CON]; } break;
                case SK_INT: { asBonusNew = bonusNew[AS_INT]; asBonusOld = bonusOld[AS_INT]; } break;
                case SK_WIS: { asBonusNew = bonusNew[AS_WIS]; asBonusOld = bonusOld[AS_WIS]; } break;
                case SK_CHA: { asBonusNew = bonusNew[AS_CHA]; asBonusOld = bonusOld[AS_CHA]; } break;
            }
            
            //NOTE: Calculate Status Effect on Skill Modifier
            s32 statusEffect = 0;
            if(status)
            {
                for(s32 i = 0; i < STATUS_COUNT; i++)
                {
                    if(!status[i].check.isActive) { continue; }
                    
                    switch(status[i].type)
                    {
                        case STATUS_ABBAGLIATO:
                        {
                            if(skType == SkillType::Percezione) { statusEffect += -1; }
                        } break;
                        
                        case STATUS_ACCECATO:
                        {
                            if(skCat == SK_STR || skCat == SK_DEX) { statusEffect += -4; }
                        } break;
                        
                        case STATUS_AFFASCINATO:
                        {
                            //TODO: -4 to `Reaction` skill checks (like Perception)
                            //      I'd prefer to modify the entire skill row itself to show both values?
                            //Assert(FALSE);
                        } break;
                        
                        case STATUS_INFERMO:
                        case STATUS_PANICO:
                        case STATUS_SPAVENTATO:
                        case STATUS_SCOSSO: { statusEffect += -2; } break;
                    }
                }
            }
            
            //NOTE: Finally we modify the value based on these changes
            value  = value - asBonusOld + asBonusNew;
            value += statusEffect;
        }
        
        if(isInParen)
        {
            if(value == SKILL_SENTINEL_VALUE)
            {
                if(wasInParen) { ls_utf32Append(&page->skills, ls_utf32Constant(U", ")); }
                else           { ls_utf32Append(&page->skills, ls_utf32Constant(U" (")); }
                ls_utf32Append(&page->skills, ls_utf32Constant(name));
            }
            else
            {
                if(wasInParen) { ls_utf32Append(&page->skills, ls_utf32Constant(U", ")); }
                else           { ls_utf32Append(&page->skills, ls_utf32Constant(U" (")); }
                
                if(value >= 0) { ls_utf32AppendChar(&page->skills, '+');}
                ls_utf32AppendInt(&page->skills, value);
                ls_utf32AppendChar(&page->skills, ' ');
                ls_utf32Append(&page->skills, ls_utf32Constant(name));
            }
            
            wasInParen = TRUE;
        }
        else
        {
            if(wasInParen)               { ls_utf32Append(&page->skills, ls_utf32Constant(U"), ")); }
            else if(firstEntry == FALSE) { ls_utf32Append(&page->skills, ls_utf32Constant(U", "));  }
            
            ls_utf32Append(&page->skills, ls_utf32Constant(name));
            ls_utf32AppendChar(&page->skills, ' ');
            if(value >= 0) { ls_utf32AppendChar(&page->skills, '+');}
            ls_utf32AppendInt(&page->skills, value);
            
            wasInParen = FALSE;
            prevCat = skCat;
        }
        
        firstEntry = FALSE;
        i += 1;
    }
}

//TODO: Legacy, kept for NPCs momentarily
s32 BuildSkillFromPackedOld_t(u32 *entries, s32 index, utf32 *tmp)
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
            
            while(nextParen == TRUE)
            {
                AssertMsgF(index < 24, "Iterated too many times while searching for paren skills");
                
                index += 1;
                u32 nextEntry = entries[index];
                nextParen = (entries[index+1] & PAREN_BIT_U32) != 0;
                
                const char32_t *nextName = SkillTypeToName[(nextEntry & SKILL_TYPE_MASK)];
                s32 nextValue = (s32)((s8)(nextEntry >> SKILL_BITS));
                
                if(nextValue == SKILL_SENTINEL_VALUE)
                {
                    ls_utf32Append(tmp, ls_utf32Constant(nextName));
                    if(index < 23 && nextParen) ls_utf32Append(tmp, ls_utf32Constant(U", "));
                }
                else
                {
                    if(nextValue >= 0) { ls_utf32AppendChar(tmp, '+');}
                    ls_utf32AppendInt(tmp, nextValue);
                    ls_utf32AppendChar(tmp, ' ');
                    ls_utf32Append(tmp, ls_utf32Constant(nextName));
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

u32 ChangeBonusToSkillIfMatching(u32 entry, SkillType skType, s32 bonusChange)
{
    //NOTE: We cannot and do not touch interned skills.
    if((entry & INTERN_BIT_U32) != 0) { return entry; }
    
    AssertMsgF(skType < SkillType::SkillTypeCount, "Skill Type %d outside of range\n", skType);
    
    SkillType entryType = SkillType(entry & SKILL_TYPE_MASK);
    if(entryType == skType)
    {
        u32 newEntry = entry;
        
        //NOTE: Extract the old value
        s32 value = (s32)((s8)(entry >> SKILL_BITS));
        s32 newValue = value + bonusChange;
        
        //NOTE: Clear the old value from new entry
        newEntry &= SKILL_INV_VALUE_MASK;
        
        //NOTE: Set new value in new entry
        newEntry |= ((newValue << SKILL_BITS) & SKILL_VALUE_MASK);
        
        return newEntry;
    }
    
    return entry;
}

u32 ChangeBonusToSkill(u32 entry, s32 bonusChange)
{
    //NOTE: We cannot and do not touch interned skills.
    if((entry & INTERN_BIT_U32) != 0) { return entry; }
    
    u32 newEntry = entry;
    
    //NOTE: Extract the old value
    s32 value = (s32)((s8)(entry >> SKILL_BITS));
    s32 newValue = value + bonusChange;
    
    //NOTE: Clear the old value from new entry
    newEntry &= SKILL_INV_VALUE_MASK;
    
    //NOTE: Set new value in new entry
    newEntry |= ((newValue << SKILL_BITS) & SKILL_VALUE_MASK);
    
    return newEntry;
}