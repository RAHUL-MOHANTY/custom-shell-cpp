# 🧠 Custom Shell in C++ (Capstone Project)

This repository contains a **day-wise implementation** of a simple Linux shell built using **C++**.  
Each day introduces a new concept — from basic command parsing to job control —  
resulting in a complete, working shell by the end of Day 5.

---

## 📅 Day-wise Progress

| Day | Task | File |
|-----|------|------|
| **1** | Parse user input and show prompt | `/Day1/day1_shell_basic.cpp` |
| **2** | Execute basic system commands | `/Day2/day2_command_exec.cpp` |
| **3** | Add process management (foreground/background execution) | `/Day3/process_mgmt_shell.cpp` |
| **4** | Implement piping and redirection features | `/Day4/piping_redirection_shell.cpp` |
| **5** | Incorporate job control (list jobs, fg/bg commands) | `/Day5/job_control_shell.cpp` |

---

## ⚙️ Features Implemented

✅ Custom command prompt  
✅ Foreground & background process management  
✅ Input/Output redirection (`<`, `>`)  
✅ Piping (`|`)  
✅ Job control (`jobs`, `fg`, `bg`)  
✅ Command history (optional in Day 5 extension)

---

## 🚀 How to Run

```bash
# Clone repository
git clone https://github.com/RAHUL-MOHANTY/custom-shell-cpp.git
cd custom-shell-cpp

# Compile any day's code (example: Day 5)
g++ Day5/job_control_shell.cpp -o shell
./shell
