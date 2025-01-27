#ifndef __SSEE_COLLISION_CAPSULE_H__
#define __SSEE_COLLISION_CAPSULE_H__


#include "collision.h"
#include "col_resp.h"

#include <yaul.h>
#include <stdbool.h>


struct COL_CAPSULE
{	
	fix16_t radius;
	fix16_t radiusSquared;
	
	fix16_vec3_t lineStart;
	fix16_vec3_t lineEnd;
	
};


bool col_Capsule_ContainsPoint(const col_capsule_t* c0, const fix16_vec3_t* p0);

bool col_Capsule_SphereIntersection(const col_capsule_t* c0, const col_sphere_t* s0, col_resp_t* resp);
bool col_Capsule_AABBIntersection(const col_capsule_t* c0, const col_aabb_t* a0, col_resp_t* resp);
bool col_Capsule_PlaneIntersection(const col_capsule_t* c0, const col_plane_t* p0, col_resp_t* resp);
bool col_Capsule_PolygonIntersection(const col_capsule_t* c0, const col_polygon_t* p0, col_resp_t* resp);



#endif
