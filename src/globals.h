#ifndef __RINGWORLDS_GLOBAL_DEFINES_H__
#define __RINGWORLDS_GLOBAL_DEFINES_H__


#include <stdint.h>

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
#define TEXTURE_MAX_COUNT		(8)


#define GRAVITY 				(FIX16(0.8f / (float)FRAME_RATE))
#define PLAYER_MAX_SPEED		(FIX16(24.0f / (float)FRAME_RATE))


extern uint32_t FrameCounter;


#endif
