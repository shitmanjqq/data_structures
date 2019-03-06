#include <iostream>
#include "avl_tree_impl1.h"

using namespace tree;
using namespace std;

int main() {
    AvlTree<int> t;
#if 0
    t.insert(37);
    t.insert(74);
    t.insert(111);
    t.insert(148);
    t.insert(185);
    // t.insert(6);
    // t.insert(20);
    return 0;
#else
    int NUMS = 2000000;
    const int GAP  =   37;
    int i;

    cout << "Checking... (no more output means success)" << endl;

    for( i = GAP; i != 0; i = ( i + GAP ) % NUMS ) {
        // std::cout << i << std::endl;
        t.insert(i);
    }
        // t.insert( i );

    return 0;
#endif
}