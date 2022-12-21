#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int fd1;

    char * myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    char buffer[100];
    char ackn[10];
    int index = 0;
    int ret;
    while (index < 50)
    {
        fd1 = open(myfifo,O_RDONLY);
        ret=read(fd1, buffer , 100);
        if(ret==-1){
            perror("Read\n");
            exit(EXIT_FAILURE);
        }

        printf("Strings received from P1\n %s", buffer);
        close(fd1);

        index += 5;
        char ind[5];
        sprintf(ind,"%d",index);
	    strcpy(ackn,ind);

        printf("Acknowledge index sent back to P1\n");

        fd1 = open(myfifo,O_WRONLY);
        write(fd1, ackn, strlen(ackn)+1);
        close(fd1);

    }

    return 0;
    
}