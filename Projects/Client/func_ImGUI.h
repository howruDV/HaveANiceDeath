#pragma once
#include "imgui.h"

void TextBox(const char* text, float width = 130.f);
void TextBox(const char* text, int id, float width = 130.f);
ImVec2 Vec2ToImVec2(Vec2 _vec);