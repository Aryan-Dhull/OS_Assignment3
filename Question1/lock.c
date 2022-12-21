#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>

pthread_mutex_t lock_forks[5];

void pickforkreverse(int p){
	pthread_mutex_lock(&lock_forks[p]);
	printf("Philosopher is trying to pick fork \n");
	printf("Philosopher %d picked up fork %d\n",p,p);
	pthread_mutex_lock(&lock_forks[(p+1)%5]);
	printf("Philosopher is trying to pick another fork \n");
	printf("Philosopher %d picked up fork %d\n",p,(p+1)%5);
}

void pickfork(int p){
	pthread_mutex_lock(&lock_forks[(p+1)%5]);
	printf("Philosopher is trying to pick fork \n");
	printf("Philosopher %d picked up fork %d\n",p,(p+1)%5);
	pthread_mutex_lock(&lock_forks[p]);
	printf("Philosopher is trying to pick another fork \n");
	printf("Philosopher %d picked up fork %d\n",p,p);
}

void releasefork(int p){
	pthread_mutex_unlock(&lock_forks[(p+1)%5]);
	printf("Philosopher %d released frok %d\n",p,(p+1)%5);
	pthread_mutex_unlock(&lock_forks[p]);
	printf("Philosopher %d released fork %d\n",p,p);
}



void* eat(void* num){
	while(1){
	int p_num=*(int *)num;
	if(p_num==0){
		pickforkreverse(p_num);
	}
	else{
		pickfork(p_num);
	}
	printf("Philosopher %d has started eating\n",p_num);
	releasefork(p_num);
	printf("Philosopher %d has finished eating\n",p_num);
	}
}

int main(){
	int a[5]={0,1,2,3,4};
	pthread_t philo[5];
	for(int i=0;i<5;i++){
		pthread_mutex_init(&lock_forks[i],NULL);
	}
	for(int i=0;i<5;i++){
		pthread_create(&philo[i],NULL,eat,(void* )&a[i]);
	}
	for(int i=0;i<5;i++){
		pthread_join(philo[i],NULL);
	}
	for(int i=0;i<5;i++){
		pthread_mutex_destroy(&lock_forks[i]);
	}
	return 0;
}
