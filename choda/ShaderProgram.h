#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace choda {

	class ShaderProgram
	{
	public:
		ShaderProgram() : programID(glCreateProgram()) { }

		ShaderProgram& vertex(const char* source);
		ShaderProgram& fragment(const char* source);

		void link();
		void use();

		void setVec3f(const GLchar* varName, const std::vector<GLfloat>& vals);
		void setVec3f(const GLchar* varName, const glm::vec3& vals);
		void setVec3f(const GLchar* varName, const GLfloat x, const GLfloat y, const GLfloat z);
		void setVec2f(const GLchar* varName, const std::vector<GLfloat>& vals);
		void setVec2f(const GLchar* varName, const glm::vec2& vals);
		void setVec2f(const GLchar* varName, const GLfloat x, const GLfloat y);
		void setFloat(const GLchar* varName, const GLfloat value);
		void setInt(const GLchar* varName, const GLint value);
		void setMat4(const GLchar* varName, const glm::mat4& mat);

		~ShaderProgram();

	private:
		void compileShader(GLuint shader, const char* source);

	private:
		GLuint programID;
		std::vector<GLuint> shaderID;
	};
}

