// tracey is a callstack based static library that requires no source modification.

int main() {
    int *make_leak = new int;
    int *make_leaks = new int [400];
    return 0;
}
