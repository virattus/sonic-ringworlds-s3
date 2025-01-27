#include "plane.h"

#include <assert.h>


//Plane equation is (ax + by + cz + d) == 0
//solve for d instead
fix16_t col_Plane_CalculateDistance(const fix16_vec3_t* normal, const fix16_vec3_t* point)
{
	return -(fix16_mul(normal->x, point->x) 
		+ fix16_mul(normal->y, point->y) 
		+ fix16_mul(normal->z, point->z));
}


fix16_t col_Plane_PointDistance(const col_plane_t* _plane, const fix16_vec3_t* _point)
{
	return fix16_vec3_dot(_point, &_plane->normal) - _plane->distance;
}


fix16_vec3_t col_Plane_NearestPoint(const col_plane_t* _plane, const fix16_vec3_t* _point)
{
	fix16_t dist = fix16_vec3_dot(&_plane->normal, _point) - _plane->distance;
	fix16_vec3_t result;
	fix16_vec3_dup(&_plane->normal, &result);
	fix16_vec3_scale(dist, &result);
	fix16_vec3_t result2; //restrict parameters
	fix16_vec3_sub(_point, &result, &result2);
	 
	return result2;
}
