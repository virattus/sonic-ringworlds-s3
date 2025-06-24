#ifndef __SSEE_CD_LOADER_H__
#define __SSEE_CD_LOADER_H__


#include <yaul.h>

#include <stdint.h>


void CD_OpenFileList(void);
void CD_CloseFileList(void);

const char** CD_GetFileList(void);

uint16_t CD_GetFileEntryCount(void);

//temporary
const cdfs_filelist_entry_t* CD_GetFileEntry(uint16_t index);

void CD_LoadFileByIndex(uint16_t index, void* dest);

void CD_LoadFile(const char* filename, void* dest);


#endif
