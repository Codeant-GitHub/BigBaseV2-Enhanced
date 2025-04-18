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
#include "pointers.hpp"
#include "gui.hpp"

#include "memory/signature.hpp"

#include <cstdio>
#include <mutex>
#include <thread>

#include "hooks.hpp"

#include "d3d12hook.hpp"
#include "kiero.hpp"

#include <MinHook.h>

static DWORD WINAPI ReinitializeGraphicalHooks(LPVOID lpParam)
{
    LOG(INFO) << "Hooks will reinitialize!";

    H::bShuttingDown = true;

    H::Free();

    big::g_pointers->m_gta.m_hwnd = *memory::Signature("E8 ? ? ? ? 84 C0 74 25 48 8B 0D").Add(9).Add(3).Rip().Get<HWND*>();

    LOG(INFO) << "Hwnd: " << big::g_pointers->m_gta.m_hwnd;

    H::Init();

    H::bShuttingDown = false;

    return 0;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static WNDPROC oWndProc;
static LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_KEYUP && wParam == VK_INSERT)
    {
        //Persist and restore the cursor position between menu instances.
        static POINT cursor_coords{};
        if (big::g_gui.m_opened)
        {
            GetCursorPos(&cursor_coords);
        }
        else if (cursor_coords.x + cursor_coords.y != 0)
        {
            SetCursorPos(cursor_coords.x, cursor_coords.y);
        }

        big::g_gui.m_opened ^= true;
    }
    if (uMsg == WM_KEYDOWN)
    {
        if (wParam == VK_END)
        {
            H::bShuttingDown = true;
            return 0;
        }
    }
    else if (uMsg == WM_DESTROY)
    {
        HANDLE hHandle = CreateThread(NULL, 0, ReinitializeGraphicalHooks, hWnd, 0, NULL);
        if (hHandle != NULL)
            CloseHandle(hHandle);
    }

    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

namespace Hooks
{
    void Init()
    {
        if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success)
        {
            kiero::bind(54, (void**)&d3d12hook::oExecuteCommandListsD3D12, d3d12hook::hookExecuteCommandListsD3D12);
            kiero::bind(58, (void**)&d3d12hook::oSignalD3D12, d3d12hook::hookSignalD3D12);
            kiero::bind(140, (void**)&d3d12hook::oPresentD3D12, d3d12hook::hookPresentD3D12);
            kiero::bind(84, (void**)&d3d12hook::oDrawInstancedD3D12, d3d12hook::hookkDrawInstancedD3D12);
            kiero::bind(85, (void**)&d3d12hook::oDrawIndexedInstancedD3D12, d3d12hook::hookDrawIndexedInstancedD3D12);
        }

        oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(big::g_pointers->m_gta.m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));
    }

    void Free()
    {
        if (oWndProc)
        {
            SetWindowLongPtr(big::g_pointers->m_gta.m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));
        }

        d3d12hook::release();
        kiero::shutdown();
    }
}