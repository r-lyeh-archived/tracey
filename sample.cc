#include <vector>

int *static_leak = new int();

int main() {
    std::vector< int * > some_other_leak( 100, new int [400] );
    some_other_leak.clear();
    return 0;
}
