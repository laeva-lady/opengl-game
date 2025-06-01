#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

#include "objects.hpp"
#include "shader.hpp"
#include "utils.hpp"

#define DEBUG true

class Triangle : public object {
   private:
    Logging *log;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int colorVBO;

    Shader *shader;

    float position[2] = {0, 0};
    float speed = 0.1;

    // defines a triangle
    float vertices[9] = {
        // x, y, z, r, g, b
        -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f  // top
    };
    float colorVertices[9] = {
        1,   0.5, 0,  //
        0,   1,   0,  //
        0.5, 0,   1   //
    };

   public:
    Triangle() {
        this->log = new Logging();

        this->shader = new Shader("../shaders/vertex_triangle.glsl", "../shaders/fragment_triangle.glsl");
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        unsigned int colorVBO, VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &colorVBO);
        // bind the Vertex Array Object first, then bind and set vertex
        // buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

        // Verteces attribute (location = 0), (float[])
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Color attribute (location = 1), (float[])
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->colorVertices), this->colorVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);

        this->VAO = VAO;
        this->VBO = VBO;
        this->colorVBO = colorVBO;

        // note that this is allowed, the call to glVertexAttribPointer
        // registered VBO as the vertex attribute's bound vertex buffer object
        // so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't
        // accidentally modify this VAO, but this rarely happens. Modifying
        // other VAOs requires a call to glBindVertexArray anyways so we
        // generally don't unbind VAOs (nor VBOs) when it's not directly
        // necessary.
        glBindVertexArray(0);
    }
    ~Triangle() {
        // optional: de-allocate all resources once they've outlived their
        // purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &colorVBO);
        glDeleteProgram(this->shader->shaderProgram);
    }

    void Draw() override {
        glUseProgram(this->shader->shaderProgram);

        // send position offset to shader
        int offsetLoc = glGetUniformLocation(this->shader->shaderProgram, "offset");
        glUniform2f(offsetLoc, position[0], position[1]);
        if (DEBUG) this->log->print_all(offsetLoc);

        // get the buffer of the array before glDrawArrays'ing it
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // unbing the buffer to be clean afterwards
        glBindVertexArray(0);
    }

    void HandleInput(GLFWwindow *window, int key_pressed, double deltaTime) override {
        float mvt[] = {0, 0};

        auto speed = this->speed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_LEFT) == key_pressed) mvt[0] -= speed;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == key_pressed) mvt[0] += speed;

        if (glfwGetKey(window, GLFW_KEY_UP) == key_pressed) mvt[1] += speed;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == key_pressed) mvt[1] -= speed;

        float length = std::sqrt(std::pow(mvt[0], 2) + std::pow(mvt[1], 2));
        if (length > 1e-6f) {
            mvt[0] /= length;
            mvt[1] /= length;
            mvt[0] *= speed;
            mvt[1] *= speed;
        }

        if (DEBUG) {
            this->log->print_all("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
            this->log->print_all(length, mvt[0], mvt[1]);
            this->log->print_all(this->position[0], this->position[1]);
        }

        this->position[0] += mvt[0];
        this->position[1] += mvt[1];
    }
};
