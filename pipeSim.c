//Operating Systems
//Aysun Ogut, 2021

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	
	
	printf("I'm SHELL process, with PID:%d - Main command is: man df | grep -A 1 -we \"-h\" \n", (int)getpid());
	
	//pipe is created 
	int fd[2];	
	if(pipe(fd) == -1){ //if fails
		fprintf(stderr, "Could not create pipe.\n");
		exit(1);
	}
       
	//process 1 creation for MAN command
	int id = fork();
  
	if(id < 0){
        	//could not create fork
        	fprintf(stderr, "Fork failed.\n");
        	exit(1);
	}	
        else if (id == 0){
        	//child process - man command
        	printf("I'm MAN process, with PID:%d - My command is: man df\n", (int)getpid());
        	
        	dup2(fd[1], STDOUT_FILENO);
        	close(fd[0]);
        	close(fd[1]);
        	       	
        	char *args[3];
        	args[0] = strdup("man");
        	args[1] = strdup("df");
        	args[2] = NULL;
        	execvp(args[0], args);  	
        }
        else{
        	//parent process - man command 
               close(fd[1]);
        	wait(NULL);   	      
        }
        
        //process 2 creation for GREP command
        int id_2 = fork();
        if(id_2 < 0){
        	//could not create fork
        	fprintf(stderr, "Fork failed.\n");
        	exit(1);
        }
        else if (id_2 == 0){
        	//child process - grep command
        	printf("I'm GREP process, with PID:%d - My command is: grep -A 1 -we \"-h\" \n", (int)getpid());
        	
        	dup2(fd[0], STDIN_FILENO);
        	close(fd[0]);
        	close(fd[1]);
        	
        	close(STDOUT_FILENO);
        	open("./output.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
        	
        	char *args2[6];
        	args2[0] = strdup("grep");
        	args2[1] = strdup("-A");
        	args2[2] = strdup("1");
        	args2[3] = strdup("-we");
        	args2[4] = strdup("-h");
        	args2[5] = NULL;
        	execvp(args2[0], args2);    //execlp can be used which is shorter	
        }
        else{
        	//parent process - grep command
		close(fd[1]);	
        	wait(NULL);	
        }

	printf("I'm SHELL process, with PID:%d - execution is completed, you can find the results in output.txt\n", (int)getpid());

return 0;

}
