#pragma once

#include<chrono>
#include<string>
#include<iostream>

enum LogLevel  { FATAL = 4, ERROR = 3, WARN = 2, INFO = 1, DEBUG = 0 };

class Logger
{
    public:
        static void init(LogLevel priority_level = INFO,bool save_to_file = false,bool console_output = true,std::string log_file_path = "")
        {
            if(console_output == false && save_to_file == false)
            {
                //Both console and file outputs disabled. Exiting logger;
                return;
            }

            if(save_to_file)
            {
                // Logging to file enabled
                if(log_file_path != "")
                {
                    get_instance().log_file_path = log_file_path;
                }
            }
            
            if(console_output)
            {
                //Logging to console enabled
            }

            
            get_instance().priority_level = priority_level;
            get_instance().initialized    = true;
        }

        template<typename ... Args>
        static void Fatal(std::string message,Args ... args)
        {
            log(LogLevel::FATAL,message+"\n",args...);
        }

        static void Error()
        {
               
        }

        static void Warn()
        {
            
        }

        static void Info()
        {
            
        }

        static void Debug()
        {
            
        }

    private:
        static bool initialized;
        static bool console_output;
        static bool save_to_file;
        static LogLevel priority_level;
        static std::string log_file_path;

        Logger(){}

        static Logger get_instance()
        {
            static Logger instance;
            return instance;
        }
        
        template<typename... Args>
        static void log(LogLevel log_level,std::string message,Args ... args)
        {
            if(log_level >= get_instance().priority_level && get_instance().initialized)
            {
                if(get_instance().console_output)
                {
                    printf(message.c_str(),args...);   
                }

                if(get_instance().save_to_file)
                {

                }
            }
        }

};

bool Logger::initialized     = false;
bool Logger::console_output  = true;
bool Logger::save_to_file    = false;
LogLevel    Logger::priority_level = LogLevel::INFO;
std::string Logger::log_file_path  = "LOG.txt";