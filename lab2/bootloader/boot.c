#include "boot.h"
#include "../kernel/include/common.h"

#define SECTSIZE 512
#define PT_LOAD 1




void bootMain(void) {
	// TODO: 填写kMainEntry、phoff、offset...... 然后加载Kernel（可以参考NEMU的某次lab）
	//get the address of elf header
	void* buf = (void*)0x200000;
	//unsigned int elf = 0x100000;
	readSect((void*)buf, 1);
	ELFHeader* elfHeader = (ELFHeader*)buf;
	//get the offset of .text in the disk
	int phoff = elfHeader->phoff;
	ProgramHeader* ph =  (ProgramHeader*)((uint32_t)elfHeader + phoff);
	//int offset = 0;
	
	void (*kMainEntry)(void);
	kMainEntry = (void*)elfHeader->entry;
	int phnum = elfHeader->phnum;
	for(int i = 0; i < phnum;++i)
	{
		if(ph->type == PT_LOAD)
		{
			for(int j = 0; j < ph->memsz/512+1; ++j)
			{
				readSect((void*)(ph->vaddr + j*512) ,ph->off/512+1+j );
			}
				
		}
		++ph;
	}
	
	
	




	kMainEntry();
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
