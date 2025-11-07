#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

vector<string> parseCommand(string input) {
    stringstream ss(input);
    string word;
    vector<string> tokens;
    while (ss >> word)
        tokens.push_back(word);
    return tokens;
}

int main() {
    string input;
    while (true) {
        cout << "MyShell> ";
        getline(cin, input);

        if (input == "exit")
            break;

        vector<string> tokens = parseCommand(input);
        cout << "You entered: ";
        for (auto &t : tokens)
            cout << t << " ";
        cout << endl;
    }
    return 0;
}
