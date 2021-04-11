#pragma once
#include <string>
#include <queue>

class ExceptionHandler {
	struct Exception {
	private:
		std::string m_content;


	public:
		Exception(const std::string& content) {
			m_content = content;
		}

		std::string getContent() {
			return m_content;
		}
	};
	
public:
	std::queue<Exception>& getExceptionQueue() {
		static std::queue<Exception> exceptionQueue;
		return exceptionQueue;
	}

	void insertException(const std::string& content) {
		getExceptionQueue().push(Exception(content));
	}

	void err(const std::string& content) {
		getExceptionQueue().push(Exception(content));
	}

	void warn(const std::string& content) {
		getExceptionQueue().push(Exception(content));
	}
	
	Exception pollException() {
		return getExceptionQueue().front();
	}
};