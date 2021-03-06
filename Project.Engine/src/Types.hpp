#pragma once

typedef unsigned int EntityID;
typedef unsigned int EventType;
typedef unsigned char SystemMask;
typedef unsigned int CallbackType;


#define EVENT_ENTITY_DELETED 0x0000
#define SYSTEM_MASK_UNMARKED 0
#define SYSTEM_MASK_TEMP 1
#define SYSTEM_MASK_PERM 2

#define _CALLBACK(a) reinterpret_cast<void *>(a)

EntityID GetCameraID();