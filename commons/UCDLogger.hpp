#pragma once
#include <string>
#include <iostream>
#include <memory>

#include <syslog.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <libgen.h>


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
    char appName[50];

    UCDLogger(/* args */) {
        // Get binary name
        char buf[200];
        ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf)-1);
        if (len != -1) {
            buf[len] = '\0';
            sprintf(appName, "%s", basename(buf));
        }

        openlog(appName,LOG_ODELAY|LOG_PID,LOG_USER);
    };

    ~UCDLogger() {
        closelog();
    };

    inline static UCDLogger* pInstance = nullptr;

    int globalSeverity = LOG_INFO;
    bool stdoutPrint = false;

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

        if (stdoutPrint)
        {
            std::cout << std::string(appName) << " : " << msg << std::endl;
        }
    }

    /**
     * increases the verbosity on which logs 
     * are being printed.
     */
    void updateOutputVerbosity(int newSeverity)
    {
        globalSeverity = newSeverity;
    }

    void enableStdoutPrint(bool doPrint)
    {
        stdoutPrint = doPrint;
    }
};
