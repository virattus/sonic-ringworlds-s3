#include "slot.h"


/*
#include <backend/buhman_scsp.h>
#include <backend/buhman_smpc.h>
*/

#include <yaul.h>
#include <mic3d.h>


extern uint8_t asset_sound_driver[];
extern uint8_t asset_sound_driver_end[];
extern uint8_t asset_yaul_pcm8[];
extern uint8_t asset_yaul_pcm8_end[];
extern uint8_t asset_segascr_pcm8[];
extern uint8_t asset_segascr_pcm8_end[];
extern uint8_t asset_ring_collect_pcm8[];
extern uint8_t asset_ring_collect_pcm8_end[];


#define PCM_YAUL (0)
#define PCM_SEGA (1)
#define PCM_THIRD (2)
#define PCM_LOAD_OFFSET (0x408 + DRV_SYS_END + 0x20)

#define WAIT_TIME (1)

static int PCMAdditionalOffset = 0;

static smpc_peripheral_digital_t _digital;

static bool playingSound = false;
static int counter = 0;


void SlotSoundTestState_Init(void)
{
/*
	while ((smpc.reg.SF & 1) != 0);
	smpc.reg.SF = 1;
	smpc.reg.COMREG = COMREG__SNDON;
	while (smpc.reg.OREG[31].val != 0b00000110);

	for (long i = 0; i < 807; i++) { asm volatile ("nop"); }   // wait for (way) more than 30µs

	scsp.reg.ctrl.MIXER = MIXER__MEM4MB | MIXER__MVOL(0xf);

	//const uint32_t * buf = reinterpret_cast<uint32_t*>(&_sine_start);
	//copy<uint32_t>(&scsp.ram.u32[0], buf, 44100 * 2);
	//Temporarily reuse ponesound for loading to SCSP RAM
	ponesound_load_8bit_pcm(asset_yaul_pcm8, asset_yaul_pcm8_end - asset_yaul_pcm8, PCM_YAUL, 0, 15360);


	scsp_slot& slot = scsp.reg.slot[0];
	// start address (bytes)
	slot.SA = SA__KYONB | SA__LPCTL__NORMAL | SA__SA(0); // kx kb sbctl[1:0] ssctl[1:0] lpctl[1:0] 8b sa[19:0]
	slot.LSA = 0; // loop start address (samples)
	slot.LEA = 44100; // loop end address (samples)
	//slot.EG = EG__EGHOLD; // d2r d1r ho ar krs dl rr
	slot.EG = EG__AR(0x1F) | EG__D1R(0x00) | EG__D2R(0x00) | EG__RR(0x1F);
	slot.FM = 0; // stwinh sdir tl mdl mdxsl mdysl
	slot.PITCH = PITCH__OCT(0) | PITCH__FNS(0); // oct fns
	slot.LFO = 0; // lfof plfows
	slot.MIXER = MIXER__DISDL(0b101); // disdl dipan efsdl efpan

	slot.LOOP |= LOOP__KYONEX;
	*/
}


void SlotSoundTestState_Free(void)
{
}


void SlotSoundTestState_Update(void)
{
	
}



gamestate_t Get_SlotSoundTestState(void)
{
	return (gamestate_t)
	{
		.InitState = SlotSoundTestState_Init,
		.FreeState = SlotSoundTestState_Free,
		.UpdateState = SlotSoundTestState_Update,
	};
}
