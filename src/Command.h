#ifndef COMMAND_H
#define COMMAND_H

#include "Shell.h"
#include <vector>
#include <sstream>
#include <string>

using namespace std;

class Command : public Shell {
	private:
		//execvp takes two arguments: cmd[0](which is the
		//executable), and it takes in the whole char* array
		//for the rest of the arguments(i.e. cmd = ls -a 
		//then cmd[0] = ls)

		string cmd;

	public:
		//a constructor that takes in string arguments and converts to char**
		Command(string c) :cmd(c) {};
		~Command() {};	
		//function returns true if command executed
		status execute() {
			
			//check if the string is "exit"
			//we need a special case for this because execvp does not
			//take exit as a command
			
			//if the string is exit then we want to return EXIT and go back to
			//our prompt class and delete the tree and call exit(0)
			if (cmd == "exit") {
				return EXIT;
			}
			
			//get rid of quotation marks
			for (int i = 0; i < cmd.size(); i++) {
				if (cmd.at(i) == '\"') {
					cmd.erase(i, 1);
				}
			}
			//convert string to char []

			unsigned int sz = 0; //will keep track of the size of the char**
			string in = ""; //takes input from stringstream
			vector<string> v; //stores the "words"
			
			stringstream str(cmd); //get rid of white spaces
			while (str >> in) {
				v.push_back(in);
			}
			sz = v.size() + 1; //set the char** array size(including NULL)
		
			char* arr[1024]; //set the size of the char* array
			
			//populate the array
			for (unsigned int i = 0; i < v.size(); i++) {
				arr[i] = (char* ) v.at(i).c_str();
			}
			
			arr[sz] = NULL; //set the null terminating character

			//fork() returns two pids: 0 for child, pid for parent
			pid_t pid = fork();
			
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
				//to return false(FAILED)
				//if the command in execvp did not execute
				if (execvp(arr[0],arr) == -1) {

					//print out error message
					perror("Error");
					
					//we can put any number(I think) and when this child process
					//terminates(through exit) it should return a number
					//and we can use WEXITSTATUS to get the number 50
					exit(50);
				}

				//if the command executed then we exit with 0
				exit(0);
			}

			//parent will have a pid > 0

			//now we want to check what value the child returned(either a 50
			//or 0). A 50 indicates that the execvp failed to execute and
			//therefore we want to return false in this execute function
				
			int status = 0;
				
			//wait for child to terminate(prevent "zombie" processes)
			wait(&status);

			//get the value that the child returned(0 or 50)
			int childReturnVal = WEXITSTATUS(status);
			if (childReturnVal == 50) {
				//this means the command did not execute so return false
				return FAILED;
			}
			else 
				return EXECUTED; //command executed therefore return true
		};

		void freeMem() {
            // this will work like a base case
            // this is how we know we have recursed all the way to the bottom
            // so we know that the parent node from this point and one will
            // be a class inherited from the Connector class
            // from there, the connector's left and right children will be
            // deleted and this will occur all the way back up to the root
			return;
		};
};
#endif






















