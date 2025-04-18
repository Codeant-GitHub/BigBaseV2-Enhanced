#pragma once
#include "pointers.hpp"

#include <types/rage/atArray.hpp>
#include <types/rage/tlsContext.hpp>
#include <types/script/GtaThread.hpp>

#include <types/network/CNetworkPlayerMgr.hpp>
#include <types/ped/CPedFactory.hpp>

namespace rage
{
	class netObjectIds;
}

namespace big::gta_util
{
	inline CPed* get_local_ped()
	{
		if (auto ped_factory = *g_pointers->m_gta.m_ped_factory)
		{
			return ped_factory->m_LocalPed;
		}

		return nullptr;
	}

	inline CNetworkPlayerMgr* get_network_player_mgr()
	{
		return *g_pointers->m_gta.m_network_player_mgr;
	}

	inline void execute_as_script(rage::scrThread* thread, std::function<void()> callback)
	{
		auto og_thread = rage::tlsContext::get_t()->m_CurrentScriptThread;
		auto og_running_in_scrthread = rage::tlsContext::get_t()->m_ScriptThreadActive;
		rage::tlsContext::get_t()->m_CurrentScriptThread = thread;
		rage::tlsContext::get_t()->m_ScriptThreadActive = true;
		callback();
		rage::tlsContext::get_t()->m_ScriptThreadActive = og_running_in_scrthread;
		rage::tlsContext::get_t()->m_CurrentScriptThread = og_thread;
	}

	inline rage::scrThread* find_script_thread(rage::joaat_t hash)
	{
		for(auto & thread : *g_pointers->m_gta.m_script_threads)
		{
			if (thread && thread->m_Context.m_ThreadId && thread->m_ScriptHash == hash)
			{
				return thread;
			}
		}

		return nullptr;
	}

	inline rage::scrThread* find_script_thread_by_id(uint32_t id)
	{
		for (auto thread : *g_pointers->m_gta.m_script_threads)
		{
			if (thread && thread->m_Context.m_ThreadId == id)
			{
				return thread;
			}
		}

		return nullptr;
	}
}
