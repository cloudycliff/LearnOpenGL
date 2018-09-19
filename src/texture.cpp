//
// Created by cliff on 2018/9/19.
//

#include "vapp.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "texture.h"
#include "shader.h"

class Texture : public GinApplication {
public:
    typedef class GinApplication base;

    static GinApplication *Create(void) {
        return (s_app = new Texture);
    }

    virtual void init(const char *title);
    virtual void display();
    virtual void onKey(int key, int scancode, int action, int mods);
    virtual void finalize();

    Shader *ourShader;
    unsigned int VBO, VAO, EBO;
    unsigned int texture1, texture2;

};

GinApplication *GinApplication::s_app;

void GinApplication::mainLoop() {
    do {
        display();
        glfwPollEvents();
    } while (!glfwWindowShouldClose(m_pWindow));
}

int main(int argc, char **argv) {
    GinApplication *app = Texture::Create();

    app->init("Texture");
    app->mainLoop();
    app->finalize();

    return 0;
}

void Texture::init(const char *title) {
    base::init(title);

    ourShader = new Shader("shaders/texture_vs.glsl", "shaders/texture_fs.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load and create texture
    texture1 = loadTexture("resources/container.jpg");
    texture2 = loadTexture("resources/awesomeface.png");

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    ourShader->use();
    ourShader->setInt("texture1", 0);
    ourShader->setInt("texture2", 1);

}

void Texture::display() {
    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // render container
    ourShader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    base::display();
}

void Texture::onKey(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE: {
                glfwSetWindowShouldClose(m_pWindow, true);
                return;
            }
        }
    }

    base::onKey(key, scancode, action, mods);
}

void Texture::finalize() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
}