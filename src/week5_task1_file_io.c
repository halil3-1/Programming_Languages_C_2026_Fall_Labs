// week5_task1_file_io.c
// Name: Halil Ibrahim  Surname: Sert  Student ID: 260AMM011
//
// Task 1: Read and write data from text files
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task1_file_io [filename]

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_FILENAME "data.txt"
#define LINE_LEN 256

int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    const char *filename = DEFAULT_FILENAME;
    char line[LINE_LEN];
    int line_count = 0;

    // BONUS: an optional command-line argument overrides the default file name.
    if (argc > 1) {
        filename = argv[1];
    }

    // 1. Open the file for writing. Mode "w" creates the file, or truncates
    //    it if it already exists, so every run starts from a clean file.
    printf("Writing lines to %s...\n", filename);
    fp = fopen(filename, "w");

    // 2. fopen() returns NULL on failure (no permission, bad path, ...).
    //    perror() prints the reason stored in errno.
    if (fp == NULL) {
        perror(filename);
        return EXIT_FAILURE;
    }

    // 3. fprintf() works like printf() but writes to the given stream.
    fprintf(fp, "Hello, file I/O in C!\n");
    fprintf(fp, "This is another line.\n");

    // 4. fclose() flushes the buffered data to disk and releases the handle.
    //    It can fail (e.g. disk full), so its result is checked too.
    if (fclose(fp) != 0) {
        perror(filename);
        return EXIT_FAILURE;
    }
    fp = NULL;

    // 5. Re-open the same file, this time for reading.
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        return EXIT_FAILURE;
    }

    // 6. fgets() reads at most LINE_LEN-1 characters, stops after '\n',
    //    and returns NULL at end of file, which ends the loop.
    //    The line keeps its '\n', so it is printed with "%s" only.
    printf("Reading contents:\n");
    while (fgets(line, sizeof line, fp) != NULL) {
        printf("%s", line);
        line_count++;   // BONUS: count the lines read
    }

    // 7. Close the file after reading as well.
    fclose(fp);
    fp = NULL;

    // BONUS: report how many lines were read.
    printf("Lines read: %d\n", line_count);

    return EXIT_SUCCESS;
}