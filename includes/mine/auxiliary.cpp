#include "auxiliary.h"
#include <stdio.h>
#include <string>
#include <iostream>
#ifdef _WIN32

#include <windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include "../GLFW/glfw3native.h"
#pragma comment(lib, "dwmapi.lib")
#include <Psapi.h>
#endif

bool windowResized = false;

int WINDOW_WIDTH = 0;
int WINDOW_HEIGHT = 0;

char *OpenGL_Vendor = nullptr;
char *OpenGL_Renderer = nullptr;
char *OpenGL_Version = nullptr;
char *GLSL_Version = nullptr;

void resizeFunc(GLFWwindow *window, int w, int h)
{
    glViewport(0, 0, w, h);
    windowResized = true;
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
}

GLFWwindow *initGLFWGLAD()
{
    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);

    GLFWwindow *window = glfwCreateWindow(1080, 720, "Shadow Mapping", nullptr, nullptr);
    WINDOW_WIDTH = 1080;
    WINDOW_HEIGHT = 720;

    if (!window)
        return nullptr;

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return nullptr;

    loadOpenGLInfo();

    glfwSetFramebufferSizeCallback(window, resizeFunc);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(MessageCallback, 0);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    return window;
}

#ifdef _WIN32

void setCaptionColor(COLORREF color)
{
    HWND hwnd = glfwGetWin32Window(glfwGetCurrentContext());

   DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &color, sizeof(color));
}

size_t getRAMUsage()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if(GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc)))
    {
        return pmc.WorkingSetSize;
    }
    return 0;
}

#endif

void loadOpenGLInfo()
{
    const auto *glVendorPtr = glGetString(GL_VENDOR);
    const auto *glRendererPtr = glGetString(GL_RENDERER);
    const auto *glVersionPtr = glGetString(GL_VERSION);
    const auto *glGLSLVersionPtr = glGetString(GL_SHADING_LANGUAGE_VERSION);

    int ptrLength = 0;

    while (glVendorPtr[ptrLength++])
        ;

    OpenGL_Vendor = new char[ptrLength + 1];
    OpenGL_Vendor[ptrLength] = 0;

    for (int i = 0; i < ptrLength; ++i)
        OpenGL_Vendor[i] = (char)glVendorPtr[i];

    ptrLength = 0;

    while (glRendererPtr[ptrLength++])
        ;

    OpenGL_Renderer = new char[ptrLength + 1];
    OpenGL_Renderer[ptrLength] = 0;

    for (int i = 0; i < ptrLength; ++i)
        OpenGL_Renderer[i] = (char)glRendererPtr[i];

    ptrLength = 0;

    while (glVersionPtr[ptrLength++])
        ;

    OpenGL_Version = new char[ptrLength + 1];
    OpenGL_Version[ptrLength] = 0;

    for (int i = 0; i < ptrLength; ++i)
        OpenGL_Version[i] = (char)glVersionPtr[i];

    ptrLength = 0;

    while (glGLSLVersionPtr[ptrLength++])
        ;

    GLSL_Version = new char[ptrLength + 1];
    GLSL_Version[ptrLength] = 0;

    for (int i = 0; i < ptrLength; ++i)
        GLSL_Version[i] = (char)glGLSLVersionPtr[i];
}

void terminateApp()
{
    terminateImGui();
    terminateGLFW(glfwGetCurrentContext());

    delete[] OpenGL_Vendor;
    delete[] OpenGL_Renderer;
    delete[] OpenGL_Version;
    delete[] GLSL_Version;
}

ImGuiIO &initImGui(GLFWwindow *window)
{
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    ImGui::StyleColorsDark();
    return io;
}

void newFrame()
{
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}

void renderFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void terminateImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool leftAltPressed = false, enterPressed = false, leftCtrlPressed = false;

bool rightClickCurrentFrame = false, rightClickLastFrame = false;

bool toggleCursorCaptured = false;

void proccesEvents(GLFWwindow *window)
{
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);

    leftAltPressed = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
    enterPressed = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;

    if (leftAltPressed && enterPressed)
        glfwMaximizeWindow(window);

    leftCtrlPressed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;

    if (leftCtrlPressed && enterPressed)
        glfwSetWindowSize(window, 1080, 720);

    rightClickCurrentFrame = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;

    if (rightClickCurrentFrame && !rightClickLastFrame)
        toggleCursorCaptured = !toggleCursorCaptured;

    if (toggleCursorCaptured)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
     

    rightClickLastFrame = rightClickCurrentFrame;
}

void terminateGLFW(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar *message,
                                const void *userParam)
{
    std::string severityStr;
    if (severity == GL_DEBUG_SEVERITY_HIGH)
        severityStr = "GL_ERROR";
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
        severityStr = "GL_WARNING";
    else if (severity == GL_DEBUG_SEVERITY_LOW)
        severityStr = "GL_INFO";
    else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        severityStr = "GL_NOTIFICATION";

    std::cout << severityStr << " [type=0x" << std::hex << type
              << ", severity=0x" << severity << "]: " << message << std::endl;
}
