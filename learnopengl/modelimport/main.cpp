#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "shader_m.h"
#include "camera.h"

#include <iostream>
#include <imgui_internal.h>

#include "Model.h"

struct RenderPassInfo {

    Shader *lightingShader;
    Shader *lightCubeShader;

    unsigned int cubeVAO;
    unsigned int lightCubeVAO;
    unsigned int texture1;
    unsigned int texture2;

    glm::vec3 *pointLightPositions;
    glm::vec3 *cubePositions;

    float *vertices;
    unsigned verticesCount;

    ImRect rect;
    Model *model;
};

RenderPassInfo *gContext;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int loadTexture(const char *path);
void drawCube(RenderPassInfo *renderPassInfo);

// settings
float SCR_WIDTH = 1920.0f;
float SCR_HEIGHT = 1080.0f;

float viewportWidth = 1920.0f;
float viewportHeight = 1080.0f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// IMGUI
void imguiInit(GLFWwindow *window);
void imguiSetup();
float col1[3] = { 1.0f, 0.0f, 0.2f };

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    imguiInit(window);

    std::string path = "./nanosuit/nanosuit.obj";
    Model model(path);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("cube.vs", "cube.fs"); //聚光
    Shader lightCubeShader("light_cube.vs", "light_cube.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            // position nomal uv

            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // textcoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    unsigned int texture1 = loadTexture("container.png");
    unsigned int texture2 = loadTexture("container2_specular.png");

    lightingShader.use();
    lightingShader.setInt("material.diffuse",0);
    lightingShader.setInt("material.specular",1);

//    Shader *lightingShader;
//    Shader *lightCubeShader;
//
//    unsigned int cubeVAO;
//    unsigned int lightCubeVAO;
//    unsigned int texture1;
//    unsigned int texture2;
//
//    glm::vec3 *pointLightPositions;
//    glm::vec3 *cubePositions;
//
//    float *vertices;

    RenderPassInfo context;
    context.lightingShader = &lightingShader;
    context.lightCubeShader = &lightCubeShader;
    context.cubeVAO = cubeVAO;
    context.lightCubeVAO = lightCubeVAO;
    context.texture1 = texture1;
    context.texture2 = texture2;
    context.pointLightPositions = pointLightPositions;
    context.cubePositions = cubePositions;
    context.vertices = vertices;
    context.verticesCount = sizeof(vertices);
    context.model = &model;
    gContext = &context;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        imguiSetup();

        static bool showDemo = true;

        ImGui::ShowDemoWindow(&showDemo);

        ImGui::SetNextWindowSize(ImVec2(414 * 2,375 * 2),ImGuiCond_FirstUseEver);
        ImGui::Begin("RenderViewer");
        ImGui::Text("Test");//至少要添加一个widget，不然不会渲染window
        ImGuiWindow *currentImgWindow = ImGui::GetCurrentContext()->CurrentWindow;
        ImRect rect = currentImgWindow->Rect();
        gContext->rect = rect;
        currentImgWindow->DrawList->AddCallback([](const ImDrawList* parent_list, const ImDrawCmd* cmd){
            drawCube(gContext);
        }, nullptr);
        currentImgWindow->DrawList->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(278,55),ImGuiCond_FirstUseEver);
        ImGui::Begin("Light Cube Color");
        ImGui::ColorEdit3("color 1", col1);
        ImGui::End();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS)
        firstMouse = true;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS) return;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(const char *path) {
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width,height,nrChannels;
    unsigned char * data = stbi_load(path,&width,&height,&nrChannels,0);
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_set_flip_vertically_on_load(true);
    if (data)
    {
        GLint format = GL_RGB;
        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture1;
}

void imguiInit(GLFWwindow *window) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
};


void imguiSetup() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void drawCube(RenderPassInfo *renderPassInfo) {

    GLenum last_active_texture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);

    ImRect rect = renderPassInfo->rect;
    viewportWidth = rect.GetWidth() * 2;
    viewportHeight = rect.GetHeight() * 2;
    glViewport(rect.Min.x * 2, SCR_HEIGHT * 2 - rect.Max.y * 2  ,viewportWidth,viewportHeight);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderPassInfo->lightingShader->use();
    renderPassInfo->lightingShader->setVec3("viewPos", camera.Position);

    // material
    renderPassInfo->lightingShader->setFloat("material.shininess", 32.0);
    glBindVertexArray(renderPassInfo->cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderPassInfo->texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderPassInfo->texture2);

    // lights
    // direction light
    renderPassInfo->lightingShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    renderPassInfo->lightingShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    renderPassInfo->lightingShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    renderPassInfo->lightingShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // point light 1
    renderPassInfo->lightingShader->setVec3("pointLights[0].position", renderPassInfo->pointLightPositions[0]);
    renderPassInfo->lightingShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    renderPassInfo->lightingShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    renderPassInfo->lightingShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    renderPassInfo->lightingShader->setFloat("pointLights[0].constant", 1.0f);
    renderPassInfo->lightingShader->setFloat("pointLights[0].linear", 0.09);
    renderPassInfo->lightingShader->setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    renderPassInfo->lightingShader->setVec3("pointLights[1].position", renderPassInfo->pointLightPositions[1]);
    renderPassInfo->lightingShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    renderPassInfo->lightingShader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    renderPassInfo->lightingShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    renderPassInfo->lightingShader->setFloat("pointLights[1].constant", 1.0f);
    renderPassInfo->lightingShader->setFloat("pointLights[1].linear", 0.09);
    renderPassInfo->lightingShader->setFloat("pointLights[1].quadratic", 0.032);
    // point light 3
    renderPassInfo->lightingShader->setVec3("pointLights[2].position", renderPassInfo->pointLightPositions[2]);
    renderPassInfo->lightingShader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    renderPassInfo->lightingShader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    renderPassInfo->lightingShader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    renderPassInfo->lightingShader->setFloat("pointLights[2].constant", 1.0f);
    renderPassInfo->lightingShader->setFloat("pointLights[2].linear", 0.09);
    renderPassInfo->lightingShader->setFloat("pointLights[2].quadratic", 0.032);
    // point light 4
    renderPassInfo->lightingShader->setVec3("pointLights[3].position", renderPassInfo->pointLightPositions[3]);
    renderPassInfo->lightingShader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    renderPassInfo->lightingShader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    renderPassInfo->lightingShader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    renderPassInfo->lightingShader->setFloat("pointLights[3].constant", 1.0f);
    renderPassInfo->lightingShader->setFloat("pointLights[3].linear", 0.09);
    renderPassInfo->lightingShader->setFloat("pointLights[3].quadratic", 0.032);

    // spotLight
    lightPos = glm::vec3(glm::sin(glfwGetTime() * 2.0),0,3.5f);
    renderPassInfo->lightingShader->setVec3("spotLight.direction", camera.Front);
    renderPassInfo->lightingShader->setVec3("spotLight.position", lightPos);
    renderPassInfo->lightingShader->setVec3("spotLight.ambient", col1[0], col1[1], col1[2]);
    renderPassInfo->lightingShader->setVec3("spotLight.diffuse", col1[0], col1[1], col1[2]);
    renderPassInfo->lightingShader->setVec3("spotLight.specular", col1[0], col1[1], col1[2]);
    renderPassInfo->lightingShader->setFloat("spotLight.constant", 1.0f);
    renderPassInfo->lightingShader->setFloat("spotLight.linear", 0.09);
    renderPassInfo->lightingShader->setFloat("spotLight.quadratic", 0.032);
    renderPassInfo->lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    renderPassInfo->lightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), viewportWidth / viewportHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    renderPassInfo->lightingShader->setMat4("projection", projection);
    renderPassInfo->lightingShader->setMat4("view", view);

    for (int i = 1; i < 10; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, renderPassInfo->cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        renderPassInfo->lightingShader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, renderPassInfo->verticesCount);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(0.0f,-2.0f,0.0f));
    model = glm::scale(model,glm::vec3(0.2,0.2,0.2));
    renderPassInfo->lightingShader->setMat4("model", model);
    renderPassInfo->model->Draw(*renderPassInfo->lightingShader);

    // also draw the lamp object
    renderPassInfo->lightCubeShader->use();
    renderPassInfo->lightCubeShader->setMat4("projection", projection);
    renderPassInfo->lightCubeShader->setMat4("view", view);

    for (int i = 0; i < renderPassInfo->pointLightPositions->length(); ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, renderPassInfo->pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        renderPassInfo->lightCubeShader->setMat4("model", model);
        renderPassInfo->lightCubeShader->setVec3("LightCubeColor", col1[0], col1[1], col1[2]);
        glBindVertexArray(renderPassInfo->lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, renderPassInfo->verticesCount);
    }

    glActiveTexture(last_active_texture);
}

