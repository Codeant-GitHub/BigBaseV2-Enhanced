#pragma once
#include "gta/fwddec.hpp"
#include "gta/natives.hpp"

#include <types/script/scrVector.hpp>
#include <types/script/globals/GPBD_FM.hpp>
#include <types/network/rlGamerInfo.hpp>
#include <types/network/CNetGamePlayer.hpp>
#include <types/network/sync/nodes/vehicle/CVehicleProximityMigrationDataNode.hpp>
#include <types/network/CNetworkPlayerMgr.hpp>
#include <types/network/netSocketAddress.hpp>
#include <types/ped/CPedFactory.hpp>

class CBlipList;
class PoolEncryption;
class CNetGamePlayer;
union netAddress;

namespace rage
{
	class rlGamerInfo;
	class rlGamerInfoBase;
	class netAddress;
	class netConnectionManager;
}

namespace big::functions
{
	using run_script_threads_t = bool(*)(std::uint32_t ops_to_execute);
	using get_native_handler_t = rage::scrNativeHandler(*)(rage::scrNativeRegistrationTable*, rage::scrNativeHash);
	using fix_vectors_t = void(*)(rage::scrNativeCallContext*);

	using get_player_ped_fn = int(__fastcall*)(int player);
	using is_dlc_present_fn = int(__fastcall*)(__int64 a1, DWORD dlchash);

	using script_vm = int (*)(uint64_t* stack, int64_t** scr_globals, rage::scrProgram* program, void* ctx);

	using ptr_to_handle = int (*)(void* pointer);
	using handle_to_ptr = void* (*)(int handle);

	using get_net_object = rage::netObject* (*)(CNetworkObjectMgr* mgr, int16_t id, bool can_delete_be_pending);
	using queue_packet = void(*)(rage::netConnectionManager* mgr, int msg_id, void* data, int size, int flags, std::uint16_t* out_seq_id);
	using request_control = void (*)(rage::netObject* net_object);
	using migrate_object = void (*)(CNetGamePlayer* player, rage::netObject* object, int type);

	using GetNetPlayerFromPid = CNetGamePlayer * (*)(int id);
	using GetNetObjectById = rage::netObject* (*)(uint16_t id);
	using TriggerWeaponDamageEvent = void (*)(CEntity* source, CEntity* target, rage::fvector3* position, int hit_component, bool override_default_damage, int weapon_type, float override_damage, int tire_index, int suspension_index, int flags, uint32_t action_result_hash, int16_t action_result_id, int action_unk, bool hit_weapon, bool hit_weapon_ammo_attachment, bool silenced, bool unk, rage::fvector3* impact_direction);

}
