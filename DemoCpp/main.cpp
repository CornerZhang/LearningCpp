//
//  main.cpp
//  DemoCpp
//
//  Created by huazhang on 11/9/15.
//  Copyright © 2015 huazhang. All rights reserved.
//

#include <string>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <memory>
#include <scoped_allocator>
#include <vector>
#include <cstrfunc.h>
#include <cmd_arg.h>
#include <allocator_def.h>
#include "MultiMethods_Test.h"

namespace {
    template <typename Type>
    void swap(Type& a, Type& b) {
        Type t = a;
        a = b;
        b = t;
    }
    
    template <typename T>
    struct arg {
        
    };
    
    template <typename T>
    using args_u = arg<T>;
    
    template<typename tStringType, typename tTraits = typename tStringType::traits_type>
    void print_code_unit_sequence(tStringType str)
    {
        using char_type = typename tTraits::char_type;
        static_assert(std::is_same<char_type, char>::value || std::is_same<char_type, char16_t>::value || std::is_same<char_type, char32_t>::value, "error");
        using unsigned_char_type = typename std::make_unsigned<char_type>::type;
        using unsigned_int_type = typename std::make_unsigned<typename tTraits::int_type>::type;
        int w = std::is_same<char, char_type>::value ? 2 : std::is_same<char16_t, char_type>::value ? 4 : 8;
        for(auto c : str) {
            auto value = static_cast<unsigned_int_type>(static_cast<unsigned_char_type>(c));
            std::cout << "0x" << std::hex << std::uppercase << std::setw(w) << std::setfill('0') << value << ' ';
        }
    }
}

class Base {
public:
    Base(int a) {
        
        
    }
    
    Base(int a, int b) {
        
    }
    
    void foo() {}
    void foo(int a) {}
};

class Extent: private Base {
public:
    using Base::Base;
    using Base::foo;
    
    explicit Extent(int a) : Base(a) {
        
    }
    
    void func() {
        foo();
    }
};

template <typename T>
class vector_ex: private std::vector<T> {
public:
    using std::vector<T>::vector;
    
    T* alloc_to_back() {
        return nullptr;
    }
};

void char_test() {
    std::allocator_traits<inf::allocator<char>>::allocator_type a;
    
    std::scoped_allocator_adaptor<inf::allocator<char>>::value_type sa;
    
    std::wstring asdad( wt("dafads") );
    asdad += wt("dfaf");
    
    std::wstring ostr;
    inf::cstr_substrn(ostr, asdad.c_str(), asdad.size(), 1, 3);

    char str_base[512] = {0};
    inf::cstr_append(str_base, 512, "ftp://123.150.12.122/");
    inf::cstr_append(str_base, 512, "dfafasf_");
    inf::cstr_append(str_base, 512, "313423.png");
    
    const wchar_t* as = L"d";
    
    std::wstring text1(wt("abcd/34452/ghnhgh/"));
    const wchar_t* str_ptrs[20];
    std::size_t strn = inf::cstr_split(str_ptrs, text1.c_str(), text1.length(), wt('/'));
    
    Extent et(45);
    et.foo();
    
    vector_ex<int> vit(34, 8);

    // utf-8
    char u_c = u8c('C');
    char u8_array[] = u8s("中");
    std::string u8_str = u8s("近期啊你");
    std::string absc;
    bool bst = u8_str != absc;
    absc += u8_str;
    
    std::basic_string<char, std::char_traits<char>, inf::allocator<char>> sssss;
    sssss = "dafdsaf";
    
    bool vb = std::uses_allocator<char, inf::allocator<char>>::value;
    
    inf::cstr_length(u8_array);
    
    const char* dis = "324352346";
    int rds = std::isdigit(*dis);
    
    // utf-16
    char16_t u16_array[] = ut("中");
    std::u16string u16_str = ut("近期啊你");
    
    // ucs4
    char32_t u32_c = u4t('中');
    std::u32string u32_str = u4t("近期啊你");
    
    std::cout << u4t("utf-8: ");
    print_code_unit_sequence(u8_str);
    std::cout << std::endl;
    std::cout << "utf-16: ";
    print_code_unit_sequence(u16_str);
    std::cout << std::endl;
    std::cout << "ucs4: ";
    print_code_unit_sequence(u32_str);
    std::cout << std::endl;

    {
        std::string u8_source_str = u8"\xF0\x9F\x98\xBB"; // utf-8
        
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt;
        std::u16string u16_cvt_str = cvt.from_bytes(u8_source_str); // utf-8 to utf-16
        
        std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> utf8_ucs2_cvt;
        std::u16string ucs2_cvt_str = utf8_ucs2_cvt.from_bytes(u8_source_str); // utf-8 to ucs2

        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8_ucs4_cvt;
        std::u32string ucs4_cvt_str = utf8_ucs4_cvt.from_bytes(u8_source_str); // utf-8 to ucs4

    }
    
    {
        // UTF-8到UTF-16和UTF-16到UTF-8的编码转换
        std::string u8_source_str = u8"破晓的博客"; // utf-8
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt;
        std::u16string u16_cvt_str = cvt.from_bytes(u8_source_str); // utf-8 to utf-16
        std::string u8_cvt_str = cvt.to_bytes(u16_cvt_str); // utf-16 to utf-8
        std::cout << "u8_source_str = ";
        print_code_unit_sequence(u8_source_str);
        std::cout << std::endl;
        std::cout << "u16_cvt_str = ";
        print_code_unit_sequence(u16_cvt_str);
        std::cout << std::endl;
        std::cout << "u8_cvt_str = ";
        print_code_unit_sequence(u8_cvt_str);
        std::cout << std::endl;
    }

    {
        // UTF-8与UCS2或UCS4编码转换使用std::codecvt_utf8类模板
        std::string u8_source_str = u8"破晓的博客"; // utf-8
        std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> utf8_ucs2_cvt;
        std::u16string ucs2_cvt_str = utf8_ucs2_cvt.from_bytes(u8_source_str); // utf-8 to ucs2
        std::string u8_str_from_ucs2 = utf8_ucs2_cvt.to_bytes(ucs2_cvt_str); // ucs2 to utf-8
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8_ucs4_cvt;
        std::u32string ucs4_cvt_str = utf8_ucs4_cvt.from_bytes(u8_source_str); // utf-8 to ucs4
        std::string u8_str_from_ucs4 = utf8_ucs4_cvt.to_bytes(ucs4_cvt_str); // ucs4 to utf-8
        std::cout << "u8_source_str: ";
        print_code_unit_sequence(u8_source_str);
        std::cout << std::endl;
        std::cout << "ucs2_cvt_str: ";
        print_code_unit_sequence(ucs2_cvt_str);
        std::cout << std::endl;
        std::cout << "u8_str_from_ucs2: ";
        print_code_unit_sequence(u8_str_from_ucs2);
        std::cout << std::endl;
        std::cout << "ucs4_cvt_str: ";
        print_code_unit_sequence(ucs4_cvt_str);
        std::cout << std::endl;
        std::cout << "u8_str_from_ucs4: ";
        print_code_unit_sequence(u8_str_from_ucs4);
        std::cout << std::endl;
    }
    
    {
        // 当Elem为char16_t时转换为UTF-8与UCS2，当Elem为char32_t时转换为UTF-16与UCS4，当Elem为wchar_t时转换取决于实现
        std::string u8_source_str = u8"破晓的博客"; // utf-8
        std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> utf8_ucs2_cvt;
        std::u16string ucs2_cvt_str = utf8_ucs2_cvt.from_bytes(u8_source_str); // utf-8 to ucs2
        std::string u8_str_from_ucs2 = utf8_ucs2_cvt.to_bytes(ucs2_cvt_str); // ucs2 to utf-8
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8_ucs4_cvt;
        std::u32string ucs4_cvt_str = utf8_ucs4_cvt.from_bytes(u8_source_str); // utf-8 to ucs4
        std::string u8_str_from_ucs4 = utf8_ucs4_cvt.to_bytes(ucs4_cvt_str); // ucs4 to utf-8
        std::cout << "u8_source_str: ";
        print_code_unit_sequence(u8_source_str);
        std::cout << std::endl;
        std::cout << "ucs2_cvt_str: ";
        print_code_unit_sequence(ucs2_cvt_str);
        std::cout << std::endl;
        std::cout << "u8_str_from_ucs2: ";
        print_code_unit_sequence(u8_str_from_ucs2);
        std::cout << std::endl;
        std::cout << "ucs4_cvt_str: ";
        print_code_unit_sequence(ucs4_cvt_str);
        std::cout << std::endl;
        std::cout << "u8_str_from_ucs4: ";
        print_code_unit_sequence(u8_str_from_ucs4);
        std::cout << std::endl;
    }
    
    {
        // UTF-16 BE和UTF-16 LE编码到UCS4编码的转换
        std::string u16le_byte_str = "\x34\x78\x53\x66"; // utf-16 Little Endian
        std::string u16be_byte_str = "\x78\x34\x66\x53"; // utf-16 Big Endian
        std::wstring_convert<std::codecvt_utf16<char32_t, 0x10ffff, std::little_endian>, char32_t> utf16le_cvt; // little endian
        std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> utf16be_cvt; // default big endian
        std::u32string u32_str_from_le = utf16le_cvt.from_bytes(u16le_byte_str); // utf-16 to ucs4
        std::u32string u32_str_from_be = utf16be_cvt.from_bytes(u16be_byte_str); // utf-16 to ucs4
        std::cout << "u16le_byte_str: ";
        print_code_unit_sequence(u16le_byte_str);
        std::cout << std::endl;
        std::cout << "u16be_byte_str: ";
        print_code_unit_sequence(u16be_byte_str);
        std::cout << std::endl;
        std::cout << "u32_str_from_le: ";
        print_code_unit_sequence(u32_str_from_le);
        std::cout << std::endl;
        std::cout << "u32_str_from_be: ";
        print_code_unit_sequence(u32_str_from_be);
        std::cout << std::endl;
    }
    
    {
        // 通过BOM指示UTF-16编码字符串的字节序，codecvt的第3个参数需设置为std::consume_header
        std::string u16le_byte_str = "\xff\xfe\x34\x78\x53\x66"; // utf-16 little endian with BOM
        std::string u16be_byte_str = "\xfe\xff\x78\x34\x66\x53"; // utf-16 big endian with BOM
        std::wstring_convert<std::codecvt_utf16<char32_t, 0x10ffff, std::consume_header>, char32_t> utf16le_cvt; // little endian
        std::wstring_convert<std::codecvt_utf16<char32_t, 0x10ffff, std::consume_header>, char32_t> utf16be_cvt; // default big endian
        std::u32string u32_str_from_le = utf16le_cvt.from_bytes(u16le_byte_str); // utf-16 to ucs4
        std::u32string u32_str_from_be = utf16be_cvt.from_bytes(u16be_byte_str); // utf-16 to ucs4
        std::cout << "u16le_byte_str: ";
        print_code_unit_sequence(u16le_byte_str);
        std::cout << std::endl;
        std::cout << "u16be_byte_str: ";
        print_code_unit_sequence(u16be_byte_str);
        std::cout << std::endl;
        std::cout << "u32_str_from_le: ";
        print_code_unit_sequence(u32_str_from_le);
        std::cout << std::endl;
        std::cout << "u32_str_from_be: ";
        print_code_unit_sequence(u32_str_from_be);
        std::cout << std::endl;
    }
    
    {
        // 通过UCS4转UTF-16 LE和UTF-16 BE演示了std::generate_header的使用
        std::u32string u32_str = U"破晓"; // ucs4
        std::wstring_convert<std::codecvt_utf16<char32_t, 0x10ffff, static_cast<std::codecvt_mode>(std::generate_header | std::little_endian)>, char32_t> utf16le_cvt; // little endian
        std::wstring_convert<std::codecvt_utf16<char32_t, 0x10ffff, static_cast<std::codecvt_mode>(std::generate_header)>, char32_t> utf16be_cvt; // default big endian
        std::string u16le_byte_str = utf16le_cvt.to_bytes(u32_str); // ucs4 to utf-16 little endian with BOM
        std::string u16be_byte_str = utf16be_cvt.to_bytes(u32_str); // ucs4 to utf-16 big endian with BOM
        std::cout << "u32_str: ";
        print_code_unit_sequence(u32_str);
        std::cout << std::endl;
        std::cout << "u16le_byte_str: ";
        print_code_unit_sequence(u16le_byte_str);
        std::cout << std::endl;
        std::cout << "u16be_byte_str: ";
        print_code_unit_sequence(u16be_byte_str);
        std::cout << std::endl;
    }
    
}

int main(int argc, char* argv[]) {
    using namespace std;
    
    wchar_t bool_buf[8] = {'c','c','c','c','c','c','c','c'};
    std::size_t l = inf::cstr_from_bool(bool_buf, 8, false);
    
    inf::cmd_arg cmdArgs;
    
    cmdArgs.initialize(argc, argv);
    cmdArgs.self_test();
    
    const char* arg0 = cmdArgs.get_arg(0);
    int c = cmdArgs.count();
    
    char_test();
    Test_MultiMethod();
    
    float i=56;
    char targetStr[20]={0};
    sprintf(targetStr, "%f", i);
    
    float a = 3.4f, b = 6.8f;
    double s = 3.4, t = 6.8;
    int r = 5, q = 9;
    std::string sa("aaa"), sb("bbb");
    
    ::swap(a,b);
    ::swap(s,t);
    ::swap(r,q);
    ::swap(sa,sb);

    
    return 0;
}