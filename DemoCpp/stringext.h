#ifndef stringext_h
#define stringext_h

#include "constliteral.h"
#include <cassert>

//#define INTSIGNBITNOTSET(i)		((~((const unsigned int)(i))) >> 31)

namespace inf {
    template <typename type>
    constexpr const unsigned int INTSIGNBITNOTSET(type i) {
        return (~((const unsigned int)(i))) >> 31;
    }
    
    static const int INVALID_POSITION = -1;

    
    
    
    // char
    bool char_is_printable( int c ) {
        // test for regular ascii and western European high-ascii chars
        return ( c >= 0x20 && c <= 0x7E ) || ( c >= 0xA1 && c <= 0xFF );
    }
    
    template<typename char_type>
    bool char_is_lower( char_type c ) {
        // test for regular ascii and western European high-ascii chars
        return ( c >= const_char<char_type>::a && c <= const_char<char_type>::z ) || ( c >= 0xE0 && c <= 0xFF );
    }
    
    template<typename char_type>
    bool char_is_upper( char_type c ) {
        // test for regular ascii and western European high-ascii chars
        return ( c <= const_char<char_type>::Z && c >= const_char<char_type>::A ) || ( c >= 0xC0 && c <= 0xDF );
    }
    
    template<typename char_type>
    bool char_is_alpha( char_type c ) {
        // test for regular ascii and western European high-ascii chars
        return ( ( c >= const_char<char_type>::a && c <= const_char<char_type>::z ) || ( c >= const_char<char_type>::A && c <= const_char<char_type>::Z ) ||
                ( c >= 0xC0 && c <= 0xFF ) );
    }
    
    template<typename char_type>
    bool char_is_numeric( char_type c ) {
        return ( c <= const_char<char_type>::_9 && c >= const_char<char_type>::_0 );
    }
    
    template<typename char_type>
    bool char_is_newline( char_type c ) {
        return ( c == const_char<char_type>::newline || c == const_char<char_type>::return_r || c == const_char<char_type>::vertical_table );
    }
    
    template<typename char_type>
    bool char_is_tab( char_type c ) {
        return ( c == const_char<char_type>::table );
    }
    
    template<typename char_type>
    bool char_is_punctuation( char_type c ) {
        return ( !char_is_alpha(c) && !char_is_numeric(c) && !char_is_printable(c) && !char_is_newline(c) );
    }
    
    template<typename char_type>
    bool char_is_hex( char_type c ) {
        return ( ( c >= const_char<char_type>::_0 && c <= const_char<char_type>::_9 ) || ( c >= const_char<char_type>::A && c <= const_char<char_type>::F ) || ( c >= const_char<char_type>::a && c <= const_char<char_type>::f ) );
    }
    
    template<typename char_type>
    int hex_for_char( char_type c ) {
        return ( c > const_char<char_type>::_9 ? ( c >= const_char<char_type>::a ? ( c - const_char<char_type>::a + 10 ) : ( c - const_char<char_type>::_7 ) ) : ( c - const_char<char_type>::_0 ) );
    }
    
    template<typename char_type>
    char_type to_lower( char_type c ) {
        if ( c <= const_char<char_type>::Z && c >= const_char<char_type>::A ) {
            return ( c + ( const_char<char_type>::a - const_char<char_type>::A ) );
        }
        return c;
    }
    
    template<typename char_type>
    char_type to_upper( char_type c ) {
        if ( c >= const_char<char_type>::a && c <= const_char<char_type>::z ) {
            return ( c - ( const_char<char_type>::a - const_char<char_type>::A ) );
        }
        return c;
    }
    
    
    
    
    // string
    template <typename char_type>
    std::size_t length(const char_type* s) {
        std::size_t i=0;
        for (; s[i] != const_char<char_type>::zero; ++i) {}
        return i;
    }
    
    template<typename char_type>
    std::size_t count_char( const char_type *str, const char_type c ) {
        std::size_t i, count = 0;
        for ( i = 0; str[i] != const_char<char_type>::zero; i++ ) {
            if ( str[i] == c ) {
                count++;
            }
        }
        return count;
    }
    
    template<typename char_type>
    std::size_t hash(const char_type* string, bool casesensitive = true) {
        std::size_t i, hash = 0;
        if( casesensitive ) {
            for ( i = 0; *string != const_char<char_type>::zero; i++ ) {
                hash += ( *string++ ) * ( i + 119 );
            }
        } else {
            for( i = 0; *string != const_char<char_type>::zero; i++ ) {
                hash += to_lower( *string++ ) * ( i + 119 );
            }
        }
        return hash;
    }
    
    template<typename char_type>
    std::size_t hash( const char_type* string, int length, bool casesensitive = true) {
        std::size_t i, hash = 0;
        if( casesensitive ) {
            for ( i = 0; i < length; i++ ) {
                hash += ( *string++ ) * ( i + 119 );
            }
        } else {
            for ( i = 0; i < length; i++ ) {
                hash += to_lower( *string++ ) * ( i + 119 );
            }
        }
        return hash;
    }
    
    template<typename char_type>
    bool check_extension( const char_type *name, const char_type *ext ) {
        const char_type *s1 = name + length( name ) - 1;
        const char_type *s2 = ext + length( ext ) - 1;
        char_type c1, c2, d;
        
        do {
            c1 = *s1--;
            c2 = *s2--;
            
            d = c1 - c2;
            while( d ) {
                if ( c1 <= const_char<char_type>::Z && c1 >= const_char<char_type>::A ) {
                    d += (const_char<char_type>::a - const_char<char_type>::A);
                    if ( !d ) {
                        break;
                    }
                }
                if ( c2 <= const_char<char_type>::Z && c2 >= const_char<char_type>::A ) {
                    d -= (const_char<char_type>::a - const_char<char_type>::A);
                    if ( !d ) {
                        break;
                    }
                }
                return false;
            }
        } while( s1 >= name && s2 >= ext );
        
        return ( s1 >= name ) && ( *s1 == const_char<char_type>::dot );
    }
    

    
    template<typename char_type>
    bool is_numeric( const char_type *s ) {
        std::size_t		i;
        bool	dot;
        
        if ( *s == const_char<char_type>::sub ) {
            s++;
        }
        
        dot = false;
        for ( i = 0; s[i]; i++ ) {
            if ( !std::isdigit( s[i] ) ) {	// crt function
                if ( ( s[ i ] == const_char<char_type>::dot ) && !dot ) {
                    dot = true;
                    continue;
                }
                return false;
            }
        }
        
        return true;
    }
    
    template<typename char_type>
    bool is_alpha( const char_type *s ) {
        int		i;
        for ( i = 0; s[i]; i++ ) {
            if ( !char_is_alpha(s[i]) ) {
                return false;
            }
        }
        
        return true;
    }
    
    template<typename char_type>
    bool has_lower( const char_type *s ) {
        if ( !s ) {
            return false;
        }
        
        while ( *s ) {
            if ( char_is_lower( *s ) ) {
                return true;
            }
            s++;
        }
        
        return false;
    }
    
    template<typename char_type>
    bool has_upper( const char_type *s ) {
        if ( !s ) {
            return false;
        }
        
        while ( *s ) {
            if ( char_is_upper( *s ) ) {
                return true;
            }
            s++;
        }
        
        return false;
    }
    
    template<typename char_type>
    bool is_quad_string(const char_type* string, int length, char_type with_quad_char) {
        if( (string[0] == with_quad_char) && (string[length-1] == with_quad_char) )
            return true;
        return false;
    }
    
    template<typename char_type>
    int cmp( const char_type *s1, const char_type *s2, bool casesensitive = true ) {
        int c1, c2, d;
        
        if( casesensitive ) {
            do {
                c1 = *s1++;
                c2 = *s2++;
                
                d = c1 - c2;
                if ( d ) {
                    return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;	// INTSIGNBITNOTSET in Mathematics.h
                }
            } while( c1 );
        }else {
            do {
                c1 = *s1++;
                c2 = *s2++;
                
                d = c1 - c2;
                const int Offset = const_char<char_type>::a - const_char<char_type>::A;
                while( d ) {
                    if ( c1 <= const_char<char_type>::Z && c1 >= const_char<char_type>::A ) {
                        d += Offset;
                        if ( !d ) {
                            break;
                        }
                    }
                    if ( c2 <= const_char<char_type>::A && c2 >= const_char<char_type>::Z ) {
                        d -= Offset;
                        if ( !d ) {
                            break;
                        }
                    }
                    return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
                }
            } while( c1 );
        }
        
        
        return 0;		// strings are equal
        
    }
    
    template<typename char_type>
    int cmpn( const char_type *s1, const char_type *s2, int n, bool casesensitive = true ) {
        int c1, c2, d;
        
        assert( n >= 0 );
        
        if( casesensitive ) {
            do {
                c1 = *s1++;
                c2 = *s2++;
                
                if ( !n-- ) {
                    return 0;		// strings are equal until end point
                }
                
                d = c1 - c2;
                if ( d ) {
                    return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
                }
            } while( c1 );
            
        } else {
            do {
                c1 = *s1++;
                c2 = *s2++;
                
                if ( !n-- ) {
                    return 0;		// strings are equal until end point
                }
                
                d = c1 - c2;
                const int Offset = const_char<char_type>::a - const_char<char_type>::A;
                while( d ) {
                    if ( c1 <= const_char<char_type>::Z && c1 >= const_char<char_type>::A ) {
                        d += Offset;
                        if ( !d ) {
                            break;
                        }
                    }
                    if ( c2 <= const_char<char_type>::Z && c2 >= const_char<char_type>::A ) {
                        d -= Offset;
                        if ( !d ) {
                            break;
                        }
                    }
                    return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
                }
            } while( c1 );
        }
        return 0;		// strings are equal
    }
    
    template<typename char_type>
    int cmp_prefix( const char_type* s1, const char_type* s2, bool casesensitive = true ) {
        assert( s1 );
        assert( s2 );
        return cmpn( s1, s2, length( s2 ), casesensitive );
    }
    
    template<typename char_type>
    int icmp_path( const char_type *s1, const char_type *s2 ) {
        int c1, c2, d;
        
        do {
            c1 = *s1++;
            c2 = *s2++;
            
            d = c1 - c2;
            while( d ) {
                if ( c1 <= const_char<char_type>::Z && c1 >= const_char<char_type>::A ) {
                    d += (const_char<char_type>::a - const_char<char_type>::A);
                    if ( !d ) {
                        break;
                    }
                }
                if ( c1 == const_char<char_type>::back_slash ) {
                    d += (const_char<char_type>::slash - const_char<char_type>::back_slash);
                    if ( !d ) {
                        break;
                    }
                }
                if ( c2 <= const_char<char_type>::Z && c2 >= const_char<char_type>::A ) {
                    d -= (const_char<char_type>::a - const_char<char_type>::A);
                    if ( !d ) {
                        break;
                    }
                }
                if ( c2 == const_char<char_type>::back_slash ) {
                    d -= (const_char<char_type>::slash - const_char<char_type>::back_slash);
                    if ( !d ) {
                        break;
                    }
                }
                // make sure folders come first
                while( c1 ) {
                    if ( c1 == const_char<char_type>::slash || c1 == const_char<char_type>::back_slash ) {
                        break;
                    }
                    c1 = *s1++;
                }
                while( c2 ) {
                    if ( c2 == const_char<char_type>::slash || c2 == const_char<char_type>::back_slash ) {
                        break;
                    }
                    c2 = *s2++;
                }
                if ( c1 && !c2 ) {
                    return -1;
                } else if ( !c1 && c2 ) {
                    return 1;
                }
                // same folder depth so use the regular compare
                return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
            }
        } while( c1 );
        
        return 0;
    }
    
    template<typename char_type>
    double to_double(const char_type* string) {
        unsigned long long val;
        double m = (string[0] == const_char<char_type>::sub ) ? -1.0 : 1.0;
        int i = 0;
        
        if ( string[0] == const_char<char_type>::sub  || string[0] == const_char<char_type>::add )
            i++;
        int max = 17 + i;
        for ( val=0; char_is_numeric(string[i]) && i<max; i++ )
            val = 10 * val + (string[i] - const_char<char_type>::_0);
        
        if ( string[i] == const_char<char_type>::dot ) {
            i++;
            
            for ( ; char_is_numeric(string[i]) && i<max; i++ ) {
                val = 10 * val + (string[i] - const_char<char_type>::_0);
                m *= 0.1;
            }
        }
        return val * m;
    }
    
    template<typename char_type>
    float to_float(const char_type* string) {
        unsigned long val;
        float m = (string[0] == const_char<char_type>::sub ) ? -1.0f : 1.0f;
        int i = 0;
        
        if ( string[0] == const_char<char_type>::sub  || string[0] == const_char<char_type>::add )
            i++;
        int max = 8 + i;
        for ( val=0; char_is_numeric(string[i]) && i<max; i++ )
            val = 10 * val + (string[i] - const_char<char_type>::_0);
        
        if ( string[i] == const_char<char_type>::Dot ) {
            i++;
            
            for ( ; char_is_numeric(string[i]) && i<max; i++ ) {
                val = 10 * val + (string[i] - const_char<char_type>::_0);
                m *= 0.1f;
            }
        }
        return val * m;
    }
    
    template<typename char_type>
    int	to_integer(const char_type* string) {
        int val = 0;
        int i = 0;
        int m = (string[0] == const_char<char_type>::sub ) ? -1 : 1;
        
        if ( string[0] == const_char<char_type>::sub  || string[0] == const_char<char_type>::add )
            i++;
        
        int max = 10 + i;
        for ( ; char_is_numeric(string[i]) && i<max; i++ )
            val = 10 * val + (string[i] - const_char<char_type>::_0);
        
        return val * m;
    }
    
    template<typename char_type>
    bool to_float_array( float *dstF, int floatNum, const char_type *srcText, const char_type splitChar = const_char<char_type>::comma) {
        unsigned long val;
        float div;
        int i = 0;
        int j, max;
        
        for ( j=0; j < floatNum; j++ ) {
            if ( j && srcText[i-1] != splitChar )
                break;
            
            div = (srcText[i] == const_char<char_type>::sub ) ? -1.0f : 1.0f;
            
            if ( srcText[i] == const_char<char_type>::sub  || srcText[i] == const_char<char_type>::add )
                i++;
            
            if ( char_is_numeric(srcText[i]) )
                break;
            
            max = 8 + i;
            for ( val=0; char_is_numeric(srcText[i]) && i<max; i++ )
                val = 10 * val + (srcText[i] - const_char<char_type>::_0);
            
            //! @todo	fix this nicer
            // skip remaining digits we where not able to read.
            for ( ; char_is_numeric(srcText[i]); i++ )
                continue;
            
            if ( srcText[i] == const_char<char_type>::dot ) {
                for ( i++; char_is_numeric(srcText[i]) && i<max; i++ ) {
                    val = 10 * val + (srcText[i] - const_char<char_type>::_0);
                    div *= 10;
                }
                //! @todo	fix this nicer
                // skip remaining digits we where not able to read.
                for ( ; char_is_numeric(srcText[i]); i++ )
                    continue;
            }
            dstF[j] = val / div;
            
            i++;
        }
        if ( j != floatNum ) {
            while(j<floatNum)
                dstF[j++] = 0.0f;
            return false;
        }
        return true;
    }
    
    template<typename char_type>
    bool to_double_array( double *dstD, int doubleNum, const char_type *srcText, const char_type splitChar = const_char<char_type>::comma) {
        unsigned long long val;
        double div;
        int i = 0;
        int j, max;
        
        for ( j=0; j < doubleNum; j++ ) {
            if ( j && srcText[i-1] != splitChar )
                break;
            
            div = (srcText[i] == const_char<char_type>::sub ) ? -1.0 : 1.0;
            
            if ( srcText[i] == const_char<char_type>::sub  || srcText[i] == const_char<char_type>::add )
                i++;
            
            if ( !char_is_numeric(srcText[i]) )
                break;
            
            max = 17 + i;
            for ( val=0; char_is_numeric(srcText[i]) && i<max; i++ )
                val = 10 * val + (srcText[i] - const_char<char_type>::_0);
            
            if ( srcText[i] == const_char<char_type>::Dot ) {
                for ( i++; char_is_numeric(srcText[i]) && i<max; i++ ) {
                    val = 10 * val + (srcText[i] - const_char<char_type>::_0);
                    div *= 10;
                }
            }
            dstD[j] = val / div;
            
            i++;
        }
        if ( j != doubleNum ) {
            while(j<doubleNum)
                dstD[j++] = 0.0;
            return false;
        }
        return true;
    }

    template<typename char_type>
    std::size_t filename_hash( const char_type *string, const int hashSize ) {
        std::size_t		i;
        long	hash;
        char_type	letter;
        
        hash = 0;
        i = 0;
        while( string[i] != const_char<char_type>::zero ) {
            letter = to_lower( string[i] );
            if ( letter == const_char<char_type>::dot ) {
                break;				// don't include extension
            }
            if ( letter ==const_char<char_type>::back_slash ) {
                letter = const_char<char_type>::slash;
            }
            hash += (long)letter * ( i + 119 );
            i++;
        }
        hash &= ( hashSize - 1 );
        return hash;
    }
    
    template<typename char_type>
    char_type*    skip_leading_whitespace( char_type* s ) {
        int i;
        int len = length(s);
        // cast to unsigned char_type to prevent stripping off high-ASCII characters
        for ( i = 0; i < len && (char_type)(s[ i ]) <= const_char<char_type>::white_space; i++ )
            ;
        
        return s + i;
    }
    
    template<typename char_type>
    std::size_t find_char( const char_type *str, const char_type c, int start = 0, int end = INVALID_POSITION) {
        std::size_t i;
        
        if ( end == INVALID_POSITION ) {
            end = length( str ) - 1;
        }
        for ( i = start; i <= end; i++ ) {
            if ( str[i] == c ) {
                return i;
            }
        }
        return INVALID_POSITION;
    }
    
    template<typename char_type>
    std::size_t find_text( const char_type *str, const char_type *text, bool casesensitive = true, int start = 0, int end = INVALID_POSITION) {
        std::size_t l, i, j;
        
        if ( end == INVALID_POSITION ) {
            end = length( str );
        }
        l = end - length( text );
        for ( i = start; i <= l; i++ ) {
            if ( casesensitive ) {
                for ( j = 0; text[j]; j++ ) {
                    if ( str[i+j] != text[j] ) {
                        break;
                    }
                }
            } else {
                for ( j = 0; text[j]; j++ ) {
                    if ( to_upper( str[i+j] ) != to_upper( text[j] ) ) {
                        break;
                    }
                }
            }
            if ( !text[j] ) {
                return i;
            }
        }
        return INVALID_POSITION;
    }
    
    template<typename char_type>
    const char_type*	find_string( const char_type *str, const char_type *text, bool casesensitive = true, int start = 0, int end = INVALID_POSITION) {
        std::size_t i;
        i = find_text( str, text, casesensitive, start, end );
        return ( i == INVALID_POSITION ) ? NULL : &str[i] ;
    }

    /*
     ============
     Returns true if the string conforms the given filter.
     Several metacharacter may be used in the filter.
     
     *          match any string of zero or more characters
     ?          match any single character
     [abc...]   match any of the enclosed characters; a hyphen can
     be used to specify a range (e.g. a-z, A-Z, 0-9)
     
     ============
     */
    template<typename char_type>
    bool filter( const char_type *filter, const char_type *name, bool casesensitive = true ) {
        using loc_str = std::basic_string<char_type>;
        loc_str buf;
        int i, found, index;
        
        while(*filter) {
            if (*filter == const_char<char_type>::star) {
                filter++;
                buf.empty();
                for (i = 0; *filter; i++) {
                    if ( *filter == const_char<char_type>::star || *filter == const_char<char_type>::question || (*filter == const_char<char_type>::open_quad && *(filter+1) != const_char<char_type>::open_quad) ) {
                        break;
                    }
                    buf += *filter;
                    if ( *filter == const_char<char_type>::open_quad ) {
                        filter++;
                    }
                    filter++;
                }
                if ( buf.length() ) {
                    loc_str text(name);
                    index = find( text.c_str(), buf.c_str(), casesensitive );
                    if ( index == INVALID_POSITION ) {
                        return false;
                    }
                    name += index + buf.length();
                }
            }
            else if (*filter == const_char<char_type>::question) {
                filter++;
                name++;
            }
            else if (*filter == const_char<char_type>::open_quad) {
                if ( *(filter+1) == const_char<char_type>::open_quad ) {
                    if ( *name != const_char<char_type>::open_quad ) {
                        return false;
                    }
                    filter += 2;
                    name++;
                }
                else {
                    filter++;
                    found = false;
                    while(*filter && !found) {
                        if (*filter == const_char<char_type>::close_quad && *(filter+1) != const_char<char_type>::close_quad) {
                            break;
                        }
                        if (*(filter+1) == const_char<char_type>::sub && *(filter+2) && (*(filter+2) != const_char<char_type>::close_quad || *(filter+3) == const_char<char_type>::close_quad)) {
                            if (casesensitive) {
                                if (*name >= *filter && *name <= *(filter+2)) {
                                    found = true;
                                }
                            }
                            else {
                                if ( to_upper(*name) >= to_upper(*filter) && to_upper(*name) <= to_upper(*(filter+2)) ) {
                                    found = true;
                                }
                            }
                            filter += 3;
                        }
                        else {
                            if (casesensitive) {
                                if (*filter == *name) {
                                    found = true;
                                }
                            }
                            else {
                                if ( to_upper(*filter) == to_upper(*name) ) {
                                    found = true;
                                }
                            }
                            filter++;
                        }
                    }
                    if (!found) {
                        return false;
                    }
                    while(*filter) {
                        if ( *filter == const_char<char_type>::close_quad && *(filter+1) != const_char<char_type>::close_quad ) {
                            break;
                        }
                        filter++;
                    }
                    filter++;
                    name++;
                }
            }
            else {
                if (casesensitive) {
                    if (*filter != *name) {
                        return false;
                    }
                }
                else {
                    if ( to_upper(*filter) != to_upper(*name) ) {
                        return false;
                    }
                }
                filter++;
                name++;
            }
        }
        return true;
    }
    
}
#endif /* stringext_h */
