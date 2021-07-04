#pragma once

#include <string>
#include <deque>
#include <vector>

#define LOG_ERROR(x)                                                                                                   \
	graphene::tools::log::addLogEntry(x, __FILE__, __LINE__, graphene::tools::log::LogEntry::Severity::ERR)
#define LOG_WARNING(x)                                                                                                 \
	graphene::tools::log::addLogEntry(x, __FILE__, __LINE__, graphene::tools::log::LogEntry::Severity::WARNING)
#define LOG_INFO(x)                                                                                                    \
	graphene::tools::log::addLogEntry(x, __FILE__, __LINE__, graphene::tools::log::LogEntry::Severity::INFO)
#define LOG_DEBUG(x)                                                                                                   \
	graphene::tools::log::addLogEntry(x, __FILE__, __LINE__, graphene::tools::log::LogEntry::Severity::DEBUG)
#define LOG_VERBOSE(x)                                                                                                 \
	graphene::tools::log::addLogEntry(x, __FILE__, __LINE__, graphene::tools::log::LogEntry::Severity::VERBOSE)

namespace graphene::tools::log {
int nIndex = 0;

struct LogEntry {
	int index;
	std::string message;
	std::string file;
	int line;

	enum class Severity { ERR, WARNING, INFO, DEBUG, VERBOSE };
	Severity severity;

	LogEntry(std::string _message, std::string _file, int _line, LogEntry::Severity _severity)
		: index(nIndex++), message(_message), file(_file), line(_line), severity(_severity) {}
};

std::deque<LogEntry> logBuffer;

void addLogEntry(std::string message, std::string file, int line, LogEntry::Severity severity) {
	logBuffer.push_back(LogEntry(message, file, line, severity));
	if (logBuffer.size() == 10001)
		logBuffer.pop_front();
}

} // namespace graphene::tools::log