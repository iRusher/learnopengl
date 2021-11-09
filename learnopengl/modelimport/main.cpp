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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

void process_input(GLFWwindow *window);

int main(int argc, const char * argv[]) {
    
    GLFWwindow* window;
    

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
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
    
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile("./diablo3_pose.obj", aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "import error" << std::endl;
    }

    aiNode *rootNode = scene->mRootNode;
    for (int i = 0; i < rootNode->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[rootNode->mMeshes[i]];
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
