#ifndef ACE_LOGGER_HEADER
#define ACE_LOGGER_HEADER

#include<chrono>
#include<string>
#include<iostream>
#include<fstream>
#include<mutex>
#include<exception>

enum class LogLevel : std::uint8_t  { DEBUG, INFO, WARN, ERROR, FATAL };

class Logger
{
    public:
        Logger(LogLevel priority_level = LogLevel::INFO,std::string log_file_path = "",bool console_output = true);

        void Fatal(const std::string& message);
        void Error(const std::string& message);
        void Warn (const std::string& message);
        void Info (const std::string& message);
        void Debug(const std::string& message);

    private:
        bool save_to_file;
        bool console_output;
        std::mutex logger_mutex; 
        LogLevel priority_level;
        std::string log_file_path;

        void log(LogLevel log_level,const std::string& message);
};

#ifdef ACE_LOGGER_IMPLEMENTATION

Logger::Logger(LogLevel priority_level,std::string log_file_path,bool console_output)
{
    if (log_file_path.empty())
    {
        if(!console_output)
        {
            throw std::invalid_argument("Console output disabled and Log file path not specified");
        }
        
        this->save_to_file  = false;
    }
    else
    {
        this->log_file_path = log_file_path;
        this->save_to_file  = true;
    }

    this->console_output = console_output;
    this->priority_level = priority_level;
}

void Logger::log(LogLevel log_level,const std::string& message)
{
    if (log_level < this->priority_level)
    {
        return;
    }

    std::string out_text;
    
    logger_mutex.lock();
    using clock = std::chrono::system_clock;

    auto now          = clock::now();
    auto seconds      = std::chrono::time_point_cast<std::chrono::seconds>(now);
    auto fraction     = now - seconds;
    std::time_t cnow  = clock::to_time_t(now);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);

    char time_str[20];
    std::strftime(time_str, sizeof(time_str), "%H:%M:%S:", std::localtime(&cnow));
    std::snprintf(time_str+9,5,"%03ld",milliseconds.count());
    
    out_text += time_str;

    switch(log_level)
    {
        case LogLevel::FATAL : out_text+=" [FATAL] ";break;
        case LogLevel::ERROR : out_text+=" [ERROR] ";break;
        case LogLevel::WARN  : out_text+=" [WARN ] ";break;
        case LogLevel::INFO  : out_text+=" [INFO ] ";break;
        case LogLevel::DEBUG : out_text+=" [DEBUG] ";break;  
    }

    out_text += message;

    if (this->console_output)
    {
        std::cout << out_text << "\n";
    }

    if (this->save_to_file)
    {
        std::ofstream file(log_file_path, std::ios_base::app);
        file << out_text << "\n";
        file.close();
    }
    logger_mutex.unlock();
}

void Logger::Fatal(const std::string& message)
{
    log(LogLevel::FATAL,message);
}

void Logger::Error(const std::string& message)
{
    log(LogLevel::ERROR,message);
}

void Logger::Warn(const std::string& message)
{
    log(LogLevel::WARN,message);
}

void Logger::Info(const std::string& message)
{
    log(LogLevel::INFO,message);
}

void Logger::Debug(const std::string& message)
{
    log(LogLevel::DEBUG,message);
}

#endif

#endif