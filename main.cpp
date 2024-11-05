/**
    @author Christopher Edmunds
    @date 10/20/2024
*/

//====| Includes |====//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <stdio.h>

#include "Engine/Shader.h"
#include "Engine/Shape.h"
#include "Engine/Texture.h"
#include "Engine/MatrixStack.h"
#include "Engine/Camera.h"
//====| Namespaces |====//
using namespace std;

//====| Global Variables |====//
int _width = 800, _height = 600;
float lastX = 400, lastY = 300; // Mouse variables
bool isFirstMouse = true;
Shape *currentShape;
MatrixStack *ms;
Camera *camera;
//====| Function Declarations |====//
GLFWwindow *initWindow();                                                  // Create and initialize window to default variables
bool initGlad();                                                           // Initialize glad to expose OpenGL function pointers
void framebuffer_size_callback(GLFWwindow *window, int width, int height); // function that sets GLFWwindow size when user changes it
void mouse_callback(GLFWwindow *window, double xpos, double ypos);         // Mouse input callback
void processInput(GLFWwindow *window);                                     // Process user input

//====| Main |====//
int main(int, char **)
{
    GLFWwindow *window = initWindow();
    if (window == NULL) // If failed, exit
    {
        return 1;
    }

    Shader shader1("../Resources/Shaders/Simple.vs", "../Resources/Shaders/Simple.fs");
    Shader shader2("../Resources/Shaders/4.1.texture.vs", "../Resources/Shaders/4.1.texture.fs");

    ms = MatrixStack::getInstance();
    camera = new Camera(ms);

    // VAO textureVAO = bindImageToVAO();
    // Vertices coordinates
    GLfloat vertices[] =
        {
            -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,    // Lower left corner
            0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,     // Lower right corner
            0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,  // Upper corner
            -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
            0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // Inner right
            0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f      // Inner down
        };

    // Indices for vertices order
    GLuint indices[] =
        {
            0, 3, 5, // Lower left triangle
            3, 2, 4, // Lower right triangle
            5, 4, 1  // Upper triangle
        };

    float texVerts[] = {
        // positions          // colors           // texture coords
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int texInds[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    float triVerts[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    // Shape triangle(GL_STATIC_DRAW, triVerts, sizeof(triVerts));
    // triangle.SetVertexPointer(0, 3, 3, 0);
    // triangle.SetDrawData(0, 3);
    // triangle.SetShader(shader1);
    // triangle.Unbind();

    // Shape triangles(GL_STATIC_DRAW, vertices, sizeof(vertices), indices, sizeof(indices));
    // triangles.SetVertexPointer(0, 3, 3, 0);
    // triangles.SetDrawData(0, sizeof(indices));
    // triangles.SetShader(shader1);
    // triangles.Unbind();

    // Shape texShape(GL_STATIC_DRAW, texVerts, sizeof(texVerts), texInds, sizeof(texInds));
    // texShape.SetVertexPointer(0, 3, 8, 0);
    // texShape.SetVertexPointer(1, 3, 8, 3);
    // texShape.SetVertexPointer(2, 2, 8, 6);
    // texShape.SetShader(shader2);
    // texShape.SetDrawData(0, sizeof(texInds));
    // Texture tex(GL_TEXTURE_2D);
    // if (!tex.LoadTexture("../Resources/Photos/Homogeneous-1.png"))
    // {
    //     return 1;
    // }
    // texShape.SetTexture(tex);
    // texShape.Unbind();

    Shape shape1 = Shape(GL_STATIC_DRAW, "../Resources/Models/cube.obj");
    shape1.SetVertexPointer(0, 3, 3, 0);
    shape1.SetDrawData(0, 12 * 3);
    shape1.SetShader(shader1);

    // Move the shape into the view volume for viewing
    shape1.Translate(glm::vec3(0, 0, 5.0f));

    Shape shape2 = Shape(GL_STATIC_DRAW, "../Resources/Models/cube.obj");
    shape2.SetVertexPointer(0, 3, 3, 0);
    shape2.SetDrawData(0, 12 * 3);
    shape2.SetShader(shader1);

    // Move the shape into the view volume for viewing
    shape2.Translate(glm::vec3(0, 5.0f, 5.0f));

    currentShape = &shape1;

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) // Where the window stuff happens.
    {
        // input
        processInput(window);

        // rendering commands

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // texShape.Draw();
        shape1.Draw();
        shape2.Draw();

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);       // Fix the cursor to the middle of the window
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!initGlad()) // If failed, exit
    {
        if (window != NULL) // If window was successful, terminate it first
        {
            glfwTerminate();
        }
        return NULL;
    }
    if (MACOSX)
    {
        glViewport(0, 0, _width * 2, _height * 2); // Tell OpenGL size of rendering window
    }
    else
    {
        glViewport(0, 0, _width, _height); // Tell OpenGL size of rendering window
    }

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

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (isFirstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        isFirstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if (camera != nullptr)
    {
        camera->Pitch(yoffset);
        camera->Yaw(xoffset);
    }
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
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Shape controls
    if (currentShape != nullptr)
    {

        // Translation
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) != GLFW_PRESS)
            currentShape->Translate(glm::vec3(-.025, 0, 0));
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) != GLFW_PRESS)
            currentShape->Translate(glm::vec3(.025, 0, 0));
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) != GLFW_PRESS)
            currentShape->Translate(glm::vec3(0, -.025, 0));
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) != GLFW_PRESS)
            currentShape->Translate(glm::vec3(0, .025, 0));

        // Rotation
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
            currentShape->Rotate(-.025, glm::vec3(0, 1, 0));
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
            currentShape->Rotate(.025, glm::vec3(0, 1, 0));
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
            currentShape->Rotate(.025, glm::vec3(1, 0, 0));
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
            currentShape->Rotate(-.025, glm::vec3(1, 0, 0));

        // Scaling
        if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
            currentShape->Scale(1.01);
        if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
            currentShape->Scale(0.99);
    }

    if (camera != nullptr)
    {
        // Translation
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->SlideFront(0.05);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->SlideFront(-.05);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->SlideSide(.05);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->SlideSide(-.05);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera->SlideUp(.05);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera->SlideUp(-.025);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera->Roll(.05);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera->Roll(-.025);
    }
}