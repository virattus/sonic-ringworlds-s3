#include "sphere.h"

#include "aabb.h"
#include "plane.h"
#include "polygon.h"

#include <assert.h>


fix16_t col_Sphere_PointDistanceSquared(const col_sphere_t* s0, const fix16_vec3_t* p0)
{
	fix16_vec3_t direction;
	fix16_vec3_sub(&s0->position, p0, &direction);
	return fix16_vec3_sqr_length(&direction);
}


fix16_t col_Sphere_PointDistance(const col_sphere_t* s0, const fix16_vec3_t* p0)
{
	fix16_vec3_t direction;
	fix16_vec3_sub(&s0->position, p0, &direction);
	return fix16_vec3_length(&direction);
}


fix16_vec3_t col_Sphere_ClosestPointOnSurface(const col_sphere_t* s0, const fix16_vec3_t* p0)
{
	fix16_vec3_t direction;
	fix16_vec3_sub(p0, &s0->position, &direction);
	fix16_vec3_normalize(&direction);
	fix16_vec3_scale(s0->radius, &direction);
	
	return direction;
}


bool col_Sphere_ContainsPoint(const col_sphere_t* s0, const fix16_vec3_t* p0)
{
	return (col_Sphere_PointDistanceSquared(s0, p0) < s0->radiusSquared);
}


bool col_Sphere_AABBIntersection(const col_sphere_t* s0, const col_aabb_t* a0, col_resp_t* resp)
{
	assert(s0);
	assert(a0);
	assert(resp);
	
	fix16_vec3_t closestPoint = col_AABB_GetClosestPoint(a0, &s0->position);
	
	bool contains = col_Sphere_ContainsPoint(s0, &closestPoint);
	
	//TODO collision responce
	
	return contains;
}


bool col_Sphere_SphereIntersection(const col_sphere_t* s0, const col_sphere_t* s1, col_resp_t* resp)
{
	assert(s0);
	assert(s1);
	assert(resp);
	
	fix16_vec3_t direction;
	fix16_vec3_sub(&s0->position, &s1->position, &direction);
	fix16_t distance = fix16_vec3_sqr_length(&direction);
	
	if(distance < (s0->radiusSquared + s1->radiusSquared))
	{
		fix16_vec3_normalize(&direction);
		
		resp->penetration = distance;
		resp->normal = direction;
		
		return true;
	}
	
	return false;
}


bool col_Sphere_PlaneIntersection(const col_sphere_t* s0, const col_plane_t* p0, col_resp_t* resp)
{
	assert(s0);
	assert(p0);
	
	fix16_t distance = col_Plane_PointDistance(p0, &s0->position);
	
	if(fix16_abs(distance) <= s0->radius)
	{
		resp->normal = p0->normal;
		resp->penetration = -(distance - s0->radius);

		return true;
	}
	
	return false;
}


bool col_Sphere_PolygonIntersection(const col_sphere_t* s0, const col_polygon_t* p0, col_resp_t* resp)
{
	assert(s0);
	assert(p0);
	assert(resp);
	
	col_plane_t polyPlane = col_Polygon_FindPlane(p0);
	
	if(col_Sphere_PlaneIntersection(s0, &polyPlane, resp))
	{
		if(col_Polygon_ContainsPoint(p0, &resp->position))
		{
			//col_Polygon_GetNormal(p0, &resp->normal);
			return true;
		}
		else
		{
			//Sphere isn't directly over the polygon, check the edges
			fix16_vec3_t closestPoint = col_Polygon_ClosestPoint(p0, &resp->position);
			
			if(col_Sphere_ContainsPoint(s0, &closestPoint))
			{
				resp->penetration = col_Sphere_PointDistance(s0, &closestPoint);
				
				return true;
			}
		}
	}
	
	return false;
}
