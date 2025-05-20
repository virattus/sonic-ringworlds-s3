#ifndef __SSEE_TEXTURE_LOAD_H__
#define __SSEE_TEXTURE_LOAD_H__


#include <yaul.h>
#include <mic3d.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


size_t _texture_load(
	texture_t *textures, 
	uint32_t slot, 
	const picture_t *picture, 
	vdp1_vram_t texture_base);

void _palette_load(uint16_t bank_256, uint16_t bank_16, const palette_t *palette);





#endif
