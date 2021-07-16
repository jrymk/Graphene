#pragma once

#include <vector>
#include <string>
#include <deque>

#define logInsert(x) gfn::logging::messageBuffer = gfn::logging::messageBuffer + x
#define logError(x) gfn::logging::addLogEntry(x, __FILE__, __LINE__, gfn::logging::LogItem::Severity::ERR)
#define logWarning(x) gfn::logging::addLogEntry(x, __FILE__, __LINE__, gfn::logging::LogItem::Severity::WARNING)
#define logInfo(x) gfn::logging::addLogEntry(x, __FILE__, __LINE__, gfn::logging::LogItem::Severity::INFO)
#define logDebug(x) gfn::logging::addLogEntry(x, __FILE__, __LINE__, gfn::logging::LogItem::Severity::DEBUG)
#define logVerbose(x) gfn::logging::addLogEntry(x, __FILE__, __LINE__, gfn::logging::LogItem::Severity::VERBOSE)

namespace gfn::logging {
int nIndex = 0;
std::string messageBuffer = "";

struct LogItem {
	int index;
	std::string message;
	std::string file;
	int line;

	enum class Severity { ERR, WARNING, INFO, DEBUG, VERBOSE };
	Severity severity;

	LogItem(std::string _message, std::string _file, int _line, LogItem::Severity _severity)
		: index(nIndex++), message(messageBuffer + _message), file(_file), line(_line), severity(_severity) {}
};

std::deque<LogItem> logBuffer;

void addLogEntry(std::string message, std::string file, int line, LogItem::Severity severity) {
	logBuffer.push_back(LogItem(message, file, line, severity));
	if (logBuffer.size() == 10001)
		logBuffer.pop_front();
}

} // namespace gfn::logging