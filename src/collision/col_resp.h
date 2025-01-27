#ifndef __SSEE_COLLISION_RESPONCE_H__
#define __SSEE_COLLISION_RESPONCE_H__


#include <yaul.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct
{
	fix16_vec3_t position;
	fix16_vec3_t normal;
	fix16_t penetration;
	
} col_resp_t;


#endif
