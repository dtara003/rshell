#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "Shell.h"
#include "Prompt.h"

#include <iostream>

using namespace std;

//Composite class for Connectors(&& and || and ;)
//following same format as Lab3
class Connector : public Shell {
    public:
        Shell* left;
        Shell* right;

        // constructor
        Connector() {};
        
        //destructor
		~Connector() {};

        virtual status execute() = 0;
        
        // recursive destruction of tree whenever all commands are done
        // executing or the exit command is executed
        // must call this function before exiting for exit command or there
        // will be a memory leak because the tree will be left behind
        
        // references

        // daniweb.com//programming/software-development/threads/121994-
            // recursive-binary-tree-header-file
        // cprogramming.com/tutorial/lesson18.html
        void freeMem() {
            left->freeMem();
            right->freeMem();

            delete left;
            delete right;
        };
};

class Semi : public Connector {
    public:
        // constructor
        //Semi() {};

        Semi(Shell* l, Shell* r) {
            left = l;
            right = r;
        };

        ~Semi() {};

        status execute() {
            // semicolon works so that regardless of the left
            // side of the connector, the right side will always
            // pass through the execute function

			//execute the left child and determine its status
            status check = left->execute();

            if (check == EXECUTED) {
                return right->execute();
            } else if (check == EXIT) {
				return EXIT;
			}
			
            return right->execute();
        };
};

class And : public Connector {
	public:
		//And() {};
		
		// can't use initializaion lists when setting pointers for
        // some reason -- a limitation of inheritance?
        // probably because left and right aren't directly members of
        // the classes that inherit from the Connector class
		And(Shell* l, Shell* r) { 
            left = l;
            right = r;
        };

        ~And() {};

		//execute function that will perform as &&
		status execute() {
			
			//check the status of the left->execute()
			status check = left->execute();

		    if (check == EXECUTED) {
                return right->execute();
            } else if (check == EXIT) {
				return EXIT;
			}
            
            return FAILED;
		};
};

class Or : public Connector {
	public:
        // constructors
        //Or() {};

		Or(Shell* l, Shell* r) {
            left = l;
            right = r;
        };

        ~Or() {};

		status execute() {
            //check the what left->execute() returns
            status check = left->execute();

            if (check == EXECUTED) {
                return EXECUTED;
            } else if (check == EXIT) {
				return EXIT;
			}
		    
            return right->execute();
        };
};

#endif
