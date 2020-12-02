#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	pid_t child_pid, wpid;
	int ret, status;

	// int  stdout_bk; //is fd for stdout backup
	// fprintf(stderr,"this is before redirection\n");
	// stdout_bk = dup(fileno(stdout));
	// int pipefd[2];
	// pipe2(pipefd, 0); // O_NONBLOCK);
	// // What used to be stdout will now go to the pipe.
	// dup2(pipefd[1], fileno(stdout));



	for (int i = 0; i < 5; i++) {
		child_pid = fork();
		if(child_pid == -1) { 
			perror("fork");
			exit(EXIT_FAILURE);
		} else if(child_pid == 0) { //Child
			printf("HOHO %d ", i);
			break;
			// ret = execl("/bin/file", "file", "--mime-type", "-b","a.png", (char *)NULL);
			// if (ret == -1) {
			// 	perror("execl");
			// 	exit(EXIT_FAILURE);
			// }
		} else {  // Parent
			do {
		        wpid = waitpid(child_pid, &status, WUNTRACED);
			    if (wpid == -1) {
		            perror("waitpid");
		            exit(EXIT_FAILURE);
			    }
				// fflush(stdout);//flushall();
				// write(pipefd[1], "good-bye", 9); // null-terminated string!
				// close(pipefd[1]);

				// dup2(stdout_bk, fileno(stdout));//restore
				// printf("this is now\n");

				// char buf[101];
				// read(pipefd[0], buf, 100); 
				// fprintf(stderr, "got this from the pipe >>>%s<<<\n", buf);
			    fprintf(stderr, "child %d exited, status=%d\n", i, WEXITSTATUS(status));
		    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}	
	}
}
