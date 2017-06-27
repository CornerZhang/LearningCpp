//
//  PrefixHeader.h
//  Demo_RapidJSON
//
//  Created by CornerZhang on 6/27/17.
//  Copyright © 2017 Cyberzei. All rights reserved.
//

#ifndef PrefixHeader_h
#define PrefixHeader_h

// Include any system framework and library headers here that should be included in all compilation units.
// You will also need to set the Prefix Header build setting of one or more of your targets to reference this file.

// common
#include <cassert>
#include <cstdarg>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <functional>

// NetConsole
#include "PlatformConfig.h"
#include "NetConsolePrequest.h"
#include "ConsoleUtility.h"
#include "NetConsole.h"

// JsonCommand
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "JsonCommand.h"

#endif /* PrefixHeader_h */
