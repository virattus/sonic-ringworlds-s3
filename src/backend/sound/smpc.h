#ifndef __SSEE_SMPC_REIMP_H__
#define __SSEE_SMPC_REIMP_H__


#include <stdint.h>
#include <assert.h>


#define SMPC_COMREG			(*(volatile uint8_t*)0x2010001F)
#define SMPC_SR				(*(volatile uint8_t*)0x20100061)
#define SMPC_SF				(*(volatile uint8_t*)0x20100063)


#define SMPC_CMD_SNDON		0x06
#define SMPC_CMD_SNDOFF		0x07
#define SMPC_CMD_CDON		0x08
#define SMPC_CMD_CDOFF		0x09


static inline void SMPC_SoundOn(void)
{
	while(SMPC_SF & 0x1);
	
	SMPC_SF = 0x1;
	SMPC_COMREG = SMPC_CMD_SNDON;
	
	while(SMPC_SF & 0x1);
}


static inline void SMPC_SoundOff(void)
{
	while(SMPC_SF & 0x1);
	
	SMPC_SF = 0x1;
	SMPC_COMREG = SMPC_CMD_SNDOFF;
	
	while(SMPC_SF & 0x1);
}


static inline void SMPC_CDOn(void)
{
	while(SMPC_SF & 0x1);
	
	SMPC_SF = 0x1;
	SMPC_COMREG = SMPC_CMD_CDON;
	
	while(SMPC_SF & 0x1);
}


static inline void SMPC_CDOff(void)
{
	while(SMPC_SF & 0x1);
	
	SMPC_SF = 0x1;
	SMPC_COMREG = SMPC_CMD_CDOFF;
	
	while(SMPC_SF & 0x1);
}


static inline void SMPC_SSHOn(void)
{
	assert(0);
}


static inline void SMPC_SSHOff(void)
{
	assert(0);
}

#endif
