#ifndef __SSEE_COLLISION_VIEW_FRUSTUM_H__
#define __SSEE_COLLISION_VIEW_FRUSTUM_H__


#include "collision.h"
#include "col_resp.h"

#include "plane.h"
#include "octree.h"

#include <yaul.h>
#include <stdint.h>
#include <stdbool.h>


void col_Frustum_SetFOV(double FOVRadians);
void col_Frustum_SetPosition(const fix16_vec3_t* v0);
void col_Frustum_SetForwardVector(const fix16_vec3_t* v0);

bool col_Frustum_PointIntersection(const fix16_vec3_t* p0);
bool col_Frustum_SphereIntersection(const col_sphere_t* s0);
bool col_Frustum_AABBIntersection(const col_aabb_t* a0);
bool col_Frustum_PolygonIntersection(const col_polygon_t* p0);




#endif
