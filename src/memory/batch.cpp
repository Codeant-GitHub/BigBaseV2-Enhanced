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
#include "batch.hpp"
#include "range.hpp"

#include <future>//std::async

static std::mutex s_entry_mutex;
static std::vector<std::future<bool>> g_futures;

namespace memory
{
	void batch::add(std::string name, pattern pattern, std::function<void(handle)> callback)
	{
		m_entries.emplace_back(std::move(name), std::move(pattern), std::move(callback));
	}

	bool scan_pattern_and_execute_callback(range region, memory::batch::entry entry)
	{
		if (auto result = region.scan(entry.m_pattern))
		{
			if (entry.m_callback)
			{
				std::lock_guard<std::mutex> lock(s_entry_mutex);// Acquire a lock on the mutex to synchronize access.

				std::invoke(std::move(entry.m_callback), *result);
				
				LOG(INFO) << "Found '" << entry.m_name << "' GTA5_Enhanced.exe+" << HEX_TO_UPPER(result->as<DWORD64>() - region.begin().as<DWORD64>());

				return true;
			}
		}

		LOG(WARNING) << "Failed to find '" << entry.m_name << "'." << " Hook.";

		return false;
	}

	bool batch::run(range region)
	{
		for (auto& entry : m_entries)
		{
			g_futures.emplace_back(std::async(&scan_pattern_and_execute_callback, region, entry));
		}

		bool found_all_patterns = true;
		for (auto& future : g_futures)
		{
			future.wait();

			if (!future.get())
				found_all_patterns = false;
		}

		m_entries.clear();
		g_futures.clear();

		return found_all_patterns;
	}
}