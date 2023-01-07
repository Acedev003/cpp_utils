#ifndef ACE_LOGGER_HEADER
#define ACE_LOGGER_HEADER

#include<chrono>
#include<string>
#include<iostream>
#include<fstream>
#include<mutex>

enum class LogLevel : std::uint8_t  { DEBUG, INFO, WARN, ERROR, FATAL };

class Logger
{
    public:
        static void init(LogLevel priority_level = LogLevel::INFO,bool save_to_file = false,bool console_output = true,std::string log_file_path = "");

        static void Fatal(const std::string& message);
        static void Error(const std::string& message);
        static void Warn (const std::string& message);
        static void Info (const std::string& message);
        static void Debug(const std::string& message);

    private:
        static bool initialized;
        static bool console_output;
        static bool save_to_file;
        static LogLevel    priority_level;
        static std::string log_file_path;
        static std::mutex  logger_mutex; 

        Logger(){}
        
        static Logger get_instance();
        static void log(LogLevel log_level,const std::string& message);
};

#ifdef ACE_LOGGER_IMPLEMENTATION

bool Logger::initialized     = false;
bool Logger::console_output  = true;
bool Logger::save_to_file    = false;
LogLevel    Logger::priority_level = LogLevel::INFO;
std::string Logger::log_file_path  = "LOG.txt";
std::mutex  Logger::logger_mutex;

Logger Logger::get_instance()
{
    static Logger instance;
    return instance;
}

void Logger::init(LogLevel priority_level,bool save_to_file,bool console_output,std::string log_file_path)
{
    if(!initialized)
    {
        Logger this_logger = get_instance();
        if (console_output == false && save_to_file == false)
        {
            // Both console and file outputs disabled. Exiting logger;
            return;
        }

        if (save_to_file)
        {
            // Logging to file enabled
            if (log_file_path != "")
            {
                this_logger.log_file_path = log_file_path;
            }
            this_logger.save_to_file = true;
        }

        this_logger.console_output = console_output;
        this_logger.priority_level = priority_level;
        this_logger.initialized = true;
    }
}

void Logger::log(LogLevel log_level,const std::string& message)
{
    Logger this_logger = get_instance();

    if (log_level >= this_logger.priority_level && this_logger.initialized)
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

        if (this_logger.console_output)
        {
            if (time_format_avail)
            {
                std::cout << time_str << milliseconds.count() << " ";
            }
            std::cout << message << std::endl;
        }

        if (this_logger.save_to_file)
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