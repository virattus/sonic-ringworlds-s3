#include "light.h"

#include <assert.h>


void Lighting_BuildLantern(
	vdp1_gouraud_table_t* table,
	const fix16_vec3_t* dirLight,
	//const fix8_vec3_t* normalData, 
	const uint16_t* paletteData
)
{
	assert(0);
	
	//Decompose the vertex colours into u8s, 
	//use dirlight to figure out brightness (x axis) and palette slot (y axis)
	//average everything together?
	
	//Turns out that you can do the colour math without decomposing into multiple ints: 
	//https://stackoverflow.com/questions/18937701/combining-two-16-bits-rgb-colors-with-alpha-blending
}
