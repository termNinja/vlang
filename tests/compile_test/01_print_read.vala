void print_int(int x);
int read_int();

int main() {
    int x = 10;
    x = read_int();
    while (x > 0) {
        print_int(x);
        x = x - 1;
    }
    return 0;
}
