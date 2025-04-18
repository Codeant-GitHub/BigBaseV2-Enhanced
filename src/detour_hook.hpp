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

namespace big
{
	class detour_hook
	{
	public:
		explicit detour_hook(std::string name, void* target, void* detour);
		~detour_hook() noexcept;

		detour_hook(detour_hook&& that) = delete;
		detour_hook& operator=(detour_hook&& that) = delete;
		detour_hook(detour_hook const&) = delete;
		detour_hook& operator=(detour_hook const&) = delete;

		void enable();
		void disable();

		template <typename T>
		T get_original();

		void fix_hook_address();
	private:
		std::string m_name;
		void* m_target;
		void* m_detour;
		void* m_original{};
	};

	template <typename T>
	inline T detour_hook::get_original()
	{
		return static_cast<T>(m_original);
	}
}
