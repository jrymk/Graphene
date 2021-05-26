#pragma once

#include <vector>

#define LOG_ERROR(x) Utils::Log::addLogEntry(x, __FILE__, __LINE__, Utils::Log::LogItem::Severity::ERR)
#define LOG_WARNING(x) Utils::Log::addLogEntry(x, __FILE__, __LINE__, Utils::Log::LogItem::Severity::WARNING)
#define LOG_INFO(x) Utils::Log::addLogEntry(x, __FILE__, __LINE__, Utils::Log::LogItem::Severity::INFO)
#define LOG_DEBUG(x) Utils::Log::addLogEntry(x, __FILE__, __LINE__, Utils::Log::LogItem::Severity::DEBUG)
#define LOG_VERBOSE(x) Utils::Log::addLogEntry(x, __FILE__, __LINE__, Utils::Log::LogItem::Severity::VERBOSE)

namespace Utils {
    namespace Log {

        struct LogItem {
            std::string message;
            std::string file;
            int line;

            enum class Severity {
                ERR,
                WARNING,
                INFO,
                DEBUG,
                VERBOSE
            };
            Severity severity;

            LogItem(std::string _message, std::string _file, int _line, LogItem::Severity _severity) :
                    message(_message),
                    file(_file),
                    line(_line),
                    severity(_severity) {

            }
        };

        std::vector<LogItem> logBuffer;

        void addLogEntry(std::string message, std::string file, int line, LogItem::Severity severity) {
            logBuffer.push_back(
                    LogItem(message, file, line, severity)
            );
        }

    }
}