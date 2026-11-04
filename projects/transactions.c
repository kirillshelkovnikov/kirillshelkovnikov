/* Portfolio extension: read synthetic transactions from a CSV file.
   CSV columns: date,category,amount. Positive amount = cost; negative = refund.
   Simplified educational implementation, not a banking production system. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define MAX_CATEGORIES 100
#define MAX_LINE 512
typedef struct {
    char name[64];
    double sum;
    int count;
} Category;
static int parse(char *line, char *category, double *amount) {
    char date[16], extra;
    if (sscanf(line, " %15[^,],%63[^,],%lf %c", date, category, amount, &extra) != 3)
        return 0;
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return 0;
    for (int j = 0; j < 10; j++)
        if (j != 4 && j != 7 && !isdigit((unsigned char)date[j])) return 0;
    if (!isfinite(*amount) || !*category) return 0;
    for (int j = 0; category[j]; j++)
        if (!isalnum((unsigned char)category[j]) && category[j] != '_') return 0;
    return 1;
}
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s transactions.csv\n", argv[0]);
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    if (!input) { perror("open"); return 1; }
    Category categories[MAX_CATEGORIES] = {{0}};
    int used = 0, invalid = 0, line_no = 0;
    double total = 0;
    char line[MAX_LINE];
    while (fgets(line, sizeof line, input)) {
        line_no++;
        if (line_no == 1 && strncmp(line, "date,category,amount", 20) == 0) continue;
        if (!strchr(line, '\n') && !feof(input)) {
            int c; while ((c = fgetc(input)) != '\n' && c != EOF) {}
            fprintf(stderr, "Line %d too long\n", line_no);
            invalid++;
            continue;
        }
        char name[64];
        double amount;
        if (!parse(line, name, &amount)) {
            fprintf(stderr, "Invalid CSV row: %d\n", line_no);
            invalid++;
            continue;
        }
        int k;
        for (k = 0; k < used; k++) if (!strcmp(name, categories[k].name)) break;
        if (k == used) {
            if (used >= MAX_CATEGORIES) { fputs("Too many categories\n", stderr); fclose(input); return 1; }
            strcpy(categories[k].name, name);
            used++;
        }
        categories[k].sum += amount;
        categories[k].count++;
        total += amount;
        if (amount >= 10000) printf("Large operation: %s %.2f\n", name, amount);
    }
    if (ferror(input)) { perror("read"); fclose(input); return 1; }
    fclose(input);
    puts("Spending by category:");
    for (int j = 0; j < used; j++)
        printf("%-16s %3d operations %12.2f\n", categories[j].name, categories[j].count, categories[j].sum);
    printf("TOTAL: %.2f | Invalid rows: %d\n", total, invalid);
    return invalid ? 2 : 0;
}
