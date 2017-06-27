#ifndef ConsoleUtility_h
#define ConsoleUtility_h


class ConsoleUtility {
public:
    // Trimming functions
    static std::string& ltrim(std::string& s);
    static std::string& rtrim(std::string& s);
    static std::string& trim(std::string& s);
    
    // split
    static std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems);
    static std::vector<std::string> split(const std::string& s, char delim);
    
    /** Checks myString is a floating-point type. */
    static bool isFloat(const std::string& myString);
    
    /** send a message to console */
    static ssize_t sendToConsole(int fd, const void* buffer, size_t length, int flags = 0);
    
    /** my dprintf() */
    static ssize_t mydprintf(int sock, const char *format, ...);
    
    /** send prompt string to console */
    static void sendPrompt(int fd);
    
    /** set a new string for the prompt. */
    static void setPrompt(const std::string &prompt);
    
    /** get the prompt string. */
    static const std::string& getPrompt();
    
private:
    static std::string _prompt;  /*!< prompt */
    static std::mutex _consoleOutputMutex;

};

#endif /* ConsoleUtility_h */
