#include<stdio.h>
#include<fcntl.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<pthread.h>
#include<sys/stat.h>
#include<sched.h>
#include<stdlib.h>
#include<unistd.h>

//sem_t sem_mutex_entry;
sem_t sem_forks[5];
void pickforkreverse(int p){
	sem_wait(&sem_forks[p]);
	printf("Philosopher is trying to pick fork\n");
	printf("Philosopher %d has picked up fork %d\n",p,p);
	sem_wait(&sem_forks[(p+1)%5]);
	printf("Philosopher is trying to pick another fork\n");
	printf("Philosopher %d has picked up fork %d\n",p,(p+1)%5);
}

void pickfork(int p){
	sem_wait(&sem_forks[(p+1)%5]);
	printf("Philosopher is trying to pick fork\n");
	printf("Philosopher %d has picked up fork %d\n",p,(p+1)%5);
	sem_wait(&sem_forks[p]);
	printf("Philosopher is trying to pick another fork\n");
	printf("Philosopher %d has picked up fork %d\n",p,p);
}

void releasefork(int p){
	sem_post(&sem_forks[p]);
	printf("Philosopher %d has released fork %d\n",p,p);
	sem_post(&sem_forks[(p+1)%5]);
	printf("Philosopher %d has released fork %d\n",p,(p+1)%5);
}

void* eat(void* num){
	while(1){
	int p_num=*(int *)num;
	//sem_wait(&sem_mutex_entry);
	if(p_num==4){
		pickforkreverse(p_num);
	}
	else{
		pickfork(p_num);
	}
	//printf("Philoopher %d finished eating\n",p);
	releasefork(p_num);
	printf("Philosopher %d has finished eating\n",p_num);
	//sem_post(&sem_mutex_entry);
	}
}

int main(){
	int a[5]={0,1,2,3,4};
	pthread_t philo[5];
	//sem_init(&sem_mutex_entry,0,4);
	for(int i=0;i<5;i++){
		sem_init(&sem_forks[i],0,1);
	}
	for(int i=0;i<5;i++){
		pthread_create(&philo[i],NULL,eat,(void *)&a[i]);
	}
	for(int i=0;i<5;i++){
		pthread_join(philo[i],NULL);
	}
}
