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
#include "folder.hpp"
#include "file_manager.hpp"

namespace big
{
	folder::folder(const std::filesystem::path& folder_path) :
	    m_folder_path(folder_path)
	{
	}

	file folder::get_file(std::filesystem::path file_path) const
	{
		if (file_path.is_absolute())
			throw std::runtime_error("folder#get_file requires a relative path.");

		return file(m_folder_path / file_path);
	}

	const std::filesystem::path folder::get_path() const
	{
		return m_folder_path;
	}
}