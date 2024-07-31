#ifndef LOGGER_CLASS_H
#define LOGGER_CLASS_H
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
class Log
{
public:
    std::ofstream logfile;
   
    void openLogFile(std::string name)
    {
        logfile.open(name, std::ios::out | std::ios::app);
        if (!logfile.is_open())
        {
            std::cerr << "Failed To Open LogFile" << "\n";
        }
    }

    void logMessage(const std::string& time, const std::string& value)
    {
       
        //  openLogFile();
        if (logfile.is_open())
        {
            logfile << time << "\t" << value << "\n";
        }
        else
        {

            std::cerr << "Failed To log error in LogFile" << "\n";
        }
    }
    void logvalues(const std::string& x, const std::string& y, const std::string& z)
    {

        //  openLogFile();
        if (logfile.is_open())
        {
            logfile << x << "\t" << y << "\t" << z << "\n";
        }
        else
        {
            std::cerr << "Failed To log error in LogFile" << "\n";
        }
    }
    void closeLogFile()
    {
        logfile.close();
    }
};
#endif
