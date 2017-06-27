//
//  NetConsole.hpp
//  NetConsole
//
//  Created by Cyberzei on 8/24/16.
//  Copyright © 2016 Cyberzei. All rights reserved.
//

#ifndef NetConsole_hpp
#define NetConsole_hpp


/*
 *  usage:
 *      * add "netConsole->listenOnTCP(5678);" to main function, and with #include "NetConsole.h"
 *      * open in terminal
 *      * >nc localhost 5678
 */

class NetConsole {
    friend NetConsole* _PrivateInstallNetConsole();
    
public:
    static constexpr int MAX_LOG_LENGTH = 16 * 1024;
    
    /** Command Struct */
    struct Command {
        using Callback = std::function<void(int fd, const std::string& args)>;
        std::string name;
        std::string help;
        Callback callback{nullptr};
        std::map<std::string, Command> subCommands;
        
        /** Constructor */
        Command() {}
        Command(std::string name_, std::string help_) : name(name_), help(help_) {};
        Command(std::string name_, std::string help_, Callback callback_) : name(name_), help(help_), callback(callback_) {};
        
        /** add callback */
        void addCallback(const Callback& callback);
        
        /** add sub command */
        void addSubCommand(const Command& subCmd);
        
        /** get sub command */
        const Command* getSubCommand(const std::string& subCmdName) const;
        
        /** delete sub command */
        void delSubCommand(const std::string& subCmdName);
        
        /** help command handler */
        void commandHelp(int fd, const std::string& args);
        
        /** generic command handler */
        void commandGeneric(int fd, const std::string& args);
    };
    
    /**
     @brief Output Debug message.
     */
    static void log(const char * format, ...) NC_FORMAT_PRINTF(1, 2);

    
    NetConsole();
    
    ~NetConsole();
    
    /** starts listening to specified TCP port */
    bool listenOnTCP(int port);
    
    /** starts listening to specified file descriptor */
    bool listenOnFileDescriptor(int fd);
    
    /** stops the Console. 'stop' will be called at destruction time as well */
    void stop();
    
    /** add custom command */
    void addCommand(const Command& cmd);
    void addSubCommand(const std::string& cmdName, const Command& subCmd);
    void addSubCommand(Command& cmd, const Command& subCmd);
    
    /** get custom command */
    const Command* getCommand(const std::string& cmdName);
    const Command* getSubCommand(const std::string& cmdName, const std::string& subCmdName);
    const Command* getSubCommand(const Command& cmd, const std::string& subCmdName);
    
    /** delete custom command */
    void delCommand(const std::string& cmdName);
    void delSubCommand(const std::string& cmdName, const std::string& subCmdName);
    void delSubCommand(Command& cmd, const std::string& subCmdName);
    
    /** log something in the console */
    void nlog(const char *buf);
    
    /**
     * set bind address
     *
     * @address : 127.0.0.1
     */
    void setBindAddress(const std::string &address);
protected:
    void loop();
    
    // Helpers
    ssize_t readline(int fd, char *buf, size_t maxlen);
    ssize_t readBytes(int fd, char* buffer, size_t maxlen, bool* more);
    bool parseCommand(int fd);
    
    void addClient();
    
    // create a map of command.
    void createCommand_GetCode();
    void createCommand_RegisterUser();
    void createCommand_LoginUser();
    void createCommand_ResetPSW();
    void createCommand_ChangePSW();
    void createCommand_ChangeUserProfileImage();
    void createCommand_BindCar();
    void createCommand_UnbindCar();
    void createCommand_CommentOtherUser();
    void createCommand_CommentListForMe();
    void createCommand_FinishSnapshotUpload();
    void createCommand_GetSystemMsg();
    void createCommand_DebugMsg();
    void createCommand_Exit();
    void createCommand_Help();

    // Add commands here
    void command_GetCode(int fd, const std::string& args);
    void command_RegisterUser(int fd, const std::string& args);
    void command_LoginUser(int fd, const std::string& args);
    void command_ResetPSW(int fd, const std::string& args);
    void command_ChangePSW(int fd, const std::string& args);
    void command_ChangeUserProfileImage(int fd, const std::string& args);
    void command_BindCar(int fd, const std::string& args);
    void command_UnbindCar(int fd, const std::string& args);
    void command_CommentOtherUser(int fd, const std::string& args);
    void command_CommentListForMe(int fd, const std::string& args);
    void command_FinishSnapshotUpload(int fd, const std::string& args);
    void command_GetSystemMsg(int fd, const std::string& args);
    void command_DebugMsg(int fd, const std::string& args);
    void command_DebugMsgSubCommandOnOff(int fd, const std::string& args);
    void command_Exit(int fd, const std::string& args);
    void command_Help(int fd, const std::string& args);

    
    // file descriptor: socket, console, etc.
    int _listenfd;
    int _maxfd;
    std::vector<int> _fds;
    std::thread _thread;
    
    fd_set _read_set;
    
    bool _running;
    bool _endThread;
    
    std::map<std::string, Command> _commands;
    
    // strings generated by cocos2d sent to the remote console
    bool _sendDebugStrings;
    std::mutex _DebugStringsMutex;
    std::vector<std::string> _DebugStrings;
    
    intptr_t _touchId;
    
    std::string _bindAddress;
private:
    /** send help message to console */
    static void sendHelp(int fd, const std::map<std::string, Command>& commands, const char* msg);

};

extern NetConsole* netConsole;

#endif /* NetConsole_hpp */
