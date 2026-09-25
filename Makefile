# xOS Master Build Script - Full Multi-Module ARM64 Compilation
CC = aarch64-linux-gnu-gcc
AS = aarch64-linux-gnu-as
CXX = aarch64-linux-gnu-g++
LD = aarch64-linux-gnu-ld

CFLAGS = -c -O2 -Wall -Wextra
CXXFLAGS = -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

all: xos_kernel.elf

xos_kernel.elf: boot.o kernel.o driver.o ui.o appopen.o
	$(LD) -T linker.ld boot.o kernel.o driver.o ui.o appopen.o -o xos_kernel.elf

# Automatically use the correct ARM assembler tool!
boot.o: boot.asm
	$(AS) boot.asm -o boot.o

kernel.o: kernel.cpp
	$(CXX) $(CXXFLAGS) kernel.cpp -o kernel.o

driver.o: driver.cpp
	$(CXX) $(CXXFLAGS) driver.cpp -o driver.o

ui.o: ui.cpp
	$(CXX) $(CXXFLAGS) ui.cpp -o ui.o

appopen.o: apps/appopen.cpp
	$(CXX) $(CXXFLAGS) apps/appopen.cpp -o appopen.o

clean:
	rm -rf *.o *.elf


