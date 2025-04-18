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
#include "detour_hook.hpp"
#include "memory/handle.hpp"
#include <MinHook.h>

namespace big
{
	detour_hook::detour_hook(std::string name, void *target, void *detour) :
		m_name(std::move(name)),
		m_target(target),
		m_detour(detour)
	{
		fix_hook_address();

		if (auto status = MH_CreateHook(m_target, m_detour, &m_original); status == MH_OK)
		{
			LOG(INFO) << "Created hook '" << m_name << "'.";
		}
		else
		{
			throw std::runtime_error(fmt::format("Failed to create hook '{}' at 0x{:X} (error: {})", m_name, reinterpret_cast<std::uintptr_t>(m_target), MH_StatusToString(status)));

		}
	}

	detour_hook::~detour_hook() noexcept
	{
		if (m_target)
		{
			MH_RemoveHook(m_target);
		}

		LOG(INFO) << "Removed hook '" << m_name << "'.";
	}

	void detour_hook::enable()
	{
		if (auto status = MH_EnableHook(m_target); status == MH_OK)
		{
			LOG(INFO) << "Enabled hook '" << m_name << "'.";
		}
		else
		{
			throw std::runtime_error(fmt::format("Failed to enable hook 0x{:X} ({})", reinterpret_cast<std::uintptr_t>(m_target), MH_StatusToString(status)));
		}
	}

	void detour_hook::disable()
	{
		if (auto status = MH_DisableHook(m_target); status == MH_OK)
		{
			LOG(INFO) << "Disabled hook '" << m_name << "'.";
		}
		else
		{
			LOG(WARNING) << "Failed to disable hook '" << m_name << "'.";
		}
	}

	DWORD exp_handler(PEXCEPTION_POINTERS exp, std::string const &name)
	{
		return exp->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION
			? EXCEPTION_EXECUTE_HANDLER
			: EXCEPTION_CONTINUE_SEARCH;
	}

	void detour_hook::fix_hook_address()
	{
		__try
		{
			auto ptr = memory::handle(m_target);
			while (ptr.as<std::uint8_t&>() == 0xE9)
			{
				ptr = ptr.add(1).rip();
			}

			m_target = ptr.as<void*>();
		}
		__except (exp_handler(GetExceptionInformation(), m_name))
		{
			[this]()
			{
				throw std::runtime_error(fmt::format("Failed to fix hook address for '{}'", m_name));
			}();
		}
	}
}
