#include "octree.h"


#include <assert.h>
#include <stdlib.h>



//TODO make this actually build a tree, currently it's just a single node
uint32_t Octree_CreateOctree(
	octree_t* octree, 
	col_polygon_t* polygons, 
	uint16_t polygonCount, 
	uint8_t maxDepth)
{
	//Irrelevant for now
	//assert(maxDepth > 0);
	
	
	//Create just one node for now
	octree->nodes = (octree_node_t*)malloc(sizeof(octree_node_t));
	octree->children = NULL;
	octree->polygons = (octree_polygon_t*)malloc(sizeof(octree_polygon_t) * polygonCount);
	
	OctreeNode_SetLeaf(&octree->nodes[0], 0, polygonCount);
	
	
	uint16_t totalVertexCount = 0;
	
	col_aabb_t poly0Box = col_Polygon_CreateAABB(&polygons[0]);
	octree_aabb_FromColAABB(&octree->nodes[0].boundingBox, &poly0Box);
	
	for(uint16_t i = 0; i < polygonCount; i++)
	{
		col_aabb_t polyBox = col_Polygon_CreateAABB(&polygons[i]);
		octree_aabb_expandColAABB(&octree->nodes[0].boundingBox, &polyBox);
		
		octreePolygon_SetNormal(&octree->polygons[i], &polygons[i].normal);
		octree->polygons[i].vertexOffset = totalVertexCount;
		octree->polygons[i].properties = polygons[i].properties;
		
		totalVertexCount += PolyProperties_GetVertexCount(polygons[i].properties);
	}
	
	octree->vertices = (octree_vertex_t*)malloc(sizeof(octree_vertex_t) * totalVertexCount);
	
	fix16_vec3_t nodeMidpoint;
	octree_aabb_midpoint(&octree->nodes[0].boundingBox, &nodeMidpoint);
	
	uint16_t vertexOffset = 0;
	for(uint16_t i = 0; i < polygonCount; i++)
	{
		const uint16_t vertexCount = PolyProperties_GetVertexCount(polygons[i].properties);
		for(uint8_t j = 0; j < vertexCount; j++)
		{
			fix16_vec3_t vertex;
			col_Polygon_GetVertex(&polygons[i], &vertex, j);
			octreeVertex_FromFix16(&octree->vertices[vertexOffset], &vertex, &nodeMidpoint);
			vertexOffset++;
		}
	}
	
	//Return size in bytes
	return sizeof(octree_node_t) + (sizeof(octree_vertex_t) * totalVertexCount) + (sizeof(octree_polygon_t) * polygonCount);
}


uint8_t Octree_DrillDown(
	const octree_t* octree,
	const octree_node_t* node
	)
{
	uint8_t childCollisions = 0;
	
	if(OctreeNode_GetLeaf(node))
	{
		const uint8_t polygonCount = OctreeNode_GetPolygonCount(node);
		
		for(uint8_t i = 0; i < polygonCount; i++)
		{
			
		}
		
	}
	else
	{
	}
	
	return childCollisions;
}


//TODO
uint8_t Octree_SphereCollision(
	const octree_t* octree, 
	const col_sphere_t* sphere, 
	const polygon_mask_types_t sphereMask,
	col_resp_t* respRoot, 
	uint8_t maxCollisions)
{
	return 0;
}


uint8_t Octree_LineCollision(
	const octree_t* octree, 
	const col_line_t* line, 
	const polygon_mask_types_t lineMask,
	col_resp_t* respRoot, 
	uint8_t maxCollisions)
{
	return 0;
}


/*
uint8_t OctreeNode_SphereCollisionNodeTest(
	const octree_node_t* octreeRoot, 
	uint16_t currentNodeID, 
	const col_sphere_t* sphere,
	const col_polygon_mask_types_t sphereMask,
	col_resp_t* respRoot,
	uint8_t currentCollisionCount,
	uint8_t maxCollisions)
{
	//leaf node
	if(octreeRoot[currentNodeID].polygonCount)
	{
		const octree_node_t* currentNode = &octreeRoot[currentNodeID];
		uint8_t collisions = currentCollisionCount;
	
		for(uint16_t i = 0; i < currentNode->polygonCount; i++)
		{
			//Skip non relevant collisions
			if(!((currentNode->polyData[i].properties && COL_POLYGON_COLL_MASK_TYPE_MASK) && sphereMask))
			{
				continue;
			}
			
			if(col_Sphere_PolygonIntersection(sphere, &currentNode->polyData[i], &respRoot[collisions]))
			{
				collisions++;

				//Early exit
				if(collisions >= maxCollisions)
				{
					return collisions;
				}
			}
		}
		
		return collisions;
	}
	else
	{
		//TODO
		//Recursively interate through the tree
		//return Octree_SphereCollision();
		return 0;
	}	
}


uint8_t Octree_SphereCollision(
	const octree_node_t* octree, 
	const col_sphere_t* sphere, 
	const col_polygon_mask_types_t sphereMask,
	col_resp_t* respRoot, 
	uint8_t maxCollisions)
{
	assert(octree);
	assert(sphere);
	assert(respRoot);
	
	return OctreeNode_SphereCollisionNodeTest(octree, 0, sphere, sphereMask, respRoot, 0, maxCollisions);
}


uint8_t Octree_LineCollision(
	const octree_node_t* octree, 
	const col_line_t* line, 
	const col_polygon_mask_types_t lineMask,
	col_resp_t* respRoot, 
	uint8_t maxCollisions)
{
	
	return 0;
}
*/
