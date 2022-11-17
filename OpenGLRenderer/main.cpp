
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_opengl3_loader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/Shader.h"
#include "Texture.h"
#include "camera/Camera.h"

#include "vector"

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


//CAMERA
glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 4.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera camera = Camera(cameraPos, cameraFront, cameraUp);

float lastX = 400, lastY = 300;

float yaw = -90.0f;
float pitch = 0.0f;

//DELTA
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

bool showMenu = false;
bool movementEnabled = true;
bool firstMouse = true;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1200, 900, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);


    // -----------  IMGUI SETUP ----------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    // ----------- END IMGUI SETUP -------------


    glViewport(0, 0, 1200, 900);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader lampShader("shader/shader_without_texture.vs", "shader/shader_lamp.fs");
    Shader cubeShader("shader/shader_without_texture.vs", "shader/shader_color.fs");


    glEnable(GL_DEPTH_TEST);

    //VERTICES
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };



    //LIGHT VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    unsigned int VBO; //Vertex Buffer Object

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //CUBE VAO
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    std::vector<glm::vec3> cubePositions = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(1.0f,  0.0f,  0.0f),
        glm::vec3(-1.0f,  0.0f,  0.0f),
        glm::vec3(0.0f, 0.0f,  1.0f),
        glm::vec3(0.0f,  0.0f,  -1.0f)
    };

    float lightY = 0.0f;
    float lightSpeed = 1.0f;
    float lampColor[3] = { 1.0f, 1.0f, 1.0f };
    float ambientStrength = 0.2f;
    float specularStrength = 0.7f;
    float backgroundColor[3] = { 0.1f, 0.1f, 0.1f };
    float objectColor[3] = { 1.0f, 1.0f, 1.0f };

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 900.0f, 0.1f, 100.0f);


        lampShader.use();

        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", camera.view);


        glm::vec3 lightColor(lampColor[0], lampColor[1], lampColor[2]);
        
        glm::vec3 lightPos(1.2f, lightY, 2.0f);

        glm::mat2 rotationMat = glm::mat2(1.0f);
        rotationMat[0][0] = cos(glfwGetTime()*lightSpeed);
        rotationMat[1][0] = -sin(glfwGetTime()*lightSpeed);
        rotationMat[0][1] = sin(glfwGetTime()*lightSpeed);
        rotationMat[1][1] = cos(glfwGetTime()*lightSpeed);

        glm::vec2 lightPosXY(lightPos.x, lightPos.z);

        glm::vec2 lightPosXYChanged = rotationMat* lightPosXY;

        //lightPos.y = sin(glfwGetTime()*2);
        lightPos.x = lightPosXYChanged.x;
        lightPos.z = lightPosXYChanged.y;
    

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lampShader.setMat4("model", model);
        lampShader.setVec3("lightColor", lightColor);
        
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        cubeShader.use();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", camera.view);

        for (int i = 0; i < cubePositions.size(); i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            cubeShader.setMat4("model", model);
            cubeShader.setVec3("objectColor", objectColor[0], objectColor[1], objectColor[2]);
            cubeShader.setVec3("lightColor", lightColor);
            cubeShader.setVec3("lightPos", lightPos);
            cubeShader.setVec3("viewPos", camera.cameraPos);

            cubeShader.setFloat("ambientStrength", ambientStrength);
            cubeShader.setFloat("specularStrength", specularStrength);

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        

        if (showMenu) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Light");
            ImGui::DragFloat("Light Y", &lightY, 0.05f);
            ImGui::DragFloat("Light Speed", &lightSpeed, 0.05f);
            ImGui::ColorPicker3("Light Color", lampColor);
            ImGui::DragFloat("Ambient Strength", &ambientStrength, 0.01, 0.0, 1.0);
            ImGui::DragFloat("Specular Strength", &specularStrength, 0.01, 0.0, 1.0);
            ImGui::End();

            ImGui::Begin("Scene");
            ImGui::ColorPicker3("Background Color", backgroundColor);
            ImGui::End();

            ImGui::Begin("Objects");
            ImGui::ColorPicker3("Cube Color", objectColor);
            ImGui::End();


            ImGui::Begin("Info");
            std::string FPS_COUNTER = "Draw Time: " + std::to_string((float)(deltaTime * 1000)) + "ms";
            ImGui::Text(FPS_COUNTER.c_str());
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }



        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (movementEnabled) {
        float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            cameraSpeed *= 2;
        }


        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.movePos(CAMERA_UP, cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.movePos(CAMERA_DOWN, cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.movePos(CAMERA_LEFT, cameraSpeed);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.movePos(CAMERA_RIGHT, cameraSpeed);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) {
        showMenu = !showMenu;

        if (showMenu) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        }

        movementEnabled = !movementEnabled;
    }
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (movementEnabled){
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        const float sensitivity = 0.02f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        //Restrain pitch to not flip
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        camera.moveView(yaw, pitch);
    }
}