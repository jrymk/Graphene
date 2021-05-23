#pragma once

#include <queue>
#include <vector>

#define LOG_FATAL(x) Utils::Log::addLogEntry(x, __FILE__, __LINE__, Utils::Log::LogItem::Severity::FATAL)
#define LOG_ERROR(x) Utils::Log::addLogEntry(x, __FILE__, __LINE__, Utils::Log::LogItem::Severity::ERR)
#define LOG_WARNING(x) Utils::Log::addLogEntry(x, __FILE__, __LINE__, Utils::Log::LogItem::Severity::WARNING)
#define LOG_OK(x) Utils::Log::addLogEntry(x, __FILE__, __LINE__, Utils::Log::LogItem::Severity::OK)
#define LOG_LOG(x) Utils::Log::addLogEntry(x, __FILE__, __LINE__, Utils::Log::LogItem::Severity::LOG)

namespace Utils {
    namespace Log {

        struct LogItem {
            std::string message;
            std::string file;
            int line;
            enum class Severity {
                FATAL,
                ERR,
                WARNING,
                OK,
                LOG
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