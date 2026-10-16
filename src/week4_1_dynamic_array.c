/*
 * week4_1_dynamic_array.c
 * Author: Halil Ibrahim Sert
 * Student ID: 260AMM011
 * Description:
 *   Demonstrates creation and usage of a dynamic array using malloc.
 *   Allocate memory for n integers, read them from the user,
 *   print their sum and average, and then free the memory.
 *
 *   Output must match the format in the Week 4 instructions exactly
 *   (it is checked by the autograder).
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    int *arr = NULL;

    printf("Enter number of elements: ");
    /* scanf returns the number of items read; anything other than 1
       means the input was not an integer. Size must also be positive. */
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid size.\n");
        return 1;
    }

    /* Allocate exactly n ints on the heap. sizeof(int) keeps the code
       portable across platforms where int may differ in size. */
    arr = malloc((size_t)n * sizeof(int));

    /* malloc returns NULL when it cannot satisfy the request;
       dereferencing NULL would crash, so we must check first. */
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    /* Read n integers. %d skips whitespace (spaces and newlines),
       so the values may be on one line or on separate lines. */
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            printf("Invalid input.\n");
            free(arr);          /* release memory before the early exit */
            return 1;
        }
    }

    /* Accumulate in long long so large inputs do not overflow int. */
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    /* Cast to double BEFORE dividing; otherwise integer division
       would truncate (e.g. 15 / 2 = 7 instead of 7.5). */
    double average = (double)sum / n;

    printf("Sum = %lld\n", sum);
    printf("Average = %.2f\n", average);

    /* Every successful malloc needs a matching free. */
    free(arr);
    arr = NULL;

    return 0;
}