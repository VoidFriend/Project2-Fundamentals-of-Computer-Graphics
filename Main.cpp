#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "stb_image.h"
#include "Texture.h"

#include <iostream>
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void Translate(float velocity, int axis);
void Scale(float percent);
void Reset();
glm::mat4 Rotate(glm::mat4 model);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Cube properties
glm::vec3 position;
float scale;
glm::vec3 rotation;
float angleVelocity = 0.015f;

float xAngle;
float yAngle;
float zAngle;

float movementVelocity = 0.01f;


int main()
{
    //set random seed
    srand(static_cast <unsigned> (time(0)));

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("shader.vs", "shader.fs"); // you can name your shader files however you like
// set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------

    int width, height, nrChannels;

    Texture box;

    box.buildTexture("resources/textures/box.jpg", false);

    Texture kirby;

    kirby.buildTexture("resources/textures/kirby.png", true);


    ourShader.Use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    ourShader.setFloat("scale", 1.0f);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Reset();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

         // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, box.texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, kirby.texture);

        // activate shader
        ourShader.Use();

        // create transformations
        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection = glm::mat4(1.0f);

        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

        glBindVertexArray(VAO);

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, position);

        model = Rotate(model);

        model = glm::scale(model, scale * glm::vec3(1.0f, 1.0f, 1.0f));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // pass transformation matrices to the shader
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

// glfw: terminate, clearing all previously allocated GLFW resources.
// ------------------------------------------------------------------
glfwTerminate();
return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Reset();
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Translate(-movementVelocity, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Translate(movementVelocity, 0);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Translate(movementVelocity, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Translate(-movementVelocity, 1);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        Translate(-movementVelocity, 2);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        Translate(movementVelocity, 2);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        Scale(.01f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        Scale(-.01f);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {

        yAngle += -angleVelocity;

    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {

        yAngle += angleVelocity;

    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {

        xAngle += -angleVelocity;

    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        xAngle += angleVelocity;

    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {

        zAngle += angleVelocity;

    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        zAngle += -angleVelocity;
    }

}

void Translate(float velocity, int axis) {

    if (axis == 0) {
        position.x += velocity;
    }
    else if (axis == 1) {
        position.y += velocity;
    }
    else if (axis == 2) {
        position.z += velocity;
    }
}

void Scale(float percent) {

    scale *= (1 + percent);

    if (scale < 0.01f) {
        scale = 0.01f;
    }
}

glm::mat4 Rotate(glm::mat4 model) {

    glm::mat4 rotationX = glm::rotate(model, xAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(model, yAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(model, zAngle, glm::vec3(0.0f, 0.0f, 1.0f));

    model = rotationX * rotationY * rotationZ;

    return model;
}

void Reset() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(1.0f, 0.0f, 0.0f);
    scale = 1.0f;
    xAngle = 0;
    yAngle = 0;
    zAngle = 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
   // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

