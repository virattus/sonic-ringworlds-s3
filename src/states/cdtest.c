#include "cdtest.h"


#include <assert.h>
#include <stdlib.h>


static smpc_peripheral_digital_t _digital;

static uint16_t _frt_overflow_count = 0;


void CDTestState_Init(void)
{
	CD_OpenFileList();
	
	CD_LoadFile("TEST.TXT", LWRAM(0));
}


void CDTestState_Free(void)
{
	CD_CloseFileList();
}


void CDTestState_Update(void)
{
	
	smpc_peripheral_process();
	smpc_peripheral_digital_port(1, &_digital);

	dbgio_printf("CD Test State\n");
	dbgio_printf("Total number of entries: %u\n", CD_GetFileEntryCount());
	
	for(int i = 0; i < CD_GetFileEntryCount(); i++)
	{
		const cdfs_filelist_entry_t* entry = CD_GetFileEntry(i);
		dbgio_printf("%s: FAD: %li Sectors: %i\n", entry->name, entry->starting_fad, entry->sector_count);
	}
	
	dbgio_printf((const char*)(LWRAM(0)));
	
}


gamestate_t Get_CDTestState(void)
{
	return (gamestate_t)
	{
		.InitState = CDTestState_Init,
		.FreeState = CDTestState_Free,
		.UpdateState = CDTestState_Update,
	};
}
