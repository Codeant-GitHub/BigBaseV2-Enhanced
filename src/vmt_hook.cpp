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
#include "common.hpp"
#include "vmt_hook.hpp"

namespace big
{
	vmt_hook::vmt_hook(void* obj, std::size_t num_funcs) :
		m_object(static_cast<void***>(obj)),
		m_num_funcs(num_funcs + 1),
		m_original_table(*m_object),
		m_new_table(std::make_unique<void* []>(m_num_funcs))
	{
		std::copy_n(m_original_table - 1, m_num_funcs, m_new_table.get());
	}

	void vmt_hook::hook(std::size_t index, void* func)
	{
		m_new_table[index + 1] = func;
	}

	void vmt_hook::unhook(std::size_t index)
	{
		m_new_table[index + 1] = m_original_table[index];
	}

	void vmt_hook::enable()
	{
		*m_object = m_new_table.get() + 1;
	}

	void vmt_hook::disable()
	{
		*m_object = m_original_table;
	}
}
