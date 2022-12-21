#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

sem_t sem_forks[5];
sem_t sem_bowl;
//sem_t mutex;

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

void bowl(int p){
	int val;
	sem_getvalue(&sem_bowl,&val);
	printf("Value of bowl semaphore is %d\n",val); 
	sem_wait(&sem_bowl);
	printf("Philosopher %d picked bowl %d\n",p,val);

}

void bowlrelease(int p){
	int val;
	sem_post(&sem_bowl);
	printf("Philosopher %d released bowl %d\n",p,val);
	sem_getvalue(&sem_bowl,&val);
	printf("Value of semaphore is %d\n",val);
}

void forkrelease(int p){
	sem_post(&sem_forks[(p+1)%5]);
	printf("Philosopher %d released fork %d\n",p,(p+1)%5);
	sem_post(&sem_forks[p]);
	printf("Philosopher %d released fork %d\n",p,p);
}

void* eat(void* num){
	while(1){
		int p_num=*(int *)num;
		//int val;
		//sem_wait(&mutex);
		if(p_num==4){
			pickforkreverse(p_num);
		}
		else{
			pickfork(p_num);
		}
		bowl(p_num);
		printf("Philosopher is eating\n");
		bowlrelease(p_num);
		forkrelease(p_num);
		printf("Philosopher %d finished eating\n",p_num);
		//sem_post(&mutex);
	}		
}

int main(){
	pthread_t philo[5];
	int a[5]={0,1,2,3,4};
	
	//sem_init(&mutex,0,2);
	sem_init(&sem_bowl,0,2);

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
