#include "soundcontrol.h"

#include <backend/cd_loader.h>
#include <backend/sound/smpc.h>
#include <backend/sound/scsp.h>
#include <backend/sound/adp68k.h>

#include <yaul.h>
#include <mic3d.h>

#include <stdbool.h>

#define SOUND_DRIVER_FILENAME	("ADP68K.BIN")
#define SOUND_DRIVER_SIZE		(0x2000)
#define SOUND_DRIVER_TEMP_LOC	(LWRAM(0)) //TODO put this in a universal scratch location


const unsigned int marker = 0xDEADBEEF;





void snd_LoadDriver(void)
{
	bool OpenedCD = false;
	if(!CD_FileListInitialised())
	{
		OpenedCD = true;
		CD_OpenFileList();
	}
	
	CD_LoadFile(SOUND_DRIVER_FILENAME, SOUND_DRIVER_TEMP_LOC);
	
	SMPC_SoundOff();
	SCSP_Init();
	
	SCSP_MCIRE = 0xFFFF;
	SCSP_SCIRE = 0xFFFF;
	
	//TODO upload driver more... better...
	uint8_t* driverData = SOUND_DRIVER_TEMP_LOC;
	
	for(uint32_t i = 0; i < SOUND_DRIVER_SIZE; i++)
	{
		SCSP8(i) = driverData[i];
	}
	
	SMPC_SoundOn();
	
	while(!(SCSP_MCIPD & 0x20));
	
	SCSP_MCIRE_LO = 0x20;
	
	if(OpenedCD)
	{
		CD_CloseFileList();
	}
}


void snd_LoadSampleData(uint32_t offset, uint32_t data_length, uint8_t* data)
{
	while(SCSP_SCIPD & 0x20);
	
	//TODO transfer data better
	for(uint32_t i = 0; i < data_length; i++)
	{
		adp68k_sdbase[offset + i] = data[i];
	}
	
	SCSP_SCIPD_LO = 0x20;
}


void snd_PlayADPCM(uint8_t slot, uint8_t sndID)
{
	while(SCSP_SCIPD & 0x20);
	volatile ADPCMChannelControl* adpcc = &adp68k_scblock->adpcm[(0 + slot) & 0x7];
	
	adpcc->id = sndID;
	adpcc->volume[0] = 0x4000;
	adpcc->volume[1] = 0x4000;
	adpcc->action = ADP68K_ACTION_PLAY;
	SCSP_SCIPD_LO = 0x20;
	
}


void snd_StopAllADPCM(void)
{
	while(SCSP_SCIPD & 0x20);

	for(uint32_t i = 0; i < ADPCM_MAX_CHANNELS; i++)
	{
		adp68k_scblock->adpcm[i].action = ADP68K_ACTION_STOP;
	}
	
	SCSP_SCIPD_LO = 0x20;
}
