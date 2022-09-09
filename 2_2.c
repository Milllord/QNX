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

int main(int argc,char *argv[])
{
	setbuf(stdout, NULL);
	srand(time(NULL));
	pid_t pid = getpid();
	
	sem_t *sem;
	const char* name = "/sema";
	const char* str_out = "this is child pid = ";
	sem=sem_open(name,O_EXCL);
	
	if (sem == SEM_FAILED)
		cout << "ERR\n";
	while (sem_wait(sem) && errno == EINTR);
	
	for (int i = 0; str_out[i] != '\0'; i++)
	{
		cout << str_out[i];
		parasite();
	}
	
	cout << pid << endl;
	sem_post(sem);
	
	return 0;
}
