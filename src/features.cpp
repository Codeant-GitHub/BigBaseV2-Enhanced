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
#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "gui.hpp"
#include "memory/module.hpp"
#include "memory/pattern.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "features.hpp"
#include "renderer.hpp"
#include "script.hpp"

#include <psapi.h>

namespace big
{
	bool m_IsFSLLoaded = false;
	bool m_BattlEyeRunning = false;

	bool featuress::CheckForMultipleVersionDLL()
	{
		DWORD processId = GetCurrentProcessId();
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
		if (hProcess == NULL)
		{
			LOG(INFO) << "Failed to open process.";
			return false;
		}

		HMODULE hModules[1024];
		DWORD cbNeeded;
		if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded))
		{
			int num_versions = 0;

			for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
			{
				char moduleName[MAX_PATH];
				if (GetModuleFileNameExA(hProcess, hModules[i], moduleName, sizeof(moduleName) / sizeof(char)))
				{
					if (strstr(moduleName, "version.dll") != nullptr)
					{
						num_versions++;
					}
				}
			}

			LOG(INFO) << "Found " << num_versions << " instances of version.dll.";

			return num_versions > 0;
		}
		else
		{
			LOG(INFO) << "Failed to enumerate process modules.";
			CloseHandle(hProcess);
			return false;
		}

		CloseHandle(hProcess);
	}

	void featuress::anticheat_fsl_init()
	{
		m_IsFSLLoaded = CheckForMultipleVersionDLL();

		m_BattlEyeRunning = (NETWORK::_NETWORK_GET_GAME_RESTART_REASON() == 0 && GetModuleHandleA("BEClient_x64.dll")) && !m_IsFSLLoaded;

		const char* mode = "Codeant";

		if (m_BattlEyeRunning)
			mode = "Legit BattlEye";
		else if (m_IsFSLLoaded)
			mode = "FSL";

		LOGF(INFO, "Anticheat bypass mode: {}", mode);

		if (!m_IsFSLLoaded)
			memory::byte_patch::make(g_pointers->m_gta.m_battleye_status_update_bypass.sub(0x26).as<std::uint8_t*>(), 0xC3)->apply();
	}

	void featuress::script_init()
	{
		LOG(INFO) << "11111111111";
		LOG(INFO) << PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID());
	}

	void featuress::script_on_tick()
	{
		if (g_pointers->m_gta.m_is_session_started)
		{
			if (!m_IsFSLLoaded)
			{
				*g_pointers->m_gta.BERestartStatus = 0;
				*g_pointers->m_gta.NeedsBERestart = false;
				*g_pointers->m_gta.IsBEBanned = false;
			}
		}
	}

	void featuress::script_func()
	{
		g_features.anticheat_fsl_init();
		g_features.script_init();

		while (true)
		{
			g_features.script_on_tick();
			Script::get_current()->yield();
		}
	}
}
