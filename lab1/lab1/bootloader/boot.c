#include "boot.h"

#define SECTSIZE 512

void bootMain(void) {
	//将磁盘第1个扇区内容app.bin装入内存0x8c00，第0个扇区内容是bootloader.bin
        //之后跳转到0x8c00处执行app.bin中的指令

	readSect((void*)0x8c00, 1);
	asm("jmp 0x8c00");	
}

void waitDisk(void) { // waiting for disk
	while((inByte(0x1F7) & 0xC0) != 0x40);
}

void readSect(void *dst, int offset) { // reading a sector of disk
	int i;
	waitDisk();
	outByte(0x1F2, 1);
	outByte(0x1F3, offset);
	outByte(0x1F4, offset >> 8);
	outByte(0x1F5, offset >> 16);
	outByte(0x1F6, (offset >> 24) | 0xE0);
	outByte(0x1F7, 0x20);

	waitDisk();
	for (i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = inLong(0x1F0);
	}
}
