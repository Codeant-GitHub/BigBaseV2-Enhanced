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
#include "pointers.hpp"
#include "memory/batch.hpp"

#define GTA_VERSION_TARGET "2025-04-05 11:40:45 1.70 | b1.0.813.11"

namespace big
{
	void pointers::get_gta_batch(memory::module region)
	{
		memory::batch main_batch;

		main_batch.add("IDXGI", "72 C7 EB 02 31 C0 8B 0D", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_commandqueue = ptr.add(0x1A).add(3).rip().as<ID3D12CommandQueue**>();
				g_pointers->m_gta.m_swapchain = ptr.add(0x21).add(3).rip().as<IDXGISwapChain1**>();
			});

		main_batch.add("WP", "3D 85 00 00 00 0F 87 2D 02 00 00", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_wndproc = ptr.sub(0x4F).as<PVOID>();
			});

		main_batch.add("HWND", "E8 ? ? ? ? 84 C0 74 25 48 8B 0D", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_hwnd = *ptr.add(9).add(3).rip().as<HWND*>();
			});

		main_batch.add("SX/SY", "75 39 0F 57 C0 F3 0F 2A 05", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_screenresx = ptr.add(0x5).add(4).rip().as<std::uint32_t*>();
				g_pointers->m_gta.m_screenresy = ptr.add(0x1E).add(4).rip().as<std::uint32_t*>();
			});

		main_batch.add("GV/OV", "4C 8D 0D ? ? ? ? 48 8D 5C 24 ? 48 89 D9 48 89 FA", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_game_version = ptr.add(3).rip().as<const char*>();
				g_pointers->m_gta.m_online_version = ptr.add(0x47).add(3).rip().as<const char*>();
			});

		main_batch.add("ISS", "80 3D ? ? ? ? ? 66 0F 7E DA 0F 44 C1", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_is_session_started = ptr.add(2).rip().add(1).as<bool*>();
			});

		main_batch.add("PF", "C7 40 30 03 00 00 00 48 8B 0D", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_ped_factory = ptr.add(7).add(3).rip().as<CPedFactory**>();
			});

		main_batch.add("NPM", "75 0E 48 8B 05 ? ? ? ? 48 8B 88 F0 00 00 00", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_network_player_mgr = ptr.add(2).add(3).rip().as<CNetworkPlayerMgr**>();
			});

		main_batch.add("NOM", "41 83 7E FA 02 40 0F 9C C5 C1 E5 02", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_network_object_mgr = ptr.add(0xC).add(3).rip().as<CNetworkObjectMgr**>();
			});

		main_batch.add("NRT/GNH", "4C 8D 0D ? ? ? ? 4C 8D 15 ? ? ? ? 45 31 F6 EB 2A", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_native_registration_table = ptr.add(3).rip().as<rage::scrNativeRegistrationTable*>();
				g_pointers->m_gta.m_get_native_handler = ptr.add(10).rip().as<functions::get_native_handler_t>();
			});

		/*
E8 4F 49 18 00 48 83 3D 47 0F EF 02 00 74 55 48 8D 05 DC 0E 82 02 48 89 
44 24 38 48 8D 05 49 E5 80 02 48 89 44 24 30 48 8D 05 12 E5 80 02 48 89 
44 24 28 48 8D 05 BD 1C 84 02 48 89 44 24 20 48 8D 0D 66 FF EE 02 48 8D 
15 47 D4 81 02 4C 8D 05 E3 73 84 02 4C 8D 0D 70 24 84 02 E8 B4 DA 17 00 
84 C0 74 2A E8 0B 12 00 00 89 C1 31 C0 84 C9 75 34 B9 2F A9 C2 F4 B2 01 
E8 F7 77 15 00 B8 02 00 00 00 EB 21 41 C7 46 1C B2 7D 09 00 EB 12 41 C7 
46 1C BD 7D 09 00 EB 08 41 C7 46 1C B1 7D 09 00 B8 01 00 00 00 48 81 C4 
48 02 00 00 5B 5F 5E 41 5E C3 90 C7 41 57 41 56 56 57 53 48 81 EC 50 03 
00 00 83 F9 FF 0F 84 20 01 00 00 48 89 D6 48 85 D2 0F 84 14 01 00 00 F6 
05 3A 4B A4 02 01 0F 85 07 01 00 00 89 CF FF 15 48 5B D7 04 85 C0 0F 84

E8 ? ? ? ? B8 ? ? ? ? EB 21 41 C7 46 ? ? ? ? ? EB 12
E8 ? ? ? ? B8 ? ? ? ? EB 21 ? C7 ? 34 01 00 00 00 00 00 00 B8 02 00 00 00 EB 0F 48 8B 87 A4 00 00 00 48 89 06 B8 03 00 00 00 48 83 C4 38
		*/

		//GTA5_Enhanced.Ordinal0+27C4 - E8 F7771500           - call GTA5_Enhanced.exe+159FC0

		main_batch.add("FVS", "E8 ? ? ? ? B8 ? ? ? ? EB 21 ? C7", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_fix_vectors = ptr.add(1).rip().as<functions::fix_vectors_t>();
			});

		main_batch.add("ST", "48 8B 05 ? ? ? ? 48 89 34 F8 48 FF C7 48 39 FB 75 97", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_script_threads = ptr.add(3).rip().as<rage::atArray<rage::scrThread*>*>();
			});

		main_batch.add("SG", "48 8D 15 ? ? ? ? 49 89 D8 4D 89 F1", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_script_globals = ptr.add(3).rip().as<std::int64_t**>();
			});

		/*
41 5D 41 5E 41 5F C3 98 64 E7 52 90 7A 26 48 81 EC 88 00 00 00 85 D2 74 1A 83 FA 02 74 21 83 FA 
01 0F 85 CE 00 00 00 C6 05 A0 AE 67 03 01 E9 C2 00 00 00 C6 05 94 AE 67 03 00 E9 B6 00 00 00 83 
3D 28 9C F9 01 06 0F 82 A9 00 00 00 8B 05 80 AE 67 03 89 44 24 38 48 8D 44 24 38 48 89 44 24 70 
48 C7 44 24 78 04 00 00 00 48 B8 00 00 00 0B 05 00 00 00 48 89 44 24 40 48 C7 44 24 48 00 00 00 
00 48 8B 05 EE 9B F9 01 48 89 44 24 50 0F B7 00 89 44 24 58 C7 44 24 5C 02 00 00 00 48 8D 05 7F 
BE EC 01 48 89 44 24 60 48 B8 1B 00 00 00 01 00 00 00 48 89 44 24 68 48 8D 05 08 BE EC 01 48 8D 
0D 3F BF EC 01 29 C1 89 4C 24 3C 48 8B 0D BC 9B F9 01 48 8D 44 24 50 48 89 44 24 28 C7 44 24 20 
03 00 00 00 48 8D 54 24 40 45 31 C0 45 31 C9 FF 15 D9 27 84 04 90 48 81 C4 88 00 00 00 C3 48 83 
EC 28 39 15 AE AD 67 03 74 16 31 C0 81 FA 00 01 00 00 75 26 0F B7 0D 9F AD 67 03 4C 39 C1 75 1A
8B 15 2C EF 5C 02 FF C2 B9 08 00 00 00 E8 1C 00 00 00 FF 05 AE AD 67 03 B0 01 48 83 C4 28 C3 C7 
85 8C 00 00 00 01 00 00 00 E9 79 06 95 04 56 57 53 48 81 EC C0 00 00 00 80 3D 77 AD 67 03 00 74 
64 48 89 D3 89 CF E8 13 5F C2 00 80 78 09 01 75 5B 0F 57 C0 0F 29 44 24 50 0F 29 44 24 60 0F 29 
84 24 90 00 00 00 0F 29 84 24 80 00 00 00 0F 29 44 24 70 48 C7 84 24 A0 00 00 00 00 00 00 00 C7 
44 24 50 58 00 01 00 48 89 5C 24 58 89 7C 24 60 48 8B 0D 7F D4 59 04 48 8D 54 24 50 E8 BD 37 9F 
01 85 C0 74 07 31 F6 E9 DA 00 00 00 40 B6 01 83 3D 98 9A F9 01 06 0F 82 CA 00 00 00 48 89 5C 24 
40 89 7C 24 38 48 8D 44 24 40 48 89 84 24 80 00 00 00 48 C7 84 24 88 00 00 00 08 00 00 00 48 8D 
44 24 38 48 89 44 24 70 48 C7 44 24 78 04 00 00 00 48 B8 00 00 00 0B 05 00 00 00 48 89 84 24 B0 
00 00 00 48 C7 84 24 B8 00 00 00 00 00 00 00 48 8B 05 40 9A F9 01 48 89 44 24 50 0F B7 00 89 44 
24 58 C7 44 24 5C 02 00 00 00 48 8D 05 3A BD EC 01 48 89 44 24 60 48 B8 25 00 00 00 01 00 00 00
		*/

		//m_frame_count GTA5_Enhanced.exe+938292 - 8B 15 2CEF5C02        - mov edx,[GTA5_Enhanced.exe+2F071C4] { (110943) }

		main_batch.add("FC", "8B 15 2C EF 5C 02 FF C2 B9 08", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_frame_count = ptr.add(2).rip().as<uint32_t*>();
				LOG(INFO) << "m_frame_count" << " - " << HEX_TO_UPPER(g_pointers->m_gta.m_frame_count);
			});

		main_batch.add("SVM/RST", "E8 ? ? ? ? 8B 0D ? ? ? ? 65 48 8B 14 25 ? ? ? ? 48 8B 0C CA 4C 89 A1 ? ? ? ? 4C 89 3D ? ? ? ?", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_script_vm = ptr.add(1).rip().as<functions::script_vm>();
				g_pointers->m_gta.m_run_script_threads = ptr.add(1).rip().as<uintptr_t>();
			});

		main_batch.add("NR", "FF E3", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_native_return = ptr.add(0).as<PVOID>();
			});

		main_batch.add("INT", "EB 2A 0F 1F 40 00 48 8B 54 17 10", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_initnativetables = ptr.sub(0xA).as<PVOID>();
			});

		main_batch.add("HTP/PTH", "0F 1F 84 00 00 00 00 00 89 F8 0F 28 FE 41", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_handle_to_ptr = ptr.add(0x21).add(1).rip().as<functions::handle_to_ptr>();
				g_pointers->m_gta.m_ptr_to_handle = ptr.sub(0xB).add(1).rip().as<functions::ptr_to_handle>();
			});

		main_batch.add("MSB", "E8 ? ? ? ? 48 8B 78 48", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_model_spawn_bypass = ptr;
			});

		main_batch.add("SP", "74 26 66 83 FF 0D 77 20 0F B7 C7", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_spectate_patch_bypass = ptr;
			});

		main_batch.add("WMSB", "4C 8B 2C 01 4D 85 ED 0F 84 ? ? ? ?", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_world_model_spawn_bypass = ptr;
			});

		main_batch.add("BSUB", "80 B9 92 0A 00 00 01", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_battleye_status_update_bypass = ptr;
			});

		main_batch.add("SPT", "48 C7 84 C8 D8 00 00 00 00 00 00 00", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_script_program_table = ptr.add(0x13).add(3).rip().add(0xD8).as<decltype(gta_pointers::m_script_program_table)>();
			});

		//main_batch.add("BL", "48 8D 0D ? ? ? ? 41 B8 ? ? ? ? 31 D2 E8 ? ? ? ? 8B 0D", [](memory::handle ptr)
		//	{
		//		g_pointers->m_gta.m_blip_list = ptr.add(3).rip().as<CBlipList*>();
		//	});

		main_batch.add("GNO", "0F B7 4E 60 E8", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_get_net_object = ptr.add(4).add(1).rip().as<decltype(gta_pointers::m_get_net_object)>();
			});

		main_batch.add("QP", "45 89 F1 E8 ? ? ? ? 84 C0 74 25", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_queue_packet = ptr.add(3).add(1).rip().as<functions::queue_packet>();
			});

		main_batch.add("RC", "74 0A 48 89 F9 E8 ? ? ? ? 31 F6 89 F0 48 83 C4 20", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_request_control = ptr.add(5).add(1).rip().as<functions::request_control>();
			});

		main_batch.add("PEP", "48 8B 3D ? ? ? ? 48 8B 07 8B 40 10 48 85 C0 74 6E", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_ped_pool = ptr.add(3).rip().as<PoolEncryption*>();
			});

		main_batch.add("MO", "48 8B 96 D0 00 00 00 4C 89 F9 41 B8 03 00 00 00", [](memory::handle ptr)
			{
				g_pointers->m_gta.m_migrate_object = ptr.add(0x10).add(1).rip().as<functions::migrate_object>();
			});

		main_batch.add("BEDA", "48 C7 05 ? ? ? ? 00 00 00 00 E8 ? ? ? ? 48 89 C1 E8 ? ? ? ? E8 ? ? ? ? BD 0A 00 00 00", [](memory::handle ptr)
			{
				g_pointers->m_gta.BERestartStatus = ptr.add(3).rip().add(8).add(4).as<int*>();
				g_pointers->m_gta.NeedsBERestart = ptr.add(3).rip().add(8).add(4).add(8).as<bool*>();
				g_pointers->m_gta.IsBEBanned = ptr.add(3).rip().add(8).add(4).add(8).add(4).as<bool*>();
			});

		main_batch.add("GNPFP", "83 FB 20 74 2A 89 D9", [](memory::handle ptr)
			{
				g_pointers->m_gta.GetNetPlayerFromPid = ptr.add(8).rip().as<functions::GetNetPlayerFromPid>();
			});

		main_batch.add("GNOB", "0F B7 4E 60 E8", [](memory::handle ptr)
			{
				g_pointers->m_gta.GetNetObjectById = ptr.add(4).add(1).rip().as<functions::GetNetObjectById>();
			});

		main_batch.add("RC", "4C 8D 05 ? ? ? ? 48 89 F1 48 89 FA E8 ? ? ? ? 84 C0 74 3D", [](memory::handle ptr)
			{
				g_pointers->m_gta.RegionCode = ptr.add(3).rip().as<int*>();
			});

		main_batch.add("TWDE", "4C 8D 05 ? ? ? ? 48 89 F1 48 89 FA E8 ? ? ? ? 84 C0 74 3D", [](memory::handle ptr)
			{
				g_pointers->m_gta.TriggerWeaponDamageEvent = ptr.sub(0x51).as< functions::TriggerWeaponDamageEvent>();
			});

		main_batch.run(region);
	}

#ifdef ENABLE_SOCIALCLUB
	void pointers::get_sc_batch(memory::module region)
	{
		// clang-format off

		memory::batch socialclub_batch;

		socialclub_batch.run(region);
	}
#endif // ENABLE_SOCIALCLUB

	pointers::pointers()
	{
		g_pointers = this;

		const auto mem_region = memory::module("GTA5_Enhanced.exe");

		pointers::get_gta_batch(mem_region);

#ifdef ENABLE_SOCIALCLUB
		auto sc_module = memory::module("socialclub.dll");

		if (sc_module.wait_for_module())
		{
			pointers::get_sc_batch(sc_module);
		}
		else
		{
			LOG(INFO) << "socialclub.dll module was not loaded within the time limit.";
		}
#endif // ENABLE_SOCIALCLUB

		if (!g_pointers->m_gta.m_hwnd)
		{
			throw std::runtime_error("Failed to find the game's window.");
		}
		else
		{
			char name[128];
			GetWindowTextA(g_pointers->m_gta.m_hwnd, name, RTL_NUMBER_OF(name));

			LOG(INFO) << "Got window with name: " << name;

			LOG(INFO) << "Hwnd: " << HEX_TO_UPPER(g_pointers->m_gta.m_hwnd);

			LOG(INFO) << "Screen Resolution: " << *g_pointers->m_gta.m_screenresx << "x" << *g_pointers->m_gta.m_screenresy;

			LOG(INFO) << "Game Version: " << g_pointers->m_gta.m_game_version;

			LOG(INFO) << "Online Version: " << g_pointers->m_gta.m_online_version;
		}
	}

	pointers::~pointers()
	{
		g_pointers = nullptr;
	}
}
