#ifndef _LITDEF_H_
#define _LITDEF_H_

// macro defines
#define u8s(s) (u8##s)
#define u8c(c) (c)
#define u16t(e) (u##e)
#define u32t(e) (U##e)
#define ut u16t
#define u4t u32t
#define wt(e) (L##e)


namespace inf {
    
    template<typename Type>
    struct c_def {
        
    };
    
    template<>
    struct c_def<char> {
        static const char zero;
        static const char back_slash;
        static const char slash;
        static const char quote;
        static const char single_quote;
        static const char white_space;
        static const char dot;
        static const char add;
        static const char sub;
        static const char formfeed;
        static const char newline;
        static const char return_c;
        static const char table;
        static const char vertical_table;
        static const char question;
        static const char star;
        static const char open_quad;
        static const char close_quad;
        static const char comma;
        
        static const char _0;
        static const char _1;
        static const char _2;
        static const char _3;
        static const char _4;
        static const char _5;
        static const char _6;
        static const char _7;
        static const char _8;
        static const char _9;
        
        static const char A;
        static const char F;
        static const char Z;
        
        static const char a;
        static const char f;
        static const char z;
    };
    
    template<>
    struct c_def<wchar_t> {
        static const wchar_t zero;
        static const wchar_t back_slash;
        static const wchar_t slash;
        static const wchar_t quote;
        static const wchar_t single_quote;
        static const wchar_t white_space;
        static const wchar_t dot;
        static const wchar_t add;
        static const wchar_t sub;
        static const wchar_t formfeed;
        static const wchar_t newline;
        static const wchar_t return_c;
        static const wchar_t table;
        static const wchar_t vertical_table;
        static const wchar_t question;
        static const wchar_t star;
        static const wchar_t open_quad;
        static const wchar_t close_quad;
        static const wchar_t comma;
        
        static const wchar_t _0;
        static const wchar_t _1;
        static const wchar_t _2;
        static const wchar_t _3;
        static const wchar_t _4;
        static const wchar_t _5;
        static const wchar_t _6;
        static const wchar_t _7;
        static const wchar_t _8;
        static const wchar_t _9;
        
        static const wchar_t A;
        static const wchar_t F;
        static const wchar_t Z;
        
        static const wchar_t a;
        static const wchar_t f;
        static const wchar_t z;
    };
    
    template<>
    struct c_def<char16_t> {
        static const char16_t zero;
        static const char16_t back_slash;
        static const char16_t slash;
        static const char16_t quote;
        static const char16_t single_quote;
        static const char16_t white_space;
        static const char16_t dot;
        static const char16_t add;
        static const char16_t sub;
        static const char16_t formfeed;
        static const char16_t newline;
        static const char16_t return_c;
        static const char16_t table;
        static const char16_t vertical_table;
        static const char16_t question;
        static const char16_t star;
        static const char16_t open_quad;
        static const char16_t close_quad;
        static const char16_t comma;

        static const char16_t _0;
        static const char16_t _1;
        static const char16_t _2;
        static const char16_t _3;
        static const char16_t _4;
        static const char16_t _5;
        static const char16_t _6;
        static const char16_t _7;
        static const char16_t _8;
        static const char16_t _9;
        
        static const char16_t A;
        static const char16_t F;
        static const char16_t Z;
        
        static const char16_t a;
        static const char16_t f;
        static const char16_t z;
    };
    template<>
    struct c_def<char32_t> {
        static const char32_t zero;
        static const char32_t back_slash;
        static const char32_t slash;
        static const char32_t quote;
        static const char32_t single_quote;
        static const char32_t white_space;
        static const char32_t dot;
        static const char32_t add;
        static const char32_t sub;
        static const char32_t formfeed;
        static const char32_t newline;
        static const char32_t return_c;
        static const char32_t table;
        static const char32_t vertical_table;
        static const char32_t question;
        static const char32_t star;
        static const char32_t open_quad;
        static const char32_t close_quad;
        static const char32_t comma;

        static const char32_t _0;
        static const char32_t _1;
        static const char32_t _2;
        static const char32_t _3;
        static const char32_t _4;
        static const char32_t _5;
        static const char32_t _6;
        static const char32_t _7;
        static const char32_t _8;
        static const char32_t _9;
        
        static const char32_t A;
        static const char32_t F;
        static const char32_t Z;
        
        static const char32_t a;
        static const char32_t f;
        static const char32_t z;
    };
    
    template <typename char_type>
    struct cstr_def {
        
    };
    
    template <>
    struct cstr_def<char> {
        static const char* empty;
        
        static const char* bool_false;
        static const char* bool_true;
        
        static const char* fmt_int;
        static const char* fmt_uint;
        static const char* fmt_float;
        static const char* fmt_double;
    };

    
    template <>
    struct cstr_def<wchar_t> {
        static const wchar_t* empty;
        
        static const wchar_t* bool_false;
        static const wchar_t* bool_true;
        
        static const wchar_t* fmt_int;
        static const wchar_t* fmt_uint;
        static const wchar_t* fmt_float;
        static const wchar_t* fmt_double;
    };

    
    template <>
    struct cstr_def<char16_t> {
        static const char16_t* empty;
        
        static const char16_t* bool_false;
        static const char16_t* bool_true;
        
        static const char16_t* fmt_int;
        static const char16_t* fmt_uint;
        static const char16_t* fmt_float;
        static const char16_t* fmt_double;
    };

    
    template <>
    struct cstr_def<char32_t> {
        static const char32_t* empty;
        
        static const char32_t* bool_false;
        static const char32_t* bool_true;
        
        static const char32_t* fmt_int;
        static const char32_t* fmt_uint;
        static const char32_t* fmt_float;
        static const char32_t* fmt_double;
    };

}   // end of ::inf

#endif /* _LITDEF_H_ */
