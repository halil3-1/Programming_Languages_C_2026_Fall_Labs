/*
 * week4_3_struct_database.c
 * Author: Halil Ibrahim Sert
 * Student ID: 260AMM011
 * Description:
 *   Simple in-memory "database" using an array of structs.
 *   Use malloc to allocate space for n Student records,
 *   read each record from the user, print them as a table,
 *   and then free the memory.
 *
 *   Output must match the format in the Week 4 instructions exactly
 *   (it is checked by the autograder).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Same record layout as in Task 2. */
struct Student {
    char name[50];
    int id;
    float grade;
};

int main(void) {
    int n;
    struct Student *students = NULL;

    printf("Enter number of students: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number.\n");
        return 1;
    }

    /* One contiguous block holding n Student records; it can be
       indexed like a normal array: students[i]. */
    students = malloc((size_t)n * sizeof(struct Student));

    /* Never use the pointer without checking that malloc succeeded. */
    if (students == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    /* Read each record. %49s leaves room for the terminating '\0' in
       name[50], preventing a buffer overflow. scanf must return 3
       (name, id and grade all read) for the record to be valid. */
    for (int i = 0; i < n; i++) {
        printf("Enter data for student %d: ", i + 1);
        if (scanf("%49s %d %f", students[i].name,
                  &students[i].id, &students[i].grade) != 3) {
            printf("Invalid input.\n");
            free(students);     /* release memory before the early exit */
            return 1;
        }
    }

    /* Empty line, then the table in input order. */
    printf("\n");
    printf("%-6s %-11s %s\n", "ID", "Name", "Grade");
    for (int i = 0; i < n; i++) {
        printf("%-6d %-11s %.1f\n",
               students[i].id, students[i].name, students[i].grade);
    }

    /* Optional bonus (printed AFTER the table so required lines are
       unchanged): average grade and the top student. */
    double total = 0.0;
    int best = 0;
    for (int i = 0; i < n; i++) {
        total += students[i].grade;
        if (students[i].grade > students[best].grade) {
            best = i;
        }
    }
    printf("\nAverage grade = %.2f\n", total / n);
    printf("Top student: %s (%.1f)\n",
           students[best].name, students[best].grade);

    /* Free the whole array with a single call - it was one malloc. */
    free(students);
    students = NULL;

    return 0;
}