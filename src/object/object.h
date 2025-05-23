#ifndef __RINGWORLDS_OBJECT_H__
#define __RINGWORLDS_OBJECT_H__


#include "objectEntry.h"

#include <globals.h>

#include <stdint.h>
#include <stdbool.h>


#define PERSISTENCE_BIT_OFFSET_MASK		(15)
#define PERSISTENCE_BYTE_OFFSET_MASK	(1008)


typedef void (*objectFunc)(game_object_entry_t*);


typedef struct
{
	objectFunc Init;
	objectFunc Free;
	objectFunc Update;
	
} game_object_t;


//TODO make sure these functions actually work
static inline bool ReadPersistentBit(uint16_t persistentOffset)
{
	uint16_t PersistByte = (persistentOffset & PERSISTENCE_BYTE_OFFSET_MASK) >> 4;
	uint16_t PersistentShort = *(uint16_t*)(PERSISTENCE_VALUE_LOC + (PersistByte));
	
	return (PersistentShort & (1 << (persistentOffset & PERSISTENCE_BIT_OFFSET_MASK)));
}


static inline void WritePersistentBit(uint16_t persistentOffset)
{
	uint16_t PersistByte = (persistentOffset & PERSISTENCE_BYTE_OFFSET_MASK) >> 4;
	uint16_t PersistBit = (1 << (persistentOffset & PERSISTENCE_BIT_OFFSET_MASK));
	
	*(uint16_t*)(PERSISTENCE_VALUE_LOC + (PersistByte)) |= PersistBit;
}


#endif
