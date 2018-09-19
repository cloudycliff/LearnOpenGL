//
// Created by cliff on 2018/8/15.
//

#include "vapp.h"

void GinApplication::window_size_callback(GLFWwindow* window, int width, int height) {
    auto * pThis = (GinApplication*)glfwGetWindowUserPointer(window);

    pThis->resize(width, height);
}

void GinApplication::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto * pThis = (GinApplication*)glfwGetWindowUserPointer(window);

    pThis->onKey(key, scancode, action, mods);
}

void GinApplication::char_callback(GLFWwindow* window, unsigned int codepoint) {
    auto * pThis = (GinApplication*)glfwGetWindowUserPointer(window);

    pThis->onChar(codepoint);
}

void GinApplication::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    auto * pThis = (GinApplication*)glfwGetWindowUserPointer(window);

    pThis->onMouse(xpos, ypos);
}

void GinApplication::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto * pThis = (GinApplication*)glfwGetWindowUserPointer(window);

    pThis->onScroll(xoffset, yoffset);
}

void GinApplication::init(const char *title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_pWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, nullptr, nullptr);

    glfwMakeContextCurrent(m_pWindow);
    glfwSetWindowUserPointer(m_pWindow, this);
    glfwSetFramebufferSizeCallback(m_pWindow, window_size_callback);
    glfwSetKeyCallback(m_pWindow, key_callback);
    glfwSetCharCallback(m_pWindow, char_callback);
    glfwSetCursorPosCallback(m_pWindow, mouse_callback);
    glfwSetScrollCallback(m_pWindow, scroll_callback);

    glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    //resize(SCR_WIDTH, SCR_HEIGHT);
}