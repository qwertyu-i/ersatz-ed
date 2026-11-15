#include <iostream>
// maybe making our own fstream could be a
// fun project 
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <functional>

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

int main(int argc, char *argv[]) {
    vector<string> docbuf;
    // col, row
    int c = 0;
    int r = 0;

    unordered_map<string, function<void()>> cmds = {
	// turn r and c to addresses later
	{"a", [&docbuf, r, c](){
	    string sub1 = docbuf[r].substr(0,c+1);
	    string sub2 = docbuf[r].substr(c+1, docbuf[r].length()-1);
	    docbuf[r] = sub1 + "a" + sub2;
	}},
	// note for future: to get anything from docbuf you need to pass it
	// by reference to lambda functions
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
	}}
    };

    ifstream in(argv[argc-1]);
    
    if (!in.is_open()) {
	cerr << "no";
	return 1;
    }
    
    int i = 0;
    string line;
    while (getline(in, line)) {
	docbuf.push_back(line);
	i++;
    }

    in.close();

    vector<string> test1 = { "a" };
    execcmd(cmds, 1, test1);

    vector<string> test2 = { "w" };
    execcmd(cmds, 1, test2);

    return 0;
}
