#ifndef PROMPT_H
#define PROMPT_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string/trim.hpp>
//#include "Shell.h"

using namespace std;
using namespace boost;

class Prompt {
    private:
        //Shell* 
        //char input[1000];
        string input;
        //Shell* ptr;
        //vector <Shell*> roots;
        vector <int> connVals;
        
        
    public:
		//Prompt() : input(""), ptr(NULL) {};
		Prompt() {};
		//Prompt(string s, vector <int> v) : input(s), connVals(v) {};
		
		//Shell(string )
		
        void promptUser() {
            /*string user = getlogin();
            char hostname[100];
            gethostname(hostname, 100)''
            
            // prompt for input
            cout << "[" << user << "@" << hostname << "]$ "; */
            cout << "$ ";                                                       // delete this line later
            
            getline(cin, input);
            
            // exit if prompted to do so
            if (input == "exit") {
                exit(0);
            }
            
            rmComments(input);
            
            // check
            cout << input << endl;
            
            parse();
            //complete();
        };
        
        // remove comments
		void rmComments(string &input) {
		    for (unsigned int i = 0; i < input.size(); ++i) {
                if (input.at(i) == '#') {
                    input = input.substr(0, i);
                    i = input.size();
                }
            }
		};
		
		void parse() {
		    // lines 70 - 83 probably not a viable method of interpreting
		    // command line inputs
                // weak method error-catching
                // just gets rid of starting and ending errors altogether
                
		    // remove extra spaces from start and end of input string
		    trim(input);
		    
		    // remove any leading or trailing connectors
		    while ( (input.at(input.size() - 1) == '&') || (input.at(input.size()
		    - 1) == '|') || (input.at(input.size() - 1) == ';') ) {
		        input = input.substr(0, input.size() - 2);
		    }
		    
		    while ( (input.at(0) == '&') || (input.at(0) == '|') || (input.at(0)
		    == ';') ) {
		        input = input.substr(1, input.size() - 1);
		    }
		    
		    trim(input);
		    
		    // check
		    cout << input << endl << "that was trimmed" << endl;
		    
		    /*Shell* a;
		    Shell* b;
		    Shell* c;*/
		    
		    // 1 if semicolon, 2 if and, 3 if or
		    // vector <int> connVals;
		    unsigned int i = 0;
		    bool quotes = false;
		    //int connType = 0;
		    while (i < input.size()) {
		        // toggles to check for quotes used for commands like echo
		        if (input.at(i) =='\"') {
		            if (!quotes) {
		                quotes = true;
		            } else {
		                quotes = false;
		            }
		        }
		        
		        if (!quotes) {
    		        if (input.at(i) == ';') {
    		            connVals.push_back(1);
    		            
    		            if (ptr == NULL) {
    		            	// create Command object of everything up until that
    		            	// semicolon
    		            	//ptr = new Command(trim(input.substr(0, i)));
    		            	
    		            	input = input.substr(i + 1, input.size() - 1);
    		            	i = 0;
    		            } else {
    		            	
    		            }
    		            
                    } else if (input.at(i) == '&' && input.at(i + 1) == '&') {
    		            connVals.push_back(2);
    		            
    		            if (ptr == NULL) {
    		            	// create Command object of everything up until the
    		            	// and symbols
    		            	//ptr = new Command(trim(input.substr(0, i)));
    		            	
    		            	input = input.substr(i + 2, input.size() - 1);
    		            	i = 0;
    		            } else {
    		            	
    		            }
    		            
    		        } else if (input.at(i) == '|' && input.at(i + 1) == '|') {
    		            connVals.push_back(3);
    		            
    		            if (ptr == NULL) {
    		            	// create Command object of everything up until the
    		            	// or symbols
    		            	//ptr = new Command(trim(input.substr(0, i)));
    		            	
    		            	input = input.substr(i + 2, input.size() - 1);
    		            	i = 0;
    		            } else {
    		            	
    		            }
    		        }
		        }
		        
		        i++;
		    }
		    
		    for (unsigned int i = 0; i < connVals.size(); ++i) {
		    	cout << connVals.at(i) << " ";
		    }
		    
		    cout << endl << input << endl;
		};
		
		void freeMem() {
		    // DO NOT FORGET
		    // FREE ANY MEMORY THAT WAS ALLOCATED
		};
    
};

#endif
