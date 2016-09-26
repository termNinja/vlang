#include <stdio.h>

void print_int(int t) {
    printf("%d\n", t);
}
void print_double(double t) {
    printf("%g\n", t);
}
void print_str(char* str) {
    printf("%s\n", str);
}
int read_int() {
    int t;
    scanf("%d", &t);
    return t;
}
double read_double() {
    double t;
    scanf("%lf", &t);
    return t;
}
