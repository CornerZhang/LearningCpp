#include "endian.h"
#include <algorithm>

namespace inf {
    using byte = unsigned char;
    
    /*
    ===============================================================================

    Byte order functions

    ===============================================================================
    */

    // can't just use function pointers, or dll linkage can mess up
    static short	(*_big_short)( short l );
    static short	(*_little_short)( short l );
    static int		(*_big_long)( int l );
    static int		(*_little_long)( int l );
    static float	(*_big_float)( float l );
    static float	(*_little_float)( float l );
    static double	(*_little_double)( double l );
    static void		(*_big_rev_bytes)( void *bp, int elsize, int elcount );
    static void		(*_little_rev_rytes)( void *bp, int elsize, int elcount );
    static void     (*_LittleBitField)( void *bp, int elsize );
    static void		(*_SixtetsForInt)( byte *out, int src );
    static int		(*_IntForSixtets)( byte *in );

    short	big_short( short l ) { return _big_short( l ); }
    short	little_short( short l ) { return _little_short( l ); }
    int		big_long( int l ) { return _big_long( l ); }
    int		little_long( int l ) { return _little_long( l ); }
    float	big_float( float l ) { return _big_float( l ); }
    float	little_float( float l ) { return _little_float( l ); }
    double	little_double( double l ) { return _little_double( l ); }
    void	big_rev_bytes( void *bp, int elsize, int elcount ) { _big_rev_bytes( bp, elsize, elcount ); }
    void	little_rev_bytes( void *bp, int elsize, int elcount ) { _little_rev_rytes( bp, elsize, elcount ); }
    void	little_bit_field( void *bp, int elsize ) { _LittleBitField( bp, elsize ); }

    void	SixtetsForInt( byte *out, int src ) { _SixtetsForInt( out, src ); }
    int		IntForSixtets( byte *in ) { return _IntForSixtets( in ); }

    /*
    ================
    ShortSwap
    ================
    */
    short ShortSwap( short l ) {
        byte    b1,b2;

        b1 = l&255;
        b2 = (l>>8)&255;

        return (b1<<8) + b2;
    }

    /*
    ================
    ShortNoSwap
    ================
    */
    short ShortNoSwap( short l ) {
        return l;
    }

    /*
    ================
    LongSwap
    ================
    */
    int LongSwap ( int l ) {
        byte    b1,b2,b3,b4;

        b1 = l&255;
        b2 = (l>>8)&255;
        b3 = (l>>16)&255;
        b4 = (l>>24)&255;

        return ((int)b1<<24) + ((int)b2<<16) + ((int)b3<<8) + b4;
    }

    /*
    ================
    LongNoSwap
    ================
    */
    int	LongNoSwap( int l ) {
        return l;
    }

    /*
    ================
    FloatSwap
    ================
    */
    float FloatSwap( float f ) {
        union {
            float	f;
            byte	b[4];
        } dat1, dat2;


        dat1.f = f;
        dat2.b[0] = dat1.b[3];
        dat2.b[1] = dat1.b[2];
        dat2.b[2] = dat1.b[1];
        dat2.b[3] = dat1.b[0];
        return dat2.f;
    }

    /*
    ================
    FloatNoSwap
    ================
    */
    float FloatNoSwap( float f ) {
        return f;
    }

    /*
    ================
    DoubleSwap
    ================
    */
    double DoubleSwap( double d ) {
        union {
            double	d;
            byte	b[8];
        } dat1, dat2;


        dat1.d = d;
        dat2.b[0] = dat1.b[7];
        dat2.b[1] = dat1.b[6];
        dat2.b[2] = dat1.b[5];
        dat2.b[3] = dat1.b[4];
        dat2.b[4] = dat1.b[3];
        dat2.b[5] = dat1.b[2];
        dat2.b[6] = dat1.b[1];
        dat2.b[7] = dat1.b[0];
        return dat2.d;
    }

    /*
    ================
    DoubleNoSwap
    ================
    */
    double DoubleNoSwap( double d ) {
        return d;
    }

    /*
    =====================================================================
    RevBytesSwap

    Reverses byte order in place.

    INPUTS
    bp       bytes to reverse
    elsize   size of the underlying data type
    elcount  number of elements to swap

    RESULTS
    Reverses the byte order in each of elcount elements.
    =====================================================================
    */
    void RevBytesSwap( void *bp, int elsize, int elcount ) {
        unsigned char *p, *q;

        p = ( unsigned char * ) bp;

        if ( elsize == 1 ) {
            return;
        }

        if ( elsize == 2 ) {
            q = p + 1;
            while( elcount-- ) {
                *p ^= *q;
                *q ^= *p;
                *p ^= *q;
                p += 2;
                q += 2;
            }
            return;
        }

        while( elcount-- ) {
            q = p + elsize - 1;
            while( p < q ) {
                *p ^= *q;
                *q ^= *p;
                *p ^= *q;
                ++p;
                --q;
            }
            p += elsize >> 1;
        }
    }

    /*
    =====================================================================
    RevBitFieldSwap

    Reverses byte order in place, then reverses bits in those bytes

    INPUTS
    bp       bitfield structure to reverse
    elsize   size of the underlying data type

    RESULTS
    Reverses the bitfield of size elsize.
    =====================================================================
    */
    void RevBitFieldSwap( void *bp, int elsize) {
        int i;
        unsigned char *p, t, v;

        little_rev_bytes( bp, elsize, 1 );

        p = (unsigned char *) bp;
        while( elsize-- ) {
            v = *p;
            t = 0;
            for ( i = 7; i; i-- ) {
                t <<= 1;
                v >>= 1;
                t |= v & 1;
            }
            *p++ = t;
        }
    }

    /*
    ================
    RevBytesNoSwap
    ================
    */
    void RevBytesNoSwap( void *bp, int elsize, int elcount ) {
        return;
    }

    /*
    ================
    RevBytesNoSwap
    ================
    */
    void RevBitFieldNoSwap( void *bp, int elsize ) {
        return;
    }

    /*
    ================
    SixtetsForIntLittle
    ================
    */
    void SixtetsForIntLittle( byte *out, int src) {
        byte *b = (byte *)&src;
        out[0] = ( b[0] & 0xfc ) >> 2;
        out[1] = ( ( b[0] & 0x3 ) << 4 ) + ( ( b[1] & 0xf0 ) >> 4 );
        out[2] = ( ( b[1] & 0xf ) << 2 ) + ( ( b[2] & 0xc0 ) >> 6 );
        out[3] = b[2] & 0x3f;
    }

    /*
    ================
    SixtetsForIntBig
    TTimo: untested - that's the version from initial base64 encode
    ================
    */
    void SixtetsForIntBig( byte *out, int src) {
        for( int i = 0 ; i < 4 ; i++ ) {
            out[i] = src & 0x3f;
            src >>= 6;
        }
    }

    /*
    ================
    IntForSixtetsLittle
    ================
    */
    int IntForSixtetsLittle( byte *in ) {
        int ret = 0;
        byte *b = (byte *)&ret;
        b[0] |= in[0] << 2;
        b[0] |= ( in[1] & 0x30 ) >> 4;
        b[1] |= ( in[1] & 0xf ) << 4;
        b[1] |= ( in[2] & 0x3c ) >> 2;
        b[2] |= ( in[2] & 0x3 ) << 6;
        b[2] |= in[3];
        return ret;
    }

    /*
    ================
    IntForSixtetsBig
    TTimo: untested - that's the version from initial base64 decode
    ================
    */
    int IntForSixtetsBig( byte *in ) {
        int ret = 0;
        ret |= in[0];
        ret |= in[1] << 6;
        ret |= in[2] << 2*6;
        ret |= in[3] << 3*6;
        return ret;
    }

    bool is_big_endian( void ) {
        short s = 256;
        return ( *((byte *)&s) != 0 );
    }

    /*
    ================
    endian_init
    ================
    */
    void endian_init( void ) {

        // set the byte swapping variables in a portable manner	
        if ( is_big_endian() ) {
            // big endian ex: ppc
            _big_short = ShortNoSwap;
            _little_short = ShortSwap;
            _big_long = LongNoSwap;
            _little_long = LongSwap;
            _big_float = FloatNoSwap;
            _little_float = FloatSwap;
            _little_double = DoubleSwap;
            _big_rev_bytes = RevBytesNoSwap;
            _little_rev_rytes = RevBytesSwap;
            _LittleBitField = RevBitFieldSwap;
            _SixtetsForInt = SixtetsForIntBig;
            _IntForSixtets = IntForSixtetsBig;
        } else {
            // little endian ex: x86
            _big_short = ShortSwap;
            _little_short = ShortNoSwap;
            _big_long = LongSwap;
            _little_long = LongNoSwap;
            _big_float = FloatSwap;
            _little_float = FloatNoSwap;
            _little_double = DoubleNoSwap;
            _big_rev_bytes = RevBytesSwap;
            _little_rev_rytes = RevBytesNoSwap;
            _LittleBitField = RevBitFieldNoSwap;
            _SixtetsForInt = SixtetsForIntLittle;
            _IntForSixtets = IntForSixtetsLittle;
        }
    }

    void swap_bytes(unsigned char* pBytes, int byteNum)
    {
        int i = 0, j = byteNum - 1;
        
        while(i < j)
            std::swap<unsigned char>(pBytes[i++], pBytes[j--]);
    }

}   // ::inf
