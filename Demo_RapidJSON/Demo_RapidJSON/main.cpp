//
//  main.cpp
//  Demo_RapidJSON
//
//  Created by Cyberzei on 8/26/16.
//  Copyright © 2016 Cyberzei. All rights reserved.
//

#include "JsonCommand.h"
#include "NetConsole.h"

/*
 terminal:
    nc 127.0.0.1 5600
    >help
    >
 */

int main() {
    // listen on console
    netConsole->listenOnTCP(5600);
    
    // on logic code
    jcmd->build_cmd_GetCode("13611748901", 1);
    jcmd->print();
    
    jcmd->build_cmd_RegisterUser("CornerZhang", "111111", "231123");
    jcmd->print();
    
    jcmd->build_cmd_LoginUser("CornerZhang", "111111", 1, "nouse", 23.1123, 43.233, 4);
    jcmd->print();
    
    
    sleep(1000000);
    
    return 0;
}