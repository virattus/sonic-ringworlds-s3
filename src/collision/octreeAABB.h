#ifndef __SSEE_COLLISION_OCTREE_AABB_H__
#define __SSEE_COLLISION_OCTREE_AABB_H__


#include "collision.h"
#include "col_resp.h"

#include "aabb.h"

#include <yaul.h>
#include <stdint.h>
#include <stdbool.h>



typedef struct OCTREE_AABB
{
	int16_t minX;
	int16_t minY;
	int16_t minZ;
	
	int16_t maxX;
	int16_t maxY;
	int16_t maxZ;
	
} octree_aabb_t;


static inline void octree_aabb_ToColAABB(const octree_aabb_t* o0, col_aabb_t* a0)
{
	a0->minExtents = (fix16_vec3_t){
		.x = (o0->minX << 16),
		.y = (o0->minY << 16),
		.z = (o0->minZ << 16),
	};
	
	a0->maxExtents = (fix16_vec3_t){
		.x = (o0->maxX << 16),
		.y = (o0->maxY << 16),
		.z = (o0->maxZ << 16),
	};
}


static inline void octree_aabb_FromColAABB(octree_aabb_t* o0, const col_aabb_t* a0)
{
	o0->minX = a0->minExtents.x >> 16;
	o0->minY = a0->minExtents.y >> 16;
	o0->minZ = a0->minExtents.z >> 16;

	o0->maxX = a0->maxExtents.x >> 16;
	o0->maxY = a0->maxExtents.y >> 16;
	o0->maxZ = a0->maxExtents.z >> 16;
}


static inline void octree_aabb_expandColAABB(octree_aabb_t* o0, const col_aabb_t* a0)
{
	o0->minX = MIN(o0->minX, a0->minExtents.x >> 16);
	o0->minY = MIN(o0->minY, a0->minExtents.y >> 16);
	o0->minZ = MIN(o0->minZ, a0->minExtents.z >> 16);

	o0->maxX = MAX(o0->maxX, a0->maxExtents.x >> 16);
	o0->maxY = MAX(o0->maxY, a0->maxExtents.y >> 16);
	o0->maxZ = MAX(o0->maxZ, a0->maxExtents.z >> 16);
}


static inline void octree_aabb_midpoint(const octree_aabb_t* o0, fix16_vec3_t* mp)
{
	mp->x = ((((o0->maxX - o0->minX) >> 1) + o0->minX) << 16);
	mp->y = ((((o0->maxY - o0->minY) >> 1) + o0->minY) << 16);
	mp->z = ((((o0->maxZ - o0->minZ) >> 1) + o0->minZ) << 16);	
}


#endif
