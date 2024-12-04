#include <filesystem>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <Windows.h>

namespace fs = std::filesystem;

static double calculate_average_txt_file_size(const fs::path& directory) {
    std::vector<std::size_t> file_sizes;

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            try {
                file_sizes.push_back(fs::file_size(entry.path()));
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Error reading file size: " << e.what() << "\n";
            }
        }
    }

    if (file_sizes.empty()) {
        return 0.0;
    }

    std::size_t total_size = 0;
    for (const auto& size : file_sizes) {
        total_size += size;
    }

    return static_cast<double>(total_size) / file_sizes.size();
}

int main() {
    try {
        std::string input_path;

        std::cout << "Enter the directory path: ";
        std::getline(std::cin, input_path);

        fs::path directory(input_path);

        if (!fs::exists(directory)) {
            std::cerr << "Error: Directory does not exist.\n";
            return 1;
        }

        if (!fs::is_directory(directory)) {
            std::cerr << "Error: The path is not a directory.\n";
            return 1;
        }

        double average_size = calculate_average_txt_file_size(directory);

        if (average_size == 0.0) {
            std::cout << "No text files found in the directory.\n";
        }
        else {
            std::cout << "The average size of text files in the directory is: "
                << average_size << " bytes.\n";
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
