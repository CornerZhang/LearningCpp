#ifndef _CSTRFUNC_H_
#define _CSTRFUNC_H_

#include "litdef.h"
#include <cstddef>
#include <memory>
#include <cassert>

namespace inf {
    
    inline bool c_is_printable( int c ) {
        return ( c >= 0x20 && c <= 0x7E ) || ( c >= 0xA1 && c <= 0xFF );
    }
    
    template<typename char_type>
    bool c_is_lower( char_type c ) {
        return ( c >= c_def<char_type>::a && c <= c_def<char_type>::z ) || ( c >= 0xE0 && c <= 0xFF );
    }
    
    template<typename char_type>
    bool c_is_upper( char_type c ) {
        return ( c <= c_def<char_type>::Z && c >= c_def<char_type>::A ) || ( c >= 0xC0 && c <= 0xDF );
    }
    
    template<typename char_type>
    bool c_is_alpha( char_type c ) {
        return ( ( c >= c_def<char_type>::a && c <= c_def<char_type>::z ) || ( c >= c_def<char_type>::A && c <= c_def<char_type>::Z ) ||
                ( c >= 0xC0 && c <= 0xFF ) );
    }
    
    template<typename char_type>
    bool c_is_digit( char_type c ) {
        return ( c <= c_def<char_type>::_9 && c >= c_def<char_type>::_0 );
    }
    
    template<typename char_type>
    bool c_is_newline( char_type c ) {
        return ( c == c_def<char_type>::newline || c == c_def<char_type>::return_c || c == c_def<char_type>::vertical_table );
    }
    
    template<typename char_type>
    bool c_is_tab( char_type c ) {
        return ( c == c_def<char_type>::table );
    }
    
    template<typename char_type>
    bool c_is_whitechar( char_type c ) {
        return ( c == c_def<char_type>::white_space || c==c_def<char_type>::formfeed || c==c_def<char_type>::newline || c==c_def<char_type>::return_c || c==c_def<char_type>::table || c==c_def<char_type>::vertical_table );
    }
    
    template<typename char_type>
    bool c_is_punctuation( char_type c ) {
        return ( !is_alpha(c) && !is_numeric(c) && !is_printable(c) && !is_newline(c) );
    }
    
    template<typename char_type>
    bool c_is_hex( char_type c ) {
        return ( ( c >= c_def<char_type>::_0 && c <= c_def<char_type>::_9 ) || ( c >= c_def<char_type>::A && c <= c_def<char_type>::F ) || ( c >= c_def<char_type>::a && c <= c_def<char_type>::f ) );
    }
    
    template<typename char_type>
    int c_hex_for_char( char_type c ) {
        return ( c > c_def<char_type>::_9 ? ( c >= c_def<char_type>::a ? ( c - c_def<char_type>::a + 10 ) : ( c - c_def<char_type>::_7 ) ) : ( c - c_def<char_type>::_0 ) );
    }
    
    template<typename char_type>
    char_type c_to_lower( char_type c ) {
        if ( c <= c_def<char_type>::Z && c >= c_def<char_type>::A ) {
            return ( c + ( c_def<char_type>::a - c_def<char_type>::A ) );
        }
        return c;
    }
    
    template<typename char_type>
    char_type c_to_upper( char_type c ) {
        if ( c >= c_def<char_type>::a && c <= c_def<char_type>::z ) {
            return ( c - ( c_def<char_type>::a - c_def<char_type>::A ) );
        }
        return c;
    }
    
    
    
    
    
    template <typename type>
    constexpr const unsigned int INTSIGNBITNOTSET(type i) {
        return (~((const unsigned int)(i))) >> 31;
    }
    
    static const int INVALID_POSITION = -1;
    
    
    template <typename char_type>
    std::size_t cstr_length(const char_type* s) {
        std::size_t i=0;
        for (; s[i] != c_def<char_type>::zero; ++i) {}
        return i;
    }
    
    template<typename char_type>
    std::size_t cstr_count_char( const char_type *str, const char_type c ) {
        std::size_t i, count = 0;
        for ( i = 0; str[i] != c_def<char_type>::zero; i++ ) {
            if ( str[i] == c ) {
                count++;
            }
        }
        return count;
    }
    
    template<typename char_type>
    std::size_t cstr_hash(const char_type* str, bool casesensitive = true) {
        std::size_t i, hash = 0;
        if( casesensitive ) {
            for ( i = 0; *str != c_def<char_type>::zero; i++ ) {
                hash += ( *str++ ) * ( i + 119 );
            }
        } else {
            for( i = 0; *str != c_def<char_type>::zero; i++ ) {
                hash += to_lower( *str++ ) * ( i + 119 );
            }
        }
        return hash;
    }
    
    template<typename char_type>
    std::size_t cstr_hash( const char_type* str, int length, bool casesensitive = true) {
        std::size_t i, hash = 0;
        if( casesensitive ) {
            for ( i = 0; i < length; i++ ) {
                hash += ( *str++ ) * ( i + 119 );
            }
        } else {
            for ( i = 0; i < length; i++ ) {
                hash += to_lower( *str++ ) * ( i + 119 );
            }
        }
        return hash;
    }
    
    template<typename char_type>
    bool cstr_check_extension( const char_type *name, const char_type *ext ) {
        const char_type *s1 = name + length( name ) - 1;
        const char_type *s2 = ext + length( ext ) - 1;
        char_type c1, c2, d;
        
        do {
            c1 = *s1--;
            c2 = *s2--;
            
            d = c1 - c2;
            while( d ) {
                if ( c1 <= c_def<char_type>::Z && c1 >= c_def<char_type>::A ) {
                    d += (c_def<char_type>::a - c_def<char_type>::A);
                    if ( !d ) {
                        break;
                    }
                }
                if ( c2 <= c_def<char_type>::Z && c2 >= c_def<char_type>::A ) {
                    d -= (c_def<char_type>::a - c_def<char_type>::A);
                    if ( !d ) {
                        break;
                    }
                }
                return false;
            }
        } while( s1 >= name && s2 >= ext );
        
        return ( s1 >= name ) && ( *s1 == c_def<char_type>::dot );
    }
    
    template<typename char_type>
    bool cstr_is_numeric( const char_type * s ) {
        std::size_t		i;
        bool	dot;
        
        if ( *s == c_def<char_type>::sub ) {
            s++;
        }
        
        dot = false;
        for ( i = 0; s[i]; i++ ) {
            if ( !c_is_digit( s[i] ) ) {
                if ( ( s[ i ] == c_def<char_type>::dot ) && !dot ) {
                    dot = true;
                    continue;
                }
                return false;
            }
        }
        
        return true;
    }
    
    template<typename char_type>
    bool cstr_is_alpha( const char_type *s ) {
        int		i;
        for ( i = 0; s[i]; i++ ) {
            if ( !c_is_alpha(s[i]) ) {
                return false;
            }
        }
        
        return true;
    }
    
    template<typename char_type>
    bool cstr_has_lower( const char_type *s ) {
        if ( !s ) {
            return false;
        }
        
        while ( *s ) {
            if ( c_is_lower( *s ) ) {
                return true;
            }
            s++;
        }
        
        return false;
    }
    
    template<typename char_type>
    bool cstr_has_upper( const char_type *s ) {
        if ( !s ) {
            return false;
        }
        
        while ( *s ) {
            if ( c_is_upper( *s ) ) {
                return true;
            }
            s++;
        }
        
        return false;
    }
    
    template<typename char_type>
    bool cstr_is_quad_str(const char_type* str, int length, char_type with_quad_char) {
        if( (str[0] == with_quad_char) && (str[length-1] == with_quad_char) )
            return true;
        return false;
    }
    
    template<typename char_type>
    int cstr_cmp( const char_type *s1, const char_type *s2, bool casesensitive = true ) {
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
                const int Offset = c_def<char_type>::a - c_def<char_type>::A;
                while( d ) {
                    if ( c1 <= c_def<char_type>::Z && c1 >= c_def<char_type>::A ) {
                        d += Offset;
                        if ( !d ) {
                            break;
                        }
                    }
                    if ( c2 <= c_def<char_type>::A && c2 >= c_def<char_type>::Z ) {
                        d -= Offset;
                        if ( !d ) {
                            break;
                        }
                    }
                    return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
                }
            } while( c1 );
        }
        
        
        return 0;		// strs are equal
        
    }
    
    template<typename char_type>
    int cstr_cmpn( const char_type *s1, const char_type *s2, int n, bool casesensitive = true ) {
        int c1, c2, d;
        
        assert( n >= 0 );
        
        if( casesensitive ) {
            do {
                c1 = *s1++;
                c2 = *s2++;
                
                if ( !n-- ) {
                    return 0;		// strs are equal until end point
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
                    return 0;		// strs are equal until end point
                }
                
                d = c1 - c2;
                const int Offset = c_def<char_type>::a - c_def<char_type>::A;
                while( d ) {
                    if ( c1 <= c_def<char_type>::Z && c1 >= c_def<char_type>::A ) {
                        d += Offset;
                        if ( !d ) {
                            break;
                        }
                    }
                    if ( c2 <= c_def<char_type>::Z && c2 >= c_def<char_type>::A ) {
                        d -= Offset;
                        if ( !d ) {
                            break;
                        }
                    }
                    return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
                }
            } while( c1 );
        }
        return 0;		// strs are equal
    }
    
    template<typename char_type>
    int cstr_cmp_prefix( const char_type* s1, const char_type* s2, bool casesensitive = true ) {
        assert( s1 );
        assert( s2 );
        return cstr_cmpn( s1, s2, length( s2 ), casesensitive );
    }
    
    template<typename char_type>
    int cstr_icmp_path( const char_type *s1, const char_type *s2 ) {
        int c1, c2, d;
        
        do {
            c1 = *s1++;
            c2 = *s2++;
            
            d = c1 - c2;
            while( d ) {
                if ( c1 <= c_def<char_type>::Z && c1 >= c_def<char_type>::A ) {
                    d += (c_def<char_type>::a - c_def<char_type>::A);
                    if ( !d ) {
                        break;
                    }
                }
                if ( c1 == c_def<char_type>::back_slash ) {
                    d += (c_def<char_type>::slash - c_def<char_type>::back_slash);
                    if ( !d ) {
                        break;
                    }
                }
                if ( c2 <= c_def<char_type>::Z && c2 >= c_def<char_type>::A ) {
                    d -= (c_def<char_type>::a - c_def<char_type>::A);
                    if ( !d ) {
                        break;
                    }
                }
                if ( c2 == c_def<char_type>::back_slash ) {
                    d -= (c_def<char_type>::slash - c_def<char_type>::back_slash);
                    if ( !d ) {
                        break;
                    }
                }
                // make sure folders come first
                while( c1 ) {
                    if ( c1 == c_def<char_type>::slash || c1 == c_def<char_type>::back_slash ) {
                        break;
                    }
                    c1 = *s1++;
                }
                while( c2 ) {
                    if ( c2 == c_def<char_type>::slash || c2 == c_def<char_type>::back_slash ) {
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
    double cstr_to_double(const char_type* str) {
        unsigned long long val;
        double m = (str[0] == c_def<char_type>::sub ) ? -1.0 : 1.0;
        int i = 0;
        
        if ( str[0] == c_def<char_type>::sub || str[0] == c_def<char_type>::add )
            i++;
        int max = 17 + i;
        for ( val=0; c_is_digit(str[i]) && i<max; i++ )
            val = 10 * val + (str[i] - c_def<char_type>::_0);
        
        if ( str[i] == c_def<char_type>::dot ) {
            i++;
            
            for ( ; c_is_digit(str[i]) && i<max; i++ ) {
                val = 10 * val + (str[i] - c_def<char_type>::_0);
                m *= 0.1;
            }
        }
        return val * m;
    }
    
    template<typename char_type>
    float cstr_to_float(const char_type* str) {
        unsigned long val;
        float m = (str[0] == c_def<char_type>::sub ) ? -1.0f : 1.0f;
        int i = 0;
        
        if ( str[0] == c_def<char_type>::sub  || str[0] == c_def<char_type>::add )
            i++;
        int max = 8 + i;
        for ( val=0; c_is_digit(str[i]) && i<max; i++ )
            val = 10 * val + (str[i] - c_def<char_type>::_0);
        
        if ( str[i] == c_def<char_type>::Dot ) {
            i++;
            
            for ( ; c_is_digit(str[i]) && i<max; i++ ) {
                val = 10 * val + (str[i] - c_def<char_type>::_0);
                m *= 0.1f;
            }
        }
        return val * m;
    }
    
    template<typename char_type>
    int	cstr_to_integer(const char_type* str) {
        int val = 0;
        int i = 0;
        int m = (str[0] == c_def<char_type>::sub ) ? -1 : 1;
        
        if ( str[0] == c_def<char_type>::sub  || str[0] == c_def<char_type>::add )
            i++;
        
        int max = 10 + i;
        for ( ; c_is_digit(str[i]) && i<max; i++ )
            val = 10 * val + (str[i] - c_def<char_type>::_0);
        
        return val * m;
    }
    
    template<typename char_type>
    bool cstr_to_float_array( float *dstF, int floatNum, const char_type *srcText, const char_type splitChar = c_def<char_type>::comma) {
        unsigned long val;
        float div;
        int i = 0;
        int j, max;
        
        for ( j=0; j < floatNum; j++ ) {
            if ( j && srcText[i-1] != splitChar )
                break;
            
            div = (srcText[i] == c_def<char_type>::sub ) ? -1.0f : 1.0f;
            
            if ( srcText[i] == c_def<char_type>::sub  || srcText[i] == c_def<char_type>::add )
                i++;
            
            if ( c_is_digit(srcText[i]) )
                break;
            
            max = 8 + i;
            for ( val=0; c_is_digit(srcText[i]) && i<max; i++ )
                val = 10 * val + (srcText[i] - c_def<char_type>::_0);
            
            //! @todo	fix this nicer
            // skip remaining digits we where not able to read.
            for ( ; c_is_digit(srcText[i]); i++ )
                continue;
            
            if ( srcText[i] == c_def<char_type>::dot ) {
                for ( i++; c_is_digit(srcText[i]) && i<max; i++ ) {
                    val = 10 * val + (srcText[i] - c_def<char_type>::_0);
                    div *= 10;
                }
                //! @todo	fix this nicer
                // skip remaining digits we where not able to read.
                for ( ; c_is_digit(srcText[i]); i++ )
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
    bool cstr_to_double_array( double *dstD, int doubleNum, const char_type *srcText, const char_type splitChar = c_def<char_type>::comma) {
        unsigned long long val;
        double div;
        int i = 0;
        int j, max;
        
        for ( j=0; j < doubleNum; j++ ) {
            if ( j && srcText[i-1] != splitChar )
                break;
            
            div = (srcText[i] == c_def<char_type>::sub ) ? -1.0 : 1.0;
            
            if ( srcText[i] == c_def<char_type>::sub  || srcText[i] == c_def<char_type>::add )
                i++;
            
            if ( !c_is_digit(srcText[i]) )
                break;
            
            max = 17 + i;
            for ( val=0; c_is_digit(srcText[i]) && i<max; i++ )
                val = 10 * val + (srcText[i] - c_def<char_type>::_0);
            
            if ( srcText[i] == c_def<char_type>::Dot ) {
                for ( i++; c_is_digit(srcText[i]) && i<max; i++ ) {
                    val = 10 * val + (srcText[i] - c_def<char_type>::_0);
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
    std::size_t cstr_filename_hash( const char_type *str, const int hashSize, bool ignoreExt = true ) {
        std::size_t		i;
        long	hash;
        char_type	letter;
        
        hash = 0;
        i = 0;
        while( str[i] != c_def<char_type>::zero ) {
            letter = to_lower( str[i] );
            if ( ignoreExt && (letter == c_def<char_type>::dot) ) {
                break;				// don't include extension
            }
            if ( letter ==c_def<char_type>::back_slash ) {
                letter = c_def<char_type>::slash;
            }
            hash += (long)letter * ( i + 119 );
            i++;
        }
        hash &= ( hashSize - 1 );
        return hash;
    }
    
    template<typename char_type>
    char_type*    cstr_skip_leading_whitespace( char_type* s ) {
        std::size_t i;
        std::size_t len = cstr_length(s);
        // cast to unsigned char_type to prevent stripping off high-ASCII characters
        for ( i = 0; i < len && (char_type)(s[ i ]) <= c_def<char_type>::white_space; i++ )
            ;
        
        return s + i;
    }
    
    template<typename char_type>
    std::size_t cstr_find_first_char( const char_type *str, const char_type c, int start = 0, int end = INVALID_POSITION) {
        std::size_t i;
        
        assert(str);
        
        if (*str==c_def<char_type>::zero) {
            return INVALID_POSITION;
        }
        
        if ( end == INVALID_POSITION ) {
            end = cstr_length( str ) - 1;
        }
        for ( i = start; i <= end; i++ ) {
            if ( str[i] == c ) {
                return i;
            }
        }
        return INVALID_POSITION;
    }
    
    template<typename char_type>
    std::size_t cstr_find_first_text( const char_type *str, const char_type *text, bool casesensitive = true, int start = 0, int end = INVALID_POSITION) {
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
                    if ( c_to_upper( str[i+j] ) != to_upper( text[j] ) ) {
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
    const char_type*	cstr_find_first_string( const char_type *str, const char_type *text, bool casesensitive = true, int start = 0, int end = INVALID_POSITION) {
        std::size_t i;
        i = cstr_find_text( str, text, casesensitive, start, end );
        return ( i == INVALID_POSITION ) ? NULL : &str[i] ;
    }
    
    template<typename char_type>
    std::size_t cstr_find_last_char( const char_type* text, std::size_t len, const char_type c ) {
        int i = static_cast<int>(len);
        for( ; i >= 0; i-- ) {
            if ( text[ i ] == c ) {
                return i;
            }
        }
        return INVALID_POSITION;
    }
    
    template<typename char_type>
    std::size_t cstr_find_last_stringn( const char_type* text, std::size_t text_len, const char_type* search_str, std::size_t search_len, bool casesensitive = true) {
        int index = static_cast<int>(text_len);
        
        if( text_len > search_len ) {
            index -= search_len;
        }
        
        for( ; index >= 0; index-- ) {
            if( cstr_cmpn( &text[ index ], search_str, search_len, casesensitive ) == 0 ) {
                return index;
            }
        }
        return INVALID_POSITION;
    }
    
    template<typename char_type>
    std::size_t cstr_find_last_string( const char_type* text, std::size_t text_len, const char_type* search_str, bool casesensitive = true) {
        int index = static_cast<int>(text_len);
        int search_len = cstr_length(search_str);
        
        if( text_len > search_len ) {
            index -= search_len;
        }
        
        for( ; index >= 0; index-- ) {
            if( cstr_cmp( &text[ index ], search_str, casesensitive ) == 0 ) {
                return index;
            }
        }
        return INVALID_POSITION;
    }
    
    /*
     ============
     Returns true if the str conforms the given filter.
     Several metacharacter may be used in the filter.
     
     *          match any str of zero or more characters
     ?          match any single character
     [abc...]   match any of the enclosed characters; a hyphen can
     be used to specify a range (e.g. a-z, A-Z, 0-9)
     
     ============
     */
    template<typename char_type>
    bool cstr_filter( const char_type *filter, const char_type *name, bool casesensitive = true ) {
        using loc_str = std::basic_string<char_type>;
        loc_str buf;
        int i, found, index;
        
        while(*filter) {
            if (*filter == c_def<char_type>::star) {
                filter++;
                buf.empty();
                for (i = 0; *filter; i++) {
                    if ( *filter == c_def<char_type>::star || *filter == c_def<char_type>::question || (*filter == c_def<char_type>::open_quad && *(filter+1) != c_def<char_type>::open_quad) ) {
                        break;
                    }
                    buf += *filter;
                    if ( *filter == c_def<char_type>::open_quad ) {
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
            else if (*filter == c_def<char_type>::question) {
                filter++;
                name++;
            }
            else if (*filter == c_def<char_type>::open_quad) {
                if ( *(filter+1) == c_def<char_type>::open_quad ) {
                    if ( *name != c_def<char_type>::open_quad ) {
                        return false;
                    }
                    filter += 2;
                    name++;
                }
                else {
                    filter++;
                    found = false;
                    while(*filter && !found) {
                        if (*filter == c_def<char_type>::close_quad && *(filter+1) != c_def<char_type>::close_quad) {
                            break;
                        }
                        if (*(filter+1) == c_def<char_type>::sub && *(filter+2) && (*(filter+2) != c_def<char_type>::close_quad || *(filter+3) == c_def<char_type>::close_quad)) {
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
                        if ( *filter == c_def<char_type>::close_quad && *(filter+1) != c_def<char_type>::close_quad ) {
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
                    if ( c_to_upper(*filter) != c_to_upper(*name) ) {
                        return false;
                    }
                }
                filter++;
                name++;
            }
        }
        return true;
    }
    
    template <typename char_type>
    void cstr_copynz(char_type* dest, const char_type* src, int count) {
        if ( !src ) {
            assert( false && "cstr_copynz: NULL src" );
            return;
        }
        if ( count < 1 ) {
            assert( false && "cstr_copynz: destsize < 1" );
            return;
        }
        
        std::memcpy( dest, src, sizeof(char_type)*(count - 1) );
        dest[ count - 1 ] = c_def<char_type>::zero;
    }
    
    template <typename char_type>
    void cstr_append( char_type *dest_buf, int dest_size, const char_type *src ) {
        std::size_t		dest_realsize;
        
        dest_realsize = cstr_length( dest_buf );
        if ( dest_realsize >= dest_size ) {
            assert( false && "cstr_append: already overflowed" );
        }
        cstr_copynz( dest_buf + dest_realsize, src, dest_size - dest_realsize );
    }
    
    template <typename char_type>
    void cstr_replace_charn( const char_type* str, std::size_t len, char_type oldChar, char_type newChar ) {
        std::size_t i;
        for ( i = 0; i < len; i++ ) {
            if ( str[ i ] != oldChar ) {
                continue;
            }
            
            str[ i ] = newChar;
        }
    }

    template <typename char_type>
    void cstr_replace_first_charn( const char_type* str, std::size_t len, char_type oldChar, char_type newChar ) {
        std::size_t i;
        for ( i = 0; i < len; i++ ) {
            if ( str[ i ] == oldChar ) {
                str[ i ] = newChar;
                break;
            }
        }
    }

    template <typename char_type>
    void cstr_replace_last_charn( const char_type* str, std::size_t len, char_type oldChar, char_type newChar ) {
        int i = static_cast<int>(len);
        for (; i >= 0; i-- ) {
            if ( str[ i ] == oldChar ) {
                str[ i ] = newChar;
                break;
            }
        }
    }
    
    namespace { // private
        template <typename char_type>
        struct basic_string_traits {
            basic_string_traits() = delete;
            basic_string_traits(const basic_string_traits& ) = delete;
            basic_string_traits(const basic_string_traits&& ) = delete;
            
            using char_traits       = std::char_traits<char_type>;
            using allocator         = std::allocator<char_type>;
            using value_type        = std::basic_string<char_type, char_traits, allocator>;
            using pointer           = value_type*;
            using const_pointer     = const pointer;
            using reference         = value_type&;
            using const_reference   = const value_type&;
        };
    }

    // define: substr for C style string
    template <typename char_type>
    const char_type* cstr_substrn( typename basic_string_traits<char_type>::reference outstr,
                                 const char_type* str, std::size_t str_len, std::size_t substr_start, std::size_t substr_len ) {
        outstr.clear();
        
        if ( str_len == 0 || substr_start >= str_len ) {
            return NULL;
        }
        
        if ( substr_start + substr_len >= str_len ) {
            substr_len = str_len - substr_start;
        }
        
        outstr.append( &str[ substr_start ], substr_len );
        return outstr.c_str();
    }
    
    template <typename char_type>
    std::size_t cstr_split(const char_type* splited_str_pointers[], const char_type* str, std::size_t str_len, char_type split_char) {
        const char_type* str_end = str + str_len;
        std::size_t substr_count=0;
        std::size_t found_pos=0;
        
        assert(str);
        
        // checking
        if (*str==c_def<char_type>::zero || split_char==c_def<char_type>::zero)
            return substr_count;
        
        do {
            if(*str==split_char)
                ++str;
            
            splited_str_pointers[substr_count] = str;
            ++substr_count;
            
            found_pos = cstr_find_first_char(str, split_char);
            if (found_pos==INVALID_POSITION) {
                break;
            }

            str += found_pos+1;  // skip split_char

        } while ( str <= str_end );
                
        return substr_count;
    }
}   // ::inf

#endif /* _CSTRFUNC_H_ */
