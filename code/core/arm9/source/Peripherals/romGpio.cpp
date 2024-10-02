#include <nds/ndstypes.h>
#include <cstdlib>
#include "romGpioRtc.h"
#include "romGpio.hpp"
#include "GbaGpio.h"

struct game_hw_info_t {
    u32 gameCode;
    RomGpioHwMask hardware;
};

#define GAMECODE(x)        ((((x) & 0xFF) << 24) | ((((x) >> 8) & 0xFF) << 16) | ((((x) >> 16) & 0xFF) << 8) | ((x) >> 24))

static game_hw_info_t sGameHardwareTable[] =
        {
                // Boktai: The Sun is in Your Hand
                {GAMECODE('U3IJ'), RIO_RTC | RIO_LIGHT},
                {GAMECODE('U3IE'), RIO_RTC | RIO_LIGHT},
                {GAMECODE('U3IP'), RIO_RTC | RIO_LIGHT},

                // Boktai 2: Solar Boy Django
                {GAMECODE('U32J'), RIO_RTC | RIO_LIGHT},
                {GAMECODE('U32E'), RIO_RTC | RIO_LIGHT},
                {GAMECODE('U32P'), RIO_RTC | RIO_LIGHT},

                // Pokemon Ruby
                {GAMECODE('AXVJ'), RIO_RTC},
                {GAMECODE('AXVE'), RIO_RTC},
                {GAMECODE('AXVP'), RIO_RTC},
                {GAMECODE('AXVI'), RIO_RTC},
                {GAMECODE('AXVS'), RIO_RTC},
                {GAMECODE('AXVD'), RIO_RTC},
                {GAMECODE('AXVF'), RIO_RTC},

                // Pokemon Sapphire
                {GAMECODE('AXPJ'), RIO_RTC},
                {GAMECODE('AXPE'), RIO_RTC},
                {GAMECODE('AXPP'), RIO_RTC},
                {GAMECODE('AXPI'), RIO_RTC},
                {GAMECODE('AXPS'), RIO_RTC},
                {GAMECODE('AXPD'), RIO_RTC},
                {GAMECODE('AXPF'), RIO_RTC},

                // Pokemon Emerald
                {GAMECODE('BPEJ'), RIO_RTC},
                {GAMECODE('BPEE'), RIO_RTC},
                {GAMECODE('BPEP'), RIO_RTC},
                {GAMECODE('BPEI'), RIO_RTC},
                {GAMECODE('BPES'), RIO_RTC},
                {GAMECODE('BPED'), RIO_RTC},
                {GAMECODE('BPEF'), RIO_RTC},

                // RockMan EXE 4.5 - Real Operation
                {GAMECODE('BR4J'), RIO_RTC},

                // Sennen Kazoku
                {GAMECODE('BKAJ'), RIO_RTC},

                // Shin Bokura no Taiyou: Gyakushuu no Sabata
                {GAMECODE('U33J'), RIO_RTC | RIO_LIGHT},
        };

u16 gRioGpioData = 0;
u16 gRioGpioDirection = 0;
u16 gRioGpioControl = 0;

SharedGpioRegs gGpioRegs;

void rio_init() {
    rio_rtcInit();
}

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
