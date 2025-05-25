#include "cdtest.h"


#include <assert.h>
#include <stdlib.h>


static smpc_peripheral_digital_t _digital;

static cdfs_filelist_t _filelist;

static uint16_t _frt_overflow_count = 0;


void CDTestState_Init(void)
{
	/* Load the maximum number. We have to free the allocated filelist
         * entries, but since we never exit, we don't have to */
        cdfs_filelist_entry_t * const filelist_entries =
            cdfs_entries_alloc(-1);
        assert(filelist_entries != NULL);

        cdfs_config_default_set();
        cdfs_filelist_init(&_filelist, filelist_entries, -1);
        cdfs_filelist_root_read(&_filelist);

}


void CDTestState_Free(void)
{
}


void CDTestState_Update(void)
{
	                smpc_peripheral_process();
                smpc_peripheral_digital_port(1, &_digital);

                dbgio_printf("[H[2J");

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
