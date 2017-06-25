//
//  main.cpp
//  Demo_RapidJSON
//
//  Created by Cyberzei on 8/26/16.
//  Copyright © 2016 Cyberzei. All rights reserved.
//

#include "JsonCommand.h"
#include "NetConsole.h"

<<<<<<< HEAD
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

typedef rapidjson::UTF8<> Encoding;
typedef rapidjson::GenericStringBuffer<Encoding> StringBuffer;
typedef StringBuffer::Ch    Char;
typedef rapidjson::GenericDocument<Encoding> Document;
typedef rapidjson::GenericValue<Encoding> Value;
typedef rapidjson::Writer<StringBuffer> JsonWriter;

using namespace rapidjson;

static unsigned int msgNoIncrement = 0;

struct JValue;

class Command {
    
    Document cmdDoc;
    Value& cmd_tpl;

    bool parse_checking_ready() {
        bool error = cmdDoc.HasParseError();
        if (error) {
            rapidjson::ParseErrorCode errorCode = cmdDoc.GetParseError();
            std::cout<<"Error: Failed to parse json -- code = "<<(int)errorCode<<std::endl;
            return false;
        }
        
        return true;
    }
    
    Value& build_cmd_template() {
        static const char* json =
        "{\
            \"CMD_BODY\": \
            { \
            }, \
            \"CMD_ID\":0, \
            \"CMD_STATUS\":1, \
            \"CMD_TYPE\":1, \
            \"msgno\":0 \
        }";
        
        cmdDoc.Parse(json);
        assert( parse_checking_ready() );
        
        return cmdDoc;
    }
    
    void cmd_common_dynamic_update() {
        cmd_tpl["CMD_ID"] = 175;
        cmd_tpl["msgno"] = msgNoIncrement++;

    }
    
    friend struct JValue;
    
public:
    enum cmd_t {
        GetCode,
        RegisterUser,
        LoginUser,
        EventOccur
    };
    
    Command() : cmd_tpl( build_cmd_template() ) {}

    void print() {
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);

        // string stream
        cmdDoc.Accept(writer);
        std::cout << buffer.GetString() << std::endl;
    }
    
    void build_cmd_get_code(const char* mobileNumber, int type) {
        
        // modify CMD_BODY
        Value& body = cmd_tpl["CMD_BODY"];
        Value mobile( kStringType );
        mobile.SetString(mobileNumber, static_cast<SizeType>(strlen(mobileNumber)), cmdDoc.GetAllocator());
        if( !body.HasMember("mobile") )
            body.AddMember("mobile", mobile, cmdDoc.GetAllocator());
        else
            body["mobile"] = mobile;

        if( !body.HasMember("type") )
            body.AddMember("type", type, cmdDoc.GetAllocator());
        else
            body["type"] = type;
        //    Value from( kArrayType );
        //    from.PushBack(1, cmdDoc.GetAllocator());
        //    from.PushBack(2, cmdDoc.GetAllocator());
        //    body.AddMember("from", from, cmdDoc.GetAllocator());
        
        cmd_common_dynamic_update();
    }
    
    void build_cmd(cmd_t code, ...);
    void build_cmd(cmd_t code, JValue values[], ...);
    void build_cmd(cmd_t code, const char* jsonBody);
    void build_cmd_raw(const char* cmdText);
    
//    void clear() {
//        Value& cmd_tpl = build_cmd_template();
//        
//        // modify CMD_BODY
//        Value& body = cmd_tpl["CMD_BODY"];
//        body.RemoveAllMembers();
//    }
    
    Document::AllocatorType& get_allocator() {
        return cmdDoc.GetAllocator();
    }
};

Command c;

struct JValue: protected Value {
    using Value::Value;
    
    
    template<typename T>
    void PushValue(T value) {
        PushBack(value, c.get_allocator());
    }
};
=======
/*
 terminal:
    nc 127.0.0.1 5600
    >help
    >
 */
>>>>>>> 61a844674c78dab3e8891970b905a94b6471cf98

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