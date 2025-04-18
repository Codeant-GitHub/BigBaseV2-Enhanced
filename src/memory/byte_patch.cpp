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
#include "byte_patch.hpp"

namespace memory
{
	byte_patch::~byte_patch()
	{
		restore();
	}

	void byte_patch::apply() const
	{
		DWORD old_protect;
		VirtualProtect(m_address, m_size, PAGE_EXECUTE_READWRITE, (PDWORD)&old_protect);

		memcpy(m_address, m_value.get(), m_size);

		DWORD unused;
		VirtualProtect(m_address, m_size, old_protect, &unused);
	}

	void byte_patch::restore() const
	{
		DWORD old_protect;
		VirtualProtect(m_address, m_size, PAGE_EXECUTE_READWRITE, (PDWORD)&old_protect);

		memcpy(m_address, m_original_bytes.get(), m_size);

		DWORD unused;
		VirtualProtect(m_address, m_size, old_protect, &unused);
	}

	void byte_patch::remove() const
	{
		std::erase_if(m_patches, [this](auto& el) {
			return el.get() == this;
		});
	}

	void byte_patch::restore_all()
	{
		m_patches.clear();
	}

	bool operator==(const std::shared_ptr<byte_patch>& a, const byte_patch* b)
	{
		return a->m_address == b->m_address;
	}
}
