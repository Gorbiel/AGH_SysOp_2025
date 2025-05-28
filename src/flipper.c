#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h> // For opendir and readdir
#include <string.h> // For strdup
#include <sys/stat.h> // For stat and mkdir

char **get_file_names(const char *directory, size_t *num_files) {
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Error opening directory");
        return NULL;
    }

    struct dirent *entry;
    char **file_names = NULL;
    *num_files = 0;

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Reallocate memory to store the file name
        file_names = realloc(file_names, (*num_files + 1) * sizeof(char *));
        if (file_names == NULL) {
            perror("Error allocating memory");
            closedir(dir);
            return NULL;
        }

        // Store a copy of the file name
        file_names[*num_files] = strdup(entry->d_name);
        if (file_names[*num_files] == NULL) {
            perror("Error duplicating file name");
            closedir(dir);
            return NULL;
        }

        (*num_files)++;
    }

    closedir(dir);
    return file_names;
}

void free_file_names(char **file_names, size_t num_files) {
    for (size_t i = 0; i < num_files; i++) {
        free(file_names[i]);
    }
    free(file_names);
}

unsigned long calculate_byte_sum(const char *line) {
    if (line == NULL) {
        return 0;
    }

    unsigned long sum = 0;
    for (size_t i = 0; line[i] != '\0'; i++) {
        sum += (unsigned char)line[i]; // Cast to unsigned char to handle all byte values
    }

    return sum;
}

void read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Extract the base filename from the full path
    const char *base_filename = strrchr(filename, '/');
    base_filename = (base_filename == NULL) ? filename : base_filename + 1;

    // Create the output file paths
    char output_filepath_1[256];
    snprintf(output_filepath_1, sizeof(output_filepath_1), "reversed/%s_reversed", base_filename);

    char output_filepath_control_1[256];
    snprintf(output_filepath_control_1, sizeof(output_filepath_control_1), "reversed/%s_reversed_control", base_filename);

    char output_filepath_2[256];
    snprintf(output_filepath_2, sizeof(output_filepath_2), "restored/%s", base_filename);

    char output_filepath_control_2[256];
    snprintf(output_filepath_control_2, sizeof(output_filepath_control_2), "restored/%s_restored_control", base_filename);

    // Open the output files for writing in binary mode
    FILE *output_file_1 = fopen(output_filepath_1, "wb");
    if (output_file_1 == NULL) {
        perror("Error creating output file");
        fclose(file);
        return;
    }

    FILE *output_file_2 = fopen(output_filepath_2, "wb");
    if (output_file_2 == NULL) {
        perror("Error creating output file");
        fclose(file);
        fclose(output_file_1);
        return;
    }

    FILE *control_file_1 = fopen(output_filepath_control_1, "w");
    if (control_file_1 == NULL) {
        perror("Error creating control file");
        fclose(file);
        fclose(output_file_1);
        fclose(output_file_2);
        return;
    }

    FILE *control_file_2 = fopen(output_filepath_control_2, "w");
    if (control_file_2 == NULL) {
        perror("Error creating control file");
        fclose(file);
        fclose(output_file_1);
        fclose(output_file_2);
        fclose(control_file_1);
        return;
    }

    // Read the entire file into memory
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = malloc(file_size);
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(file);
        fclose(output_file_1);
        fclose(output_file_2);
        fclose(control_file_1);
        fclose(control_file_2);
        return;
    }

    fread(buffer, 1, file_size, file);

    // Calculate the control sum of the original file
    unsigned long control_sum_1 = 0;
    for (long i = 0; i < file_size; i++) {
        control_sum_1 += buffer[i];
    }

    // Reverse the buffer
    for (long i = 0; i < file_size / 2; i++) {
        unsigned char temp = buffer[i];
        buffer[i] = buffer[file_size - i - 1];
        buffer[file_size - i - 1] = temp;
    }

    // Write the reversed buffer to the first output file
    fwrite(buffer, 1, file_size, output_file_1);

    // Calculate the control sum of the reversed file
    unsigned long control_sum_2 = 0;
    for (long i = 0; i < file_size; i++) {
        control_sum_2 += buffer[i];
    }

    // Reverse the buffer again to get the original content
    for (long i = 0; i < file_size / 2; i++) {
        unsigned char temp = buffer[i];
        buffer[i] = buffer[file_size - i - 1];
        buffer[file_size - i - 1] = temp;
    }

    // Write the original buffer to the second output file
    fwrite(buffer, 1, file_size, output_file_2);

    // Write control sums to control files
    fprintf(control_file_1, "%lu\n", control_sum_1);
    fprintf(control_file_2, "%lu\n", control_sum_2);

    free(buffer);
    fclose(file);
    fclose(output_file_1);
    fclose(output_file_2);
    fclose(control_file_1);
    fclose(control_file_2);

    printf("Reversed file written to: %s\n", output_filepath_1);
    printf("Control sum for file: %s, written to %s\n", output_filepath_1, output_filepath_control_1);
    printf("Reversed (twice) file written to: %s\n", output_filepath_2);
    printf("Control sum for file: %s, written to %s\n", output_filepath_2, output_filepath_control_2);
    if (control_sum_1 == control_sum_2) {
        printf("Control sums are equal\n");
    } else {
        printf("Control sums are not equal\n");
    }
    printf("\n");
}

int main() {
    const char *directory = "resources/art/";
    const char *output_directory = "reversed/";
    const char *output_directory_2 = "restored/";
    size_t num_files = 0;

    // Check if the output directory exists, and create it if it doesn't
    struct stat st;
    if (stat(output_directory, &st) == -1) {
        if (mkdir(output_directory, 0755) == -1) {
            perror("Error creating output directory");
            return 1;
        }
        printf("Created directory: %s\n", output_directory);
    } else {
        printf("Directory already exists: %s\n", output_directory);
    }
    
    if (stat(output_directory_2, &st) == -1) {
        if (mkdir(output_directory_2, 0755) == -1) {
            perror("Error creating output directory");
            return 1;
        }
        printf("Created directory: %s\n", output_directory_2);
    } else {
        printf("Directory already exists: %s\n", output_directory_2);
    }

    printf("\n");

    // Get file names from the directory
    char **files = get_file_names(directory, &num_files);
    if (files == NULL) {
        return 1;
    }

    for (size_t i = 0; i < num_files; i++) {
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "%s%s", directory, files[i]);
        read_file(filepath);
    }

    // Free allocated memory for file names
    free_file_names(files, num_files);

    // Path to the shell script
    const char *script_path = "./test/md5checker.sh";

    // Run the shell script
    int result = system(script_path);

    // Check the result
    if (result == -1) {
        perror("Error executing script");
        return 1;
    } else {
        printf("Script executed successfully\n");
    }

    return 0;
}