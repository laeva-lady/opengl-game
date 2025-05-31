#pragma once

#include <GLFW/glfw3.h>
class object
{
  public:
    virtual void HandleInput(GLFWwindow *window, int key_pressed, double deltaTime) {};
    virtual void Draw() {};
};
