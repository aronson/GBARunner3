#include <nds.h>
#include "gbarunner9_bin.h"

typedef void (*gbarunner9_func_t)(void);

int main(int argc, char* argv[])
{
    *(vu32*)0x04000208 = 0; // IME = 0
    *(vu32*)0x04000210 = 0; // IE = 0
    *(vu32*)0x04000214 = ~0u; // ack IF

    *(vu8*)0x04000240 = 0x80; // VRAM A
    *(vu8*)0x04000241 = 0x80; // VRAM B

    DC_FlushAll();
    DC_InvalidateAll();
    IC_InvalidateAll();
    dmaCopyHalfWords(3, gbarunner9_bin, (void*)0x06800000, gbarunner9_bin_size);

    ((gbarunner9_func_t)0x06800000)();

    return 0;
}