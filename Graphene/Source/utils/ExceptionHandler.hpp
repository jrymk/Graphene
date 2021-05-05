#pragma once
#include <string>
#include <queue>
#include <iostream>
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define GL(x) ExceptionHandler::gl(x, __FILE__, __LINE__)
#define ERR(x) ExceptionHandler::err(x, __FILE__, __LINE__)
#define WARN(x) ExceptionHandler::warn(x, __FILE__, __LINE__)
#define OK(x) ExceptionHandler::ok(x, __FILE__, __LINE__)
#define DBG(x) ExceptionHandler::dbg(x, __FILE__, __LINE__)


static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static std::string fileName(std::string fullPath) {
	std::string temp = "";
	for (unsigned long long i = 5; i < fullPath.size(); i++) {
		temp.push_back(fullPath[i]);
		if (fullPath[i - 5] == 'S' && fullPath[i - 4] == 'o' && fullPath[i - 3] == 'u' &&
			fullPath[i - 2] == 'r' && fullPath[i - 1] == 'c' && fullPath[i] == 'e')
			temp = "";
	}
	return temp;
}


class ExceptionHandler {
public:
	struct Exception {
		enum class Severity { DBG, OK, WARN, ERR, GL };

	private:
		Severity m_severity;
		std::string m_content;
		std::string m_file;
		int m_line;

	public:
		Exception() = default;


		Exception(Severity severity, const std::string& content, const std::string& file, int line) {
			m_severity = severity;
			m_content = content;
			m_file = fileName(file);
			m_line = line;
		}

		Severity getSeverity() {
			return m_severity;
		}

		std::string getContent() {
			return m_content;
		}

		std::string getFile() {
			return m_file;
		}

		int getLine() {
			return m_line;
		}
	};

private:
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

public:

	static std::queue<Exception*>& getExceptionQueue() {
		static std::queue<Exception*> exceptionQueue;
		return exceptionQueue;
	}

	static void insertException(Exception::Severity severity, std::string& content, const std::string& file, int line) {
		getExceptionQueue().push(new Exception(severity, content, file, line));
	}

	static void gl(const std::string& content, const std::string& file, int line) {
		getExceptionQueue().push(new Exception(Exception::Severity::GL, content, file, line));
	}

	static void err(const std::string& content, const std::string& file, int line) {
		getExceptionQueue().push(new Exception(Exception::Severity::ERR, content, file, line));
	}

	static void warn(const std::string& content, const std::string& file, int line) {
		getExceptionQueue().push(new Exception(Exception::Severity::WARN, content, file, line));
	}

	static void ok(const std::string& content, const std::string& file, int line) {
		getExceptionQueue().push(new Exception(Exception::Severity::OK, content, file, line));
	}

	static void dbg(const std::string& content, const std::string& file, int line) {
		getExceptionQueue().push(new Exception(Exception::Severity::DBG, content, file, line));
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
			case Exception::Severity::GL:
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				std::cout << "  GL";
				break;
			}
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << "] ";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << exception.getContent();
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			std::cout << " [" << exception.getFile() << ":" << exception.getLine() << "]\n";

		}
	}

};

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		ExceptionHandler::gl(function + std::string(": ") + std::to_string(error), file, line);
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		std::cout << "  GL";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		std::cout << "] ";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		std::cout << function + std::string(": ") + std::to_string(error);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << " [" << fileName(file) << ":" << line << "]\n";
		return false;
	}
	return true;
}