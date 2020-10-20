#include "ShaderProgram.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

using namespace choda;

void ShaderProgram::compileShader(GLuint shader, const char* path) {
	std::ifstream in;
	in.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	std::stringstream ss;

	try {
		in.open(path);
		ss << in.rdbuf();
		in.close();
	}
	catch (std::ifstream::failure& e) {
		std::cerr << "[File Error]: " << e.what() << '\n';
	}

	std::string temp = ss.str();
	const char* source = temp.c_str();
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint ok;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "[Compile Error]: " << infoLog << '\n';
	}
}


ShaderProgram& ShaderProgram::vertex(const char* source) {
	GLuint id = glCreateShader(GL_VERTEX_SHADER);
	compileShader(id, source);
	glAttachShader(programID, id);

	shaderID.push_back(id);
	return *this;
}


ShaderProgram& ShaderProgram::fragment(const char* source) {
	GLuint id = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(id, source);
	glAttachShader(programID, id);

	shaderID.push_back(id);
	return *this;
}


ShaderProgram::~ShaderProgram() {
	glDeleteProgram(programID);

	for (GLuint& shader : shaderID)
		glDeleteShader(shader);
}


void ShaderProgram::link() {
	glLinkProgram(programID);

	GLint ok;
	char infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &ok);
	if (!ok) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "[Link Error]: " << infoLog << '\n';
	}
}


void ShaderProgram::use() {
	glUseProgram(programID);
}


void ShaderProgram::setVec3f(const GLchar* varName, const std::vector<GLfloat>& vals) {
	GLuint loc = glGetUniformLocation(programID, varName);
	glUniform3f(loc, vals[0], vals[1], vals[2]);
}


void ShaderProgram::setVec2f(const GLchar* varName, const std::vector<GLfloat>& vals) {
	GLuint loc = glGetUniformLocation(programID, varName);
	glUniform2f(loc, vals[0], vals[1]);
}


void ShaderProgram::setFloat(const GLchar* varName, const GLfloat value) {
	GLuint loc = glGetUniformLocation(programID, varName);
	glUniform1f(loc, value);
}

void ShaderProgram::setInt(const GLchar* varName, const GLint value) {
	GLuint loc = glGetUniformLocation(programID, varName);
	glUniform1i(loc, value);
}

void ShaderProgram::setMat4(const GLchar* varName, const glm::mat4& mat)
{
	GLuint loc = glGetUniformLocation(programID, varName);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}
