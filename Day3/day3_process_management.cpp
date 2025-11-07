#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    string input;
    vector<string> history;

    cout << "Welcome to Rahul's Shell (Day 3 - Process Management)\n";
    cout << "Supports foreground and background processes.\n";
    cout << "Use '&' at the end to run in background.\n";
    cout << "Type 'exit' to quit.\n\n";

    while (true) {
        cout << "shell> ";
        getline(cin, input);

        if (input.empty()) continue;
        history.push_back(input);

        if (input == "exit") {
            cout << "Goodbye!\n";
            break;
        }

        // Check for background process (&)
        bool runInBackground = false;
        if (input.back() == '&') {
            runInBackground = true;
            input.pop_back(); // remove '&'
            if (!input.empty() && input.back() == ' ') input.pop_back();
        }

        // Tokenize input
        istringstream iss(input);
        vector<string> args;
        string word;
        while (iss >> word) args.push_back(word);

        // Convert to char* array for execvp
        vector<char*> argv;
        for (auto &arg : args) argv.push_back(const_cast<char*>(arg.c_str()));
        argv.push_back(nullptr);

        // Fork a new process
        pid_t pid = fork();

        if (pid < 0) {
            cerr << "Fork failed!\n";
        } 
        else if (pid == 0) {
            // Child process: execute command
            if (execvp(argv[0], argv.data()) == -1) {
                cerr << "Command not found: " << argv[0] << endl;
            }
            exit(1);
        } 
        else {
            // Parent process: wait if foreground
            if (!runInBackground)
                waitpid(pid, nullptr, 0);
            else
                cout << "[Running in background] PID: " << pid << endl;
        }
    }

    return 0;
}
