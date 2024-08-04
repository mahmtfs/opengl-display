#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Graphics{
public:
    void init();
    GLFWwindow* setupWindow(int width, int height, const char* title);
};

#endif
