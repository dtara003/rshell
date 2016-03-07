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

        void extendTree(Shell* a, int connVal, Shell* &root)
        {
            Shell* d;
            if (connVal == 2)
            {
                d = new And(root, a);
                root = d;
            }
            else if (connVal == 3)
            {
                d = new Or(root, a);
                root = d;
            }
            else if (connVal == 1)
            {
                d = new Semi(root, a);
                root = d;
            }
        };

        Shell* nested(string inp)
        {
            int connVal = 0;
            bool moreComms = false;
            string op;
            //Shell* a;
            //Shell* b;
            Shell* subRoot = 0;
            for (unsigned i = 0; i < inp.size(); ++i)
            {
                if (inp.at(i) == '(')
                {
                    unsigned parentheses = 1;
                    for(unsigned j = i + 1; parentheses != 0 && j < inp.size(); ++ j)
                    {
                        if (inp.at(j) == '(')
                        {
                            parentheses++;
                        }
                        else if (inp.at(j) == ')')
                        {
                            parentheses--;
                        }
                        if (parentheses == 0)
                        {
                            string str_a = inp.substr(i+1, j-i-1);
                            Shell *a = nested(str_a);
                            inp = inp.substr(j+1, inp.size()-1);
                            i = 0;
                            if (subRoot == 0)
                            {
                                subRoot = a;
                            }
                            else
                            {
                                extendTree(a, connVal, subRoot);
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
                        extendTree(a, connVal, subRoot);
                        connVal = 2;
                    }
                }
                //all parts same as and parse
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
                        extendTree(a, connVal, subRoot);
                        connVal = 3;
                    }

                }
                //all parts same as and parse
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
                        extendTree(a, connVal, subRoot);
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
            }

            if (subRoot == 0)
            {
                subRoot= new Command(inp);
            }
            //creates operator if node already exists
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

        void parse()
        {
            string op;
            int parentheses = 0;
            int connVal = 0;
            bool moreCommands = false;
            
            unsigned int i = 0;
            int numParen = 0;
            for (i = 0; i < input.size(); ++i) {
                if (input.at(i) == '(') {
                    numParen++;
                }
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

            while (i < input.size() - 1)
            {
                if (input.at(i) == '(')
                {
                    parentheses++;

                    unsigned int j = i;
                    j++;

                    while (parentheses != 0)
                    {
                        if (input.at(j) == '(')
                        {
                            parentheses++;
                            numParen = parentheses;
                        }
                        else if (input.at(j) == ')')
                        {
                            parentheses--;
                            numParen = parentheses;
                        }

                        if (parentheses == 0)
                        {
                            pos = j;
                            string str_a = input.substr(i+1, j-i-1);
                            Shell* a = nested(str_a);
                            input = input.substr(pos + 1, input.size()-1);
                            i = 0;
                            if (ptr == 0)
                            {
                                ptr = a;
                            }
                            else
                            {
                                extendTree(a, connVal, ptr);
                            }
                            if (input.size() != 0)
                            {
                                for (unsigned k = 0; input.size() + 1 > k; ++k)
                                {
                                    if (input.at(k) == '&' && input.at(k+1) == '&')
                                    {
                                       connVal = 2;
                                       moreCommands = true;
                                    }
                                    else if ((input.at(k) == '|') && (input.at(k+1) == '|'))
                                    {
                                        connVal = 3;
                                        moreCommands = true;
                                    }
                                    else if (input.at(k) == ';')
                                    {
                                        connVal = 1;
                                        input = input.substr(k+1, input.size()-1);
                                    }
                                    if (moreCommands)
                                    {
                                        if (input.size() > k+2)
                                        {
                                            input = input.substr(k+2, input.size());
                                        }
                                        else
                                        {
                                            input = " ";
                                        }
                                        k = 0;
                                    }
                                    if (moreCommands) break; 
                                }
                                moreCommands = false;
                            }
                        }
                        
                        j++;
                    }
                }
                if (input.size() == 0) break;
                if (input.at(i) == '&' && input.at(i+1) == '&')
                {
                    moreCommands = true;
                    //empty list check to create first node
                    if (ptr == 0)
                    {
                        op = input.substr(0, i);
                        ptr = new Command(op);
                        connVal = 2;
                    }
                    //creates operator Shelld on what was last seen and
                    //updates what operator was last seen while creating
                    //the operator
                    else
                    {
                        op = input.substr(0, i);
                        Shell* a = new Command(op);
                        extendTree(a, connVal, ptr);
                        connVal = 2;
                    }
                }
                //all parts same as and parse
                else if (input.at(i) == '|' && input.at(i+1) == '|')
                {
                    moreCommands = true;
                    if (ptr == 0)
                    {
                        op = input.substr(0, i);
                        ptr = new Command(op);
                        connVal = 3;
                    }
                    else
                    {
                        op = input.substr(0, i);
                        Shell* a = new Command(op);
                        extendTree(a, connVal, ptr);
                        connVal = 3;
                    }

                }
                //all parts same as and parse
                else if (input.at(i) == ';')
                {
                    if (ptr == 0)
                    {
                        op = input.substr(0, i);
                        input = input.substr(i+1, input.size()-1);
                        i = 0;
                        ptr = new Command(op);
                        connVal = 1;
                    }
                    else
                    {
                        op = input.substr(0, i);
                        input = input.substr(i+1, input.size());
                        i = 0;
                        Shell* a = new Command(op);
                        extendTree(a, connVal, ptr);
                        connVal = 1;
                    }

                }
                if (moreCommands)
                {
                    if (input.size() > i+2)
                    {
                        moreCommands = false;
                        input = input.substr(i+2, input.size());
                    }
                    else
                    {
                        input = " ";
                    }
                    i = 0;
                }

                i++;
            }
            //creates inital node if none were created
            if (ptr == 0)
            {
                ptr = new Command(input);
            }
            //creates operator if node already exists
            else
            {
                Shell* a = new Command(input);
                if (connVal == 2)
                {
                    Shell* b = new And(ptr, a);
                    ptr = b;
                }
                else if (connVal == 3)
                {
                    Shell* b = new  Or(ptr, a);
                    ptr = b;
                }
                else if (connVal == 1)
                {
                    Shell* b = new Semi(ptr, a);
                    ptr = b;
                }
            }
        };

};

#endif

