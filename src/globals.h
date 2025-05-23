#ifndef __RINGWORLDS_GLOBAL_DEFINES_H__
#define __RINGWORLDS_GLOBAL_DEFINES_H__

#include <yaul.h>
#include <mic3d.h>

#include <stdint.h>


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
#define TEXTURE_MAX_COUNT		(8)


#define USE_MIC3D //make this into a flag
#ifdef USE_MIC3D

static vdp1_gouraud_table_t _pool_shading_tables[CONFIG_MIC3D_CMDT_COUNT] __aligned(16);
static vdp1_gouraud_table_t _pool_shading_tables2[512] __aligned(16);

static workarea_mic3d_depth_values_t _pool_depth_values;
static workarea_mic3d_z_values_t _pool_z_values;
static workarea_mic3d_screen_points_t _pool_screen_points;
static workarea_mic3d_sort_singles_t _pool_sort_singles;
static workarea_mic3d_cmdts_t _pool_cmdts;
static workarea_mic3d_render_matrices_t _pool_render_matrices;
static workarea_mic3d_light_matrices_t _pool_light_matrices;
static workarea_mic3d_colors_t _pool_colors;
static workarea_mic3d_work_t _pool_work;

static workarea_mic3d_t _workarea = {
    &_pool_depth_values,   &_pool_z_values, &_pool_screen_points,
    &_pool_sort_singles,   &_pool_cmdts,    &_pool_render_matrices,
    &_pool_light_matrices, &_pool_colors,   &_pool_work
};

static sort_list_t _sort_list[512] __aligned(4);

#endif //USE_MIC3D


extern uint32_t FrameCounter;


////////Main Gameplay values////////

#define GRAVITY 				(FIX16(0.8f / (float)FRAME_RATE))
#define PLAYER_MAX_SPEED		(FIX16(24.0f / (float)FRAME_RATE))

#define PERSISTENCE_VALUE_LOC	(LWRAM(0)) //TODO set actual address once the memory map's figured out

////////END Main Gameplay values////////


#endif
