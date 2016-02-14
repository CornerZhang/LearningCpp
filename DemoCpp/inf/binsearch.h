#ifndef __BINSEARCH_H__
#define __BINSEARCH_H__

/*
===============================================================================

Binary Search templates

The array elements have to be ordered in increasing order.

===============================================================================
*/

namespace inf {

    /*
    ====================
    binsearch_greater_equal

    Finds the last array element which is smaller than the given value.
    ====================
    */
    template< class type >
    int binsearch_less( const type *array, const int arraySize, const type &value ) {
        int len = arraySize;
        int mid = len;
        int offset = 0;
        while( mid > 0 ) {
            mid = len >> 1;
            if ( array[offset+mid] < value ) {
                offset += mid;
            }
            len -= mid;
        }
        return offset;
    }

    /*
    ====================
    binsearch_greater_equal

    Finds the last array element which is smaller than or equal to the given value.
    ====================
    */
    template< class type >
    int binsearch_less_equal( const type *array, const int arraySize, const type &value ) {
        int len = arraySize;
        int mid = len;
        int offset = 0;
        while( mid > 0 ) {
            mid = len >> 1;
            if ( array[offset+mid] <= value ) {
                offset += mid;
            }
            len -= mid;
        }
        return offset;
    }

    /*
    ====================
    binsearch_greater

    Finds the first array element which is greater than the given value.
    ====================
    */
    template< class type >
    int binsearch_greater( const type *array, const int arraySize, const type &value ) {
        int len = arraySize;
        int mid = len;
        int offset = 0;
        int res = 0;
        while( mid > 0 ) {
            mid = len >> 1;
            if ( array[offset+mid] > value ) {
                res = 0;
            } else {
                offset += mid;
                res = 1;
            }
            len -= mid;
        }
        return offset+res;
    }

    /*
    ====================
    binsearch_greater_equal

    Finds the first array element which is greater than or equal to the given value.
    ====================
    */
    template< class type >
    int binsearch_greater_equal( const type *array, const int arraySize, const type &value ) {
        int len = arraySize;
        int mid = len;
        int offset = 0;
        int res = 0;
        while( mid > 0 ) {
            mid = len >> 1;
            if ( array[offset+mid] >= value ) {
                res = 0;
            } else {
                offset += mid;
                res = 1;
            }
            len -= mid;
        }
        return offset+res;
    }

    /*
    ============
    binary_search
    ============
    */
    template< class type, class iterator, class cmp >
    iterator binary_search( const type& element, iterator begin, iterator end, cmp compare ) {
        iterator lo = begin;
        iterator hi = end;
        iterator mid;

        std::size_t num = hi - lo;
        std::size_t half;
        int result;	

        while (lo <= hi) {
            half = num / 2;
            if( half ) {
                mid = lo + (num & 1 ? half : (half - 1)); // width;
                result = compare( &element, mid );
                if( result == 0 ) {
                    return mid;
                } else if ( result < 0 ) {
                    hi = mid - 1;
                    num = num & 1 ? half : half - 1;
                } else {
                    lo = mid + 1;
                    num = half;
                }
            } else if( num != 0 ) {
                return compare( &element, lo ) ? end : lo;
            } else {
                break;
            }
        }
        return end;
    }


}   // ::inf

#endif /* !__BINSEARCH_H__ */
