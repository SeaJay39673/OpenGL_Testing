/*
    Author: Christopher Edmunds
    Last Updated: 10/20/2024
*/

//====| Includes |====//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Engine/ShapeFactory.h"
#include "Shader.h"
//====| Namespaces |====//
using namespace std;

//====| Global Variables |====//
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
                                   "}\0";
int _width = 800, _height = 600;
//====| Function Declarations |====//
GLFWwindow *
initWindow();                                                                       // Create and initialize window to default variables
bool initGlad();                                                                    // Initialize glad to expose OpenGL function pointers
void framebuffer_size_callback(GLFWwindow *window, int width, int height);          // function that sets GLFWwindow size when user changes it
void processInput(GLFWwindow *window);                                              // Process user input
unsigned int updateShader(unsigned int type, const char *shader);                   // Update the target shader
unsigned int compileShader(unsigned int vertexShader, unsigned int fragmentShader); // Compile the target shaders
void deleteShaders(unsigned int vertexShader, unsigned int fragmentShader);         // Delete the shaders (after compiled) as they aren't needed
unsigned int bindImageToVAO();

//====| Main |====//
int main(int, char **)
{
    GLFWwindow *window = initWindow();
    if (window == NULL) // If failed, exit
    {
        return 1;
    }

    unsigned int vertexShader = updateShader(GL_VERTEX_SHADER, vertexShaderSource);
    if (vertexShader == NULL)
    {
        return 1;
    }
    unsigned int fragmentShader = updateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (fragmentShader == NULL)
    {
        return 1;
    }

    unsigned int shaderProgram = compileShader(vertexShader, fragmentShader);

    // TODO: abstract image loading code into it's own function/class
    float texCoords[] = {
        0.0f, 0.0f, // lower-left corner
        1.0f, 0.0f, // lower-right corner
        0.5f, 1.0f  // top-center corner
    };

    Shader shader("../Resources/Shaders/4.1.texture.vs", "../Resources/Shaders/4.1.texture.fs");

    int textureVAO = bindImageToVAO();

    ShapeFactory &sf = ShapeFactory::GetInstance();
    unsigned int TriVAO = sf.getTriVAO();
    unsigned int SquareVAO = sf.getSquareVAO();
    while (!glfwWindowShouldClose(window)) // Where the window stuff happens.
    {
        // input
        processInput(window);

        // rendering commands

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(TriVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(SquareVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        shader.use();
        glBindVertexArray(textureVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate(); // Properly exit the application
    return 0;
}

//====| Function Definitions |====//

/*
    Creates and initializes window with default variables
    Parameters: None
    returns: GLWwindow*
*/
GLFWwindow *initWindow()
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
        return NULL;
    }
    glfwMakeContextCurrent(window);

    glfwGetWindowSize(window, &_width, &_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Call back so user can resize the window

    if (!initGlad()) // If failed, exit
    {
        if (window != NULL) // If window was successful, terminate it first
        {
            glfwTerminate();
        }
        return NULL;
    }
    glViewport(0, 0, _width, _height); // Tell OpenGL size of rendering window

    return window;
}
/*
    Initializes GLAD and exposes OpenGL function pointers
    Parameters: None
    Returns: Boolean
*/
bool initGlad()
{
    // Initialize glad to find and expose OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

/*
    Callback function to reset the window size global variables and set viewport accordingly
    Parameters: GLFWwindow* window, int width, int height
    Returns: None
*/
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    _width = width;
    _height = height;
    glViewport(0, 0, _width, _height);
}

/*
    Gets user input and responds accordingly
    Parameters: GLFWwindow* window
    Returns: None
*/
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

/*
    Updates the target shader
    Parameters: unsigned int type (ex: GL_FRAGMENT_SHADER), const char* shaderCode (ex: char array of shader code)
    Returns: NULL (if failure), unsigned int (shaderID if success)
*/
unsigned int updateShader(unsigned int type, const char *shaderCode)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
             << infoLog << endl;
        return NULL;
    }
    return shader;
}

/*
    Compiles the updated shaders (vertex and fragment)
    Parameters: unsigned int vertexShader (vertex shaderID), unsigned int fragmentShader (fragment shaderID)
    Returns: Boolean (success)
*/
unsigned int compileShader(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::PROGRAM_FAILED\n"
             << infoLog << endl;
        deleteShaders(vertexShader, fragmentShader);
        return NULL;
    }
    deleteShaders(vertexShader, fragmentShader);
    return shaderProgram;
}

/*
    Deletes the vertexShader and fragmentShader
    Parameters: unsigned int (vertex shader), unsigned int (fragment shader)
    Returns: None
*/
void deleteShaders(unsigned int vertexShader, unsigned int fragmentShader)
{
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int bindImageToVAO()
{
    float vertices[] = {
        // positions          // colors           // texture coords
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../Resources/Photos/Homogeneous-1.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return NULL;
    }

    stbi_image_free(data);
    return VAO;
}