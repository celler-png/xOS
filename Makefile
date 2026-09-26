# =============================================================================
# xOS MASTER BUILD SYSTEM AUTOMATION FACTORY - Day 3 Raw ROM Image Output
# =============================================================================

CC = aarch64-linux-gnu-gcc
CXX = aarch64-linux-gnu-g++
AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy

CFLAGS = -ffreestanding -O2 -Wall -Wextra
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# Target list matching your clean Android 12 base kernel integration tracks
OBJ = boot.o driver.o ui.o secure.o Storagefiles.o touch_pipeline.o xos_kernel_mod_bin.o

# The absolute master target is now a pure raw ROM system image file!
all: xos_firmware.img

# 1. FIRMWARE EXPORT STEP: Strips all computer layout headers and outputs a raw binary ROM
xos_firmware.img: xos_intermediate.elf
	$(OBJCOPY) -O binary xos_intermediate.elf xos_firmware.img

# 2. LINKING STEP: Combines your code elements inside an intermediate linker layer
xos_intermediate.elf: $(OBJ)
	$(LD) -T linker.ld -o xos_intermediate.elf $(OBJ)

# 3. COMPILATION RULES: Translates your source text lines into raw machine bytes
boot.o: boot.asm
	$(AS) boot.asm -o boot.o

driver.o: driver.cpp
	$(CXX) $(CXXFLAGS) -c driver.cpp -o driver.o

ui.o: ui.cpp
	$(CXX) $(CXXFLAGS) -c ui.cpp -o ui.o

secure.o: secure.c
	$(CC) $(CFLAGS) -c secure.c -o secure.o

Storagefiles.o: Storagefiles.c
	$(CC) $(CFLAGS) -c Storagefiles.c -o Storagefiles.o

touch_pipeline.o: touch_pipeline.cpp
	$(CXX) $(CXXFLAGS) -c touch_pipeline.cpp -o touch_pipeline.o

xos_kernel_mod_bin.o: Image-XOS.bin
	$(LD) -r -b binary -o xos_kernel_mod_bin.o Image-XOS.bin

clean:
	rm -f *.o *.elf xos_firmware.img
