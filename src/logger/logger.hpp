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
#include "file_manager.hpp"
#include <AsyncLogger/Logger.hpp>

using namespace al;

namespace stream
{
	inline auto net_events   = std::make_shared<LogStream>("net_events");
	inline auto net_messages = std::make_shared<LogStream>("net_messages");
	inline auto net_sync      = std::make_shared<LogStream>("net_sync");
	inline auto script_events     = std::make_shared<LogStream>("script_events");
}

namespace big
{
#define ADD_COLOR_TO_STREAM(color) "\x1b[" << int(color) << "m"
#define RESET_STREAM_COLOR "\x1b[" << int(LogColor::RESET) << "m"
#define WHITE_STREAM_COLOR "\x1b[" << int(LogColor::WHITE) << "m"
#define CYAN_STREAM_COLOR "\x1b[" << int(LogColor::CYAN) << "m"
#define MAGENTA_STREAM_COLOR "\x1b[" << int(LogColor::MAGENTA) << "m"
#define BLUE_STREAM_COLOR "\x1b[" << int(LogColor::BLUE) << "m"
#define GREEN_STREAM_COLOR "\x1b[" << int(LogColor::GREEN) << "m"
#define YELLOW_STREAM_COLOR "\x1b[" << int(LogColor::YELLOW) << "m"
#define RED_STREAM_COLOR "\x1b[" << int(LogColor::RED) << "m"
#define BLACK_STREAM_COLOR "\x1b[" << int(LogColor::BLACK) << "m"
#define HEX_TO_UPPER(value) "0x" << std::hex << std::uppercase << (DWORD64)value << std::dec << std::nouppercase

	enum class LogColor
	{
		RESET,//重置
		WHITE = 97,//白色
		CYAN = 36,//青色
		MAGENTA = 35,//品红色
		BLUE = 34,//蓝色
		GREEN = 32,//绿色
		YELLOW = 33,//黄色
		RED = 31,//红色
		BLACK = 30//黑色
	};

	class logger final
	{
	private:
		bool m_attach_console = true;
		bool m_did_console_exist = false;
		bool m_is_console_open = false;

		void (logger::*m_console_logger)(const LogMessagePtr msg) = &logger::format_console;

		std::string_view m_console_title;
		DWORD m_original_console_mode = 0;
		HANDLE m_console_handle;

		std::ofstream m_console_out;
		std::ofstream m_file_out;
		std::vector<LogMessagePtr> m_log_messages;

		file m_file;

	public:
		logger() = default;
		virtual ~logger() = default;

		void initialize(const std::string_view console_title, file file, bool attach_console = true);
		void destroy();

		void toggle_external_console(bool toggle);
		std::vector<LogMessagePtr> get_log_messages() { return m_log_messages; }
		void clear_log_messages();
	private:
		void create_backup();

		void format_console(const LogMessagePtr msg);
		void format_console_simple(const LogMessagePtr msg);
		void format_file(const LogMessagePtr msg);
		void format_log(const LogMessagePtr msg);

	};

	inline logger g_log{};
}

#define ADD_COLOR_TO_STREAM(color) "\x1b[" << int(color) << "m"
#define RESET_STREAM_COLOR "\x1b[" << int(LogColor::RESET) << "m"
#define HEX_TO_UPPER(value) "0x" << std::hex << std::uppercase << (DWORD64)value << std::dec << std::nouppercase