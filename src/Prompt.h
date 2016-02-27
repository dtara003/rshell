#ifndef PROMPT_H
#define PROMPT_H

#include "Shell.h"
#include "Command.h"
#include "Connector.h"

#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

class Prompt {
    private:
        string input;
        //Shell* ptr;
        vector <string> commStrs;
        vector <int> connVals;
      
    public:
        Shell* ptr;

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
            // cout << "w/o comments: " << input << endl;
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
        
        // call once parsing and tree building is done
        void run() {
            bool isNull;
            status check;
            if (this->ptr == NULL) {
                isNull = true;
            } else {
                isNull = false;
            }

            if (isNull) {
                return;
            } else {
                check = this->ptr->execute();
            }

            if (check == EXIT) {
                ptr->freeMem();
                ptr = NULL;
                isNull = true;

                exit(0);
            } else {
                ptr->freeMem();
                ptr = NULL;
            }
            return;
            /*if (this->ptr != NULL) {
                status check = this->ptr->execute();
                
                if (check == EXIT) {
                    ptr->freeMem();
                    ptr = NULL;
                    //delete ptr;
                    exit(0);
                }
                // cout << "you made it here" << endl;
                //delete ptr;
                ptr->freeMem();
            }

            ptr = NULL;*/
        };

        string cut(string str) {
            trim(str);

            return str;
        }

        string cut(string str, int start, int end) {
            return str.substr(start, end);
        }

        void parse() {
            // the followinf two while loops are probably not a viable method of
            // interpreting command line inputs
            // weak method error-catching
            // just gets rid of starting and ending errors altogether
                
            // remove extra spaces from start and end of input string
            input = cut(input);

            // remove any leading or trailing connectors
            while ((input.at(input.size() - 1) == '&') || (input.at(input.size()
            - 1) == '|') || (input.at(input.size() - 1) == ';')) {
                input = cut(input, 0, input.size() - 1);               
                input = cut(input);
            }

            // cout << "6" << endl;
		    
            while ( (input.at(0) == '&') || (input.at(0) == '|') || (input.at(0)
            == ';') ) {
                input = cut(input, 1, input.size());
                input = cut(input);
            }
		    
            input = cut(input);
		    
            // check
            // cout << "trimmed: " << input << endl;
		    
            // loop through entire input string and parse commands and
            // connectors off the left end one at a time
            // each time a command and connector are pushed into their
            // appropriate vectors, the string is cut shorter so the next
            // command and subsequent connector can be separated when the loop
            // iterates again
            // bool quotes will track whether a command's arguments are within a
            // set of quotation marks
            // if so, then any connector deliminators present inside will be
            // ignored so that the argument of a command such as echo does not
            // get parsed into multiple commands when it is simply meant to be
            // echoed to the console
            bool quotes = false;
            unsigned int i = 0;
            while (i < input.size()) {
                // toggles to check for quotes
                if (input.at(i) =='\"') {
                    if (!quotes) {
                        quotes = true;
                    } else {
                        quotes = false;
                    }
                }
                
                // if not waiting for closing quotations then parse
                if (!quotes) {
                    if (ptr == NULL) {
                        // first command pointer
                        if (input.at(i) == ';') {
                            ptr = new Command(cut(cut(input, 0, i)));
                            commStrs.push_back(cut(cut(input, 0, i)));
                            input = cut(input, i + 1, input.size() - 1);
                            i = 0;
                            connVals.push_back(1);
                        } else if ((input.at(i) == '&') && (input.at(i + 1) ==
                        '&')) {
                            ptr = new Command(cut(cut(input, 0, i)));
                            commStrs.push_back(cut(cut(input, 0, i)));
                            input = cut(input, i + 2, input.size() - 1);
                            i = 0;
                            connVals.push_back(2);
                        } else if ((input.at(i) == '|') && (input.at(i + 1) ==
                        '|')) {
                            ptr = new Command(cut(cut(input, 0, i)));
                            commStrs.push_back(cut(cut(input, 0, i)));
                            input = cut(input, i + 2, input.size() - 1);
                            i = 0;
                            connVals.push_back(3);
                        }
                    } else {
                        // if this is n't the first object to be created then
                        // make the new object and then incrporate it into the
                        // tree
                        if (input.at(i) == ';') {
                            if (connVals.at(connVals.size() - 1) == 1) {
                                Shell* a = new Command(cut(cut(input, 0, i)));
                                commStrs.push_back(cut(cut(input, 0, i)));
                                Shell* b = new Semi(ptr, a);
                                ptr = b;
                                input = cut(input, i + 1, input.size() - 1);
                                i = 0;
                                connVals.push_back(1);
                            } else if (connVals.at(connVals.size() - 1) == 2) {
                                Shell* a = new Command(cut(cut(input, 0, i)));
                                commStrs.push_back(cut(cut(input, 0, i)));
                                Shell* b = new And(ptr, a);
                                ptr = b;
                                input = cut(input, i + 1, input.size() - 1);
                                i = 0;
                                connVals.push_back(1);

                            } else if (connVals.at(connVals.size() - 1) == 3) {
                                Shell* a = new Command(cut(cut(input, 0, i)));
                                commStrs.push_back(cut(cut(input, 0, i)));
                                Shell* b = new Or(ptr, a);
                                ptr = b;
                                input = cut(input, i + 1, input.size() - 1);
                                i = 0;
                                connVals.push_back(1); 
                            }
                        } else if ((input.at(i) == '&') && (input.at(i + 1) ==
                        '&')) {
                            if (connVals.at(connVals.size() - 1) == 1) {
                                Shell* a = new Command(cut(cut(input, 0, i)));
                                commStrs.push_back(cut(cut(input, 0, i)));
                                Shell* b = new Semi(ptr, a);
                                ptr = b;
                                input = cut(input, i + 2, input.size() - 1);
                                i = 0;
                                connVals.push_back(2);  
                            } else if (connVals.at(connVals.size() - 1) == 2) {
                                Shell* a = new Command(cut(cut(input, 0, i)));
                                commStrs.push_back(cut(cut(input, 0, i)));
                                Shell* b = new And(ptr, a);
                                ptr = b;
                                input = cut(input, i + 2, input.size() - 1);
                                i = 0;
                                connVals.push_back(2);
                            } else if (connVals.at(connVals.size() - 1) == 3) {
                                Shell* a = new Command(cut(cut(input, 0, i)));
                                commStrs.push_back(cut(cut(input, 0, i)));
                                Shell* b = new Or(ptr, a);
                                ptr = b;
                                input = cut(input, i + 2, input.size() - 1);
                                i = 0;
                                connVals.push_back(2);
                            }
                        } else if ((input.at(i) == '|') && (input.at(i + 1) ==
                        '|')) {
                            if (connVals.at(connVals.size() - 1) == 1) { 
                                Shell* a = new Command(cut(cut(input, 0, i)));
                                commStrs.push_back(cut(cut(input, 0, i)));
                                Shell* b = new Semi(ptr, a);
                                ptr = b;
                                input = cut(input, i + 2, input.size() - 1);
                                i = 0;
                                connVals.push_back(3);
                            } else if (connVals.at(connVals.size() - 1) == 2) {
                                Shell* a = new Command(cut(cut(input, 0, i)));
                                commStrs.push_back(cut(cut(input, 0, i)));
                                Shell* b = new And(ptr, a);
                                ptr = b;
                                input = cut(input, i + 2, input.size() - 1);
                                i = 0;
                                connVals.push_back(3);
                            } else if (connVals.at(connVals.size() - 1) == 3) {
                                Shell* a = new Command(cut(cut(input, 0, i)));
                                commStrs.push_back(cut(cut(input, 0, i)));
                                Shell* b = new Or(ptr, a);
                                ptr = b;
                                input = cut(input, i + 2, input.size() - 1);
                                i = 0;
                                connVals.push_back(3);
                            }
                        }
                    }
                }
		        
                i++;
            }
            
            // account for final command
            commStrs.push_back(cut(input));
            
            if (ptr == NULL) {
                ptr = new Command(cut(input));
                input = "";
            } else {
                Shell* a = new Command(cut(input));
                
                if (connVals.at(connVals.size() - 1) == 1) {
                    Shell* b = new Semi(ptr, a);
                    ptr = b;
                    input = "";
                } else if (connVals.at(connVals.size() - 1) == 2) {
                    Shell* b = new And(ptr, a);
                    ptr = b;
                    input = "";
                } else if (connVals.at(connVals.size() - 1) == 3) {
                    Shell* b = new Or(ptr, a);
                    ptr = b;
                    input = "";
                }
            }
            
            /* check
            for (unsigned int i = 0; i < connVals.size(); ++i) {
                cout << connVals.at(i) << " ";
            }

            cout << endl;
		    
            for (unsigned int i = 0; i < commStrs.size(); ++i) {
                cout << commStrs.at(i) << endl;
            }

            cout << input << endl;*/
        };
		
        // DEFINED WITHIN SHELL
        /*void freeMem() {
            // DO NOT FORGET
            // FREE ANY MEMORY THAT WAS ALLOCATED
        };*/
};

#endif
