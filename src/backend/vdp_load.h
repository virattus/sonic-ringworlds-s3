#ifndef __SSEE_VDP_LOAD_H__
#define __SSEE_VDP_LOAD_H__


#include <yaul.h>
#include <mic3d.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>



/*
 * upload texture to VDP1 memory, and record it into the texture array
 */
size_t _texture_load(
	texture_t *textures, 
	uint32_t slot, 
	const picture_t *picture, 
	vdp1_vram_t texture_base);


/*
 * Upload VDP2 palette to VDP2 CRAM
 */
void _vdp2_palette_load(uint16_t bank_256, uint16_t bank_16, const palette_t *palette);


/*
 * 
 */
void _cpd_load(uint8_t *cpd_base);


/*
 * Load a palette
 */
void _palette_load(rgb1555_t *palette_base);


/*
 * load a VDP2 map
 */
void _map_load(const vdp2_scrn_cell_format_t *format, const vdp2_scrn_normal_map_t *normal_map);


#endif
