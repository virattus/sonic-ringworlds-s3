#include "collisionWorld.h"

#include <assert.h>


#define OCTREES_MAX			(8)


static octree_t* octrees[OCTREES_MAX];
static uint8_t octree_count = 0;


bool colworld_BindOctree(octree_t* octreeRoot)
{
	if(octree_count + 1 >= OCTREES_MAX)
	{
		return false;
	}
	
	octrees[octree_count] = octreeRoot;
	octree_count++;
	
	return true;
}

/*
 * Iterate through the list, and once the entry is found, 
 * shift the remaining entries up. All 
 * Currently, don't bother with bounds checks
 * for the last entry, since it should never be accessed anyways
 */
void colworld_RemoveOctree(octree_t* octreeRoot)
{
	bool foundEntry = false;
	const uint8_t original_count = octree_count; 	/*
													* Use a dedicated loop variable
													* We decrement the count if it's found during the loop
													* and I don't trust the compiler to not do something "cute"
													*/
	for(uint8_t i = 0; i < original_count; i++)
	{
		if(!foundEntry)
		{
			if(octrees[i] == octreeRoot)
			{
				foundEntry = true;
				octree_count--;
				octrees[i] = octrees[i + 1];
			}
		}
		else
		{
			octrees[i] = octrees[i + 1];
		}
	}
}


bool colworld_CheckCollisionSphere(
	const col_sphere_t* s0, 
	const polygon_mask_types_t sphereMask,
	col_resp_t* resp_array, 
	uint8_t maxCollisions)
{
	
	return false; //TODO this is not how you do it, idiot, search, build a list, and then iterate looking for maxCollisions number of collisions
	
	uint8_t totalCollisions = 0;
	for(int i = 0; i < OCTREES_MAX; i++)
	{
		//totalCollisions += Octree_SphereCollision(octrees[i], s0, sphereMask, resp_array, maxCollisions);
	}
	
	return totalCollisions > 0;
}


bool colworld_CheckCollisionLine(
	const col_line_t* l0, 
	const polygon_mask_types_t lineMask,
	col_resp_t* resp_array, 
	uint8_t maxCollisions)
{
	/*
	char dbgStr[64];
	fix16_vec3_str(&polygons[0]->offset[0], dbgStr, 3);
	dbgio_printf("checkcollisionline\n%s\n", dbgStr);
	*/
	
	uint8_t totalCollisions = 0;
	for(int i = 0; i < OCTREES_MAX; i++)
	{
		//totalCollisions += Octree_LineCollision(octrees[i], l0, lineMask, resp_array, maxCollisions);
	}

	return totalCollisions > 0;
}
