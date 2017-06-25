//
//  BinaryData_Demo.hpp
//  DemoCpp
//
//  Created by CornerZhang on 6/25/17.
//  Copyright © 2017 huazhang. All rights reserved.
//

#ifndef BinaryData_Demo_hpp
#define BinaryData_Demo_hpp

constexpr char FILE_EXT_NAME[] = "dat";

typedef struct binary_data_s {
    unsigned char    flags;
    unsigned int     version;
    unsigned long long   size;
    float       rate;
    double      scale;
    bool        passed;
} binary_data_t;


bool    dat_file_save(const char* filename, const binary_data_t& data);
void    dat_file_load(binary_data_t* data, const char* filename);

void    dat_file_testself();

#endif /* BinaryData_Demo_hpp */
