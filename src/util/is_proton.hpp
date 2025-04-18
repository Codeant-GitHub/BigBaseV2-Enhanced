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
#include "memory/module.hpp"

namespace big
{
	inline bool is_proton()
	{
		static auto module = memory::module("ntdll.dll");

		const auto env_no_color = std::getenv("NO_COLOR");
		return module.get_export("wine_get_version") || (env_no_color && strlen(env_no_color));
	}
}
