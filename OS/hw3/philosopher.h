#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <pthread.h> 
#include <unistd.h>  
#include <signal.h>  


using namespace std;

#define THINKING 0     
#define HUNGRY 1       
#define EATING 2 
#define TRUE 1
#define FALSE 0
#define N 5            
#define LEFT (i+N-1)%N  
#define RIGHT (i+1)%N  

extern int  running;


class Philosopher{
private:
	int state[N];           
	pthread_mutex_t mutex;  
	pthread_cond_t s[N];    
public:
	Philosopher();
	void take_forks(int i);
	void put_forks(int i);
	void test(int i);
	void think();
	void eat();
	void show();
};

#endif
