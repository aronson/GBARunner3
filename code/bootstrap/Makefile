# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2023-2024

export BLOCKSDS			?= /opt/blocksds/core
export BLOCKSDSEXT		?= /opt/blocksds/external

export WONDERFUL_TOOLCHAIN	?= /opt/wonderful
ARM_NONE_EABI_PATH	?= $(WONDERFUL_TOOLCHAIN)/toolchain/gcc-arm-none-eabi/bin/

# User config
# ===========

NAME 		:= bootstrap
TARGET		:= bootstrap
GAME_TITLE	:= GBARunner3
GAME_SUBTITLE   := By Gericom
GAME_AUTHOR     := github.com/Gericom/GBARunner3
GAME_ICON       := 

# DLDI and internal SD slot of DSi
# --------------------------------

# Root folder of the SD image
SDROOT          := sdroot
# Name of the generated image it "DSi-1.sd" for no$gba in DSi mode
SDIMAGE         := image.bin

# specify a directory which contains the nitro filesystem
# this is relative to the Makefile
ifdef GBAR3_ISNITRO
NITRO_FILES	:= nitrofiles
else
NITRO_FILES	:= 
endif

.PHONY: checkcorearm9 checkarm7 checkarm9 clean

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all: checkcorearm9 checkarm7 checkarm9 $(TARGET).nds

#---------------------------------------------------------------------------------
checkcorearm9:
	$(MAKE) -C ../core/arm9
	cp ../core/arm9/arm9.bin arm9/data/gbarunner9.bin

#---------------------------------------------------------------------------------
checkarm7:
	$(MAKE) -C ../core/arm7
	
#---------------------------------------------------------------------------------
checkarm9:
	$(MAKE) -C arm9

#---------------------------------------------------------------------------------
$(TARGET).nds	: $(NITRO_FILES) arm9/$(TARGET).elf
	$(BLOCKSDS)/tools/ndstool/ndstool -c $(TARGET).nds -7 ../core/arm7/arm7.bin -9 arm9/bootstrap.bin \
	$(_ADDFILES)
ifdef GBAR3_ISNITRO
	dlditool FatInNitroFS.dldi $(TARGET).nds
endif

#---------------------------------------------------------------------------------
../core/arm7/$(TARGET).elf:
	$(MAKE) -C ../core/arm7
	
#---------------------------------------------------------------------------------
arm9/$(TARGET).elf:
	$(MAKE) -C arm9

#---------------------------------------------------------------------------------
clean:
	$(MAKE) -C ../core/arm9 clean
	$(MAKE) -C arm9 clean
	$(MAKE) -C ../core/arm7 clean
	rm -f $(TARGET).nds $(TARGET).arm7 $(TARGET).arm9
	rm -f arm9/data/gbarunner9.bin
