#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("Missing patameter! Please set correct file path as parameter!\n");
        exit(1);
    }
    FILE* fp = NULL;
    fp = fopen(argv[1], "rb");
    if(fp == NULL){
        printf("Fail to open file to read!\n");
        exit(1);
    }
    
    char buf[1005];
    int n = fread(buf, sizeof(char), 1000, fp);
    if(n > 510){
        fprintf(stderr,"ERROR: boot block too large: %d bytes (max 510)\n", n);
        exit(1);
    }
    fprintf(stderr, "OK:boot block is %d bytes (max 510)\n", n);
    
    while(n < 510)
        buf[n++] = '\0';
    buf[n++] = 0x55;
    buf[n] = 0xaa;
    fclose(fp);

    fp = fopen(argv[1], "wb");
   if(fp == NULL){
       printf("Fail to open file to write!\n");
       exit(1);
   }
   fwrite(buf, sizeof(char), 512, fp);
   fclose(fp);
}
