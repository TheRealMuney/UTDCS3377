// This program executes "ls -ltr | grep 3376 | wc -l" using two pipes and two child processes.

 #include <fcntl.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <sys/stat.h>
 #include <sys/types.h>
 #include <sys/wait.h>


 int main(int argc, char **argv){
 int childpid1;
 int childpid2;

 char *ls_args[] = {"ls", "-ltr", NULL};
 char *grep_args[] = {"grep", "3376", NULL};
 char *wc_args[] = {"wc", "-l", NULL};

 //"ls" process to "grep" process.
 int pipe1[2];
 pipe(pipe1);

 // "grep" process to "wc" process.
 int pipe2[2];
 pipe(pipe2);

 //Fork creates first child
 if((childpid1 = fork()) == -1) {
 perror("Error: child process 1.");
 exit(1);
 }

 if(childpid1 == 0) { 
 //stdout to write part of 1st pipe.
 dup2(pipe1[1], 1);

 //Close
 close(pipe1[0]);
 close(pipe2[0]);

 close(pipe1[1]);
 close(pipe2[1]);

 execvp(*ls_args, ls_args);

 exit(0);
 }

 else {

//Fork creates second child

 if((childpid2 = fork()) == -1) {
 perror("Error: child process 2");
 exit(1);
 }

 if(childpid2 == 0) {  //stdin to read part of 1st pipe.
 //stdin to read part of 1st pipe.
dup2(pipe1[0], 0);

 // stdout to write part of 2nd pipe.
 dup2(pipe2[1], 1);

 //Close
 close(pipe1[0]);
 close(pipe2[0]);
 
 close(pipe2[1]);
 close(pipe1[1]);


execvp(*grep_args, grep_args);

exit(0);
}

}

// stdin to read part of 2nd pipe.
dup2(pipe2[0], 0);

//Close
close(pipe1[0]);
close(pipe2[1]);

close(pipe2[0]);
close(pipe1[1]);

execvp(*wc_args, wc_args);

return(0);
}
