#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

int main() {
    string input;
    cout << "Welcome to Rahul's Shell (Day 2 - Execute Commands)\n";
    cout << "Type 'exit' to quit.\n\n";

    while (true) {
        cout << "shell> ";
        getline(cin, input);
        if (input.empty()) continue;

        if (input == "exit") {
            cout << "Goodbye!\n";
            break;
        }

        // Execute system command
        int status = system(input.c_str());
        if (status == -1)
            cout << "Error executing command.\n";
    }
    return 0;
}

