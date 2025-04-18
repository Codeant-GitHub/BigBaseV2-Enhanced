#pragma once
#include "common.hpp"
#include "gta/fwddec.hpp"
#include "gta/enums.hpp"
#include "function_types.hpp"

#include "memory/handle.hpp"

namespace big
{
#pragma pack(push, 1)
	struct gta_pointers
	{
		HWND m_hwnd{};

		IDXGISwapChain1** m_swapchain{};
		ID3D12CommandQueue** m_commandqueue{};

		PVOID m_wndproc{};

		std::uint32_t* m_screenresx{};
		std::uint32_t* m_screenresy{};

		const char* m_game_version{};
		const char* m_online_version{};

		bool* m_is_session_started{};

		CPedFactory** m_ped_factory{};
		CNetworkPlayerMgr** m_network_player_mgr{};
		functions::GetNetObjectById GetNetObjectById;
		CNetworkObjectMgr** m_network_object_mgr{};

		rage::scrNativeRegistrationTable* m_native_registration_table{};
		functions::get_native_handler_t m_get_native_handler{};
		functions::fix_vectors_t m_fix_vectors{};

		rage::atArray<rage::scrThread*>* m_script_threads{};
		uintptr_t m_run_script_threads{};
		std::int64_t** m_script_globals{};

		uint32_t* m_frame_count;
		functions::script_vm m_script_vm{};
		PVOID m_native_return{};

		PVOID m_initnativetables{};

		functions::handle_to_ptr m_handle_to_ptr{};
		functions::ptr_to_handle m_ptr_to_handle{};

		memory::handle m_model_spawn_bypass{};
		memory::handle m_spectate_patch_bypass{};
		memory::handle m_world_model_spawn_bypass{};

		memory::handle m_battleye_status_update_bypass{};

		rage::scrProgramTable* m_script_program_table{};

		//CBlipList* m_blip_list{};

		functions::get_net_object m_get_net_object{};
		functions::queue_packet m_queue_packet{};
		functions::request_control m_request_control{};
		PoolEncryption* m_ped_pool{};
		functions::migrate_object m_migrate_object{};

		int* BERestartStatus;
		bool* NeedsBERestart;
		bool* IsBEBanned;

		functions::GetNetPlayerFromPid GetNetPlayerFromPid;
		int* RegionCode;
		functions::TriggerWeaponDamageEvent TriggerWeaponDamageEvent;
	};
#pragma pack(pop)
	static_assert(sizeof(gta_pointers) % 8 == 0, "Pointers are not properly aligned");
}