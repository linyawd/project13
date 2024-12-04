#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define MAX_PATH 1024

static void process_directory(const char* path, void (*file_handler)(const char*)) {
    struct dirent* entry;
    DIR* dp = opendir(path);

    if (!dp) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s/%s", path, entry->d_name);

        struct stat sb;
        if (stat(full_path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            process_directory(full_path, file_handler);
        }
        else {
            file_handler(full_path);
        }
    }

    closedir(dp);
}

// Task 1
static void change_c_to_cpp(const char* file_path) {
    if (strstr(file_path, ".c") && !strstr(file_path, ".cpp")) {
        char new_path[MAX_PATH];
        snprintf(new_path, MAX_PATH, "%.*s.cpp", (int)(strlen(file_path) - 2), file_path);
        if (rename(file_path, new_path) == 0) {
            printf("Renamed: %s -> %s\n", file_path, new_path);
        }
        else {
            perror("rename");
        }
    }
}

// Task 2
static void replace_single_line_comments(const char* file_path) {
    if (strstr(file_path, ".cpp")) {
        FILE* file = fopen(file_path, "r");
        if (!file) {
            perror("fopen");
            return;
        }

        char temp_path[MAX_PATH];
        snprintf(temp_path, MAX_PATH, "%s.tmp", file_path);

        FILE* temp_file = fopen(temp_path, "w");
        if (!temp_file) {
            perror("fopen");
            fclose(file);
            return;
        }

        char line[MAX_PATH];
        while (fgets(line, MAX_PATH, file)) {
            char* comment_pos = strstr(line, "//");
            if (comment_pos) {
                *comment_pos = '\0';
                fprintf(temp_file, "%s/* %s */\n", line, comment_pos + 2);
            }
            else {
                fputs(line, temp_file);
            }
        }

        fclose(file);
        fclose(temp_file);

        if (rename(temp_path, file_path) != 0) {
            perror("rename");
        }
    }
}

// Task 3
static void delete_txt_older_than_date(const char* file_path, time_t cutoff_date) {
    if (strstr(file_path, ".txt")) {
        struct stat sb;
        if (stat(file_path, &sb) == 0 && sb.st_mtime < cutoff_date) {
            if (remove(file_path) == 0) {
                printf("Deleted: %s\n", file_path);
            }
            else {
                perror("remove");
            }
        }
    }
}

// Task 4
static void move_txt_older_than_year(const char* file_path, const char* target_dir, time_t cutoff_date) {
    if (strstr(file_path, ".txt")) {
        struct stat sb;
        if (stat(file_path, &sb) == 0 && sb.st_ctime < cutoff_date) {
            char new_path[MAX_PATH];
            snprintf(new_path, MAX_PATH, "%s/%s", target_dir, strrchr(file_path, '/') + 1);
            if (rename(file_path, new_path) == 0) {
                printf("Moved: %s -> %s\n", file_path, new_path);
            }
            else {
                perror("rename");
            }
        }
    }
}

// Task 5
static void delete_word_files_small(const char* file_path) {
    if (strstr(file_path, ".doc") || strstr(file_path, ".docx")) {
        struct stat sb;
        if (stat(file_path, &sb) == 0 && sb.st_size < 100 * 1024) {
            if (remove(file_path) == 0) {
                printf("Deleted: %s\n", file_path);
            }
            else {
                perror("remove");
            }
        }
    }
}

// Task 6
static void calculate_average_txt_size(const char* path) {
    struct dirent* entry;
    DIR* dp = opendir(path);
    if (!dp) {
        perror("opendir");
        return;
    }

    size_t total_size = 0, file_count = 0;

    while ((entry = readdir(dp))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s/%s", path, entry->d_name);

        struct stat sb;
        if (stat(full_path, &sb) == 0 && S_ISREG(sb.st_mode) && strstr(entry->d_name, ".txt")) {
            total_size += sb.st_size;
            file_count++;
        }
    }

    closedir(dp);

    if (file_count > 0) {
        printf("Average size: %zu bytes\n", total_size / file_count);
    }
    else {
        printf("No text files found.\n");
    }
}

static int is_directory_in_path(const char* dir_path) {
    char* path_env = getenv("PATH");
    if (path_env == NULL) {
        printf("No PATH variable found.\n");
        return 0;
    }

    char* path_copy = strdup(path_env);
    char* token = strtok(path_copy, ":");
    while (token) {
        if (strcmp(token, dir_path) == 0) {
            free(path_copy);
            return 1;
        }
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return 0;
}

int main() {
    char input_path[MAX_PATH];

    printf("Enter directory path: ");
    fgets(input_path, MAX_PATH, stdin);
    input_path[strcspn(input_path, "\n")] = 0;

    if (is_directory_in_path(input_path)) {
        printf("Directory is in PATH.\n");
    }
    else {
        printf("Directory is not in PATH.\n");
    }

    time_t cutoff_date = time(NULL) - 365 * 24 * 60 * 60;
    const char* target_dir = "./target_directory";

    //mkdir(target_dir, 0755);
    CreateDirectoryA(target_dir, NULL); // can't find mkdir function so will use this instead

    printf("Task 1:\n");
    process_directory(input_path, change_c_to_cpp);

    printf("Task 2:\n");
    process_directory(input_path, replace_single_line_comments);

    printf("Task 3:\n");
    process_directory(input_path, (void (*)(const char*))delete_txt_older_than_date);

    printf("Task 4:\n");
    process_directory(input_path, (void (*)(const char*))move_txt_older_than_year);

    printf("Task 5:\n");
    process_directory(input_path, delete_word_files_small);

    printf("Task 6:\n");
    calculate_average_txt_size(input_path);

    return 0;
}
