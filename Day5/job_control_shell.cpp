#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <map>
#include <cstring>

using namespace std;

struct Job {
    int id;
    pid_t pid;
    string command;
    string status; // Running / Stopped
};

vector<Job> jobs;
int nextJobId = 1;

// Split input into tokens
vector<string> split(const string &input) {
    istringstream iss(input);
    vector<string> tokens;
    string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

// Update job statuses (remove finished ones)
void updateJobs() {
    for (auto it = jobs.begin(); it != jobs.end();) {
        int status;
        pid_t result = waitpid(it->pid, &status, WNOHANG);
        if (result == it->pid) {
            cout << "[Job " << it->id << " completed] " << it->command << endl;
            it = jobs.erase(it);
        } else {
            ++it;
        }
    }
}

// Show current background jobs
void listJobs() {
    updateJobs();
    if (jobs.empty()) {
        cout << "No active background jobs." << endl;
        return;
    }

    for (auto &job : jobs)
        cout << "[" << job.id << "] PID: " << job.pid
             << "  " << job.command << "  (" << job.status << ")\n";
}

// Bring a job to foreground
void bringToForeground(int jobId) {
    for (auto it = jobs.begin(); it != jobs.end(); ++it) {
        if (it->id == jobId) {
            cout << "Bringing job [" << jobId << "] to foreground: " << it->command << endl;
            int status;
            waitpid(it->pid, &status, 0);
            jobs.erase(it);
            return;
        }
    }
    cout << "No such job ID: " << jobId << endl;
}

// Resume a stopped job in background
void bringToBackground(int jobId) {
    for (auto &job : jobs) {
        if (job.id == jobId) {
            kill(job.pid, SIGCONT);
            job.status = "Running";
            cout << "Resumed job [" << job.id << "] in background: " << job.command << endl;
            return;
        }
    }
    cout << "No such job ID: " << jobId << endl;
}

// Execute a command
void executeCommand(vector<string> args, bool background) {
    vector<char*> argv;
    for (auto &arg : args)
        argv.push_back(const_cast<char*>(arg.c_str()));
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0) {
        if (background)
            setpgid(0, 0); // Create new process group for background job
        if (execvp(argv[0], argv.data()) == -1) {
            cerr << "Command not found: " << argv[0] << endl;
            exit(1);
        }
    } else if (pid > 0) {
        if (background) {
            jobs.push_back({nextJobId++, pid, args[0], "Running"});
            cout << "[Started background job] PID: " << pid << endl;
        } else {
            waitpid(pid, nullptr, 0);
        }
    } else {
        cerr << "Fork failed!" << endl;
    }
}

int main() {
    string input;

    cout << "==============================\n";
    cout << " Rahul's Shell (Day 5: Job Control)\n";
    cout << "==============================\n";
    cout << "Features:\n";
    cout << "1️⃣ Run background jobs using '&'\n";
    cout << "2️⃣ View all jobs using 'jobs'\n";
    cout << "3️⃣ Bring job to foreground using 'fg <job_id>'\n";
    cout << "4️⃣ Resume stopped job using 'bg <job_id>'\n";
    cout << "Type 'exit' to quit.\n\n";

    while (true) {
        cout << "shell> ";
        getline(cin, input);
        if (input.empty()) continue;

        if (input == "exit") break;

        updateJobs(); // Keep list clean

        vector<string> args = split(input);

        if (args[0] == "jobs") {
            listJobs();
        } else if (args[0] == "fg" && args.size() > 1) {
            bringToForeground(stoi(args[1]));
        } else if (args[0] == "bg" && args.size() > 1) {
            bringToBackground(stoi(args[1]));
        } else {
            bool background = false;
            if (args.back() == "&") {
                background = true;
                args.pop_back();
            }
            executeCommand(args, background);
        }
    }

    cout << "Exiting shell. Goodbye 👋\n";
    return 0;
}

