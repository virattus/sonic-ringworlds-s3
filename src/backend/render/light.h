#ifndef __SSEE_RENDER_LIGHTING_H__
#define __SSEE_RENDER_LIGHTING_H__


#include <yaul.h>
#include <stdint.h>


void Lighting_BuildLantern(
	vdp1_gouraud_table_t* table,
	const fix16_vec3_t* dirLight,
	//const fix8_vec3_t* normalData, //figure out how we're gonna store this in the model
	const uint16_t* paletteData
);


#endif //__SSEE_RENDER_LIGHTING_H__
