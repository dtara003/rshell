#ifndef PROMPT_H
#define PROMPT_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string/trim.hpp>
#include "Shell.h"

using namespace std;
using namespace boost;

class Prompt {
    private:
        string input;
        Shell* ptr;
        //vector <Shell*> roots;
        vector <int> connVals;
      
    public:
        Prompt() {
            ptr = NULL;
        };

        Prompt(string s, vector <int> v) : input(s), connVals(v) {};
		
        //Shell(string ) {}

        void promptUser() {
            string user = getlogin();
            char hostname[100];
            gethostname(hostname, 100);
            
            // prompt for input
            cout << "[" << user << "@" << hostname << "]$ ";          
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
               
            trim(input);
        }
		    
        while ( (input.at(0) == '&') || (input.at(0) == '|') || (input.at(0)
        == ';') ) {
            input = input.substr(1, input.size() - 1);

            trim(input);
        }
		    
        //trim(input);
		    
        // check
        // cout << input << endl << "trimmed" << endl;
		   		    
        // 1 if semicolon, 2 if and, 3 if or
        unsigned int i = 0;
        bool quotes = false;
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
                    // connVals.push_back(1);
    		            
                    if (ptr == NULL) {
                        // create Command object of everything up until that
                        // semicolon
                        ptr = new Command(trim(input.substr(0, i)));
                        input = input.substr(i + 1, input.size() - 1);
                        i = 0;
                        connVals.push_back(1);
                    } else {
                        if (connVals.at(connVals.size() - 1) == 1) {
                            // need new pointer variable to hold new connector
                            // object
                            Shell* a = new Command(trim(input.substr(0, i)));
                            input = input.substr(i + 1, input.size() - 1);
                            Shell* b = new Semi(ptr, a);
                                
                            // Shell* pointer now points to semicolon
                            ptr = b;
                            i = 0;
                            connVals.push_back(1);
                        } else if (connVals.at(connVals.size() - 1) == 2) {
                            Shell* a = new Command(trim(input.substr(0, i)));
                            input = input.substr(i + 1, input.size() - 1);
                            Shell* b = new And(ptr, a);

                            ptr = b;
                            i = 0;
                            connVals.push_back(1);
                        } else if (connVals.at(connVals.size() - 1) == 3) {
                            Shell* a = new Command(trim(input.substr(0, i)));
                            input = input.substr(i + 1, input.size() - 1);
                            Shell* b = new Or(ptr, a);

                            ptr = b;
                            i = 0;
                            connVals.push_back(1);
                        }
                    }
                } else if (input.at(i) == '&' && input.at(i + 1) == '&') {
                    // connVals.push_back(2);
    		            
                    if (ptr == NULL) {
                        // create Command object of everything up until the
                        // and symbols
                        ptr = new Command(trim(input.substr(0, i)));
                        input = input.substr(i + 2, input.size() - 1);
                        i = 0;
                        connVals.push_back(2);
                    } else {
                        if (connVals.at(connVals.size() - 1) == 1) {
                            Shell* a = new Command(trim(input.substr(0, i)));
                            input = input.substr(i + 2, input.size() - 1);
                            Shell* b = new Semi(ptr, a);

                            ptr = b;
                            i = 0;
                            connVals.push_back(3);
                        } else if (connVals.at(connVals.size() - 1) == 2) {
                            Shell* a = new Command(trim(input.substr(0, i)));
                            input = input.substr(i + 2, input.size() - 1);
                            Shell* b = new And(ptr, a);

                            ptr = b;
                            i = 0;
                            connVals.push_back(2);
                        } else if (connVals.at(connVals.size() - 1) == 3) {
                            Shell* a = new Command(trim(input.substr(0, i)));
                            input = input.substr(i + 2, input.size() - 1);
                            Shell* b = new Or(ptr, a);

                            ptr = b;
                            i = 0;
                            connVals.push_back(2);
                        }
                    }
                } else if (input.at(i) == '|' && input.at(i + 1) == '|') {
                    // connVals.push_back(3);
    		            
                    if (ptr == NULL) {
                        // create Command object of everything up until the
                        // or symbolptr = new Command(trim(input.substr(0, i)));
                        ptr = new Command(trim(input.substr(0, i)));
                        input = input.substr(i + 2, input.size() - 1);
                        i = 0;
                        connVals.push_back(3);
                    } else {
                        if (connVals.at(connVals.size() - 1) == 1) {
                            Shell* a = new Command(trim(input.substr(0, i)));
                            input = input.substr(i + 2, input.size() - 1);
                            Shell* b = new Semi(ptr, a);

                            ptr = b;
                            i = 0;
                            connVals.push_back(3);
                        } else if (connVals.at(connVals.size() - 1) == 2) {
                            Shell* a = new Command(trim(input.substr(0, i)));
                            input = input.substr(i + 2, input.size() - 1);
                            Shell* b = new And(ptr, a);

                            ptr = b;
                            i = 0;
                            connVals.push_back(3);
                        } else if (connVals.at(connVals.size() - 1) == 3) {
                            Shell* a = new Command(trim(input.substr(0, i)));
                            input = input.substr(i + 2, input.size() - 1);
                            Shell* b = new Or(ptr, a);

                            ptr = b;
                            i = 0;
                            connVals.push_back(3);
                        }
                    }
                }
            }
		        
            i++;
        }

        // account for final command
        trim(input);
            


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
