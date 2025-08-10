#pragma once
#include <syslog.h>
#include <string>
#include <iostream>
#include <memory>

/**
 * This class is a Singleton that implements the logging for this system.
 * Today it is using syslog for logging but it can make it possible to add
 * different types, as stdout, stderr and even ELK.
 * 
 * In a first moment it was only a static class, but every time the 'log'
 * method was called it was necessary to call 'openlog', the 'closelog' along
 * with the 'syslog' function. Making it as a Singleton make the open and close 
 * calls only once. Also, making it a Singleton improve the architecture for a
 * future log messaging to ELK.
 */
class UCDLogger
{
private:
    UCDLogger(/* args */) {
        globalSeverity = LOG_INFO;
        openlog("Unnecessarily Complex Dictionary: ",LOG_ODELAY|LOG_PID,LOG_USER);
    };

    ~UCDLogger() {
        closelog();
    };

    int globalSeverity;
    inline static UCDLogger* pInstance = nullptr;

public:

    static UCDLogger* getInstance() {
        if ( pInstance == nullptr ) {
            pInstance = new UCDLogger();
        }
        return pInstance;
    }

    /**
     * Logs an error message
     */
    void log(int severity, const std::string& msg)
    {
        if (severity > globalSeverity)
        {
            return;
        }
        syslog(severity, "%s", msg.c_str());
    }

    /**
     * increases the verbosity on which logs 
     * are being printed.
     */
    void updateGlobalSeverity(int newSeverity)
    {
        globalSeverity = newSeverity;
    }
};
