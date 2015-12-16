#ifndef __FINDERS_H__
#define __FINDERS_H__

namespace inf {

    
    /*
     ================
     Searches for a NULL pointer in the container.  Returns -1 if NULL is not found.
     
     NOTE: This function can only be called on the containing pointers. Calling it
     on non-pointer container will cause a compiler error.
     ================
     */
    template< typename iterator>
    iterator find_null( iterator begin, const iterator end) {
        while( begin != end ) {
            if ( begin == NULL ) {
                return begin;
            }
            ++begin;
        }
        
        // Not found
        return end;
    }
    
    // Comparison functions
    // find an object that passes the predicate
    // IMPORTANT: unlike find, this returns end() if the element is not found, NOT NULL	
    template< class iterator, class cmp >
    iterator		find_iterator_if( iterator begin, const iterator end, cmp predicate ) {
        while( begin != end ) {
            if( predicate( *begin ) ) {
                break;
            }
            ++begin;
        }
        return begin;
    }

    // Comparison functions
    // find an object that passes the predicate
    // IMPORTANT: unlike find, this returns end() if the element is not found, NOT NULL	
    template< class iterator, class Type, class cmp >
    iterator		find_iterator_if( iterator begin, const iterator end, const Type& element, const cmp predicate ) {
        while( begin != end ) {
            if( predicate( element, *begin ) ) {
                break;
            }
            ++begin;
        }
        return begin;
    }

    template< class iterator, class Type>
    iterator		find_iterator( iterator begin, const iterator end, Type const & obj ) {
        while( begin != end ) {
            if( *begin == obj ) {
                break;
            }
            ++begin;
        }
        return begin;
    }

#if 1
    // Sorting functions
    template< class iterator, class cmp >
    void			sort( iterator begin, iterator end, cmp compare ) {
        quick_sort( begin, end, compare );
    }
#endif

    // Searching functions
    // IMPORTANT: unlike find_element, this returns end() if the element is not found, NOT NULL
    template< class iterator, class type, class cmp >
    iterator		binsearch_iterator( const iterator begin, const iterator end, const type& searchElement, cmp compare ) {
        return binary_search( searchElement, begin, end, compare );
    }

    template< class iterator_dist, class iterator_src >
    void			fill_from_range( iterator_dist to, iterator_src begin, const iterator_src end ) {
        for( ; begin != end; ++begin, ++to ) {
            *to = *begin;
        }
    }

}


#endif // __FINDERS_H__