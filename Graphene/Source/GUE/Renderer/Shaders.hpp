#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
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
			DBG("loading shaders");
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

			OK("shaders loaded successfully");

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

		const std::string defaultVertexShader =
			"#version 330 core\n\
			layout(location = 0) in vec2 aPosition;\n\
			layout(location = 1) in vec4 aColor;\n\
			out vec4 vertexColor;\n\
			\n\
			void main() {\n\
			  vertexColor = vec4(aColor.rgba);\n\
			  gl_Position.xy = aPosition;\n\
			  gl_Position.z = 1.0;\n\
			  gl_Position.w = 1.0;\n\
			}\n";

		const std::string defaultFragmentShader =
			"#version 330 core\n\
			out vec4 FragColor;\n\
			in vec4 vertexColor;\n\
			\n\
			void main() {\n\
			FragColor = vertexColor;\n\
			\n}";

	};
}

