/* Extra practice: sum matrix rows and swap rows with smallest/largest sums. */
#include <stdio.h>
#define ROWS 4
#define COLS 5
int main(void) {
    int a[ROWS][COLS] = {
        {2, 4, 6, 8, 10},
        {3, 5, 7, 9, 11},
        {1, 2, 3, 4, 5},
        {8, 6, 4, 2, 0}
    };
    int sums[ROWS] = {0}, min_index = 0, max_index = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) sums[i] += a[i][j];
        printf("Sum of row %d: %d\n", i + 1, sums[i]);
        if (sums[i] < sums[min_index]) min_index = i;
        if (sums[i] > sums[max_index]) max_index = i;
    }
    printf("Swapping rows %d and %d\n", min_index + 1, max_index + 1);
    for (int j = 0; j < COLS; j++) {
        int tmp = a[min_index][j];
        a[min_index][j] = a[max_index][j];
        a[max_index][j] = tmp;
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) printf("%4d", a[i][j]);
        putchar('\n');
    }
    return 0;
}
