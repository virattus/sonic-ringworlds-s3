#ifndef __SSEE_COLLISION_OCTREE_POLYGON_H__
#define __SSEE_COLLISION_OCTREE_POLYGON_H__


#include "polygonProperties.h"
#include "polygon.h"

#include <yaul.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct OCTREE_VERTEX
{
	int16_t x;
	int16_t y;
	int16_t z;
	int16_t additional;
	
} octree_vertex_t;


static inline void octreeVertex_FromFix16(octree_vertex_t* v0, const fix16_vec3_t* v1, const fix16_vec3_t* offset)
{
	v0->x = (v1->x - offset->x) >> 8;
	v0->y = (v1->y - offset->y) >> 8;
	v0->z = (v1->z - offset->z) >> 8;
	//TODO additional
}


static inline void octreeVertex_ToFix16(const octree_vertex_t* v0, fix16_vec3_t* v1, const fix16_vec3_t* offset)
{
	v1->x = ((v0->x << 8) + offset->x);
	v1->y = ((v0->y << 8) + offset->y);
	v1->z = ((v0->z << 8) + offset->z);
	//TODO additional
}


#define OCTREE_POLYGON_FACE_NORMAL_X		(0x00FF0000)
#define OCTREE_POLYGON_FACE_NORMAL_Y		(0x0000FF00)
#define OCTREE_POLYGON_FACE_NORMAL_Z		(0x000000FF)
#define OCTREE_POLYGON_FACE_NORMAL_ADD		(0xFF000000) //Probably will go unused


typedef struct OCTREE_POLYGON
{
	uint32_t faceNormal;
	int16_t vertexOffset;
	polyProperties properties;
	
} octree_polygon_t;


static inline void octreePolygon_GetVertex(
	const octree_vertex_t* vertex,
	fix16_vec3_t* posAddition, 
	fix16_vec3_t* dest)
{
	*dest = (fix16_vec3_t){
		.x = (vertex->x << 8) + posAddition->x,
		.y = (vertex->y << 8) + posAddition->y,
		.z = (vertex->z << 8) + posAddition->z,
	};
}


static inline void octreePolygon_GetNormal(const octree_polygon_t* poly, fix16_vec3_t* normal)
{
	normal->x = (int16_t)((poly->faceNormal & OCTREE_POLYGON_FACE_NORMAL_X) >> 8);
	normal->y = (int16_t)(poly->faceNormal & OCTREE_POLYGON_FACE_NORMAL_Y);
	normal->z = (int16_t)((poly->faceNormal & OCTREE_POLYGON_FACE_NORMAL_Z) << 8);
}


static inline void octreePolygon_SetNormal(octree_polygon_t* poly, const fix16_vec3_t* normal)
{
	poly->faceNormal = 
		(poly->faceNormal & OCTREE_POLYGON_FACE_NORMAL_ADD) +
		((normal->x << 8) & OCTREE_POLYGON_FACE_NORMAL_X) +
		(normal->y & OCTREE_POLYGON_FACE_NORMAL_Y) +
		((normal->z >> 8) & OCTREE_POLYGON_FACE_NORMAL_Z);		
}

//TODO
//Returns number of vertices
//Don't forget to allocate enough space for them all
static inline uint8_t octreePolygon_GetCollisionPolygon(
	const octree_polygon_t* poly, 
	const octree_vertex_t* vertexArray,
	fix16_vec3_t* positionOffset,
	col_polygon_t* p0, 
	fix16_vec3_t* vertices)
{
	
	p0->properties = poly->properties;
	octreePolygon_GetNormal(poly, &p0->normal);
	
	uint8_t vertexCount = PolyProperties_GetVertexCount(poly->properties);	
	for(uint8_t i = 0; i < vertexCount; i++)
	{
		octreePolygon_GetVertex(&vertexArray[poly->vertexOffset + i], positionOffset, &vertices[i]);
	}
	
	return vertexCount;
}


#endif
