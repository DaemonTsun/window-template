
#pragma once

struct GLFWwindow;

void window_init();
void window_exit();

GLFWwindow *window_create(const char *title, int width, int height);
void window_close(GLFWwindow *window);
void window_destroy(GLFWwindow *window);

float window_get_scaling(GLFWwindow *window);
void window_set_size(GLFWwindow *window, int width, int height);
void window_get_size(GLFWwindow *window, int *width, int *height);
void window_set_position(GLFWwindow *window, int x, int y);
void window_get_position(GLFWwindow *window, int *x, int *y);
void window_maximize(GLFWwindow *window);
void window_restore(GLFWwindow *window);
bool window_is_maximized(GLFWwindow *window);

// events
typedef void (*keyboard_callback)(GLFWwindow *window, int key, int scancode, int action, int mods);
void window_set_keyboard_callback(GLFWwindow *window, keyboard_callback cb);

typedef void (*event_loop_update_callback)(GLFWwindow *, double);
typedef void (*event_loop_render_callback)(GLFWwindow *, double);

// basically just renders the UI
void default_render_function(GLFWwindow *window, double dt);

void window_event_loop(GLFWwindow *window
                     , event_loop_update_callback update
                     , event_loop_render_callback render = default_render_function
                     , double min_fps = -1.0);

// UI
void imgui_init(GLFWwindow *window);
void imgui_exit(GLFWwindow *window);

void imgui_new_frame();
void imgui_end_frame();
void imgui_set_next_window_full_size();

unsigned int imgui_hash(const char *str);
void imgui_push_override_id(unsigned int id);
void imgui_pop_id();

// use when opening & beginning of a popup are in different ID stacks
void imgui_open_global_popup(const char *id);

#define if_imgui_begin_global_popup(Id, ...) \
    imgui_push_override_id(imgui_hash(Id));\
    if constexpr (defer { imgui_pop_id(); }; true)\
    if (ImGui::BeginPopup(Id __VA_OPT__(,) __VA_ARGS__))

#define if_imgui_begin_global_modal_popup(Id, ...) \
    imgui_push_override_id(imgui_hash(Id));\
    if constexpr (defer { imgui_pop_id(); }; true)\
    if (ImGui::BeginPopupModal(Id __VA_OPT__(,) __VA_ARGS__))

