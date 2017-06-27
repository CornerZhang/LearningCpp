//
//  JsonCommand.cpp
//  Demo_RapidJSON
//
//  Created by Cyberzei on 8/29/16.
//  Copyright © 2016 Cyberzei. All rights reserved.
//

#include "PrefixHeader.h"


JsonCommand* _PrivateInstallJsonCommand() {
    static JsonCommand jsonCommand;
    return &jsonCommand;
}


JsonCommand* jcmd = _PrivateInstallJsonCommand();

unsigned int JsonCommand::msgNoIncrement = 0;

JsonCommand::JsonCommand() : cmd_tpl( build_cmd_template() ) {}


bool JsonCommand::parse_checking_ready() {
    bool error = cmdDoc.HasParseError();
    if (error) {
        rapidjson::ParseErrorCode errorCode = cmdDoc.GetParseError();
        std::cout<<"Error: Failed to parse json -- code = "<<(int)errorCode<<std::endl;
        return false;
    }
    
    return true;
}

Value& JsonCommand::build_cmd_template() {
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

Value& JsonCommand::before_body(unsigned int cmdId) {
    cmd_tpl["CMD_ID"] = cmdId;
    
    // modify CMD_BODY
    Value& body = cmd_tpl["CMD_BODY"];
    body.RemoveAllMembers();
    
    return body;
}

void JsonCommand::bind_member(Value& body, const char* const key, const char* value) {
    Value localValue( kStringType );
    localValue.SetString(value, static_cast<SizeType>(strlen(value)), get_allocator());
    if( !body.HasMember(key) ) {
        body.AddMember(StringRef(key), localValue, get_allocator());
    }
    else
        body[key] = localValue;
}

void JsonCommand::bind_member(Value& body, const char* const key, unsigned int value) {
    if( !body.HasMember(key) ) {
        body.AddMember(StringRef(key), value, get_allocator());
    }
    else
        body[key] = value;
}

void JsonCommand::bind_member(Value& body, const char* const key, double value) {
    if( !body.HasMember(key) ) {
        body.AddMember(StringRef(key), value, get_allocator());
    }
    else
        body[key] = value;
}

void JsonCommand::cmd_common_dynamic_update() {
    cmd_tpl["msgno"] = msgNoIncrement++;
}

void JsonCommand::print() {
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    
    // string stream
    cmdDoc.Accept(writer);
    std::cout << buffer.GetString() << std::endl;
}

void JsonCommand::build_cmd_GetCode(const char* mobile, unsigned int type) {
    Value& body = before_body(174);
    
    bind_member(body, "mobile", mobile);
    bind_member(body, "type", type);
    
    cmd_common_dynamic_update();
}

void JsonCommand::build_cmd_RegisterUser(const char *nuname, const char *nupwd, const char *nuv) {
    Value& body = before_body(112);

    bind_member(body, "nuname", nuname);
    bind_member(body, "nupwd", nupwd);
    bind_member(body, "nuv", nuv);
    
    cmd_common_dynamic_update();
}

void JsonCommand::build_cmd_LoginUser(const char *uname, const char *upwd, unsigned int FORCE, const char *token, double lng, double lat, unsigned int dtype) {
    Value& body = before_body(26);
    
    bind_member(body, "uname", uname);
    bind_member(body, "upwd", upwd);
    bind_member(body, "FORCE", FORCE);
    bind_member(body, "token", token);
    bind_member(body, "lng", lng);
    bind_member(body, "lat", lat);
    bind_member(body, "dtype", dtype);
    
    // fixed member
    Value from( kArrayType );
    from.PushBack(1, get_allocator());
    from.PushBack(2, get_allocator());
    body.AddMember("from", from, get_allocator());
    
    cmd_common_dynamic_update();
}

void JsonCommand::build_cmd_ChangePWD(long long userId /*PLAYER_ID*/, const char* oldpwd, const char* newpwd) {
    
}

void JsonCommand::build_cmd_ResetPWD(const char* mobile /*nuname*/, const char* newpwd /*nupwd*/, const char* nuv) {
    
}

void JsonCommand::build_cmd_GetVerifyImage(long long userId) {
    
}

void JsonCommand::build_cmd_ChangeImage(long long userId /*PLAYER_ID*/, int headImageId /*imgid*/) {
    
}

void JsonCommand::build_cmd_FinishUpload(long long userId /*PLAYER_ID*/, const char* url /*ufn*/) {
    
}

void JsonCommand::build_cmd_FinishPlateUpload(long long userId /*PLAYER_ID*/, const char* platefilename) {
    
}

void JsonCommand::build_cmd_EventOccur(long long userId /*PLAYER_ID*/, const char* eventname, int eventtype, double lng, double lat, const char* refresource) {
    
}

void JsonCommand::build_cmd_EventUpdateFromServer(long long userId /*PLAYER_ID*/, double rangeRadius) {
    
}

void JsonCommand::build_cmd_EventQueryFromServer(long long userId /*PLAYER_ID*/, const char* timeBegin, const char* timeEnd, const char* keywords) {
    
}

void JsonCommand::build_cmd_GetSystemMessage(long long userId) {
    
}

void JsonCommand::build_cmd_Ping(long long userId /*PLAYER_ID*/) {
    
}


Document::AllocatorType& JsonCommand::get_allocator() {
    return cmdDoc.GetAllocator();
}
