//
//  JsonCommand.hpp
//  Demo_RapidJSON
//
//  Created by Cyberzei on 8/29/16.
//  Copyright © 2016 Cyberzei. All rights reserved.
//

#ifndef JsonCommand_hpp
#define JsonCommand_hpp


using namespace rapidjson;


struct JValue;

class JsonCommand {
    friend JsonCommand* _PrivateInstallJsonCommand();
public:
    void build_cmd_GetCode(const char* mobile, unsigned int type);
    void build_cmd_RegisterUser(const char* nuname, const char* nupwd, const char* nuv);
    void build_cmd_LoginUser(const char* uname, const char* upwd, unsigned int FORCE, const char* token, double lng, double lat, unsigned int dtype);   //3 = android, 4 = ios, 5 = wp
    void build_cmd_ChangePWD(long long userId /*PLAYER_ID*/, const char* oldpwd, const char* newpwd);
    void build_cmd_ResetPWD(const char* mobile /*nuname*/, const char* newpwd /*nupwd*/, const char* nuv);
    void build_cmd_GetVerifyImage(long long userId);
    void build_cmd_ChangeImage(long long userId /*PLAYER_ID*/, int headImageId /*imgid*/);
    void build_cmd_FinishUpload(long long userId /*PLAYER_ID*/, const char* url /*ufn*/);
    void build_cmd_FinishPlateUpload(long long userId /*PLAYER_ID*/, const char* platefilename);
    void build_cmd_EventOccur(long long userId /*PLAYER_ID*/, const char* eventname, int eventtype, double lng, double lat, const char* refresource);
    void build_cmd_EventUpdateFromServer(long long userId /*PLAYER_ID*/, double rangeRadius);
    void build_cmd_EventQueryFromServer(long long userId /*PLAYER_ID*/, const char* timeBegin, const char* timeEnd, const char* keywords =nullptr /*all*/);
    void build_cmd_GetSystemMessage(long long userId);
    void build_cmd_Ping(long long userId /*PLAYER_ID*/);
    
    void print();
    
    
private:
    Document cmdDoc;
    Value& cmd_tpl;

    static unsigned int msgNoIncrement;
    
    JsonCommand();

    
    Document::AllocatorType& get_allocator();
    
    bool parse_checking_ready();
    Value& build_cmd_template();
    Value& before_body(unsigned int cmdId);
    void cmd_common_dynamic_update();

    void bind_member(Value& body, const char* const key, const char* value);
    void bind_member(Value& body, const char* const key, unsigned int value);
    void bind_member(Value& body, const char* const key, double value);
    
};

extern JsonCommand* jcmd;

#endif /* JsonCommand_hpp */
