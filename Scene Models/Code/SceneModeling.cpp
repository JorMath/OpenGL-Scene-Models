#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader.h> 
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>
#include <iostream>
#include <SFML/Audio.hpp>
#include <stdexcept>
#include <vector>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void processInput(GLFWwindow* window);

unsigned int loadTexture(const char* path);


const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
std::vector<glm::vec3> posicionCubos;
std::vector<glm::vec3> direccionCubos;

Camera camera(glm::vec3(-200.0f, 0.0f, -300.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;


bool eshlightaEncendida = true;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Examen 2B Grupo 1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize glad" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    Shader lightingShader("shaders/shader_Proyecto.vs", "shaders/shader_Proyecto.fs");
    Shader lightCubeShader("shaders/shader_exercise15_lightcube.vs", "shaders/shader_exercise15_lightcube.fs");

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };


    glm::vec3 pointLightPositions[] = {
        glm::vec3(70.0f,  20.0f,  70.0f)    
    };
    glm::vec3 posicionesNaves[] = {
        glm::vec3(1.0f),
        glm::vec3(0.6f),
        glm::vec3(0.7f),
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    Model nave("model/x-wing/x-wing.obj");
    Model skybox("model/skybox_of_constellations/skybox.obj");
    Model cohete("model/cohete/cohete.obj");
    Model alien("model/alien_planet/alien_planet.obj");
    Model halcon_milenario("model/halcon-milenario/halcon-milenario.obj");
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float currentTime = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

        
        glm::vec3 cameraPosition = camera.Position;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 20.0f);
        //luz direccional
        //Configuración de la dirección de la luz
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        // Configuración de la luz ambiental
        lightingShader.setVec3("dirLight.ambient", 0.80f, 0.80f, 0.80f);
        // Configuración de la luz difusa
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        // Configuración de la luz especular
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("pointLights[0].specular", 0.5f, 0.5f, 0.7f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.00014);
        lightingShader.setFloat("pointLights[0].quadratic", 0.00007);
       
        // spotLight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.9f, 0.9f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09);
        lightingShader.setFloat("spotLight.quadratic", 0.032);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(20.0f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 20000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        // Modelo nave
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 posicionPiloto = camera.Position - glm::vec3(0.0f, 0.9f, 0.0f);
        model = glm::translate(model, posicionPiloto);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-camera.Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(camera.Roll), camera.Front); 
        lightingShader.setMat4("model", model);
        nave.Draw(lightingShader);

        // SKYBOX=UNIVERSO
        glm::mat4 modelSkybox = glm::mat4(1.0f);
        modelSkybox = glm::translate(modelSkybox, glm::vec3(0.0f, 0.0f, 0.0f)); 
        modelSkybox = glm::scale(modelSkybox, glm::vec3(1000.0f, 1000.0f, 1000.0f));
        lightingShader.setMat4("model", modelSkybox);
        skybox.Draw(lightingShader);        
       
   
        //Modelo alien-planet
        glm::mat4 modelAlien = glm::mat4(1.0f);
        float angulo = glfwGetTime()*1.9f;
        modelAlien = glm::translate(modelAlien, glm::vec3(100.0f, 0.0f, 100.0f)); 
        modelAlien = glm::scale(modelAlien, glm::vec3(175.0f, 175.0f, 175.0f));
        modelAlien = glm::rotate(modelAlien, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", modelAlien);
        alien.Draw(lightingShader);

        //Modelo halcon Big
        glm::mat4 modelHalconBig = glm::mat4(1.0f);
        modelHalconBig = glm::translate(modelHalconBig, glm::vec3(100.0f, 200.0f, 100.0f));
        modelHalconBig = glm::scale(modelHalconBig, glm::vec3(0.8f));
        lightingShader.setMat4("model", modelHalconBig);
		halcon_milenario.Draw(lightingShader);

        for (int i = 0; i < posicionesNaves->length(); i++) {
            float radio = 10000.0f;
            float velocidad = 0.5f;
            float x = radio * cos(velocidad * glfwGetTime());
            float z = radio * sin(velocidad * glfwGetTime());
            //Modelo halcon
            glm::mat4 modelHalcon = glm::mat4(1.0f);
            modelHalcon = glm::scale(modelHalcon, glm::vec3(0.08f));
            modelHalcon = glm::translate(modelHalcon, glm::vec3(x, 3.0f * i*100, z)* posicionesNaves[i]);
            float dx = -radio * velocidad * sin(velocidad * glfwGetTime());
            float dz = radio * velocidad * cos(velocidad * glfwGetTime());
            float tan = atan2(dx, dz);

            glm::vec3 direccion = glm::normalize(glm::vec3(dx, 0.0f, dz));
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 right = glm::cross(up, direccion);
            up = glm::cross(direccion, right);
            glm::mat4 rotacion = glm::mat4(glm::vec4(right, 0.0f), glm::vec4(up, 0.0f), glm::vec4(direccion, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            modelHalcon *= rotacion;
            lightingShader.setMat4("model", modelHalcon);
            halcon_milenario.Draw(lightingShader);
        }
		
        // Configuración de la intensidad de la linterna en el shader
        float flashlightIntensidadLinterna = eshlightaEncendida ? 1.0f : 0.0f;
        lightingShader.setFloat("spotLight.diffuse", flashlightIntensidadLinterna);
        lightingShader.setFloat("spotLight.specular", flashlightIntensidadLinterna);
        // Envia el estado de la linterna al shader
        lightingShader.use();
        lightingShader.setBool("flashlightOn", eshlightaEncendida);
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(80.0f)); // Make it a smaller cube
        lightCubeShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Creacion de las balas y el comportamiento de las mismas.
        for (unsigned int i = 0; i < posicionCubos.size(); i++)
        {
            lightingShader.use();
            glm::mat4 modelCohete = glm::mat4(1.0f);
            posicionCubos.at(i) = posicionCubos.at(i) + direccionCubos.at(i);
            modelCohete = glm::translate(modelCohete, posicionCubos.at(i));
            modelCohete = glm::rotate(modelCohete, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            modelCohete = glm::rotate(modelCohete, glm::radians(-camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
            modelCohete = glm::rotate(modelCohete, glm::radians(-camera.Pitch), glm::vec3(1.0f, 0.0f, 1.0f));
            modelCohete = glm::rotate(modelCohete, glm::radians(camera.Roll), camera.Front);
            // No rotamos el modelo
            lightingShader.setMat4("model", modelCohete);
            cohete.Draw(lightingShader);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        eshlightaEncendida = !eshlightaEncendida;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Fallo al cargar la textura en la ruta: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        posicionCubos.push_back(camera.Position + glm::vec3(1.5f, -0.4f, 0.0f));
        posicionCubos.push_back(camera.Position - glm::vec3(1.5f, 0.4f, 0.0f));
        direccionCubos.push_back(camera.Front*.1f);
        direccionCubos.push_back(camera.Front*.1f);
    }
}
