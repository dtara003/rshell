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
        
        void outputTree() {
            ptr->output();
        };

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
        };

        string cut(string str) {
            trim(str);

            return str;
        };

        string cut(string str, int start, int end) {
            return str.substr(start, end);
        };

        void parse() {
            int parentheses = 0;
            int connVal = 0;
            bool moreCommands = false;
            ptr = NULL; 
            unsigned int i = 0;
            int numParen = 0;
       
            for (i = 0; i < input.size(); ++i) {
                if (input.at(i) == '(') {
                    numParen++;
                }//  else if (input.at(i) == ')') {
                else if (input.at(i) == ')') {
                    numParen--;
                }
            }

            if (numParen != 0) {
                cout << "ERROR: unbalanced parentheses" << endl;

                return;
            }
            
            numParen = 0;
            int pos = 0;

            for (i = 0; i < input.size(); ++i) {
                if (input.at(i) == '(') {
                    numParen++;
                } else if (input.at(i) == ')') {
                    numParen--;
                }
                
                if (numParen == 0) {
                    pos = numParen;
                }
            }

            i = 0;

            while (i < input.size() - 1) {
                if (input.at(i) == '(') {
                    parentheses++;

                    unsigned int j = i;
                    j++;

                    while (parentheses != 0) {
                        if (input.at(j) == '(') {
                            parentheses++;
                            numParen = parentheses;
                        } else if (input.at(j) == ')') {
                            parentheses--;
                            numParen = parentheses;
                        }

                        if (parentheses == 0)
                        {
                            pos = j;
                            Shell* a = nested(cut(input, i + 1, j - i - 1));
                            input = cut(input, pos + 1, input.size() - 1);
                            i = 1;
                            i -= 1;

                            if (ptr != NULL){
                                Shell* c = NULL;
                                if (connVal == 1) {
                                    c = new Semi(ptr, a);
                                } else if (connVal == 2) {
                                    c = new And(ptr, a);
                                } else if (connVal == 3) {
                                    c = new Or(ptr, a);
                                }
                                
                                ptr = c;
                            }
                            else {
                                ptr = a;
                            }
                            if (input.size() != 0)
                            {
                                unsigned int k = 5;
                                while (k != 0) {
                                    k--;
                                }

                                while (k < input.size() + 1) {
                                    if (input.at(k) == ';') {
                                        connVal = 1;
                                        int start = k + 1;
                                        int end = input.size() - 1;
                                        input = cut(input, start, end);
                                    } else if (input.at(k) == '&' && input.at(k + 1) == '&') {
                                        connVal = 2;
                                        moreCommands = true;

                                        if (k + 2 >= input.size()) {
                                            input.erase(input.begin(), input.end());
                                            moreCommands = true;
                                            k = 0;
                                        } else {
                                            input = cut(input, k + 2, input.size());
                                            moreCommands = true;
                                            k = 0;
                                        }
                                    } else if (input.at(k) == '|' && input.at(k + 1) == '|') {
                                        connVal = 3;
                                        
                                        if (k + 2 >= input.size()) {
                                            input.erase(input.begin(), input.end());
                                            moreCommands = true;
                                            k = 0;
                                        } else {
                                            input = cut(input, k + 2, input.size());
                                            moreCommands = true;
                                            k = 0;
                                        }
                                    }
                                    
                                    if (moreCommands) break; 
                                    k++;
                                }
                                if (moreCommands == true) {
                                    moreCommands = !moreCommands;
                                }
                            }
                        }
                        
                        j++;
                    }
                }

                if (input.size() < 1) break;
                
                if (ptr == NULL) {
                    if (input.at(i) == ';') {
                        ptr = new Command(cut(cut(input, 0, i)));
                        input = cut(input, i + 1, input.size() - 1);
                        connVal = 1;
                        i = connVal - 1;
                    } else if (input.at(i) == '&' && input.at(i + 1) == '&') {
                        ptr = new Command(cut(cut(input, 0, i)));
                        connVal = 2;

                        moreCommands = true;

                        if (i + 2 >= input.size()) {
                                input.erase(input.begin(), input.end());
                                i = 0;
                        } else {
                            if (moreCommands == true) {
                                moreCommands = !moreCommands;
                            }
                            input = cut(input, i + 2, input.size());
                            i = 0;
                        }

                    } else if (input.at(i) == '|' && input.at(i + 1) == '|') {
                        ptr = new Command(cut(cut(input, 0, i)));
                        connVal = 3;

                        moreCommands = true;

                        if (i + 2 >= input.size()) {
                                input.erase(input.begin(), input.end());
                                i = 0;
                        } else {
                            if (moreCommands == true) {
                                moreCommands = !moreCommands;
                            }
                            input = cut(input, i + 2, input.size());
                            i = 0;
                        }
                    }
                } else if (ptr != NULL) {
                    if (input.at(i) == ';') {
                        Shell* a = new Command(cut(cut(input, 0, i)));
                        input = cut(input, i + 1, input.size());

                        Shell* c = NULL;
                        if (connVal == 1) {
                            c = new Semi(ptr, a);
                        } else if (connVal == 2) {
                            c = new And(ptr, a);
                        } else if (connVal == 3) {
                            c = new Or(ptr, a);
                        }

                        connVal = 1;
                        i = connVal - 1;
                        ptr = c;
                    } else if (input.at(i) == '&' && input.at(i + 1) == '&') {
                        Shell* a = new Command(cut(cut(input, 0, i)));

                        Shell* c = NULL;
                        if (connVal == 1) {
                            c = new Semi(ptr, a);
                        } else if (connVal == 2) {
                            c = new And(ptr, a);
                        } else if (connVal == 3) {
                            c = new Or(ptr, a);
                        }

                        connVal = 2;
                        ptr = c;

                        moreCommands = true;

                        if (i + 2 >= input.size()) {
                            input.erase(input.begin(), input.end());
                            i = 0;
                        } else {
                            if (moreCommands == true) {
                                moreCommands = !moreCommands;
                            }
                            input = cut(input, i + 2, input.size());
                            i = 0;
                        }
                    } else if (input.at(i) == '|' && input.at(i + 1) == '|') {
                        Shell* a = new Command(cut(cut(input, 0, i)));
                        
                        Shell* c = NULL;
                        if (connVal == 1) {
                            c = new Semi(ptr, a);
                        } else if (connVal == 2) {
                            c = new And(ptr, a);
                        } else if (connVal == 3) {
                            c = new Or(ptr, a);
                        }

                        connVal = 3;
                        ptr = c;
                        
                        moreCommands = true;

                        if (i + 2 >= input.size()) {
                            input.erase(input.begin(), input.end());
                            i = 0;
                        } else {
                            if (moreCommands == true) {
                                moreCommands = !moreCommands;
                            }
                            input = cut(input, i + 2, input.size());
                            i = 0;
                        }
                    }
                }

                i++;
            }

            Shell* b;

            if (ptr != NULL) {
                if (connVal == 1) {
                    Shell* a = new Command(cut(input));
                    b = new Semi(ptr, a);
                } else if (connVal == 2) {
                    Shell* a = new Command(cut(input));
                    b = new And(ptr, a);
                } else if (connVal == 3) {
                    Shell* a = new Command(cut(input));
                    b = new Or(ptr, a);
                }
                
                ptr = b;
            } else {
                ptr = new Command(cut(cut(input)));
            }
        };
//------------------------------------------------------------
        Shell* nested(string inp) {
            int connVal = 0;
            bool moreComms = false;
            Shell* subRoot = NULL;
            int numParen = 0;
            string op;
            unsigned int parentheses = 0;

            unsigned int i = 1;
            i -= 1;

            while (i < inp.size()) {
                if (inp.at(i) == '(') {
                    parentheses++;

                    unsigned int j = i;
                    j++;

                    while (parentheses != 0 && j < inp.size()) {
                        if (inp.at(j) == '('){
                            parentheses++;
                            numParen = parentheses;
                        } else if (inp.at(j) == ')') {
                            parentheses--;
                            numParen = parentheses;
                        }
                        
                        if (parentheses == 0) {
                            int start = i + 1;
                            int end = j - i;
                            end -= 1;

                            Shell* a = nested(cut(inp, start, end));
                            inp = cut(inp, j + 1, inp.size() - 1);
                            i = 1;
                            i -= 1;

                            if (subRoot != NULL) {
                                Shell* c = NULL;
                                if (connVal == 1) {
                                    c = new Semi(subRoot, a);
                                } else if (connVal == 2) {
                                    c = new And(subRoot, a);
                                } else if (connVal == 3) {
                                    c = new Or(subRoot, a);
                                }

                                subRoot = c;

                            } else {
                                subRoot = a;
                            }
                            if (inp.size() != 0)
                            {

                                for (unsigned k = 0; inp.size() + 1 > k; ++k)
                                {
                                    if (inp.at(k) == '&' && inp.at(k+1) == '&')
                                    {
                                       connVal = 2;
                                       moreComms = true;
                                    }
                                    else if ((inp.at(k) == '|') && (inp.at(k+1) == '|'))
                                    {
                                        connVal = 3;
                                        moreComms = true;
                                    }
                                    else if (inp.at(k) == ';')
                                    {
                                        connVal = 1;
                                        inp = inp.substr(k+1, inp.size()-1);
                                    }
                                    if (moreComms)
                                    {
                                        if (inp.size() > k+2)
                                        {
                                            inp = inp.substr(k+2, inp.size());
                                        }
                                        else
                                        {
                                            inp = " ";
                                        }
                                        k = 0;
                                    }
                                    if (moreComms) break; 
                                }
                                moreComms = false;
                            }
 
                        }

                        j++;
                    }
                }
                //run parsing for && || and ; but creating a
                //Parentheses Class and adding it to tree;
                if (inp.size() == 0) break;
                if (inp.at(i) == '&' && inp.at(i+1) == '&')
                {
                    moreComms = true;
                    //empty list check to create first node
                    if (subRoot == 0)
                    {
                        op = inp.substr(0, i);
                        subRoot = new Command(op);
                        connVal = 2;
                    }
                    //creates operator Shelld on what was last seen and
                    //updates what operator was last seen while creating
                    //the operator
                    else
                    {
                        op = inp.substr(0, i);
                        Shell* a = new Command(op);

                        Shell* c = NULL;
                        if (connVal == 1) {
                            c = new Semi(subRoot, a);
                        } else if (connVal == 2) {
                            c = new And(subRoot, a);
                        } else if (connVal == 3) {
                            c = new Or(subRoot, a);
                        }

                        subRoot = c;
                        connVal = 2;
                    }
                }
                else if (inp.at(i) == '|' && inp.at(i+1) == '|')
                {
                    moreComms = true;
                    if (subRoot == 0)
                    {
                        op = inp.substr(0, i);
                        subRoot = new Command(op);
                        connVal = 3;
                    }
                    else
                    {
                        op = inp.substr(0, i);
                        Shell* a = new Command(op);

                        Shell* c = NULL;
                        if (connVal == 1) {
                            c = new Semi(subRoot, a);
                        } else if (connVal == 2) {
                            c = new And(subRoot, a);
                        } else if (connVal == 3) {
                            c = new Or(subRoot, a);
                        }

                        subRoot = c;
                        connVal = 3;
                    }

                }
                else if (inp.at(i) == ';')
                {
                    if (subRoot == 0)
                    {
                        op = inp.substr(0, i);
                        inp = inp.substr(i+1, inp.size()-1);
                        i = 0;
                        subRoot = new Command(op);
                        connVal = 1;
                    }
                    else
                    {
                        op = inp.substr(0, i);
                        inp = inp.substr(i+1, inp.size());
                        i = 0;
                        Shell* a = new Command(op);

                        Shell* c = NULL;
                        if (connVal == 1) {
                            c = new Semi(subRoot, a);
                        } else if (connVal == 2) {
                            c = new And(subRoot, a);
                        } else if (connVal == 3) {
                            c = new Or(subRoot, a);
                        }

                        subRoot = c;
                        connVal = 1;
                    }

                }
                if (moreComms)
                {
                    if (inp.size() > i+2)
                    {
                        moreComms = false;
                        inp = inp.substr(i+2, inp.size());
                    }
                    else
                    {
                        inp = " ";
                    }
                    i = 0;
                }

                i++;
            }

            if (subRoot == 0)
            {
                subRoot= new Command(inp);
            }
            else
            {
                Shell* a = new Command(inp);
                if (connVal == 2)
                {
                    Shell* b = new And(subRoot, a);
                    subRoot = b;
                }
                else if (connVal == 3)
                {
                    Shell* b = new Or(subRoot, a);
                    subRoot = b;
                }
                else if (connVal == 1)
                {
                    Shell* b = new Semi(subRoot, a);
                    subRoot = b;
                }
            }
 
            return subRoot;
        };

};

#endif

