void AppendSimpleAlignment(u16 align, utf32 *out)
{
    AssertMsg(out, "Null out utf32 pointer\n");
    AssertMsg(align != 0, "Empty alignment passed\n");
    
    switch(align)
    {
        case Alignment_LG: { ls_utf32Append(out, U"LB"); } break;
        case Alignment_LN: { ls_utf32Append(out, U"LN"); } break;
        case Alignment_LE: { ls_utf32Append(out, U"LM"); } break;
        
        case Alignment_NG: { ls_utf32Append(out, U"NB"); } break;
        case Alignment_NN: { ls_utf32Append(out, U"N"); } break;
        case Alignment_NE: { ls_utf32Append(out, U"NM"); } break;
        
        case Alignment_CG: { ls_utf32Append(out, U"CB"); } break;
        case Alignment_CN: { ls_utf32Append(out, U"CN"); } break;
        case Alignment_CE: { ls_utf32Append(out, U"CM"); } break;
    }
}

void BuildAlignmentFromPacked_t(u16 entry, utf32 *out)
{
    b32 seeBelow = (entry & ALIGNMENT_SEE_BELOW) != 0;
    
    if((entry & ALIGNMENT_ANY) != 0) { 
        ls_utf32Set(out, U"Qualsiasi"_W);
        if(seeBelow) { ls_utf32Append(out, U" (vedi sotto)"); }
        return;
    }
    
    if((entry & ALIGNMENT_LIKE_CREATOR) != 0) {
        ls_utf32Set(out, U"Lo stesso del suo creatore"_W);
        if(seeBelow) { ls_utf32Append(out, U" (vedi sotto)"); }
        return;
    }
    
    if((entry & ALIGNMENT_OR) != 0)
    {
        u16 align1 = (entry & 0x00F0) >> ALIGNMENT_BIT_SIZE;
        u16 align2 = (entry & 0x000F);
        
        AppendSimpleAlignment(align1, out);
        ls_utf32Append(out, U" o ");
        AppendSimpleAlignment(align2, out);
        
        if(seeBelow) { ls_utf32Append(out, U" (vedi sotto)"); }
        return;
    }
    
    AppendSimpleAlignment((entry & 0x000F), out);
    if(seeBelow) { ls_utf32Append(out, U" (vedi sotto)"); }
    return;
}