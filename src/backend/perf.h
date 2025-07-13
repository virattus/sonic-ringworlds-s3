#ifndef __SSEE_PERFORMANCE_MONITORING_H__
#define __SSEE_PERFORMANCE_MONITORING_H__


//Taken from Yaul

#include <stdint.h>

typedef struct perf_counter {
        uint32_t start_tick;
        uint32_t end_tick;
        uint32_t ticks;
        uint32_t max_ticks;
} perf_counter_t;

void perf_init(void);

void perf_counter_init(perf_counter_t *perf_counter);
void perf_counter_start(perf_counter_t *perf_counter);
void perf_counter_end(perf_counter_t *perf_counter);




#endif //__SSEE_PERFORMANCE_MONITORING_H__
