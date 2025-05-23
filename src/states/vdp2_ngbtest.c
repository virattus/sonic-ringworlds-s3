#include "vdp2_ngbtest.h"

#include <backend/vdp_load.h>


const vdp2_scrn_cell_format_t format = {
                .scroll_screen = VDP2_SCRN_NBG0,
                .ccc           = VDP2_SCRN_CCC_PALETTE_16,
                .char_size     = VDP2_SCRN_CHAR_SIZE_1X1,
                .pnd_size      = 1,
                .aux_mode      = VDP2_SCRN_AUX_MODE_1,
                .plane_size    = VDP2_SCRN_PLANE_SIZE_2X2,
                .cpd_base      = (uint32_t)VDP2_VRAM_ADDR(0, 0x00000),
                .palette_base  = (uint32_t)VDP2_CRAM_MODE_0_OFFSET(0, 0, 0)
        };

        const vdp2_scrn_normal_map_t normal_map = {
                .plane_a = (vdp2_vram_t)VDP2_VRAM_ADDR(0, 0x08000),
                .plane_b = (vdp2_vram_t)VDP2_VRAM_ADDR(0, 0x08000),
                .plane_c = (vdp2_vram_t)VDP2_VRAM_ADDR(0, 0x08000),
                .plane_d = (vdp2_vram_t)VDP2_VRAM_ADDR(0, 0x08000)
        };

        const vdp2_vram_cycp_t vram_cycp = {
                .pt[0].t0 = VDP2_VRAM_CYCP_PNDR_NBG0,
                .pt[0].t1 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[0].t2 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[0].t3 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[0].t4 = VDP2_VRAM_CYCP_CHPNDR_NBG0,
                .pt[0].t5 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[0].t6 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[0].t7 = VDP2_VRAM_CYCP_NO_ACCESS,

                .pt[1].t0 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[1].t1 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[1].t2 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[1].t3 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[1].t4 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[1].t5 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[1].t6 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[1].t7 = VDP2_VRAM_CYCP_NO_ACCESS,

                .pt[2].t0 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[2].t1 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[2].t2 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[2].t3 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[2].t4 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[2].t5 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[2].t6 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[2].t7 = VDP2_VRAM_CYCP_NO_ACCESS,

                .pt[3].t0 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[3].t1 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[3].t2 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[3].t3 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[3].t4 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[3].t5 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[3].t6 = VDP2_VRAM_CYCP_NO_ACCESS,
                .pt[3].t7 = VDP2_VRAM_CYCP_NO_ACCESS
        };


fix16_vec2_t scrollOffset = (fix16_vec2_t){ 0 };


void VDP2NGBTestState_Init(void)
{
        vdp2_vram_cycp_set(&vram_cycp);

        vdp2_scrn_cell_format_set(&format, &normal_map);
        vdp2_scrn_priority_set(VDP2_SCRN_NBG0, 7);
        vdp2_scrn_display_set(VDP2_SCRN_DISP_NBG0);

        _cpd_load((uint8_t *)format.cpd_base);
        _palette_load((rgb1555_t *)format.palette_base);
        _map_load(&format, &normal_map);
        
        vdp2_scrn_scroll_x_set(VDP2_SCRN_NBG0, scrollOffset.x);
		vdp2_scrn_scroll_y_set(VDP2_SCRN_NBG0, scrollOffset.y);

}


void VDP2NGBTestState_Free(void)
{
}


void VDP2NGBTestState_Update(void)
{
	scrollOffset.x = FIX16(0.5f);
	
	vdp2_scrn_scroll_x_update(VDP2_SCRN_NBG0, scrollOffset.x);
	vdp2_scrn_scroll_y_update(VDP2_SCRN_NBG0, scrollOffset.y);
	
}


gamestate_t Get_VDP2NGBTestState(void)
{
	return (gamestate_t)
	{
		.InitState = VDP2NGBTestState_Init,
		.FreeState = VDP2NGBTestState_Free,
		.UpdateState = VDP2NGBTestState_Update,
	};
}
