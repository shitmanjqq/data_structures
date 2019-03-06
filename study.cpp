#include <iostream>
#include "avl_tree_impl1.h"

using namespace tree;
using namespace std;

int main() {
    AvlTree<int> t;
#if 0
    // t.insert(37);
    // t.insert(74);
    // t.insert(111);
    // t.insert(148);
    // t.insert(185);

    // t.insert(1);
    // t.insert(2);
    // t.insert(2);
    // t.insert(2);
    // t.insert(2);
    // t.insert(2);
    // t.insert(2);
    return 0;
#else
    int NUMS = 20000000;
    const int GAP  =   37;
    int i;

    cout << "Checking... (no more output means success)" << endl;

    for( i = GAP; i != 0; i = ( i + GAP ) % NUMS )
        t.insert( i );
    // return 0;
    t.remove( 0 );
    for( i = 1; i < NUMS; i += 2 )
        t.remove( i );

    if( NUMS < 40 )
        t.printTree( );
    if( t.findMin( ) != 2 || t.findMax( ) != NUMS - 2 )
        cout << "FindMin or FindMax error!" << endl;

    for( i = 2; i < NUMS; i += 2 )
        if( !t.contains( i ) )
            cout << "Find error1!" << endl;

    for( i = 1; i < NUMS; i += 2 )
    {
        if( t.contains( i )  )
            cout << "Find error2!" << endl;
    }
#if 0
    AvlTree<int> t2;
    t2 = t;

    for( i = 2; i < NUMS; i += 2 )
        if( !t2.contains( i ) )
            cout << "Find error1!" << endl;

    for( i = 1; i < NUMS; i += 2 )
    {
        if( t2.contains( i ) )
            cout << "Find error2!" << endl;
    }
#endif
    cout << "End of test..." << endl;
    return 0;
#endif
}