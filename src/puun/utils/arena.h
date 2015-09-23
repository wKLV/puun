#include <puun/types.h>
#ifndef puunArenaH

#define puun_ARENA(MANYSLOTS)\
typedef struct puun_Arena {\
    Data memory;\
    u32 capacity;\
    u32 slotSize;\
    u8 slots[MANYSLOTS]\
} puun_Arena##MANYSLOTS;\
\
DataArray puun_allocate(puun_Arena##MANYSLOTS* arena, u32 size) {\
    DataArray result = ZERO_STRUCT;\
    u32 manySlots = (size % arena->slotSize) + 1;\
    assert(manySlots<MANYSLOTS);/*TODO: Should we fail silently? or should we assert?*/\
    i32 i;\
    i32 j;\
    for(i=0; i<MANYSLOTS; ++i) { \
        for(j=0; j<manySlots; ++j) {\
            if(arena->slots[i] != 0) break;\
            result.value = arena->memory + i*arena->slotSize;\
            result.num = j*arena->slotSize;\
        }\
    }\
    return result;\
}; \

#ifndef puunARENA_DEFAULTS 1
#define puunARENA_DEFAULTS
#endif
#if !puunARENA_DEFAULTS
puun_ARENA(8);
puun_ARENA(32);
puun_ARENA(128);
puun_ARENA(512);
#endif

#endif
