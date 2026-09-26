# =============================================================================
# xOS MASTER BUILD SYSTEM AUTOMATION FACTORY - Day 3 Multi-Language Linking
# =============================================================================

# Define cross-compiler toolset targeting ARM64 mobile architecture
CC = aarch64-linux-gnu-gcc
CXX = aarch64-linux-gnu-g++
AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld

# Compiler optimizations and freestanding flags for bare-metal operating systems
CFLAGS = -ffreestanding -O2 -Wall -Wextra
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# Object binary layout list - Perfectly matching your complete hardware pipeline!
OBJ = boot.o kernel.o driver.o ui.o secure.o Storagefiles.o touch_pipeline.o

# Master production target
all: xos_kernel.elf

# 1. LINKING STEP: Stitches Assembly, C++, and pure C modules into the final green binary
xos_kernel.elf: $(OBJ)
	$(LD) -T linker.ld -o xos_kernel.elf $(OBJ)

# 2. COMPILATION RULES: Translates your source text lines into raw machine bytes
boot.o: boot.asm
	$(AS) boot.asm -o boot.o

kernel.o: kernel.cpp
	$(CXX) $(CXXFLAGS) -c kernel.cpp -o kernel.o

driver.o: driver.cpp
	$(CXX) $(CXXFLAGS) -c driver.cpp -o driver.o

ui.o: ui.cpp
	$(CXX) $(CXXFLAGS) -c ui.cpp -o ui.o

secure.o: secure.c
	$(CC) $(CFLAGS) -c secure.c -o secure.o

Storagefiles.o: Storagefiles.c
	$(CC) $(CFLAGS) -c Storagefiles.c -o Storagefiles.o

# Brand-new compiled line target for your raw touch register hardware driver!
touch_pipeline.o: touch_pipeline.cpp
	$(CXX) $(CXXFLAGS) -c touch_pipeline.cpp -o touch_pipeline.o

# Maintenance routine to flush out cached object files and clear the console
clean:
	rm -f *.o xos_kernel.elf
