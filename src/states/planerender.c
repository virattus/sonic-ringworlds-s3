#include "planerender.h"


#define ORDER_SYSTEM_CLIP_COORDS 	(0)
#define ORDER_LOCAL_COORDS			(1)
#define ORDER_POLYGON				(2)
#define ORDER_DRAW_END				(3)
#define ORDER_COUNT					(4)

#define CMDT_MAX					(128)


vdp1_cmdt_list_t* cmdt_list = NULL;
vdp1_vram_partitions_t vdp1_partitions;

int16_vec2_t ADSlope = INT16_VEC2_INITIALIZER(-1, -1);



static const int16_vec2_t polygon_coords[] = {
	INT16_VEC2_INITIALIZER(50, 20),
	INT16_VEC2_INITIALIZER(300, 50),
	INT16_VEC2_INITIALIZER(300, 150),
	INT16_VEC2_INITIALIZER(10, 200),
};

static const vdp1_cmdt_draw_mode_t polygon_draw = {
	.msb_enable = false,
	.hss_enable = true,
	.pre_clipping_disable = false,
	.user_clipping_mode = false,
	.mesh_enable = false,
	.end_code_disable = false,
	.trans_pixel_disable = false,
	.color_mode = 0,
	.cc_mode = 0,
};


static void BuildCmdt(vdp1_cmdt_t* cmdt, const int16_vec2_t* points)
{
	uint16_t colour = (
		points[0].x | points[0].y |
		points[1].x | points[1].y |
		points[2].x | points[2].y |
		points[3].x | points[3].y
	) | 0x8000;
	
	//vdp1_cmdt_polygon_set(cmdt);
	vdp1_cmdt_polyline_set(cmdt);
	vdp1_cmdt_color_set(cmdt, (rgb1555_t)colour);
	vdp1_cmdt_draw_mode_set(cmdt, polygon_draw);
	vdp1_cmdt_vtx_set(cmdt, points);
}



static void RenderSubdivPlane(const int16_vec2_t* points, uint16_t xDiv, uint16_t yDiv)
{	
	int16_vec2_t newPoints[4];
	int16_vec2_t slopeAB, slopeAD, slopeAC, slopeDC;
		
	slopeAB.x = (points[1].x - points[0].x) / (xDiv + 1);
	slopeAB.y = (points[1].y - points[0].y) / (yDiv + 1);
	
	slopeAD.x = (points[3].x - points[0].x) / (xDiv + 1);
	slopeAD.y = (points[3].y - points[0].y) / (yDiv + 1);
	
	slopeAC.x = slopeAB.x;
	slopeAC.y = slopeAD.y;
	
	slopeDC.x = (points[2].x - points[3].x) / (xDiv + 1);
	slopeDC.y = (points[2].y - points[3].y) / (yDiv + 1);
	
	int16_vec2_dup(&slopeAD, &ADSlope);
	
	vdp1_cmdt_t* polys = &cmdt_list->cmdts[ORDER_POLYGON];
	uint16_t polyIndex = 0;
	
	for(int32_t i = 0; i < (yDiv + 1); i++)
	{
		int16_vec2_t startPoints[2];		
		int16_vec2_t tempSlope;
		int16_vec2_scale(i, &slopeAD, &tempSlope);
		int16_vec2_add(&points[0], &tempSlope, &startPoints[0]);
		int16_vec2_scale(i + 1, &slopeAD, &tempSlope);
		int16_vec2_add(&points[0], &tempSlope, &startPoints[1]);
		
		for(int32_t j = 0; j < (xDiv + 1); j++)
		{
			int16_vec2_dup(&startPoints[0], &newPoints[0]);
			
			int16_vec2_add(&startPoints[0], &slopeAB, &newPoints[1]);
			
			int16_vec2_add(&startPoints[1], &slopeDC, &newPoints[2]);
			
			int16_vec2_dup(&startPoints[1], &newPoints[3]);
			
			int16_vec2_dup(&newPoints[1], &startPoints[0]);
			int16_vec2_dup(&newPoints[2], &startPoints[1]);
			
			vdp1_cmdt_t* poly = &polys[polyIndex];
			BuildCmdt(poly, &newPoints[0]);
			
			polyIndex += 1;
		}
	}
	
	vdp1_cmdt_end_set(&polys[polyIndex]);
}


static void InitCmds(void)
{
	const int16_vec2_t system_clip = INT16_VEC2_INITIALIZER(
		DISPLAY_WIDTH - 1,
		DISPLAY_HEIGHT - 1);
	
	const int16_vec2_t local_coords = INT16_VEC2_INITIALIZER(
		0, //(DISPLAY_WIDTH >> 1),
		0); //(DISPLAY_HEIGHT >> 1));
	
	vdp1_cmdt_t* cmdts = &cmdt_list->cmdts[0];
	
	vdp1_cmdt_system_clip_coord_set(&cmdts[ORDER_SYSTEM_CLIP_COORDS]);
	vdp1_cmdt_vtx_system_clip_coord_set(&cmdts[ORDER_SYSTEM_CLIP_COORDS], system_clip);
	
	vdp1_cmdt_local_coord_set(&cmdts[ORDER_LOCAL_COORDS]);
	vdp1_cmdt_vtx_local_coord_set(&cmdts[ORDER_LOCAL_COORDS], local_coords);
	
	vdp1_cmdt_end_set(&cmdts[ORDER_DRAW_END]);
}


static void PlaneRenderState_Init(void)
{
	vdp1_vram_partitions_get(&vdp1_partitions);
	
	//Init cmdt list
	cmdt_list = vdp1_cmdt_list_alloc(CMDT_MAX);
	cmdt_list->count = CMDT_MAX;
	
	(void)memset(&cmdt_list->cmdts[0], 0x00, sizeof(vdp1_cmdt_t) * ORDER_COUNT);

	
	InitCmds();
}


static void PlaneRenderState_Free(void)
{
}


static void PlaneRenderState_Update(void)
{
	RenderSubdivPlane(&polygon_coords[0], 1, 1);
	
	dbgio_printf("AD Slope x: %d y: %d\n", ADSlope.x, ADSlope.y);
	
	//0 here sets the start of the cmdts to be uploaded, so 
	//you can use it to skip entries
	vdp1_sync_cmdt_list_put(cmdt_list, 0);
}


gamestate_t Get_PlaneRenderState(void)
{
	return (gamestate_t)
	{
		.InitState = PlaneRenderState_Init,
		.FreeState = PlaneRenderState_Free,
		.UpdateState = PlaneRenderState_Update,
	};
}
