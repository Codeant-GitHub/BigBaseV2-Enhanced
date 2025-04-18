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
#include "crossmap.hpp"
#include "invoker.hpp"
#include "pointers.hpp"

#include <cstdint>
using QWORD = uint64_t;

extern "C" void	_call_asm(void* context, void* function, void* ret);

namespace big
{
    PVOID native_invoker::GetNatiHandler(uintptr_t Hash)
    {
        uintptr_t* qword_7FF6BB86F5F0 = (uintptr_t*)g_pointers->m_gta.m_native_registration_table;
        QWORD v3; // rdx
        __int64 v5 = Hash; // r15
        DWORD* v6; // rdi
        int v7; // ebp
        unsigned __int64 v8; // rdx
        int v9; // esi


        v6 = (DWORD*)qword_7FF6BB86F5F0[(unsigned __int8)v5];
        if (v6)
        {
            while ((DWORD)v6 + 72 == (v6[19] ^ v6[18]))
            {
            LABEL_11:
                v6 = (DWORD*)((unsigned int)v6 ^ v6[2] ^ *v6 | ((unsigned __int64)(v6[1] ^ (unsigned int)v6 ^ v6[2]) << 32));
                if (!v6)
                    goto LABEL_10;
            }
            v7 = (DWORD)v6 + 84;
            v8 = 0i64;
            while (1)
            {
                v9 = v7 ^ v6[v8 / 2 + 23];
                if ((((unsigned __int64)(v6[v8 / 2 + 22] ^ (unsigned int)v9) << 32) | (unsigned int)v9 ^ v6[v8 / 2 + 21]) == v5)
                    break;
                v8 += 8i64;
                v7 += 16;
                if (8i64 * (((DWORD)v6 + 72) ^ (unsigned int)(v6[19] ^ v6[18])) == v8)
                    goto LABEL_11;
            }
            v3 = *(QWORD*)&v6[v8 / 4 + 4];
        }
        else
        {
        LABEL_10:
            v3 = 0i64;
        }

        if (!v3)
        {
            return (PVOID)nullptr;
        }
        return (PVOID)v3;
    }

	native_call_context::native_call_context()
	{
		m_return_value = &m_return_stack[0];
		m_args = &m_arg_stack[0];
	}

	void native_invoker::cache_handlers()
	{
		for (const rage::scrNativeMapping &mapping : g_crossmap)
		{
            rage::scrNativeHandler handler = reinterpret_cast<rage::scrNativeHandler>(GetNatiHandler(mapping.second));
			m_handler_cache.emplace(mapping.first, handler);
		}
	}

	void native_invoker::begin_call()
	{
		m_call_context.reset();
	}

	void native_invoker::end_call(rage::scrNativeHash hash)
	{
		if (auto it = m_handler_cache.find(hash); it != m_handler_cache.end())
		{
			rage::scrNativeHandler handler = it->second;

			__try
			{
                _call_asm(&m_call_context, handler, g_pointers->m_gta.m_native_return);
				handler(&m_call_context);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
                //[hash]() { LOG(WARNING) << "Exception caught while trying to call " << hash << " native."; }();
            }
		}
		else
		{
            [hash]() { LOG(WARNING) << "Failed to find " << HEX_TO_UPPER(hash) << " native's handler."; }();
		}
	}
}
