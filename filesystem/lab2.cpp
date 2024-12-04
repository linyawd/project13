#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

namespace fs = std::filesystem;

void replace_comments(const fs::path& directory) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
            std::ifstream file(entry.path());
            if (!file) {
                std::cerr << "Could not open file: " << entry.path() << "\n";
                continue;
            }
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();

            std::regex comment_regex(R"(//(.*))");
            std::string new_content = std::regex_replace(content, comment_regex, "/*$1 */");

            std::ofstream out_file(entry.path());
            if (!out_file) {
                std::cerr << "Could not write to file: " << entry.path() << "\n";
                continue;
            }
            out_file << new_content;
            out_file.close();

            std::cout << "Processed: " << entry.path() << "\n";
        }
    }
}

int main() {
    std::string directory;
    std::cout << "Enter directory: ";
    std::getline(std::cin, directory);

    replace_comments(directory);

    return 0;
}
