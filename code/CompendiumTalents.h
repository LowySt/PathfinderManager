#ifndef _COMPENDIUM_TALENTS_H
#define _COMPENDIUM_TALENTS_H

/*NOTE: The talents are mapped in a 32 bit value as follows
*       [0|1] [0|1] [0|1] [1 UNUSED] [16 bit interned talent string] [12 bit index in talent module]
*         ^     ^     ^                   ^                                  ^
 *         |     |     | Mithic            |                                  |
*         |    Bonus                      |                                  |
*         |                               |                                  |
   *        The high bit (bit index 31)     The 16 bits in indices 27..12      The low 12 bits in indices 11..0
 *         is used to indicate wether     indicate the interned display      indicate the index in the talent module
*         the display name has been      name idx in the talents buffer     to retrieve all informations about the
 *         interned or not.               if present.                        talent (The entire talent page).
*         (0 NO, 1 YES)
*/

const u32 TALENT_INTERN_BIT_U32   = 0x80000000;
const u32 TALENT_BONUS_BIT_U32    = 0x40000000;
const u32 TALENT_MITHIC_BIT_U32   = 0x20000000;
const u32 TALENT_INTERN_IDX_SHIFT = 12;
const u32 TALENT_INTERN_MASK      = 0x0FFFF000;
const u32 TALENT_MODULE_IDX_MASK  = 0x00000FFF;

const u32 TALENT_NOT_FOUND        = 0x00000FFF;

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

struct CachedTalentEntry
{
    s32 entryIndex = -1;
    
    utf32 name;
    utf32 desc;
    utf32 pre;
    utf32 gain;
    utf32 norm;
    utf32 spec;
    utf32 source;
};

enum TalentDisplayResult
{
    TSR_NORMAL,
    TSR_AVAILABLE,
    TSR_MISSING,
};

CachedTalentEntry compendiumCachedTalent = {};
CachedTalentEntry mainCachedTalent       = {};

void InitCachedTalentEntry(CachedTalentEntry *cachedEntry);
void CacheTalentEntry(CachedTalentEntry *cachedEntry, s32 talentIndex);

void BuildTalentFromPacked_t(struct Codex *c, u32 entry, utf32 *tmp);
TalentDisplayResult CheckTalentTooltipAndClick(UIContext *c, struct CachedPageEntry *page, UILayoutRect oldLayout,
                                               utf32 talent, s32 talentEntry);

u32 GetTalentPackedFromName(utf32 talentName, b32 isBonus = FALSE, b32 isMithic = FALSE);

s32 DrawTalentPage(UIContext *c, CachedTalentEntry *entry, s32 baseX, s32 baseY, s32 width, s32 minY);

#endif //_COMPENDIUM_TALENTS_H
