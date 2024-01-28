#ifndef _COMPENDIUM_RESISTANCE_H
#define _COMPENDIUM_RESISTANCE_H

const u64 RES_SENTINEL_VALUE = 0xFFFFFFFFFFFFFFFF;
const s32 RES_SINGLE_BIT_LEN = 10;
const s32 RES_SINGLE_MASK    = 0x3FF;
const s32 RES_MAX_COUNT      = 6;

const u16 RES_INVALID_TYPE   = 0x0000;
const u16 RES_NULL_VALUE     = 0x0000;

const u16 RES_ACID_TYPE      = 0x0040;
const u16 RES_FIRE_TYPE      = 0x0080;
const u16 RES_COLD_TYPE      = 0x00C0;
const u16 RES_ELEC_TYPE      = 0x0100;
const u16 RES_SOUND_TYPE     = 0x0140;
const u16 RES_POS_TYPE       = 0x0180;
const u16 RES_NEG_TYPE       = 0x01C0;
const u16 RES_CHAN_TYPE      = 0x0200;
const u16 RES_ADAP_TYPE      = 0x0240;
const u16 RES_ALL_TYPE       = 0x0280;
const u16 RES_POS_50_TYPE    = 0x02C0;
const u16 RES_PSN_TYPE       = 0x0300;
const u16 RES_SNAKESKIN_TYPE = 0x0340;

const u16 RES_MAX_VALUE      = 0x3F;

void BuildResistanceFromPacked_t(u64 val, utf32 *);

#endif //_COMPENDIUM_RESISTANCE_H
