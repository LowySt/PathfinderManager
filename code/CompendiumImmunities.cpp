void BuildImmunityFromPacked_t(u64 val, utf32 *out)
{
    if(val == IMM_SENTINEL_VALUE) { return; }
    
    for(s32 i = 0; i < IMM_MAX_COUNT; i++)
    {
        u64 singleImmunity = (val >> i*IMM_SINGLE_BIT_LEN) & 0x00000000000000FF;
        
        if(singleImmunity == Immunity_Invalid) { return; }
        AssertMsgF(singleImmunity < Immunity_MaxCount, "Super Invalid single immunity (%ld) value", singleImmunity);
        
        if(i > 0) { ls_utf32Append(out, U", "_W); }
        ls_utf32Append(out, immunityToString[singleImmunity]);
    }
    
    return;
}