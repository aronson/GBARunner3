// shared_mem.h
#pragma once

#include <nds/ndstypes.h>

typedef struct SharedGpioRegs {
    u16* gRioGpioData;
    u16* gRioGpioDirection;
    u16* gRioGpioControl;
} SharedGpioRegs;

extern SharedGpioRegs gGpioRegs;
