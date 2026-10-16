// week5_task2_struct_save_load.c
// Name: Halil Ibrahim  Surname: Sert  Student ID: 260AMM011
//
// Task 2: Save and load structured records from a file
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task2_struct_save_load

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
int save_student(const Student *s, const char *filename);  // 0 on success, -1 on error
int load_student(const char *filename, Student *out);      // 0 on success, -1 on error

int main(void) {
    Student s1 = {0};
    strcpy(s1.name, "Alice");
    s1.age = 21;
    s1.gpa = 3.75f;

    Student loaded = {0};
    const char *filename = "student.txt";

    // The functions only report success/failure; main() decides what to do.
    printf("Saving student to file...\n");
    if (save_student(&s1, filename) != 0) {
        fprintf(stderr, "Error: could not save student to %s\n", filename);
        return EXIT_FAILURE;
    }

    printf("Loading student from file...\n");
    if (load_student(filename, &loaded) != 0) {
        fprintf(stderr, "Error: could not load student from %s\n", filename);
        return EXIT_FAILURE;
    }

    printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age, loaded.gpa);
    return EXIT_SUCCESS;
}

// Writes the record as one text line: "name age gpa", e.g. "Alice 21 3.75".
// Returns 0 on success, -1 if the file cannot be opened or written.
int save_student(const Student *s, const char *filename) {
    if (s == NULL || filename == NULL) {
        return -1;
    }

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return -1;              // no exit() here: the caller handles the error
    }

    // fprintf() returns a negative value if the write fails.
    int written = fprintf(fp, "%s %d %.2f\n", s->name, s->age, s->gpa);

    // fclose() flushes the buffer; a failure here also means data was lost.
    if (fclose(fp) != 0 || written < 0) {
        return -1;
    }
    return 0;
}

// Reads one "name age gpa" record into *out.
// Returns 0 on success, -1 if the file cannot be opened or the data is invalid.
int load_student(const char *filename, Student *out) {
    if (filename == NULL || out == NULL) {
        return -1;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return -1;
    }

    // Read into a temporary so *out is untouched if parsing fails.
    // %49s limits the name to 49 chars + '\0', so name[50] cannot overflow.
    Student tmp = {0};
    int fields = fscanf(fp, "%49s %d %f", tmp.name, &tmp.age, &tmp.gpa);

    fclose(fp);                 // close in every case, success or failure

    // fscanf() returns the number of fields converted (or EOF);
    // all three must be present for the record to be valid.
    if (fields != 3) {
        return -1;
    }

    *out = tmp;
    return 0;
}