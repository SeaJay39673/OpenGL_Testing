#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
char *getFileBytes(string);

int main(int, char **)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Specify which type of OpenGL to use
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);                       // Start in maximized mode
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Needed for Mac

    // Create the window and display it
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) // Error creating window
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize glad to find and expose OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600); // Tell OpenGL size of rendering window

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Call back so user can resize the window

    // TODO: Finish Shaders and Textures Tutorial
    // TODO: Render Image to Screen

    while (!glfwWindowShouldClose(window)) // Where the window stuff happens.
    {
        // input
        processInput(window);

        // rendering commands

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate(); // Properly exit the application
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

char *getFileBytes(string fileName)
{
    FILE *pFile;
    long lSize;
    char *buffer;
    size_t result;

    pFile = fopen(fileName.c_str(), "rb");
    if (pFile == NULL)
    {
        fputs("File error", stderr);
        exit(1);
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
    buffer = (char *)malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
        fputs("Memory error", stderr);
        exit(2);
    }

    // copy the file into the buffer:
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize)
    {
        fputs("Reading error", stderr);
        exit(3);
    }

    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose(pFile);

    return buffer;
}