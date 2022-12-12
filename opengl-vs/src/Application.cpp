#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "../utils/Texture.h"
#include "../shaders/Pipeline.h"

void processInput();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double scrollX, double scrollY);

// window settings
const char* TITLE = "POGLA - Grass Simulation";
const unsigned int SCREEN_W = 1920;
const unsigned int SCREEN_H = 1080;

// globals
GLFWwindow* window;
Camera camera;

float deltaTime, lastDelta = 0;
float lastXPos, lastYPos = 0;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_W, SCREEN_H, TITLE, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGL();

	glfwSwapInterval(0);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // shaders
	Shader vertexShader = Shader(ShaderType::VERTEX, "./shaders/vertex.glsl");
	Shader geometryShader = Shader(ShaderType::GEOMETRY, "./shaders/geometry.glsl");
	Shader fragmentShader = Shader(ShaderType::FRAGMENT, "./shaders/fragment.glsl");
    
	Pipeline pipeline({ vertexShader, geometryShader, fragmentShader});
    pipeline.use();
    
    // transforms
    float aspectRatio = (float)SCREEN_W / (float)SCREEN_H;
    glm::mat4 proj = camera.getPerspective(aspectRatio);
    glm::mat4 view = camera.getView();

	pipeline.setMat4("uProj", glm::value_ptr(proj));
    pipeline.setMat4("uView", glm::value_ptr(view));

    // objects
    srand(time(NULL));

    std::vector<glm::vec3> grid;
    for (float x = -10.0f; x < 10.0f; x += 0.2f)
    {
        for (float z = -10.0f; z < 10.0f; z += 0.2f)
        {
            grid.push_back(glm::vec3(x, 0, z));
        }
    }
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, grid.size() * sizeof(glm::vec3), grid.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // textures
    /*
    unsigned int texture1 = loadTexture("./assets/grass_texture.png");
    pipeline.use();
    pipeline.setInt("uTextGrass", 0);

    unsigned int texture2 = loadTexture("./assets/flowmap.png");
    pipeline.use();
    pipeline.setInt("uWind", 1);
    */

    glPointSize(5.0f);
    while (!glfwWindowShouldClose(window))
    {
        // delta time
        float currDelta = glfwGetTime();
        deltaTime = currDelta - lastDelta;
        lastDelta = currDelta;
        camera.updateSpeed(deltaTime);
        
        // input
        processInput();

        // textures
        /*
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        */
        
        // transforms
		view = camera.getView();
        proj = camera.getPerspective(aspectRatio);
        pipeline.setMat4("uProj", glm::value_ptr(proj));
		pipeline.setMat4("uView", glm::value_ptr(view));
        
		//pipeline.setVec3("uCameraPos", glm::value_ptr(camera.getPosition()));
		//pipeline.setFloat("uTime", currDelta);
        
        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pipeline.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, grid.size());

        // events and buffer swap
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    pipeline.detach();

    /*
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    */

    glfwTerminate();
    
    return 0;
}

bool doesPress(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

void processInput()
{
	if (doesPress(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);

	if (doesPress(GLFW_KEY_W))
		camera.move(FRONT);
    if (doesPress(GLFW_KEY_S))
        camera.move(BACK);
    if (doesPress(GLFW_KEY_A))
        camera.move(LEFT);
    if (doesPress(GLFW_KEY_D))
        camera.move(RIGHT);
	if (doesPress(GLFW_KEY_SPACE))
		camera.move(UP);
}

/* callbacks definitions */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double scrollX, double scrollY)
{
    camera.updateFOV(scrollY);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    float xOffset = xPos - lastXPos;
    float yOffset = lastYPos - yPos;
    lastXPos = xPos;
    lastYPos = yPos;

	camera.updateDirection(xOffset, yOffset);
}