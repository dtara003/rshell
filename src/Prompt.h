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
        //vector <string> commStrs;
        //vector <int> connVals;
      
    public:
        Shell* ptr;

        Prompt() {
            ptr = NULL;
        };

        //Prompt(string s, vector <int> v) : input(s), connVals(v) {};
        Prompt(string s)
            : input(s)
        {};
		
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
                isNull = true;
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
        };

        string cut(string str, int start, int end) {
            return str.substr(start, end);
        };

        void parse() {
            // the following two while loops are probably not a viable method of
            // interpreting command line inputs
            // weak method error-catching
            // just gets rid of starting and ending errors altogether
            // should in reality throw an error if starting with connectors and
            // ask for more input if ending with connectors
                
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
            
            //bool quotes = false;
            bool more = false;
            int connVal = 0;
            unsigned int i = 0;
            while (i < input.size()) {
                // toggles to check for quotes
                //if (input.at(i) =='\"') {
                    //if (!quotes) {
                    //    quotes = true;
                    //} else {
                    //    quotes = false;
                    //}
                //}
                
                // if not waiting for closing quotations then parse
                //if (!quotes) {

                if (input.at(i) == '(') {
                    unsigned int parentheses = 1;
                    unsigned int j = i + 1;
                    while (parentheses != 0) {
                        if (input.at(j) == '(') {
                            parentheses++;
                        } else if (input.at(j) == ')') {
                            parentheses--;
                        }

                        if (parentheses == 0) {
                            string inParen = input.substr(i + 1, j - i - 1);
                            trim(inParen);
                            Shell* a = nest(inParen);
                            input = input.substr(j + 1, input.size() - 1);
                            i = 0;

                            if (ptr == NULL) {
                                ptr = a;
                            } else {
                                Shell* d;

                                if (connVal == 1) {
                                    d = new Semi(ptr, a);
                                    ptr = d;
                                } else if (connVal == 2) {
                                    d = new And(ptr, a);
                                    ptr = d;
                                } else if (connVal == 3) {
                                    d = new Or(ptr, a);
                                    ptr = d;
                                }
                            }

                            if (input.size() != 0) {
                                unsigned int k = 0;
                                while (k < input.size() + 1) {
                                    if (input.at(k) == ';') {
                                        connVal = 1;
                                        trim(input);
                                        input = input.substr(k + 1, input.size() - 1);
                                        trim(input);
                                    } else if (input.at(k) == '&' && input.at(k + 1) == '&') {
                                        connVal = 2;
                                        more = true;
                                    } else if (input.at(k) == '|' && input.at(k + 1) == '|') {
                                        connVal = 3;
                                        more = true;
                                    }

                                    if (more) {
                                        if (input.size() > k + 2) {
                                            trim(input);
                                            input = input.substr(k + 2, input.size());
                                            trim(input);
                                        } else {
                                            input = "";
                                        }

                                        k = 0;
                                    }
                                    
                                    if (more) break;

                                    k++;
                                }

                                more = false;
                            }

                        } else if (j + 1 == input.size()) {
                            input = "";
                            cout << "syntax error" << endl;

                            return;
                        }

                        j++;
                    }
                }

                if (input.size() == 0) break;

                if (input.at(i) == ';') {
                    if (ptr == NULL) {
                        string str = input.substr(0, i);
                        trim(str);
                        input = input.substr(i + 1, input.size() - 1);
                        trim(input);
                        i = 0;
                        ptr = new Command(str);
                        connVal = 1;
                    } else {
                        string str = input.substr(0, i);
                        trim(str);
                        input = input.substr(i + 1, input.size());
                        trim(input);
                        i = 0;
                        Shell* a = new Command(str);

                        Shell* d;

                        if (connVal == 1) {
                            d = new Semi(ptr, a);
                            ptr = d;
                        } else if (connVal == 2) {
                            d = new And(ptr, a);
                            ptr = d;
                        } else if (connVal == 3) {
                            d = new Or(ptr, a);
                            ptr = d;
                        }

                        connVal = 1;
                    }
                } else if (input.at(i) == '&' && input.at(i + 1) == '&') {
                    more = true;
                    
                    if (ptr == NULL) {
                        string str = input.substr(0, i);
                        trim(str);
                        ptr = new Command(str);
                        connVal = 2;
                    } else {
                        string str = input.substr(0, i);
                        trim(str);
                        Shell* a = new Command(str);

                        Shell* d;

                        if (connVal == 1) {
                            d = new Semi(ptr, a);
                            ptr = d;
                        } else if (connVal == 2) {
                            d = new And(ptr, a);
                            ptr = d;
                        } else if (connVal == 3) {
                            d = new Or(ptr, a);
                            ptr = d;
                        }

                        connVal = 2;
                    }
                } else if (input.at(i) == '|' && input.at(i + 1) == '|') {
                    more = true;

                    if (ptr == NULL) {
                        string str = input.substr(0, i);
                        trim(str);
                        ptr = new Command(str);
                        connVal = 3;
                    } else {
                        string str = input.substr(0, i);
                        trim(str);
                        Shell* a = new Command(str);

                        Shell* d;

                        if (connVal == 1) {
                            d = new Semi(ptr, a);
                            ptr = d;
                        } else if (connVal == 2) {
                            d = new And(ptr, a);
                            ptr = d;
                        } else if (connVal == 3) {
                            d = new Or(ptr, a);
                            ptr = d;
                        }

                        connVal = 3;
                    }
                }
                
                if (more) {
                    if (input.size() > i + 2) {
                        more = false;
                        input = input.substr(i + 2, input.size());
                        trim(input);
                    } else {
                        input = "";
                    }

                    i = 0;
                }

                i++;
            }

            if (ptr == NULL) {
                ptr = new Command(input);
            } else {
                if (connVal == 1) {
                    Shell* a = new Command(input);
                    Shell* b = new Semi(ptr, a);
                    ptr = b;
                } else if (connVal == 2) {
                    Shell* a = new Command(input);
                    Shell* b = new And(ptr, a);
                    ptr = b;
                } else if (connVal == 3) {
                    Shell* a = new Command(input);
                    Shell* b = new Or(ptr, a);
                    ptr = b;
                }
            }
        };
// row
// --------------------------------------------------------------------------
                    /*if (ptr == NULL) {
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
            
            // check
            for (unsigned int i = 0; i < connVals.size(); ++i) {
                cout << connVals.at(i) << " ";
            }

            cout << endl;
		    
            for (unsigned int i = 0; i < commStrs.size(); ++i) {
                cout << commStrs.at(i) << endl;
            }

            cout << input << endl;
        };*/

        Shell* nest(string input2) {
            string inp = input2;
            int connVal = 0;
            bool more = false;
            string str = "";
            Shell* a;
            Shell* b;
            Shell* subRoot = NULL;

            unsigned int i = 0;
            while (i < inp.size()) {
                if (inp.at(i) == '(') {
                    unsigned int parentheses = 1;

                    unsigned int j = i + 1;
                    while (parentheses != 0 && j < inp.size()) {
                        if (inp.at(j) == '(') {
                            parentheses++;
                        } else if (inp.at(j) == ')') {
                            parentheses--;
                        }
                        
                        if (parentheses == 0) {
                            Shell* c = NULL;
                            string inParen = inp.substr(i + 1, j - i - 1);
                            trim(inParen);
                            c = nest(inParen);
                            inp = inp.substr(j + 1, inp.size() - 1);
                            trim(inp);
                            i = 0;

                            if (subRoot == NULL) {
                                subRoot = c;
                            } else {
                                Shell* d;

                                if (connVal == 1) {
                                    d = new Semi(subRoot, c);
                                    subRoot = d;
                                } else if (connVal == 2) {
                                    d = new And(subRoot, c);
                                    subRoot = d;
                                } else if (connVal == 3) {
                                    d = new Or(subRoot, c);
                                    subRoot = d;
                                }
                            }

                            if (inp.size() != 0) {
                                unsigned int k = 0;
                                while (k < inp.size() + 1) {
                                    if (inp.at(k) == ';') {
                                        connVal = 1;
                                        inp = inp.substr(k + 1, inp.size() - 1);
                                        trim(inp);
                                    } else if (inp.at(k) == '&' && inp.at(k + 1) == '&') {
                                        connVal = 2;
                                        more = true;
                                    } else if (inp.at(k) == '|' && inp.at(k + 1) == '|') {
                                        connVal = 3;
                                        more = true;
                                    }

                                    if (more) {
                                        if (inp.size() > k + 2) {
                                            inp = inp.substr(k + 2, inp.size());
                                            trim(inp);
                                        } else {
                                            inp = "";
                                        }

                                        k = 0;
                                    }
                                    if (more) break;

                                    k++;
                                }

                                more = false;
                            }
                        }

                        j++;
                    }
                }

                if (inp.size() == 0) break;
                if (inp.at(i) == ';') {
                    if (subRoot == NULL) {
                        str = inp.substr(0, i);
                        trim(str);
                        inp = inp.substr(i + 1, inp.size() - 1);
                        trim(inp);
                        i = 0;
                        subRoot = new Command(str);
                        connVal = 1;
                    } else {
                        str = inp.substr(0, i);
                        inp = inp.substr(i + 1, inp.size());
                        i = 0;
                        a = new Command(str);
                        
                        Shell* d;

                        if (connVal == 1) {
                            d = new Semi(subRoot, a);
                            subRoot = d;
                        } else if (connVal == 2) {
                            d = new And(subRoot, a);
                            subRoot = d;
                        } else if (connVal == 3) {
                            d = new Or(subRoot, a);
                            subRoot = d;
                        }
                        
                        connVal = 1;
                    }
                } else if (inp.at(i) == '&' && inp.at(i + 1) == '&') {
                    more = true;

                    if (subRoot == NULL) {
                        str = inp.substr(0, i);
                        trim(str);
                        subRoot = new Command(str);
                        connVal = 2;
                    } else {
                        str = inp.substr(0, i);
                        trim(str);
                        a = new Command(str);
                        
                        Shell* d;

                        if (connVal == 1) {
                            d = new Semi(subRoot, a);
                            subRoot = d;
                        } else if (connVal == 2) {
                            d = new And(subRoot, a);
                            subRoot = d;
                        } else if (connVal == 3) {
                            d = new Or(subRoot, a);
                            subRoot = d;
                        }

                        connVal = 2;
                    }
                } else if (inp.at(i) == '|' && inp.at(i + 1) == '|') {
                    more = true;
                    if (subRoot == NULL) {
                        str = inp.substr(0, i);
                        str = cut(str);
                        subRoot = new Command(str);
                        connVal = 3;
                    } else {
                        str = inp.substr(0, i);
                        str = cut(str);
                        a = new Command(str);
                        
                        Shell* d;

                        if (connVal == 1) {
                            d = new Semi(subRoot, a);
                            subRoot = d;
                        } else if (connVal == 2) {
                            d = new And(subRoot, a);
                            subRoot = d;
                        } else if (connVal == 3) {
                            d = new Or(subRoot, a);
                            subRoot = d;
                        }

                        connVal = 3;
                    }
                }

                if (more) {
                    if (inp.size() > i + 2) {
                        more = false;
                        inp = inp.substr(i + 2, inp.size());
                        inp = cut(inp);
                    } else {
                        inp = "";
                    }

                    i = 0;
                }

                i++;
            }
            
            //trim(input);
            inp = cut(inp);

            if (subRoot == NULL) {
                subRoot = new Command(inp);
            } else {
                if (connVal == 1) {
                    a = new Command(inp);
                    b = new Semi(subRoot, a);
                    subRoot = b;
                } else if (connVal == 2) {
                    a = new Command(inp);
                    b = new And(subRoot, a);
                    subRoot = a;
                } else if (connVal == 3) {
                    a = new Command(inp);
                    b = new Or(subRoot, a);
                    subRoot = b;
                }
            }

            return subRoot;

            // remove later
            //return NULL;

        };
		
        // DEFINED WITHIN SHELL
        /*void freeMem() {
            // DO NOT FORGET
            // FREE ANY MEMORY THAT WAS ALLOCATED
        };*/
};

#endif
