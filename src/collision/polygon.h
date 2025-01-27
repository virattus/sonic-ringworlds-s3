#ifndef __SSEE_COLLISION_POLYGON_H__
#define __SSEE_COLLISION_POLYGON_H__


#include "collision.h"
#include "col_resp.h"
#include "polygonProperties.h"

#include <yaul.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>



struct COL_POLYGON
{
	fix16_vec3_t* offset;
	fix16_vec3_t normal;
	polyProperties properties;
	
};


static inline void col_Polygon_GetVertex(const col_polygon_t* p0, fix16_vec3_t* v0, uint8_t index)
{
	assert(index < PolyProperties_GetVertexCount(p0->properties));
	
	fix16_vec3_dup(&p0->offset[index], v0);
}


void col_Polygon_GenerateNormal(const col_polygon_t* p0, fix16_vec3_t* dest);

void col_Polygon_SetNormal(col_polygon_t* p0, const fix16_vec3_t* norm);
//Planning to make normals be 3 bytes long, will need to convert to fix16
void col_Polygon_GetNormal_Inplace(const col_polygon_t* p0, fix16_vec3_t* dest);
fix16_vec3_t col_Polygon_GetNormal(const col_polygon_t* p0);

void col_Polygon_DebugDraw(const col_polygon_t* p0, const rgb1555_t col);

col_plane_t col_Polygon_FindPlane(const col_polygon_t* p0);
col_aabb_t col_Polygon_CreateAABB(const col_polygon_t* p0);

fix16_vec3_t col_Polygon_ClosestPoint(const col_polygon_t* p0, const fix16_vec3_t* p1);
bool col_Polygon_ContainsPoint(const col_polygon_t* p0, const fix16_vec3_t* p1);

#endif
