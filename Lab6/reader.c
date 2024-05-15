#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


int main (){
    const int sizeBuffer = 1024;
    mkfifo("myfifo", 0777);
    int fd = open("myfifo", O_RDONLY);
    printf("Reader has started...\n");
    while(1){
        char buffer[sizeBuffer];
        read(fd, buffer, sizeBuffer);
        printf("Received (Converted letters and numbers): %s\n", buffer);
            if(!strcmp(buffer, "exit")){
            break;
        }
        else{
            //CONVERSION NUMBERS
            for(int i=0;i<sizeBuffer;i++){
                    if ('0' >= buffer[i] && '9' <= buffer[i])  buffer[i] = '0'; 
            } 
            write(fd, buffer, sizeBuffer);

        }
    }
    close(fd);
}