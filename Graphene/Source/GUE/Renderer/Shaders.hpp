#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "../../ExceptionHandler.hpp"

namespace gue {
	class Shader {
	private:
		std::string m_vertexShaderSource;
		std::string m_fragmentShaderSource;
		GLint m_programId = 0;

	public:
		Shader() = default;

		Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
			m_vertexShaderSource = vertexShaderSource;
			m_fragmentShaderSource = fragmentShaderSource;
		}

		void setShaderSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
			m_vertexShaderSource = vertexShaderSource;
			m_fragmentShaderSource = fragmentShaderSource;
		}

		void setVertexShaderSource(const std::string& vertexShaderSource) {
			m_vertexShaderSource = vertexShaderSource;
		}

		void setFragmentShaderSource(const std::string& fragmentShaderSource) {
			m_fragmentShaderSource = fragmentShaderSource;
		}

		bool loadShader() {
			// load and compile vertex shader
			
			const GLchar* vertexShaderSourceCStr = m_vertexShaderSource.c_str();

			const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, nullptr);
			glCompileShader(vertexShader);

			GLint vertexShaderCompiled;
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompiled);

			if (!vertexShaderCompiled) {
				GLsizei logLength = 0;
				GLchar logMessage[1024];
				glGetShaderInfoLog(vertexShader, 1024, &logLength, logMessage);
				ERR("vertex shader compilation failed: " + std::string(logMessage));
				return false;
			}

			// load and compile fragment shader
			
			const GLchar* fragmentShaderSourceCStr = m_fragmentShaderSource.c_str();

			const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr);
			glCompileShader(fragmentShader);

			GLint fragmentShaderCompiled;
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompiled);

			if (!fragmentShaderCompiled) {
				GLsizei logLength = 0;
				GLchar logMessage[1024];
				glGetShaderInfoLog(fragmentShader, 1024, &logLength, logMessage);
				ERR("fragment shader compilation failed: " + std::string(logMessage));
				return false;
			}


			m_programId = glCreateProgram();

			// bind vertex attributes here if necessary

			glAttachShader(m_programId, vertexShader);
			glAttachShader(m_programId, fragmentShader);
			glLinkProgram(m_programId);

			GLint programLinked;
			glGetProgramiv(m_programId, GL_LINK_STATUS, &programLinked);

			if (!programLinked) {
				GLsizei logLength = 0;
				GLchar logMessage[1024];
				glGetShaderInfoLog(fragmentShader, 1024, &logLength, logMessage);
				ERR("shader program linkage failed: " + std::string(logMessage));
				return false;
			}


			// delete shaders

			glDetachShader(m_programId, vertexShader);
			glDetachShader(m_programId, fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			
			return true;
		}

		void useShader() {
			glUseProgram(m_programId);
		}
		
	};
}

