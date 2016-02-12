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
        Shell* ptr;
        vector <string> commStrs;
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
            
            // cout << "1" << endl;

            // exit if prompted to do so
            if (input == "exit") {
                exit(0);
            }
            
            // cout << "2" << endl;

            rmComments(input);
            
            // check
            // cout << "3" << endl;
            // cout << "w/o comments: " << input << endl;
            
            parse();
            run();
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

        void run() {
            if (this->ptr != NULL) {
                status check = this->ptr->execute();
                
                if (check == EXIT) {
                    delete ptr;

                    exit(0);
                }

                delete ptr;
                // ptr = NULL;
            }

            ptr = NULL;
        };

        void parse() {
            // lines 72 - 85 probably not a viable method of interpreting
            // command line inputs
            // weak method error-catching
            // just gets rid of starting and ending errors altogether
                
            // remove extra spaces from start and end of input string
            
            // cout << "4" << endl;

            trim(input);

            // cout << "5" << endl;
		    
            // remove any leading or trailing connectors
            while ( (input.at(input.size() - 1) == '&') || (input.at(input.size()
            - 1) == '|') || (input.at(input.size() - 1) == ';') ) {
                input = input.substr(0, input.size() - 2);               
                trim(input);
            }

            // cout << "6" << endl;
		    
            while ( (input.at(0) == '&') || (input.at(0) == '|') || (input.at(0)
            == ';') ) {
                input = input.substr(1, input.size() - 1);
                trim(input);
            }

            // cout << "7" << endl;
		    
            //trim(input);
		    
            // check
            // cout << "trimmed: " << input << endl;
		   		    
            // 1 if semicolon, 2 if and, 3 if or
            unsigned int i = 0;
            bool quotes = false;

            // cout << "9" << endl;

            while (i < input.size()) {
                // toggles to check for quotes used for commands like echo
                if (input.at(i) =='\"') {
                    if (!quotes) {
                        quotes = true;
                    } else {
                        quotes = false;
                    }
                }
		        
                // cout << "10" << endl;

                if (!quotes) {
                    if (input.at(i) == ';') {                 		            
                        if (ptr == NULL) {
                            // this is the first Command being created
                            // create Command object of everything up until that
                            // semicolon
                            string temp = input.substr(0, i);
                            trim(temp);
                   
                            ptr = new Command(temp);
                            
                            commStrs.push_back(temp);

                            input = input.substr(i + 1, input.size() - 1);
                            cout << input << endl;
                            i = 0;
                            connVals.push_back(1);
                        } else {
                            if (connVals.at(connVals.size() - 1) == 1) {
                                // need new pointer variable to hold new connector
                                // object
                                string temp = input.substr(0, i);
                                trim(temp);
                                Shell* a = new Command(temp);
                                commStrs.push_back(temp);

                                input = input.substr(i + 1, input.size() - 1);
                                cout << input << endl;
                                Shell* b = new Semi(ptr, a);
                                
                                // Shell* pointer now points to semicolon
                                ptr = b;
                                i = 0;
                                connVals.push_back(1);
                            } else if (connVals.at(connVals.size() - 1) == 2) {
                                string temp = input.substr(0, i);
                                trim(temp);
                                
                                Shell* a = new Command(temp);
                                commStrs.push_back(temp);

                                input = input.substr(i + 1, input.size() - 1);
                                cout << input << endl;
                                Shell* b = new And(ptr, a);
    
                                ptr = b;
                                i = 0;
                                connVals.push_back(1);
                            } else if (connVals.at(connVals.size() - 1) == 3) {
                                string temp = input.substr(0, i);
                                trim(temp);
                                Shell* a = new Command(temp);
                                commStrs.push_back(temp);

                                input = input.substr(i + 1, input.size() - 1);
                                cout << input << endl;
                                Shell* b = new Or(ptr, a);
    
                                ptr = b;
                                i = 0;
                                connVals.push_back(1);
                            }
                        }
                    } else if (input.at(i) == '&' && input.at(i + 1) == '&') {    		            
                        if (ptr == NULL) {
                            // create Command object of everything up until the
                            // and symbols
                            string temp = input.substr(0, i);
                            trim(temp);

                            ptr = new Command(temp);
                            commStrs.push_back(temp);
                            
                            input = input.substr(i + 2, input.size() - 1);
                            cout << input << endl;
                            i = 0;
                            connVals.push_back(2);
                        } else {
                            if (connVals.at(connVals.size() - 1) == 1) {
                                string temp = input.substr(0, i);
                                trim(temp);
                                Shell* a = new Command(temp);
                                commStrs.push_back(temp);

                                input = input.substr(i + 2, input.size() - 1);
                                cout << input << endl;
                                Shell* b = new Semi(ptr, a);

                                ptr = b;
                                i = 0;
                                connVals.push_back(2);
                            } else if (connVals.at(connVals.size() - 1) == 2) {
                                string temp = input.substr(0, i);
                                trim(temp);
                                Shell* a = new Command(temp);
                                commStrs.push_back(temp);

                                input = input.substr(i + 2, input.size() - 1);
                                cout << input << endl;
                                Shell* b = new And(ptr, a);
    
                                ptr = b;
                                i = 0;
                                connVals.push_back(2);
                            } else if (connVals.at(connVals.size() - 1) == 3) {
                                string temp = input.substr(0, i);
                                trim(temp);
                                Shell* a = new Command(temp);
                                commStrs.push_back(temp);
                                
                                input = input.substr(i + 2, input.size() - 1);
                                cout << input << endl;
                                Shell* b = new Or(ptr, a);
    
                                ptr = b;
                                i = 0;
                                connVals.push_back(2);
                            }
                        }
                    } else if (input.at(i) == '|' && input.at(i + 1) == '|') {   
                        if (ptr == NULL) {
                            // create Command object of everything up until the
                            // or symbol
                            string temp = input.substr(0, i);
                            trim(temp);
                            ptr = new Command(temp);
                            commStrs.push_back(temp);

                            input = input.substr(i + 2, input.size() - 1);
                            cout << input << endl;
                            i = 0;
                            connVals.push_back(3);
                        } else {
                            if (connVals.at(connVals.size() - 1) == 1) {
                                string temp = input.substr(0, i);
                                trim(temp);
                                Shell* a = new Command(temp);
                                commStrs.push_back(temp);

                                input = input.substr(i + 2, input.size() - 1);
                                cout << input << endl;
                                Shell* b = new Semi(ptr, a);
    
                                ptr = b;
                                i = 0;
                                connVals.push_back(3);
                            } else if (connVals.at(connVals.size() - 1) == 2) {
                                string temp = input.substr(0, i);
                                trim(temp);
                                Shell* a = new Command(temp);
                                commStrs.push_back(temp);

                                input = input.substr(i + 2, input.size() - 1);
                                cout << input << endl;
                                Shell* b = new And(ptr, a);
    
                                ptr = b;
                                i = 0;
                                connVals.push_back(3);
                            } else if (connVals.at(connVals.size() - 1) == 3) {
                                string temp = input.substr(0, i);
                                trim(temp);
                                Shell* a = new Command(temp);
                                commStrs.push_back(temp);

                                input = input.substr(i + 2, input.size() - 1);
                                cout << input << endl;
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
            
            // cout << "11" << endl;

            // account for final command
            trim(input);
            commStrs.push_back(input);
            
            if (ptr == NULL) {
                ptr = new Command(input);
                input = "";
            } else {
                Shell* a = new Command(input);
                Shell* b = new Semi(ptr, a);
                ptr = b;
                input = "";
            }
            // `cout << "12" << endl;

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
