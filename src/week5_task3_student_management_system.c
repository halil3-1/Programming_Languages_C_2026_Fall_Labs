// week5_task3_student_management_system.c
// Name: Halil Ibrahim  Surname: Sert  Student ID: 260AMM011
//
// Task 3: Mini-project – Student management system with file persistence
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task3_student_management_system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <math.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
    char name[NAME_LEN];
    int id;
    float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls them directly)
int  load_students(Student arr[], int max);          // returns number of records loaded
int  save_students(const Student arr[], int count);  // 0 on success, -1 on error
void add_student(Student arr[], int *count);
void list_students(const Student arr[], int count);
int  read_line(const char *prompt, char *buf, size_t size);

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    char input[LINE_LEN];
    int running = 1;
    int status = EXIT_SUCCESS;

    // Load whatever was saved in a previous run (0 records on the first run).
    count = load_students(students, MAX_STUDENTS);
    printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

    while (running) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");

        int choice;
        if (read_line("Select an option: ", input, sizeof input) != 0) {
            choice = 3;                         // end of input: save and exit instead of looping forever
        } else {
            char *end;
            long value = strtol(input, &end, 10);
            choice = (end != input && *end == '\0') ? (int)value : 0;   // non-numbers become invalid
        }

        switch (choice) {
            case 1:
                list_students(students, count);
                break;
            case 2:
                add_student(students, &count);
                break;
            case 3:
                // Persist everything before leaving; report honestly if it fails.
                if (save_students(students, count) != 0) {
                    perror("Error: could not save " DATA_FILE);
                    status = EXIT_FAILURE;
                } else {
                    printf("Data saved. Goodbye!\n");
                }
                running = 0;
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return status;
}

// Provided helper – prints a prompt and reads one line from stdin into buf.
// The trailing newline is removed. Returns 0 on success, -1 on end of input (EOF).
int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buf, (int)size, stdin) == NULL) {
        return -1;
    }
    size_t len = strcspn(buf, "\r\n");
    if (buf[len] == '\0' && len == size - 1) {
        // Line was longer than the buffer: discard the rest of it.
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    buf[len] = '\0';
    return 0;
}

// Loads up to 'max' records from DATA_FILE in the format "name,id,gpa".
// A missing file is not an error (first run): it simply loads 0 records.
// Blank or malformed lines are skipped. Returns the number of records loaded.
int load_students(Student arr[], int max) {
    if (arr == NULL || max <= 0) {
        return 0;
    }

    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        return 0;
    }

    char line[LINE_LEN];
    int count = 0;

    // Stop at end of file OR when the array is full, so it can never overflow.
    while (count < max && fgets(line, sizeof line, fp) != NULL) {
        // If the line did not fit in the buffer, discard the rest of it so the
        // leftover part is not mistaken for a new record.
        if (strchr(line, '\n') == NULL && !feof(fp)) {
            int c;
            while ((c = fgetc(fp)) != '\n' && c != EOF) { }
        }

        // %49[^,] reads up to 49 characters that are not commas, so names may
        // contain spaces. Only lines where all 3 fields were parsed are kept;
        // a blank line returns fewer than 3 and is skipped.
        Student tmp = {0};
        if (sscanf(line, "%49[^,],%d,%f", tmp.name, &tmp.id, &tmp.gpa) == 3) {
            arr[count] = tmp;
            count++;
        }
    }

    fclose(fp);
    return count;
}

// Overwrites DATA_FILE with all current records, one per line.
// Returns 0 on success, -1 if the file cannot be opened or written.
int save_students(const Student arr[], int count) {
    if (arr == NULL && count > 0) {
        return -1;
    }

    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        return -1;
    }

    int ok = 1;
    for (int i = 0; i < count; i++) {
        if (fprintf(fp, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa) < 0) {
            ok = 0;
        }
    }

    // fclose() flushes buffered data, so a failure here also means data loss.
    if (fclose(fp) != 0) {
        ok = 0;
    }
    return ok ? 0 : -1;
}

// Asks for name, ID and GPA, validates them, and appends a new record.
void add_student(Student arr[], int *count) {
    if (arr == NULL || count == NULL) {
        return;
    }

    // Check capacity first so the user is not asked for data that cannot be stored.
    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students (limit reached).\n");
        return;
    }

    char name_buf[LINE_LEN];
    char id_buf[LINE_LEN];
    char gpa_buf[LINE_LEN];

    // read_line() returns -1 at end of input: stop quietly, main() will exit.
    if (read_line("Enter name: ", name_buf, sizeof name_buf) != 0) return;
    if (read_line("Enter ID: ", id_buf, sizeof id_buf) != 0) return;
    if (read_line("Enter GPA: ", gpa_buf, sizeof gpa_buf) != 0) return;

    // Name: must not be empty (or only spaces) and must not contain a comma,
    // because the comma is the field separator in students.txt.
    int blank = 1;
    for (const char *p = name_buf; *p != '\0'; p++) {
        if (*p != ' ' && *p != '\t') {
            blank = 0;
            break;
        }
    }
    if (blank || strchr(name_buf, ',') != NULL) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // ID: strtol must consume the whole string (no "12abc") and fit in an int.
    char *end;
    errno = 0;
    long id = strtol(id_buf, &end, 10);
    if (end == id_buf || *end != '\0' || errno == ERANGE ||
        id < INT_MIN || id > INT_MAX) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // GPA: must be a complete number in the range 0.0–4.0.
    // isfinite() also rejects "nan" and "inf", which strtof accepts.
    errno = 0;
    float gpa = strtof(gpa_buf, &end);
    if (end == gpa_buf || *end != '\0' || errno == ERANGE ||
        !isfinite(gpa) || gpa < 0.0f || gpa > 4.0f) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // All checks passed: store the record. A very long name is truncated
    // to NAME_LEN-1 characters so it always fits, and '\0' is added.
    Student *s = &arr[*count];
    size_t len = strlen(name_buf);
    if (len >= sizeof s->name) {
        len = sizeof s->name - 1;
    }
    memcpy(s->name, name_buf, len);
    s->name[len] = '\0';
    s->id = (int)id;
    s->gpa = gpa;
    (*count)++;

    printf("Student added successfully!\n");
}

// Prints all students as a table, or a message if the list is empty.
void list_students(const Student arr[], int count) {
    if (arr == NULL || count <= 0) {
        printf("No students found.\n");
        return;
    }

    printf("%-5s %-20s %s\n", "ID", "Name", "GPA");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
    }
}