#ifndef __RINGWORLDS_OCTREE_H__
#define __RINGWORLDS_OCTREE_H__



#include "collision.h"
#include "col_resp.h"

#include "octreeAABB.h"
#include "octreePolygon.h"
#include "aabb.h"
#include "polygon.h"

#include <yaul.h>
#include <stdint.h>
#include <stdbool.h>


#define OCTREE_NODE_LEAF_FLAG		(0x8000)	//MSB
#define OCTREE_NODE_COLLISION_MASK	(0x0)		//TODO
#define OCTREE_NODE_POLYGON_MASK	(0x000F)	//MAX of 16 polygons per node

#define OCTREE_CHILDREN_CHILD_COUNT	(8)
#define OCTREE_CHILDREN_NULL_NODE	(0xFFFF)



typedef struct
{
	octree_aabb_t boundingBox;
	uint16_t dataOffset;
	uint16_t properties;
	/* Parent Nodes:
	 * 
	 * Leaf Nodes:
	 * 	Last 4 bits: polygon count
	 * 
	 */

} octree_node_t;


typedef struct
{
	uint16_t childNodes[OCTREE_CHILDREN_CHILD_COUNT];
	
} octree_children_t;


typedef struct
{
	octree_node_t* nodes;
	octree_children_t* children;
	octree_vertex_t* vertices;
	octree_polygon_t* polygons;
	
} octree_t;


static inline uint8_t OctreeNode_GetCollisionMask(const octree_node_t* node)
{
	assert(0); //TODO
	
	return 0;
}


static inline uint8_t OctreeNode_GetPolygonCount(const octree_node_t* node)
{	
	return node->properties & OCTREE_NODE_POLYGON_MASK;
}


static inline void OctreeNode_SetPolygonCount(octree_node_t* node, uint8_t count)
{
	node->properties = 
		(node->properties & ~OCTREE_NODE_POLYGON_MASK) +
		(count & OCTREE_NODE_POLYGON_MASK);
}


static inline bool OctreeNode_GetLeaf(const octree_node_t* node)
{
	return node->properties & OCTREE_NODE_LEAF_FLAG;
}


static inline void OctreeNode_SetLeaf(octree_node_t* node, uint16_t polygonOffset, uint8_t polygonCount)
{
	node->dataOffset = polygonOffset;
	
	node->properties = (node->properties & ~OCTREE_NODE_POLYGON_MASK);
	node->properties += polygonCount << 8;
	
	node->properties |= OCTREE_NODE_LEAF_FLAG;
}


static inline void OctreeNode_SetContainer(octree_node_t* node, uint16_t childrenOffset)
{
	//TODO set leaf flag to zero
	node->dataOffset = childrenOffset;
}


/*
 * Returns size of entire octree in bytes
 */
uint32_t Octree_CreateOctree(
	octree_t* octree, 
	col_polygon_t* polygons, 
	uint16_t polygonCount, 
	uint8_t maxDepth);

void Octree_GenerateNode(uint8_t currentDepth, uint8_t maxDepth);

/*
 * Following collision tests return number of collisions detected
 */
uint8_t Octree_SphereCollision(
	const octree_t* octree, 
	const col_sphere_t* sphere, 
	const polygon_mask_types_t sphereMask,
	col_resp_t* respRoot, 
	uint8_t maxCollisions);


uint8_t Octree_LineCollision(
	const octree_t* octree, 
	const col_line_t* line, 
	const polygon_mask_types_t lineMask,
	col_resp_t* respRoot, 
	uint8_t maxCollisions);

#endif
