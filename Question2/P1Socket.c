#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<time.h>
#include<stdint.h>

#define SOCKET_NAME "Aryan_Dhull.socket"
#define BUFFER_SIZE 10

void getRandStr(char randstrs[][12]) {        
    srand (time(NULL));                            
    for (int j = 0; j <= 50; j++) {
        randstrs[j][0] = j;
    }
    
    for(int i = 0; i <= 50; i++) {
        for(int j = 1; j <= 12; j++) {
            randstrs[i][j] = rand() % 26 + 65;
        }
    }
}

void printer(char randStr[51][12]){
        for(int i=1;i<=50;i++){
        for(int j=0;j<12;j++){
            if(j==0){
                printf("%d ",randStr[i][j]);
            }
            else{
                printf("%c",randStr[i][j]);
            }
        }
        printf("\n");
    }
}

int main(int argc,char* argv[]){
    struct timespec s,e;
    clock_gettime(CLOCK_MONOTONIC,&s);
    struct sockaddr_un soc;
    int connector;
    int dsoc;
    char buffer[10];
    dsoc=socket(AF_UNIX,SOCK_SEQPACKET,0);
    if(dsoc==-1){
        perror("Couldn't create socket");
        exit(EXIT_FAILURE);
    }

    memset(&soc,0,sizeof(soc));
    
    soc.sun_family= AF_UNIX;
    strncpy(soc.sun_path,SOCKET_NAME,sizeof(soc.sun_path)-1);
    connector=connect(dsoc,(const struct sockaddr *)&soc,sizeof(soc));

    if(connector==-1){
        perror("Server is down\n");
        exit(EXIT_FAILURE);
    }

    char strg[51][12]={{0}};
    getRandStr(strg);

    while(dsoc!=-1){
        printf("Error while sending strings\n");
        break;
    }
    
    printer(strg);
    
    if(argc>1){
        strncpy(buffer,"DOWN",sizeof("DOWN"));
        write(dsoc,buffer,sizeof(buffer));
        close(dsoc);
        exit(EXIT_SUCCESS);
    }
    else{
        int count=0;
        int ind=1;
        while(1){
            printf("Sending Strings from %d to %d\n",ind,ind+4);
            for(int i=ind;i<ind+5;i++){
                count++;
                connector=write(dsoc,strg[i],strlen(strg[i])+1);
                if(connector==-1){
                    count--;
                    perror("Error :- couldn't write");
                }
            }
            connector=read(dsoc,buffer,sizeof(buffer));
            buffer[sizeof(buffer)-1]=0;

            int finind=atoi(buffer);
            printf("MAX ID received %s\n\n",buffer);
            if(finind==50){
                printf("Successfully sent all Strings\n");
                clock_gettime(CLOCK_MONOTONIC,&e);
                uint64_t difference=(e.tv_sec-s.tv_sec);
                long double d,t;
                d=(long double)(e.tv_nsec-s.tv_nsec)/1000000000;
                t=(long long unsigned int)difference+d;
                printf("Time elapsed %Lf\n",t);
                strncpy(buffer,"DOWN",sizeof("DOWN"));
                write(dsoc,buffer,sizeof(buffer));
                close(dsoc);
                exit(EXIT_SUCCESS);
                break;
            }
            else{
                ind=finind+1;
            }
        }
    }

}
