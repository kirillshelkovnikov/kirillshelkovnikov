/* Lab 2, variant 30, adapted from original course report. */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#define MAX 100
static int input_long(const char *label, long min, long max, long *out) {
    char s[128], *end;
    for (;;) {
        printf("%s", label);
        if (!fgets(s, sizeof s, stdin)) return 0;
        errno = 0;
        long v = strtol(s, &end, 10);
        while (isspace((unsigned char)*end)) ++end;
        if (s != end && !*end && errno != ERANGE && v >= min && v <= max) {
            *out = v; return 1;
        }
        puts("Invalid integer.");
    }
}
static int minimum_abs_above(const int a[], int n, double threshold, long long *result) {
    int found = 0;
    for (int i = 0; i < n; i++) {
        long long mag = a[i] < 0 ? -(long long)a[i] : a[i];
        if (mag > threshold && (!found || mag < *result)) {
            *result = mag; found = 1;
        }
    }
    return found;
}
static int average_between_odds(const int a[], int n, double *result) {
    int first = -1, last = -1;
    for (int i = 0; i < n; i++) if (a[i] % 2) {
        if (first == -1) first = i;
        last = i;
    }
    if (first < 0 || last - first <= 1) return 0;
    long long sum = 0;
    for (int i = first + 1; i < last; i++) sum += a[i];
    *result = (double)sum / (last - first - 1);
    return 1;
}
int main(void) {
    long n, value;
    int a[MAX];
    if (!input_long("Array length (1..100): ", 1, MAX, &n)) return 1;
    for (int i = 0; i < n; i++) {
        char prompt[32];
        snprintf(prompt, sizeof prompt, "a[%d]: ", i);
        if (!input_long(prompt, INT_MIN, INT_MAX, &value)) return 1;
        a[i] = (int)value;
    }
    char s[128], *end;
    double x;
    for (;;) {
        printf("Threshold x: ");
        if (!fgets(s, sizeof s, stdin)) return 1;
        errno = 0;
        x = strtod(s, &end);
        while (isspace((unsigned char)*end)) ++end;
        if (end != s && !*end && errno != ERANGE && x == x) break;
        puts("Invalid threshold.");
    }
    long long minimum; double average;
    if (minimum_abs_above(a, (int)n, x, &minimum))
        printf("Minimum |a[i]| above x: %lld\n", minimum);
    else puts("No matching element.");
    if (average_between_odds(a, (int)n, &average))
        printf("Average strictly between first and last odd elements: %.6f\n", average);
    else puts("Average undefined: insufficient elements between odd boundaries.");
    return 0;
}
