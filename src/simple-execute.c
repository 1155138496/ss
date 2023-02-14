#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>



#define WRITE_END 1     // pipe write end
#define READ_END 0      // pipe read end

/*	shell_execute - Function to execute the command with specified command content and command arguments. 
*	@args	String array with command and its arguments. 
*	@argc	Total number of strings including the command, its arguments and the NULL in the end
*/

	
	

char * command;
char ** option;

int pipe_position[10], pipecount;

void setcommand(char ** args, int index)
{
    int totalindex = index;  
    if (totalindex == 0) totalindex = -1;
    char * realcommand = malloc(sizeof(char) * 1025);
    strcpy(realcommand, args[totalindex+1]);
    int length = strlen(args[totalindex+1]);
    realcommand[length] = '\0';
    option = &args[totalindex+1];
    command = realcommand;
}

int pipe_count(char ** args, int argc)
{
    pipe_position[0] = 0;
    int pipecount = 0;
    for ( int i=0; i<argc; i++ )
    {
        if ( strcmp(args[i], "|") == 0 )
        { 
            pipecount++;
            pipe_position[pipecount] = i;
     args[i] = NULL;
        }
    }
    return pipecount;
}
int shell_execute(char ** args, int argc){


  int child_pid, wait_return, status;

	/* Execute built-in commands */
	/* exit */
	if(strcmp(args[0], "exit") == 0 ){
		return -1; 
	}
	/* TODO: cd */
	if(strcmp(args[0], "cd")== 0){
		if(chdir(args[1])<0){
			printf("cd: %s: No such file or error \n ", args[1]);
		}
		return 0;
	}
	
  /* Non-built-in commands. These commands should be executed in a child process so the parent process can continue to invoke other commands */	
	/* One command without pipe */
	if((child_pid = fork()) < 0){
		printf("fork() error \n");
	}
	else if(child_pid == 0 ){
		/* TODO: execute the command and check if the command is correctly executed */
		if(execvp(args[0],args)<0){
			printf("execute error\n");
			exit(-1);
		}
	}else{
		if ((wait_return=wait(&status))<0)
		printf("wait error\n");
		
	}
	
	
	/* TODOs: one pipe and two pipes */

 char ** argscopy = args;
 int child_pid, wait_return, status;
        int pipecount = pipe_count(argscopy, argc-1);
 int pipe_pos[10];
 for (int i=0;i<10;i++) {pipe_pos[i] = pipe_position[i];}
 int ** pipe_list = malloc(sizeof(int* ) * pipecount);
 for (int i=0;i<pipecount;i++) pipe_list[i] = malloc(sizeof(int)*2);
 if ( strcmp(args[0], "EXIT") == 0 )
  return -1; 
        if ( pipecount == 0){ 
         if( (child_pid = fork()) < 0 ){
      printf("fork() error \n");
         }else if (child_pid == 0 )
             {
                 if ( execvp(args[0], args) < 0)
                 { 
        printf("execvp() error \n");
   exit(-1);
        }   
  }
  else
  {
  if ( (wait_return = wait(&status) ) < 0 )
   printf("wait() error \n"); 
         } 
        }
        else 
        {
     int children_id;
     int status;
     for (int j=0;j<pipecount;j++){
      pipe(pipe_list[j]);
     }
     int i =pipecount;
            if ((children_id = fork()) < 0 )
            printf("fork() error\n");
     else if (children_id == 0){
            for (i=pipecount; i >-1; i--)
            {

               if ( (i!=0) && (children_id = fork()) < 0 ){
            printf("fork() error\n");
        }
        else if ((i!=0) &&  children_id == 0 )
               {
                       
               }
        else if (i!=0) 
        {
       for (int j=0;j < pipecount;j++){
             if (j!=i-1 && j != i) {
       close(pipe_list[j][0]);
    close(pipe_list[j][1]);
      }
         }
         
   if ( (wait_return = wait(&status) ) < 0 )
             printf("wait() error \n"); 
       if (i != pipecount){ 
        close(1);
        dup(pipe_list[i][1]);
        close(pipe_list[i][1]);
        close(pipe_list[i][0]);
       }
                  
                      close(0);
        dup(pipe_list[i-1][0]);
        close(pipe_list[i-1][0]);

        close(pipe_list[i-1][1]);
    setcommand(argscopy, pipe_pos[i]);    

                  if (execvp(command, option) < 0)
                  {
        printf("execvp() error \n");
        exit(-1);
                  }
        }
        else if (i==0){
                       for (int j=1;j<pipecount;j++){
       close(pipe_list[j][0]);
    close(pipe_list[j][1]);
         }

        close(1);
        dup(pipe_list[i][1]);
        close(pipe_list[i][1]);
        close(pipe_list[i][0]);
   
        setcommand(argscopy, pipe_pos[i]);    
                      if (execvp(command, option) < 0)
                      {
            printf("execvp() error \n");
            exit(-1);
     }

        }
        
        }
            }

     else {
            for (int j=0;j<pipecount;j++){
           close(pipe_list[j][0]);
           close(pipe_list[j][1]);
     }

        if ( (wait_return = wait(&status) ) < 0 )
               printf("wait() error \n"); 
  
     }
 }

 /* wait for child process to terminate */
	while((wait_return = wait(&status)) > 0);
			
 return 0;

}