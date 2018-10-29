//
// Created by cliff on 2018/9/19.
//

#include "vapp.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Triangle : public GinApplication {
public:
    typedef class GinApplication base;

    static GinApplication *Create(void) {
        return (s_app = new Triangle);
    }

    virtual void init(const char *title);
    virtual void display();
    virtual void onKey(int key, int scancode, int action, int mods);
    virtual void finalize();

    Shader *ourShader;
    unsigned int VBO, VAO, EBO;

};

GinApplication *GinApplication::s_app;

void GinApplication::mainLoop() {

    bool init = false;

    do {
        display();
        glfwPollEvents();

        if(!init)
        {
            int x,y;
            glfwGetWindowPos(m_pWindow, &x, &y);
            glfwSetWindowPos(m_pWindow, x+1, y);

            init = true;
        }
    } while (!glfwWindowShouldClose(m_pWindow));
}

int main(int argc, char **argv) {
    GinApplication *app = Triangle::Create();

    app->init("Triangle");
    app->mainLoop();
    app->finalize();

    return 0;
}

void Triangle::init(const char *title) {
    base::init(title);

    ourShader = new Shader("shaders/triangle_vs.glsl", "shaders/triangle_fs.glsl");

    float vertices[] = {
            0.5f,  0.5f, 0.0f,// top right
            0.5f, -0.5f, 0.0f,// bottom right
            -0.5f, -0.5f, 0.0f,// bottom left
            -0.5f,  0.5f, 0.0f,// top left
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //index, size, data type, normalized, stride, pointer offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void Triangle::display() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    base::display();
}

void Triangle::onKey(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE: {
                glfwSetWindowShouldClose(m_pWindow, true);
                return;
            }
            case GLFW_KEY_W: {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            }
            case GLFW_KEY_F: {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            }
        }
    }

    base::onKey(key, scancode, action, mods);
}

void Triangle::finalize() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
}