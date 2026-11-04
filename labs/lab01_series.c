/* Lab 1, variant 1: Taylor series approximation of sinh(x), without math.h. */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <float.h>
static int read_num(const char *prompt, double *out) {
    char s[128], *end;
    for (;;) {
        printf("%s", prompt);
        if (!fgets(s, sizeof s, stdin)) return 0;
        errno = 0;
        double v = strtod(s, &end);
        while (isspace((unsigned char)*end)) ++end;
        if (end != s && !*end && errno != ERANGE && v == v && v <= DBL_MAX && v >= -DBL_MAX) {
            *out = v; return 1;
        }
        puts("Invalid number; try again.");
    }
}
int main(void) {
    double x, choice, value;
    if (!read_num("Enter x: ", &x) || !read_num("Mode (1: precision, 2: term count): ", &choice)) return 1;
    while (choice != 1 && choice != 2) {
        if (!read_num("Choose 1 or 2: ", &choice)) return 1;
    }
    if (!read_num(choice == 1 ? "Enter precision eps > 0: " : "Enter term count 1..10000: ", &value)) return 1;
    if (choice == 1) {
        while (!(value > 0)) if (!read_num("eps must be positive: ", &value)) return 1;
    } else {
        while (value < 1 || value > 10000 || (int)value != value)
            if (!read_num("Enter integer 1..10000: ", &value)) return 1;
    }
    double term = x, sum = 0;
    int count = 0, limit = choice == 1 ? 10000 : (int)value;
    while (count < limit) {
        double mag = term < 0 ? -term : term;
        if (choice == 1 && mag <= value) break;
        sum += term;
        ++count;
        term *= x * x / ((2.0 * count) * (2.0 * count + 1));
        if (term != term || sum != sum || term > DBL_MAX || term < -DBL_MAX ||
            sum > DBL_MAX || sum < -DBL_MAX) { fputs("Overflow\n", stderr); return 1; }
    }
    printf("sinh approximation: %.12g (%d terms)\n", sum, count);
    return 0;
}
