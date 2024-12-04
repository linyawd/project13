#include <filesystem>
#include <iostream>
#include <chrono>

namespace fs = std::filesystem;

static std::time_t to_time_t(std::filesystem::file_time_type fileTime)
{
    const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fileTime);
    const auto time = std::chrono::system_clock::to_time_t(systemTime);
    return time;
}

static void move_old_txt_files(const fs::path& source_dir, const fs::path& dest_dir) {
    fs::create_directories(dest_dir);
    auto now = std::chrono::system_clock::now();
    auto one_year_ago = now - std::chrono::hours(24 * 365);

    for (const auto& entry : fs::recursive_directory_iterator(source_dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            auto creation_time = fs::last_write_time(entry);
            if (to_time_t(creation_time) > std::chrono::system_clock::to_time_t(one_year_ago)) {
                fs::path new_path = dest_dir / entry.path().filename();
                fs::rename(entry.path(), new_path);
                std::cout << "Moved: " << entry.path() << " -> " << new_path << "\n";
            }
        }
    }
}

int main() {
    std::string source_dir, dest_dir;
    std::cout << "Enter source directory: ";
    std::getline(std::cin, source_dir);
    std::cout << "Enter destination directory: ";
    std::getline(std::cin, dest_dir);

    move_old_txt_files(source_dir, dest_dir);

    return 0;
}
