#include "pch.h"
#include "func_ImGUI.h"
#include "UIParam.h"

void TextBox(const char* text, float width)
{
    string ID = "##TB_" + std::to_string(UIParam::GetID());
    ImGui::BeginChild(ID.c_str(), ImVec2(width, 15));
    ImGui::Text("%s", text);
    ImGui::EndChild();
}

ImVec2 Vec2ToImVec2(Vec2 _vec)
{
    return ImVec2(_vec.x, _vec.y);
}