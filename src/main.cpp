#include "Shell.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>

//comment

using namespace std;

int main() {
    string user = getlogin();
    char hostname[100];
    gethostname(hostname, 100);
    
    cout << "[" << user << "@" << hostname << "]$ ";
    

    //Shell s;
    //s.prompt();
    
    /*while(1) {
        string input;
        getline(cin, input);
        
        stringstream str(input);
        string s;
        
        vector <string> words;

        while (str >> s) {
            words.push_back(s);

            if (s == "exit") {
                return 0;
            } else {
                cout << s << endl;
            }
        }

        // check
        // cout << input << endl;
    }*/

    return 0;
}
