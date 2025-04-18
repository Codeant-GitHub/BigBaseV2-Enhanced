/*
 * 免责声明 / Disclaimer
 *
 * 本项目由 Codeant 开源，仅供学习与研究使用，**禁止任何形式的商业用途**。
 * 使用本代码所造成的任何后果由使用者承担，Codeant 不承担任何责任。
 *
 * This project is open-sourced by Codeant and is intended for **non-commercial use only**.
 * The author shall not be held responsible for any consequences caused by the use of this code.
 *
 * — Codeant
 */
#pragma once

#include <imgui.h>
#include <string.h>

namespace ImGui
{
	template<typename T>
	bool Bitfield(const char* name, T* param)
	{
		bool ret_val = false;
		ImGuiDataType type;
		size_t size = sizeof(T);
		std::string format;
		if (size == 1)
		{
			type = ImGuiDataType_U8;
			format = "%02X";
		}
		if (size == 2)
		{
			type = ImGuiDataType_U16;
			format = "%04X";
		}
		if (size == 4)
		{
			type = ImGuiDataType_U32;
			format = "%08X";
		}
		if (size == 8)
		{
			type = ImGuiDataType_U64;
			format = "%p";
		}
		
		if (ImGui::InputScalar(name, type, param, NULL, NULL, format.c_str(), ImGuiInputTextFlags_CharsHexadecimal))
			ret_val = true;
		size_t bits = (size * 8) - 1ULL;
		for (size_t i = bits; i < SIZE_MAX; i--)
		{
			if (i % 4 == 0)
				ImGui::BeginGroup();
			if (*param & (1ULL << i))
			{
				ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32_BLACK_TRANS);
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(41, 134, 204, 255));
				if (ImGui::Button(std::string("1##").append(std::to_string(i)).append(name).c_str(), ImVec2(25, 0)))
				{
					ret_val = true;
					*param ^= (1ULL << i);
				}
				ImGui::PopStyleColor(2);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32_BLACK_TRANS);
				if (ImGui::Button(std::string("0##").append(std::to_string(i)).append(name).c_str(), ImVec2(25, 0)))
				{
					ret_val = true;
					*param ^= (1ULL << i);
				}
				ImGui::PopStyleColor();
			}
			if (i % 4 == 0)
			{
				ImGui::Text("%i", i);
				ImGui::EndGroup();
			}
			if(i % 16 != 0)
				ImGui::SameLine();
		}
		
		return ret_val;
	}
}