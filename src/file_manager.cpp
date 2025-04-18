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
#include "file_manager.hpp"

namespace big
{
    bool file_manager::init(const std::filesystem::path& base_dir)
    {
        m_base_dir = base_dir;
        file_manager::ensure_folder_exists(m_base_dir);

        return true;
    }

    const std::filesystem::path& file_manager::get_base_dir()
    {
        return m_base_dir;
    }
    
	file file_manager::get_project_file(std::filesystem::path file_path)
    {
        if (file_path.is_absolute())
            throw std::runtime_error("Project files are relative to the BaseDir, don't use absolute paths!");

        return file_manager::ensure_file_can_be_created(m_base_dir / file_path);
    }

	folder file_manager::get_project_folder(std::filesystem::path folder_path)
    {
        if (folder_path.is_absolute())
            throw std::runtime_error("Project folders are relative to the BaseDir, don't use absolute paths!");

        return file_manager::ensure_folder_exists(m_base_dir / folder_path);
    }

	std::filesystem::path file_manager::ensure_file_can_be_created(const std::filesystem::path file_path)
    {
        file_manager::ensure_folder_exists(file_path.parent_path());

        return file_path;
    }
    
	std::filesystem::path file_manager::ensure_folder_exists(const std::filesystem::path folder_path)
    {
        bool create_path = !std::filesystem::exists(folder_path);

        if (!create_path && !std::filesystem::is_directory(folder_path))
        {
            std::filesystem::remove(folder_path);
            create_path = true;
        }
        if (create_path)
            std::filesystem::create_directory(folder_path);

        return folder_path;
    }
}
