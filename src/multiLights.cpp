//
// Created by cliff on 2018/9/4.
//


#include "vapp.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "shader.h"

#include "camera.h"

#include "texture.h"

class MultiLights : public GinApplication {
public:
    typedef class GinApplication base;

    static GinApplication *Create(void) {
        return (s_app = new MultiLights);
    }

    virtual void init(const char *title);
    virtual void display();
    virtual void onKey(int key, int scancode, int action, int mods);
    virtual void onMouse(double xpos, double ypos);
    virtual void onScroll(double xoffset, double yoffset);
    virtual void finalize();

    Shader *lightShader;
    Shader *lampShader;

    unsigned int VBO;
    unsigned int cubeVAO, lightVAO;

    unsigned int diffuseMap;
    unsigned int specularMap;

    Camera *camera;

    // world space positions of our cubes
    glm::vec3 cubePositions[10] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[4] = {
            glm::vec3(0.7f, 0.2f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -3.0f)
    };

};

GinApplication *GinApplication::s_app;

void GinApplication::mainLoop() {
    do {
        display();
        glfwPollEvents();
    } while (!glfwWindowShouldClose(m_pWindow));
}

int main(int argc, char **argv) {
    GinApplication *app = MultiLights::Create();

    app->init("MultiLights Example");
    app->mainLoop();
    app->finalize();

    return 0;
}

void MultiLights::init(const char *title) {
    base::init(title);

    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    lightShader = new Shader("shaders/multi_lights_vs.glsl", "shaders/multi_lights_fs.glsl");
    lampShader = new Shader("shaders/lamp_vs.glsl", "shaders/lamp_fs.glsl");

    glEnable(GL_DEPTH_TEST);

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

    //cube
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //light
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    //texture
    diffuseMap = loadTexture("resources/container2.png");
    specularMap = loadTexture("resources/container2_specular.png");

    lightShader->use();
    lightShader->setInt("material.diffuse", 0);
    lightShader->setInt("material.specular", 1);

}

void MultiLights::display() {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightShader->use();
    lightShader->setVec3("viewPos", camera->Position);
    lightShader->setFloat("material.shininess", 32.0f);

    // directional light
    lightShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    lightShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    lightShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    lightShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    lightShader->setVec3("pointLights[0].position", pointLightPositions[0]);
    lightShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    lightShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    lightShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    lightShader->setFloat("pointLights[0].constant", 1.0f);
    lightShader->setFloat("pointLights[0].linear", 0.09);
    lightShader->setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    lightShader->setVec3("pointLights[1].position", pointLightPositions[1]);
    lightShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    lightShader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    lightShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    lightShader->setFloat("pointLights[1].constant", 1.0f);
    lightShader->setFloat("pointLights[1].linear", 0.09);
    lightShader->setFloat("pointLights[1].quadratic", 0.032);
    // point light 3
    lightShader->setVec3("pointLights[2].position", pointLightPositions[2]);
    lightShader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    lightShader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    lightShader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    lightShader->setFloat("pointLights[2].constant", 1.0f);
    lightShader->setFloat("pointLights[2].linear", 0.09);
    lightShader->setFloat("pointLights[2].quadratic", 0.032);
    // point light 4
    lightShader->setVec3("pointLights[3].position", pointLightPositions[3]);
    lightShader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    lightShader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    lightShader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    lightShader->setFloat("pointLights[3].constant", 1.0f);
    lightShader->setFloat("pointLights[3].linear", 0.09);
    lightShader->setFloat("pointLights[3].quadratic", 0.032);
    // spotLight
    lightShader->setVec3("spotLight.position", camera->Position);
    lightShader->setVec3("spotLight.direction", camera->Front);
    lightShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    lightShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    lightShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    lightShader->setFloat("spotLight.constant", 1.0f);
    lightShader->setFloat("spotLight.linear", 0.09);
    lightShader->setFloat("spotLight.quadratic", 0.032);
    lightShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    lightShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    lightShader->setMat4("projection", projection);
    lightShader->setMat4("view", view);

    glm::mat4 model;
    lightShader->setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glBindVertexArray(cubeVAO);
    for (unsigned int i = 0; i < 10; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        lightShader->setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    lampShader->use();
    lampShader->setMat4("projection", projection);
    lampShader->setMat4("view", view);

    glBindVertexArray(lightVAO);
    for (unsigned int i = 0; i < 4; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader->setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    base::display();
}

void MultiLights::onKey(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_ESCAPE: {
                glfwSetWindowShouldClose(m_pWindow, true);
                return;
            }
            case GLFW_KEY_W:
                camera->ProcessKeyboard(FORWARD, deltaTime);
                break;
            case GLFW_KEY_S:
                camera->ProcessKeyboard(BACKWARD, deltaTime);
                break;
            case GLFW_KEY_A:
                camera->ProcessKeyboard(LEFT, deltaTime);
                break;
            case GLFW_KEY_D:
                camera->ProcessKeyboard(RIGHT, deltaTime);
                break;
        }
    }

    base::onKey(key, scancode, action, mods);
}

void MultiLights::onMouse(double xpos, double ypos) {

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

void MultiLights::onScroll(double xoffset, double yoffset) {
    camera->ProcessMouseScroll(yoffset);
}

void MultiLights::finalize() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
}