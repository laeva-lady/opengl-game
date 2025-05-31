#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct Shader
{
    const char *vertexPath;
    const char *fragmentPath;
    GLuint shaderProgram;

    Shader(const char *vertexPath, const char *fragmentPath)
    {
        this->vertexPath = vertexPath;
        this->fragmentPath = fragmentPath;

        this->shaderProgram = this->createShaderProgram(this->vertexPath, this->fragmentPath);
    }

    std::string readFile(const char *filePath)
    {
        std::ifstream file(filePath);
        if (!file)
        {
            std::cerr << "Failed to open " << filePath << std::endl;
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    GLuint compileShader(GLenum type, const std::string &source)
    {
        GLuint shader = glCreateShader(type);
        const char *src = source.c_str();

        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        // Check compilation errors
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader Compilation Error:\n" << infoLog << std::endl;
        }

        return shader;
    }

    GLuint createShaderProgram(const char *vertexPath, const char *fragmentPath)
    {
        std::string vertexCode = readFile(vertexPath);
        std::string fragmentCode = readFile(fragmentPath);

        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Check linking errors
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cerr << "Shader Linking Error:\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }
};
