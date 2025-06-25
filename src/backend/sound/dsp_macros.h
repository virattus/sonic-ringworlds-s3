#ifndef __SSEE_DSP_MACRO_REIMP_H__
#define __SSEE_DSP_MACRO_REIMP_H__


#include <stdint.h>

#define DSP_NXADDR		((uint64_t)(         1) <<  0)
#define DSP_ADRGB		((uint64_t)(         1) <<  1)
#define DSP_MASA(v)		((uint64_t)((v) & 0x1F) <<  2)
#define DSP_NOFL		((uint64_t)(         1) <<  8)
#define DSP_CRA(v)		((uint64_t)((v) & 0x1F) <<  9)
#define DSP_BSEL(v)		((uint64_t)((v) & 0x1F) << 16)
#define DSP_ZERO		((uint64_t)(         1) << 17)
#define DSP_NEGB		((uint64_t)(         1) << 18)
#define DSP_YRL			((uint64_t)(         1) << 19)
#define DSP_SHFT0		((uint64_t)(         1) << 20)
#define DSP_SHFT1		((uint64_t)(         1) << 21)
#define DSP_FRCL		((uint64_t)(         1) << 22)
#define DSP_ADLR		((uint64_t)(         1) << 23)
#define DSP_EWA(v)		((uint64_t)((v) & 0x1F) << 24)
#define DSP_EWT			((uint64_t)(         1) << 28)
#define DSP_MRT			((uint64_t)(         1) << 29)
#define DSP_MWT			((uint64_t)(         1) << 30)
#define DSP_TABLE		((uint64_t)(         1) << 31)
#define DSP_IWA(v)		((uint64_t)((v) & 0x1F) << 32)
#define DSP_IWT			((uint64_t)(         1) << 37)
#define DSP_IRA(v)		((uint64_t)((v) & 0x1F) << 38)
#define DSP_YSEL(v)		((uint64_t)((v) & 0x1F) << 45)
#define DSP_XSEL(v)		((uint64_t)((v) & 0x1F) << 47)
#define DSP_TWA(v)		((uint64_t)((v) & 0x1F) << 48)
#define DSP_TWT			((uint64_t)(         1) << 55)
#define DSP_TRA(v)		((uint64_t)((v) & 0x1F) << 56)


#endif
