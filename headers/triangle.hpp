#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "objects.hpp"
#include <iostream>

class Triangle : public object
{
  private:
    unsigned int VBO;

  public:
    Triangle()
    {
    }

    void setVBO(unsigned int vbo) {
        this->VBO = vbo;
    }

    // defines a triangle
    float vertices[9] = {-0.5f, -0.5f, 0, 0.5f, -0.5f, 0, 0, 0.5f, 0};

    void handleInput() override
    {
        std::cout << "some here" << std::endl;
        for (int i = 0; i < 9; i++)
        {
            this->vertices[i] += 0.01;
        }
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
