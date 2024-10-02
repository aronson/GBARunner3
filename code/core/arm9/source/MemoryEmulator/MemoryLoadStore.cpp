#include "MemoryLoadStore.h"
#include "Peripherals/romGpio.hpp"
#include "Peripherals/romGpioRtc.h"

void rio_write(u32 offset, u16 val) {
    switch (offset) {
        case 0:  // RIO_REG_DATA
            gRioGpioData = (gRioGpioData & ~gRioGpioDirection) | (val & gRioGpioDirection);
            rio_rtcUpdate();
            break;
        case 2:  // RIO_REG_DIRECTION
            gRioGpioDirection = val & 0xF;
            break;
        case 4:  // RIO_REG_CONTROL
            gRioGpioControl = val & 1;
            break;
    }
    rio_invalidate();
}