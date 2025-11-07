#include <iostream>
#include <vector>
#include <sstream>
#include <string>

int main() {
    std::vector<std::string> history;
    std::string input;

    std::cout << "Welcome to Rahul's Shell (Day 2)\n";
    std::cout << "Type 'exit' to quit.\n\n";

    while (true) {
        std::cout << "shell> ";
        std::getline(std::cin, input);

        if (input.empty()) continue;

        // Add to history
        history.push_back(input);

        if (input == "exit") {
            std::cout << "Goodbye!\n";
            break;
        } else if (input == "history") {
            std::cout << "\nCommand History:\n";
            for (size_t i = 0; i < history.size(); ++i)
                std::cout << i + 1 << ": " << history[i] << "\n";
            std::cout << "\n";
        } else {
            std::cout << "You typed: " << input << "\n";
        }
    }
    return 0;
}
