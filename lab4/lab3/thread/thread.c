#include"thread.h"

__asm__                    //声明一个函数，叫做callNext                          
(                                                                                
".global callNext       \n\t"    
"callNext:              \n\t"                                
"   pop (%ecx)          \n\t"
"   jmp *%edx           \n\t"
);

//终止并返回
#define yield(x)  asmV("pushal");\
    asmV("mov %0, %1":"=r"(tcbList[currentId].tarResValue):"r"(x));\
    asmV("mov %%esp,%0":"=r"(tcbList[currentId].tarESP));\
    asmV("mov %0, %%esp"::"r"(tcbList[currentId].srcESP));\
    asmV("mov %0, %%ecx     \n\t"\
        "mov %1, %%edx      \n\t"\
        "call callNext      \n\t"\
        ::"r"(&tcbList[currentId].tarEIP),"r"(tcbList[currentId].srcEIP)\
    );\
    asmV("popal");

//销毁线程
#define finish  asmV("pushal");\
    asmV("mov %0, %1":"=r"(tcbList[currentId].status):"r"(status_finish));\
    asmV("mov %%esp,%0":"=r"(tcbList[currentId].tarESP));\
    asmV("mov %0, %%esp"::"r"(tcbList[currentId].srcESP));\
    asmV("mov %0, %%ecx     \n\t"\
        "mov %1, %%edx      \n\t"\
        "call callNext      \n\t"\
        ::"r"(&tcbList[currentId].tarEIP),"r"(tcbList[currentId].srcEIP)\
    );\
    asmV("popal");

//触发线程
#define launch(x) currentId = x;\
    asmV("pushal");\
    asmV("mov %%esp, %0":"=r"(tcbList[currentId].srcESP));\
    asmV("mov %0, %%esp"::"r"(tcbList[currentId].tarESP));\
    asmV("mov %0, %%ecx \n\t"\
        "mov %1, %%edx \n\t"\
        "call callNext \n\t"\
        ::"r"(&tcbList[currentId].srcEIP),"r"(tcbList[currentId].tarEIP)\
    );\
    asmV("popal");

tcb tcbList[maxThread];
int currentId;
empty_tcb* emptyList = NULL;

void init_empty_tcb()
{
    for(int i = maxThread-1; i >= 0; i--)
    {
       
        empty_tcb* p  = (empty_tcb*)malloc(sizeof(empty_tcb));
        p->id = i;
        p->next = emptyList;
        emptyList = p;
    }
}
//创建一个线程，众所周知，一个线程相当于一个函数，第一个参数是【函数指针】，第二个参数是【函数参数多少】，后面的变长参数就是【函数的参数】
int create_thread(__uint32_t func, int argNum, ...){
    
    //下面是查找空闲tcb，那么这个管理方式，查找的时间复杂度是多少？你是否能够设计更加合适的数据结构，来加快访问速度？
    int id;
    
    for(id = 0; id < maxThread; id++){
        if (tcbList[id].status == status_empty){
            break;
        }
    }
    if (id >= maxThread) return -1;

    /*
   if (emptyList == NULL)
        return -1;
    else{
        id = emptyList->id;
        emptyList = emptyList->next;
    }
    */

    //初始化
    printf("***thread(id %d) create***\n", id);
    tcbList[id].status = status_ready;
    tcbList[id].tarESP = maxStackSize;
    
    //函数参数
    va_list valist;
    va_start(valist, argNum);
    tcbList[id].tarESP = tcbList[id].stack + maxStackSize;
    tcbList[id].tarESP -= 4 * argNum;
    for(int i = 0; i < argNum ; i++){
        *(__uint32_t*)(tcbList[id].tarESP + i * 4) = va_arg(valist, int);
    }
    va_end(valist);
    tcbList[id].tarESP -= 4;
    tcbList[id].tarEIP = func;
    return id;
}

int launch_thread(int id){
    if (tcbList[id].status == status_empty){
        printf("***thread(id %d) is empty!***\n", id);
        return -1;
    }
    if (tcbList[id].status == status_ready){
        printf("***thread(id %d) begin***\n", id);
        tcbList[id].status = status_run;
    }
    launch(id);
    if (tcbList[id].status == status_finish){
        printf("***thread(id %d) finish***\n", id);
        tcbList[id].status = status_empty;
        empty_tcb* p = (empty_tcb*)malloc(sizeof(empty_tcb));
        p->id = id;
        p->next = emptyList;
        emptyList = p;
        return 0;
    }
    return tcbList[id].tarResValue;
}

void yield_thread(int id){
    yield(id)
}

void destory_thread(){
    finish
}

enum threadStatus getThreadStatus(int id){
    return tcbList[id].status;
}
