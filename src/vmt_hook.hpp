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

namespace big
{
	class vmt_hook
	{
	public:
		explicit vmt_hook(void* obj, std::size_t num_funcs);

		vmt_hook(vmt_hook&& that) = delete;
		vmt_hook& operator=(vmt_hook&& that) = delete;
		vmt_hook(vmt_hook const&) = delete;
		vmt_hook& operator=(vmt_hook const&) = delete;

		void hook(std::size_t index, void* func);
		void unhook(std::size_t index);

		template <typename T>
		T get_original(std::size_t index);

		void enable();
		void disable();
	private:
		void*** m_object;
		std::size_t m_num_funcs;

		void** m_original_table;
		std::unique_ptr<void* []> m_new_table;
	};

	template<typename T>
	inline T vmt_hook::get_original(std::size_t index)
	{
		return static_cast<T>(m_original_table[index]);
	}
}
