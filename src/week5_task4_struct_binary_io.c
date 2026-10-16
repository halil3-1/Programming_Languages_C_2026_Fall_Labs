// week5_task4_struct_binary_io.c   (BONUS)
// Name: Halil Ibrahim  Surname: Sert  Student ID: 260AMM011
//
// Task 4: Save and load a struct in binary format with fwrite()/fread()
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task4_struct_binary_io

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

typedef struct Student {
    char name[MAX_NAME_LEN];
    int age;
    float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls them directly)
int  save_student_binary(const Student *s, const char *filename);  // 0 on success, -1 on error
int  load_student_binary(const char *filename, Student *out);      // 0 on success, -1 on error
long file_size(const char *filename);                              // bytes, -1 on error

// Helper: the same text format as Task 2 ("name age gpa"), used for the size comparison.
static int save_student_text(const Student *s, const char *filename);

int main(void) {
    Student s1 = {0};          // zero-initialised so unused name bytes are not garbage on disk
    strcpy(s1.name, "Alice");
    s1.age = 21;
    s1.gpa = 3.75f;

    Student loaded = {0};
    const char *bin_file = "student.bin";
    const char *txt_file = "student.txt";

    printf("Saving student in binary format...\n");
    if (save_student_binary(&s1, bin_file) != 0) {
        fprintf(stderr, "Error: could not save student to %s\n", bin_file);
        return EXIT_FAILURE;
    }

    // Same record as text, so the two storage formats can be compared.
    if (save_student_text(&s1, txt_file) != 0) {
        fprintf(stderr, "Error: could not save student to %s\n", txt_file);
        return EXIT_FAILURE;
    }

    printf("Loading student from binary file...\n");
    if (load_student_binary(bin_file, &loaded) != 0) {
        fprintf(stderr, "Error: could not load student from %s\n", bin_file);
        return EXIT_FAILURE;
    }

    printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age, loaded.gpa);

    // Text stores only the characters needed ("Alice 21 3.75\n" = 14 bytes);
    // binary stores the whole struct: 50-byte name + padding + int + float.
    printf("Text file size: %ld bytes\n", file_size(txt_file));
    printf("Binary file size: %ld bytes\n", file_size(bin_file));

    return EXIT_SUCCESS;
}

// Writes the struct's raw bytes. "wb" avoids any newline translation
// (which matters on Windows). fwrite() returns the number of whole
// items written, so anything other than 1 means the write failed.
int save_student_binary(const Student *s, const char *filename) {
    if (s == NULL || filename == NULL) {
        return -1;
    }

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return -1;
    }

    size_t written = fwrite(s, sizeof(Student), 1, fp);

    if (fclose(fp) != 0 || written != 1) {
        return -1;
    }
    return 0;
}

// Reads exactly one struct back. fread() returns 1 only if all
// sizeof(Student) bytes were read; a short or empty file gives 0.
int load_student_binary(const char *filename, Student *out) {
    if (filename == NULL || out == NULL) {
        return -1;
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return -1;
    }

    Student tmp;                // *out is only changed after a full, valid read
    size_t got = fread(&tmp, sizeof(Student), 1, fp);
    fclose(fp);

    if (got != 1) {
        return -1;
    }

    // Defensive: guarantee the name is terminated even if the file was corrupted.
    tmp.name[MAX_NAME_LEN - 1] = '\0';
    *out = tmp;
    return 0;
}

// Size in bytes: seek to the end and ask for the current position.
long file_size(const char *filename) {
    if (filename == NULL) {
        return -1;
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return -1;
    }

    long size = -1;
    if (fseek(fp, 0, SEEK_END) == 0) {
        size = ftell(fp);       // ftell() itself returns -1 on error
    }

    fclose(fp);
    return size;
}

static int save_student_text(const Student *s, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return -1;
    }

    int written = fprintf(fp, "%s %d %.2f\n", s->name, s->age, s->gpa);

    if (fclose(fp) != 0 || written < 0) {
        return -1;
    }
    return 0;
}