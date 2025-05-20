#include "testcollision.h"


#include "object/ring/ring_bounce.h"
#include "collision/polygon.h"
#include "collision/collisionWorld.h"
#include "collision/octree.h"


extern const mesh_t mesh_m;
extern const mesh_t mesh_i;
extern const mesh_t mesh_c;
extern const mesh_t mesh_cube;
extern const mesh_t mesh_torus;
extern const mesh_t mesh_torus2;
extern const mesh_t mesh_torus3;

extern const picture_t picture_mika;
extern const picture_t picture_tails;
extern const picture_t picture_baku;

extern const palette_t palette_baku;

static texture_t _textures[8];

vdp1_vram_partitions_t vdp1_vram_partitions;

camera_t camera;

octree_node_t testOctree;

static fix16_mat43_t world[5];
static fix16_mat43_t lineMat;

angle_t theta = DEG2ANGLE(0.0);

col_sphere_t sphere = {0};
fix16_vec3_t velocity;

col_line_t lineBase;


void TestCollisionState_Init(void)
{
	vdp1_vram_partitions_get(&vdp1_vram_partitions);
	
	mic3d_init(&_workarea);
	render_sort_depth_set(_sort_list, RENDER_SORT_DEPTH);
	
	tlist_set(_textures, TEXTURE_MAX_COUNT);
	
	vdp1_vram_t texture_base;
	texture_base = (vdp1_vram_t)vdp1_vram_partitions.texture_base;
	
	texture_base += _texture_load(_textures, 0, &picture_mika, texture_base);
	texture_base += _texture_load(_textures, 1, &picture_tails, texture_base);
	texture_base += _texture_load(_textures, 2, &picture_baku, texture_base);
	
	Ring_SetSpriteData(&_textures[0]);
	
	RingBounce_Initialise();
	
	fix16_vec3_t vertices[] = 
	{
		/*
		FIX16_VEC3_INITIALIZER(-0.93969f, -0.16318f, -1.0442f),
		FIX16_VEC3_INITIALIZER(0.93969f, 0.51407f, -0.92542f),
		FIX16_VEC3_INITIALIZER(0.93969f, 0.16318f, 1.0442f),
		FIX16_VEC3_INITIALIZER(-0.93969f, -0.51047f, 0.92542f),
		*/
		
		FIX16_VEC3_INITIALIZER(-20.0f, -5.0f, -20.0f),
		FIX16_VEC3_INITIALIZER(20.0f, -5.0f, -20.0f),
		FIX16_VEC3_INITIALIZER(20.0f, -5.0f, 20.0f),
		FIX16_VEC3_INITIALIZER(-20.0f, -5.0f, 20.0f),
		
	};
	
	lineBase = (col_line_t)
	{
		.start = 
		{
			0,
			FIX16(-10.0),
			0,
		},
		.end = 
		{
			0,
			FIX16(10.0),
			0,
		}
	};
	
	col_polygon_t polygon = 
	{
		.offset = &vertices[0],
		.normal = {
			0, 
			FIX16(1.0f),
			0,
		},
		.properties = 0,
	};
	
	PolyProperties_SetVertexCount(&polygon.properties, 4);
	col_Polygon_GenerateNormal(&polygon, &polygon.normal);
	
	Octree_CreateOctree(&testOctree, &polygon, 1, 1);

	colworld_BindOctree(&testOctree);
	
	
	camera.position.x = FIX16(0.0);
	camera.position.y = FIX16(10.0);
	camera.position.z = FIX16(-90.0);
	
	camera.target.x = FIX16(0.0);
	camera.target.y = FIX16(0.0);
	camera.target.z = FIX16(0.0);

	camera.up.x = FIX16(0.0);
	camera.up.y = FIX16(1.0);
	camera.up.z = FIX16(0.0);
	
	camera_lookat(&camera);
	
	fix16_mat33_identity(&world[0].rotation);
	fix16_mat33_identity(&world[1].rotation);
	
	fix16_mat33_identity(&lineMat.rotation);
	fix16_vec3_zero(&lineMat.translation);
		
	xform_t transforms[4];
	render_points_xform(&world[0], vertices, transforms, 4);

	vdp1_cmdt_t polyCmdt = {0};
	vdp1_cmdt_polyline_set(&polyCmdt);
	vdp1_cmdt_color_set(&polyCmdt, RGB1555(1, 31, 31, 0));
	
	for(int i = 0; i < 4; i++)
	{
		int16_vec2_dup(&transforms[i].screen_point, &polyCmdt.cmd_vertices[i]);
	}
		
		
	sphere.position.z = FIX16(10.0f);
	sphere.position.y = FIX16(50.0f);
	sphere.radius = FIX16(20.0f);
	sphere.radiusSquared = FIX16(20.0f * 20.0f);
			
	fix16_vec3_zero(&velocity);
	
	RingBounce_SetSpawnSource(&sphere.position);
	
}


void TestCollisionState_Free(void)
{
}


void TestCollisionState_Update(void)
{
	char dbgStr[64];
	
	/*
	fix16_vec3_str(&polygon.offset[0], dbgStr, 3);
	dbgio_printf("first vertex:\n%s\n", dbgStr);
	*/
	
	smpc_peripheral_digital_t digital;
	
	smpc_peripheral_process();
	smpc_peripheral_digital_port(1, &digital);
	
	const uint16_t pressed = digital.pressed.raw;
	/*
	 * Held only fires once per hold action, so use pressed when checking each frame
	 */
	const uint16_t held = digital.held.raw;
	
	bool moving = false;
	
#define MOVE_DIST (1 << 13)
	
	if(pressed & PERIPHERAL_DIGITAL_LEFT)
	{
		//sphere.position.x += 1 << 12;
		//moving = true;
		lineMat.translation.x -= MOVE_DIST;
	}
	
	if(pressed & PERIPHERAL_DIGITAL_RIGHT)
	{
		//sphere.position.x -= 1 << 12;
		//moving = true;
		lineMat.translation.x += MOVE_DIST;
	}
	
	if(pressed & PERIPHERAL_DIGITAL_UP)
	{
		lineMat.translation.y -= MOVE_DIST;
	}
	
	if(pressed & PERIPHERAL_DIGITAL_DOWN)
	{
		lineMat.translation.y += MOVE_DIST;
	}
	
	if(pressed & PERIPHERAL_DIGITAL_Z)
	{
		lineMat.translation.z -= MOVE_DIST;
	}
	
	if(pressed & PERIPHERAL_DIGITAL_C)
	{
		lineMat.translation.z += MOVE_DIST;
	}
	
	if(held & PERIPHERAL_DIGITAL_A)
	{
		RingBounce_AddRings(1);
	}
	
	
	//else //temp
	{ 
		//velocity.y = clamp(velocity.y - GRAVITY, -PLAYER_MAX_SPEED, PLAYER_MAX_SPEED);	
		//sphere.position.y += velocity.y;
	}

	const polygon_mask_types_t lineMask = TYPE_PLAYER;
	col_resp_t coll_array[8];
	rgb1555_t lineCol = RGB1555(1, 31, 31, 31);

	col_line_t lineTransformed;
	
	//How to transform a vertex manually:
	fix16_mat43_pos3_mul(&lineMat, &lineBase.start, &lineTransformed.start);
	fix16_mat43_pos3_mul(&lineMat, &lineBase.end, &lineTransformed.end);
	
	/*
	fix16_vec3_str(&lineTransformed.start, dbgStr, 3);
	dbgio_printf("new line positions:\n%s\n", dbgStr);
	fix16_vec3_str(&lineTransformed.end, dbgStr, 3);
	dbgio_printf("%s\n", dbgStr);
	*/
			
	if(colworld_CheckCollisionLine(&lineTransformed, lineMask, coll_array, 8))
	{
		//dbgio_printf("line collision\n");
		fix16_vec3_dup(&coll_array[0].position, &world[1].translation);
		lineCol = RGB1555(1, 31, 0, 0);
	}
	
	if(false)
	{
	if(colworld_CheckCollisionSphere(&sphere, lineMask, coll_array, 8))
	{
		/*
		dbgio_printf("Collision responce:\n");
		
		char fx16buff[64];
		fix16_vec3_str(&coll.normal, fx16buff, 3);
		dbgio_printf("Normal: %s\n", fx16buff);
		
		fix16_str(coll.penetration, fx16buff, 7);
		dbgio_printf("Penetration: %s\n", fx16buff);
		*/
		
		fix16_vec3_t position;
		fix16_vec3_t resolution;
		fix16_vec3_dup(&sphere.position, &position);
		fix16_vec3_dup(&coll_array[0].normal, &resolution);
		
		fix16_vec3_scale(&coll_array[0].penetration, &resolution);
		fix16_vec3_add(&resolution, &position, &sphere.position);
		velocity.y = -velocity.y;
		//velocity.y = 0;
	}
	}
	
	world[0].translation = sphere.position;
	

	
	theta += DEG2ANGLE(2.5);
	
			
	RingBounce_Update();
	
	
	render_start();
	
	fix16_mat43_t result;
	fix16_mat33_identity(&result.rotation);
	
	fix16_mat33_z_rotate(&world[0].rotation, theta, &result.rotation);
	fix16_vec3_dup(&world[0].translation, &result.translation);
	
	
	//Render sphere
	render_mesh_xform(&mesh_m, &result);
	
	//Render Line collision point
	render_mesh_xform(&mesh_m, &world[1]);
	
	RingBounce_DrawRings();
	
	//Draw floor polygon
	////render_cmdt_insert(&polyCmdt, FIX16(-50.0f));
	//col_Polygon_DebugDraw(&polygon, RGB1555(1, 31, 31, 0));
	
	//Draw line
	//col_Line_DebugDraw(&lineTransformed, lineCol);
	
	render_end();
	
	fix16_str(coll_array[0].penetration, dbgStr, 7);
	
	
	/*
	dbgio_printf("%d total rings\n", RingBounce_GetTotal());
	//dbgio_printf("%d rng\n", RNG_Generate());
	
	fix16_vec3_t pos0;
	for(int i = 0; i < 1; i++)
	{
		RingBounce_GetPosition(i, &pos0);
		fix16_vec3_str(&pos0, dbgStr, 3);
		dbgio_printf("ring %d pos: %s\n", i, dbgStr);
		RingBounce_GetVelocity(i, &pos0);
		fix16_vec3_str(&pos0, dbgStr, 3);
		dbgio_printf("vel: %s\n", dbgStr);
		dbgio_printf("lifetime: %d\n", RingBounce_GetLifetime(i));
	}
	*/
}


gamestate_t Get_TestCollisionState(void)
{
	return (gamestate_t)
	{
		.InitState = TestCollisionState_Init,
		.FreeState = TestCollisionState_Free,
		.UpdateState = TestCollisionState_Update,
	};
}
