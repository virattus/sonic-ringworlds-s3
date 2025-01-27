#include "ring.h"

#include <assert.h>


//currently all rings will use the same sprite each frame
static uint8_t ringFrameCounter = 0;

static texture_t* ringTex = NULL;





void Ring_Update(void)
{
	ringFrameCounter++;
}



void Ring_SetSpriteData(texture_t* tex)
{
	ringTex = tex;
}


void Ring_AssignSpriteData(vdp1_cmdt_t* cmdt)
{
	assert(ringTex);
	assert(cmdt);
	
	vdp1_cmdt_scaled_sprite_set(cmdt);
	vdp1_cmdt_draw_mode_t dm = {
		.color_mode = 5,
		.trans_pixel_disable = true,
		.pre_clipping_disable = false,
		.end_code_disable = false,
		.cc_mode = 0
	};
	
	vdp1_cmdt_draw_mode_set(cmdt, dm);
	
	//vdp1_cmdt_color_set(cmdt, RGB1555(1, 31, 31, 31));	
	
	vdp1_cmdt_zoom_set(cmdt, VDP1_CMDT_ZOOM_POINT_CENTER);
	
	cmdt->cmd_srca = ringTex->vram_index;
	cmdt->cmd_size = ringTex->size;
	vdp1_cmdt_char_flip_set(cmdt, VDP1_CMDT_CHAR_FLIP_NONE);
}


//TODO
void Ring_ScaleToDepth(vdp1_cmdt_t* cmdt, fix16_t depth_value)
{
	assert(cmdt);
	
	cmdt->cmd_vertices[1].x = 25;
	cmdt->cmd_vertices[1].y = 25;
}
