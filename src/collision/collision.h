#ifndef __SSEE_COLLISION_H__
#define __SSEE_COLLISION_H__


#include <yaul.h>


struct COL_AABB;
struct COL_CAPSULE;
struct COL_LINE;
struct COL_PLANE;
struct COL_POLYGON;
struct COL_SPHERE;



typedef struct COL_AABB 	col_aabb_t;
typedef struct COL_CAPSULE 	col_capsule_t;
typedef struct COL_LINE 	col_line_t;
typedef struct COL_PLANE 	col_plane_t;
typedef struct COL_POLYGON 	col_polygon_t;
typedef struct COL_SPHERE 	col_sphere_t;


//Taken from the fix16 master 
static inline fix16_t fix16_abs(fix16_t x)
{ 
	return (x < 0 ? -x : x); 
}




#endif
