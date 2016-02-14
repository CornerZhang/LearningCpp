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
    struct c_def;
    
    template<>
    struct c_def<char> {
        static const char zero = '\0';
        static const char back_slash = '\\';
        static const char slash = '/';
        static const char quote = '\"';
        static const char single_quote = '\'';
        static const char white_space = ' ';
        static const char dot = '.';
        static const char add = '+';
        static const char sub = '-';
        static const char formfeed = '\f';
        static const char newline = '\n';
        static const char return_c = '\r';
        static const char table = '\t';
        static const char vertical_table = '\v';
        static const char question = '?';
        static const char star = '*';
        static const char open_quad = '[';
        static const char close_quad = ']';
        static const char comma = ',';
        
        static const char _0 = '0';
        static const char _1 = '1';
        static const char _2 = '2';
        static const char _3 = '3';
        static const char _4 = '4';
        static const char _5 = '5';
        static const char _6 = '6';
        static const char _7 = '7';
        static const char _8 = '8';
        static const char _9 = '9';
        
        static const char A = 'A';
        static const char F = 'F';
        static const char Z = 'Z';
        
        static const char a = 'a';
        static const char f = 'f';
        static const char z = 'z';
    };
    template<>
    struct c_def<wchar_t> {
        static const wchar_t zero = wt('\0');
        static const wchar_t back_slash = wt('\\');
        static const wchar_t slash = wt('/');
        static const wchar_t quote = wt('\"');
        static const wchar_t single_quote = wt('\'');
        static const wchar_t white_space = wt(' ');
        static const wchar_t dot = wt('.');
        static const wchar_t add = wt('+');
        static const wchar_t sub = wt('-');
        static const wchar_t formfeed = wt('\f');
        static const wchar_t newline = wt('\n');
        static const wchar_t return_c = wt('\r');
        static const wchar_t table = wt('\t');
        static const wchar_t vertical_table = wt('\v');
        static const wchar_t question = wt('?');
        static const wchar_t star = wt('*');
        static const wchar_t open_quad = wt('[');
        static const wchar_t close_quad = wt(']');
        static const wchar_t comma = wt(',');
        
        static const wchar_t _0 = wt('0');
        static const wchar_t _1 = wt('1');
        static const wchar_t _2 = wt('2');
        static const wchar_t _3 = wt('3');
        static const wchar_t _4 = wt('4');
        static const wchar_t _5 = wt('5');
        static const wchar_t _6 = wt('6');
        static const wchar_t _7 = wt('7');
        static const wchar_t _8 = wt('8');
        static const wchar_t _9 = wt('9');
        
        static const wchar_t A = wt('A');
        static const wchar_t F = wt('F');
        static const wchar_t Z = wt('Z');
        
        static const wchar_t a = wt('a');
        static const wchar_t f = wt('f');
        static const wchar_t z = wt('z');
    };
    template<>
    struct c_def<char16_t> {
        static const char16_t zero = u16t('\0');
        static const char16_t back_slash = u16t('\\');
        static const char16_t slash = u16t('/');
        static const char16_t quote = u16t('\"');
        static const char16_t single_quote = u16t('\'');
        static const char16_t white_space = u16t(' ');
        static const char16_t dot = u16t('.');
        static const char16_t add = u16t('+');
        static const char16_t sub = u16t('-');
        static const char16_t formfeed = u16t('\f');
        static const char16_t newline = u16t('\n');
        static const char16_t return_c = u16t('\r');
        static const char16_t table = u16t('\t');
        static const char16_t vertical_table = u16t('\v');
        static const char16_t question = u16t('?');
        static const char16_t star = u16t('*');
        static const char16_t open_quad = u16t('[');
        static const char16_t close_quad = u16t(']');
        static const char16_t comma = u16t(',');

        static const char16_t _0 = u16t('0');
        static const char16_t _1 = u16t('1');
        static const char16_t _2 = u16t('2');
        static const char16_t _3 = u16t('3');
        static const char16_t _4 = u16t('4');
        static const char16_t _5 = u16t('5');
        static const char16_t _6 = u16t('6');
        static const char16_t _7 = u16t('7');
        static const char16_t _8 = u16t('8');
        static const char16_t _9 = u16t('9');
        
        static const char16_t A = u16t('A');
        static const char16_t F = u16t('F');
        static const char16_t Z = u16t('Z');
        
        static const char16_t a = u16t('a');
        static const char16_t f = u16t('f');
        static const char16_t z = u16t('z');
    };
    template<>
    struct c_def<char32_t> {
        static const char32_t zero = u32t('\0');
        static const char32_t back_slash = u32t('\\');
        static const char32_t slash = u32t('/');
        static const char32_t quote = u32t('\"');
        static const char32_t single_quote = u32t('\'');
        static const char32_t white_space = u32t(' ');
        static const char32_t dot = u32t('.');
        static const char32_t add = u32t('+');
        static const char32_t sub = u32t('-');
        static const char32_t formfeed = u32t('\f');
        static const char32_t newline = u32t('\n');
        static const char32_t return_c = u32t('\r');
        static const char32_t table = u32t('\t');
        static const char32_t vertical_table = u32t('\v');
        static const char32_t question = u32t('?');
        static const char32_t star = u32t('*');
        static const char32_t open_quad = u32t('[');
        static const char32_t close_quad = u32t(']');
        static const char32_t comma = u32t(',');

        static const char32_t _0 = u32t('0');
        static const char32_t _1 = u32t('1');
        static const char32_t _2 = u32t('2');
        static const char32_t _3 = u32t('3');
        static const char32_t _4 = u32t('4');
        static const char32_t _5 = u32t('5');
        static const char32_t _6 = u32t('6');
        static const char32_t _7 = u32t('7');
        static const char32_t _8 = u32t('8');
        static const char32_t _9 = u32t('9');
        
        static const char32_t A = u32t('A');
        static const char32_t F = u32t('F');
        static const char32_t Z = u32t('Z');
        
        static const char32_t a = u32t('a');
        static const char32_t f = u32t('f');
        static const char32_t z = u32t('z');
    };
    
    template <typename char_type>
    struct cstr_def;
    
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
    const char* cstr_def<char>::empty = u8s("");
    const char* cstr_def<char>::bool_false = u8s("false");
    const char* cstr_def<char>::bool_true = u8s("true");
    const char* cstr_def<char>::fmt_int = u8s("%d");
    const char* cstr_def<char>::fmt_uint = u8s("%u");
    const char* cstr_def<char>::fmt_float = u8s("%f");
    const char* cstr_def<char>::fmt_double = u8s("%.16f");
    
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
    const wchar_t* cstr_def<wchar_t>::empty = wt("");
    const wchar_t* cstr_def<wchar_t>::bool_false = wt("false");
    const wchar_t* cstr_def<wchar_t>::bool_true = wt("true");
    const wchar_t* cstr_def<wchar_t>::fmt_int = wt("%d");
    const wchar_t* cstr_def<wchar_t>::fmt_uint = wt("%u");
    const wchar_t* cstr_def<wchar_t>::fmt_float = wt("%f");
    const wchar_t* cstr_def<wchar_t>::fmt_double = wt("%.16f");
    
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
    const char16_t* cstr_def<char16_t>::empty = u16t("");
    const char16_t* cstr_def<char16_t>::bool_false = u16t("false");
    const char16_t* cstr_def<char16_t>::bool_true = u16t("true");
    const char16_t* cstr_def<char16_t>::fmt_int = u16t("%d");
    const char16_t* cstr_def<char16_t>::fmt_uint = u16t("%u");
    const char16_t* cstr_def<char16_t>::fmt_float = u16t("%f");
    const char16_t* cstr_def<char16_t>::fmt_double = u16t("%.16f");
    
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
    const char32_t* cstr_def<char32_t>::empty = u32t("");
    const char32_t* cstr_def<char32_t>::bool_false = u32t("false");
    const char32_t* cstr_def<char32_t>::bool_true = u32t("true");
    const char32_t* cstr_def<char32_t>::fmt_int = u32t("%d");
    const char32_t* cstr_def<char32_t>::fmt_uint = u32t("%u");
    const char32_t* cstr_def<char32_t>::fmt_float = u32t("%f");
    const char32_t* cstr_def<char32_t>::fmt_double = u32t("%.16f");
}   // end of ::inf

#endif /* _LITDEF_H_ */
