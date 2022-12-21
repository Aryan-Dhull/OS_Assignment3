#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<stdint.h>

#define SHM_KEY 0x1234

struct shmseg{
    int complete;
    int receive;
    char buffer[5][12];
};

char str[50][12]={{0}};

int main(){
    struct timespec st,e;
    clock_gettime(CLOCK_MONOTONIC,&st);
    int shm;
    struct shmseg* s;
    
    shm=shmget(SHM_KEY,sizeof(struct shmseg),0644|IPC_CREAT);
    
    s=shmat(shm,NULL,0);
                                
    for (int j = 0; j <= 50; j++) {
        str[j][0] = j;
    }
    
    for(int i = 0; i <= 50; i++) {
        for(int j = 1; j <= 10; j++) {
            str[i][j] = rand() % 26 + 65;
        }
    }

    int t=1;int r=1;
    while(r==1){
        for(int i=0;i<5;i++){
            for(int j=0;j<12;j++){
                s->buffer[i][j]=str[t][j];
            }
            t++;
        }
        s->complete=0;
        printf("Sending strings\n");
        sleep(3);
        printf("Id received is %d\n",s->receive);
        if(s->receive==50){
            r=0;
            break;
        }
    }
    printf("Strings sent successfully\n");
    s->complete=1;
    clock_gettime(CLOCK_MONOTONIC,&e);
    uint64_t difference = e.tv_sec-st.tv_sec;
    long double d=(long double)(e.tv_nsec-st.tv_nsec)/1000000000;
    long double k=(long long unsigned int)difference+d;
    printf("Time taken : %Lf\n",k);
    return 0;
}