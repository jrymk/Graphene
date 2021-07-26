#pragma once

#include <System/Timer/Timer.hpp>
#include <deque>
#include <string>
#include <sstream>
#include <mutex>

#undef ERROR

#define logMessage gfn::core::logging::messageStream
#define logError                                                                                                       \
	gfn::core::logging::logBuffer->error(__FILE__, __LINE__);                                                          \
	gfn::core::logging::messageStream.str("")
#define logWarning                                                                                                     \
	gfn::core::logging::logBuffer->warning(__FILE__, __LINE__);                                                        \
	gfn::core::logging::messageStream.str("")
#define logInfo                                                                                                        \
	gfn::core::logging::logBuffer->info(__FILE__, __LINE__);                                                           \
	gfn::core::logging::messageStream.str("")
#define logDebug                                                                                                       \
	gfn::core::logging::logBuffer->debug(__FILE__, __LINE__);                                                          \
	gfn::core::logging::messageStream.str("")
#define logVerbose                                                                                                     \
	gfn::core::logging::logBuffer->verbose(__FILE__, __LINE__);                                                        \
	gfn::core::logging::messageStream.str("")

namespace gfn::core::logging {
struct LogEntry {
	enum class Severity { ERROR, WARNING, INFO, DEBUG, VERBOSE };
	std::string timeStamp = "TIMESTAMP";
	std::string message = "MESSAGE";
	std::string file = "FILE";
	int line = -1;
	Severity severity = Severity::ERROR;

	LogEntry(std::string _timeStamp, std::string _message, std::string _file, int _line, LogEntry::Severity _severity)
		: timeStamp(_timeStamp), message(_message), file(_file), line(_line), severity(_severity) {}
};

std::stringstream messageStream;

class LogBuffer {
  public:
	std::deque<LogEntry*> buffer;

	void insert(std::string message, std::string file, int line, LogEntry::Severity severity) {
		/*if (buffer.size() == 1000000)
			buffer.pop_front();*/
		auto entry = new LogEntry(gfn::timer::getNowStamp(), message, file, line, severity);
		buffer.push_back(entry);
	}

	void error(std::string file, int line) {
		insert(messageStream.str(), file, line, LogEntry::Severity::ERROR);
		std::cerr << messageStream.str() << "\n";
	}

	void warning(std::string file, int line) {
		insert(messageStream.str(), file, line, LogEntry::Severity::WARNING);
		std::cerr << messageStream.str() << "\n";
	}

	void info(std::string file, int line) {
		insert(messageStream.str(), file, line, LogEntry::Severity::INFO);
		std::cerr << messageStream.str() << "\n";
	}

	void debug(std::string file, int line) {
		insert(messageStream.str(), file, line, LogEntry::Severity::DEBUG);
		std::cerr << messageStream.str() << "\n";
	}

	void verbose(std::string file, int line) {
		insert(messageStream.str(), file, line, LogEntry::Severity::VERBOSE);
		std::cerr << messageStream.str() << "\n";
	}
};

LogBuffer* logBuffer;
} // namespace gfn::core::logging