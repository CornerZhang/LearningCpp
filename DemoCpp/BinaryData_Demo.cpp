//
//  BinaryData_Demo.cpp
//  DemoCpp
//
//  Created by CornerZhang on 6/25/17.
//  Copyright © 2017 huazhang. All rights reserved.
//

#include "BinaryData_Demo.h"
#include <cstdio>

#include "inf/endian.h"
#include "inf/cstrfunc.h"
#include "inf/cstrprintf.h"

typedef struct binary_data_conv_e {
    unsigned char flags;
    unsigned int    version;
    unsigned long long size;
    char    rate[9];
    char    scale[16];
    bool    passed;
} binary_data_conv_t;

bool    dat_file_save(const char* filename, const binary_data_t& data) {
    FILE* fh = fopen(filename, "wb");

    size_t c;
    binary_data_conv_t target_data;
    
    target_data.flags = data.flags;
    if(inf::is_big_endian()) {
        target_data.version = inf::little_long(data.version);
        target_data.size = inf::little_long(data.size);
        target_data.passed = inf::little_short(data.passed);
    }else{
        target_data.version = data.version;
        target_data.size = data.size;
        target_data.passed = data.passed;
    }
    inf::snPrintf(target_data.rate, 9, "%f",inf::little_float(data.rate));
    inf::snPrintf(target_data.scale, 16, "%g",inf::little_double(data.scale));
    c = fwrite(&target_data, 1, sizeof(target_data), fh);
    
    if(c==0) {  // Error
        fclose(fh);
        return false;
    }
        
    fclose(fh);
    return true;
}

void    dat_file_load(binary_data_t* data, const char* filename) {
    FILE* fh = fopen(filename, "rb");
    
    size_t c;
    binary_data_conv_t source_data;

    c = fread(&source_data, 1, sizeof(source_data), fh);
    if(c==0) {
        fclose(fh);
        return;
    }
    data->flags = source_data.flags;
    if(inf::is_big_endian()) {
        data->version = inf::big_long(source_data.version);
        data->size = inf::big_long(source_data.size);
        data->passed = inf::big_short(source_data.passed);
    }else{
        data->version = source_data.version;
        data->size = source_data.size;
        data->passed = source_data.passed;
    }
    data->rate = inf::cstr_to_float(source_data.rate);
    data->scale = inf::cstr_to_double(source_data.scale);
    
    fclose(fh);
}

void    dat_build_test(binary_data_t* data) {
    data->flags = 0;
    data->version = 1;
    data->size = 16;
    data->rate = 0.1;
    data->scale = 1.0;
    data->passed = true;
}

void    dat_build_print(binary_data_t& data) {
    printf("<----------bin data---------->\n");
    printf("%x\n", data.flags);
    printf("%d\n", data.version);
    printf("%lld\n", data.size);
    printf("%f\n", data.rate);
    printf("%f\n", data.scale);
    printf("%d\n", data.passed);
}

void    dat_file_testself() {
    const char* test_filename = "test";
    
    char target_filename[80]={0};
    inf::cstr_append(target_filename, 80, test_filename);
    inf::cstr_append(target_filename, 80, ".");
    inf::cstr_append(target_filename, 80, FILE_EXT_NAME);

    inf::endian_init();
    
    binary_data_t data;
    dat_build_test(&data);
    dat_file_save(target_filename, data);
    
    binary_data_t loaded_data;
    dat_file_load(&loaded_data, target_filename);
    dat_build_print(loaded_data);
}
