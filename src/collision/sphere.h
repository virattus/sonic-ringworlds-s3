#ifndef __SSEE_COLLISION_SPHERE_H__
#define __SSEE_COLLISION_SPHERE_H__


#include "collision.h"
#include "col_resp.h"

#include <yaul.h>
#include <stdbool.h>


struct COL_SPHERE
{
	fix16_vec3_t position;	
	fix16_t radius;
	fix16_t radiusSquared;
	
};


/*
 * 
 */

fix16_t col_Sphere_PointDistanceSquared(const col_sphere_t* s0, const fix16_vec3_t* p0);
fix16_t col_Sphere_PointDistance(const col_sphere_t* s0, const fix16_vec3_t* p0);
fix16_vec3_t col_Sphere_ClosestPointOnSurface(const col_sphere_t* s0, const fix16_vec3_t* p0);
bool col_Sphere_ContainsPoint(const col_sphere_t* s0, const fix16_vec3_t* p0);

bool col_Sphere_AABBIntersection(const col_sphere_t* s0, const col_aabb_t* a0, col_resp_t* resp);
bool col_Sphere_SphereIntersection(const col_sphere_t* s0, const col_sphere_t* s1, col_resp_t* resp);
bool col_Sphere_PlaneIntersection(const col_sphere_t* s0, const col_plane_t* p0, col_resp_t* resp);
bool col_Sphere_PolygonIntersection(const col_sphere_t* s0, const col_polygon_t* p0, col_resp_t* resp);

#endif
