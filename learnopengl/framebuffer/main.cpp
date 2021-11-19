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

#include "glcheck.h"

struct RenderPassInfo {

    Shader *lightingShader;
    unsigned int cubeVAO;
    unsigned int texture1;
    unsigned int texture2;

    Shader *lightCubeShader;
    unsigned int lightCubeVAO;

    glm::vec3 *pointLightPositions;
    glm::vec3 *cubePositions;

    float *vertices;
    unsigned int verticesCount;

    unsigned int quadVAO;
    float *quadVertices;
    unsigned int quadCnt;
    Shader *quadShader;

    unsigned int FBO;
    unsigned int RBO;
    unsigned int fTexture;
    unsigned int fDepthTxt;

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

float viewportWidth = 818;
float viewportHeight = 703;

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

    std::cout << "window size : " << SCR_WIDTH << "," << SCR_HEIGHT << std::endl;

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
    Shader quadShader("quad.vs", "quad.fs");

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

    float quadVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
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


    unsigned int quadVBO,quadVAO;
    glGenVertexArrays(1,&quadVAO);
    glGenBuffers(1,&quadVBO);

    glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);

    glBindVertexArray(quadVAO);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4 *sizeof(float),(void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4 *sizeof(float), (void *)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    std::cout << "init vierport size : " << viewportWidth << "," << viewportHeight << std::endl;
    std::cout << "framebuffer texture size : " << viewportWidth  << "," << viewportHeight << std::endl;

    // 创建framebuffer
    unsigned int FBO;
    glGenFramebuffers(1,&FBO);
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);

    unsigned int fTexture;
    glGenTextures(1,&fTexture);
    glBindTexture(GL_TEXTURE_2D,fTexture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,viewportWidth * 2,viewportHeight * 2,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fTexture,0);

    unsigned int fDepthTxt;
    glGenTextures(1,&fDepthTxt);
    glBindTexture(GL_TEXTURE_2D,fDepthTxt);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,viewportWidth * 2,viewportHeight * 2,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE,NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,fDepthTxt,0);

    unsigned int RBO;
//    glGenRenderbuffers(1,&RBO);
//    glBindRenderbuffer(GL_RENDERBUFFER,RBO);
//    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,viewportWidth * 2,viewportHeight * 2);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "framebuffer is not complete" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int texture1 = loadTexture("container.png");
    unsigned int texture2 = loadTexture("container2_specular.png");
    lightingShader.use();
    lightingShader.setInt("material.diffuse",0);
    lightingShader.setInt("material.specular",1);


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

    context.quadVAO = quadVAO;
    context.quadVertices = quadVertices;
    context.quadCnt = sizeof(quadVertices);
    context.quadShader = &quadShader;

    context.FBO = FBO;
    context.RBO = RBO;
    context.fTexture = fTexture;
    context.fDepthTxt = fDepthTxt;


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

    std::cout << "framebuffer callback size : " << width << "," << height << std::endl;

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
    glDisable(GL_SCISSOR_TEST);

    static float viewportWidthS = 0;
    static float viewportHeightS = 0;
    static ImRect rectS;

    ImRect rect = renderPassInfo->rect;
    viewportWidth = rect.GetWidth();
    viewportHeight = rect.GetHeight();

//    if ( (viewportWidthS != viewportWidth)
//        || viewportHeightS != viewportHeight
//        || rectS.Min.x != rect.Min.x
//        || rectS.Min.y != rect.Min.y
//        || rectS.Max.x != rect.Max.x
//        || rectS.Max.y != rect.Max.y ) {
//        rectS = rect;
//        viewportWidthS = viewportWidth;
//        viewportHeightS = viewportHeight;
//        std::cout << "render view size : " << viewportWidth << "," << viewportHeight << std::endl;
//        std::cout << "rect " << rect.Max.x << "," << rect.Max.y << ";" << rect.Min.x << "," << rect.Min.y << std::endl;
//        std::cout << "rectS " << rectS.Max.x << "," << rectS.Max.y << ";" << rectS.Min.x << "," << rectS.Min.y << std::endl;
//        std::cout << "render view position " << rect.Min.x * 2 << "," << SCR_HEIGHT - rect.Max.y << ";" << viewportWidth << "," << viewportHeight << std::endl;
//    }

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER,renderPassInfo->FBO));
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glViewport(0,0 ,viewportWidth*2,viewportHeight*2);


//    glViewport(rect.Min.x * 2, (SCR_HEIGHT - rect.Max.y) * 2 ,viewportWidth*2,viewportHeight*2);
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

    glm::mat4 model = glm::mat4(1.0f);
    for (int i = 1; i < 10; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, renderPassInfo->cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        renderPassInfo->lightingShader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, renderPassInfo->verticesCount);
    }

    model = glm::mat4(1.0f);
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

    glBindFramebuffer(GL_FRAMEBUFFER,0);

    glViewport(rect.Min.x * 2, (SCR_HEIGHT - rect.Max.y) * 2 ,viewportWidth*2,viewportHeight*2);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    renderPassInfo->quadShader->use();
    glBindVertexArray(renderPassInfo->quadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,renderPassInfo->fDepthTxt);
//    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    GL_CHECK(glDrawArrays(GL_TRIANGLES,0,6));
//    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

//    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);
    glActiveTexture(last_active_texture);
}

