#include <nds/ndstypes.h>
#include <cstdlib>
#include "romGpio.hpp"
#include "romGpioRtc.h"

// RTC command constants
#define RTC_COMMAND_MAGIC 0x06
#define RTC_COMMAND_MAGIC_MASK 0x0F
#define RTC_COMMAND_CMD_MASK 0x70
#define RTC_COMMAND_CMD_SHIFT 4
#define RTC_COMMAND_READ 0x80

// RTC command types
enum RtcCommand : u16 {
    RTC_CMD_RESET = 0,
    RTC_CMD_DATETIME = 2,
    RTC_CMD_FORCE_IRQ = 3,
    RTC_CMD_CONTROL = 4,
    RTC_CMD_TIME = 6
};

// RTC state
typedef struct RtcState {
    u16 transferState = 0;
    u16 bits = 0;
    s16 bitCount = 0;
    u16 command = 0;
    u16 commandActive = 0;
    s16 bytesLeft = 0;
    u8 dateTime[8]{};
    u16 status = 0;
} RtcState __aligned(2);

RtcState sRtc = {0};

void updateDateTime() {
    // Hardcoded to a modern 2024 date and 10:09:30
    *(u32 *) &sRtc.dateTime[0] = 0x030A0224;
    *(u32 *) &sRtc.dateTime[4] = 0x00300910;
}

void rio_rtcInit() {
    sRtc = {0};
    sRtc.status = 0x40; // Should be 0x82 if not reset once but doesn't matter for emus
}

void processByte() {
    sRtc.bytesLeft--;
    if (!sRtc.commandActive) {
        if ((sRtc.bits & RTC_COMMAND_MAGIC_MASK) == RTC_COMMAND_MAGIC) {
            sRtc.command = sRtc.bits;
            auto cmd = static_cast<RtcCommand>((sRtc.command & RTC_COMMAND_CMD_MASK) >> RTC_COMMAND_CMD_SHIFT);
            switch (cmd) {
                case RTC_CMD_RESET:
                    *(u64*)&sRtc.dateTime = 0; // clear data
                    *(u16*)&sRtc.dateTime[1] = 0x0101; // set month, day
                    sRtc.status = 0;
                    sRtc.bytesLeft = 0;
                    break;
                case RTC_CMD_DATETIME:
                    updateDateTime();
                    sRtc.bytesLeft = 7;
                    break;
                case RTC_CMD_FORCE_IRQ:
                    sRtc.bytesLeft = 0;
                    break;
                case RTC_CMD_CONTROL:
                    sRtc.bytesLeft = 1;
                    break;
                case RTC_CMD_TIME:
                    updateDateTime();
                    sRtc.bytesLeft = 3; // Time data is 3 bytes
                    break;
                default:
                    sRtc.bytesLeft = 0;
                    break;
            }
            sRtc.commandActive = sRtc.bytesLeft > 0;
        }
    } else {
        auto cmd = static_cast<RtcCommand>((sRtc.command & RTC_COMMAND_CMD_MASK) >> RTC_COMMAND_CMD_SHIFT);
        switch (cmd) {
            case RTC_CMD_FORCE_IRQ:
                // TODO: Implement force IRQ
                break;
            case RTC_CMD_CONTROL:
                // Preserve the lower byte and update only the upper byte
                sRtc.status = sRtc.bits;
                break;
            default:
                break;
        }
    }

    sRtc.bits = 0;
    sRtc.bitCount = 0;
    if (!sRtc.bytesLeft) {
        sRtc.commandActive = 0;
        sRtc.command = 0;
    }
}

u32 getOutputBit() {
    if (!sRtc.commandActive) return 0;
    u16 outByte = 0;
    auto cmd = static_cast<RtcCommand>((sRtc.command & RTC_COMMAND_CMD_MASK) >> RTC_COMMAND_CMD_SHIFT);
    switch (cmd) {
        case RTC_CMD_DATETIME:
        case RTC_CMD_TIME:
            outByte = sRtc.dateTime[7 - sRtc.bytesLeft];
            break;
        case RTC_CMD_CONTROL:
            outByte = sRtc.status;
            break;
        default:
            break;
    }
    return (outByte >> sRtc.bitCount) & 1;
}

void rio_rtcUpdate() {
    switch (sRtc.transferState) {
        case 0:
            if ((gRioGpioData & 5) == 1) sRtc.transferState = 1;
            break;
        case 1:
            if ((gRioGpioData & 5) == 5) sRtc.transferState = 2;
            else if ((gRioGpioData & 5) != 1) sRtc.transferState = 0;
            break;
        case 2:
            if (!(gRioGpioData & 1)) {
                sRtc.bits = (sRtc.bits & ~(1 << sRtc.bitCount)) | (((gRioGpioData >> 1) & 1) << sRtc.bitCount);
            } else if (gRioGpioData & 4) {
                if (sRtc.command & RTC_COMMAND_READ) {
                    gRioGpioData =
                            (gRioGpioData & gRioGpioDirection) | ((5 | (getOutputBit() << 1)) & ~gRioGpioDirection);
                    rio_invalidate();
                    if (++sRtc.bitCount == 8) {
                        sRtc.bitCount = 0;
                        if (--sRtc.bytesLeft <= 0) {
                            sRtc.commandActive = 0;
                            sRtc.command = 0;
                        }
                    }
                } else if (++sRtc.bitCount == 8) {
                    processByte();
                }
            } else {
                const u16 control = sRtc.status;
                sRtc = {0};
                sRtc.status = control;
                sRtc.transferState = gRioGpioData & 1;
                gRioGpioData = (gRioGpioData & gRioGpioDirection) | (1 & ~gRioGpioDirection);
                rio_invalidate();
            }
            break;
    }
}