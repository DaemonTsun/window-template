
#pragma once

#include "imgui.h"

namespace FsUi
{
void Init();
void Exit();

bool Filepicker(const char *label, char *buf, size_t buf_size, ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll, ImGuiInputTextCallback callback = nullptr, void *user_data = nullptr);
}

