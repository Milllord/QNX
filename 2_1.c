#include <process.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <sys/neutrino.h>
#include <inttypes.h>
#include <sys/syspage.h>
#include <semaphore.h>
#include <stdlib.h>

using namespace std;
#define TIME 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void bubbleSort(int arr[], int n)
{
    int i, j;
    int temp;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j+1];
                arr[j+1] = arr[j];
                arr[j] = temp;
            }
}

void reverseArray(int arr[], int n)
{
	int i = 0;
    for (i = 0; i < n; i++)
    {
        arr[i] = n - i;
    }
}

void parasite() {
	const int ARR_SIZE = 3000;
    int arr[ARR_SIZE];
    int n = 0;
    
    while (n != 3) {
        reverseArray(arr, ARR_SIZE);
        bubbleSort(arr, ARR_SIZE);
        n = rand() % 4;
    }
}

void* print_thread(void*)
{
	const char* str_out = "this is child thread tid = ";
	pthread_mutex_lock(&mutex);
	for(int i = 0; str_out[i] != '\0'; i++)
	{
		cout << str_out[i];
		parasite();
	} 
	cout << pthread_self() << endl;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main(int argc,char *argv[])
{
	srand(time(NULL));
	setbuf(stdout, NULL);
	
	const char* str_out = "THIS IS PARENT PID = ";
	sem_t *sem;
	errno = EOK;
	const char* name = "/sema";
	sem_unlink(name);
	sem = sem_open(name, O_CREAT|O_EXCL, S_IRWXU,1);
	
	while (sem_wait(sem) && errno == EINTR);
	if (sem == SEM_FAILED)
		cout << "PARANT ERR\n";
	if (spawnl(P_NOWAIT,"prog.out","prog.out",NULL) == -1)
		cout << "ERROR\n";
	if (spawnl(P_NOWAIT,"prog.out","prog.out",NULL) == -1)
		cout << "ERROR\n";
	if (spawnl(P_NOWAIT,"prog.out","prog.out",NULL) == -1)
		cout << "ERROR\n";
	
	for(int i = 0; str_out[i] != '\0'; i++)
	{
		cout << str_out[i];
		parasite();
	}
	
	cout<<getpid()<<endl;
	sem_post(sem);
	while(sem_wait(sem) && errno == EINTR);
	
	int status = -1;
	wait(&status);

	status = -1;
	wait(&status);

	sem_destroy(sem);
	sem_unlink("/dev/sem/sema");
	
	pthread_t tid1,tid2,tid3,tid4;
	pthread_attr_t attr1;
	pthread_attr_t attr2;
	pthread_attr_t attr3;
	pthread_attr_t attr4;
	pthread_attr_init(&attr1);
	pthread_attr_init(&attr2);
	pthread_attr_init(&attr3);
	pthread_attr_init(&attr4);
	
	pthread_create(&tid1,&attr1,&print_thread,NULL);
	pthread_create(&tid2,&attr2,&print_thread,NULL);
	pthread_create(&tid3,&attr3,&print_thread,NULL);
	pthread_create(&tid4,&attr4,&print_thread,NULL);
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
	pthread_join(tid4,NULL);
	
	return 0;
}
