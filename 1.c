#include <sys/neutrino.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/syspage.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void mutexFunction()
{
	int i;
	uint64_t cycle1, cycle2, nCycles, cps;
	double sec;
	FILE* f;
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	
	f = fopen("Result.txt","a");
	cycle1 = ClockCycles();

	for(i = 0; i<10000; i++)
	{
	    pthread_mutex_lock(&mutex);
   	    pthread_mutex_unlock(&mutex);
	}
	cycle2 = ClockCycles();
	
	nCycles = cycle2 - cycle1;
	cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
	sec = (double)nCycles / cps;
	
	pthread_mutex_destroy(&mutex);
	
	fprintf(f, "Mutex\n");
	fprintf(f, "%lld cycles\n", nCycles);
	fprintf(f, "%lld cycles per second \n", cps);
	fprintf(f, "%f seconds\n", sec);
	fprintf(f, "\n");
	fclose(f);			
}

void unnamedSemaphoreFunction()
{
	int i;
	uint64_t cycle1, cycle2, nCycles, cps;
	double sec;
	FILE* f;
	sem_t sem;
	sem_init(&sem, 0,1);
	
	f = fopen("Result.txt","a");
	cycle1 = ClockCycles();
	for(i = 0; i < 10000; i++)
	{
		sem_post(&sem);
		sem_wait(&sem);	
	}
	cycle2 = ClockCycles();
	
	nCycles = cycle2 - cycle1;
	cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
	sec = (double)nCycles / cps;
	
	sem_destroy(&sem);
	
	fprintf(f, "UnnamedSemaphore\n");
	fprintf(f, "%lld cycles\n",nCycles);
	fprintf(f, "%lld cycles per second \n", cps);
	fprintf(f, "%f seconds\n", sec);
	fprintf(f, "\n");
	fclose(f);			
}

void namedSemaphoreFunction()
{
	int i;
	uint64_t cycle1, cycle2, nCycles, cps;
	double sec;
	FILE* f;
	sem_t *sem;
	sem_unlink("/my_semaphore");
	sem = sem_open("/my_semaphore", O_CREAT|O_EXCL, S_IRWXU, 1);
	
	f = fopen("Result.txt", "a");
	cycle1 = ClockCycles();
	cycle2 = ClockCycles();
	
	nCycles = cycle2 - cycle1;
	cps = SYSPAGE_ENTRY(qtime)->cycles_per_sec;
	sec = (double)nCycles / cps;
	
	fprintf(f, "NamedSemaphore\n");
	fprintf(f, "%lld cycles\n", nCycles);
	fprintf(f, "%lld cycles per second \n", cps);
	fprintf(f, "%f seconds\n", sec);
	fprintf(f, "\n");
	fclose(f);			
	
	sem_destroy(sem);
}

int main(int argc, char* argv[])
{
	FILE *f;
	f = fopen("Result.txt","w");
	fclose(f);
	
	mutexFunction();
	unnamedSemaphoreFunction();
	namedSemaphoreFunction();
	
	return 0;
}
