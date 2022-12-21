#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

char arr[51][10];
char send[100];


void Randomgenerator(){
    for(int i = 1; i<51 ; i++){
        char string[10] = "random";
        char no[5];
        sprintf(no,"%d",i);
        strcat(string,no);
        strcpy(arr[i],string);
    }
}


void cocatination(int in){
    strcat(arr[in],"\n");

    for(int i = in+1 ; i<= in+4 ; i++){

        strcat(arr[in],arr[i]);
        strcat(arr[in],"\n");	

	}

}


int main()
{
    struct timespec st,end;
	clock_gettime(CLOCK_MONOTONIC,&st);
    int fd;
    Randomgenerator();

    char * myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    int in = 1;
    char receive[5];
    int ret;

    while (in < 50)
    {
        fd = open(myfifo, O_WRONLY);
        
        cocatination(in);
        
        printf("\nStrings been sent to p2\n");
        strcpy(send,arr[in]);

        ret= write(fd, send, strlen(send)+1);
        if(ret==-1){
            perror("Write\n");
            exit(EXIT_FAILURE);
        }
        close(fd);

        fd = open(myfifo, O_RDONLY);

        read(fd, receive, sizeof(receive));
        in = atoi(receive);

        printf("Acknowledge id received from P2 %d\n", in);

        close(fd);

    }
    clock_gettime(CLOCK_MONOTONIC,&end);
	long long int diff = end.tv_sec-st.tv_sec;
	long double d=(long double)(end.tv_nsec-st.tv_nsec)/1000000000;
	printf("Time taken : %Lf\n", (long long unsigned int)diff+d);
    return 0;
}