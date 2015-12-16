#ifndef __DEL_CONTAINER_H__
#define __DEL_CONTAINER_H__

namespace inf {


    /*
    ================
    Calls the destructor of all elements in the container.  Conditionally frees up memory used by the container.
    Note that this only works on lists containing pointers to objects and will cause a compiler error
    if called with non-pointers.  Since the list was not responsible for allocating the object, it has
    no information on whether the object still exists or not, so care must be taken to ensure that
    the pointers are still valid when this function is called.  Function will set all pointers in the
    container to NULL.
    ================
    */
    template< typename iterator >
    void delete_contents( iterator begin, iterator end, bool zero = false ) {
        iterator it = begin;
        while( it != end ) {
            delete (*it);

            if( *it )
                *it = NULL;

            ++it;
        }
    }

    template< typename container >
    void delete_contents( container& c, bool clear = false) {
        if( c.size() == 0 )
            return;

        delete_contents( c.begin(), c.end(), true );

        if ( clear ) {
            c.clear();
        }
    }

}   // ::inf

#endif // __DEL_CONTAINER_H__