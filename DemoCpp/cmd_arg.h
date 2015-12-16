//
//  cmd_arg.h
//  DemoCpp
//
//  Created by huazhang on 11/25/15.
//  Copyright © 2015 huazhang. All rights reserved.
//

#ifndef cmd_arg_h
#define cmd_arg_h

namespace inf {
    class cmd_arg {
    public:
        cmd_arg() = default;
        
        void initialize(int argc, char* argv[]);
        void initialize(const wchar_t* cmd_line_str, int count);
        
        int         count() const;
        const char* get_arg(int index) const;
        const char* get_arg_value(int index, char split_char = '=') const;
        
        static void self_test();
    private:
        void        debug_printf() const;
        
        using arg_t = const char*;
        static constexpr int ARG_NUM = 64;
        
        arg_t   args[ARG_NUM];
        int     arg_count;
    };
    
}

#endif /* cmd_arg_h */
