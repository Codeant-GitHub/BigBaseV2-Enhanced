﻿/*
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
#include "features.hpp"
#include "fiber_pool.hpp"
#include "file_manager.hpp"
#include "gui.hpp"	
#include "hooking.hpp"
#include "logger/exception_handler.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script_mgr.hpp"
#include "util/is_proton.hpp"

BOOL APIENTRY DllMain(HMODULE hmod, DWORD reason, PVOID)
{
	using namespace big;
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hmod);

		g_hmodule = hmod;
		g_main_thread = CreateThread(nullptr, 0, [](PVOID) -> DWORD
			{
				std::filesystem::path base_dir = std::getenv("appdata");
				base_dir /= "BigBaseV2";
				g_file_manager.init(base_dir);
				g_log.initialize("BigBaseV2", g_file_manager.get_project_file("./cout.log"));
				try
				{					
					// more tech debt, YAY!
					if (is_proton())
					{
						LOG(INFO) << "Running on proton!";
					}

					LOG(RAW_RED) << R"kek(
 ______  _       ______                        ______  
(____  \(_)     (____  \                      (_____ \ 
 ____)  )_  ____ ____)  ) ____  ___  ____ _   _ ____) )
|  __  (| |/ _  |  __  ( / _  |/___)/ _  ) | | /_____/ 
| |__)  ) ( ( | | |__)  | ( | |___ ( (/ / \ V /_______ 
|______/|_|\_|| |______/ \_||_(___/ \____) \_/(_______)
          (_____|
                                                          1.70 | b1.0.813.11

					)kek";

					auto pointers_instance = std::make_unique<pointers>();
					LOG(INFO) << "Pointers initialized.";

					auto renderer_instance = std::make_unique<renderer>();
					LOG(INFO) << "Renderer initialized.";

					auto fiber_pool_instance = std::make_unique<fiber_pool>(10);
					LOG(INFO) << "Fiber pool initialized.";

					auto hooking_instance = std::make_unique<hooking>();
					LOG(INFO) << "Hooking initialized.";

					g_settings.load();
					LOG(INFO) << "Settings Loaded.";

					g_script_mgr.add_script(std::make_unique<Script>(&gui::script_func));
					g_script_mgr.add_script(std::make_unique<Script>(&featuress::script_func));
					LOG(INFO) << "Scripts registered.";

					g_hooking->enable();
					LOG(INFO) << "Hooking enabled.";

					while (g_running)
					{
						if (GetAsyncKeyState(VK_DELETE) & 0x8000)
							g_running = false;

						std::this_thread::sleep_for(10ms);
					}

					g_hooking->disable();
					LOG(INFO) << "Hooking disabled.";

					std::this_thread::sleep_for(1000ms);

					g_script_mgr.remove_all_scripts();
					LOG(INFO) << "Scripts unregistered.";

					hooking_instance.reset();
					LOG(INFO) << "Hooking uninitialized.";

					fiber_pool_instance.reset();
					LOG(INFO) << "Fiber pool uninitialized.";

					renderer_instance.reset();
					LOG(INFO) << "Renderer uninitialized.";

					pointers_instance.reset();
					LOG(INFO) << "Pointers uninitialized.";
				}
				catch (std::exception const& ex)
				{
					LOG(WARNING) << ex.what();
					MessageBoxA(nullptr, ex.what(), nullptr, MB_OK | MB_ICONEXCLAMATION);
				}

				LOG(INFO) << "Farewell!";
				g_log.destroy();

				CloseHandle(g_main_thread);
				FreeLibraryAndExitThread(g_hmodule, 0);
			}, nullptr, 0, &g_main_thread_id);
	}

	return true;
}
