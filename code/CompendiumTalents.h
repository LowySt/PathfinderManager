#ifndef _COMPENDIUM_TALENTS_H
#define _COMPENDIUM_TALENTS_H

const u32 TALENT_INTERN_BIT_U32   = 0x80000000;
const u32 TALENT_BONUS_BIT_U32    = 0x40000000;
const u32 TALENT_MITHIC_BIT_U32   = 0x20000000;
const u32 TALENT_INTERN_IDX_SHIFT = 12;
const u32 TALENT_INTERN_MASK      = 0x0FFFF000;
const u32 TALENT_MODULE_IDX_MASK  = 0x00000FFF;

struct TalentEntry
{
    u32 name;
    u32 desc;
    u32 pre;
    u32 gain;
    u32 norm;
    u32 spec;
    u32 source;
};

void BuildTalentFromPacked_t(struct Codex *c, u32 entry, utf32 *tmp);

#endif //_COMPENDIUM_TALENTS_H
