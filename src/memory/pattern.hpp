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
#include "fwddec.hpp"
#include "handle.hpp"

#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace memory
{
	class pattern
	{
		friend pattern_batch;
		friend range;

	public:
		pattern(std::string_view ida_sig);

		inline pattern(const char* ida_sig) :
		    pattern(std::string_view(ida_sig))
		{
		}

		std::vector<std::optional<uint8_t>> m_bytes;
	};
}
