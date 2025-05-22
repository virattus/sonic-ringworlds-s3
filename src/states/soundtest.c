#include "soundtest.h"


#include <backend/ponesound.h>

#include <yaul.h>
#include <mic3d.h>


extern uint8_t asset_sound_driver[];
extern uint8_t asset_sound_driver_end[];
extern uint8_t asset_yaul_pcm8[];
extern uint8_t asset_yaul_pcm8_end[];


#define PCM_NUMBER (0)
#define PCM_LOAD_OFFSET (0x408 + DRV_SYS_END + 0x20)

#define WAIT_TIME (FRAME_RATE)

static smpc_peripheral_digital_t _digital;

static bool playingSound = false;
static int counter = 0;


void SoundTestState_Init(void)
{
	//Loading binary blob driver to SMPC
	ponesound_load_driver(asset_sound_driver, asset_sound_driver_end - asset_sound_driver, ADX_MASTER_768);
	
	ponesound_load_8bit_pcm(asset_yaul_pcm8, asset_yaul_pcm8_end - asset_yaul_pcm8, PCM_NUMBER, PCM_LOAD_OFFSET, 15360);
	
	ponesound_set_master_volume(0xF);
	
	vdp_sync_vblank_in_set(ponesound_vblank_rq, NULL);	
}


void SoundTestState_Free(void)
{
}


void SoundTestState_Update(void)
{
	if(!playingSound)
	{
		dbgio_printf("Press A to start 8-bit PCM\r\n");
	
		smpc_peripheral_process();
		smpc_peripheral_digital_port(1, &_digital);
		
		if(_digital.held.button.a != 0)
		{			
			ponesound_pcm_play(PCM_NUMBER, PCM_PROTECTED, 6);
			playingSound = true;
		}
	}
	else
	{
		dbgio_printf("\r\nStarting playback...\n");
		
		counter += 1;
		if(counter >= WAIT_TIME)
		{
			playingSound = false;
			counter = 0;
		}
	}
}



gamestate_t Get_SoundTestState(void)
{
	return (gamestate_t)
	{
		.InitState = SoundTestState_Init,
		.FreeState = SoundTestState_Free,
		.UpdateState = SoundTestState_Update,
	};
}
