#ifndef __SSEE_COLLISION_LINE_H__
#define __SSEE_COLLISION_LINE_H__


#include <yaul.h>

#include "collision.h"
#include "col_resp.h"


struct COL_LINE
{
	fix16_vec3_t start;
	fix16_vec3_t end;
	
};

void col_Line_DebugDraw(const col_line_t* l0, const rgb1555_t col);

fix16_vec3_t col_Line_ClosestPoint(const col_line_t* l0, const fix16_vec3_t* p0);

bool col_Line_PlaneCollision(const col_line_t* l0, const col_plane_t* p0, col_resp_t* resp);
bool col_Line_PolygonCollision(const col_line_t* l0, const col_polygon_t* p0, col_resp_t* resp);
bool col_Line_SphereCollision(const col_line_t* l0, const col_sphere_t* s0, col_resp_t* resp);


#endif
