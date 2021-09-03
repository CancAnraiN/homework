#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <pthread.h> 
#include <unistd.h>  
#include <signal.h>
  
#include "philosopher.h"
using namespace std;


Philosopher::Philosopher()
{
	pthread_mutex_init(&mutex,NULL);
	for(int i = 0 ; i < N ; ++i)
		pthread_cond_init(&s[i],NULL);
}


void Philosopher::take_forks(int i) 
{
	pthread_mutex_lock(&mutex);   
	state[i] = HUNGRY;            
	test(i);                       
	pthread_mutex_unlock(&mutex); 
}

void Philosopher::put_forks(int i) 
{
	pthread_mutex_lock(&mutex);     
	state[i] = THINKING;            
	pthread_cond_signal(&s[LEFT]);
	pthread_cond_signal(&s[RIGHT]);
	pthread_mutex_unlock(&mutex); 
}

void Philosopher::test(int i) 
{
	while(TRUE && running)
	{
		if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
		{
			state[i] = EATING;
			break;
		}
		else
			pthread_cond_wait(&s[i],&mutex);
	}
}

void Philosopher::think()
{
	sleep(rand()%3+1);
}

void Philosopher::eat()
{
	sleep(rand()%3+1);
}

void Philosopher::show()
{
	
	for(int i = 0; i < N; i++)
	{
		cout << '\t';
		if(state[i] == THINKING){
			cout << "think" ;
		}
		else if(state[i] == HUNGRY){
			cout <<  "hungr";
		}
		else if(state[i] == EATING){
			cout <<  "eatin";
		}
	}
	cout << endl;
}
