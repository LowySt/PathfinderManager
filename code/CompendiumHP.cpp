void BuildHPFromPacked_t(CachedPageEntry *page, u64 entry, s32 totalHP)
{
    utf32 *hp = &page->HP;
    ls_utf32AppendInt(hp, totalHP);
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