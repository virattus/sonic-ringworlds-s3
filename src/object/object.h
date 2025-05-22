#ifndef __RINGWORLDS_OBJECT_H__
#define __RINGWORLDS_OBJECT_H__


#include "objectEntry.h"

#include <globals.h>

#include <stdint.h>
#include <stdbool.h>


typedef void (*objectFunc)(game_object_entry_t*);


typedef struct
{
	objectFunc Init;
	objectFunc Free;
	objectFunc Update;
	
} game_object_t;


#endif
