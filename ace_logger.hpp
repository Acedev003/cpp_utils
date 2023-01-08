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
    if (log_level >= this->priority_level)
    {
        logger_mutex.lock();
        bool time_format_avail = false;
        typedef std::chrono::system_clock clock;

        auto now          = clock::now();
        auto seconds      = std::chrono::time_point_cast<std::chrono::seconds>(now);
        auto fraction     = now - seconds;
        std::time_t cnow  = clock::to_time_t(now);
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);

        char time_str[100];
        if (std::strftime(time_str, sizeof(time_str), "%H:%M:%S:", std::localtime(&cnow)))
        {
            time_format_avail = true;
        }

        if (this->console_output)
        {
            if (time_format_avail)
            {
                std::cout << time_str << milliseconds.count() << " ";
            }
            std::cout << message << std::endl;
        }

        if (this->save_to_file)
        {
            std::ofstream file(log_file_path, std::ios_base::app);
            if (time_format_avail)
            {
                file << time_str << milliseconds.count() << " ";
            }
            file << message << std::endl;
            file.close();
        }
        logger_mutex.unlock();
    }
}

void Logger::Fatal(const std::string& message)
{
    log(LogLevel::FATAL,"[FATAL]\t"+message);
}

void Logger::Error(const std::string& message)
{
    log(LogLevel::ERROR,"[ERROR]\t"+message);
}

void Logger::Warn(const std::string& message)
{
    log(LogLevel::WARN,"[WARN] \t"+message);
}

void Logger::Info(const std::string& message)
{
    log(LogLevel::INFO,"[INFO] \t"+message);
}

void Logger::Debug(const std::string& message)
{
    log(LogLevel::DEBUG,"[DEBUG]\t"+message);
}

#endif

#endif