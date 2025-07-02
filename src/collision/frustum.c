#include "frustum.h"

#include "aabb.h"
#include "sphere.h"
#include "plane.h"
#include "polygon.h"

#include <assert.h>


fix16_vec3_t* camPosition = NULL;
fix16_vec3_t* forwardVector = NULL;
fix16_t fovThreshold = 0;


void col_Frustum_SetFOV(double FOVRadians)
{
	fovThreshold = fix16_cos(RAD2ANGLE(FOVRadians) >> 1);
}


void col_Frustum_SetPosition(const fix16_vec3_t* v0)
{
	camPosition = v0;
}


void col_Frustum_SetForwardVector(const fix16_vec3_t* v0)
{
	forwardVector = v0;
}


bool col_Frustum_PointIntersection(const fix16_vec3_t* p0)
{
	fix16_vec3_t targetPos;
	fix16_vec3_sub(p0, camPosition, &targetPos);
	fix16_vec3_normalize(&targetPos);
	
	if(fix16_vec3_dot(forwardVector, &targetPos) >= fovThreshold)
	{
		return true;
	}
	
	return false;
}

//TODO
bool col_Frustum_SphereIntersection(const col_sphere_t* s0)
{
	return false;
}

//TODO
bool col_Frustum_AABBIntersection(const col_aabb_t* a0)
{
	
	
	return false;
}

//TODO
bool col_Frustum_PolygonIntersection(const col_polygon_t* p0)
{
	
	return false;
}

