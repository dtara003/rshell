#ifndef COMMAND_H
#define COMMAND_H

#include "Shell.h"

using namespace std;

class Command : public Shell {
	private:

		//execvp takes two arguments: cmd[0](which is the
		//executable), and it takes in the whole char* array
		//for the rest of the arguments(i.e. cmd = ls -a 
		//then cmd[0] = ls)
		char** cmd;

	public:
		//FIXME Can you initialize char** together like that?
		Command (char** c) : cmd(c) {}

		//function returns true if command executed
		bool execute() {
			//fork() returns two pids: 0 for child, pid for parent
			pid_t pid = fork();

			//fork() returns a pid_t that determines which
			//process it is(i.e. child or parent)

			//if pid < 0 then the fork failed
			if (pid < 0) {
				perror("Fork failed");
			}
			
			//child will return a pid of 0
			//we want to run execvp in the child because
			//execvp automatically terminates all processes
			//after it executes
			if (pid == 0) {

				//error check execvp
				//execvp returns -1 if the command did not execute
				//we also want this whole function(execute)
				//to return false if the command in execvp did not execute
				if (execvp(cmd[0],cmd) == -1) {

					//print out error message
					perror("Error");
					
					//we can put any number(I think) and when this child process
					//terminates(through exit) it should return the number
					//argument(in this case 50)
					exit(50);
				}

				//if the command executed then we exit with 0
				exit(0);
			}

			//parent will have a pid > 0
			if (pid > 0) {

				//now we want to check what value the child returned(either a 50
				//or 0). A 50 indicates that the execvp failed to execute and
				//therefore we want to return false in this execute function
				
				int status = 0;
				
				//wait for child to terminate(prevent "zombie" processes
				wait(&status);

				//get the value that the child returned(0 or 50)
				int childReturnVal = WEXITSTATUS(status);
				if (childReturnVal == 50) {
					//this means the command did not execute so return false
					return false;
				}
				else
					return true; //command executed therefore return true
			}
		}
};
#endif






















