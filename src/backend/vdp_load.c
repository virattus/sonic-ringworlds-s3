#include "vdp_load.h"


// TODO: rewrite these so they work how you would think


size_t _texture_load(
	texture_t *textures, 
	uint32_t slot, 
	const picture_t *picture, 
	vdp1_vram_t texture_base)
{
        texture_t * const texture = &textures[slot];

        texture->size       = TEXTURE_SIZE(picture->width, picture->height);
        texture->vram_index = TEXTURE_VRAM_INDEX(texture_base);

        scu_dma_transfer(0, (void *)texture_base, picture->data, picture->data_size);
        scu_dma_transfer_wait(0);

        return picture->data_size;
}

void _vdp2_palette_load(uint16_t bank_256, uint16_t bank_16, const palette_t *palette)
{
        scu_dma_transfer(0, (void *)VDP2_CRAM_MODE_0_OFFSET(bank_256, bank_16, 0), palette->data, palette->data_size);
        scu_dma_transfer_wait(0);
}


void _cpd_load(uint8_t *cpd_base)
{
        (void)memset(cpd_base + 0x00, 0x00, 0x20);
        (void)memset(cpd_base + 0x20, 0x11, 0x20);
}

void _palette_load(rgb1555_t *palette_base)
{
        palette_base[0] = RGB1555(1, 31, 31, 31);
        palette_base[1] = RGB1555(1, 31,  0,  0);
}


void _map_load(const vdp2_scrn_cell_format_t *format, const vdp2_scrn_normal_map_t *normal_map)
{
        const uint32_t page_width = VDP2_SCRN_PAGE_WIDTH_CALCULATE(format);
        const uint32_t page_height = VDP2_SCRN_PAGE_HEIGHT_CALCULATE(format);
        const uint32_t page_size = VDP2_SCRN_PAGE_SIZE_CALCULATE(format);

        uint16_t * const a_pages[4] = {
                (uint16_t *)normal_map->base_addr[0],
                (uint16_t *)normal_map->base_addr[1 * (page_size / sizeof(uint16_t))],
                (uint16_t *)normal_map->base_addr[2 * (page_size / sizeof(uint16_t))],
                (uint16_t *)normal_map->base_addr[3 * (page_size / sizeof(uint16_t))]
        };

        uint16_t tile;
        tile = 0;

        for (uint32_t page_y = 0; page_y < page_height; page_y++) {
                for (uint32_t page_x = 0; page_x < page_width; page_x++) {
                        const uint16_t page_idx = page_x + (page_width * page_y);
                        const uint16_t pnd = VDP2_SCRN_PND_CONFIG_1(1, (uint32_t)format->cpd_base,
                            (uint32_t)format->palette_base);

                        a_pages[0][page_idx] = pnd | tile;

                        tile ^= 1;
                }

                tile ^= 1;
        }
}
