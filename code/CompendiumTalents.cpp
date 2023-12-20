void BuildTalentFromPacked_t(Codex *c, u32 entry, utf32 *tmp)
{
    u32 tempUTF32Buffer[256] = {};
    utf32 tempString = { tempUTF32Buffer, 0, 256 };
    
    b32 isMithic   = (entry & TALENT_MITHIC_BIT_U32) != 0;
    b32 isBonus    = (entry & TALENT_BONUS_BIT_U32)  != 0;
    b32 isInterned = (entry & TALENT_INTERN_BIT_U32) != 0;
    
    if(isInterned)
    {
        u16 internedIdx = (u16)((entry & TALENT_INTERN_MASK) >> TALENT_INTERN_IDX_SHIFT);
        GetEntryFromBuffer_t(&c->talents, &tempString, internedIdx, "talents");
    }
    else
    {
        u32 moduleIdx       = entry & TALENT_MODULE_IDX_MASK;
        u32 nameIdxInModule = c->talentPages[moduleIdx].name;
        GetEntryFromBuffer_t(&c->talentsModule, &tempString, nameIdxInModule, "talentsMod");
    }
    
    if(isMithic && isBonus)
    {
        s32 bracketIdx = ls_utf32LeftFind(tempString, '(');
        s32 BMpos = bracketIdx == -1 ? tempString.len : bracketIdx-1;
        
        ls_utf32InsertCStr(&tempString, "[B, M]", BMpos);
    }
    else if(isBonus)
    {
        s32 bracketIdx = ls_utf32LeftFind(tempString, '(');
        s32 Bpos = bracketIdx == -1 ? tempString.len : bracketIdx-1;
        
        ls_utf32InsertCStr(&tempString, "[B]", Bpos);
    }
    else if(isMithic)
    {
        s32 bracketIdx = ls_utf32LeftFind(tempString, '(');
        s32 Mpos = bracketIdx == -1 ? tempString.len : bracketIdx-1;
        
        ls_utf32InsertCStr(&tempString, "[M]", Mpos);
    }
    
    ls_utf32Set(tmp, tempString);
    
    return;
}