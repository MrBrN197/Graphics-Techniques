#pragma once

#include "core.h"
#include <stdio.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

class Shader{
public:
	uint32 m_Program;
	const char* m_Path;

public:
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath) : m_Path(vertexShaderPath) {
		uint32 program = glCreateProgram();
		m_Program = program;

		// VERTEX
        FILE* fp = fopen(vertexShaderPath, "rb");
        if(!fp){
			printf("Failed To Locate File %s\n", vertexShaderPath);
			getchar();
        }
		fseek(fp, 0, SEEK_END);
		uint32 size = ftell(fp);
		fseek(fp, 0, SEEK_SET); 
		char* vertexShaderSource = new char[size+1];
		fread(vertexShaderSource, size*sizeof(char), 1, fp);
		vertexShaderSource[size] = (char)'\0'; 
		fclose(fp);

		uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const char* sourceCstr = vertexShaderSource;
		glShaderSource(vertexShader, 1, &sourceCstr, 0);

		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE){
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			char* infoLog = new char[maxLength];
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog);

			glDeleteShader(vertexShader);
			ASSERT(false, "[%d] SHADER COMPILE ERROR: [%s] path: %s\n", GL_VERTEX_SHADER, infoLog, vertexShaderPath);
			return;
		}


		// FRAGMENT
        fp = fopen(fragmentShaderPath, "rb");
        if(!fp){
			printf("Failed To Locate File %s\n", fragmentShaderPath);
			getchar();
        }
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET); 
		char* fragmentShaderSource = new char[size+1];
		fread(fragmentShaderSource, size*sizeof(char), 1, fp);
		fragmentShaderSource[size] = (char)'\0';

		uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		sourceCstr = fragmentShaderSource;
		glShaderSource(fragmentShader, 1, &sourceCstr, 0);

		glCompileShader(fragmentShader);

		isCompiled = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE){
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			char* infoLog = new char[maxLength];
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog);

			glDeleteShader(fragmentShader);
			ASSERT(false, "[%d] SHADER COMPILE ERROR: [%s] path: %s\n", GL_FRAGMENT_SHADER, infoLog, fragmentShaderPath);
			return;
		}

		glAttachShader(program, fragmentShader);
		glAttachShader(program, vertexShader);

		// LINKAGE
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)& isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			char* infoLog = new char[maxLength];
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			ASSERT(false, "SHADER LINKAGE ERROR: [%s] path: %s\n", infoLog, vertexShaderPath);
			return;
		}

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		m_Program = program;
	}
	~Shader(){
		glDeleteProgram(m_Program);
	}

	void Bind(){
		glUseProgram(m_Program);
	}
	uint32 GetUniformLocation(const char* name){
		glUseProgram(m_Program);
		uint32 location = glGetUniformLocation(m_Program, name);
		ASSERT(location != -1, "Failed To Get Location %s", name);
		return location;
	}
	void SetMat4(const char* name, float* matrix){
		uint32 location = GetUniformLocation(name);
		if(location == -1){
			return;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
	}
	void SetFloat(const char* name, float value){
		uint32 location = GetUniformLocation(name);
		if(location == -1){
			return;
		}
		glUniform1f(location, value);
	}
	void SetFloat2(const char* name, float x, float y){
		uint32 location = GetUniformLocation(name);
		if(location == -1){
			return;
		}
		glUniform2f(location, x, y);
	}
	void SetFloat3(const char* name, float x, float y, float z){
		uint32 location = GetUniformLocation(name);
		if(location == -1){
			return;
		}
		glUniform3f(location, x, y, z);
	}
	void SetInt(const char* name, int value){
		uint32 location = GetUniformLocation(name);
		if(location == -1){
			return;
		}
		glUniform1i(location, value);
	}
};