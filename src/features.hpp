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
	class featuress
	{
	public:
		static bool CheckForMultipleVersionDLL();
		static void anticheat_fsl_init();

		static void script_init();
		static void script_on_tick();
		static void script_func();
	};

	inline featuress g_features;
}
