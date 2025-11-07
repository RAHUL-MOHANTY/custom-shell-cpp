#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

using namespace std;

// Helper: split string by spaces
vector<string> split(const string &input) {
    istringstream iss(input);
    vector<string> tokens;
    string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

// Helper: execute a single command (with redirection)
void executeCommand(vector<string> args) {
    int in_fd = -1, out_fd = -1;
    vector<char*> argv;
    
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == "<" && i + 1 < args.size()) {
            in_fd = open(args[i + 1].c_str(), O_RDONLY);
            args.erase(args.begin() + i, args.begin() + i + 2);
            i--;
        } 
        else if (args[i] == ">" && i + 1 < args.size()) {
            out_fd = open(args[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            args.erase(args.begin() + i, args.begin() + i + 2);
            i--;
        }
    }

    for (auto &arg : args)
        argv.push_back(const_cast<char*>(arg.c_str()));
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid == 0) {
        if (in_fd != -1) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != -1) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }

        if (execvp(argv[0], argv.data()) == -1) {
            cerr << "Command not found: " << argv[0] << endl;
        }
        exit(1);
    } else {
        waitpid(pid, nullptr, 0);
    }
}

// Helper: execute piped commands
void executePipedCommands(vector<string> commands) {
    int numPipes = commands.size() - 1;
    int pipefds[2 * numPipes];

    // Create all pipes
    for (int i = 0; i < numPipes; i++)
        pipe(pipefds + i * 2);

    int j = 0;
    for (int i = 0; i < commands.size(); i++) {
        vector<string> args = split(commands[i]);
        vector<char*> argv;
        for (auto &arg : args) argv.push_back(const_cast<char*>(arg.c_str()));
        argv.push_back(nullptr);

        pid_t pid = fork();
        if (pid == 0) {
            if (i != 0)
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            if (i != numPipes)
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);

            for (int k = 0; k < 2 * numPipes; k++)
                close(pipefds[k]);

            if (execvp(argv[0], argv.data()) == -1)
                cerr << "Command not found: " << argv[0] << endl;

            exit(1);
        }
    }

    // Close all pipes and wait
    for (int i = 0; i < 2 * numPipes; i++)
        close(pipefds[i]);

    for (int i = 0; i < commands.size(); i++)
        wait(nullptr);
}

int main() {
    string input;

    cout << "Welcome to Rahul's Shell (Day 4 - Piping & Redirection)\n";
    cout << "Supports '|', '>', '<'\n";
    cout << "Type 'exit' to quit.\n\n";

    while (true) {
        cout << "shell> ";
        getline(cin, input);

        if (input.empty()) continue;
        if (input == "exit") {
            cout << "Goodbye!\n";
            break;
        }

        // Check for pipes
        if (input.find('|') != string::npos) {
            vector<string> commands;
            stringstream ss(input);
            string segment;
            while (getline(ss, segment, '|'))
                commands.push_back(segment);
            executePipedCommands(commands);
        } else {
            executeCommand(split(input));
        }
    }
    return 0;
}
