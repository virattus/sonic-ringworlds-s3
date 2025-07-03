#include "ssv.h"


#include <assert.h>


#define SSV_MAX_FACE_COUNT 		(512)
#define SSV_MAX_POINTS_COUNT 	(SSV_MAX_FACE_COUNT * 4)


fix16_vec3_t yaul_mesh_points_buffer[SSV_MAX_POINTS_COUNT];
fix16_vec3_t yaul_mesh_normals_buffer[SSV_MAX_POINTS_COUNT];
attribute_t yaul_mesh_attribute_buffer[SSV_MAX_FACE_COUNT];
polygon_t yaul_mesh_polygon_buffer[SSV_MAX_FACE_COUNT];



void SSV_LoadFromMemory(ssv_mesh_t* mesh, void* ptrData)
{
	mesh->points_count = *(uint16_t*)(ptrData);
	mesh->polygons_count = *(uint16_t*)(ptrData + 1);
	mesh->frame_count = *(uint16_t*)(ptrData + 2);
	mesh->fix8_points = (fix8_vec3_t*)(&ptrData + 3);
	mesh->face_entries = (ssv_face_t*)(&ptrData + 3 + ((mesh->points_count * 3) * mesh->frame_count));
	mesh->normals = (int8_vec3_t*)(&ptrData + 6 + ((mesh->points_count * 6) * mesh->frame_count) + (mesh->polygons_count * 12));
	mesh->texture_offset = 0;
}




void SSV_DrawMeshAnim(const ssv_mesh_t* mesh, const fix16_mat43_t* worldMatrix, fix16_t frameTime)
{
	assert(mesh->points_count < SSV_MAX_POINTS_COUNT);
	
	uint16_t current_frame = frameTime >> 16;
	uint16_t next_frame = current_frame + 1;
	
	//get time elapsed since start of current frame
	fix16_t LerpInator = frameTime - FIX16(current_frame);
	
	//If we go past the length of the animation array
	if(frameTime > FIX16(mesh->frame_count))
	{
		frameTime = 0;
		LerpInator = 0;
		
		current_frame = mesh->frame_count - 1;
		next_frame = mesh->frame_count - 1;
	}
	
	
	if(frameTime < 0)
	{
		frameTime = 0;
		LerpInator = 0;
		
		current_frame = 0;
		next_frame = 0;
	}
	
	
	//convert fix8 to fix16 in preparation for yaul
	for(int i = 0; i < mesh->points_count; i++)
	{
		yaul_mesh_points_buffer[i].x = mesh->fix8_points[i].x << 8;
		yaul_mesh_points_buffer[i].y = mesh->fix8_points[i].y << 8;
		yaul_mesh_points_buffer[i].z = mesh->fix8_points[i].z << 8;
		
		yaul_mesh_normals_buffer[i].x = mesh->normals[i].x << 8;
		yaul_mesh_normals_buffer[i].y = mesh->normals[i].y << 8;
		yaul_mesh_normals_buffer[i].z = mesh->normals[i].z << 8;
	}
	
	polygon_t polyTemplate = {
		.flags.sort_type = SORT_TYPE_CENTER,
		.flags.plane_type = PLANE_TYPE_SINGLE,
		.flags.use_texture = false,
	};
	
	attribute_t attrTemplate = {
		.draw_mode.color_mode = VDP1_CMDT_CM_RGB_32768,
		.control.command = COMMAND_TYPE_DISTORTED_SPRITE,
		.control.link_type = LINK_TYPE_JUMP_ASSIGN,
		.palette_data.base_color = RGB1555(1, 25, 3, 3),
	};
	
	for(int i = 0; i < mesh->polygons_count; i++)
	{
		attrTemplate.texture_slot = mesh->face_entries[i].texture_id;
		
		polyTemplate.indices.p0 = mesh->face_entries[i].point1;
		polyTemplate.indices.p1 = mesh->face_entries[i].point2;
		polyTemplate.indices.p2 = mesh->face_entries[i].point3;
		polyTemplate.indices.p3 = mesh->face_entries[i].point4;
		
		yaul_mesh_attribute_buffer[i] = attrTemplate;
		yaul_mesh_polygon_buffer[i] = polyTemplate;
	}
	
	
	
	mesh_t yaulMesh = {
		.points = yaul_mesh_points_buffer,
		.points_count = mesh->points_count,
		.normals = yaul_mesh_normals_buffer,
		.polygons = yaul_mesh_polygon_buffer,
		.polygons_count = mesh->polygons_count,
	};
	
	render_mesh_xform(&yaulMesh, worldMatrix);
}
