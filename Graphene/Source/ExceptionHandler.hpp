#pragma once
#include <string>
#include <queue>
#include <iostream>
#include <windows.h>

class ExceptionHandler {
public:
	struct Exception {
		enum class Severity { DBG, OK, WARN, ERR };

	private:
		Severity m_severity;
		std::string m_content;

	public:
		Exception() = default;

		Exception(Severity severity, const std::string& content) {
			m_severity = severity;
			m_content = content;
		}

		Severity getSeverity() {
			return m_severity;
		}

		std::string getContent() {
			return m_content;
		}
	};

private:
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

public:
	static std::queue<Exception*>& getExceptionQueue() {
		static std::queue<Exception*> exceptionQueue;
		return exceptionQueue;
	}

	static void insertException(Exception::Severity severity, std::string& content) {
		getExceptionQueue().push(new Exception(severity, content));
	}

	static void err(const std::string& content) {
		getExceptionQueue().push(new Exception(Exception::Severity::ERR, content));
	}

	static void warn(const std::string& content) {
		getExceptionQueue().push(new Exception(Exception::Severity::WARN, content));
	}

	static void ok(const std::string& content) {
		getExceptionQueue().push(new Exception(Exception::Severity::OK, content));
	}

	static void dbg(const std::string& content) {
		getExceptionQueue().push(new Exception(Exception::Severity::DBG, content));
	}

	static bool pollException(Exception& exception) {
		if (getExceptionQueue().empty()) return false;
		else {
			exception = *getExceptionQueue().front();
			delete getExceptionQueue().front();
			getExceptionQueue().pop();
			return true;
		}
	}

	void flushExceptionsToIOStream() {
		// Display exceptions to the terminal
		ExceptionHandler::Exception exception;
		while (pollException(exception)) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << "[";
			switch (exception.getSeverity()) {
			case Exception::Severity::DBG:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				std::cout << " DBG";
				break;
			case Exception::Severity::OK:
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				std::cout << "  OK";
				break;
			case Exception::Severity::WARN:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				std::cout << "WARN";
				break;
			case Exception::Severity::ERR:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
				std::cout << " ERR";
				break;
			}
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << "]";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			std::cout << " | ";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << exception.getContent() << "\n";
		}
	}

};