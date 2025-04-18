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
#include "pointers.hpp"

namespace big
{
	struct sc_pointers_layout_info
	{
		// get the beginning and the end of what we need to save / load
		inline static constexpr size_t offset_of_cache_begin_field = offsetof(big::pointers, m_sc);
		inline static constexpr size_t offset_of_cache_end_field = offsetof(big::pointers, m_sc) + sizeof(socialclub_pointers);
		inline static constexpr size_t field_count = (offset_of_cache_end_field - offset_of_cache_begin_field) / sizeof(void*);
	};
}
