#ifndef __SSEE_COLLISION_POLYGON_PROPERTIES_H__
#define __SSEE_COLLISION_POLYGON_PROPERTIES_H__


#include <stdint.h>


#define POLYGON_VERTEX_COUNT_MIN		(3)
#define POLYGON_VERTEX_COUNT_MASK		(0x0003)
#define POLYGON_TYPE_MASK				(0x000C)
#define POLYGON_COLL_MASK_TYPE_MASK		(0x00F0)
#define POLYGON_PROPERTIES_MASK			(0xFFFC)




typedef uint32_t polyProperties;
/*
	 * Properties holds a lot here:
	 * Bits 0-1: Length (gets added to minimum of 3, so range of 3-6 verts)
	 * Bits 2-3: Polygon Type (defines if the polygon is collidable or not
	 * Bits 4-6: Polygon Collision Mask (Used for collision filtering)
	 * Bit 6: 
	 */



/*
 * Defines if the polygon is visual (for rendering), collision, or both
 * PUSH is a special type that pushes the player towards one edge, used
 * for aligning the player in the centre of things like loops, etc.
 */
typedef enum POLYGON_TYPES
{
	VISUAL			= (1 << 2),
	COLLISION_ONLY,
	VISUAL_AND_COL,
	PUSH,
	
} polygon_types_t;


/*
 * Defines what object types the plane can collide with
 * Note that these are flags
 */
typedef enum POLYGON_COLLISION_MASK_TYPES
{
	TYPE_PLAYER		= (1 << 4),
	TYPE_ENEMY		= (1 << 5),
	TYPE_CAMERA		= (1 << 6),
	TYPE_RING		= (1 << 7),
	
} polygon_mask_types_t;




//TODO signal when count is greater than the max number of verts
static inline void PolyProperties_SetVertexCount(polyProperties* p, uint8_t count)
{
	assert(count >= POLYGON_VERTEX_COUNT_MIN);
	
	*p = 
		(*p & (~POLYGON_VERTEX_COUNT_MASK)) + 
		(count - POLYGON_VERTEX_COUNT_MIN) & POLYGON_VERTEX_COUNT_MASK;
}


//Reminder, we only store the number of verts past 3
static inline uint8_t PolyProperties_GetVertexCount(polyProperties p)
{
	return (p & POLYGON_VERTEX_COUNT_MASK) + POLYGON_VERTEX_COUNT_MIN;
}


static inline void PolyProperties_SetPolygonType(polyProperties* p, const polygon_types_t type)
{
	*p = (*p & (~POLYGON_TYPE_MASK)) + (type);
}


static inline uint16_t PolyProperties_GetPolygonType(polyProperties p)
{
	return p & POLYGON_TYPE_MASK;
}


static inline void PolyProperties_SetCollisionMask(polyProperties* p, const polygon_mask_types_t mask)
{
	*p = (*p & (~POLYGON_COLL_MASK_TYPE_MASK)) + (mask);
}


static inline uint16_t PolyProperties_GetCollisionMask(polyProperties p)
{
	return p & POLYGON_COLL_MASK_TYPE_MASK;
}


//Mask out vertex count bits to save headaches in the future
static inline void PolyProperties_SetPropertiesOnly(polyProperties* p, polyProperties _newProp)
{
	*p |= (_newProp & POLYGON_PROPERTIES_MASK);
}



#endif
