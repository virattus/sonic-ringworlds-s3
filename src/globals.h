#ifndef __RINGWORLDS_GLOBAL_DEFINES_H__
#define __RINGWORLDS_GLOBAL_DEFINES_H__

#include <yaul.h>
#include <mic3d.h>

#include <stdint.h>


//////// VDP1 DEFINES ////////

#define VIDEO_NTSC
//#define VIDEO_PAL

#define FRAME_RATE				(30)

/* Normalize the frame rate:
         *  0: no frame rate cap
         *  1: cap at 30Hz
         *  2: cap at 20Hz
         *  3: cap at 15Hz
         *  4: cap at 12Hz
         *  6: cap at 10Hz */
#if FRAME_RATE >= 60
	#define FRAME_RATE_SYNC		(0)
#elif FRAME_RATE >= 30
	#define FRAME_RATE_SYNC		(-1)
#elif FRAME_RATE >= 20
	#define FRAME_RATE_SYNC		(-2)
#elif FRAME_RATE >= 15
	#define FRAME_RATE_SYNC		(-3)
#endif

#define DISPLAY_WIDTH			(320)
#define DISPLAY_HEIGHT			(224)

#define RENDER_SORT_DEPTH		(512)



//////// END VDP1 DEFINES /////////



//////// VDP2 DEFINES ////////


#define VDP2_NBGX_CPD         VDP2_VRAM_ADDR(0, 0x04000)
#define VDP2_NBGX_PAL         VDP2_CRAM_MODE_1_OFFSET(0, 0, 0)

#define VDP2_BACK_SCREEN      VDP2_VRAM_ADDR(3, 0x01FFFE)


//////// END VDP2 DEFINES ////////


extern uint32_t FrameCounter;


////////Main Gameplay values////////

#define GAME_FLAG_FREEZE_CMDTS		(0)
#define GAME_FLAG_FORCE_WIREFRAME	(1)
#define GAME_FLAG_DISABLE_LIGHTING	(2)
#define GAME_FLAG_DISABLE_FOG		(3)
#define GAME_FLAG_DISABLE_LANTERN	(4)
#define GAME_FLAG_CURR_CMDTS		(5)

#define GAME_FLAG_PLAYER_INVINCIBLE	(8)
#define GAME_FLAG_ENEMY_INVINCIBLE	(9)
#define GAME_FLAG_FREE_CAM			(10)
#define GAME_FLAG_FREEZE_CAM		(11)
#define GAME_FLAG_CURRENT_LEVEL		(12)

#define GAME_FLAG_COUNT				(32)

extern uint16_t GameFlags[GAME_FLAG_COUNT];
extern uint16_t GameFlagsMax[GAME_FLAG_COUNT];




#define GRAVITY 				(FIX16(0.8f / (float)FRAME_RATE))
#define PLAYER_MAX_SPEED		(FIX16(24.0f / (float)FRAME_RATE))

#define PERSISTENCE_VALUE_LOC	(LWRAM(0)) //TODO set actual address once the memory map's figured out

////////END Main Gameplay values////////


#endif
