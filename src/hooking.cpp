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
#include "function_types.hpp"
#include "gui.hpp"
#include "hooking.hpp"
#include "memory/module.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script_mgr.hpp"
#include "gta_util.hpp"

#include <MinHook.h>

namespace big
{
	void* run_script_threads_t;

	void Detour(uintptr_t address, void* detour, void** original, const char* name) {
		MH_CreateHook(
			(void*)address,
			detour,
			original
		);
		MH_EnableHook((void*)address);
	}

	void Initialize()
	{
		MH_Initialize();

		Detour(
			g_pointers->m_gta.m_run_script_threads,
			&hooks::run_script_threads,
			(void**)(&run_script_threads_t),
			"Script Vm"
		);
	}

	hooking::hooking()
	{
		Initialize();
		g_hooking = this;
	}

	hooking::~hooking()
	{
		if (m_enabled)
			disable();

		g_hooking = nullptr;
	}

	void hooking::enable()
	{
		m_enabled = true;
	}

	void hooking::disable()
	{
		m_enabled = false;

		if (m_main_persistent_hook)
		{
			m_main_persistent_hook->disable();
		}
	}

	minhook_keepalive::minhook_keepalive()
	{
		MH_Initialize();
	}

	minhook_keepalive::~minhook_keepalive()
	{
		MH_Uninitialize();
	}

	static uint32_t this_script_Count;
	bool hooks::run_script_threads(__int64 a1, __int64** a2, unsigned __int64* a3, __int64 a4)
	{
		uint32_t script_count = *g_pointers->m_gta.m_frame_count;

		if (this_script_Count != script_count)
		{
			if (g_running)
			{
				auto thread = gta_util::find_script_thread("freemode"_J);
				if (!thread)
					thread = gta_util::find_script_thread("main_persistent"_J);
				if (!thread)
					thread = gta_util::find_script_thread("startup"_J);
				if (thread)

					gta_util::execute_as_script(thread, [&]
						{
							g_script_mgr.tick();
						});
			}

			this_script_Count = script_count;
		}

		return static_cast<decltype(&run_script_threads)>(run_script_threads_t)(a1, a2, a3, a4);
	}
}
