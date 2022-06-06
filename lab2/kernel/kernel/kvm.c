#include "x86.h"
#include "device.h"
#include "common.h"

#define PT_LOAD 1

SegDesc gdt[NR_SEGMENTS];       // the new GDT, NR_SEGMENTS=7, defined in x86/memory.h
TSS tss;

//init GDT and LDT
void initSeg() { // setup kernel segements
	gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_KERN);
	gdt[SEG_KDATA] = SEG(STA_W,         0,       0xffffffff, DPL_KERN);
	//gdt[SEG_UCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_USER);
	gdt[SEG_UCODE] = SEG(STA_X | STA_R, 0x00200000,0x000fffff, DPL_USER);
	//gdt[SEG_UDATA] = SEG(STA_W,         0,       0xffffffff, DPL_USER);
	gdt[SEG_UDATA] = SEG(STA_W,         0x00200000,0x000fffff, DPL_USER);
	gdt[SEG_TSS] = SEG16(STS_T32A,      &tss, sizeof(TSS)-1, DPL_KERN);
	gdt[SEG_TSS].s = 0;
	setGdt(gdt, sizeof(gdt)); // gdt is set in bootloader, here reset gdt in kernel

	/*
	 * 初始化TSS
	 */
	tss.esp0 = 0x1fffff;
	tss.ss0 = KSEL(SEG_KDATA);
	asm volatile("ltr %%ax":: "a" (KSEL(SEG_TSS)));

	/*设置正确的段寄存器*/
	asm volatile("movw %%ax,%%ds":: "a" (KSEL(SEG_KDATA)));
	//asm volatile("movw %%ax,%%es":: "a" (KSEL(SEG_KDATA)));
	//asm volatile("movw %%ax,%%fs":: "a" (KSEL(SEG_KDATA)));
	//asm volatile("movw %%ax,%%gs":: "a" (KSEL(SEG_KDATA)));
	asm volatile("movw %%ax,%%ss":: "a" (KSEL(SEG_KDATA)));

	lLdt(0);
	
}

void enterUserSpace(uint32_t entry) {
	/*
	 * Before enter user space 
	 * you should set the right segment registers here
	 * and use 'iret' to jump to ring3
	 */
	uint32_t EFLAGS = 0;
	asm volatile("pushl %0":: "r" (USEL(SEG_UDATA))); // push ss
	asm volatile("pushl %0":: "r" (0x2fffff)); 
	asm volatile("pushfl"); //push eflags, sti
	asm volatile("popl %0":"=r" (EFLAGS));
	asm volatile("pushl %0"::"r"(EFLAGS|0x200));
	asm volatile("pushl %0":: "r" (USEL(SEG_UCODE))); // push cs
	asm volatile("pushl %0":: "r" (entry)); 
	asm volatile("iret");
}

/*
kernel is loaded to location 0x100000, i.e., 1MB
size of kernel is not greater than 200*512 bytes, i.e., 100KB
user program is loaded to location 0x200000, i.e., 2MB
size of user program is not greater than 200*512 bytes, i.e., 100KB
*/

void loadUMain(void) {
	// TODO: 参照bootloader加载内核的方式，具体加载到哪里请结合手册提示思考！
	uint32_t elf = 0x2f000;
	readSect((void*)elf, 201);
	ELFHeader* elfHeader = (ELFHeader*)elf;

	int i = 0;
	int phoff = elfHeader->phoff;
	int phnum = elfHeader->phnum;
	uint32_t uMainEntry = elfHeader->entry ;//read to 0x200000

	
	ProgramHeader* ph = (ProgramHeader*) ((uint32_t)elfHeader + phoff);

	for(int i = 0; i < phnum;++i)
	{
		if(ph->type == PT_LOAD)
		{
			for(int j = 0; j < ph->memsz/512+1; ++j)
			{
				readSect((void*)(0x200000+ph->vaddr + j*512) ,ph->off/512+201+j );
			}
				
		}
		++ph;
	}

	enterUserSpace(uMainEntry);
	
}
