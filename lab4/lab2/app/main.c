#include "lib.h"
#include "types.h"

//用户程序，在用户空间，lab测试方法：make clean后make，然后make play
void testScanf();

int uEntry(void) {
	//下面三行不用管
	uint16_t selector;
	asm volatile("movw %%ss, %0":"=m"(selector)); // necessary or not, iret may reset ds in QEMU
	asm volatile("movw %%ax, %%ds"::"a"(selector));
	
	printf("I/O test begin...\n");
	printf("the answer should be:\n");
	printf("#######################################################\n");
	printf("Hello, welcome to OSlab! I'm the body of the game.\n");
	printf("Now I will test your printf:\n");
	printf("1 + 1 = 2, 123 * 456 = 56088, 0, -1, -2147483648, -1412505855, -32768, 102030, 0, ffffffff, 80000000, abcdef01, ffff8000, 18e8e\n");
	printf("Now I will test your getChar: ");
	printf("1 + 1 = 2\n2 * 123 = 246\n");
	printf("Now I will test your getStr: ");
	printf("Alice is stronger than Bob\nBob is weaker than Alice\n");
	printf("#######################################################\n");
	printf("your answer:\n");
	printf("=======================================================\n");
	printf("%s %s%scome %co%s", "Hello,", "", "wel", 't', " ");
	printf("%c%c%c%c%c! ", 'O', 'S', 'l', 'a', 'b');
	printf("I'm the %s of %s.\n", "body", "the game");
	printf("Now I will test your printf:\n");
	printf("%d + %d = %d, %d * %d = %d, ", 1, 1, 1 + 1, 123, 456, 123 * 456);
	printf("%d, %d, %d, %d, %d, %d, ", 0, 0xffffffff, 0x80000000, 0xabcedf01, -32768, 102030);
	printf("%x, %x, %x, %x, %x, %x\n", 0, 0xffffffff, 0x80000000, 0xabcedf01, -32768, 102030);
	printf("Now I will test your getChar: ");
	printf("1 + 1 = ");
	char num = getChar();
	printf("%c * 123 = 246\n",num);
	printf("Now I will test your getStr: ");
	printf("Alice is stronger than ");
	char name[20];
	getStr(name,20);
	printf("%s is stronger than Alice\n",name);
	printf("=======================================================\n");
	printf("Test end!!! Good luck!!!\n");
	//testScanf();
	while(1);
	return 0;
}

void testScanf()
{
	char ch, ch1, ch2;
	int integer, integer1, integer2;
	char str[40], str1[40], str2[40];
	
	printf("tests for scanf:\n");
	printf("input an integer: ");
	scanf("%d", &integer);
	printf("integer = %d\n", integer);
	printf("input an character: ");
	scanf("%c",&ch);
	printf("ch = %c\n", ch);
	printf("input an string: ");
	scanf("%s", str);
	printf("str = %s\n", str);
	printf("input three characters: ");
	scanf("%c %c %c", &ch, &ch1, &ch2);
	printf("ch = %c,  ch1 = %c, ch2 = %c\n", ch, ch1, ch2);
	printf("input three integers: ");
	scanf("%d %d %d", &integer, &integer1, &integer2);
	printf("integer = %d, integer1 = %d, integer2 = %d\n", integer, integer1, integer2);
	
	for(int i = 0; i < 40;++i)
		str[i] = 0;
	printf("input two strings: ");
	scanf("%s %s", str, str1);
	printf("str = %s, str1 = %s\n", str, str1);
	printf("input an character, integer, string: ");
	scanf("%c,%d,%s", &ch, &integer, str2);
	printf("ch = %c, integer = %d, str2 = %s\n", ch, integer, str2);
	printf("test finished!\n");
}



