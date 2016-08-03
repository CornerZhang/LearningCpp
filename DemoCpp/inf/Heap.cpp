#include "Heap.h"

#include <memory.h>
#include <cstdio>
#include <cassert>
#include <string.h>
#include "cstrfunc.h"
#include "sys_mac.h"

#undef new
#undef INF_DEBUG_NEW
#undef INF_DEBUG_DELETE

typedef unsigned char			byte;		// 8 bits
typedef unsigned short			word;		// 16 bits
typedef unsigned int            dword;

//#ifndef CRASH_ON_STATIC_ALLOCATION
//#define CRASH_ON_STATIC_ALLOCATION
//#endif

//===============================================================
//
//	Heap
//
//===============================================================

#define SMALL_HEADER_SIZE		( (int) ( sizeof( byte ) + sizeof( byte ) ) )
#define MEDIUM_HEADER_SIZE		( (int) ( sizeof( mediumHeapEntry_s ) + sizeof( byte ) ) )
#define LARGE_HEADER_SIZE		( (int) ( sizeof( dword * ) + sizeof( byte ) ) )

#define ALIGN_SIZE( bytes )		( ( (bytes) + ALIGN - 1 ) & ~(ALIGN - 1) )
#define SMALL_ALIGN( bytes )	( ALIGN_SIZE( (bytes) + SMALL_HEADER_SIZE ) - SMALL_HEADER_SIZE )
#define MEDIUM_SMALLEST_SIZE	( ALIGN_SIZE( 256 ) + ALIGN_SIZE( MEDIUM_HEADER_SIZE ) )

namespace inf {

	class Heap {
	public:
		Heap( void );
		~Heap( void );				// frees all associated data
		void			init( void );					// initialize

		void *			allocate( const size_t bytes );	// allocate memory
		void			free( void *p );				// free memory
		void *			allocate_aligned( const size_t bytes, align_t align );
		void			free_aligned( void *p );
		size_t			msize( void *p );				// return size of data block
		void			dump( void  );

		void 			alloc_defrag_block( void );		// hack for huge renderbumps

	private:

		enum {
			ALIGN = 8									// memory alignment in bytes
		};

		enum {
			INVALID_ALLOC	= 0xdd,
			SMALL_ALLOC		= 0xaa,						// small allocation
			MEDIUM_ALLOC	= 0xbb,						// medium allocaction
			LARGE_ALLOC		= 0xcc						// large allocaction
		};

		struct page_s {									// allocation page
			void *				data;					// data pointer to allocated memory
			unsigned long		dataSize;				// number of bytes of memory 'data' points to
			page_s *			next;					// next free page in same page manager
			page_s *			prev;					// used only when allocated
			unsigned long		largestFree;			// this data used by the medium-size heap manager
			void *				firstFree;				// pointer to first free entry
		};

		struct mediumHeapEntry_s {
			page_s *			page;					// pointer to page
			unsigned long		size;					// size of block
			mediumHeapEntry_s *	prev;					// previous block
			mediumHeapEntry_s *	next;					// next block
			mediumHeapEntry_s *	prevFree;				// previous free block
			mediumHeapEntry_s *	nextFree;				// next free block
			dword				freeBlock;				// non-zero if free block
		};

		// variables
		void *			smallFirstFree[256/ALIGN+1];	// small heap allocator lists (for allocs of 1-255 bytes)
		page_s *		smallCurPage;					// current page for small allocations
		dword			smallCurPageOffset;				// byte offset in current page
		page_s *		smallFirstUsedPage;				// first used page of the small heap manager

		page_s *		mediumFirstFreePage;			// first partially free page
		page_s *		mediumLastFreePage;				// last partially free page
		page_s *		mediumFirstUsedPage;			// completely used page

		page_s *		largeFirstUsedPage;				// first page used by the large heap manager

		page_s *		swapPage;

		dword			pagesAllocated;					// number of pages currently allocated
		dword			pageSize;						// size of one alloc page in bytes

		dword			pageRequests;					// page requests
		dword			OSAllocs;						// number of allocs made to the OS

		int				c_heapAllocRunningCount;

		void			*defragBlock;					// a single huge block that can be allocated
		// at startup, then freed when needed

		// methods
		page_s *		AllocatePage( unsigned long bytes );	// allocate page from the OS
		void			FreePage( Heap::page_s *p );	// free an OS allocated page

		void *			SmallAllocate( unsigned long bytes );	// allocate memory (1-255 bytes) from small heap manager
		void			SmallFree( void *ptr );			// free memory allocated by small heap manager

		void *			MediumAllocateFromPage( Heap::page_s *p, unsigned long sizeNeeded );
		void *			MediumAllocate( unsigned long bytes );	// allocate memory (256-32768 bytes) from medium heap manager
		void			MediumFree( void *ptr );		// free memory allocated by medium heap manager

		void *			LargeAllocate( unsigned long bytes );	// allocate large block from OS directly
		void			LargeFree( void *ptr );			// free memory allocated by large heap manager

		void			ReleaseSwappedPages( void );
		void			FreePageReal( Heap::page_s *p );
	};


	/*
	================
	Heap::init
	================
	*/
	void Heap::init ( void ) {
		OSAllocs			= 0;
		pageRequests		= 0;
		pageSize			= 65536 - sizeof( Heap::page_s );
		pagesAllocated		= 0;								// reset page allocation counter

		largeFirstUsedPage	= NULL;								// init large heap manager
		swapPage			= NULL;

		memset( smallFirstFree, 0, sizeof(smallFirstFree) );	// init small heap manager
		smallFirstUsedPage	= NULL;
		smallCurPage		= AllocatePage( pageSize );
		assert( smallCurPage );
		smallCurPageOffset	= SMALL_ALIGN( 0 );

		defragBlock = NULL;

		mediumFirstFreePage	= NULL;								// init medium heap manager
		mediumLastFreePage	= NULL;
		mediumFirstUsedPage	= NULL;

		c_heapAllocRunningCount = 0;
	}

	/*
	================
	Heap::Heap
	================
	*/
	Heap::Heap( void ) {        
		init();
	}

	/*
	================
	Heap::~Heap

	returns all allocated memory back to OS
	================
	*/
	Heap::~Heap( void ) {

		Heap::page_s	*p;

		if ( smallCurPage ) {
			FreePage( smallCurPage );			// free small-heap current allocation page
		}
		p = smallFirstUsedPage;					// free small-heap allocated pages 
		while( p ) {
			Heap::page_s *next = p->next;
			FreePage( p );
			p= next;
		}

		p = largeFirstUsedPage;					// free large-heap allocated pages
		while( p ) {
			Heap::page_s *next = p->next;
			FreePage( p );
			p = next;
		}

		p = mediumFirstFreePage;				// free medium-heap allocated pages
		while( p ) {
			Heap::page_s *next = p->next;
			FreePage( p );
			p = next;
		}

		p = mediumFirstUsedPage;				// free medium-heap allocated completely used pages
		while( p ) {
			Heap::page_s *next = p->next;
			FreePage( p );
			p = next;
		}

		ReleaseSwappedPages();			

		if ( defragBlock ) {
			free( defragBlock );
		}

		assert( pagesAllocated == 0 );
	}

	/*
	================
	Heap::alloc_defrag_block
	================
	*/
	void Heap::alloc_defrag_block( void ) {
		int		size = 0x40000000; // 1GB

		if ( defragBlock ) {
			return;
		}
		while( 1 ) {
			defragBlock = malloc( size );
			if ( defragBlock ) {
				break;
			}
			size >>= 1;
		}
		printf( "allocated a %i mb defrag block\n", size / (1024*1024) );
	}

	/*
	================
	Heap::allocate
	================
	*/
	void *Heap::allocate( const size_t bytes ) {
		if ( !bytes ) {
			return NULL;
		}
		c_heapAllocRunningCount++;

		if ( !(bytes & ~255) ) {
			return SmallAllocate( bytes );
		}
		if ( !(bytes & ~32767) ) {
			return MediumAllocate( bytes );
		}
		return LargeAllocate( bytes );
	}

	/*
	================
	Heap::free
	================
	*/
	void Heap::free( void *p ) {
		if ( !p ) {
			return;
		}
		c_heapAllocRunningCount--;

		switch( ((byte *)(p))[-1] ) {
		case SMALL_ALLOC:
			SmallFree( p );
			break;
		case MEDIUM_ALLOC:
			MediumFree( p );
			break;
		case LARGE_ALLOC:
			LargeFree( p );
			break;
		default:
			fatal_error( "Heap::free: invalid memory block (%d)", c_heapAllocRunningCount );
			break;
		}
	}

	/*
	================
	Heap::AllocateAligned
	================
	*/
	void *Heap::allocate_aligned( const size_t bytes, const align_t align ) {
		byte *ptr, *alignedPtr;

		if ( align != ALIGN_NONE && align != ALIGN_4 && align != ALIGN_8 && align != ALIGN_16 && align != ALIGN_32 && align != ALIGN_64 && align != ALIGN_128 ) {
			printf( "Heap::AllocateAligned: bad alignment %d", align );	// fatal_error
		}

		ptr = (byte *) malloc( bytes + align + sizeof( UINT_PTR ) );
		if ( !ptr ) {
			if ( defragBlock ) {
				printf( "Freeing defragBlock on alloc of %li.\n", bytes );
				free( defragBlock );
				defragBlock = NULL;
				ptr = (byte *) malloc( bytes + align + sizeof( UINT_PTR ) );
				alloc_defrag_block();
			}
			if ( !ptr ) {
#if defined( _WIN32 )
				MEMORYSTATUSEX statex;
				statex.dwLength = sizeof( statex );
				GlobalMemoryStatusEx( &statex );
				printf( "\nTotal Physical Memory: %I64d bytes\nAvailable Physical Memory: %I64d bytes\nMemory Utilization: %d %%\n\n", 
					statex.ullTotalPhys, statex.ullAvailPhys, (int)statex.dwMemoryLoad );
#endif
				fatal_error( "Heap::AllocateAligned request for %li bytes aligned at %i failed", bytes, align );
			}
		}
		if ( align == ALIGN_NONE ) {
			alignedPtr = ptr + sizeof( UINT_PTR );
		} else {
			alignedPtr = (byte *) ( ( (unsigned long)ptr + ( align - 1 ) ) & ~( align - 1 ) );
			if ( alignedPtr - ptr < sizeof( UINT_PTR ) ) {
				alignedPtr += align;
			}
		}
		*((UINT_PTR *)( (UINT_PTR)alignedPtr - sizeof( UINT_PTR ) )) = (UINT_PTR) ptr;
		return (void *) alignedPtr;
	}

	/*
	================
	Heap::free
	================
	*/
	void Heap::free_aligned( void *p ) {
		free( (void *) *((int *) (( (byte *) p ) - sizeof( uintptr_t ))) );
	}

	/*
	================
	Heap::Msize

	returns size of allocated memory block
	p	= pointer to memory block
	Notes:	size may not be the same as the size in the original
	allocation request (due to block alignment reasons).
	================
	*/
	size_t Heap::msize( void *p ) {

		if ( !p ) {
			return 0;
		}

		switch( ((byte *)(p))[-1] ) {
		case SMALL_ALLOC:
			return SMALL_ALIGN( ((byte *)(p))[-SMALL_HEADER_SIZE] * ALIGN );
		case MEDIUM_ALLOC:
			return ((mediumHeapEntry_s *)(((byte *)(p)) - ALIGN_SIZE( MEDIUM_HEADER_SIZE )))->size - ALIGN_SIZE( MEDIUM_HEADER_SIZE );
		case LARGE_ALLOC:
			return ((Heap::page_s*)(*((unsigned long *)(((byte *)p) - ALIGN_SIZE( LARGE_HEADER_SIZE )))))->dataSize - ALIGN_SIZE( LARGE_HEADER_SIZE );
		default:
			fatal_error( "Heap::Msize: invalid memory block\n" );
			return 0;
		}
	}

	/*
	================
	Heap::Dump

	dump contents of the heap
	================
	*/
	void Heap::dump( void ) {
		Heap::page_s	*pg;

		for ( pg = smallFirstUsedPage; pg; pg = pg->next ) {
			printf( "%p  bytes %-8lu  (in use by small heap)\n", pg->data, pg->dataSize);
		}

		if ( smallCurPage ) {
			pg = smallCurPage;
			printf( "%p  bytes %-8ld  (small heap active page)\n", pg->data, pg->dataSize );
		}

		for ( pg = mediumFirstUsedPage; pg; pg = pg->next ) {
			printf( "%p  bytes %-8ld  (completely used by medium heap)\n", pg->data, pg->dataSize );
		}

		for ( pg = mediumFirstFreePage; pg; pg = pg->next ) {
			printf( "%p  bytes %-8ld  (partially used by medium heap)\n", pg->data, pg->dataSize );
		}

		for ( pg = largeFirstUsedPage; pg; pg = pg->next ) {
			printf( "%p  bytes %-8ld  (fully used by large heap)\n", pg->data, pg->dataSize );
		}

		printf( "pages allocated : %d\n", pagesAllocated );
	}

	/*
	================
	Heap::FreePageReal

	frees page to be used by the OS
	p	= page to free
	================
	*/
	void Heap::FreePageReal( Heap::page_s *p ) {
		assert( p );
		::free( p );
	}

	/*
	================
	Heap::ReleaseSwappedPages

	releases the swap page to OS
	================
	*/
	void Heap::ReleaseSwappedPages ( void ) {
		if ( swapPage ) {
			FreePageReal( swapPage );
		}
		swapPage = NULL;
	}

	/*
	================
	Heap::AllocatePage

	allocates memory from the OS
	bytes	= page size in bytes
	returns pointer to page
	================
	*/
	Heap::page_s* Heap::AllocatePage( unsigned long bytes ) {
		Heap::page_s*	p;

		pageRequests++;

		if ( swapPage && swapPage->dataSize == bytes ) {			// if we've got a swap page somewhere
			p			= swapPage;
			swapPage	= NULL;
		}
		else {
			unsigned long size;

			size = bytes + sizeof(Heap::page_s);

			p = (Heap::page_s *) ::malloc( size + ALIGN - 1 );
			if ( !p ) {
				if ( defragBlock ) {
					printf( "Freeing defragBlock on alloc of %li.\n", size + ALIGN - 1 );
					free( defragBlock );
					defragBlock = NULL;
					p = (Heap::page_s *) ::malloc( size + ALIGN - 1 );			
					alloc_defrag_block();
				}
				if ( !p ) {
#if defined( _WIN32 )
					MEMORYSTATUSEX statex;
					statex.dwLength = sizeof( statex );
					GlobalMemoryStatusEx( &statex );
					printf( "\nTotal Physical Memory: %I64d bytes\nAvailable Physical Memory: %I64d bytes\nMemory Utilization: %i %%\n\n", 
						statex.ullTotalPhys, statex.ullAvailPhys, (int)statex.dwMemoryLoad );
#endif
					fatal_error( "Heap::AllocatePage request for %i bytes failed", bytes );
				}
			}

			p->data		= (void *) ALIGN_SIZE( ((unsigned long)p) + sizeof( Heap::page_s ) );
			p->dataSize	= size - sizeof(Heap::page_s);
			p->firstFree = NULL;
			p->largestFree = 0;
			OSAllocs++;
		}

		p->prev = NULL;
		p->next = NULL;

		pagesAllocated++;

		return p;
	}

	/*
	================
	Heap::FreePage

	frees a page back to the operating system
	p	= pointer to page
	================
	*/
	void Heap::FreePage( Heap::page_s *p ) {
		assert( p );

		if ( p->dataSize == pageSize && !swapPage ) {			// add to swap list?
			swapPage = p;
		}
		else {
			FreePageReal( p );
		}

		pagesAllocated--;
	}

	//===============================================================
	//
	//	small heap code
	//
	//===============================================================

	/*
	================
	Heap::SmallAllocate

	allocate memory (1-255 bytes) from the small heap manager
	bytes = number of bytes to allocate
	returns pointer to allocated memory
	================
	*/
	void *Heap::SmallAllocate( unsigned long bytes ) {
		// we need the at least sizeof( dword ) bytes for the free list
		if ( bytes < sizeof( dword ) ) {
			bytes = sizeof( dword );
		}

		// increase the number of bytes if necessary to make sure the next small allocation is aligned
		bytes = SMALL_ALIGN( bytes );

		byte *smallBlock = (byte *)(smallFirstFree[bytes / ALIGN]);
		if ( smallBlock ) {
			unsigned long *link = (unsigned long *)(smallBlock + SMALL_HEADER_SIZE);
			smallBlock[1] = SMALL_ALLOC;					// allocation identifier
			smallFirstFree[bytes / ALIGN] = (void *)(*link);
			return (void *)(link);
		}

		unsigned long bytesLeft = (long)(pageSize) - smallCurPageOffset;
		// if we need to allocate a new page
		if ( bytes >= bytesLeft ) {

			smallCurPage->next	= smallFirstUsedPage;
			smallFirstUsedPage	= smallCurPage;
			smallCurPage		= AllocatePage( pageSize );
			if ( !smallCurPage ) {
				return NULL;
			}
			// make sure the first allocation is aligned
			smallCurPageOffset	= SMALL_ALIGN( 0 );
		}

		smallBlock			= ((byte *)smallCurPage->data) + smallCurPageOffset;
		smallBlock[0]		= (byte)(bytes / ALIGN);		// write # of bytes/ALIGN
		smallBlock[1]		= SMALL_ALLOC;					// allocation identifier
		smallCurPageOffset  += bytes + SMALL_HEADER_SIZE;	// increase the offset on the current page
		return ( smallBlock + SMALL_HEADER_SIZE );			// skip the first two bytes
	}

	/*
	================
	Heap::SmallFree

	frees a block of memory allocated by SmallAllocate() call
	data = pointer to block of memory
	================
	*/
	void Heap::SmallFree( void *ptr ) {
		((byte *)(ptr))[-1] = INVALID_ALLOC;

		byte *d = ( (byte *)ptr ) - SMALL_HEADER_SIZE;
		unsigned long *dt = (unsigned long *)ptr;
		// index into the table with free small memory blocks
		dword ix = *d;

		// check if the index is correct
		if ( ix > (256 / ALIGN) ) {
			printf( "SmallFree: invalid memory block" );
		}

		*dt = (unsigned long)(smallFirstFree[ix]);	// write next index
		smallFirstFree[ix] = (void *)d;		// link
	}

	//===============================================================
	//
	//	medium heap code
	//
	//	Medium-heap allocated pages not returned to OS until heap destructor
	//	called (re-used instead on subsequent medium-size malloc requests).
	//
	//===============================================================

	/*
	================
	Heap::MediumAllocateFromPage

	performs allocation using the medium heap manager from a given page
	p				= page
	sizeNeeded	= # of bytes needed
	returns pointer to allocated memory
	================
	*/
	void *Heap::MediumAllocateFromPage( Heap::page_s *p, unsigned long sizeNeeded ) {

		mediumHeapEntry_s	*best,*nw = NULL;
		byte				*ret;

		best = (mediumHeapEntry_s *)(p->firstFree);			// first block is largest

		assert( best );
		assert( best->size == p->largestFree );
		assert( best->size >= sizeNeeded );

		// if we can allocate another block from this page after allocating sizeNeeded bytes
		if ( best->size >= (dword)( sizeNeeded + MEDIUM_SMALLEST_SIZE ) ) {
			nw = (mediumHeapEntry_s *)((byte *)best + best->size - sizeNeeded);
			nw->page		= p;
			nw->prev		= best;
			nw->next		= best->next;
			nw->prevFree	= NULL;
			nw->nextFree	= NULL;
			nw->size		= sizeNeeded;
			nw->freeBlock	= 0;			// used block
			if ( best->next ) {
				best->next->prev = nw;
			}
			best->next	= nw;
			best->size	-= sizeNeeded;

			p->largestFree = best->size;
		}
		else {
			if ( best->prevFree ) {
				best->prevFree->nextFree = best->nextFree;
			}
			else {
				p->firstFree = (void *)best->nextFree;
			}
			if ( best->nextFree ) {
				best->nextFree->prevFree = best->prevFree;
			}

			best->prevFree  = NULL;
			best->nextFree  = NULL;
			best->freeBlock = 0;			// used block
			nw = best;

			p->largestFree = 0;
		}

		ret		= (byte *)(nw) + ALIGN_SIZE( MEDIUM_HEADER_SIZE );
		ret[-1] = MEDIUM_ALLOC;		// allocation identifier

		return (void *)(ret);
	}

	/*
	================
	Heap::MediumAllocate

	allocate memory (256-32768 bytes) from medium heap manager
	bytes	= number of bytes to allocate
	returns pointer to allocated memory
	================
	*/
	void *Heap::MediumAllocate( unsigned long bytes ) {
		Heap::page_s		*p;
		void				*data;

		unsigned long sizeNeeded = ALIGN_SIZE( bytes ) + ALIGN_SIZE( MEDIUM_HEADER_SIZE );

		// find first page with enough space
		for ( p = mediumFirstFreePage; p; p = p->next ) {
			if ( p->largestFree >= sizeNeeded ) {
				break;
			}

			assert( p->next != mediumFirstFreePage );	// this should never happen
		}

		if ( !p ) {								// need to allocate new page?
			p = AllocatePage( pageSize );
			if ( !p ) {
				return NULL;					// malloc failure!
			}
			p->prev		= NULL;
			p->next		= mediumFirstFreePage;
			if (p->next) {
				p->next->prev = p;
			}
			else {
				mediumLastFreePage	= p;
			}

			mediumFirstFreePage		= p;

			p->largestFree	= pageSize;
			p->firstFree	= (void *)p->data;

			mediumHeapEntry_s *e;
			e				= (mediumHeapEntry_s *)(p->firstFree);
			e->page			= p;
			// make sure ((byte *)e + e->size) is aligned
			e->size			= pageSize & ~(ALIGN - 1);
			e->prev			= NULL;
			e->next			= NULL;
			e->prevFree		= NULL;
			e->nextFree		= NULL;
			e->freeBlock	= 1;
		}

		data = MediumAllocateFromPage( p, sizeNeeded );		// allocate data from page

		// if the page can no longer serve memory, move it away from free list
		// (so that it won't slow down the later alloc queries)
		// this modification speeds up the pageWalk from O(N) to O(sqrt(N))
		// a call to free may swap this page back to the free list

		if ( p->largestFree < MEDIUM_SMALLEST_SIZE ) {
			if ( p == mediumLastFreePage ) {
				mediumLastFreePage = p->prev;
			}

			if ( p == mediumFirstFreePage ) {
				mediumFirstFreePage = p->next;
			}

			if ( p->prev ) {
				p->prev->next = p->next;
			}
			if ( p->next ) {
				p->next->prev = p->prev;
			}

			// link to "completely used" list
			p->prev = NULL;
			p->next = mediumFirstUsedPage;
			if ( p->next ) {
				p->next->prev = p;
			}
			mediumFirstUsedPage = p;
			return data;
		}

		// re-order linked list (so that next malloc query starts from current
		// matching block) -- this speeds up both the page walks and block walks

		if ( p != mediumFirstFreePage ) {
			assert( mediumLastFreePage );
			assert( mediumFirstFreePage );
			assert( p->prev );

			mediumLastFreePage->next	= mediumFirstFreePage;
			mediumFirstFreePage->prev	= mediumLastFreePage;
			mediumLastFreePage			= p->prev;
			p->prev->next				= NULL;
			p->prev						= NULL;
			mediumFirstFreePage			= p;
		}

		return data;
	}

	/*
	================
	Heap::MediumFree

	frees a block allocated by the medium heap manager
	ptr	= pointer to data block
	================
	*/
	void Heap::MediumFree( void *ptr ) {
		((byte *)(ptr))[-1] = INVALID_ALLOC;

		mediumHeapEntry_s	*e = (mediumHeapEntry_s *)((byte *)ptr - ALIGN_SIZE( MEDIUM_HEADER_SIZE ));
		Heap::page_s		*p = e->page;
		bool				isInFreeList;

		isInFreeList = p->largestFree >= MEDIUM_SMALLEST_SIZE;

		assert( e->size );
		assert( e->freeBlock == 0 );

		mediumHeapEntry_s *prev = e->prev;

		// if the previous block is free we can merge
		if ( prev && prev->freeBlock ) {
			prev->size += e->size;
			prev->next = e->next;
			if ( e->next ) {
				e->next->prev = prev;
			}
			e = prev;
		}
		else {
			e->prevFree		= NULL;				// link to beginning of free list
			e->nextFree		= (mediumHeapEntry_s *)p->firstFree;
			if ( e->nextFree ) {
				assert( !(e->nextFree->prevFree) );
				e->nextFree->prevFree = e;
			}

			p->firstFree	= e;
			p->largestFree	= e->size;
			e->freeBlock	= 1;				// mark block as free
		}

		mediumHeapEntry_s *next = e->next;

		// if the next block is free we can merge
		if ( next && next->freeBlock ) {
			e->size += next->size;
			e->next = next->next;

			if ( next->next ) {
				next->next->prev = e;
			}

			if ( next->prevFree ) {
				next->prevFree->nextFree = next->nextFree;
			}
			else {
				assert( next == p->firstFree );
				p->firstFree = next->nextFree;
			}

			if ( next->nextFree ) {
				next->nextFree->prevFree = next->prevFree;
			}
		}

		if ( p->firstFree ) {
			p->largestFree = ((mediumHeapEntry_s *)(p->firstFree))->size;
		}
		else {
			p->largestFree = 0;
		}

		// did e become the largest block of the page ?

		if ( e->size > p->largestFree ) {
			assert( e != p->firstFree );
			p->largestFree = e->size;

			if ( e->prevFree ) {
				e->prevFree->nextFree = e->nextFree;
			}
			if ( e->nextFree ) {
				e->nextFree->prevFree = e->prevFree;
			}

			e->nextFree = (mediumHeapEntry_s *)p->firstFree;
			e->prevFree = NULL;
			if ( e->nextFree ) {
				e->nextFree->prevFree = e;
			}
			p->firstFree = e;
		}

		// if page wasn't in free list (because it was near-full), move it back there
		if ( !isInFreeList ) {

			// remove from "completely used" list
			if ( p->prev ) {
				p->prev->next = p->next;
			}
			if ( p->next ) {
				p->next->prev = p->prev;
			}
			if ( p == mediumFirstUsedPage ) {
				mediumFirstUsedPage = p->next;
			}

			p->next = NULL;
			p->prev = mediumLastFreePage;

			if ( mediumLastFreePage ) {
				mediumLastFreePage->next = p;
			}
			mediumLastFreePage = p;
			if ( !mediumFirstFreePage ) {
				mediumFirstFreePage = p;
			}
		} 
	}

	//===============================================================
	//
	//	large heap code
	//
	//===============================================================

	/*
	================
	Heap::LargeAllocate

	allocates a block of memory from the operating system
	bytes	= number of bytes to allocate
	returns pointer to allocated memory
	================
	*/
	void *Heap::LargeAllocate( unsigned long bytes ) {
		Heap::page_s *p = AllocatePage( bytes + ALIGN_SIZE( LARGE_HEADER_SIZE ) );

		assert( p );

		if ( !p ) {
			return NULL;
		}
        
		byte *	d	= (byte*)(p->data) + ALIGN_SIZE( LARGE_HEADER_SIZE );
		unsigned long *	dw	= (unsigned long*)(d - ALIGN_SIZE( LARGE_HEADER_SIZE ));
		dw[0]		= (unsigned long)p;				// write pointer back to page table
		d[-1]		= LARGE_ALLOC;			// allocation identifier

		// link to 'large used page list'
		p->prev = NULL;
		p->next = largeFirstUsedPage;
		if ( p->next ) {
			p->next->prev = p;
		}
		largeFirstUsedPage = p;

		return (void *)(d);
	}

	/*
	================
	Heap::LargeFree

	frees a block of memory allocated by the 'large memory allocator'
	p	= pointer to allocated memory
	================
	*/
	void Heap::LargeFree( void *ptr) {
		Heap::page_s*	pg;

		((byte *)(ptr))[-1] = INVALID_ALLOC;

		// get page pointer
		pg = (Heap::page_s *)(*((unsigned long *)(((byte *)ptr) - ALIGN_SIZE( LARGE_HEADER_SIZE ))));

		// unlink from doubly linked list
		if ( pg->prev ) {
			pg->prev->next = pg->next;
		}
		if ( pg->next ) {
			pg->next->prev = pg->prev;
		}
		if ( pg == largeFirstUsedPage ) {
			largeFirstUsedPage = pg->next;
		}
		pg->next = pg->prev = NULL;

		FreePage(pg);
	}




	//===============================================================
	//
	//	memory allocation all in one place
	//
	//===============================================================

	static Heap *			mem_heap = NULL;
	static memoryStats_t	mem_total_allocs = { 0, 0x0fffffff, 0, 0 };
	static memoryStats_t	mem_frame_allocs;
	static memoryStats_t	mem_frame_frees;

	/*
	==================
	mem_clear_frame_stats
	==================
	*/
	void mem_clear_frame_stats( void ) {
		mem_frame_allocs.num = mem_frame_frees.num = 0;
		mem_frame_allocs.minSize = mem_frame_frees.minSize = 0x0fffffff;
		mem_frame_allocs.maxSize = mem_frame_frees.maxSize = 0;
		mem_frame_allocs.total_size = mem_frame_frees.total_size = 0;
	}

	/*
	==================
	mem_get_frame_stats
	==================
	*/
	void mem_get_frame_stats( memoryStats_t &allocs, memoryStats_t &frees ) {
		allocs = mem_frame_allocs;
		frees = mem_frame_frees;
	}

	/*
	==================
	mem_get_stats
	==================
	*/
	void mem_get_stats( memoryStats_t &stats ) {
		stats = mem_total_allocs;
	}

	/*
	==================
	mem_UpdateStats
	==================
	*/
	void mem_UpdateStats( memoryStats_t &stats, size_t size ) {
		stats.num++;
		if ( size < stats.minSize ) {
			stats.minSize = size;
		}
		if ( size > stats.maxSize ) {
			stats.maxSize = size;
		}
		stats.total_size += size;
	}

	/*
	==================
	mem_UpdateAllocStats
	==================
	*/
	void mem_UpdateAllocStats( size_t size ) {
		mem_UpdateStats( mem_frame_allocs, size );
		mem_UpdateStats( mem_total_allocs, size );
	}

	/*
	==================
	mem_UpdateFreeStats
	==================
	*/
	void mem_UpdateFreeStats( size_t size ) {
		mem_UpdateStats( mem_frame_frees, size );
		mem_total_allocs.num--;
		mem_total_allocs.total_size -= size;
	}

	/*
	==================
	mem_size
	==================
	*/
	size_t mem_size( void *ptr ) {
		return mem_heap->msize(ptr);
	}

#ifndef INF_DEBUG_MEMORY
	/*
	==================
	mem_alloc
	==================
	*/
	void *mem_alloc( const size_t size ) {
		if ( !size ) {
			return NULL;
		}
		if ( !mem_heap ) {
#ifdef CRASH_ON_STATIC_ALLOCATION
			*((int*)0x0) = 1;
#endif
			return malloc( size );
		}
		void *mem = mem_heap->allocate( size );
		mem_UpdateAllocStats( mem_heap->msize( mem ) );
		if ( !mem ) {
#if defined( _WIN32 ) //&& !defined( _XENON )
			MEMORYSTATUSEX statex;
			statex.dwLength = sizeof( statex );
			GlobalMemoryStatusEx( &statex );
			printf( "\nTotal Physical Memory: %I64d bytes\nAvailable Physical Memory: %I64d bytes\nMemory Utilization: %i %%\n\n", 
				statex.ullTotalPhys, statex.ullAvailPhys, (int)statex.dwMemoryLoad );
#endif
			fatal_error( "mem_alloc request for %li bytes failed", size );
		}
		return mem;
	}

	/*
	==================
	mem_free
	==================
	*/
	void mem_free( void *ptr ) {
		if ( ptr == NULL ) {
			return;
		}
#if _DEBUG // used to catch memory allocated with mem_alloc_aligned
		UINT_PTR checkPtr = *((UINT_PTR *)( (UINT_PTR)ptr - sizeof( UINT_PTR ) )); 
		if ( checkPtr > (UINT_PTR)ptr - 132 && checkPtr < (UINT_PTR)ptr ) {
			assert( !"possible aligned memory deallocation" );
		}
#endif
		if ( !mem_heap ) {
#ifdef CRASH_ON_STATIC_ALLOCATION
			*((int*)0x0) = 1;
#endif
			free( ptr );
			return;
		}
		mem_UpdateFreeStats( mem_heap->msize( ptr ) );
		mem_heap->free( ptr );
	}

	/*
	==================
	mem_alloc_aligned
	==================
	*/
	void *mem_alloc_aligned( const size_t size, const align_t align ) {
		// DirectX relies on being able to allocate 0 bytes and get back a legit pointer
		if ( !size ) {
			return NULL;
		}
		if ( !mem_heap ) {
#ifdef CRASH_ON_STATIC_ALLOCATION
			*((int*)0x0) = 1;
#endif
			return malloc( size );
		}
		void *mem = mem_heap->allocate_aligned( size, align );
		// make sure the memory is aligned
		assert( align == ALIGN_NONE || ( (unsigned long)mem & (align-1)) == 0 );
		return mem;
	}

	/*
	==================
	mem_free_aligned
	==================
	*/
	void mem_free_aligned( void *ptr ) {
		if ( !ptr ) {
			return;
		}
		if ( !mem_heap ) {
#ifdef CRASH_ON_STATIC_ALLOCATION
			*((int*)0x0) = 1;
#endif
			free( ptr );
			return;
		}
		mem_heap->free_aligned( ptr );
	}

	/*
	==================
	mem_cleared_alloc
	==================
	*/
	void *mem_cleared_alloc( const size_t size ) {
		void *mem = mem_alloc( size );
		memset(mem, 0, size);
		return mem;
	}

	/*
	===============
	mem_cleared_alloc_aligned
	===============
	*/
	void *mem_cleared_alloc_aligned( const size_t size, const align_t align ) {
		void *mem = mem_alloc_aligned( size, align );
		memset(mem, 0, size);
		return mem;
	}

	/*
	==================
	mem_cleared_alloc
	==================
	*/
	void mem_alloc_defrag_block( void ) {
		mem_heap->alloc_defrag_block();
	}

	/*
	==================
	mem_copy_string
	==================
	*/
	char *mem_copy_string( const char *in ) {
		char	*out;
		int		len = (int)cstr_length( in ) + 1;
		out = (char *)mem_alloc( len );
		strncpy( out, in, len);
		return out;
	}

	/*
	==================
	mem_init
	==================
	*/
    void mem_init( void )
    {
		if(!mem_heap) {
			mem_heap = new Heap;
			mem_clear_frame_stats();
		}
	}

	/*
	==================
	mem_shutdown
	==================
	*/
    void mem_shutdown( void )
    {
		if(mem_heap) {
			Heap *m = mem_heap;
			mem_heap = NULL;
			delete m;
		}
	}

	/*
	==================
	mem_enable_leak_test
	==================
	*/
	void mem_enable_leak_test( const char *name ) {
	}

#else /* !INF_DEBUG_MEMORY */

#undef		mem_alloc
#undef		mem_cleared_alloc
#undef		mem_free
#undef		mem_copy_string
#undef		mem_alloc_aligned
#undef		mem_free_aligned
#undef		mem_cleared_alloc_aligned

#define MAX_CALLSTACK_DEPTH				16
#define DEBUG_MEMORY_INFO_HASH_SIZE		1024

	typedef size_t  address_t;
	struct debugMemoryInfo_t {
		const char *			fileName;
		int						lineNumber;
		int						size;
		debugMemoryInfo_t *		next;
	};

	// size of this struct must be a multiple of 8 bytes
	struct debugMemory_t {
		debugMemoryInfo_t *		info;
		short					offset;
		debugMemory_t *			prev;
		debugMemory_t *			next;
	};

	assert_sizeof_8_byte_multiple( debugMemory_t );

	static debugMemory_t *		mem_debugMemory = NULL;
	static debugMemoryInfo_t *	mem_debugMemoryInfo[DEBUG_MEMORY_INFO_HASH_SIZE];
	static char					mem_leakName[256] = "";

	/*
	==================
	mem_CleanupFileName
	==================
	*/
	debugMemoryInfo_t *mem_FindDebugMemoryInfo( const debugMemoryInfo_t &info ) {
		int hash;
		//int i, j;
		debugMemoryInfo_t *ip;

		hash = cstr_hash( info.fileName );
		hash ^= info.lineNumber ^ info.size;
		hash &= ( DEBUG_MEMORY_INFO_HASH_SIZE - 1 );

		for ( ip = mem_debugMemoryInfo[hash]; ip != NULL; ip = ip->next ) {
			if ( ip->lineNumber != info.lineNumber ) {
				continue;
			}
			if ( ip->size != info.size ) {
				continue;
			}
			if ( cstr_cmp( ip->fileName, info.fileName ) != 0 ) {
				continue;
			}
			return ip;
		}

		ip = (debugMemoryInfo_t *) malloc( sizeof( debugMemoryInfo_t ) );
		ip->fileName = info.fileName;
		ip->lineNumber = info.lineNumber;
		ip->size = info.size;
		ip->next = mem_debugMemoryInfo[hash];
		mem_debugMemoryInfo[hash] = ip;

		return ip;
	}

	/*
	==================
	mem_CleanupFileName
	==================
	*/
	const char *mem_CleanupFileName( const char *fileName ) {
		int i1, i2;
		Str newFileName;
		static char newFileNames[4][INF_MAX_STRING_CHARS];
		static int index;

		newFileName = fileName;
		newFileName.backslashes_to_slashes();
		i1 = newFileName.find( "quack", false );
		if ( i1 >= 0 ) {
			i1 = newFileName.find( "/", false, i1 );
			newFileName = newFileName.right( newFileName.length() - ( i1 + 1 ) );
		}
		while( 1 ) {
			i1 = newFileName.find( "/../" );
			if ( i1 <= 0 ) {
				break;
			}
			i2 = i1 - 1;
			while( i2 > 1 && newFileName[i2-1] != '/' ) {
				i2--;
			}
			newFileName = newFileName.left( i2 - 1 ) + newFileName.right( newFileName.length() - ( i1 + 4 ) );
		}
		newFileName.strip_leading( "./" );
		index = ( index + 1 ) & 3;
		cstr_copynz( newFileNames[index], newFileName.c_str(), sizeof( newFileNames[index] ) );
		return newFileNames[index];
	}

	/*
	==================
	mem_dump
	==================
	*/
	void mem_dump( const char *fileName ) {
		int i, numBlocks, total_size;
		char dump[32], *ptr;
		debugMemory_t *b;
		Str module, funcName;
		FILE *f;

		f = fopen( fileName, "wb" );
		if ( !f ) {
			return;
		}

		total_size = 0;
		for ( numBlocks = 0, b = mem_debugMemory; b; b = b->next, numBlocks++ ) {
			ptr = ((char *) b) + sizeof(debugMemory_t);
			total_size += b->info->size;
			for ( i = 0; i < (sizeof(dump)-1) && i < b->info->size; i++) {
				if ( ptr[i] >= 32 && ptr[i] < 127 ) {
					dump[i] = ptr[i];
				} else {
					dump[i] = '_';
				}
			}
			dump[i] = '\0';
			if ( ( b->info->size >> 10 ) != 0 ) {
				fprintf( f, "size: %6d kB: %s, line: %d [%s]\r\n", ( b->info->size >> 10 ), mem_CleanupFileName( b->info->fileName ), b->info->lineNumber, dump );
			}
			else {
				fprintf( f, "size: %7d B: %s, line: %d [%s]\r\n", b->info->size, mem_CleanupFileName( b->info->fileName ), b->info->lineNumber, dump );
			}
		}

		fprintf( f, "%8d total memory blocks allocated\r\n", numBlocks );
		fprintf( f, "%8d kB memory allocated\r\n", ( total_size >> 10 ) );

		fclose( f );
	}

	/*
	==================
	mem_dump_compressed
	==================
	*/
	struct allocInfo_t {
		char *					fileName;
		int						lineNumber;
		int						size;
		int						numAllocs;
		address_t				callStack[MAX_CALLSTACK_DEPTH];
		allocInfo_t *			next;
	};

	void mem_dump_compressed( const char *fileName, memoryGroupType_t memGroup, memorySortType_t memSort, int sortCallStack, int numFrames, bool xlFriendly ) {
		int numBlocks, total_size, r, lineNumber;
		debugMemory_t *b;
		allocInfo_t *a, *nexta, *allocInfo = NULL, *sortedAllocInfo = NULL, *prevSorted, *nextSorted;
		Str module, funcName, path;
		FILE *f;

		// build list with memory allocations
		total_size = 0;
		numBlocks = 0;
		for ( b = mem_debugMemory; b; b = b->next ) {

			if ( numFrames ) {
				continue;
			}

			numBlocks++;
			total_size += b->info->size;

			lineNumber = b->info->lineNumber;

			if ( memGroup == MEMGROUP_FOLDER ) {
				//path.StripFilename();
				path += "\\";
				lineNumber = cstr_hash( path );
			} else if ( memGroup == MEMGROUP_FILE ) {
				lineNumber = cstr_hash( path );
			}

			// search for an allocation from the same source location
			for ( a = allocInfo; a; a = a->next ) {
				if ( a->lineNumber != lineNumber ) {
					continue;
				}
				if ( path.cmp( a->fileName ) != 0 ) {
					continue;
				}
				a->numAllocs++;
				a->size += b->info->size;
				break;
			}

			// if this is an allocation from a new source location
			if ( !a ) {
				int len = path.length() + 1;
				a = (allocInfo_t *) ::malloc( sizeof( allocInfo_t ) + len );
				a->fileName = ((char *)a) + sizeof( allocInfo_t );
				cstr_copynz( a->fileName, path, len );
				a->lineNumber = lineNumber;
				a->size = b->info->size;
				a->next = allocInfo;
				allocInfo = a;
			}
		}

		// sort list
		for ( a = allocInfo; a; a = nexta ) {
			nexta = a->next;

			prevSorted = NULL;
			switch( memSort ) {
				// sort on size
			case MEMSORT_SIZE:
				{
					for ( nextSorted = sortedAllocInfo; nextSorted; nextSorted = nextSorted->next ) {
						if ( a->size > nextSorted->size ) {
							break;
						}
						prevSorted = nextSorted;
					}
					break;
				}
				// sort on file name and line number
			case MEMSORT_LOCATION:
				{
					for ( nextSorted = sortedAllocInfo; nextSorted; nextSorted = nextSorted->next ) {
						r = cstr_icmp_path( a->fileName, nextSorted->fileName );
						if ( r < 0 || ( r == 0 && a->lineNumber < nextSorted->lineNumber ) ) {
							break;
						}
						prevSorted = nextSorted;
					}
					break;
				}
				// sort on the number of allocations
			case MEMSORT_NUMALLOCS:
				{
					for ( nextSorted = sortedAllocInfo; nextSorted; nextSorted = nextSorted->next ) {
						if ( a->numAllocs > nextSorted->numAllocs ) {
							break;
						}
						prevSorted = nextSorted;
					}
					break;
				}
				// sort on call stack
			}

			if ( !prevSorted ) {
				a->next = sortedAllocInfo;
				sortedAllocInfo = a;
			} else {
				prevSorted->next = a;
				a->next = nextSorted;
			}
		}

		f = fopen( fileName, "wb" );
		if ( !f ) {
			warning( "Could not open '%s' for writing", fileName );
			return;
		}

		if ( xlFriendly ) {
			if ( memGroup == MEMGROUP_FILE || memGroup == MEMGROUP_FOLDER ) {
				fprintf( f, "  total_size Allocs File\r\n" );
			} else {
				fprintf( f, "  total_size Allocs  Line File Call Stack\r\n" );
			}
		}
		// write list to file
		if ( xlFriendly ) {
			if ( memGroup == MEMGROUP_FILE || memGroup == MEMGROUP_FOLDER ) {
				for ( a = sortedAllocInfo; a; a = nexta ) {
					nexta = a->next;
					fprintf( f, "%6d %6d %s\r\n", ( a->size >> 10 ), a->numAllocs, a->fileName );
					::free( a );
				}
			} else {
				for ( a = sortedAllocInfo; a; a = nexta ) {
					nexta = a->next;
					fprintf( f, "%6d %6d %5d %s\r\n", ( a->size >> 10 ), a->numAllocs, a->lineNumber, a->fileName );
					::free( a );
				}
			}
		} else {
			if ( memGroup == MEMGROUP_FILE || memGroup == MEMGROUP_FOLDER ) {
				for ( a = sortedAllocInfo; a; a = nexta ) {
					nexta = a->next;
					fprintf( f, "size: %6d kB, allocs: %5d: %s\r\n", ( a->size >> 10 ), a->numAllocs, a->fileName );
					::free( a );
				}
			} else {
				for ( a = sortedAllocInfo; a; a = nexta ) {
					nexta = a->next;
					fprintf( f, "size: %6d kB, allocs: %5d: %s, line: %d\r\n", ( a->size >> 10 ), a->numAllocs, a->fileName, a->lineNumber );
					::free( a );
				}
			}
		}

		if ( !xlFriendly ) {
			fprintf( f, "%8d total memory blocks allocated\r\n", numBlocks );
			fprintf( f, "%8d KB memory allocated\r\n", ( total_size >> 10 ) );
		}

		fclose( f );
	}


	/*
	==================
	mem_dump_per_class
	==================
	*/
	//struct allocClass_t {
	//    char				className[INF_MAX_STRING_CHARS];
	//    int					total_size;
	//    int					numAllocs;
	//    allocClass_t *		next;
	//};

#define ALLOC_CLASS_HASH_SIZE		4096

	void mem_dump_per_class( const char *fileName ) {
		int numBlocks, totalBlocks, total_size;
		debugMemory_t *b;
		FILE *f;

		f = fopen( fileName, "wb" );
		if ( !f ) {
			return;
		}

		for ( totalBlocks = 0, b = mem_debugMemory; b != NULL; b = b->next, totalBlocks++ ) {
		}

		int lastPercentage = 0;

		total_size = 0;
		for ( numBlocks = 0, b = mem_debugMemory; b != NULL; b = b->next, numBlocks++ ) {
			total_size += b->info->size;


			int percentage = numBlocks * 100 / totalBlocks;
			if ( percentage != lastPercentage ) {
				printf( "\r%3d%%", percentage );
				lastPercentage = percentage;
			}
		}

		printf( "\r100%%" );


		fprintf( f, "%8d total memory blocks allocated\r\n", numBlocks );
		fprintf( f, "%8d kB memory allocated\r\n", ( total_size >> 10 ) );

		fclose( f );
	}

	/*
	==================
	mem_allocDebugMemory
	==================
	*/
	void *mem_allocDebugMemory( const size_t size, const char *fileName, const int lineNumber, const align_t align ) {
		void *p;
		debugMemory_t *m;
		debugMemoryInfo_t info;
		short offset = 0;

		if ( !size ) {
			return NULL;
		}

		if ( !mem_heap ) {
#ifdef CRASH_ON_STATIC_ALLOCATION
			*((int*)0x0) = 1;
#endif
			// NOTE: set a breakpoint here to find memory allocations before mem_heap is initialized
			return malloc( size );
		}

		if ( align > ALIGN_8 ) {
			offset = ( ( sizeof( debugMemory_t ) + align - 1 ) / align ) * align - sizeof( debugMemory_t ) ;
			p = mem_heap->AllocateAligned( size + sizeof( debugMemory_t ) + offset, align );
		} else {
			p = mem_heap->allocate( size + sizeof( debugMemory_t ) );
		}

		mem_UpdateAllocStats( size );

		info.fileName = fileName;
		info.lineNumber = lineNumber;
		info.size = size;
		info.next = NULL;

		m = (debugMemory_t *) ( ( (byte *) p ) + offset );
		m->info = mem_FindDebugMemoryInfo( info );
		m->offset = offset;
		m->next = mem_debugMemory;
		m->prev = NULL;
		if ( mem_debugMemory ) {
			mem_debugMemory->prev = m;
		}
		mem_debugMemory = m;

		return ( ( (byte *) p ) + sizeof( debugMemory_t ) + offset );
	}

	/*
	==================
	mem_freeDebugMemory
	==================
	*/
	void mem_freeDebugMemory( void *p, const char *fileName, const int lineNumber, const bool aligned ) {
		debugMemory_t *m;
		short offset;

		if ( !p ) {
			return;
		}

		if ( !mem_heap ) {
#ifdef CRASH_ON_STATIC_ALLOCATION
			*((int*)0x0) = 1;
#endif
			// NOTE: set a breakpoint here to find memory being freed before mem_heap is initialized
			free( p );
			return;
		}

		m = (debugMemory_t *) ( ( (byte *) p ) - sizeof( debugMemory_t ) );

		if ( m->offset < 0 ) {
			fatal_error( "memory freed twice. file: %s, line number: %d, aligned: %n\n", fileName, lineNumber, aligned );
		}

		mem_UpdateFreeStats( m->info->size );

		if ( m->next ) {
			m->next->prev = m->prev;
		}
		if ( m->prev ) {
			m->prev->next = m->next;
		} else {
			mem_debugMemory = m->next;
		}

		offset = m->offset;
		m->offset = -1;

		if ( aligned ) {
			mem_heap->FreeAligned( ( ( byte * )m ) - offset );
		} else {
			mem_heap->free( m );
		}
	}

	/*
	==================
	mem_alloc
	==================
	*/
	void *mem_alloc( const size_t size, const char *fileName, const int lineNumber ) {
		if ( !size ) {
			return NULL;
		}
		return mem_allocDebugMemory( size, fileName, lineNumber, ALIGN_8 );
	}

	/*
	==================
	mem_free
	==================
	*/
	void mem_free( void *ptr, const char *fileName, const int lineNumber ) {
		if ( !ptr ) {
			return;
		}
		mem_freeDebugMemory( ptr, fileName, lineNumber, false );
	}

	/*
	==================
	mem_alloc_aligned
	==================
	*/
	void *mem_alloc_aligned( const size_t size, const align_t align, const char *fileName, const int lineNumber ) {
		if ( !size ) {
			return NULL;
		}
		void *mem = mem_allocDebugMemory( size, fileName, lineNumber, align );
		// make sure the memory is aligned
		assert( align == ALIGN_NONE || ( ((int)mem) & (align-1)) == 0 );
		return mem;
	}

	/*
	==================
	mem_free_aligned
	==================
	*/
	void mem_free_aligned( void *ptr, const char *fileName, const int lineNumber ) {
		if ( !ptr ) {
			return;
		}
		mem_freeDebugMemory( ptr, fileName, lineNumber, true );
	}

	/*
	==================
	mem_cleared_alloc
	==================
	*/
	void *mem_cleared_alloc( const size_t size, const char *fileName, const int lineNumber ) {
		void *mem = mem_alloc( size, fileName, lineNumber );
		mem_set( mem, 0, size );
		return mem;
	}

	/*
	===============
	mem_cleared_alloc_aligned
	===============
	*/
	void *mem_cleared_alloc_aligned( const size_t size, const align_t align, const char *fileName, const int lineNumber ) {
		void *mem = mem_alloc_aligned( size, align, fileName, lineNumber );
		memset( mem, 0, size );
		return mem;
	}

	/*
	==================
	mem_copy_string
	==================
	*/
	char *mem_copy_string( const char *in, const char *fileName, const int lineNumber ) {
		char	*out;

		out = (char *)mem_alloc( cstr_length( in ) + 1, fileName, lineNumber );
		cstr_copynz( out, in, cstr_length( in ) + 1 );
		return out;
	}

	/*
	==================
	mem_init
	==================
	*/
	void mem_init( void ) {
		if(!mem_heap)
			mem_heap = new Heap;
	}

	/*
	==================
	mem_shutdown
	==================
	*/
	void mem_shutdown( void ) {

		char fileName[INF_MAX_STRING_CHARS] = {0};


		if ( mem_leakName[0] != '\0' ) {
			Str::snPrintf( fileName, INF_MAX_STRING_CHARS, "%s_leak_size.txt", mem_leakName );
			mem_dump_compressed( fileName, MEMGROUP_LINE, MEMSORT_SIZE, 0, 0, false );
			Str::snPrintf( fileName, INF_MAX_STRING_CHARS, "%s_leak_location.txt", mem_leakName );
			mem_dump_compressed( fileName, MEMGROUP_LINE, MEMSORT_LOCATION, 0, 0, false );
		}

		if( mem_heap ) {
			Heap *m = mem_heap;
			mem_heap = NULL;
			delete m;
		}
	}

	/*
	==================
	mem_enable_leak_test
	==================
	*/
	void mem_enable_leak_test( const char *name ) {
		cstr_copynz( mem_leakName, name, sizeof( mem_leakName ) );
	}
#endif /* !INF_DEBUG_MEMORY */


}   // ::inf

