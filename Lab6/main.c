#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (){
    const int diff = 32;
    const int sizeBuffer = 1024;
    int handles [2] ;   // [0] - read, [1] - write
    int handles2 [2] ;   // [0] - read, [1] - write

    if(pipe (handles)<0){
        return 0;
    }
    if(pipe (handles2)<0){
        return 0;
    }

    int pid = fork();
                    
    //PARENT->WRITE
    if (pid>0){
        close(handles[0]);
        while(1){
            char buffer[sizeBuffer];
            // printf("Input:");
            scanf("%s", buffer);
            printf("Client send message: %s\n", buffer);
            write(handles[1], buffer, sizeBuffer);
            if(!strcmp(buffer, "exit")){
                break;
            }
        }
        wait(NULL);
    }
    
    else {
        close(handles[1]);
        int pid = fork();
        //CHILD_1->READ(from Parent)->CONVERT->WRTIE(to CHILD_2)
        if(pid>0){
            close(handles2[0]);
            while(1){
                char buffer[sizeBuffer];
                read(handles[0], buffer, sizeBuffer);
                // printf("Received (original): %s\n", buffer);
                if(!strcmp(buffer, "exit")){
                    write(handles2[1], buffer, sizeBuffer);
                    break;
                }
                else{
                    //CONVERSION LETTERS
                    for(int i=0;i<sizeBuffer;i++){
                        if ('a' <= buffer[i] && 'z' >= buffer[i]) buffer[i] = buffer[i] - diff;
                        
                        else if ('A' <= buffer[i] && 'Z' >= buffer[i])  buffer[i] = buffer[i] + diff;
                    } 
                    write(handles2[1], buffer, sizeBuffer);       
                }
            }
            wait(NULL);
        }
        else{
            close(handles2[1]);
            // mkfifo("myFifo", 0777);
            int fd = open("myfifo", O_WRONLY);
            while(1){
                char buffer[sizeBuffer];
                read(handles2[0], buffer, sizeBuffer);
                // printf("Received (Converted letters): %s\n", buffer);
                 if(!strcmp(buffer, "exit")){
                    write(fd, buffer, sizeBuffer);
                    break;
                }
                else{
                    //CONVERSION NUMBERS
                    for(int i=0;i<sizeBuffer;i++){
                         if ('0' <= buffer[i] && '9' >= buffer[i])  buffer[i] = '0'; 
                    } 
                    write(fd, buffer, sizeBuffer);
                }
            }
            close(fd);
        }
    }

}