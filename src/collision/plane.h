#ifndef __SSEE_COLLISION_PLANE_H__
#define __SSEE_COLLISION_PLANE_H__


#include "collision.h"
#include "col_resp.h"

#include <yaul.h>
#include <stdbool.h>


struct COL_PLANE
{
	fix16_t distance;
	fix16_vec3_t normal;
	
};


//TODO rename this because it's confusing (This finds the distance of the plane from the origin)
fix16_t col_Plane_CalculateDistance(const fix16_vec3_t* normal, const fix16_vec3_t* point);

/*
 * For test point (x,y,z), if plane equation
 * > 0: point on ‘front’ side (in direction of normal),
 * < 0: on ‘back’ side
 * = 0: directly on plane
 */
fix16_t col_Plane_PointDistance(const col_plane_t* _plane, const fix16_vec3_t* _point);

fix16_vec3_t col_Plane_NearestPoint(const col_plane_t* _plane, const fix16_vec3_t* _point);




#endif
