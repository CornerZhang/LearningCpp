//
//  chap_03.cpp
//  CppPrimer
//
//  Created by huazhang on 10/30/15.
//  Copyright © 2015 huazhang. All rights reserved.
//

#include "chap_03.hpp"
#include "iostream"
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

void stdstream() {
    cout<<"Enter two numbers:"<<endl;
    int v1, v2;
    cin >> v1 >> v2;
    cout<<v1 + v2<<endl;
    
}

void Test_chap_03() {
    string s("Hello world!");
    for(auto& c : s) {
        c = toupper(c);
    }
    
    cout<<s<<endl;
}