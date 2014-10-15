//
//  TestSTDC.c
//  ModernCpp
//
//  Created by CornerZhang on 14-10-14.
//  Copyright (c) 2014年 NeXtreme.com. All rights reserved.
//

#include "TestSTDC.h"
#include <iostream>

void TestSTDC_version() {
    using namespace std;
    cout<< "Standard clib: "<< __STDC_HOSTED__ <<endl;
    cout<<"Standard C: "<<__STDC__<<endl;
//    cout<<"C Standard verison: "<<__STDC_VERSION__<<endl;
//    cout<"ISO/IEC "<<__STDC_ISO_10646__<<endl;
}