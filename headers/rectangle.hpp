#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "objects.hpp"
#include "shader.hpp"
#include "utils.hpp"

#define DEBUG true

class Rectangle : public object
{
  private:
    Logging *log;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    Shader *shader;

    float position[2] = {0, 0};
    float speed = 0.1;

    // defines a triangle
    float vertices[24] = {
        // x, y, z, r, g, b
        0.5f,  0.5f,  0, 1, 0, 1, // top right
        0.5f,  -0.5f, 0, 1, 0, 0, // bottom right
        -0.5f, -0.5f, 0, 0, 1, 1, // bottom left
        -0.5f, 0.5f,  0, 0, 0, 1  // top left
    };
    unsigned int indexes[6] = {0, 1, 3, 1, 2, 3};

  public:
    Rectangle()
    {
        this->log = new Logging();

        this->shader = new Shader("../shaders/vertex_triangle.glsl", "../shaders/fragment_triangle.glsl");
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
        this->VAO = VAO;
        this->VBO = VBO;

        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indexes), this->indexes, GL_STATIC_DRAW);
        this->EBO = EBO;

        // Position attribute (location = 0), 3 floats per vertex, starting at offset 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Color attribute (location = 1), 3 floats per vertex, offset = 3 floats
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
        // vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely
        // happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs
        // (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }
    ~Rectangle()
    {
        // todo : deallocate all the shader related stuff when the triangle is destroyed
        //
        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(this->shader->shaderProgram);
    }

    void Draw() override
    {
        glUseProgram(this->shader->shaderProgram);

        // send position offset to shader
        int offsetLoc = glGetUniformLocation(this->shader->shaderProgram, "offset");
        glUniform2f(offsetLoc, position[0], position[1]);
        if (DEBUG)
            this->log->print_all(offsetLoc);

        // Bind VAO before drawing
        glBindVertexArray(this->VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0); // optional, safe to unbind
    }
};
