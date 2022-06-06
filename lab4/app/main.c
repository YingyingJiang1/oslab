#include "lib.h"
#include "types.h"
#define N 5



void producer(sem_t empty, sem_t full, sem_t mutex);
void consumer(sem_t empty, sem_t full, sem_t mutex);

int uEntry(void) {

	// 测试scanf	
	int dec = 0;
	int hex = 0;
	char str[6];
	char cha = 0;
	int ret = 0;
	while(1){
		printf("Input:\" Test %%c Test %%6s %%d %%x\"\n");
		ret = scanf(" Test %c Test %6s %d %x", &cha, str, &dec, &hex);
		printf("Ret: %d; %c, %s, %d, %x.\n", ret, cha, str, dec, hex);
		if (ret == 4)
			break;
	}
	

	// 测试信号量
	int i = 4;
	sem_t sem;
	printf("Father Process: Semaphore Initializing.\n");
	ret = sem_init(&sem, 0);
	if (ret == -1) {
		printf("Father Process: Semaphore Initializing Failed.\n");
		exit();
	}

	ret = fork();
	if (ret == 0) {
		while( i != 0) {
			i --;
			printf("Child Process: Semaphore Waiting.\n");
			sem_wait(&sem);
			printf("Child Process: In Critical Area.\n");
		}
		printf("Child Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		exit();
	}
	else if (ret != -1) {
		while( i != 0) {
			i --;
			printf("Father Process: Sleeping.\n");
			sleep(128);
			printf("Father Process: Semaphore Posting.\n");
			sem_post(&sem);
		}
		printf("Father Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		exit();
	}

	// For lab4.3
	// TODO: You need to design and test the philosopher problem.
	// Note that you can create your own functions.
	// Requirements are demonstrated in the guide.
	
	//哲学家
	/*
	sem_t nums;
	sem_t forks[5];
	sem_init(&nums,4);
	for(int i = 0; i < 5; ++i)
		sem_init(&forks[i], 1);
	int ret = 0;
	int index = 0;
	for(int i = 0; i < 4;++i)
	{
		ret = fork();
		if(ret == -1)
		{
			printf("fail to create child process\n");
			return 0;
		}
		if(ret == 0)
		{
			index = i+1;
			printf("create child process: id = %d index = %d\n",getpid() , index);
			sleep(128);
			break;
		}
		else if (ret != -1)
			sleep(1);

	}
	while(1)
	{
		int id = getpid();
		printf("Philosopher %d: think\n", id);
		sleep(128);
		sem_wait(&nums);
		sleep(128);
		sem_wait(&(forks[index]));
		sleep(128);
		sem_wait(&(forks[(index+1)%5]));
		sleep(128);
		printf("Philosopher %d: eat\n", id);
		sleep(128);
		sem_post(&(forks[id-1]));
		sleep(128);
		sem_post(&(forks[id%5]));
		sleep(128);
		sem_post(&nums);
		
		//exit();
	}
	*/
	//生产者消费者问题
	/*
	sem_t empty, full, mutex;
	sem_init(&empty, 5);
	sem_init(&full, 0);
	sem_init(&mutex, 1);
	int flag = 0;
	printf("buffer size : %d\n", 5);
	for(int i = 0; i < 4; ++i)
	{
		int ret = fork();
		if(ret == 0)
		{
			printf("create producer process: %d\n", getpid());
			flag = 1;//producer;
			sleep(128);
			break;
		}

		else if(ret != -1)
		{
			sleep(1);
		}
	}
	while(1)
	{
		if(flag == 1)
			producer(empty,  full, mutex);
		else if(flag == 0)
			consumer(empty,  full, mutex);
	}
	*/
	//读者写者问题
	
	exit(0);
	return 0;
}

void producer(sem_t empty, sem_t full, sem_t mutex)
{
	sem_wait(&empty);
	sleep(128);
	sem_wait(&mutex);
	sleep(128);
	printf("Producer %d: produce\n", getpid());
	sleep(128);
	sem_post(&mutex);
	sleep(128);
	sem_post(&full);
	sleep(128);
}

void consumer(sem_t empty, sem_t full,sem_t mutex)
{
	sem_wait(&full);
	sleep(128);
	sem_wait(&mutex);
	sleep(128);
	printf("Consumer : consume\n");
	sleep(128);
	sem_post(&mutex);
	sleep(128);
	sem_post(&empty);
	sleep(128);
}