//
//  main.cpp
//  NetConsole
//
//  Created by Cyberzei on 8/24/16.
//  Copyright © 2016 Cyberzei. All rights reserved.
//

#include <iostream>
#include "NetConsole.h"

int main(int argc, const char * argv[]) {
    netConsole->listenOnTCP(5678);
    
    sleep(100000);
    return 0;
}
