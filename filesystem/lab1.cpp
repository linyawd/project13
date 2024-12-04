#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

static void replace_extension(const fs::path& directory) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".c") {
            fs::path new_path = entry.path();
            new_path.replace_extension(".cpp");
            fs::rename(entry.path(), new_path);
            std::cout << "Renamed: " << entry.path() << " -> " << new_path << "\n";
        }
    }
}

int main() {
    std::string directory;
    std::cout << "Enter directory: ";
    std::getline(std::cin, directory);

    replace_extension(directory);

    return 0;
}
