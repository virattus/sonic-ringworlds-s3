#include "ring_bounce.h"

#include <src/object/ring/ring.h>
#include <src/backend/rng.h>
#include <src/collision/collisionWorld.h>

#include <src/globals.h>

#include <assert.h>


#define RING_RADIUS_FLOAT					(0.5f)
#define RING_RADIUS							(FIX16(RING_RADIUS_FLOAT))
#define RING_RADIUS_SQU						(FIX16(RING_RADIUS_FLOAT * RING_RADIUS_FLOAT))

#define RING_LIFESPAN_EMPTY					(1)
#define RING_LIFESPAN_NULL					(0)

#define RING_DESPAWN_MAX					(4)
#define RING_BOUNCE_MAX_COUNT				(32)
#define RING_BOUNCE_LIFETIME_FLICKER_MIN	(32)

#define RING_BOUNCE_MAX_SPEED_FLOAT			(2.0f / (float)FRAME_RATE)
#define RING_BOUNCE_MAX_SPEED				(FIX16(RING_BOUNCE_MAX_SPEED_FLOAT))
#define RING_BOUNCE_MAX_SPEED_SQU			(FIX16(RING_BOUNCE_MAX_SPEED_FLOAT * RING_BOUNCE_MAX_SPEED_FLOAT))


static fix16_vec3_t ringBouncePositions[RING_BOUNCE_MAX_COUNT];
static fix16_vec3_t ringBounceVelocities[RING_BOUNCE_MAX_COUNT];
static uint8_t ringBounceLifetimes[RING_BOUNCE_MAX_COUNT];

static xform_t transforms[RING_BOUNCE_MAX_COUNT]; //temporary

static uint8_t ringBounceTotal = 0;
static uint8_t ringBounceAdd = 0;
static fix16_vec3_t ringBounceAddSource;

static col_sphere_t ringBounceSphere;



static void RingBounce_ClampVelocity(uint8_t index)
{
	assert(index < RING_BOUNCE_MAX_COUNT);
	
	if(fix16_vec3_length(&ringBounceVelocities[index]) > RING_BOUNCE_MAX_SPEED_SQU)
	{
		fix16_vec3_normalize(&ringBounceVelocities[index]);
		fix16_vec3_scale(RING_BOUNCE_MAX_SPEED_SQU, &ringBounceVelocities[index]);
	}
}


static void RingBounce_GenerateRing(uint8_t index)
{
	assert(index < RING_BOUNCE_MAX_COUNT);
		
	fix16_vec3_dup(&ringBounceAddSource, &ringBouncePositions[index]);
	
	ringBounceVelocities[index].x = (RNG_Generate() << 16);
	ringBounceVelocities[index].y = 0;
	ringBounceVelocities[index].z = (RNG_Generate() << 16);
	
	RingBounce_ClampVelocity(index);
	
	//fix16_vec3_zero(&ringBounceVelocities[index]);
	
	ringBounceVelocities[index].y = (RNG_Generate() << 1); //Should always point up, so we don't care about setting MSB
	
	ringBounceLifetimes[index] = RNG_Generate() | 0x80; //Lifetime range of 127-255
	
	ringBounceTotal++;
}


void RingBounce_Initialise(void)
{
	ringBounceSphere.radius = RING_RADIUS;
	ringBounceSphere.radiusSquared = RING_RADIUS_SQU;
	
	for(uint8_t i = 0; i < RING_BOUNCE_MAX_COUNT; i++)
	{
		ringBounceLifetimes[i] = RING_LIFESPAN_NULL;
	}
	
}


void RingBounce_SetSpawnSource(const fix16_vec3_t* sourcePos)
{
	fix16_vec3_dup(sourcePos, &ringBounceAddSource);
	//ringBounceAddSource = sourcePos;
}


uint8_t RingBounce_AddRings(uint8_t quantity)
{	
	assert(quantity < RING_BOUNCE_MAX_COUNT);
	
	
	if((ringBounceTotal + ringBounceAdd + quantity) > RING_BOUNCE_MAX_COUNT)
	{
		uint8_t newQuantity = quantity - (RING_BOUNCE_MAX_COUNT - (ringBounceTotal + ringBounceAdd));
		ringBounceAdd += newQuantity;
		return newQuantity;
	}
	else
	{
		ringBounceAdd += quantity;
		return 0;
	}
}


uint8_t RingBounce_GetTotal()
{
	return ringBounceTotal;
}


void RingBounce_GetPosition(uint8_t index, fix16_vec3_t* dest)
{
	assert(index < RING_BOUNCE_MAX_COUNT);
	assert(dest);
	
	//char output[16];
	//fix16_str(transforms[index].depth_value, output, 7);
	//dbgio_printf("true depth value: %s\n", output);
	
	fix16_vec3_dup(&ringBouncePositions[index], dest);
}


void RingBounce_GetVelocity(uint8_t index, fix16_vec3_t* dest)
{
	assert(index < RING_BOUNCE_MAX_COUNT);
	assert(dest);
	
	fix16_vec3_dup(&ringBounceVelocities[index], dest);
}


uint8_t RingBounce_GetLifetime(uint8_t index)
{
	assert(index < RING_BOUNCE_MAX_COUNT);
	
	return ringBounceLifetimes[index];	
	
}


void RingBounce_Update(void)
{
	uint8_t totalFreedRings = 0;

	for(uint8_t i = 0; i < RING_BOUNCE_MAX_COUNT; i++)
	{
		if(ringBounceLifetimes[i])
		{
			if(!RingBounce_UpdateLifetime(i, totalFreedRings))
			{
				totalFreedRings++;
			}
			else
			{
				RingBounce_UpdateCollision(i);
			}
		}	
	}

	if(ringBounceAdd > 0)
	{
		for(uint8_t i = 0; i < RING_BOUNCE_MAX_COUNT; i++)
		{
			if(!ringBounceLifetimes[i])
			{
				RingBounce_GenerateRing(i);
				ringBounceAdd--;
				break; //Only add one ring per frame
			}
		}
	}
}


bool RingBounce_UpdateLifetime(uint8_t index, uint8_t totalFreedRings)
{
	if(ringBounceLifetimes[index] == RING_LIFESPAN_EMPTY)
	{
		if(totalFreedRings >= RING_DESPAWN_MAX)
		{
			ringBounceLifetimes[index] += 2;
			return true;
		}
		else
		{
			ringBounceLifetimes[index] = RING_LIFESPAN_NULL;
			ringBounceTotal--;
			return false;
		}
	}
	else
	{
		ringBounceLifetimes[index]--;
		return true;
	}
}


void RingBounce_UpdateCollision(uint8_t index)
{	
	fix16_vec3_t addResult;
	
	fix16_vec3_add(&ringBouncePositions[index], &ringBounceVelocities[index], &addResult);
	fix16_vec3_dup(&addResult, &ringBouncePositions[index]);
	
	ringBounceVelocities[index].y -= GRAVITY;	
	
	fix16_vec3_dup(&ringBouncePositions[index], &ringBounceSphere.position);
	
	return; //checking collision is still TODO
	
	col_resp_t col;
	
	if(colworld_CheckCollisionSphere(&ringBounceSphere, TYPE_RING, &col, 1))
	{	
		dbgio_printf("ring collision\n");
		
		fix16_vec3_t resolution;
		fix16_vec3_dup(&col.normal, &resolution);
		fix16_vec3_scale(col.penetration, &resolution);
		fix16_vec3_add(&resolution, &ringBounceSphere.position, &ringBouncePositions[index]);
		
		
		//fix16_vec3 Reflect is planned for C but currently only c++, so we gotta do it
		const fix16_t factor = fix16_vec3_dot(&ringBounceVelocities[index], &col.normal);
		fix16_vec3_t proj;
		fix16_vec3_dup(&col.normal, &proj);
		fix16_vec3_scale(factor, &proj);
		
		fix16_vec3_t vel;
		fix16_vec3_dup(&ringBounceVelocities[index], &vel);
		fix16_vec3_sub(&vel, &proj, &ringBounceVelocities[index]);
	
		
		//ringBounceVelocities[index].y = -ringBounceVelocities[index].y;
		//ringBounceVelocities[index].y = FIX16(1.0f);
	}
	
}


void RingBounce_DrawRings()
{
	if(ringBounceTotal == 0)
	{
		return;
	}
	
	fix16_mat43_t world = {0};
	fix16_mat33_identity(&world.rotation);
	
	//xform_t transforms[RING_BOUNCE_MAX_COUNT];
	
	fix16_vec3_t activePositions[RING_BOUNCE_MAX_COUNT];
	uint8_t activeCount = 0;
	for(uint8_t i = 0; i < RING_BOUNCE_MAX_COUNT; i++)
	{
		if(ringBounceLifetimes[i])
		{
			fix16_vec3_dup(&ringBouncePositions[i], &activePositions[activeCount++]);
		}
	}
	
	//This should never fire
	if(activeCount == 0)
	{
		assert(false);
		return;
	}
	
	render_points_xform(&world, activePositions, transforms, activeCount);
	
	vdp1_cmdt_t cmdt = {0};	
	Ring_AssignSpriteData(&cmdt);
	
	
	for(uint8_t i = 0; i < activeCount; i++)
	{
		
		if(transforms[i].depth_value > 0)
		{
			continue;
		}

		/*
		if(transforms[i].screen_point.x < -200 || 
			transforms[i].screen_point.x > 200)
		{
			continue;
		}
		
		if(transforms[i].screen_point.y < -150 ||
			transforms[i].screen_point.y > 150)
		{
			continue;
		}
		*/
		
		if((ringBounceLifetimes[i] > RING_BOUNCE_LIFETIME_FLICKER_MIN)
			)//|| (ringBounceLifetimes[i] & 0x01))  //If flickering, only show on odd frames
		{
			int16_vec2_dup(&transforms[i].screen_point, &cmdt.cmd_vertices[0]);
			//cmdt.cmd_vertices[0].x = transforms[i].screen_point.x;
			//cmdt.cmd_vertices[0].y = transforms[i].screen_point.y;
			
			Ring_ScaleToDepth(&cmdt, transforms[i].depth_value);
			
			render_cmdt_insert(&cmdt, transforms[i].depth_value);
		}
	}
	
}
