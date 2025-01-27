#include "polygon.h"

#include "aabb.h"
#include "sphere.h"
#include "plane.h"
#include "line.h"

#include <assert.h>


static inline void col_Polygon_GenerateUnnormalisedNormal(
	const fix16_vec3_t* p0,
	const fix16_vec3_t* p1,
	const fix16_vec3_t* p2,
	fix16_vec3_t* result)
{
	assert(p0);
	assert(p1);
	assert(p2);
	assert(result);
	
	fix16_vec3_t a, b;
	fix16_vec3_sub(p1, p0, &a);
	fix16_vec3_sub(p2, p0, &b);
	fix16_vec3_cross(&a, &b, result);
}


static inline void col_Polygon_GenerateEdgeNormal(
	const fix16_vec3_t* p1,
	const fix16_vec3_t* start, 
	const fix16_vec3_t* end, 
	fix16_vec3_t* result)
{
	col_Polygon_GenerateUnnormalisedNormal(p1, start, end, result);
	fix16_vec3_normalize(result);
}


void col_Polygon_GenerateNormal(const col_polygon_t* p0, fix16_vec3_t* dest)
{
	assert(p0);
	
	col_Polygon_GenerateUnnormalisedNormal(&p0->offset[0], &p0->offset[1], &p0->offset[2], dest);
	fix16_vec3_normalize(dest);	
}


//TODO
void col_Polygon_SetNormal(col_polygon_t* p0, const fix16_vec3_t* norm)
{
	assert(p0);
	
	fix16_vec3_dup(norm, &p0->normal);
}


//TODO
void col_PolygonGetNormal(const col_polygon_t* p0, fix16_vec3_t* dest)
{
	assert(p0);
	
	fix16_vec3_dup(&p0->normal, dest);
}


fix16_vec3_t col_Polygon_GetNormal(const col_polygon_t* p0)
{
	assert(p0);
	
	fix16_vec3_t normal;
	col_PolygonGetNormal(p0, &normal);
	return normal;
}


void col_Polygon_DebugDraw(const col_polygon_t* p0, const rgb1555_t col)
{
	assert(p0);
	
	const uint8_t vertexCount = PolyProperties_GetVertexCount(p0->properties);
	
	fix16_mat43_t world = {0};
	fix16_mat33_identity(&world.rotation);
	
	fix16_vec3_t positions[6];
	for(uint8_t i = 0; i < vertexCount; i++)
	{
		fix16_vec3_dup(&p0->offset[i], &positions[i]);
	}
	
	xform_t transforms[6];
	
	render_points_xform(&world, positions, transforms, vertexCount);
	
	vdp1_cmdt_t cmdt = {0};	
	
	vdp1_cmdt_line_set(&cmdt);
	vdp1_cmdt_draw_mode_t drawMode = 
	{
		.raw = 0x0000,
	};
	vdp1_cmdt_draw_mode_set(&cmdt, drawMode);
	vdp1_cmdt_color_set(&cmdt, col);
	
	for(uint8_t i = 0; i < vertexCount; i++)
	{
		const uint8_t nextPoint = (i + 1) % vertexCount;
		
		int16_vec2_dup(&transforms[i].screen_point, &cmdt.cmd_vertices[i]);
		int16_vec2_dup(&transforms[nextPoint].screen_point, &cmdt.cmd_vertices[nextPoint]);
		
		fix16_t depth = (transforms[i].depth_value + transforms[nextPoint].depth_value) >> 1;

		render_cmdt_insert(&cmdt, depth);
	}	
}


col_plane_t col_Polygon_FindPlane(const col_polygon_t* p0)
{
	assert(p0);
	
	col_plane_t retval = {0};
	
	col_PolygonGetNormal(p0, &retval.normal);
	
	retval.distance = col_Plane_CalculateDistance(&p0->normal, &p0->offset[0]);
	
	return retval;
}


col_aabb_t col_Polygon_CreateAABB(const col_polygon_t* p0)
{
	assert(p0);
	
	col_aabb_t retval = {0};
	fix16_vec3_dup(&p0->offset[0], &retval.minExtents);
	fix16_vec3_dup(&p0->offset[0], &retval.maxExtents);
	
	const uint8_t vertexCount = PolyProperties_GetVertexCount(p0->properties);
	
	for(uint8_t i = 1; i < vertexCount; i++)
	{
		col_AABB_ExpandSizePoint(&retval, &p0->offset[i]);
	}
	
	return retval;
}


fix16_vec3_t col_Polygon_ClosestPoint(const col_polygon_t* p0, const fix16_vec3_t* p1)
{
	assert(p0);
	assert(p1);
	
	fix16_vec3_t closestEdgePoint;
	fix16_t closestEdgeDist;
	
	//First entry
	{
		col_line_t l0 = 
		{
			.start = p0->offset[0],
			.end = p0->offset[1],
		};
		
		//TODO find out if we need to use the square root length
		closestEdgePoint = col_Line_ClosestPoint(&l0, p1);
		fix16_vec3_t temp;
		fix16_vec3_sub(&closestEdgePoint, p1, &temp);
		closestEdgeDist = fix16_vec3_sqr_length(&temp);
	}
	
	const uint8_t vertexCount = PolyProperties_GetVertexCount(p0->properties);
	
	for(uint8_t i = 1; i < vertexCount - 1; i++)
	{
		col_line_t l0 = 
		{
			.start = p0->offset[i],
			.end = p0->offset[i + 1],
		};

		fix16_vec3_t newEdgePoint = col_Line_ClosestPoint(&l0, p1);
		fix16_vec3_t temp;
		fix16_vec3_sub(&newEdgePoint, p1, &temp);
		fix16_t newDist = fix16_vec3_sqr_length(&temp);
		
		if(newDist < closestEdgeDist)
		{
			closestEdgeDist = newDist;
			fix16_vec3_dup(&newEdgePoint, &closestEdgePoint);
		}
	}
	
	return closestEdgePoint;
}


bool col_Polygon_ContainsPoint(const col_polygon_t* p0, const fix16_vec3_t* p1)
{
	assert(p0);
	assert(p1);
	
	const uint8_t vertexCount = col_Polygon_GetVertexCount(p0);
		
	//Generate cross product for vertex n to zero, we'll compare against this one
	fix16_vec3_t lastNormal;
	col_Polygon_GenerateEdgeNormal(
		p1, 
		&p0->offset[vertexCount - 1], 
		&p0->offset[0], 
		&lastNormal);
	
	for(uint8_t i = 1; i < vertexCount; i++)
	{
		fix16_vec3_t newNormal;
		col_Polygon_GenerateEdgeNormal(
			p1,
			&p0->offset[i - 1],
			&p0->offset[i], 
			&newNormal);

		fix16_t dot = fix16_vec3_dot(&newNormal, &lastNormal);	
		
		if(dot < 0)
		{
			return false;
		}
	}
	
	return true;
}


bool col_Polygon_SphereIntersection(const col_polygon_t* p0, const col_sphere_t* s0, col_resp_t* resp)
{
	assert(p0);
	assert(s0);
	assert(resp);
	
	
	fix16_t d = col_Plane_CalculateDistance(&p0->normal, &s0->position);
	
	char fx16Buff[64];
	fix16_str(d, fx16Buff, 7);
	//dbgio_printf("distance: %s\n", fx16Buff);
	
	
	//If sphere doesn't collide with polygon's plane, we're done
	if(fix16_abs(d) > s0->radius)
	{
		//dbgio_printf("no collision\n");
				
		return false;
	}
	
	col_plane_t plane = col_Polygon_FindPlane(p0);
	
	fix16_vec3_t closestPointOnPlane = col_Plane_NearestPoint(&plane, &s0->position);
	
	if(col_Polygon_ContainsPoint(p0, &closestPointOnPlane))
	{
		//If the sphere is directly over the polygon, then we can skip edge checks
		
		//dbgio_printf("non-edge collision\n");
		fix16_vec3_dup(&closestPointOnPlane, &resp->position);
		fix16_vec3_dup(&p0->normal, &resp->normal);
		resp->penetration = d;
		
		return true;
	}
	
	//sphere isn't over polygon, do edge checks and penetrate against the edges instead
		
	fix16_vec3_t closestPoint = col_Polygon_ClosestPoint(p0, &s0->position);
	
	if(!col_Sphere_ContainsPoint(s0, &closestPoint))
	{
		//dbgio_printf("closest point not inside sphere\n");
				
		return false;
	}
	
	
	//retval.penetration = col_SpherePointDistance(s0, &closestPoint);
	//retval.penetration = -(col_PlanePointDistance(&(col_plane_t){ .normal = p0->normal, .distance = d }, &s0->position) - s0->radius);
	resp->penetration = d;
	
	fix16_vec3_dup(&closestPoint, &resp->position);
	
	//TODO find out if we can get away with always returning the polygon normal or if we 
	//need to generate a normal for edges
	fix16_vec3_dup(&p0->normal, &resp->normal);
	
	fix16_vec3_str(&closestPoint, fx16Buff, 3);
	
	//dbgio_printf("Sph-Poly coll:\nClosest Point: %s\n", fx16Buff);
	
	return true;
}
