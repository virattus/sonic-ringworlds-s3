#include "capsule.h"

#include "aabb.h"
#include "line.h"
#include "sphere.h"
#include "plane.h"
#include "polygon.h"




bool col_Capsule_ContainsPoint(const col_capsule_t* c0, const fix16_vec3_t* p0)
{
	col_line_t line;
	fix16_vec3_dup(&c0->lineStart, &line.start);
	fix16_vec3_dup(&c0->lineEnd, &line.end);
	
	fix16_vec3_t sphereCentre = col_Line_ClosestPoint(&line, p0);
	
	col_sphere_t sphere;
	fix16_vec3_dup(&sphere.position, &sphereCentre);
	sphere.radius = c0->radius;
	sphere.radiusSquared = c0->radiusSquared;
	
	return col_Sphere_ContainsPoint(&sphere, p0);
}

//TODO
bool col_Capsule_SphereIntersection(
	const col_capsule_t* c0, 
	const col_sphere_t* s0, 
	col_resp_t* resp)
{
	
	return false;
}

//TODO
bool col_Capsule_AABBIntersection(
	const col_capsule_t* c0, 
	const col_aabb_t* a0, 
	col_resp_t* resp)
{
	return false;
}

//TODO
bool col_Capsule_PlaneIntersection(
	const col_capsule_t* c0, 
	const col_plane_t* p0, 
	col_resp_t* resp)
{
	return false;
}

//TODO
bool col_Capsule_PolygonIntersection(
	const col_capsule_t* c0, 
	const col_polygon_t* p0, 
	col_resp_t* resp)
{
	return false;
}
