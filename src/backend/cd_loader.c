#include "cd_loader.h"


#include <assert.h>


static cdfs_filelist_t _filelist;


void CD_InitFileList()
{
	/* Load the maximum number. We have to free the allocated filelist
	 * entries, but since we never exit, we don't have to */
	cdfs_filelist_entry_t * const filelist_entries =
		cdfs_entries_alloc(-1);
	assert(filelist_entries != NULL);

	cdfs_filelist_init(&_filelist, filelist_entries, -1);
	cdfs_filelist_root_read(&_filelist);
}


void CD_LoadFile(const char* filename, void* dest)
{
	uint32_t file_index;
	cdfs_filelist_entry_t *file_entry;

	for (file_index = 0; file_index < _filelist.entries_count; file_index++) 
	{
		file_entry = &_filelist.entries[file_index];

		if (file_entry->type == CDFS_ENTRY_TYPE_FILE) 
		{
			if ((strcmp(file_entry->name, filename)) == 0) 
			{
				int ret __unused;
				ret = cd_block_sectors_read(file_entry->starting_fad, dest, file_entry->size);
				assert(ret == 0);
				return;
			}
		}
	}
}
