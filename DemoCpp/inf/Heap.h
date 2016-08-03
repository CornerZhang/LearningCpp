#ifndef _HEAP_H_
#define _HEAP_H_

/*
===============================================================================

Memory Management

This is a replacement for the compiler heap code (i.e. "C" malloc() and
free() calls). On average 2.5-3.0 times faster than MSVC malloc()/free().
Worst case performance is 1.65 times faster and best case > 70 times.

===============================================================================
*/

#include <cstddef>
#include "Lock.h"

#define INF_INLINE		inline

namespace inf {

	// common
    typedef unsigned long* UINT_PTR;
    
	struct memoryStats_t {
		int		num;
		size_t	minSize;
		size_t	maxSize;
		size_t	total_size;
	};

	enum align_t {
		ALIGN_NONE	= 0,
		ALIGN_4		= 4,
		ALIGN_8		= 8,		// memory is always at least aligned on an 8-byte boundary
		ALIGN_16	= 16,
		ALIGN_32	= 32,
		ALIGN_64	= 64,
		ALIGN_128	= 128
	};

    void        mem_init( void );
    void        mem_shutdown( void );
	void		mem_enable_leak_test( const char *name );
	void		mem_clear_frame_stats( void );
	void		mem_get_frame_stats( memoryStats_t &allocs, memoryStats_t &frees );
	void		mem_get_stats( memoryStats_t &stats );
	void		mem_alloc_defrag_block( void );

	size_t		mem_size( void *ptr );

#ifndef INF_DEBUG_MEMORY

	// release
	void *		mem_alloc( const size_t size );
	void *		mem_cleared_alloc( const size_t size );
	void		mem_free( void *ptr );
	char *		mem_copy_string( const char *in );
	void *		mem_alloc_aligned( const size_t size, const align_t align );
	void		mem_free_aligned( void *ptr );
	void *		mem_cleared_alloc_aligned( const size_t size, const align_t align );

#else   // INF_DEBUG_MEMORY

	// debug
	enum memoryGroupType_t {
		MEMGROUP_LINE,
		MEMGROUP_FILE,
		MEMGROUP_FOLDER
	};

	enum memorySortType_t {
		MEMSORT_SIZE,
		MEMSORT_LOCATION,
		MEMSORT_NUMALLOCS
	};

	void		mem_dump( const char* fileName );
	void		mem_dump_compressed( const char *fileName, memoryGroupType_t memGroup, memorySortType_t memSort, int sortCallStack, int numFrames, bool xlFriendly = false );
	void		mem_dump_per_class( const char *fileName );

	void *		mem_alloc( const size_t size, const char *fileName, const int lineNumber );
	void *		mem_cleared_alloc( const size_t size, const char *fileName, const int lineNumber );
	void		mem_free( void *ptr, const char *fileName, const int lineNumber );
	char *		mem_copy_string( const char *in, const char *fileName, const int lineNumber );
	void *		mem_alloc_aligned( const size_t size, const align_t align, const char *fileName, const int lineNumber );
	void		mem_free_aligned( void *ptr, const char *fileName, const int lineNumber );
	void *		mem_cleared_alloc_aligned( const size_t size, const align_t align, const char *fileName, const int lineNumber );
#endif  // INF_DEBUG_MEMORY

    /*
    ===============================================================================

    Block based allocator for fixed size objects.

    All objects of the 'type' are properly constructed.
    However, the constructor is not called for re-used objects.

    ===============================================================================
    */
    template<typename type, int blockSize, bool threadSafe = false >
    class BlockAlloc {
    public:
        BlockAlloc( void );
        ~BlockAlloc( void );

        void					shutdown( void );

        type *					alloc( void );
        void					free( type *element );

        size_t					get_total_count( void ) const { return total; }
        int						get_alloc_count( void ) const { return active; }
        int						get_free_count( void ) const { return total - active; }

    private:
        struct element_t {
            type				t;
            element_t *			next;
        };
        struct block_t {
            element_t			elements[blockSize];
            block_t *			next;
        };

        block_t *				blocks;
        element_t *				freeElem;
        size_t					total;
        int						active;
        Lock                    lock;					// lock for thread safe memory allocation
    };

    template<typename type, int blockSize, bool threadSafe>
    BlockAlloc<type,blockSize,threadSafe>::BlockAlloc( void ) {
        blocks = NULL;
        freeElem = NULL;
        total = active = 0;
    }

    template<typename type, int blockSize, bool threadSafe>
    BlockAlloc<type,blockSize,threadSafe>::~BlockAlloc( void ) {
        shutdown();
    }

    template<typename type, int blockSize, bool threadSafe>
    type *BlockAlloc<type,blockSize,threadSafe>::alloc( void ) {

        ScopedLock< threadSafe > scopedLock( lock );

        if ( freeElem == NULL ) {
            block_t *block = new block_t;
            block->next = blocks;
            blocks = block;
            for ( int i = 0; i < blockSize; i++ ) {
                block->elements[i].next = freeElem;
                freeElem = &block->elements[i];
            }
            total += blockSize;
        }
        active++;
        element_t *element = freeElem;
        freeElem = freeElem->next;
        element->next = NULL;
        return &element->t;
    }

    template<typename type, int blockSize, bool threadSafe>
    void BlockAlloc<type,blockSize,threadSafe>::free( type *t ) {
        if( t == NULL ) {
            return;
        }

        ScopedLock< threadSafe > scopedLock( lock );

        element_t *element = (element_t *)( t - ( (UINT_PTR) &((element_t *)0)->t ) );
        //element_t *element;
        //element = ( element_t * )( ( (unsigned char *)t ) - offsetof( element_t, t ) );
        element->next = freeElem;
        freeElem = element;
        active--;
    }

    template<typename type, int blockSize, bool threadSafe>
    void BlockAlloc<type,blockSize,threadSafe>::shutdown( void ) {

        ScopedLock< threadSafe > scopedLock( lock );

        while( blocks ) {
            block_t *block = blocks;
            blocks = blocks->next;
            delete block;
        }
        blocks = NULL;
        freeElem = NULL;
        total = active = 0;
    }

}   // ::inf

#define _alloca16( x )					((void *)((((int)_alloca( (x)+15 )) + 15) & ~15))

// replace ::new & ::delete
#ifdef INF_REDIRECT_NEWDELETE

#ifndef INF_DEBUG_MEMORY	// release

INF_INLINE void *operator new( size_t s, size_t t1, int t2) {
	return inf::mem_alloc( s );
}
INF_INLINE void operator delete( void *p, size_t size, int t2 ) {
	inf::mem_free( p );
}
INF_INLINE void *operator new[]( size_t s, size_t t1, int t2 ) {
	return inf::mem_alloc( s );
}
INF_INLINE void operator delete[]( void *p, size_t size, int t2 ) {
	inf::mem_free( p );
}
INF_INLINE void *operator new( size_t s ) {
	return inf::mem_alloc( s );
}
INF_INLINE void operator delete( void *p ) {
	inf::mem_free( p );
}
INF_INLINE void *operator new[]( size_t s ) {
	return inf::mem_alloc( s );
}
INF_INLINE void operator delete[]( void *p ) {
	inf::mem_free( p );
}

#define INF_DEBUG_NEW							new( 0, 0 )
#define INF_DEBUG_DELETE						delete( 0, 0 )
#undef new
#define new									    INF_DEBUG_NEW

#else /* INF_DEBUG_MEMORY */
INF_INLINE void *operator new( size_t s, size_t t1, int t2, char *fileName, int lineNumber ) {
	return inf::mem_alloc( s, fileName, lineNumber );
}
INF_INLINE void operator delete( void *p, size_t size, int t2, char *fileName, int lineNumber ) {
	inf::mem_free( p, fileName, lineNumber );
}
INF_INLINE void *operator new[]( size_t s, size_t t1, int t2, char *fileName, int lineNumber ) {
	return inf::mem_alloc( s, fileName, lineNumber );
}
INF_INLINE void operator delete[]( void *p, size_t size, int t2, char *fileName, int lineNumber ) {
	inf::mem_free( p, fileName, lineNumber );
}
INF_INLINE void *operator new( size_t s ) {
	return inf::mem_alloc( s, "__global", 0 );
}
INF_INLINE void operator delete( void *p ) {
	inf::mem_free( p, "__global", 0 );
}
INF_INLINE void *operator new[]( size_t s ) {
	return inf::mem_alloc( s, "__global", 0 );
}
INF_INLINE void operator delete[]( void *p ) {
	inf::mem_free( p, "__global", 0 );
}

#define INF_DEBUG_NEW							new( 0, 0, __FILE__, __LINE__ )
#define INF_DEBUG_DELETE						delete( 0, 0, __FILE__, __LINE__ )
#undef new
#define new									    INF_DEBUG_NEW
#endif /* INF_DEBUG_MEMORY */


#endif /* INF_REDIRECT_NEWDELETE */


// replace to debug version mem_XXX
#ifdef INF_DEBUG_MEMORY
#define		mem_alloc( size )				mem_alloc( size, __FILE__, __LINE__ )
#define		mem_cleared_alloc( size )		mem_cleared_alloc( size, __FILE__, __LINE__ )
#define		mem_free( ptr )					mem_free( ptr, __FILE__, __LINE__ )
#define		mem_copy_string( s )				mem_copy_string( s, __FILE__, __LINE__ )
#define		mem_alloc_aligned( size, align )	mem_alloc_aligned( size, align, __FILE__, __LINE__ )
#define		mem_free_aligned( ptr )			mem_free_aligned( ptr, __FILE__, __LINE__ )
#define		mem_cleared_alloc_aligned( size, align )	mem_cleared_alloc_aligned( size, align, __FILE__, __LINE__ )
#endif /* INF_DEBUG_MEMORY */

#endif // _HEAP_H_
