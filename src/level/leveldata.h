#ifndef __RINGWORLDS_LEVEL_DATA_H__
#define __RINGWORLDS_LEVEL_DATA_H__


#include <stdint.h>


//TODO find out the correct sizes for these
typedef struct
{
	uint32_t cd_sector_start;
	uint32_t cd_sector_count;
	
} leveldata_entry_t;


void LevelData_Init();


const leveldata_entry_t* LevelData_GetEntry(uint16_t Index);

uint16_t LevelData_GetLevelCount();

#endif //__RINGWORLDS_LEVEL_DATA_H__
