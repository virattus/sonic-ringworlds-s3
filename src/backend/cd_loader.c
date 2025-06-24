#include "cd_loader.h"


#include <stdbool.h>
#include <assert.h>


static cdfs_filelist_t _filelist;
static cdfs_filelist_entry_t* file_entries = NULL;
static bool filelist_Initialised = false;


void CD_OpenFileList()
{
	assert(!filelist_Initialised);
	
	cdfs_filelist_entry_t * const filelist_entries = cdfs_entries_alloc(-1);
	assert(filelist_entries != NULL);
	
	file_entries = filelist_entries;

	cdfs_config_default_set();
	cdfs_filelist_init(&_filelist, filelist_entries, -1);
	cdfs_filelist_root_read(&_filelist);
		
	filelist_Initialised = true;
}


void CD_CloseFileList(void)
{
	cdfs_entries_free(file_entries);

	filelist_Initialised = false;
}


const char** CD_GetFileList(void)
{
	return NULL;
}


uint16_t CD_GetFileEntryCount(void)
{
	assert(filelist_Initialised);
	
	return _filelist.entries_count;
}


const cdfs_filelist_entry_t* CD_GetFileEntry(uint16_t index)
{
	assert(filelist_Initialised);
	
	return &_filelist.entries[index];
}


void CD_LoadFileByIndex(uint16_t index, void* dest)
{
	assert(filelist_Initialised);
	
	int32_t ret __unused;
	ret = cd_block_sectors_read(_filelist.entries[index].starting_fad, dest, _filelist.entries[index].size);
	assert(ret == 0);
	return;
}


void CD_LoadFile(const char* filename, void* dest)
{
	assert(filelist_Initialised);
	
	uint32_t file_index;
	cdfs_filelist_entry_t *file_entry;

	for (file_index = 0; file_index < _filelist.entries_count; file_index++) 
	{
		file_entry = &_filelist.entries[file_index];

		if (file_entry->type == CDFS_ENTRY_TYPE_FILE) 
		{
			if ((strcmp(file_entry->name, filename)) == 0) 
			{
				CD_LoadFileByIndex(file_index, dest);
				return;
			}
		}
	}
}
