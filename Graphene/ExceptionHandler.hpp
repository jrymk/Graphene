#pragma once
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <queue>

using namespace std;

// Handles exceptions thrown by functions, can draw on window instead of printing on terminal for better visuals.
class ExceptionHandler {
public:
	enum class Level { INFO, OK, WARN, ERR, FATAL };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	struct Exception {
		Level level = Level::FATAL;
		string message = "NULL";
		string file = "NULL";
		int line = -1;
	};

	queue<Exception*> exceptionBuffer;

	void throwException(string _message, Level _level, string _file, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = _level;
		newException->file = _file;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void info(string _message, string _file, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = Level::INFO;
		newException->file = _file;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void ok(string _message, string _file, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = Level::OK;
		newException->file = _file;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void warn(string _message, string _file, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = Level::WARN;
		newException->file = _file;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void err(string _message, string _file, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = Level::ERR;
		newException->file = _file;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void fatal(string _message, string _file, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = Level::FATAL;
		newException->file = _file;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	// Expected use case: Draw on screen without deleting buffer, print on terminal with deleting buffer in the end.
	bool getException(Exception* exception, bool deleteBuffer) {
		if (!exceptionBuffer.empty()) {
			exception->message = exceptionBuffer.front()->message;
			exception->level = exceptionBuffer.front()->level;
			exception->file = exceptionBuffer.front()->file;
			exception->line = exceptionBuffer.front()->line;
			delete exceptionBuffer.front();
			exceptionBuffer.pop();
			return true;
		}
		else return false;
	}

	string fileName(string fullPath) {
		string temp = "";
		for (int i = 0; i < fullPath.size(); i++) {
			temp.push_back(fullPath[i]);
			if (fullPath[i] == '\\')
				temp = "";
		}
		return temp;
	}

	void flushExceptionsToIOStream() {
		// Display exceptions to the terminal
		ExceptionHandler::Exception* exception = new ExceptionHandler::Exception;
		while (getException(exception, true)) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << "[";
			switch (exception->level) {
			case ExceptionHandler::Level::INFO:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				std::cout << " INFO";
				break;
			case ExceptionHandler::Level::OK:
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				std::cout << "  OK ";
				break;
			case ExceptionHandler::Level::WARN:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				std::cout << " WARN";
				break;
			case ExceptionHandler::Level::ERR:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
				std::cout << "ERROR";
				break;
			case ExceptionHandler::Level::FATAL:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				std::cout << "FATAL";
				break;
			}
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << "]";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			string file = exception->file;
			std::cout << " " << std::setw(20) << fileName(exception->file) << " | ";
			std::cout << " " << std::setw(4) << exception->line << " | ";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << exception->message << "\n";
		}
	}
};
