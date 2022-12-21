#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<time.h>
#include<stdint.h>

#define SOCKET_NAME "Aryan_Dhull.socket"

int main(){
    struct sockaddr_un server;
    int dflg=0;
    int connect_socket;
    int connector;
    int mind=0;
    int data_socket;
    char buffer[12];
    
    struct timespec s,e;
    clock_gettime(CLOCK_MONOTONIC,&s);
    connect_socket=socket(AF_UNIX,SOCK_SEQPACKET,0);
    if(connect_socket==-1){
        perror("Socket couldn't be made");
        exit(EXIT_FAILURE);
    }

    memset(&server,0,sizeof(server));
    server.sun_family=AF_UNIX;
    strncpy(server.sun_path,SOCKET_NAME,sizeof(server.sun_path)-1);
    connector=bind(connect_socket,(const struct sockaddr*) &server,sizeof(server));

    if(connector==-1){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    connector=listen(connect_socket,100);
    if(connector==-1){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){
        data_socket=accept(connect_socket,NULL,NULL);
        if(data_socket==-1){
            perror("Couldn't accept");
            exit(EXIT_FAILURE);
        }
        mind=0;
        int currentindex=mind;
        while(1){
            connector=read(data_socket,buffer,sizeof(buffer));
            if(connector==-1){
                perror("Couldn't read");
                exit(EXIT_FAILURE);
            }
            buffer[sizeof(buffer)-1]=0;
            currentindex=buffer[0];

            if(!strncmp(buffer,"DOWN",sizeof(buffer))){
                dflg=1;
                break;
            }
            printf("Strings received from CLIENT: ");

            for(int i=1;i<11;i++){
                printf("%c",buffer[i]);
            }
            printf("\n");
            sprintf(buffer,"%d",currentindex);
            printf("ID received from client = %s\n",buffer);
            
            if(currentindex==mind+5){
                mind=currentindex;
                connector=write(data_socket,buffer,sizeof(buffer));
            }
            if(currentindex>=50) break;
        }
        close(data_socket);

        if(dflg){
            printf("SHUTTING SERVER");
            close(connect_socket);
            unlink(SOCKET_NAME);
            exit(EXIT_SUCCESS);
            break;
        }
        int r=0;
        if(r==-1){
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    clock_gettime(CLOCK_MONOTONIC,&e);
    uint64_t diff=e.tv_sec-s.tv_sec;
    long double d=(long double)(e.tv_nsec-s.tv_nsec)/1000000000;
    long double t=(long long unsigned int)diff+d;
    printf("Time taken %Lf\n",t); 
    return 0;
}
