//  Contains the callback functions

#include "Functions.h"

//
//  Whenever the window size is changed (automatically by OS, or manually by user) this function is called
//
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

//
//  Processes all input
//  When input keys are pressed or camera is moved, this function reacts acccordingly
//
void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta_time);
}

//
//  Whenever the mouse moves, this function is called  
//
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {
    if (first_mouse) {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;

    last_x = x_pos;
    last_y = y_pos;

    if (mouse_click_active)
        camera.ProcessMouseMovement(x_offset, y_offset);
}

//
//  Activates mouse when left button is clicked
//
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mouse_click_active = true;
    else
        mouse_click_active = false;
}

//
//  Whenever mouse scroll wheel is used, this function is called
//
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    camera.ProcessMouseScroll(y_offset);
}

