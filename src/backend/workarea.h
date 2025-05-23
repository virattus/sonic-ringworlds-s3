#ifndef __RINGWORLDS_WORKAREA_H__
#define __RINGWORLDS_WORKAREA_H__

#include <yaul.h>
#include <mic3d.h>



//TODO: Unimplemented, returns zero
uint32_t GetWorkareaSize(uint16_t cmdt_count);

uint32_t GetSortListSize(uint16_t sort_count);

uint32_t GetShadingTableSize(uint16_t shading_count);


//TODO create a work area at the specified address
workarea_mic3d_t* CreateWorkarea(void* address);

//TODO same as above
sort_list_t* CreateSortList(void* address, uint16_t count);

//TODO same as above
vdp1_gouraud_table_t* CreateShadingTable(void* address, uint16_t count);


#endif
