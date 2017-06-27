//
//  NetConsole.cpp
//  NetConsole
//
//  Created by Cyberzei on 8/24/16.
//  Copyright © 2016 Cyberzei. All rights reserved.
//

#include "PrefixHeader.h"

namespace {
    
    
#if defined(__MINGW32__)
    // inet
    const char* inet_ntop(int af, const void* src, char* dst, int cnt)
    {
        struct sockaddr_in srcaddr;
        
        memset(&srcaddr, 0, sizeof(struct sockaddr_in));
        memcpy(&(srcaddr.sin_addr), src, sizeof(srcaddr.sin_addr));
        
        srcaddr.sin_family = af;
        if (WSAAddressToStringA((struct sockaddr*) &srcaddr, sizeof(struct sockaddr_in), 0, dst, (LPDWORD) &cnt) != 0)
        {
            return nullptr;
        }
        return dst;
    }
#endif
    
    //
    // Free functions to log
    //
#if (NC_TARGET_PLATFORM == NC_PLATFORM_WIN32)
    void SendLogToWindow(const char *log)
    {
        static const int CCLOG_STRING_TAG = 1;
        // Send data as a message
        COPYDATASTRUCT myCDS;
        myCDS.dwData = CCLOG_STRING_TAG;
        myCDS.cbData = (DWORD)strlen(log) + 1;
        myCDS.lpData = (PVOID)log;
        if (Director::getInstance()->getOpenGLView())
        {
            HWND hwnd = Director::getInstance()->getOpenGLView()->getWin32Window();
            SendMessage(hwnd,
                        WM_COPYDATA,
                        (WPARAM)(HWND)hwnd,
                        (LPARAM)(LPVOID)&myCDS);
        }
    }
#elif NC_TARGET_PLATFORM == NC_PLATFORM_WINRT
    void SendLogToWindow(const char *log)   // BE EMPTY!!!
    {
    }
#endif
    
    void _log(const char *format, va_list args)
    {
        int bufferSize = NetConsole::MAX_LOG_LENGTH;
        char* buf = nullptr;
        
        do
        {
            buf = new (std::nothrow) char[bufferSize];
            if (buf == nullptr)
                return; // not enough memory
            
            int ret = vsnprintf(buf, bufferSize - 3, format, args);
            if (ret < 0)
            {
                bufferSize *= 2;
                
                delete [] buf;
            }
            else
                break;
            
        } while (true);
        
        strcat(buf, "\n");
        
#if NC_TARGET_PLATFORM == NC_PLATFORM_ANDROID
        __android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x debug info", "%s", buf);
        
#elif NC_TARGET_PLATFORM ==  NC_PLATFORM_WIN32 || NC_TARGET_PLATFORM == NC_PLATFORM_WINRT
        
        int pos = 0;
        int len = strlen(buf);
        char tempBuf[MAX_LOG_LENGTH + 1] = { 0 };
        WCHAR wszBuf[MAX_LOG_LENGTH + 1] = { 0 };
        
        do
        {
            std::copy(buf + pos, buf + pos + MAX_LOG_LENGTH, tempBuf);
            
            tempBuf[MAX_LOG_LENGTH] = 0;
            
            MultiByteToWideChar(CP_UTF8, 0, tempBuf, -1, wszBuf, sizeof(wszBuf));
            OutputDebugStringW(wszBuf);
            WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, tempBuf, sizeof(tempBuf), nullptr, FALSE);
            printf("%s", tempBuf);
            
            pos += MAX_LOG_LENGTH;
            
        } while (pos < len);
        SendLogToWindow(buf);
        fflush(stdout);
#else
        // Linux, Mac, iOS, etc
        fprintf(stdout, "%s", buf);
        fflush(stdout);
#endif
        
        netConsole->nlog(buf);
        delete [] buf;
    }
    
}   // ::

void NetConsole::log(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    _log(format, args);
    va_end(args);
}

NetConsole* _PrivateInstallNetConsole() {
    static NetConsole staticNetConsole;
    return &staticNetConsole;
}

NetConsole* netConsole = _PrivateInstallNetConsole();

NetConsole::NetConsole()
    : _listenfd(-1)
    , _running(false)
    , _endThread(false)
    , _sendDebugStrings(false)
    , _bindAddress("")
{
    createCommand_GetCode();
    createCommand_RegisterUser();
    createCommand_LoginUser();
    createCommand_ResetPSW();
    createCommand_ChangePSW();
    createCommand_ChangeUserProfileImage();
    createCommand_BindCar();
    createCommand_UnbindCar();
    createCommand_CommentOtherUser();
    createCommand_CommentListForMe();
    createCommand_FinishSnapshotUpload();
    createCommand_GetSystemMsg();
    createCommand_DebugMsg();
    createCommand_Exit();
    createCommand_Help();
    
}

NetConsole::~NetConsole() {
    stop();
}

bool NetConsole::listenOnTCP(int port)
{
    int listenfd = -1, n;
    const int on = 1;
    struct addrinfo hints, *res, *ressave;
    char serv[30];
    
    snprintf(serv, sizeof(serv)-1, "%d", port );
    
    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
#if (NC_TARGET_PLATFORM == NC_PLATFORM_WIN32) || (NC_TARGET_PLATFORM == NC_PLATFORM_WINRT)
    WSADATA wsaData;
    n = WSAStartup(MAKEWORD(2, 2),&wsaData);
#endif
    
    if ( (n = getaddrinfo(nullptr, serv, &hints, &res)) != 0) {
        fprintf(stderr,"net_listen error for %s: %s", serv, gai_strerror(n));
        return false;
    }
    
    ressave = res;
    
    do {
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (listenfd < 0)
            continue;       /* error, try next one */
        
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on));
        
        // bind address
        if (!_bindAddress.empty())
        {
            if (res->ai_family == AF_INET)
            {
                struct sockaddr_in *sin = (struct sockaddr_in*) res->ai_addr;
                inet_pton(res->ai_family, _bindAddress.c_str(), (void*)&sin->sin_addr);
            }
            else if (res->ai_family == AF_INET6)
            {
                struct sockaddr_in6 *sin = (struct sockaddr_in6*) res->ai_addr;
                inet_pton(res->ai_family, _bindAddress.c_str(), (void*)&sin->sin6_addr);
            }
        }
        
        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
            break;          /* success */
        
        /* bind error, close and try next one */
#if (NC_TARGET_PLATFORM == NC_PLATFORM_WIN32) || (NC_TARGET_PLATFORM == NC_PLATFORM_WINRT)
        closesocket(listenfd);
#else
        close(listenfd);
#endif
    } while ( (res = res->ai_next) != nullptr);
    
    if (res == nullptr) {
        perror("net_listen:");
        freeaddrinfo(ressave);
        return false;
    }

    
    listen(listenfd, 50);
    
    if (res->ai_family == AF_INET) {
        char buf[INET_ADDRSTRLEN] = "";
        struct sockaddr_in *sin = (struct sockaddr_in*) res->ai_addr;
        if( inet_ntop(res->ai_family, &sin->sin_addr, buf, sizeof(buf)) != nullptr )
            NetConsole::log("Console: listening on  %s : %d", buf, ntohs(sin->sin_port));
        else
            perror("inet_ntop");
    } else if (res->ai_family == AF_INET6) {
        char buf[INET6_ADDRSTRLEN] = "";
        struct sockaddr_in6 *sin = (struct sockaddr_in6*) res->ai_addr;
        if( inet_ntop(res->ai_family, &sin->sin6_addr, buf, sizeof(buf)) != nullptr )
            NetConsole::log("Console: listening on  %s : %d", buf, ntohs(sin->sin6_port));
        else
            perror("inet_ntop");
    }
    
    
    freeaddrinfo(ressave);
    return listenOnFileDescriptor(listenfd);
}

bool NetConsole::listenOnFileDescriptor(int fd)
{
    if(_running) {
        NetConsole::log("Console already started. 'stop' it before calling 'listen' again");
        return false;
    }
    
    _listenfd = fd;
    _thread = std::thread( std::bind( &NetConsole::loop, this) );
    
    return true;
}

void NetConsole::stop()
{
    if( _running ) {
        _endThread = true;
        if (_thread.joinable())
        {
            _thread.join();
        }
    }
}

void NetConsole::addCommand(const Command& cmd)
{
    _commands[cmd.name] = cmd;
}

void NetConsole::addSubCommand(const std::string& cmdName, const Command& subCmd)
{
    auto it = _commands.find(cmdName);
    if(it != _commands.end()) {
        auto& cmd = it->second;
        addSubCommand(cmd, subCmd);
    }
}

void NetConsole::addSubCommand(Command& cmd, const Command& subCmd)
{
    cmd.subCommands[subCmd.name] = subCmd;
}

const NetConsole::Command* NetConsole::getCommand(const std::string& cmdName)
{
    auto it = _commands.find(cmdName);
    if(it != _commands.end()) {
        auto& cmd = it->second;
        return &cmd;
    }
    return nullptr;
}

const NetConsole::Command* NetConsole::getSubCommand(const std::string& cmdName, const std::string& subCmdName)
{
    auto it = _commands.find(cmdName);
    if(it != _commands.end()) {
        auto& cmd = it->second;
        return getSubCommand(cmd, subCmdName);
    }
    return nullptr;
}

const NetConsole::Command* NetConsole::getSubCommand(const Command& cmd, const std::string& subCmdName)
{
    return cmd.getSubCommand(subCmdName);
}

void NetConsole::Command::addCallback(const Callback& callback_)
{
    callback = callback_;
}

void NetConsole::Command::addSubCommand(const Command& subCmd)
{
    subCommands[subCmd.name] = subCmd;
}

const NetConsole::Command* NetConsole::Command::getSubCommand(const std::string& subCmdName) const
{
    auto it = subCommands.find(subCmdName);
    if(it != subCommands.end()) {
        auto& subCmd = it->second;
        return &subCmd;
    }
    return nullptr;
}

void NetConsole::Command::delSubCommand(const std::string& subCmdName)
{
    auto it = subCommands.find(subCmdName);
    if(it != subCommands.end()) {
        subCommands.erase(it);
    }
}

void NetConsole::Command::commandHelp(int fd, const std::string& args)
{
    if (! help.empty()) {
        ConsoleUtility::mydprintf(fd, "%s\n", help.c_str());
    }
    
    if (! subCommands.empty()) {
        sendHelp(fd, subCommands, "");
    }
}

void NetConsole::Command::commandGeneric(int fd, const std::string& args)
{
    // The first argument (including the empty)
    std::string key(args);
    auto pos = args.find(" ");
    if ((pos != std::string::npos) && (0 < pos)) {
        key = args.substr(0, pos);
    }
    
    // help
    if (key == "help" || key == "-h") {
        commandHelp(fd, args);
        return;
    }
    
    // find sub command
    auto it = subCommands.find(key);
    if (it != subCommands.end()) {
        auto subCmd = it->second;
        if (subCmd.callback) {
            subCmd.callback(fd, args);
        }
        return;
    }
    
    // can not find
    if (callback) {
        callback(fd, args);
    }
}

void NetConsole::delCommand(const std::string& cmdName)
{
    auto it = _commands.find(cmdName);
    if(it != _commands.end()) {
        _commands.erase(it);
    }
}

void NetConsole::delSubCommand(const std::string& cmdName, const std::string& subCmdName)
{
    auto it = _commands.find(cmdName);
    if(it != _commands.end()) {
        auto& cmd = it->second;
        delSubCommand(cmd, subCmdName);
    }
}

void NetConsole::delSubCommand(Command& cmd, const std::string& subCmdName)
{
    cmd.delSubCommand(subCmdName);
}

void NetConsole::nlog(const char* buf)
{
    if( _sendDebugStrings ) {
        _DebugStringsMutex.lock();
        _DebugStrings.push_back(buf);
        _DebugStringsMutex.unlock();
    }
}

void NetConsole::setBindAddress(const std::string &address)
{
    _bindAddress = address;
}

void NetConsole::loop()
{
    fd_set copy_set;
    struct timeval timeout, timeout_copy;
    
    _running = true;
    
    FD_ZERO(&_read_set);
    FD_SET(_listenfd, &_read_set);
    _maxfd = _listenfd;
    
    timeout.tv_sec = 0;
    
    /* 0.016 seconds. Wake up once per frame at 60PFS */
    timeout.tv_usec = 16000;
    
    while(!_endThread) {
        
        copy_set = _read_set;
        timeout_copy = timeout;
        
        int nready = select(_maxfd+1, &copy_set, nullptr, nullptr, &timeout_copy);
        
        if( nready == -1 )
        {
            /* error */
            if(errno != EINTR)
                NetConsole::log("Abnormal error in select()\n");
            continue;
        }
        else if( nready == 0 )
        {
            /* timeout. do something ? */
        }
        else
        {
            /* new client */
            if(FD_ISSET(_listenfd, &copy_set)) {
                addClient();
                if(--nready <= 0)
                    continue;
            }
            
            /* data from client */
            std::vector<int> to_remove;
            for(const auto &fd: _fds) {
                if(FD_ISSET(fd,&copy_set))
                {
                    //fix Bug #4302 Test case ConsoleTest--ConsoleUploadFile crashed on Linux
                    //On linux, if you send data to a closed socket, the sending process will
                    //receive a SIGPIPE, which will cause linux system shutdown the sending process.
                    //Add this ioctl code to check if the socket has been closed by peer.
                    
#if (NC_TARGET_PLATFORM == NC_PLATFORM_WIN32) || (NC_TARGET_PLATFORM == NC_PLATFORM_WINRT)
                    u_long n = 0;
                    ioctlsocket(fd, FIONREAD, &n);
#else
                    int n = 0;
                    ioctl(fd, FIONREAD, &n);
#endif
                    if(n == 0)
                    {
                        //no data received, or fd is closed
                        continue;
                    }
                    
                    if( ! parseCommand(fd) )
                    {
                        to_remove.push_back(fd);
                    }
                    if(--nready <= 0)
                        break;
                }
            }
            
            /* remove closed connections */
            for(int fd: to_remove) {
                FD_CLR(fd, &_read_set);
                _fds.erase(std::remove(_fds.begin(), _fds.end(), fd), _fds.end());
            }
        }
        
        /* Any message for the remote console ? send it! */
        if( !_DebugStrings.empty() ) {
            if (_DebugStringsMutex.try_lock())
            {
                for (const auto &str : _DebugStrings) {
                    for (auto fd : _fds) {
                        ConsoleUtility::sendToConsole(fd, str.c_str(), str.length());
                    }
                }
                _DebugStrings.clear();
                _DebugStringsMutex.unlock();
            }
        }
    }
    
    // clean up: ignore stdin, stdout and stderr
    for(const auto &fd: _fds )
    {
#if (NC_TARGET_PLATFORM == NC_PLATFORM_WIN32) || (NC_TARGET_PLATFORM == NC_PLATFORM_WINRT)
        closesocket(fd);
#else
        close(fd);
#endif
    }
    
#if (NC_TARGET_PLATFORM == NC_PLATFORM_WIN32) || (NC_TARGET_PLATFORM == NC_PLATFORM_WINRT)
    closesocket(_listenfd);
    WSACleanup();
#else
    close(_listenfd);
#endif
    _running = false;
}

ssize_t NetConsole::readline(int fd, char* ptr, size_t maxlen)
{
    size_t n, rc;
    char c;
    
    for( n = 0; n < maxlen - 1; n++ ) {
        if( (rc = recv(fd, &c, 1, 0)) ==1 ) {
            *ptr++ = c;
            if(c == '\n') {
                break;
            }
        } else if( rc == 0 ) {
            return 0;
        } else if( errno == EINTR ) {
            continue;
        } else {
            return -1;
        }
    }
    
    *ptr = 0;
    return n;
}

ssize_t NetConsole::readBytes(int fd, char* buffer, size_t maxlen, bool* more)
{
    size_t n, rc;
    char c, *ptr = buffer;
    *more = false;
    for( n = 0; n < maxlen; n++ ) {
        if( (rc = recv(fd, &c, 1, 0)) ==1 ) {
            *ptr++ = c;
            if(c == '\n') {
                return n;
            }
        } else if( rc == 0 ) {
            return 0;
        } else if( errno == EINTR ) {
            continue;
        } else {
            return -1;
        }
    }
    *more = true;
    return n;
}

bool NetConsole::parseCommand(int fd)
{
    char buf[512];
    bool more_data;
    auto h = readBytes(fd, buf, 6, &more_data);
    if( h < 0)
    {
        return false;
    }
    if(!more_data)
    {
        buf[h] = 0;
    }
    else
    {
        char *pb = buf + 6;
        auto r = readline(fd, pb, sizeof(buf)-6);
        if(r < 0)
        {
            const char err[] = "Unknown error!\n";
            ConsoleUtility::sendPrompt(fd);
            ConsoleUtility::sendToConsole(fd, err, strlen(err));
            return false;
        }
    }
    std::string cmdLine;
    
    std::vector<std::string> args;
    cmdLine = std::string(buf);
    
    args = ConsoleUtility::split(cmdLine, ' ');
    if(args.empty())
    {
        const char err[] = "Unknown command. Type 'help' for options\n";
        ConsoleUtility::sendToConsole(fd, err, strlen(err));
        ConsoleUtility::sendPrompt(fd);
        return true;
    }
    
    auto it = _commands.find(ConsoleUtility::trim(args[0]));
    if(it != _commands.end())
    {
        std::string args2;
        for(size_t i = 1; i < args.size(); ++i)
        {
            if(i > 1)
            {
                args2 += ' ';
            }
            args2 += ConsoleUtility::trim(args[i]);
            
        }
        auto cmd = it->second;
        cmd.commandGeneric(fd, args2);
    }else if(strcmp(buf, "\r\n") != 0) {
        const char err[] = "Unknown command. Type 'help' for options\n";
        ConsoleUtility::sendToConsole(fd, err, strlen(err));
    }
    ConsoleUtility::sendPrompt(fd);
    
    return true;
}

void NetConsole::addClient()
{
    struct sockaddr client;
    socklen_t client_len;
    
    /* new client */
    client_len = sizeof( client );
    int fd = accept(_listenfd, (struct sockaddr *)&client, &client_len );
    
    // add fd to list of FD
    if( fd != -1 ) {
        FD_SET(fd, &_read_set);
        _fds.push_back(fd);
        _maxfd = std::max(_maxfd,fd);
        
        ConsoleUtility::sendPrompt(fd);
        
        /**
         * A SIGPIPE is sent to a process if it tried to write to socket that had been shutdown for
         * writing or isn't connected (anymore) on iOS.
         *
         * The default behaviour for this signal is to end the process.So we make the process ignore SIGPIPE.
         */
#if NC_TARGET_PLATFORM == NC_PLATFORM_IOS
        int set = 1;
        setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, (void*)&set, sizeof(int));
#endif
    }
}
