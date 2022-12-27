#pragma once

#include<string>
#include<iostream>

//enum LogLevels  { FATAL, ERROR, WARN, INFO, DEBUG };

class Logger
{
    public:
        static void init(bool console_output = true,bool save_to_file = false,std::string log_file_path = "")
        {
            if(console_output == false && save_to_file == false)
            {
                std::cerr<<"Both file and console outputs are disabled. Failed to initialize logger\n";
                return;
            }

            if(save_to_file)
            {
                if(log_file_path != "")
                {
                    get_instance().log_file_path = log_file_path;
                }
            }
            else
            {
                //TODO
            }
            
            get_instance().initialized = true;
            std::cout<<get_instance().log_file_path;
            
        }

        static void Fatal()
        {
            
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
        static std::string log_file_path;

        Logger(){}

        static Logger get_instance()
        {
            static Logger instance;
            return instance;
        }

};

bool Logger::initialized  = false;
std::string Logger::log_file_path = "LOG.txt";