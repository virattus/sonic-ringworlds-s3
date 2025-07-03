#ifndef __SSEE_SCSP_REIMP_H__
#define __SSEE_SCSP_REIMP_H__


#include "smpc.h"

#include <stdint.h>

#define SCSP_SLOT_COUNT				(32)

#ifdef SATURN
#define SCSPVP(offset)				((volatile void*)(0x25A00000 + (offset)))

//SCSP is already defined in yaul, but I don't want to risk stuff not working yet
#define SND_SCSP(offset)			(*(volatile uint16_t*)SCSPVP(offset))
#define SCSP16(offset)				SND_SCSP(offset)
#define SCSP8(offset)				(*(volatile uint8_t*)SCSPVP(offset))

#define SCSP_SREG(slot, offset)		SCSP16(0x100000 + (slot) * 0x20 + (offset))
#define SCSP_SREG_HI(slot, offset)	SCSP8(0x100000 + (slot) * 0x20 + (offset))
#define SCSP_SREG_LO(slot, offset)	SCSP8(0x100001 + (slot) * 0x20 + (offset))

#define SCSP_CREG(offset)			SCSP16(0x100400 + (offset))
#define SCSP_CREG_HI(offset)		SCSP8(0x100400 + (offset))
#define SCSP_CREG_LO(offset)		SCSP8(0x100401 + (offset))

#define SCSP_SCIEB					SCSP_CREG(0x0F << 1)
#define SCSP_SCIPD					SCSP_CREG(0x10 << 1)
#define SCSP_SCIRE					SCSP_CREG(0x11 << 1)

#define SCSP_MCIEB					SCSP_CREG(0x15 << 1)
#define SCSP_MCIPD					SCSP_CREG(0x16 << 1)
#define SCSP_MCIRE					SCSP_CREG(0x17 << 1)

#define SCSP_SCIEB_HI				SCSP_CREG_HI(0x0F << 1)
#define SCSP_SCIEB_LO				SCSP_CREG_LO(0x0F << 1)

#define SCSP_SCIPD_HI				SCSP_CREG_HI(0x10 << 1)
#define SCSP_SCIPD_LO				SCSP_CREG_LO(0x10 << 1)

#define SCSP_SCIRE_HI				SCSP_CREG_HI(0x11 << 1)
#define SCSP_SCIRE_LO				SCSP_CREG_LO(0x11 << 1)

#define SCSP_MCIEB_HI				SCSP_CREG_HI(0x15 << 1)
#define SCSP_MCIEB_LO				SCSP_CREG_LO(0x15 << 1)

#define SCSP_MCIPD_HI				SCSP_CREG_HI(0x16 << 1)
#define SCSP_MCIPD_LO				SCSP_CREG_LO(0x16 << 1)

#define SCSP_MCIRE_HI				SCSP_CREG_HI(0x17 << 1)
#define SCSP_MCIRE_LO				SCSP_CREG_LO(0x17 << 1)


static inline void WaitSampleIRQ(void)
{
	SCSP_SCIRE = 0x400;
	
	while(!(SCSP_SCIPD & 0x400));
}


static inline void SCSP_Init(void)
{
#ifndef __m68k__
	SMPC_SoundOff();
#endif
	
	SND_SCSP(0x100400) = (1 << 9);
	
	for(uint32_t slot = 0; slot < SCSP_SLOT_COUNT; slot++)
	{
		SCSP_SREG(slot, 0x00) = 0x0000;
		SCSP_SREG(slot, 0x0A) = 0x001F;
	}
	
	SCSP_SREG(0, 0x00) = (1 << 12);
	
	for(uint32_t i = 0; i < 256; i++)
	{
		WaitSampleIRQ();
	}
	
	for(uint32_t t = 0; t < 2; t++)
	{
		for(uint32_t i = 0x100000; i < 0x100400; i += 2)
		{
			SND_SCSP(i) = 0;
		}
		
		for(uint32_t i = 0x100402; i < 0x101000; i += 2)
		{
			SND_SCSP(i) = 0;
		}
		
		for(uint32_t i = 0; i < 256; i++)
		{
			WaitSampleIRQ();
		}
	}
	
	SCSP_SCIRE = 0xFFFF;
	SCSP_MCIRE = 0xFFFF;
	
#ifndef __m68k__
	for(uint32_t i = 0x25A00000; i < 0x25A80000; i += 2)
	{
		*(volatile uint16_t*)i = 0;
	}
#endif //__m68k__
}


#include "dsp_macros.h"


static volatile uint64_t* const MPROG = (volatile uint64_t*)SCSPVP(0x100800); //128?
static volatile uint16_t* const MADRS = (volatile uint16_t*)SCSPVP(0x100780); //32?
static volatile uint16_t* const COEF = (volatile uint16_t*)SCSPVP(0x100700); //64?

static volatile uint32_t* const TEMP = (volatile uint32_t*)SCSPVP(0x100C00); //128?
static volatile uint32_t* const MEMS = (volatile uint32_t*)SCSPVP(0x100E00); //32?
static volatile uint16_t* const EFREG = (volatile uint16_t*)SCSPVP(0x100EC0); //16?

#define DSP_MAKE_MEMS(v)		(((uint32_t)(v & 0xFF) << 16) | (((v) >> 8) & 0xFFFF))
#define DSP_MAKE_COEF(v)		((v) << 3)

#endif //SATURN

#endif //__SSEE_SCSP_REIMP_H__
