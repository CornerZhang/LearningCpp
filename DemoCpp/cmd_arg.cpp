//
//  cmd_arg.cpp
//  DemoCpp
//
//  Created by huazhang on 11/25/15.
//  Copyright © 2015 huazhang. All rights reserved.
//

#include "cmd_arg.h"
#include <cassert>
#include <iostream>
#include <cwchar>

namespace {
    void wstr_to_str(char* out_buf, const wchar_t* input_wstr) {
        std::size_t wlen = std::wcslen(input_wstr);
        std::wcstombs(out_buf, input_wstr, wlen);
    }
    
    char* cmd_str_split_to_args(const char** args, int arg_size, int* arg_count, const char* cmd_str, const char* ignore_postfix = ".exe") {
        args[0] = cmd_str;  // program name
        const char* begin_str = std::strstr(cmd_str, ignore_postfix);   // skip program name
        begin_str += 4;
        
        char* without_program_name = const_cast<char*>(begin_str);
        
        int i=1;
        while (true) {
            begin_str = std::strchr(begin_str, ' ');
            if (!begin_str) {
                break;
            }
            
            ++begin_str;    // skip space
            args[i] = begin_str;
            ++i;
        }
        
        args[i] = nullptr;  // flag as end
        
        *arg_count = i;
        
        return without_program_name;
    }
    
    void str_replace(char* str, char orign, char replace) {
        while (*str!='\0') {
            if ((*str) == orign) {
                *str = replace;
            }
            ++str;
        }
    }
}

using namespace std;

namespace inf {
    void cmd_arg::initialize(int argc, char **argv) {
        int i = 0;
        for (; i<argc; ++i) {
            args[i] = argv[i];
        }
        args[i] = nullptr;
        arg_count = argc;
    }
    
    void cmd_arg::initialize(const wchar_t *cmd_line_wstr, int count) { // win
        static char cmd_str[512] = {0};
        
        wstr_to_str(cmd_str, cmd_line_wstr);
        
        // split args to member array, window platform
        char* without_program_name = cmd_str_split_to_args(args, ARG_NUM, &arg_count, cmd_str); // without_program_name as sub-string in cmd_str
        
        str_replace(without_program_name, ' ', '\0');
    }
    
    int cmd_arg::count() const {
        return arg_count;
    }
    
    const char* cmd_arg::get_arg(int index) const {
        assert(index < ARG_NUM);
        
        return args[index];
    }
    
    const char* cmd_arg::get_arg_value(int index, char split_char) const {
        const char* arg_str = get_arg(index);
        if(!arg_str)
            return nullptr;
        
        const char* found = std::strchr(arg_str, split_char);
        if(!found) {
            cout<<"runtime error - in get_arg_value: not found the "<<split_char<<endl;
            
            return nullptr;
        }
        
        return ( found + 1 );
    }
    
    void cmd_arg::self_test() {
        cmd_arg cmd_args;
        const wchar_t* sim_test = L"C:/windows/user/ppa.exe configName=a.txt mont=34";
        cmd_args.initialize(sim_test, 64);
        
        cmd_args.debug_printf();
    }
    
    void cmd_arg::debug_printf() const {
        const arg_t* p = args;
        while ( *p ) {
            cout<<(*p)<<endl;
            ++p;
        }
    }}
