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
#include "signature.hpp"

#include <psapi.h>

namespace memory
{
	Signature::Signature(std::string_view IDAPattern)
	{
		// Helper functions
		auto ToUpper = [](char c) -> char
		{
			return ((c >= 'a' && c <= 'z') ? c + ('A' - 'a') : c);
		};

		auto ToHex = [&](char c) -> uint8_t
		{
			switch (ToUpper(c))
			{
			case '0': return static_cast<uint8_t>(0x0);
			case '1': return static_cast<uint8_t>(0x1);
			case '2': return static_cast<uint8_t>(0x2);
			case '3': return static_cast<uint8_t>(0x3);
			case '4': return static_cast<uint8_t>(0x4);
			case '5': return static_cast<uint8_t>(0x5);
			case '6': return static_cast<uint8_t>(0x6);
			case '7': return static_cast<uint8_t>(0x7);
			case '8': return static_cast<uint8_t>(0x8);
			case '9': return static_cast<uint8_t>(0x9);
			case 'A': return static_cast<uint8_t>(0xA);
			case 'B': return static_cast<uint8_t>(0xB);
			case 'C': return static_cast<uint8_t>(0xC);
			case 'D': return static_cast<uint8_t>(0xD);
			case 'E': return static_cast<uint8_t>(0xE);
			case 'F': return static_cast<uint8_t>(0xF);
			}
			return 0;
		};

		// Loop through pattern
		for (size_t i = 0; i < IDAPattern.size(); i++)
		{
			// Skip gaps
			if (IDAPattern[i] == ' ')
				continue;

			// Unknown byte
			if (IDAPattern[i] == '?')
			{
				// Check for double question mark
				if (IDAPattern[i + 1] == '?')
					i++;
				m_Bytes.push_back(-1);
				continue;
			}

			// Skip last byte if incomplete
			if (i == (IDAPattern.size() - 1))
				break;

			// Construct whole byte from two characters
			uint8_t c1 = ToHex(IDAPattern[i]);
			uint8_t c2 = ToHex(IDAPattern[i + 1]);
			m_Bytes.push_back((c1 * 0x10) + c2);
			// Skip over the second part of the byte
			i++;
		}
		Scan();
	}

	Signature::Signature(void* CodePattern, std::string_view Mask)
	{
		// Loop through pattern
		for (size_t i = 0; i < Mask.size(); i++)
		{
			// Check for unknown byte in mask
			if (Mask[i] == '?')
			{
				m_Bytes.push_back(-1);
				continue;
			}

			m_Bytes.push_back(static_cast<uint8_t*>(CodePattern)[i]);
		}
		Scan();
	}

	Signature::Signature(uintptr_t Address) :
		m_Result(Address)
	{
		assert(m_Result);
	}

	Signature& Signature::Scan()
	{
		// Helper function to scan for pattern
		auto CheckPattern = [this](uintptr_t Location) -> bool
		{
			for (size_t i = 0; i < m_Bytes.size(); i++)
			{
				if ((m_Bytes[i] != -1) && m_Bytes[i] != *reinterpret_cast<uint8_t*>(Location + i))
					return false;
			}
			return true;
		};

		// Check if already scanned
		if (m_Result)
			return *this;

		// Get information about process
		MODULEINFO ModuleInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &ModuleInfo, sizeof(MODULEINFO));
		MEMORY_BASIC_INFORMATION Mbi;

		const auto Begin = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL)); // Begin location of scan (base address)
		const auto Size = ModuleInfo.SizeOfImage; // Total size of process (area to be scanned)

		// Loop through memory regions
		for (uintptr_t curr = Begin; curr < Begin + Size; curr += Mbi.RegionSize)
		{
			// Check if current region is invalid
			if (!VirtualQuery(reinterpret_cast<LPCVOID>(curr), &Mbi, sizeof(Mbi))
				|| Mbi.State != MEM_COMMIT || Mbi.Protect == PAGE_NOACCESS)
				continue;

			// Loop through current region
			for (size_t i = 0; i < Size; ++i)
			{
				// Check if pattern matches at current location
				if (CheckPattern(curr + i))
				{
					m_Result = curr + i;
					return *this;
				}
			}
		}

		// At this point the scan has failed so m_Result is 0
		assert(m_Result);
		return *this;
	}

	// Add an offset
	Signature& Signature::Add(ptrdiff_t n)
	{
		m_Result += n;
		return *this;
	}

	// Subtract an offset
	Signature& Signature::Sub(ptrdiff_t n)
	{
		m_Result -= n;
		return *this;
	}

	// Follow instruction pointer register (rip) offset
	Signature& Signature::Rip()
	{
		return Add(*Get<int32_t*>()).Add(4);
	}

	// Get pointer address
	uintptr_t Signature::GetRaw() const
	{
		assert(m_Result);
		return m_Result;
	}
}