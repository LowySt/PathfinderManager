b32 SearchAtLeastResistanceFromString(utf32 in, u16 inType, u16 inVal)
{
    s32 foundIdx = -1;
    s32 beginValIdx = -1;
    
    switch(inType)
    {
        case RES_ACID_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Acido "_W); beginValIdx = foundIdx + 6; } break;
        case RES_FIRE_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Fuoco "_W); beginValIdx = foundIdx + 6; } break;
        case RES_COLD_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Freddo "_W); beginValIdx = foundIdx + 7; } break;
        case RES_ELEC_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Elettricit\U000000E0 "_W); beginValIdx = foundIdx + 12; } break;
        case RES_SOUND_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Sonoro "_W); beginValIdx = foundIdx + 7; } break;
        case RES_POS_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Energia Positiva "_W); beginValIdx = foundIdx + 17; } break;
        case RES_NEG_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Energia Negativa "_W); beginValIdx = foundIdx + 17; } break;
        case RES_CHAN_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Resistenza ad Incanalare "_W); beginValIdx = foundIdx + 25; } break;
        case RES_ADAP_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Resistenza Adattiva "_W); beginValIdx = foundIdx + 20; } break;
        case RES_ALL_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Energia (Tutte) "_W); beginValIdx = foundIdx + 16; } break;
        case RES_POS_50_TYPE:
        { return FALSE; } break;
        case RES_PSN_TYPE:
        { return FALSE; } break;
        case RES_SNAKESKIN_TYPE:
        { return FALSE; } break;
        
        default:
        {
            AssertMsgF(FALSE, "Invalid Resistance Type: %d\n", inType);
            return FALSE;
        }
    }
    
    if(foundIdx == -1) { return FALSE; }
    s32 endValIdx = ls_utf32LeftFind(in, foundIdx, ',');
    s32 uncommad = -1;
    if(endValIdx == -1) { endValIdx = in.len-1; uncommad = endValIdx; }
    else                { uncommad = endValIdx - 1; }
    
    s32 val = ls_utf32ToInt({in.data + beginValIdx, uncommad-beginValIdx+1, uncommad-beginValIdx+1});
    
    if(val >= inVal) { return TRUE; }
    
    return FALSE;
}

b32 SearchLessThanResistanceFromString(utf32 in, u16 inType, u16 inVal, utf32 *found)
{
    s32 foundIdx = -1;
    s32 beginValIdx = -1;
    
    switch(inType)
    {
        case RES_ACID_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Acido "_W); beginValIdx = foundIdx + 6; } break;
        case RES_FIRE_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Fuoco "_W); beginValIdx = foundIdx + 6; } break;
        case RES_COLD_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Freddo "_W); beginValIdx = foundIdx + 7; } break;
        case RES_ELEC_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Elettricit\U000000E0 "_W); beginValIdx = foundIdx + 12; } break;
        case RES_SOUND_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Sonoro "_W); beginValIdx = foundIdx + 7; } break;
        case RES_POS_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Energia Positiva "_W); beginValIdx = foundIdx + 17; } break;
        case RES_NEG_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Energia Negativa "_W); beginValIdx = foundIdx + 17; } break;
        case RES_CHAN_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Resistenza ad Incanalare "_W); beginValIdx = foundIdx + 25; } break;
        case RES_ADAP_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Resistenza Adattiva "_W); beginValIdx = foundIdx + 20; } break;
        case RES_ALL_TYPE:
        { foundIdx = ls_utf32LeftFind(in, U"Energia (Tutte) "_W); beginValIdx = foundIdx + 16; } break;
        case RES_POS_50_TYPE:
        { return FALSE; } break;
        case RES_PSN_TYPE:
        { return FALSE; } break;
        case RES_SNAKESKIN_TYPE:
        { return FALSE; } break;
        
        default:
        {
            AssertMsgF(FALSE, "Invalid Resistance Type: %d\n", inType);
            return FALSE;
        }
    }
    
    if(foundIdx == -1) { return FALSE; }
    s32 endValIdx = ls_utf32LeftFind(in, foundIdx, ',');
    s32 uncommad = -1;
    if(endValIdx == -1) { endValIdx = in.len-1; uncommad = endValIdx; }
    else                { uncommad = endValIdx - 1; }
    
    s32 val = ls_utf32ToInt({in.data + beginValIdx, uncommad-beginValIdx+1, uncommad-beginValIdx+1});
    
    if(val < inVal) {
        ls_utf32Copy_t({in.data + foundIdx, endValIdx - foundIdx + 1, endValIdx - foundIdx + 1 }, found);
        return TRUE;
    }
    
    return FALSE;
}

void BuildSingleResistanceFromPacked_t(u16 type, u16 val, utf32 *out)
{
    switch(type)
    {
        case RES_ACID_TYPE:      { ls_utf32Append(out, U"Acido "_W); } break;
        case RES_FIRE_TYPE:      { ls_utf32Append(out, U"Fuoco "_W); } break;
        case RES_COLD_TYPE:      { ls_utf32Append(out, U"Freddo "_W); } break;
        case RES_ELEC_TYPE:      { ls_utf32Append(out, U"Elettricit\U000000E0 "_W); } break;
        case RES_SOUND_TYPE:     { ls_utf32Append(out, U"Sonoro "_W); } break;
        case RES_POS_TYPE:       { ls_utf32Append(out, U"Energia Positiva "_W); } break;
        case RES_NEG_TYPE:       { ls_utf32Append(out, U"Energia Negativa "_W); } break;
        case RES_CHAN_TYPE:      { ls_utf32Append(out, U"Resistenza ad Incanalare "_W); } break;
        case RES_ADAP_TYPE:      { ls_utf32Append(out, U"Resistenza Adattiva "_W); } break;
        case RES_ALL_TYPE:       { ls_utf32Append(out, U"Energia (Tutte) "_W); } break;
        case RES_POS_50_TYPE:    { ls_utf32Append(out, U"Energia Positiva (50%)"_W); } break;
        case RES_PSN_TYPE:       { ls_utf32Append(out, U"Resistenza al Veleno"_W); } break;
        case RES_SNAKESKIN_TYPE: { ls_utf32Append(out, U"Pelle di Serpente"_W); } break;
        
        default:
        {
            AssertMsgF(FALSE, "Invalid Resistance Type: %d\n", type);
            return;
        }
    }
    
    if(val != RES_NULL_VALUE) { ls_utf32AppendInt(out, val); }
}

void BuildResistanceFromPacked_t(u64 val, utf32 *out)
{
    if(val == RES_SENTINEL_VALUE) { return; }
    
    for(s32 i = 0; i < RES_MAX_COUNT; i++)
    {
        u16 currentRes = (val >> (i*RES_SINGLE_BIT_LEN)) & RES_SINGLE_MASK;
        if(currentRes != RES_INVALID_TYPE)
        {
            if(i != 0) { ls_utf32Append(out, U", "_W); }
            
            u16 type = currentRes & 0x3C0;
            u16 val  = currentRes & 0x3F;
            AssertMsg(type != RES_INVALID_TYPE, "Invalid Type in non-null resistance?\n");
            
            switch(type)
            {
                case RES_ACID_TYPE:      { ls_utf32Append(out, U"Acido "_W); } break;
                case RES_FIRE_TYPE:      { ls_utf32Append(out, U"Fuoco "_W); } break;
                case RES_COLD_TYPE:      { ls_utf32Append(out, U"Freddo "_W); } break;
                case RES_ELEC_TYPE:      { ls_utf32Append(out, U"Elettricit\U000000E0 "_W); } break;
                case RES_SOUND_TYPE:     { ls_utf32Append(out, U"Sonoro "_W); } break;
                case RES_POS_TYPE:       { ls_utf32Append(out, U"Energia Positiva "_W); } break;
                case RES_NEG_TYPE:       { ls_utf32Append(out, U"Energia Negativa "_W); } break;
                case RES_CHAN_TYPE:      { ls_utf32Append(out, U"Resistenza ad Incanalare "_W); } break;
                case RES_ADAP_TYPE:      { ls_utf32Append(out, U"Resistenza Adattiva "_W); } break;
                case RES_ALL_TYPE:       { ls_utf32Append(out, U"Energia (Tutte) "_W); } break;
                case RES_POS_50_TYPE:    { ls_utf32Append(out, U"Energia Positiva (50%)"_W); } break;
                case RES_PSN_TYPE:       { ls_utf32Append(out, U"Resistenza al Veleno"_W); } break;
                case RES_SNAKESKIN_TYPE: { ls_utf32Append(out, U"Pelle di Serpente"_W); } break;
                
                default:
                {
                    AssertMsgF(FALSE, "Invalid Resistance Type: %d\n", type);
                    return;
                }
            }
            
            if(val != RES_NULL_VALUE) { ls_utf32AppendInt(out, val); }
        }
    }
    
    //NOTETODO: This is kind of a shitty check to make sure we're not overwriting
    //          the cachedPage initial buffer.
    AssertMsg(out->size == RES_MAX_COUNT * 16, "Out was resized!\n");
    
    return;
}

b32 PackedContainsAtLeastResistance(u64 val, u16 inType, u16 inVal)
{
    if(val == RES_SENTINEL_VALUE) { return FALSE; }
    
    for(s32 i = 0; i < RES_MAX_COUNT; i++)
    {
        u16 currentRes = (val >> (i*RES_SINGLE_BIT_LEN)) & RES_SINGLE_MASK;
        if(currentRes != RES_INVALID_TYPE)
        {
            u16 type = currentRes & 0x3C0;
            u16 val  = currentRes & 0x3F;
            AssertMsg(type != RES_INVALID_TYPE, "Invalid Type in non-null resistance?\n");
            
            if(type == inType && val >= inVal) { return TRUE; }
        }
    }
    
    return FALSE;
}

b32 PackedContainsLessThanResistance(u64 val, u16 inType, u16 inVal, utf32 *found)
{
    if(val == RES_SENTINEL_VALUE) { return FALSE; }
    
    for(s32 i = 0; i < RES_MAX_COUNT; i++)
    {
        u16 currentRes = (val >> (i*RES_SINGLE_BIT_LEN)) & RES_SINGLE_MASK;
        if(currentRes != RES_INVALID_TYPE)
        {
            u16 type = currentRes & 0x3C0;
            u16 val  = currentRes & 0x3F;
            AssertMsg(type != RES_INVALID_TYPE, "Invalid Type in non-null resistance?\n");
            
            if(type == inType && val < inVal) {
                BuildSingleResistanceFromPacked_t(type, val, found);
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

//TODO: Now searching the packed seems irrelevant. I'm already searching the string anyway.
//     Can probably remove the packed searching and just keep the string.
void AppendResistanceStringIfNotPresent(u64 orig, u16 type, u16 atLeastVal, utf32 toAdd, utf32 *oldRes)
{
    if(!PackedContainsAtLeastResistance(orig, type, atLeastVal) &&
       !SearchAtLeastResistanceFromString(*oldRes, type, atLeastVal))
    {
        u32 buff[64] = {};
        utf32 found  = { buff, 0, 64 };
        
        if(SearchLessThanResistanceFromString(*oldRes, type, atLeastVal, &found))
        {
            ls_utf32RmSubstr(oldRes, found);
        }
        else if(PackedContainsLessThanResistance(orig, type, atLeastVal, &found))
        {
            utf32 foundWithComma = found;
            ls_utf32Append(&foundWithComma, U", "_W);
            
            if(!ls_utf32RmSubstr(oldRes, foundWithComma))
            { ls_utf32RmSubstr(oldRes, found); }
        }
        
        AssertMsg(oldRes->len + toAdd.len+2 < oldRes->size, "Insufficient space in resistances string.\n");
        if(oldRes->len != 0) { ls_utf32Append(oldRes, U", "_W); }
        ls_utf32Append(oldRes, toAdd);
    }
}