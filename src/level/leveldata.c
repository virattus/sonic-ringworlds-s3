#include "leveldata.h"

#include "globals.h"





void LoadLevelDataFromCD(uint16_t index, const char* filename)
{
	
}


void LevelData_Init()
{
	GameFlagsMax[GAME_FLAG_CURRENT_LEVEL] = LevelData_GetLevelCount();
}



const leveldata_entry_t* LevelData_GetEntry(uint16_t Index)
{
	
}


uint16_t LevelData_GetLevelCount()
{
	return 0;
}
