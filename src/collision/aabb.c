#include "aabb.h"

#include "plane.h"

#include <assert.h>


//Special thanks to Roberto Duarte for help with this:
//https://github.com/robertoduarte/SaturnMathPP/blob/main/impl/fxp.hpp


static inline bool AxisIntersection(fix16_t min0, fix16_t max0, fix16_t min1, fix16_t max1, fix16_t* overlap)
{
	fix16_t d0 = max1 - min0;
	fix16_t d1 = max0 - min1;
	
	if((d0 < 0) || (d1 < 0))
	{
		return false;
	}
	
	if(d0 < d1)
	{
		overlap = d0;
	}
	else
	{
		overlap = -d1;
	}
	
	return true;
}


fix16_vec3_t col_AABB_GetMidpoint(const col_aabb_t* a0)
{
	fix16_vec3_t retval;
	fix16_vec3_t extents;
	fix16_vec3_sub(&a0->maxExtents, &a0->minExtents, &extents);
	
	//Divide by 2
	extents.x = extents.x >> 1;
	extents.y = extents.y >> 1;
	extents.z = extents.z >> 1;
	
	fix16_vec3_add(&a0->minExtents, &extents, &retval);
	
	return retval;
}


fix16_vec3_t col_AABB_GetClosestPoint(const col_aabb_t* a0, const fix16_vec3_t* p0)
{
	assert(a0);
	assert(p0);
	
	fix16_vec3_t point = {
		.x = max(a0->minExtents.x, min(p0->x, a0->maxExtents.x)),
		.y = max(a0->minExtents.y, min(p0->y, a0->maxExtents.y)),
		.z = max(a0->minExtents.z, min(p0->z, a0->maxExtents.z))		
	};
	
	return point;
}


bool col_AABB_ContainsPoint(const col_aabb_t* a0, const fix16_vec3_t* p0)
{
	assert(a0);
	assert(p0);
	
	return (p0->x <= a0->maxExtents.x &&
			p0->x >= a0->minExtents.x &&
			p0->y <= a0->maxExtents.y &&
			p0->y >= a0->minExtents.y &&
			p0->z <= a0->maxExtents.z &&
			p0->z >= a0->minExtents.z);
}


bool col_AABB_AABBIntersection(const col_aabb_t* a0, const col_aabb_t* a1, col_resp_t* resp)
{
	assert(a0);
	assert(a1);
	assert(resp);
	
	fix16_vec3_t collNormal;
	if(!AxisIntersection(a0->minExtents.x, a0->maxExtents.x, a1->minExtents.x, a1->maxExtents.x, &collNormal.x))
	{
		return false;
	}
	if(!AxisIntersection(a0->minExtents.y, a0->maxExtents.y, a1->minExtents.y, a1->maxExtents.y, &collNormal.y))
	{
		return false;
	}
	if(!AxisIntersection(a0->minExtents.z, a0->maxExtents.z, a1->minExtents.z, a1->maxExtents.z, &collNormal.z))
	{
		return false;
	}
		
	fix16_vec3_zero(&resp->position); //Don't record penetration position
	
	//Find axis of smallest penetration and return only that one
	fix16_t minPenetration = abs(collNormal.x);
	
	if(abs(collNormal.y) < minPenetration)
	{
		minPenetration = abs(collNormal.y);
		collNormal.x = 0;
	}
	else
	{
		collNormal.y = 0;
	}
	if(abs(collNormal.z) < minPenetration)
	{
		collNormal.x = 0;
		collNormal.y = 0;
	}
	else
	{
		collNormal.z = 0;
	}
	
	fix16_vec3_dup(&collNormal, &resp->normal);
	resp->penetration = minPenetration;
	
	return true;
}


bool col_AABB_PlaneIntersection(const col_aabb_t* a0, const col_plane_t* p0)
{
	assert(a0);
	assert(p0);
	
	fix16_vec3_t testVertex = {
		.x = (p0->normal.x >= 0) ? a0->minExtents.x : a0->maxExtents.x,
		.y = (p0->normal.y >= 0) ? a0->minExtents.y : a0->maxExtents.y,
		.z = (p0->normal.z >= 0) ? a0->minExtents.z : a0->maxExtents.z,		
	};
	
	return col_Plane_CalculateDistance(&p0->normal, &testVertex) >= 0;
}


void col_AABB_ExpandSizePoint(col_aabb_t* a0, const fix16_vec3_t* p0)
{
	a0->minExtents.x = min(a0->minExtents.x, p0->x);
	a0->minExtents.y = min(a0->minExtents.y, p0->y);
	a0->minExtents.z = min(a0->minExtents.z, p0->z);
	
	a0->maxExtents.x = min(a0->maxExtents.x, p0->x);
	a0->maxExtents.y = min(a0->maxExtents.y, p0->y);
	a0->maxExtents.z = min(a0->maxExtents.z, p0->z);	
}


void col_AABB_ExpandSizeAABB(col_aabb_t* a0, const col_aabb_t* a1)
{
	a0->minExtents.x = min(a0->minExtents.x, a1->minExtents.x);
	a0->minExtents.y = min(a0->minExtents.y, a1->minExtents.y);
	a0->minExtents.z = min(a0->minExtents.z, a1->minExtents.z);
	
	a0->maxExtents.x = min(a0->maxExtents.x, a1->maxExtents.x);
	a0->maxExtents.y = min(a0->maxExtents.y, a1->maxExtents.y);
	a0->maxExtents.z = min(a0->maxExtents.z, a1->maxExtents.z);	
}
