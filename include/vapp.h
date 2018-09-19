//
// Created by cliff on 2018/8/15.
//

#ifndef GINOPENGL_VAPP_H
#define GINOPENGL_VAPP_H

#include "vgl.h"

class GinApplication {
protected:
    inline GinApplication() {}
    virtual ~GinApplication() {}

    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void char_callback(GLFWwindow* window, unsigned int codepoint);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    static GinApplication *s_app;
    GLFWwindow *m_pWindow;

    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

public:
    void mainLoop();

    virtual void init(const char *title);

    virtual void display() {
        glfwSwapBuffers(m_pWindow);
    }

    virtual void resize(int width, int height) {
        glViewport(0, 0, width, height);
    }

    virtual void onKey(int key, int scancode, int action, int mods) {}
    virtual void onChar(unsigned int codepoint) {}
    virtual void onMouse(double xpos, double ypos) {}
    virtual void onScroll(double xoffset, double yoffset) {}

    virtual void finalize() {}
};

#endif //GINOPENGL_VAPP_H
