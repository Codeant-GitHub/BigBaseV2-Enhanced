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
#include "file.hpp"
#include "file_manager.hpp"

namespace big
{
	file::file(const std::filesystem::path& file_path) :
	    m_file_path(file_path)
	{
	}

	void file::operator=(const file& other)
	{
		m_file_path = other.m_file_path;
	}

	file::operator std::filesystem::path()
	{
		return m_file_path;
	}

	file::operator std::filesystem::path&()
	{
		return m_file_path;
	}

	bool file::exists() const
	{
		return std::filesystem::exists(m_file_path);
	}

	const std::filesystem::path file::get_path() const
	{
		return m_file_path;
	}

	file file::move(std::filesystem::path new_path)
	{
		if (new_path.is_relative())
			new_path = m_file_path.parent_path() / new_path;

		file_manager::ensure_file_can_be_created(new_path);

		if (std::filesystem::exists(m_file_path))
			std::filesystem::rename(m_file_path, new_path);

		return {new_path};
	}
}