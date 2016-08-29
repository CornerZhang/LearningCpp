//
//  ConsoleUtility.cpp
//  NetConsole
//
//  Created by Cyberzei on 8/24/16.
//  Copyright © 2016 Cyberzei. All rights reserved.
//

#include "ConsoleUtility.h"
#include <functional>
#include <sstream>


static constexpr const char* PROMPT = "> ";

namespace {
    static constexpr size_t SEND_BUFSIZ = 512;
}

std::string ConsoleUtility::_prompt(PROMPT);
std::mutex ConsoleUtility::_consoleOutputMutex;

//TODO: these general utils should be in a separate class
//
// Trimming functions were taken from: http://stackoverflow.com/a/217605
//
// trim from start

std::string& ConsoleUtility::ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string& ConsoleUtility::rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
std::string& ConsoleUtility::trim(std::string& s) {
    return ConsoleUtility::ltrim(ConsoleUtility::rtrim(s));
}

std::vector<std::string>& ConsoleUtility::split(const std::string& s, char delim, std::vector<std::string>& elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> ConsoleUtility::split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    ConsoleUtility::split(s, delim, elems);
    return elems;
}

//isFloat taken from http://stackoverflow.com/questions/447206/c-isfloat-function
bool ConsoleUtility::isFloat(const std::string& myString) {
    std::istringstream iss(myString);
    float f;
    iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}

ssize_t ConsoleUtility::sendToConsole(int fd, const void* buffer, size_t length, int flags)
{
    _consoleOutputMutex.lock();
    
    if (_prompt.length() == length) {
        if (strncmp(_prompt.c_str(), static_cast<const char*>(buffer), length) == 0) {
            fprintf(stderr,"bad parameter error: a buffer is the prompt string.\n");
            return 0;
        }
    }
    
    const char* buf = static_cast<const char*>(buffer);
    ssize_t retLen = 0;
    for (size_t i = 0; i < length; ) {
        size_t len = length - i;
        if (SEND_BUFSIZ < len) len = SEND_BUFSIZ;
        retLen += send(fd, buf + i, len, flags);
        i += len;
    }
    
    _consoleOutputMutex.unlock();
    
    return retLen;
}

// dprintf() is not defined in Android
// so we add our own 'dpritnf'
ssize_t ConsoleUtility::mydprintf(int sock, const char *format, ...)
{
    va_list args;
    char buf[16386];
    
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    return sendToConsole(sock, buf, strlen(buf));
}

void ConsoleUtility::sendPrompt(int fd)
{
    const char* prompt = _prompt.c_str();
    send(fd, prompt, strlen(prompt), 0);
}

void ConsoleUtility::setPrompt(const std::string &prompt)
{
    _prompt = prompt;
}

const std::string& ConsoleUtility::getPrompt()
{
    return _prompt;
}

