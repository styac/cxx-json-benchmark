#pragma once

#if USE_MEMORYSTAT

#define malloc MemoryStatMalloc
#define calloc MemoryStatCalloc
#define realloc MemoryStatRealloc
#define free MemoryStatFree

#endif
