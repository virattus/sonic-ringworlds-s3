#ifndef __SSEE_DEBUG_CMDT_H__
#define __SSEE_DEBUG_CMDT_H__

#include <yaul.h>

#include <stdint.h>


void DebugCmdt_Register(vdp1_cmdt_list_t* list);

void DebugCmdt_Update(smpc_peripheral_digital_t* digital);


#endif //__SSEE_DEBUG_CMDT_H__
