#ifndef Endian_h__
#define Endian_h__

// little/big endian conversion

namespace inf {
    bool				is_big_endian( void );
    void				endian_init( void );

    short				big_short( short l );
    short				little_short( short l );
    int					big_long( int l );
    int					little_long( int l );
    float				big_float( float l );
    float				little_float( float l );
    double              big_double( double l );
    double				little_double( double l );
    void				big_rev_bytes( void *bp, int elsize, int elcount );
    void				little_rev_bytes( void *bp, int elsize, int elcount );
    void				little_bit_field( void *bp, int elsize );

    inline void		swap_little_short( short &c ) { c = little_short( c ); }
    inline void		swap_little_unsigned_short( unsigned short &c ) { c = little_short( c ); }
    inline void		swap_little_int( int &c ) { c = little_long( c ); }
    inline void		swap_little_unsigned_int( unsigned int &c ) { c = little_long( c ); }
    inline void		swap_little_float( float &c ) { c = little_float( c ); }
    template<class type>
    void            swap_little_float_class( type &c ) { for ( int i = 0; i < c.get_dimension(); i++ ) { c.to_float_ptr()[i] = little_float( c.to_float_ptr()[i] ); } }

    void            swap_bytes(unsigned char* pBytes, int byteNum);

    inline unsigned int swap_4byte(unsigned int ui32Long)
    {
        return ((ui32Long&0x000000FF)<<24) + ((ui32Long&0x0000FF00)<<8) + ((ui32Long&0x00FF0000)>>8) + ((ui32Long&0xFF000000) >> 24);
    }

    inline unsigned short swap_2byte(unsigned short ui16Short)
    {
        return (ui16Short>>8) | (ui16Short<<8);
    }

}   // ::inf

#endif // Endian_h__
