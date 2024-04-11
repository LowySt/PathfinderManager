void BuildHPFromPacked_t(CachedPageEntry *page, u64 entry, s32 totalHP)
{
    utf32 *hp = &page->HP;
    ls_utf32AppendInt(hp, totalHP);
    
    //NOTE: We save the HP string here to have it available when Adding Mobs from the Compendium
    page->totHP = page->HP;
    
    ls_utf32Append(hp, U" ("_W);
    
    s32 totHpStringLen = hp->len;
    utf32 without_ira = {};
    without_ira.data = hp->data;
    without_ira.size = hp->size;
    
    s32 totalHitDice = 0;
    for(s32 i = 0; i < HP_MAX_DICE_COUNT; i++)
    {
        u16 die = ((entry >> (i*HP_DIE_BITLEN)) & HP_DIE_MASK);
        
        if(die != 0)
        {
            u16 count = (die & HP_DIE_COUNT_MASK) >> HP_DIE_FACE_BITLEN;
            u16 face  = (die & HP_DIE_FACE_MASK);
            
            if(i > 0) { ls_utf32Append(hp, U"+"_W); }
            ls_utf32AppendInt(hp, count);
            ls_utf32Append(hp, dieFaces[face]);
            
            totalHitDice += count;
        }
        else { break; }
    }
    
    if((entry & HP_FLAT_NEGATIVE_BIT) == 0)
    { ls_utf32Append(hp, U"+"_W); }
    else
    { ls_utf32Append(hp, U"-"_W); }
    
    s32 flatIraDifference = totalHitDice*4;
    if((entry & HP_IRA_BIT) != 0)
    { without_ira.len = hp->len - totHpStringLen; }
    
    u16 flat = (entry & HP_FLAT_MASK) >> HP_FLAT_OFFSET;
    ls_utf32AppendInt(hp, flat);
    
    u16 optionType = (entry & HP_OPTION_TYPE_MASK) >> HP_OPTION_TYPE_OFFSET;
    if(optionType == HP_Vita_Falsata)
    {
        ls_utf32Append(hp, U" pi\U000000F9 "_W);
        u16 optionValue = (entry & HP_OPTION_VAL_MASK) >> HP_OPTION_VAL_OFFSET;
        ls_utf32AppendInt(hp, optionValue);
        ls_utf32Append(hp, HpTypeToString[optionType]);
        return;
    }
    
    ls_utf32Append(hp, U")"_W);
    
    if(optionType != HP_Invalid)
    {
        ls_utf32Append(hp, U"; "_W); 
        
        if((optionType >= HP_Guarigione_Rapida) && (optionType <= HP_Guarigione_Rapida_Ombre))
        {
            ls_utf32Append(hp, U"Guarigione Rapida "_W);
            
            u16 optionValue = (entry & HP_OPTION_VAL_MASK) >> HP_OPTION_VAL_OFFSET;
            if(optionValue > 0) { ls_utf32AppendInt(hp, optionValue); ls_utf32Append(hp, U" "_W); }
            
            ls_utf32Append(hp, HpTypeToString[optionType]);
        }
        else if((optionType >= HP_Rigenerazione) && (optionType <= HP_Rigenerazione_Speciale))
        {
            ls_utf32Append(hp, U"Rigenerazione "_W);
            
            u16 optionValue = (entry & HP_OPTION_VAL_MASK) >> HP_OPTION_VAL_OFFSET;
            ls_utf32AppendInt(hp, optionValue);
            ls_utf32Append(hp, U"/"_W);
            
            ls_utf32Append(hp, HpTypeToString[optionType]);
        }
        else
        {
            switch(optionType)
            {
                case HP_Regen_Pazuzu:
                case HP_Guarigione_Sangue:
                case HP_Raccolto_Dolore:
                case HP_Estorsione:
                { ls_utf32Append(hp, HpTypeToString[optionType]); } break;
                
                case HP_Terribile_Ringiovanimento:
                case HP_Ripararsi_Con_Rifiuti:
                case HP_Guarigione_Rapida_Planare:
                case HP_Gioia_Sostenente:
                {
                    ls_utf32Append(hp, HpTypeToString[optionType]);
                    u16 optionValue = (entry & HP_OPTION_VAL_MASK) >> HP_OPTION_VAL_OFFSET;
                    ls_utf32AppendInt(hp, optionValue);
                } break;
                
                case HP_Rigenerazione_Cera_Fuoco:
                {
                    ls_utf32Append(hp, U"Rigenerazione della Cera "_W);
                    
                    u16 optionValue = (entry & HP_OPTION_VAL_MASK) >> HP_OPTION_VAL_OFFSET;
                    ls_utf32AppendInt(hp, optionValue);
                    ls_utf32Append(hp, U"/"_W);
                    
                    ls_utf32Append(hp, HpTypeToString[optionType]);
                } break;
                
                case HP_Rigenerazione_Pluviale_Acido_Fuoco:
                {
                    ls_utf32Append(hp, U"Rigenerazione Pluviale "_W);
                    
                    u16 optionValue = (entry & HP_OPTION_VAL_MASK) >> HP_OPTION_VAL_OFFSET;
                    ls_utf32AppendInt(hp, optionValue);
                    ls_utf32Append(hp, U"/"_W);
                    
                    ls_utf32Append(hp, HpTypeToString[optionType]);
                } break;
                
                case HP_Rigenerazione_Profana_Bene_Elettricita:
                {
                    ls_utf32Append(hp, U"Rigenerazione Profana "_W);
                    
                    u16 optionValue = (entry & HP_OPTION_VAL_MASK) >> HP_OPTION_VAL_OFFSET;
                    ls_utf32AppendInt(hp, optionValue);
                    ls_utf32Append(hp, U"/"_W);
                    
                    ls_utf32Append(hp, HpTypeToString[optionType]);
                } break;
                
                case HP_Divorare_Anima:
                {
                    ls_utf32Append(hp, U"Guarigione Rapida "_W);
                    
                    u16 optionValue = (entry & HP_OPTION_VAL_MASK) >> HP_OPTION_VAL_OFFSET;
                    ls_utf32AppendInt(hp, optionValue);
                    ls_utf32Append(hp, HpTypeToString[optionType]);
                } break;
            }
        }
    }
    
    if((entry & HP_IRA_BIT) != 0)
    {
        ls_utf32Append(hp, U"\nPF non in ira: "_W);
        ls_utf32AppendInt(hp, totalHP-flatIraDifference);
        ls_utf32Append(hp, U" ("_W);
        ls_utf32Append(hp, without_ira);
        
        u16 flat = (entry & HP_FLAT_MASK) >> HP_FLAT_OFFSET;
        ls_utf32AppendInt(hp, flat-flatIraDifference);
    }
}

u64 ChangeRacialDVType(u64 oldHP, HP_Die_Face newType)
{
    //NOTE: Racial DVs are the very first (low 3 bits) of the packed value
    u64 newHP = oldHP & 0xFFFFFFFFFFFFFFF8;
    newHP |= (u64) newType;
    return newHP;
}

u64 ClearAllDVsExceptRacial(u64 oldHP)
{
    //NOTE: Racial DVs are the very first (low 9 bits) of the packed value.
    u64 newHP = oldHP & 0xFFFFFFF0000001FF;
    return newHP;
}

u64 ReplaceRacialDVCount(u64 oldHP, u16 newCount)
{
    //NOTE: Racial DVs are the very first (low 9 bits) of the packed value,
    //      And the count is stored in the 6 bits in indices [3..8]
    u64 newHP = oldHP | (u64)(newCount << HP_DIE_FACE_BITLEN);
    return newHP;
}

u64 ChangeRacialDVCount(u64 oldHP, s32 diff)
{
    //NOTE: Racial DVs are the very first (low 9 bits) of the packed value,
    //      The count is stored in the 6 bits in indices [3..8]
    u16 oldRaceDV = (oldHP >> ((HP_MAX_DICE_COUNT-1)*HP_DIE_BITLEN)) & HP_DIE_MASK;
    u16 oldCount  = (oldRaceDV & HP_DIE_COUNT_MASK) >> HP_DIE_FACE_BITLEN;
    
    if(diff < 0 && oldCount < (diff*-1))
    { AssertMsg(FALSE, "Can't make racial dv count smaller than zero\n"); return oldHP; }
    
    u16 newCount = oldCount + diff;
    
    //NOTE: Racial DVs are the very first (low 9 bits) of the packed value,
    //      And the count is stored in the 6 bits in indices [3..8]
    u64 newHP = oldHP | (u64)(newCount << HP_DIE_FACE_BITLEN);
    return newHP;
}

u64 ChangeRacialDVTypeAndCount(u64 oldHP, HP_Die_Face newType, s32 diff)
{
    //NOTE: Racial DVs are the very first (low 9 bits) of the packed value,
    //      The type is the lowest 3 bits, the count is stored in the 6 bits in indices [3..8]
    u16 oldRaceDV = (oldHP >> ((HP_MAX_DICE_COUNT-1)*HP_DIE_BITLEN)) & HP_DIE_MASK;
    u16 oldCount  = (oldRaceDV & HP_DIE_COUNT_MASK) >> HP_DIE_FACE_BITLEN;
    
    if(diff < 0 && oldCount < (diff*-1))
    { AssertMsg(FALSE, "Can't make racial dv count smaller than zero\n"); return oldHP; }
    
    u16 newCount = oldCount + diff;
    u64 newHP = oldHP & 0xFFFFFFFFFFFFFE00;
    newHP |= (u64) newType;
    newHP |= (u64)(newCount << HP_DIE_FACE_BITLEN);
    return newHP;
}

u64 AddHPOptionIfMissing(u64 oldHP, HP_Options_Type option, u16 optionVal)
{
    //NOTE: First we check if the oldHP already has an option
    u8 currentOption = (oldHP & HP_OPTION_TYPE_MASK) >> HP_OPTION_TYPE_OFFSET;
    
    if(currentOption == HP_Options_Type::HP_Invalid)
    {
        //NOTE: There's currently no option set, so we can easily add a new one.
        u64 optionAdd = (u64)option << HP_OPTION_TYPE_OFFSET;
        u64 optionAddVal = (u64)optionVal << HP_OPTION_VAL_OFFSET;
        u64 newHP = oldHP & HP_OPTION_INV_MASK;
        newHP |= optionAdd;
        newHP |= optionAddVal;
        return newHP;
    }
    else
    {
        //TODO: Gotta finish this
        TODO;
        return oldHP;
    }
}