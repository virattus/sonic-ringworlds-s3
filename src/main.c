#include <yaul.h>
#include <mic3d.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "globals.h"

#include "states/testcollision.h"
#include "states/modelloading.h"
#include "states/soundtest.h"


static uint32_t _frame_time_calculate(void);


int main()
{	
	dbgio_init();
	dbgio_dev_default_init(DBGIO_DEV_VDP2_ASYNC);
	dbgio_dev_font_load();
	
	RNG_Initialise(0x3A41);
	//RNGInitialSeed = RNG_Initialise_RTC(smpc_rtc_time_get()); //TODO use timer after smpc finally works

	//GameState_Push(Get_TestCollisionState());
	//GameState_Push(Get_ModelLoadState());
	GameState_Push(Get_SoundTestState());
	
	gamestate_t currentState;
		
	while(true)
	{
		dbgio_printf("[H[2J");
		
		currentState = GameState_Current();
		currentState.UpdateState();
		
		uint32_t frameT = _frame_time_calculate();
	
		char frameTimeStr[16];
		fix16_str(frameT, frameTimeStr, 7);
		
		dbgio_printf("Total Frames: %d\n"
			"%s ms\n",
			FrameCounter++, 
			frameTimeStr
		);
		
		dbgio_flush();

		vdp1_sync_render();
		vdp1_sync();
		vdp1_sync_wait();
		
		vdp2_sync();
		vdp2_sync_wait();
		
	}
}


static uint32_t _frame_time_calculate(void)
{
	const uint16_t frt = cpu_frt_count_get();
	
	cpu_frt_count_set(0);
	
	const uint32_t delta_fix = frt << 8;
	const uint32_t divisor_fix = CPU_FRT_NTSC_320_32_COUNT_1MS << 4;
	
	cpu_divu_32_32_set(delta_fix, divisor_fix);
	
	uint32_t result;
	result = cpu_divu_quotient_get();
	result <<= 12;
	
	return result;
}



static void _vblank_out_handler(void* work __unused)
{
	smpc_peripheral_intback_issue();
}


static void _cpu_frt_ovi_handler(void)
{
}


void user_init()
{
	vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_B, VDP2_TVMD_VERT_224);

	vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE), RGB1555(1, 0, 3, 15));
	

	vdp1_sync_interval_set(FRAME_RATE_SYNC);

	vdp1_env_default_set();

	vdp2_sprite_priority_set(0, 6);

	vdp2_tvmd_display_set();
	
	vdp_sync_vblank_out_set(_vblank_out_handler, NULL);
	
	cpu_frt_init(CPU_FRT_CLOCK_DIV_32);
	cpu_frt_ovi_set(_cpu_frt_ovi_handler);

	//cd_block_init();
	
	smpc_peripheral_init();
	
	vdp2_sync();
	vdp2_sync_wait();
}


