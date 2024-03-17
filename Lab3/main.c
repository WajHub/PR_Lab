#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include <unistd.h>

int main(int argc, char*argv[]){
	if(argc!=2){
		printf("Error");
		return 1;
	}
	int depth = atoi(argv[1]);

	pid_t pid, pid2;

	while(depth>0){
		pid = fork();
		// Child
		if(pid == 0){
			depth-=1;
			if(depth>1 && depth%2==0){
				pid2=fork();
				if(pid2==0){
					waitpid(pid,NULL, 0);
					return 0;
				}
			}
			
		}
		//Parent
		else {
			if(depth>1 && depth%2==1) pid2=fork();
			waitpid(pid, NULL, 0);
			return 0; 
		}
	}
	execlp("ps", "-u hubert", "-l", "--forest", NULL);
	return 0;
}
