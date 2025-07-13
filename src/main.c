#include <yaul.h>
#include <mic3d.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "globals.h"
#include "backend/debug/debugmenu.h"

//Test states
#include "states/testcollision.h"
#include "states/modelloading.h"
#include "states/soundtest.h"
#include "states/vdp2_ngbtest.h"
#include "states/cdtest.h"
#include "states/rng_output.h"
#include "states/musictest.h"
#include "states/planerender.h"
#include "states/customrenderer.h"


static uint32_t _frame_time_calculate(void);
static void InitialiseRNGAndClock(void);


#ifdef PCBUILD
#define WINDOW_WIDTH 	(352)
#define WINDOW_HEIGHT 	(224)
#define WINDOW_SCALE 	(1)

#define SCREEN_WIDTH	(352)
#define SCREEN_HEIGHT	(224)

void user_init();
#endif //PCBUILD


int main()
{

#ifdef PCBUILD
	Window_Init();
	
	gamewindow_t gw;
	gw.resolution.x = WINDOW_WIDTH * WINDOW_SCALE;
	gw.resolution.y = WINDOW_HEIGHT * WINDOW_SCALE;
	gw.displayFramerate = true;
	gw.limitFramerate = true;
	gw.win.flags.integerScale = 1;
	gw.win.flags.fullscreen = false;
	gw.win.flags.vsync = false;
	
	GameWindow_Open(&gw, "Ringworlds Stage 2");
	
	int16_vec2_t res = {
		.x = SCREEN_WIDTH,
		.y = SCREEN_HEIGHT,
	};
	
	Window_SetInternalResolution(&gw.win, res);
	
	user_init();	

#endif //PCBUILD
	
	dbgio_init();
	dbgio_dev_default_init(DBGIO_DEV_VDP2_ASYNC);
	dbgio_dev_font_load();
	
	DebugMenu_SetState(DB_STATE_COMMAND_TABLES);
	
	InitialiseRNGAndClock();
	
	//GameState_Push(Get_TestCollisionState());
	//GameState_Push(Get_ModelLoadState());
	//GameState_Push(Get_SoundTestState());
	//GameState_Push(Get_VDP2NGBTestState());
	//GameState_Push(Get_CDTestState());
	//GameState_Push(Get_RNGOutputState());
	//GameState_Push(Get_MusicTestState());
	GameState_Push(Get_PlaneRenderState());
	//GameState_Push(Get_CustomRendererState());
	
	vdp2_scrn_back_color_set(VDP2_BACK_SCREEN, RGB1555(1, 0, 3, 15));
	
	gamestate_t currentState;


#ifdef PCBUILD
	while(GameWindow_HandleEvents())
#else
	while(true)
#endif //PCBUILD
	{
		dbgio_printf("[H[2J");
		
		/*
		uint32_t frameT = _frame_time_calculate();
	
		char frameTimeStr[16];
		fix16_str(frameT, frameTimeStr, 7);
		
		dbgio_printf("Total Frames: %d\n"
			"%s ms\n",
			FrameCounter++, 
			frameTimeStr
		);
		*/
		
		currentState = GameState_Current();
		currentState.UpdateState();
		
		DebugMenu_Update();
		
		dbgio_flush();

		vdp1_sync_render();
		vdp1_sync();
		vdp1_sync_wait();
		
		vdp2_sync();
		vdp2_sync_wait();
		
		
#ifdef PCBUILD
		GameWindow_Update(&gw);
#endif //PCBUILD
	}
	
#ifdef PCBUILD
	GameWindow_Close(&gw);
	VDP1_Delete();
	VDP2_Delete();
	
	return 0;
#endif //PCBUILD
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


void user_init(void)
{
	vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_B, VDP2_TVMD_VERT_224);

	vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE), RGB1555(1, 0, 0, 0));

	vdp1_sync_interval_set(FRAME_RATE_SYNC);

	vdp1_env_default_set();

	vdp2_sprite_priority_set(0, 6);

	vdp2_tvmd_display_set();
	
	vdp_sync_vblank_out_set(_vblank_out_handler, NULL);
	
	cpu_frt_init(CPU_FRT_CLOCK_DIV_32);
	cpu_frt_ovi_set(_cpu_frt_ovi_handler);

	cd_block_init();
	
	smpc_peripheral_init();
	
	vdp2_sync();
	vdp2_sync_wait();
}


//taken directly from yaul
static void InitialiseRNGAndClock(void)
{
	const smpc_time_t * const time = smpc_rtc_time_get();
	smpc_time_dec_t time_dec;
	
	// Wait until the SMPC peripheral intback command has been issued at least once
	vdp2_tvmd_vblank_in_wait();
	vdp2_tvmd_vblank_out_wait();
	smpc_peripheral_process();
	smpc_rtc_time_bcd_from(time, &time_dec);
	
	//Initialise C's builtin PRNG with the clock
	const uint32_t seed = 	((uint32_t)time_dec.month * 2629800UL) +
							((uint32_t)time_dec.day * 86400UL) +
							((uint32_t)time_dec.hours * 3600UL) +
							((uint32_t)time_dec.minutes * 60UL) +
							(uint32_t)time_dec.seconds;
	
	srand(seed);	
}	
