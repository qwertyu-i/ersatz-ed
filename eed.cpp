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
	cout << "?\n";
}

// cpp is dumb
bool isint(string s) {
    for (int i = 0; i < s.length(); i++)
	if (!isdigit(s[i]))
	    return false;
    return true;
}

int main(int argc, char *argv[]) {
    vector<string> docbuf;
    // col, row
    int c = 0;
    int r = 0;

    unordered_map<string, function<void()>> cmds = {
	// turn r and c to addresses later
	{"a", [&docbuf, &r, &c](){
	    string sub1 = docbuf[r].substr(0,c+1);
	    string sub2 = docbuf[r].substr(c+1, docbuf[r].length()-1);
	    string input;
	    string output;
	    while (getline(cin,input)) {
		if (input == ".")
		    break;
		else {
		    output += input;
		}
	    }
	    docbuf[r] = sub1 + output + sub2;
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
	{"i", [&docbuf, &r, &c](){
	    cout << "on row " << r << endl;
	    string sub1 = docbuf[r].substr(0,c);
	    string sub2 = docbuf[r].substr(c, docbuf[r].length()-1);
	    string input;
	    string output;
	    while (getline(cin,input)) {
		if (input == ".")
		    break;
		else {
		    output += input;
		}
	    }
	    docbuf[r] = sub1 + output + sub2;
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
	    r = stoul(tokens[0]) - 1;
	    cout << "cursor now on row " << r << endl;
	    cout << docbuf[r] << endl;
	}
    }
    
    return 0;
}
