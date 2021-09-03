#include "philosopher.h"

void* philosopher(void*);
void program_stop(int);

Philosopher philo;
int running ;


int main()
{
	pthread_t philosopher_thread[N];
	int* philosopher_num[N];
	running = TRUE;
	signal(SIGINT, program_stop); 

	for(int i = 0 ; i < N ; ++i)
	{
		philosopher_num[i] = (int*)malloc(sizeof(int));
		*philosopher_num[i] = i;
		philosopher_thread[i] = i;
		pthread_create(&philosopher_thread[i], NULL, philosopher, (void*)philosopher_num[i]);
	}

	while(TRUE && running)
	{
		philo.show();
		sleep(1);
	}

	for(int i = 0 ; i < N ; ++i)
	{
		free(philosopher_num[i]);
		pthread_join(philosopher_thread[i], NULL);
	}

	printf("\nAll philosopher STOP ACTION ! !\n");

	return 0;
}


void* philosopher(void* i) 
{
	int num = *( (int*)i );
	while (running)
	{
		philo.think();        
		philo.take_forks(num); 
		philo.eat();           
		philo.put_forks(num);  
	}
	
	printf("\nphilosopher %d STOP ACTION ! !\n",num);

	return NULL;
}

void program_stop(int i)
{
	running = FALSE;
}


