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
#include "pattern.hpp"

#include <functional>
#include <vector>

namespace memory
{
	class batch
	{
	public:
		explicit batch()  = default;
		~batch() noexcept = default;

		void add(std::string name, pattern pattern, std::function<void(memory::handle)> callback);
		bool run(range region);

		struct entry
		{
			std::string m_name;
			pattern m_pattern;
			std::function<void(memory::handle)> m_callback;

			explicit entry(std::string name, pattern pattern, std::function<void(memory::handle)> callback) :
			    m_name(std::move(name)),
			    m_pattern(std::move(pattern)),
			    m_callback(std::move(callback))
			{
			}
		};

	private:
		std::vector<entry> m_entries;
	};
}