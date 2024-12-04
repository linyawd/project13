#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

static void delete_small_word_files(const fs::path& directory) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            auto ext = entry.path().extension().string();
            if ((ext == ".doc" || ext == ".docx") && fs::file_size(entry) < 100 * 1024) {
                fs::remove(entry.path());
                std::cout << "Deleted: " << entry.path() << "\n";
            }
        }
    }
}

int main() {
    std::string directory;
    std::cout << "Enter directory: ";
    std::getline(std::cin, directory);

    delete_small_word_files(directory);

    return 0;
}
