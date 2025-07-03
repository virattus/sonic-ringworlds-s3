#include "workarea.h"



static workarea_mic3d_depth_values_t _pool_depth_values;
static workarea_mic3d_z_values_t _pool_z_values;
static workarea_mic3d_screen_points_t _pool_screen_points;
static workarea_mic3d_sort_singles_t _pool_sort_singles;
static workarea_mic3d_cmdts_t _pool_cmdts;
static workarea_mic3d_render_matrices_t _pool_render_matrices;
static workarea_mic3d_light_matrices_t _pool_light_matrices;
static workarea_mic3d_colors_t _pool_colors;
static workarea_mic3d_work_t _pool_work;


workarea_mic3d_t _workarea = {
    &_pool_depth_values,   &_pool_z_values, &_pool_screen_points,
    &_pool_sort_singles,   &_pool_cmdts,    &_pool_render_matrices,
    &_pool_light_matrices, &_pool_colors,   &_pool_work
};


static sort_list_t _sort_list[512] __aligned(4);

static vdp1_gouraud_table_t _pool_shading_tables[CONFIG_MIC3D_CMDT_COUNT] __aligned(16);


uint32_t GetWorkareaSize(uint16_t cmdt_count)
{
	return 0;
}

uint32_t GetSortListSize(uint16_t sort_count)
{
	return sort_count * 2;
}

uint32_t GetShadingTableSize(uint16_t shading_count)
{
	return 0;
}


workarea_mic3d_t* CreateWorkarea(void* address)
{
	//CONFIG_MIC3D_CMDT_COUNT;
	
	return &_workarea;
}


sort_list_t* CreateSortList(void* address, uint16_t count)
{
	return &_sort_list[0];
}


vdp1_gouraud_table_t* CreateShadingTable(void* address, uint16_t count)
{
	return &_pool_shading_tables[0];
}
