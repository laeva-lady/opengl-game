#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "objects.hpp"
#include "shader.hpp"
#include "utils.hpp"
#include <iostream>

#define DEBUG true

class Triangle : public object
{
  private:
    Logging *log;

    unsigned int VAO;
    unsigned int VBO;

    Shader *shader;

    float speed = 0.05;

    // defines a triangle
    float vertices[9] = {-0.5f, -0.5f, 0, 0.5f, -0.5f, 0, 0, 0.5f, 0};

  public:
    Triangle()
    {
        this->log = new Logging();

        this->shader = new Shader("shaders/vertex_triangle", "shaders/fragment_triangle");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex
        // attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
        this->setVAO(VAO);
        this->setVBO(VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
        // vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely
        // happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs
        // (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }
    ~Triangle()
    {
        // todo : deallocate all the shader related stuff when the triangle is destroyed
        //
        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(this->shader->shaderProgram);
    }

    void setVAO(unsigned int vao)
    {
        this->VAO = vao;
    }

    void setVBO(unsigned int vbo)
    {
        this->VBO = vbo;
    }
    void Draw() override
    {
        // draw our first triangle
        glUseProgram(this->shader->shaderProgram);
        glBindVertexArray(this->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but
                                      // we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0); // no need to unbind it every time
    }
    void HandleInput(GLFWwindow *window, int key_pressed, double deltaTime) override
    {
        std::cout << "some here" << std::endl;

        float mvt[] = {0, 0};

        auto speed = this->speed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_LEFT))
        {
            mvt[0] -= speed;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT))
        {
            mvt[0] += speed;
        }

        if (glfwGetKey(window, GLFW_KEY_UP))
        {
            mvt[1] += speed;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN))
        {
            mvt[1] -= speed;
        }

        if (DEBUG)
            this->log->print_all(mvt[0], mvt[1]);

        vertices[0] += mvt[0];
        vertices[3] += mvt[0];
        vertices[6] += mvt[0];

        vertices[1] += mvt[1];
        vertices[4] += mvt[1];
        vertices[7] += mvt[1];

        // float vertices[9] = {
        // -0.5f, -0.5f, 0,
        //  0.5f, -0.5f, 0,
        //     0,  0.5f, 0,
        //     ------------
        //     x,     y, z,
        // };
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
