#pragma once

typedef u16 RomGpioHwMask;

#define RIO_NONE    0
#define RIO_RTC     (1 << 0)
#define RIO_LIGHT   (1 << 1)
#define ROM_LINEAR_DS_ADDRESS           0x02200000

extern u16 gRioGpioData;
extern u16 gRioGpioDirection;
extern u16 gRioGpioControl;

#ifdef __cplusplus
extern "C" {
#endif
void rio_init();
void rio_invalidate();
#ifdef __cplusplus
}
#endif