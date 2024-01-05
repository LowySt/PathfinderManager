#ifndef _COMPENDIUM_ALIGNMENT_H
#define _COMPENDIUM_ALIGNMENT_H


const u16 ALIGNMENT_SEE_BELOW    = 0x8000;
const u16 ALIGNMENT_LIKE_CREATOR = 0x4000;
const u16 ALIGNMENT_ANY          = 0x2000;
const u16 ALIGNMENT_OR           = 0x1000;

const u16 ALIGNMENT_BIT_SIZE     = 4;

enum AlignmentType
{
    AlignmentEmpty = 0,
    
    Alignment_LG,
    Alignment_LN,
    Alignment_LE,
    
    Alignment_NG,
    Alignment_NN,
    Alignment_NE,
    
    Alignment_CG,
    Alignment_CN,
    Alignment_CE,
};

void BuildAlignmentFromPacked_t(u16 val, utf32 *);

#endif //_COMPENDIUM_ALIGNMENT_H
