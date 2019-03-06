#include "avl_tree.h"

// #include <iostream>
using namespace std;
using namespace tree;

    // Test program
int main( )
{
    AvlTree<int, 1> t;

    // cout << sizeof (HelperInfo) << endl;

    // t.insert(10);
    // t.insert(23);
    // t.insert(8);
    // t.insert(11);
    // t.insert(12);
    // t.insert(3);
    // t.insert(2);
    // t.insert(4);
    // t.insert(90);
    // t.printTree();
    // return 0;
    int NUMS = 2000000;
    const int GAP  =   37;
    int i;

    cout << "Checking... (no more output means success)" << endl;

    // int total = 0;
    for( i = GAP; i != 0; i = ( i + GAP ) % NUMS ) {
        t.insert( i );
        // cout << ++total << endl;
    }
        // t.insert( i );

    // cout << "==========================" << endl;
    return 0;

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
#if 1
    decltype(t) t2;
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
}
