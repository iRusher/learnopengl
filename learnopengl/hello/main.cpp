//
//  main.cpp
//  cpplearn
//
//  Created by Rusher on 2020/4/8.
//  Copyright © 2020 Rusher. All rights reserved.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void process_input(GLFWwindow *window);

int main(int argc, const char * argv[]) {
    
    GLFWwindow* window;
    

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
//    https://www.glfw.org/docs/3.3/window_guide.html
    
//    int width = 0;
//    int height = 0;
//    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
//    std::cout << mode->width << " " << mode->height << std::endl;
    
    /* Create a windowed mode window and its OpenGL context  */
    window = glfwCreateWindow(640, 480, "Hello Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    
    // Initialize the OpenGL API with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        process_input(window);
        
        /* Render here */
        glClearColor(1, 0, 0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
