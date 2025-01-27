#include "line.h"

#include "sphere.h"
#include "plane.h"
#include "polygon.h"

#include <yaul.h>
#include <mic3d.h>
#include "assert.h"


#define LINE_SIGN_BIT		(0x80000000)


void col_Line_PlaneIntersectionPoint(const col_line_t* l0, const col_plane_t* p0, fix16_vec3_t* dest)
{
	fix16_vec3_t lineNormal;
	fix16_vec3_sub(&l0->end, &l0->start, &lineNormal);
	fix16_vec3_normalize(&lineNormal);
	
	fix16_t denominator = fix16_vec3_dot(&lineNormal, &p0->normal);
	
	//If we're gonna divide by zero, return the first point
	if(!denominator)
	{
		fix16_vec3_dup(&l0->start, dest);
		return;
	}
	
	
	fix16_t numerator = 
		-(fix16_mul(p0->normal.x, l0->start.x) + 
		fix16_mul(p0->normal.y, l0->start.y) + 
		fix16_mul(p0->normal.z, l0->start.z) + 
		p0->distance);
	
	
	fix16_t dist = fix16_div(numerator, denominator);
	
	fix16_vec3_scale(dist, &lineNormal);
	
	fix16_vec3_add(&l0->start, &lineNormal, dest);
}


void col_Line_DebugDraw(const col_line_t* l0, const rgb1555_t col)
{
	fix16_mat43_t world = {0};
	fix16_mat33_identity(&world.rotation);
	
	fix16_vec3_t positions[2];
	fix16_vec3_dup(&l0->start, &positions[0]);
	fix16_vec3_dup(&l0->end, &positions[1]);
	
	xform_t transforms[2];
	
	
	render_points_xform(&world, positions, transforms, 2);
	
	vdp1_cmdt_t cmdt = {0};	
	
	vdp1_cmdt_line_set(&cmdt);
	vdp1_cmdt_draw_mode_t drawMode = 
	{
		.raw = 0x0000,
	};
	vdp1_cmdt_draw_mode_set(&cmdt, drawMode);
	vdp1_cmdt_color_set(&cmdt, col);
	
	
	int16_vec2_dup(&transforms[0].screen_point, &cmdt.cmd_vertices[0]);
	int16_vec2_dup(&transforms[1].screen_point, &cmdt.cmd_vertices[1]);
	
	fix16_t depth = (transforms[0].depth_value + transforms[1].depth_value) >> 1;

	/*
	char dbgio[64];
	fix16_str(transforms[0].depth_value, dbgio, 7);
	dbgio_printf("line z values: %s", dbgio);
	
	fix16_str(transforms[1].depth_value, dbgio, 7);
	dbgio_printf(" %s \n", dbgio);
	
	fix16_str(depth, dbgio, 7);
	dbgio_printf("line depth avg: %s\n", dbgio);	
	*/
	
	render_cmdt_insert(&cmdt, depth);
}


fix16_vec3_t col_Line_ClosestPoint(const col_line_t* l0, const fix16_vec3_t* p0)
{
	assert(l0);
	assert(p0);
	
	fix16_vec3_t v0;
	fix16_vec3_sub(p0, &l0->start, &v0);
	
	fix16_vec3_t lineDirNorm;
	fix16_vec3_sub(&l0->end, &l0->start, &lineDirNorm);

	//Before it's normalised lol
	fix16_t d = fix16_vec3_length(&lineDirNorm);	
	
	fix16_vec3_normalize(&lineDirNorm);
	
	
	fix16_t t = fix16_vec3_dot(&v0, &lineDirNorm);
	
	if(t < 0)
	{
		return l0->start;
	}
	else if(t >= d)
	{
		return l0->end;
	}
	
	fix16_vec3_t v2 = lineDirNorm;
	fix16_vec3_scale(t, &v2);
	
	fix16_vec3_t v3; //gotta do this, the add function uses restrict
	
	fix16_vec3_add(&l0->start, &v2, &v3);
	
	return v3;
}


bool col_Line_PlaneCollision(const col_line_t* l0, const col_plane_t* p0, col_resp_t* resp)
{
	assert(l0);
	assert(p0);
	
	fix16_t dist1 = col_Plane_CalculateDistance(&p0->normal, &l0->start);
	
	//If first point is ON plane, return early
	if(!dist1)
	{		
		fix16_vec3_dup(&l0->start, &resp->position);
		fix16_vec3_dup(&p0->normal, &resp->normal);
		resp->penetration = 0;
		
		return true;
	}
	
	fix16_t dist2 = col_Plane_CalculateDistance(&p0->normal, &l0->end);
	
	//end point is on plane
	if(!dist2)
	{		
		fix16_vec3_dup(&l0->end, &resp->position);
		fix16_vec3_dup(&p0->normal, &resp->normal);
		resp->penetration = dist1;
		
		return true;
	}
	
	if((dist1 & LINE_SIGN_BIT) != (dist2 & LINE_SIGN_BIT))
	{		
		col_Line_PlaneIntersectionPoint(l0, p0, &resp->position);
		fix16_vec3_dup(&p0->normal, &resp->normal);
		resp->penetration = dist1;
		
		return true;
	}
	
	return false;
}


bool col_Line_PolygonCollision(const col_line_t* l0, const col_polygon_t* p0, col_resp_t* resp)
{	
	assert(l0);
	assert(p0);
	
	col_plane_t polyPlane = col_Polygon_FindPlane(p0);
	
	if(col_Line_PlaneCollision(l0, &polyPlane, resp))
	{	
		if(col_Polygon_ContainsPoint(p0, &resp->position))
		{
			return true;
		}
	}
	
	return false;
}


bool col_Line_SphereCollision(const col_line_t* l0, const col_sphere_t* s0, col_resp_t* resp)
{
	assert(l0);
	assert(s0);
	assert(resp);
		
	fix16_vec3_t closestPoint = col_Line_ClosestPoint(l0, &s0->position);
	
	fix16_vec3_t subPoint;
	fix16_vec3_sub(&closestPoint, &s0->position, &subPoint);
	fix16_t dist = fix16_vec3_sqr_length(&subPoint);
	
	if(dist < s0->radiusSquared)
	{
		fix16_vec3_dup(&closestPoint, &resp->position);
		//TODO calculate normal
		
		resp->penetration = dist;
		return true;
	}

	return false;
}
