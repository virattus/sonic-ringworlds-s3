#ifndef __SSEE_DEBUG_SYSTEM_H__
#define __SSEE_DEBUG_SYSTEM_H__


#include <yaul.h>
#include "backend/perf.h"


#define PERF_CPU_MASTER		(0)
#define PERF_CPU_SLAVE		(1)
#define PERF_COUNT			(2)


void DebugSystem_SetPerfLevel(uint16_t perfID, uint32_t ticks, uint32_t maxTicks);


void DebugSystem_Update(smpc_peripheral_digital_t* digital);


#endif //__SSEE_DEBUG_SYSTEM_H__
