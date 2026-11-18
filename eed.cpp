#include <iostream>
// maybe making our own fstream could be a
// fun project 
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <sstream>

using namespace std;

// pseudocode
// void execcommand(command table, argc, char *argv[]) {
// if command exists
// run with args
// else
// cout "?"
// }
// how would one interpret a line number? - if argv[n].type == int {special case, else execcommand}
// flip that to be if not int
void execcmd(unordered_map<string, function<void()>> cmds, int argc, vector<string> argv) {
    if (cmds[argv[0]])
    cmds[argv[0]]();
    else
    cout << "nocmd\n";
}

// cpp is dumb
bool isint(string s) {
    for (int i = 0; i < s.length(); i++)
    if (!(isdigit(s[i]) || s[i] == '-'))
        return false;
    return true;
}

int main(int argc, char *argv[]) {
    vector<string> docbuf;
    int r = 0;

    unordered_map<string, function<void()>> cmds = {
        // turn r and c to addresses later
        {"a", [&docbuf, &r](){
            string orig = docbuf[r];
            string input;
            string output;
            while (getline(cin,input)) {
            if (input == ".")
                break;
            else {
                output += input;
            }
            }
            docbuf[r] = orig + output;
        }},
        // note for future: to get anything from docbuf you need to pass it
        // by reference to lambda functions
        // might be because it makes a copy on runtime or something
        {"w", [argc, argv, &docbuf](){
            ofstream out(argv[argc-1]);
            if (!out.is_open()) {
            cerr << "no";
            return;
            }
            out.seekp(0);
            for (int i = 0; i < docbuf.size(); i++) {
            out << docbuf.at(i) << endl;
            }
            out.close();
        }},
        {"i", [&docbuf, &r](){
            string orig = docbuf[r];
            string input;
            string output;
            while (getline(cin,input)) {
            if (input == ".")
                break;
            else {
                output += input;
            }
            }
            docbuf[r] = output + orig;
        }}
    };

    ifstream in(argv[argc-1]);
    
    if (!in.is_open()) {
        cerr << "no";
        return 1;
    }
    
    string line;
    while (getline(in, line)) {
        docbuf.push_back(line);
    }

    in.close();

    while (true) {
        string cmdin;
        vector<string> tokens;
        cout << "> ";
        getline(cin, cmdin);
        stringstream parsed(cmdin);
        string token;
        while (parsed >> token)
            tokens.push_back(token);
        if (tokens[0] == "q")
            break;
        if (!isint(tokens[0])) {
            execcmd(cmds,tokens.size(),tokens);
        } else {
            int n = (stol(tokens[0]) > 0) ?
                stol(tokens[0]) - 1:
                stol(tokens[0]);
            string output = (abs(n) < docbuf.size()) ?
                docbuf.at((docbuf.size() + n) % docbuf.size()) :
                "noln\n";
            cout << output << endl;
        }
    }
    
    return 0;
}
