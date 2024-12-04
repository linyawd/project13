#include <filesystem>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

std::chrono::system_clock::time_point parse_date(const std::string& date_str) {
    std::tm tm = {};
    std::istringstream ss(date_str);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail()) {
        throw std::runtime_error("Invalid date format. Use YYYY-MM-DD.");
    }
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::chrono::system_clock::time_point to_system_clock(const fs::file_time_type& file_time) {
    return std::chrono::clock_cast<std::chrono::system_clock>(file_time);
}

void delete_txt_files_before_date(const fs::path& directory, const std::chrono::system_clock::time_point& target_date) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            auto last_write_time = fs::last_write_time(entry);
            auto file_time = to_system_clock(last_write_time);

            if (file_time < target_date) {
                fs::remove(entry.path());
                std::cout << "Deleted: " << entry.path() << "\n";
            }
        }
    }
}

int main() {
    try {
        std::string directory, date_str;

        std::cout << "Enter directory: ";
        std::getline(std::cin, directory);

        std::cout << "Enter date (YYYY-MM-DD): ";
        std::getline(std::cin, date_str);

        auto target_date = parse_date(date_str);

        delete_txt_files_before_date(directory, target_date);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
