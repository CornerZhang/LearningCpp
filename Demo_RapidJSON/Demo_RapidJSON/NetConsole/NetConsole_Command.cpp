#include "PrefixHeader.h"

// new callbacks based on C++11
#define NC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define NC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define NC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define NC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)

namespace {
    ssize_t (*consolePrintf)(int sock, const char *format, ...) = ConsoleUtility::mydprintf;
}   // ::

void NetConsole::createCommand_GetCode() {
    const char* usageText = "获得注册账号所需的验证短信码. Args: [-h | help | <your-mobile> <type[1|2]> ]";
    addCommand({"getCode", usageText, NC_CALLBACK_2(NetConsole::command_GetCode, this)});
}

void NetConsole::createCommand_RegisterUser() {
}

void NetConsole::createCommand_LoginUser() {
}

void NetConsole::createCommand_ResetPSW() {
}

void NetConsole::createCommand_ChangePSW() {
    
}

void NetConsole::createCommand_ChangeUserProfileImage() {
    
}

void NetConsole::createCommand_BindCar() {
    
}

void NetConsole::createCommand_UnbindCar() {
    
}



void NetConsole::createCommand_CommentOtherUser() {
    
}

void NetConsole::createCommand_CommentListForMe() {
    
}

void NetConsole::createCommand_FinishSnapshotUpload() {
}

void NetConsole::createCommand_GetSystemMsg() {
    
}

void NetConsole::createCommand_DebugMsg()
{
    addCommand({"debugMsg", "Whether or not to forward the debug messages on the console. Args: [-h | help | on | off | ]",
        NC_CALLBACK_2(NetConsole::command_DebugMsg, this)});
    addSubCommand("debugMsg", {"on", "enable debug logging", NC_CALLBACK_2(NetConsole::command_DebugMsgSubCommandOnOff, this)});
    addSubCommand("debugMsg", {"off", "disable debug logging", NC_CALLBACK_2(NetConsole::command_DebugMsgSubCommandOnOff, this)});
}

void NetConsole::createCommand_Exit()
{
    addCommand({"exit", "Close connection to the console. Args: [-h | help | ]", NC_CALLBACK_2(NetConsole::command_Exit, this)});
}

void NetConsole::createCommand_Help()
{
    addCommand({"help", "Print this message. Args: [ ]", NC_CALLBACK_2(NetConsole::command_Help, this)});
}

bool c_is_digit( char c ) {
    return ( c <= '9' && c >= '0' );
}

bool cstr_is_numeric( const char * s ) {
    std::size_t		i;
    bool	dot;
    
    if ( *s == '-' ) {
        s++;
    }
    
    dot = false;
    for ( i = 0; s[i]; i++ ) {
        if ( !c_is_digit( s[i] ) ) {
            if ( ( s[ i ] == '.' ) && !dot ) {
                dot = true;
                continue;
            }
            return false;
        }
    }
    
    return true;
}

int	cstr_to_integer(const char* str) {
    int val = 0;
    int i = 0;
    int m = (str[0] == '-' ) ? -1 : 1;
    
    if ( str[0] == '-'  || str[0] == '+' )
        i++;
    
    int max = 10 + i;
    for ( ; c_is_digit(str[i]) && i<max; i++ )
        val = 10 * val + (str[i] - '0');
    
    return val * m;
}

// commands
void NetConsole::command_GetCode(int fd, const std::string& args) {
    std::vector<std::string> argsLoc;
    ConsoleUtility::split(args, ' ', argsLoc);
    
    int type=0;
    if ( cstr_is_numeric(argsLoc[1].c_str()) ) {
        type = cstr_to_integer(argsLoc[1].c_str());
        jcmd->build_cmd_GetCode(argsLoc[0].c_str(), type);
        jcmd->print();
    }else{
        log("\t Error: %s arg[1]", argsLoc[1].c_str());
        ::consolePrintf(fd, "\t Error: %s arg[1]", argsLoc[1].c_str());
    }
}

void NetConsole::command_RegisterUser(int fd, const std::string& args) {
}

void NetConsole::command_LoginUser(int fd, const std::string& args) {
}

void NetConsole::command_ResetPSW(int fd, const std::string& args)  {
}

void NetConsole::command_ChangePSW(int fd, const std::string& args) {
    
}
void NetConsole::command_ChangeUserProfileImage(int fd, const std::string& args) {
    
}
void NetConsole::command_BindCar(int fd, const std::string& args) {
    
}
void NetConsole::command_UnbindCar(int fd, const std::string& args) {
    
}
void NetConsole::command_CommentOtherUser(int fd, const std::string& args) {
    
}
void NetConsole::command_CommentListForMe(int fd, const std::string& args) {
    
}

void NetConsole::command_FinishSnapshotUpload(int fd, const std::string& args) {
}

void NetConsole::command_GetSystemMsg(int fd, const std::string& args) {
    
}

void NetConsole::command_DebugMsg(int fd, const std::string& args)
{
    ::consolePrintf(fd, "Debug message is: %s\n", _sendDebugStrings ? "on" : "off");
}

void NetConsole::command_DebugMsgSubCommandOnOff(int fd, const std::string& args)
{
    _sendDebugStrings = (args.compare("on") == 0);
}

void NetConsole::command_Exit(int fd, const std::string& args)
{
    FD_CLR(fd, &_read_set);
    _fds.erase(std::remove(_fds.begin(), _fds.end(), fd), _fds.end());
#if (NC_TARGET_PLATFORM == NC_PLATFORM_WIN32) || (NC_TARGET_PLATFORM == NC_PLATFORM_WINRT)
    closesocket(fd);
#else
    close(fd);
#endif
}

void NetConsole::command_Help(int fd, const std::string& args)
{
    sendHelp(fd, _commands, "\nAvailable commands:\n");
}

void NetConsole::sendHelp(int fd, const std::map<std::string, Command>& commands, const char* msg)
{
    ConsoleUtility::sendToConsole(fd, msg, strlen(msg));
    for(auto it=commands.begin();it!=commands.end();++it)
    {
        auto command = it->second;
        if (command.help.empty()) continue;
        
        ::consolePrintf(fd, "\t%s", command.name.c_str());
        ssize_t tabs = strlen(command.name.c_str()) / 8;
        tabs = 3 - tabs;
        for(int j=0;j<tabs;j++){
            ::consolePrintf(fd, "\t");
        }
        ::consolePrintf(fd,"%s\n", command.help.c_str());
    }
}
