#include <nds/ndstypes.h>
#include "romGpio.hpp"
#include "GbaGpio.h"

u16 gRioGpioData = 0;
u16 gRioGpioDirection = 0;
u16 gRioGpioControl = 0;

SharedGpioRegs gGpioRegs = {nullptr};

void rio_invalidate() {
    if (gRioGpioControl) {
        *gGpioRegs.gRioGpioData = gRioGpioData;
        *gGpioRegs.gRioGpioDirection = gRioGpioDirection;
        *gGpioRegs.gRioGpioControl = gRioGpioControl;
    } else {
        *gGpioRegs.gRioGpioData = 0;
        *gGpioRegs.gRioGpioDirection = 0;
        *gGpioRegs.gRioGpioControl = 0;
    }
}