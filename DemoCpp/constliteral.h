#ifndef CONSTLITERAL_H
#define CONSTLITERAL_H

// macro defines
#define u8s(s) (u8##s)
#define u8c(c) (c)
#define u16t(e) (u##e)
#define u32t(e) (U##e)
#define ut u16t
#define u4t u32t


namespace inf {
    
    template<typename Type>
    struct const_char;
    
    template<>
    struct const_char<char> {
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
        static const char return_r = '\r';
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
    struct const_char<char16_t> {
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
        static const char16_t return_r = u16t('\r');
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
    struct const_char<char32_t> {
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
        static const char32_t return_r = u32t('\r');
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
    struct const_literal;
    
    template <>
    struct const_literal<char> {
        static const char* empty;
        
        static const char* bool_false;
        static const char* bool_true;
        
        static const char* fmt_int;
        static const char* fmt_uint;
        static const char* fmt_float;
        static const char* fmt_double;
    };
    const char* const_literal<char>::empty = u8s("");
    const char* const_literal<char>::bool_false = u8s("false");
    const char* const_literal<char>::bool_true = u8s("true");
    const char* const_literal<char>::fmt_int = u8s("%d");
    const char* const_literal<char>::fmt_uint = u8s("%u");
    const char* const_literal<char>::fmt_float = u8s("%f");
    const char* const_literal<char>::fmt_double = u8s("%.16f");
    
    template <>
    struct const_literal<char16_t> {
        static const char16_t* empty;
        
        static const char16_t* bool_false;
        static const char16_t* bool_true;
        
        static const char16_t* fmt_int;
        static const char16_t* fmt_uint;
        static const char16_t* fmt_float;
        static const char16_t* fmt_double;
    };
    const char16_t* const_literal<char16_t>::empty = u16t("");
    const char16_t* const_literal<char16_t>::bool_false = u16t("false");
    const char16_t* const_literal<char16_t>::bool_true = u16t("true");
    const char16_t* const_literal<char16_t>::fmt_int = u16t("%d");
    const char16_t* const_literal<char16_t>::fmt_uint = u16t("%u");
    const char16_t* const_literal<char16_t>::fmt_float = u16t("%f");
    const char16_t* const_literal<char16_t>::fmt_double = u16t("%.16f");
    
    template <>
    struct const_literal<char32_t> {
        static const char32_t* empty;
        
        static const char32_t* bool_false;
        static const char32_t* bool_true;
        
        static const char32_t* fmt_int;
        static const char32_t* fmt_uint;
        static const char32_t* fmt_float;
        static const char32_t* fmt_double;
    };
    const char32_t* const_literal<char32_t>::empty = u32t("");
    const char32_t* const_literal<char32_t>::bool_false = u32t("false");
    const char32_t* const_literal<char32_t>::bool_true = u32t("true");
    const char32_t* const_literal<char32_t>::fmt_int = u32t("%d");
    const char32_t* const_literal<char32_t>::fmt_uint = u32t("%u");
    const char32_t* const_literal<char32_t>::fmt_float = u32t("%f");
    const char32_t* const_literal<char32_t>::fmt_double = u32t("%.16f");
}   // end of ::inf

#endif /* CONSTLITERAL_H */
