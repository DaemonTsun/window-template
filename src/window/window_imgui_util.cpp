
#include "shl/print.hpp"
#include "shl/time.hpp"
#include "shl/at_exit.hpp"

#include "GLFW/glfw3.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "imgui.h"

#include "window/colorscheme.hpp"
#include "window/window_imgui_util.hpp"

static const char *_glsl_version = "#version 330";

static void _glfw_error_callback(int error, const char *description)
{
    tprint(stderr_handle(), "GLFW Error %d: %s\n", error, description);
    breakpoint();
}

void window_init()
{
    glfwSetErrorCallback(_glfw_error_callback);

    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void window_exit()
{
    glfwTerminate();
}

GLFWwindow *create_window(const char *title, int width, int height)
{
    GLFWwindow *ret = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (ret == nullptr)
        return nullptr;

    glfwMakeContextCurrent(ret);
    glfwSwapInterval(1);

    return ret;
}

void close_window(GLFWwindow *window)
{
    glfwSetWindowShouldClose(window, true);
}

void destroy_window(GLFWwindow *window)
{
    glfwDestroyWindow(window);
}

float get_window_scaling(GLFWwindow *window)
{
    float sx;
    float sy;
    glfwGetWindowContentScale(window, &sx, &sy);

    return sx > sy ? sx : sy;
}

void set_window_keyboard_callback(GLFWwindow *window, keyboard_callback cb)
{
    glfwSetKeyCallback(window, cb);
}

void default_render_function(GLFWwindow *window, double dt)
{
    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void window_event_loop(GLFWwindow *window, event_loop_update_callback update, event_loop_render_callback render, double min_fps)
{
    double dt;
    timespan start;
    timespan now;

    get_time(&start);
    now = start;

    while (!glfwWindowShouldClose(window))
    {
        if (min_fps > 0.0)
            glfwWaitEventsTimeout(1.0 / min_fps);
        else
            glfwPollEvents();

        get_time(&now);
        dt = get_seconds_difference(&start, &now);

        update(window, dt);
        render(window, dt);
    }
}

void ui_init(GLFWwindow *window)
{
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true /* install callbacks */);
    ImGui_ImplOpenGL3_Init(_glsl_version);

    colorscheme_init();
}

void ui_exit(GLFWwindow *window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    colorscheme_free();
}

void ui_new_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ui_end_frame()
{
    ImGui::EndFrame();
}

void ui_set_next_window_full_size()
{
#ifdef IMGUI_HAS_VIEWPORT
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
#else 
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif
}
