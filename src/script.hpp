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

namespace big
{
    class Script
    {
    private:
        void* m_script_fiber;
        void* m_main_fiber;
        std::function<void()> action;
        std::optional<std::chrono::high_resolution_clock::time_point> wake;
    public:
        Script(std::function<void()> scriptAction);
        static void ScriptExceptionHandler(PEXCEPTION_POINTERS exp);
    public:
        static Script* get_current();
        void yield(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);
        void fiber_func();
        void tick();
    };

#define TRY_CLAUSE  __try
#define EXCEPT_CLAUSE  __except (script::script_exception_handler(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) { }
#define QUEUE_JOB_BEGIN_CLAUSE(...) g_fiber_pool->queue_job([__VA_ARGS__] { __try
#define QUEUE_JOB_END_CLAUSE __except (script::script_exception_handler(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {} });
}
