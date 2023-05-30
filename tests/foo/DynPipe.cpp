 //This program executes a dynamic pipes.

 #include <fcntl.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string>
 #include <unistd.h>
 #include <sys/stat.h>
 #include <sys/types.h>
 #include <sys/wait.h>

 using namespace std;

 int main(int argc, char **argv) {
 
 //Return an error if less than two args
 
 if(argc <= 2) {
 perror("At least two arguments are required ");
 exit(1);
 }

 //Return an error if more than five args
 else if(argc >= 7) {
 perror("No more than five arguments are allowed ");
 exit(1);
 }

 int numArgs = argc - 1;
 int childpids[numArgs];
 int numPipes = numArgs - 1;
 string arg;

 //Create the required number of pipes.
 int pipes[numPipes][2];
 for(int i = 0; i < numPipes; i++)
 pipe(&pipes[i][0]);

 for(int i = 0; i < numArgs; i++) {
 //Fork to create a child.
 if((childpids[i] = fork()) == -1) {
 perror("Could not create child process ");
 exit(1);
 }

 if(childpids[i] == 0) {
 //Store the instruction
 arg = argv[i+1];

 //stdout with write end of previous pipe 
 if(i < numArgs - 1) 
 dup2(pipes[i][1], 1);

 //stdin with read end of next pipe
 if(i > 0)
 dup2(pipes[i - 1][0], 0);

 //Close all pipes.
 for(int j = 0; j < numPipes; j++) 
 {
 close(pipes[j][0]);
 close(pipes[j][1]);
 }

 //Execute the command.
 execlp("/bin/sh", "/bin/sh", "-c", arg.c_str(), NULL);

 //Exit if not final command.
 if(i < numArgs) 
 exit(0);
 }
 }

 //Close all pipes.
 for(int j = 0; j < numPipes; j++)
 {
 close(pipes[j][0]);
 close(pipes[j][1]);
 }

 return(0);
 }
