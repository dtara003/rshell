#ifndef COMMAND_H
#define COMMAND_H

#include "Shell.h"
#include <vector>
#include <sstream>
#include <string>
#include <errno.h>
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
                // check
				//cout << cmd << endl;
                return EXIT;
			}
		    
            // check
            // outputs cmd and its size to find discrepencies
            //cout << cmd << "CHECK " << cmd.size() << endl;

			//get rid of quotation marks
			for (unsigned int i = 0; i < cmd.size(); i++) {
				if (cmd.at(i) == '\"') {
					cmd.erase(i, 1);
				}
			}

            // check
            //cout << cmd << "CHECK " << cmd.size() << endl;

			//convert string to char []

			unsigned int sz = 0; //will keep track of the size of the char**
			string in = ""; //takes input from stringstream
			vector<string> v; //stores the "words"
			
			stringstream str(cmd); //get rid of white spaces
			while (str >> in) {
				v.push_back(in);
			}
			sz = v.size(); //set the char** array size(including NULL)
		
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
				if (execvp(arr[0],arr) < 0) {

					//print out error message
					perror("Error");
					//errno should be set based on the error
					exit(errno);
				}

				//if the command executed then we exit with 0(errno should be
				//set to 0)
				exit(errno);
			}

			//parent will have a pid > 0

			//now we want to check what value the child returned(either a 50
			//or 0). A 50 indicates that
				
			int status = 0;
				
			//wait for child to terminate(prevent "zombie" processes)
			wait(&status);
            
            //WIFEXITED() macro checks if the child process terminated properly
            //or not
			if (WEXITSTATUS(status) > 0) {
				//this means the command did not execute so return false
				return FAILED;
			}
			else { 
				return EXECUTED; //command executed therefore return true
            }
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






















