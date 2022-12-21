#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>

#define SHM_KEY 0x1234

struct shmseg{
    int complete;
    int receive;
    char buffer[5][12];
};

int main(){
    sleep(3);
    int shm;
    struct shmseg* s;
    shm=shmget(SHM_KEY,sizeof(struct shmseg),0644|IPC_CREAT);
    
    s=shmat(shm,NULL,0);

    while(s->complete!=1){
        for(int i=0;i<5;i++){
            for(int j=0;j<12;j++){
                if(j==0){
                    printf("%d",s->buffer[i][0]);
                }
                else{
                    printf("%c",s->buffer[i][j]);
                }
            }
            printf("\n");
        }
        printf("Strings read successfully\n");
        s->receive=(int )s->buffer[4][0];
        sleep(3);
    }
    printf("Reading done successfully\n");
    return 0;
}
