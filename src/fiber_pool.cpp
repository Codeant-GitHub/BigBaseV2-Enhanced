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
#include "script.hpp"
#include "script_mgr.hpp"

namespace big
{
	fiber_pool::fiber_pool(std::size_t num_fibers)
	{
		for (std::size_t i = 0; i < num_fibers; ++i)
		{
			g_script_mgr.add_script(std::make_unique<Script>(&fiber_func));
		}

		g_fiber_pool = this;
	}

	fiber_pool::~fiber_pool()
	{
		g_fiber_pool = nullptr;
	}

	void fiber_pool::queue_job(std::function<void()> func)
	{
		if (func)
		{
			std::lock_guard lock(m_mutex);
			m_jobs.push(std::move(func));
		}
	}

	void fiber_pool::fiber_tick()
	{
		std::unique_lock lock(m_mutex);
		if (!m_jobs.empty())
		{
			auto job = std::move(m_jobs.top());
			m_jobs.pop();
			lock.unlock();

			std::invoke(std::move(job));
		}
	}

	void fiber_pool::fiber_func()
	{
		while (true)
		{
			g_fiber_pool->fiber_tick();
			Script::get_current()->yield();
		}
	}
}

