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

namespace big
{
	class file;
	class file_manager;

	class folder
	{
	public:
		folder(const std::filesystem::path& folder_path = "");

		file get_file(std::filesystem::path file_path) const;
		const std::filesystem::path get_path() const;

	private:
		friend class file_manager;
		file_manager* m_file_manager;

		bool m_is_project_file;

		std::filesystem::path m_folder_path;
	};
}