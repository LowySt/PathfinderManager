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
    
    return;
}