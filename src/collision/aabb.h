#ifndef __SSEE_COLLISION_AABB_H__
#define __SSEE_COLLISION_AABB_H__


#include "collision.h"
#include "col_resp.h"

#include <yaul.h>
#include <stdint.h>
#include <stdbool.h>


struct COL_AABB
{
	fix16_vec3_t minExtents;
	fix16_vec3_t maxExtents;
	
};


fix16_vec3_t col_AABB_GetMidpoint(const col_aabb_t* a0);
fix16_vec3_t col_AABB_GetClosestPoint(const col_aabb_t* a0, const fix16_vec3_t* p0);

bool col_AABB_ContainsPoint(const col_aabb_t* a0, const fix16_vec3_t* p0);

bool col_AABB_AABBIntersection(const col_aabb_t* a0, const col_aabb_t* a1, col_resp_t* resp);
bool col_AABB_PlaneIntersection(const col_aabb_t* a0, const col_plane_t* p0);

//increases size to contain listed point, should probably rename to something more sensible
void col_AABB_ExpandSizePoint(col_aabb_t* a0, const fix16_vec3_t* p0);
//Likewise, but this one takes in another AABB
void col_AABB_ExpandSizeAABB(col_aabb_t* a0, const col_aabb_t* a1);

#endif
